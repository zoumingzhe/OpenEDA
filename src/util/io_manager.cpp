/* @file  io_manager.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include <vector>

#include "util/io_manager.h"
#include "util/message.h"
#include "util/enums.h"
#include "util/util.h"

namespace open_edi {
namespace util {

/// @brief IOManager Constructor of IOManager
IOManager::IOManager() {
    fp_        = nullptr;
    gzfp_      = nullptr;
    lz4fp_     = nullptr;
    zip_archive_  = nullptr;
    zip_file_     = nullptr;
    io_buffer_ = nullptr;
    compress_type_ = kCompressNull;
    compress_level_ = kCompressLevelInvalid;
}

/// @brief ~IOManager Destructor of IOManager
IOManager::~IOManager() {
    if (gzfp_) {
        gzclose(gzfp_);
        gzfp_ = nullptr;
    }
    if (lz4fp_) {
        lz4fp_->close();
        delete lz4fp_;
        lz4fp_ = nullptr;
    }
    if (zip_file_) {
        zip_fclose(zip_file_);
        zip_file_ = nullptr;
    }
    if (zip_archive_) {
        zip_close(zip_archive_);
        zip_archive_ = nullptr;
    }
    if (fp_) {
        fclose(fp_);
        fp_ = nullptr;
    }
    if (io_buffer_) {
        delete io_buffer_;
        io_buffer_ = nullptr;
    }
}

/// @brief open Open file specified by file_name with mode.
///
/// @param file_name
/// @param mode
///
/// @return
bool IOManager::open(const char *file_name, const char *mode) {
    if (nullptr == file_name) {
        message->issueMsg("UTIL", 4, kError);
        return false;
    }
    if (nullptr == mode) {
        message->issueMsg("UTIL", 5, kError);
        return false;
    }

    int name_len = strlen(file_name);
    char *copy_file_name = new char[name_len + 1]();
    strncpy(copy_file_name, file_name, name_len);
    toLower(copy_file_name);

    int mode_len = strlen(mode);
    char *copy_mode = new char[mode_len + 1]();
    strncpy(copy_mode, mode, mode_len);
    toLower(copy_mode);

    if (name_len > 3 && (0 == strcmp(".gz", &copy_file_name[name_len - 3]))) {
        gzfp_ = gzopen(file_name, mode);
        if (nullptr == gzfp_) {
            message->issueMsg("UTIL", 6, kError, file_name, mode);
            delete [] copy_file_name;
            delete [] copy_mode;
            return false;
        }
        if (kCompressLevelInvalid == compress_level_) {
            compress_level_ = kCompressLevel6;
        }
        gzsetparams(gzfp_, compress_level_, Z_DEFAULT_STRATEGY);
        compress_type_ = kCompressGz;
    } else if (name_len > 4 &&
               (0 == strcmp(".lz4", &copy_file_name[name_len - 4]))) {
        lz4fp_ = new Lz4(file_name, mode, compress_level_);
        if (false == lz4fp_->isOpen()) {
            return false;
        }
        if (kCompressLevelInvalid == compress_level_) {
            compress_level_ = kCompressLevelMin;
        }
        compress_type_ = kCompressLz4;
    } else if (name_len > 4 &&
               (0 == strcmp(".zip", &copy_file_name[name_len - 4]))) {
        int zip_error;
        if ('r' == copy_mode[0]) {
            zip_archive_ = zip_open(file_name, ZIP_CREATE, &zip_error);
            if (nullptr == zip_archive_) {
                message->issueMsg("UTIL", 6, kError, file_name, mode,
                                              zip_strerror(zip_archive_));
                delete [] copy_file_name;
                delete [] copy_mode;
                return false;
            }

            int file_count = zip_get_num_files(zip_archive_);
            if (file_count > 1) {
                message->issueMsg("UTIL", 17, kError);
                zip_close(zip_archive_);
                delete [] copy_file_name;
                delete [] copy_mode;
                return false;
            }
            zip_file_ = zip_fopen_index(zip_archive_, 0, 0);
            if (nullptr == zip_file_) {
                message->issueMsg("UTIL", 6, kError, file_name, mode);
                zip_close(zip_archive_);
                delete [] copy_file_name;
                delete [] copy_mode;
                return false;
            }
        } else if ('w' == copy_mode[0]) {
            message->issueMsg("UTIL", 11, kError);
            delete [] copy_file_name;
            delete [] copy_mode;
            return false;
        }
        compress_type_ = kCompressZip;
    } else {  // General file format
        fp_ = fopen(file_name, mode);
        if (nullptr == fp_) {
            message->issueMsg("UTIL", 6, kError, file_name, mode);
            delete [] copy_file_name;
            delete [] copy_mode;
            return false;
        }
        compress_type_ = kCompressNull;
    }
    delete [] copy_file_name;
    delete [] copy_mode;
    return true;
}

/// @brief open Open file specified by file_name with mode and compress level.
///
/// @param file_name
/// @param mode
/// @param level
///
/// @return
bool IOManager::open(const char *file_name, const char *mode,
                                                       CompressLevel level) {
    if (level < kCompressLevelMin && level > kCompressLevelMax) {
        message->issueMsg("UTIL", 8, kError, level, kCompressLevelMin,
                                                           kCompressLevelMax);
        return false;
    }
    compress_level_ = level;
    if (false == open(file_name, mode)) {
        return false;
    }
    if (compress_type_ == kCompressNull) {
        message->issueMsg("UTIL", 7, kWarn, file_name, mode, level);
    }
    return true;
}

/// @brief read Read data to buffer no more than size bytes.
///
/// @param buffer
/// @param size
///
/// @return
int IOManager::read(void *buffer, uint32_t size) {
    int read_result = kReadFail;

    if (nullptr == buffer) {
        message->issueMsg("UTIL", 12, kError);
        return kReadFail;
    }
    switch (compress_type_) {
        case kCompressNull:
            read_result = fread(buffer, 1, size, fp_);
            break;
        case kCompressLz4:
            read_result = lz4fp_->read(buffer, size);
            break;
        case kCompressGz:
            read_result = gzread(gzfp_, buffer, size);
            break;
        case kCompressZip:
            read_result = zip_fread(zip_file_, buffer, size);
            break;
        default:
            // should not in the case.
            return kReadFail;
    }
    return read_result;
}

/// @brief read Read data no more than size bytes. The return IOBuffer will
/// be overwritten in next calling this function.
/// @param size
///
/// @return
IOBuffer *IOManager::read(uint32_t size) {
    if (nullptr == io_buffer_) {
        if (size < kDefaultSize) {
            io_buffer_ = new IOBuffer(kDefaultSize);
        } else {
            io_buffer_ = new IOBuffer(size);
        }
    } else if (size > io_buffer_->getSize()) {
        delete io_buffer_;
        io_buffer_ = new IOBuffer(size);
    }
    int read_result = read(io_buffer_->getBuffer(), size);
    if (read_result <= 0) {
        return nullptr;
    }
    return io_buffer_;
}

/// @brief readCompressBlock Read compress blocks from file
/// the compress blocks must be written by IOManager.
///
/// @return
CompressBlock *IOManager::readCompressBlock() {
    uint32_t total_number = 0;
    uint32_t max_decompressed_size = 0;
    int read_result = kReadFail;

    CompressManager *cm = new CompressManager(compress_type_, this);
    return cm->decompress();
}

/// @brief write Write data in buffer to file.
///
/// @param buffer
/// @param size
///
/// @return
int IOManager::write(void *buffer, uint32_t size) {
    int write_result = kWriteFail;

    if (nullptr == buffer) {
        message->issueMsg("UTIL", 12, kError);
        return kWriteFail;
    }
    switch (compress_type_) {
        case kCompressNull:
            write_result = fwrite(buffer, 1, size, fp_);
            break;
        case kCompressLz4:
            write_result = lz4fp_->write(buffer, size);
            break;
        case kCompressGz:
            write_result = gzwrite(gzfp_, buffer, size);
            break;
        case kCompressZip:
            message->issueMsg("UTIL", 25, kError);
            break;
        default:
            // should not in the case.
            break;
    }
    return write_result;
}

/// @brief write Write data in IO buffer to file.
///
/// @param io_buffer
///
/// @return
int IOManager::write(IOBuffer *io_buffer) {
    int write_result = kWriteFail;

    if (nullptr == io_buffer) {
        message->issueMsg("UTIL", 9, kError);
        return kWriteFail;
    }
    char *buffer = io_buffer->getBuffer();
    if (nullptr == buffer) {
        message->issueMsg("UTIL", 10, kError);
        return kWriteFail;
    }
    write_result = write(buffer, io_buffer->getSize());

    return write_result;
}

/// @brief write Write string to file.
///
/// @param str
///
/// @return
int IOManager::write(std::string str) {
    uint32_t size = str.length();
    IOBuffer *io_buffer = new IOBuffer(size);
    memcpy(io_buffer->getBuffer(), str.c_str(), size);
    int write_result = write(io_buffer);
    delete io_buffer;

    return write_result;
}

/// @brief writeCompressBlock Write compress blocks with compress manager
///
/// @param compress_block
///
/// @return
int IOManager::writeCompressBlock(CompressBlock &compress_block) {
    CompressManager *cm = new CompressManager(compress_type_, this);
    int write_result = cm->compress(compress_block);
    return write_result;
}

/// @brief seek Seek the position of offset from origin
///
/// @param offset
/// @param origin
///
/// @return
int IOManager::seek(int64_t offset, int origin) {
    int result = 0;
    switch (compress_type_) {
        case kCompressNull:
            result = fseek(fp_, offset, origin);
            break;
        case kCompressLz4:
            result = lz4fp_->seek(offset, origin);
            break;
        case kCompressGz:
            result = gzseek(gzfp_, offset, origin);
            break;
        case kCompressZip:
            message->issueMsg("UTIL", 19, kError);
            break;
        default:
            // should not in the case.
            break;
    }
    return result;
}

/// @brief flush Flush the data in buffer to file.
void IOManager::flush() {
    switch (compress_type_) {
        case kCompressNull:
            if (fp_) {
                fflush(fp_);
            }
            break;
        case kCompressLz4:
            if (lz4fp_) {
                lz4fp_->flush();
            }
            break;
        case kCompressGz:
            if (gzfp_) {
                gzflush(gzfp_, Z_FULL_FLUSH);
            }
            break;
        case kCompressZip:
            message->issueMsg("UTIL", 20, kError);
            break;
        default:
            // should not in the case.
            break;
    }
}

/// @brief tell Tell the current position.
///
/// @return
int64_t IOManager::tell() {
    int64_t result = -1;
    switch (compress_type_) {
        case kCompressNull:
            result = ftell(fp_);
            break;
        case kCompressLz4:
            result = lz4fp_->tell();
            break;
        case kCompressGz:
            result = gztell(gzfp_);
            break;
        case kCompressZip:
            message->issueMsg("UTIL", 21, kError);
            break;
        default:
            // should not in the case.
            break;
    }
    return result;
}

/// @brief close Close current file.
void IOManager::close() {
    switch (compress_type_) {
        case kCompressNull:
            if (fp_) {
                fclose(fp_);
                fp_ = nullptr;
            }
            break;
        case kCompressLz4:
            if (lz4fp_) {
                lz4fp_->close();
                delete lz4fp_;
                lz4fp_ = nullptr;
            }
            break;
        case kCompressGz:
            if (gzfp_) {
                gzclose(gzfp_);
                gzfp_ = nullptr;
            }
            break;
        case kCompressZip:
            if (zip_file_) {
                zip_fclose(zip_file_);
                zip_file_ = nullptr;
            }
            break;
        default:
            // should not in the case.
            break;
    }
}

/// @brief CompressManager Constuctor of CompressManager
///
/// @param compress_type
/// @param im
CompressManager::CompressManager(CompressType compress_type, IOManager *im) {
    compress_type_ = compress_type;
    io_manager_ = im;
}

/// @brief freeIOBuffers Free io_buffers
///
/// @param io_buffers
void CompressManager::freeIOBuffers(std::vector<IOBuffer*> &io_buffers) {
    for (auto &io_buffer : io_buffers) {
        delete io_buffer;
    }
}

/// @brief compress Compress blocks in parallel with map reducer.
///
/// @param compress_block
///
/// @return
bool CompressManager::compress(CompressBlock &compress_block) {
    int i = 0;
    uint32_t total_number = compress_block.getTotalNumber();
    uint32_t max_buffer_size = compress_block.getMaxBufferSize();
    uint32_t num_thread = calcThreadNumber(total_number);
    std::vector<IOBuffer*> copy_src_buffers;
    std::vector<IOBuffer*> *src_buffers = compress_block.getIOBuffers();
    std::vector<IOBuffer*> dst_buffers;
    size_t dst_size = LZ4F_compressFrameBound(max_buffer_size, NULL);
    for (int j = 0; j < num_thread; ++j) {
        IOBuffer *new_buffer = new IOBuffer(dst_size);
        dst_buffers.push_back(new_buffer);
    }

    io_manager_->write(reinterpret_cast<void *>(&total_number),
                       sizeof(total_number));
    io_manager_->write(reinterpret_cast<void *>(&max_buffer_size),
                       sizeof(max_buffer_size));

    std::vector<int> compressed_sizes;
    compressed_sizes.resize(num_thread, 0);

    Compressor compressor;
    compressor.setCompressType(kCompressLz4);

    for (i = 0; i + num_thread < total_number; i += num_thread) {
        for (int j = 0; j < num_thread; ++j) {
            copy_src_buffers.push_back((*src_buffers)[i + j]);
        }
        CompressInput input(&copy_src_buffers, &dst_buffers, &compressed_sizes);
        compressor.setInput(&input);
        compressor.run(1, num_thread, 1);
        for (int k = 0; k < num_thread; ++k) {
            if (compressed_sizes[k] > 0) {
                io_manager_->write(reinterpret_cast<void*>(
                            &compressed_sizes[k]), sizeof(int));
                io_manager_->write(reinterpret_cast<void*>(
                            dst_buffers[k]->getBuffer()), compressed_sizes[k]);
            } else {
                message->issueMsg("UTIL", 15, kError, compressed_sizes[k]);
                freeIOBuffers(dst_buffers);
                dst_buffers.clear();
                return false;
            }
        }
        copy_src_buffers.clear();
        compressed_sizes.resize(num_thread, 0);
    }
    int num_last_buffers = total_number - i;
    for (int j = 0; j < num_last_buffers; ++j) {
        copy_src_buffers.push_back((*src_buffers)[i + j]);
    }
    CompressInput input(&copy_src_buffers, &dst_buffers, &compressed_sizes);
    compressor.setInput(&input);
    compressor.run(1, num_last_buffers, 1);
    for (int k = 0; k < num_last_buffers; ++k) {
        if (compressed_sizes[k] > 0) {
            io_manager_->write(reinterpret_cast<void*>(&compressed_sizes[k]),
                               sizeof(int));
            io_manager_->write(reinterpret_cast<void*>(
                            dst_buffers[k]->getBuffer()), compressed_sizes[k]);
        } else {
            message->issueMsg("UTIL", 15, kError, compressed_sizes[k]);
            freeIOBuffers(dst_buffers);
            dst_buffers.clear();
            return false;
        }
    }
    freeIOBuffers(dst_buffers);
    dst_buffers.clear();
    return true;
}

/// @brief decompress Decompress blocks in parallel with map reducer.
///
/// @param compress_block
///
/// @return
bool CompressManager::decompress(CompressBlock & compress_block) {
    std::vector<IOBuffer*> *io_buffers = compress_block.getIOBuffers();
    if (nullptr == io_buffers) {
        message->issueMsg("UTIL", 9, kError);
        return false;
    }
    int read_result = 0;
    uint32_t total_number = 0;
    uint32_t max_buffer_size = 0;

    read_result = io_manager_->read(reinterpret_cast<void*>(&total_number),
                                    sizeof(total_number));
    if (read_result <= 0) {
        message->issueMsg("UTIL", 13, kError);
        return false;
    }
    read_result = io_manager_->read(reinterpret_cast<void*>(&max_buffer_size),
                                    sizeof(max_buffer_size));
    if (read_result <= 0) {
        message->issueMsg("UTIL", 14, kError);
        return false;
    }

    int num_thread = calcThreadNumber(total_number);
    std::vector<IOBuffer*> src_buffers;
    std::vector<IOBuffer*> dst_buffers;
    for (int j = 0; j < num_thread; ++j) {
        IOBuffer *new_buffer = new IOBuffer(max_buffer_size);
        src_buffers.push_back(new_buffer);
    }

    std::vector<int> decompressed_sizes;
    decompressed_sizes.resize(num_thread, 0);

    Decompressor decompressor;
    decompressor.setDecompressType(kCompressLz4);

    int i = 0;
    int size = 0;
    IOBuffer *io_buffer = nullptr;
    for (i = 0; i + num_thread < total_number; i += num_thread) {
        for (int j = 0; j < num_thread; ++j) {
            io_buffer = src_buffers[j];
            io_manager_->read(reinterpret_cast<void*>(&size), sizeof(int));
            io_manager_->read(reinterpret_cast<void*>(io_buffer->getBuffer()),
                              size);
            io_buffer->setSize(size);

            dst_buffers.push_back((*io_buffers)[i + j]);
        }
        DecompressInput input(&src_buffers, &dst_buffers, &decompressed_sizes);
        decompressor.setInput(&input);
        decompressor.run(1, num_thread, 1);
        for (int k = 0; k < num_thread; ++k) {
            if (decompressed_sizes[k] < 0) {
                message->issueMsg("UTIL", 16, kError, decompressed_sizes[k]);
                freeIOBuffers(src_buffers);
                return false;
            }
            dst_buffers[k]->setSize(decompressed_sizes[k]);
        }
        decompressed_sizes.resize(num_thread, 0);
        dst_buffers.clear();
    }
    int num_last_buffers = total_number - i;
    for (int j = 0; j < num_last_buffers; ++j) {
        int size;
        IOBuffer *io_buffer = src_buffers[j];
        io_manager_->read(reinterpret_cast<void*>(&size), sizeof(int));
        io_manager_->read(reinterpret_cast<void*>(io_buffer->getBuffer()),
                          size);
        io_buffer->setSize(size);

        dst_buffers.push_back((*io_buffers)[i + j]);
    }
    DecompressInput input(&src_buffers, &dst_buffers, &decompressed_sizes);
    decompressor.setInput(&input);
    decompressor.run(1, num_last_buffers, 1);
    for (int k = 0; k < num_last_buffers; ++k) {
        if (decompressed_sizes[k] < 0) {
            message->issueMsg("UTIL", 16, kError, decompressed_sizes[k]);
            freeIOBuffers(src_buffers);
            return false;
        }
    }

    compress_block.setTotalNumber(total_number);
    compress_block.setMaxBufferSize(max_buffer_size);

    freeIOBuffers(src_buffers);
    return true;
}

/// @brief decompress Decompress blocks in parallel with map reducer.
///
/// @returnCompressBlock pointer, should be delete by caller
CompressBlock *CompressManager::decompress() {
    int read_result = 0;
    uint32_t total_number = 0;
    uint32_t max_buffer_size = 0;

    read_result = io_manager_->read(reinterpret_cast<void*>(&total_number),
                                    sizeof(uint32_t));
    if (read_result <= 0) {
        message->issueMsg("UTIL", 13, kError);
        return nullptr;
    }
    read_result = io_manager_->read(reinterpret_cast<void*>(&max_buffer_size),
                                    sizeof(uint32_t));
    if (read_result <= 0) {
        message->issueMsg("UTIL", 14, kError);
        return nullptr;
    }

    std::vector<IOBuffer*> *io_buffers = new std::vector<IOBuffer*>;

    for (uint32_t i = 0; i < total_number; ++i) {
        IOBuffer *new_buffer = new IOBuffer(max_buffer_size);
        io_buffers->push_back(new_buffer);
    }

    CompressBlock *compress_block = new CompressBlock(io_buffers);
    compress_block->setTotalNumber(total_number);
    compress_block->setMaxBufferSize(max_buffer_size);

    if (decompress(*compress_block)) {
        delete compress_block;
        freeIOBuffers(*io_buffers);
        delete io_buffers;
        return nullptr;
    }

    return compress_block;
}

/// @brief ~Compressor Destructor of Compressor
Compressor::~Compressor() {
}

/// @brief preRun Inherited from map reducer.
void Compressor::preRun() {
}

/// @brief postRun Inherited from map reducer.
void Compressor::postRun() {
}

/// @brief runMapper Create compress task.
///
/// @return
void* Compressor::runMapper() {
    CompressInput* input = reinterpret_cast<CompressInput*>(input_);
    std::vector<IOBuffer*> *src_buffers = input_->getSrcBuffers();
    std::vector<IOBuffer*> *dst_buffers = input_->getDstBuffers();
    std::vector<int>       *compressed_sizes = input->getCompressedSizes();

    for (int i = 0; i < src_buffers->size(); ++i) {
        CompressTask* task = new CompressTask((*src_buffers)[i],
                                              (*dst_buffers)[i],
                                              &(*compressed_sizes)[i]);
        task_queue_.push(task);
    }
    return nullptr;
}

/// @brief runWorker Compress in parallel.
///
/// @return
void* Compressor::runWorker() {
    while (true) {
        CompressTask* task = static_cast<CompressTask*>(task_queue_.pop());
        if (!task) {
            break;
        }
        IOBuffer *src_buffer = task->getSrcBuffer();
        IOBuffer *dst_buffer = task->getDstBuffer();

        switch (compress_type_) {
            case kCompressLz4:
                task->setCompressedSize(
                        LZ4_compress_default(
                            reinterpret_cast<char *>(src_buffer->getBuffer()),
                            reinterpret_cast<char *>(dst_buffer->getBuffer()),
                            src_buffer->getSize(),
                            dst_buffer->getSize()));
                break;
            case kCompressZip: {
                int32_t compress_size = dst_buffer->getSize();
                if (!dst_buffer) {
                    message->issueMsg("UTIL", 18, kError);
                    return nullptr;
                }
                int result = Z_OK;
                result = compress(
                            reinterpret_cast<Bytef*>(dst_buffer->getBuffer()),
                            reinterpret_cast<uLongf*>(&compress_size),
                            (const Bytef *)(src_buffer->getBuffer()),
                            (uLong)src_buffer->getSize());
                if (Z_OK != result) {
                    message->issueMsg("UTIL", 27, kError, result);
                    return nullptr;
                }
                task->setCompressedSize(compress_size);
                }
                break;
            default:
                break;
        }

        delete task;
    }

    return nullptr;
}

/// @brief runReducer Inherited from map reducer.
///
/// @return
void* Compressor::runReducer() {
    return nullptr;
}

/// @brief runMapper Create decompress task for map reducer
///
/// @return
void* Decompressor::runMapper() {
    DecompressInput* input = reinterpret_cast<DecompressInput*>(input_);
    std::vector<IOBuffer*> *src_buffers = input_->getSrcBuffer();
    std::vector<IOBuffer*> *dst_buffers = input_->getDstBuffer();
    std::vector<int>       *decompressed_sizes = input->getDecompressedSizes();

    for (int i = 0; i < src_buffers->size(); ++i) {
        DecompressTask* task = new DecompressTask((*src_buffers)[i],
                                                  (*dst_buffers)[i],
                                                  &(*decompressed_sizes)[i]);
        task_queue_.push(task);
    }
    return nullptr;
}

/// @brief runWorker Decompress tasks in task queue until the queue is null.
///
/// @return
void* Decompressor::runWorker() {
    while (true) {
        DecompressTask* task = static_cast<DecompressTask*>(task_queue_.pop());
        if (!task) {
            break;
        }
        IOBuffer *src_buffer = task->getSrcBuffer();
        IOBuffer *dst_buffer = task->getDstBuffer();

        switch (decompress_type_) {
            case kCompressLz4:
                task->setDecompressedSize(
                        LZ4_decompress_safe(
                             reinterpret_cast<char *>(src_buffer->getBuffer()),
                             reinterpret_cast<char *>(dst_buffer->getBuffer()),
                             src_buffer->getSize(),
                             dst_buffer->getSize()));
                break;
            case kCompressZip: {
                int32_t decompress_size = dst_buffer->getSize();
                if (!dst_buffer) {
                    message->issueMsg("UTIL", 18, kError);
                    return nullptr;
                }
                int result = Z_OK;
                result = uncompress(
                        reinterpret_cast<Bytef*>(dst_buffer->getBuffer()),
                        reinterpret_cast<uLongf*>(&decompress_size),
                        (const Bytef *)(src_buffer->getBuffer()),
                        (uLong)src_buffer->getSize());
                if (Z_OK != result) {
                    message->issueMsg("UTIL", 27, kError, result);
                    return nullptr;
                }
                task->setDecompressedSize(decompress_size);
                }
            default:
                break;
        }

        delete task;
    }

    return nullptr;
}


/// @brief runReducer Define reducer for pure virtual function
///
/// @return
void* Decompressor::runReducer() {
    return nullptr;
}

/// @brief Lz4 Costructor of Lz4
Lz4::Lz4() {
    fp_ = nullptr;
    decom_context_ = nullptr;
    com_context_ = nullptr;
    src_buffer_size_ = kDefaultSize;
    src_buffer_ = malloc(src_buffer_size_);
    if (!src_buffer_) {
        message->issueMsg("UTRIL", 23, kError);
        return;
    }
    dst_buffer_size_ = LZ4F_compressFrameBound(src_buffer_size_, NULL);
    dst_buffer_ = malloc(dst_buffer_size_);
    if (!dst_buffer_) {
        message->issueMsg("UTRIL", 24, kError);
        return;
    }
    next_to_load_ = 0;
    src_filled_size_ = 0;
    src_done_size_ = 0;
    dst_filled_size_ = 0;
    dst_done_size_ = 0;
    decom_context_ = nullptr;
    com_context_ = nullptr;
}

/// @brief Lz4 Constructor with file name, mode and compress level.
///
/// @param file_name
/// @param mode
/// @param compress_level
Lz4::Lz4(const char * file_name, const char * mode,
                                                CompressLevel compress_level) {
    fp_ = fopen(file_name, mode);
    if (nullptr == fp_) {
        message->issueMsg("UTIL", 6, kError, file_name, mode);
        return;
    }
    src_buffer_size_ = kDefaultSize;
    src_buffer_ = malloc(src_buffer_size_);
    if (!src_buffer_) {
        message->issueMsg("UTRIL", 23, kError);
        return;
    }
    dst_buffer_size_ = LZ4F_compressFrameBound(src_buffer_size_, NULL);
    dst_buffer_ = malloc(dst_buffer_size_);
    if (!dst_buffer_) {
        message->issueMsg("UTRIL", 24, kError);
        return;
    }
    next_to_load_ = 0;
    src_filled_size_ = 0;
    src_done_size_ = 0;
    dst_filled_size_ = 0;
    dst_done_size_ = 0;
    decom_context_ = nullptr;
    com_context_ = nullptr;
    if ((0 == strcmp("r", mode)) ||
        (0 == strcmp("rb", mode)) ||
        (0 == strcmp("rx", mode))) {
        mode_type_ = kRead;
        LZ4F_errorCode_t error_code = LZ4F_createDecompressionContext(
                                                &decom_context_, LZ4F_VERSION);
        if (LZ4F_isError(error_code)) {
            message->issueMsg("UTIL", 32, kError,
                                                LZ4F_getErrorName(error_code));
            return;
        }
        read_header();
    } else if ((0 == strcmp("w", mode)) ||
               (0 == strcmp("wb", mode)) ||
               (0 == strcmp("wx", mode))) {
        mode_type_ = kWrite;
        memset(&prefs_, 0, sizeof(prefs_));
        prefs_.autoFlush = 1;
        prefs_.compressionLevel = compress_level;
        prefs_.frameInfo.blockMode = LZ4F_blockIndependent;
        prefs_.frameInfo.blockSizeID = LZ4F_max4MB;
        prefs_.frameInfo.blockChecksumFlag = LZ4F_noBlockChecksum;
        prefs_.frameInfo.contentChecksumFlag = LZ4F_contentChecksumEnabled;

        LZ4F_errorCode_t error_code = LZ4F_createCompressionContext(
                                                  &com_context_, LZ4F_VERSION);
        if (LZ4F_isError(error_code)) {
            message->issueMsg("UTIL", 33, kError,
                                                LZ4F_getErrorName(error_code));
            return;
        }

        write_header();
    }
}

/// @brief ~Lz4 Destructor of Lz4.
Lz4::~Lz4() {
    if (kRead == mode_type_) {
        LZ4F_freeDecompressionContext(decom_context_);
        decom_context_ = nullptr;
    } else if (kWrite == mode_type_) {
        if (isOpen()) {
            close();
        }
        LZ4F_freeCompressionContext(com_context_);
        com_context_ = nullptr;
    }
    if (fp_) {
        fclose(fp_);
        fp_ = nullptr;
    }
    if (!src_buffer_) {
        free(src_buffer_);
        src_buffer_ = nullptr;
    }
    if (!dst_buffer_) {
        free(dst_buffer_);
        dst_buffer_ = nullptr;
    }
}

/// @brief write_header Write Lz4 header.
///
/// @return
int Lz4::write_header() {
    int headerSize = LZ4F_compressBegin(com_context_, dst_buffer_,
                                                dst_buffer_size_, &prefs_);
    return fwrite(dst_buffer_, 1, headerSize, fp_);
}

/// @brief write Write data in buffer to file, no more than size bytes.
///
/// @param buffer
/// @param size
///
/// @return
int Lz4::write(void *buffer, int size) {
    int write_size = 0;
    while (size - write_size > src_buffer_size_ - src_filled_size_) {
        memcpy(reinterpret_cast<char*>(src_buffer_) + src_filled_size_,
               reinterpret_cast<char*>(buffer) + write_size,
               src_buffer_size_ - src_filled_size_);

        int compress_size = LZ4F_compressUpdate(com_context_,
                                      dst_buffer_, dst_buffer_size_,
                                      src_buffer_, src_buffer_size_, NULL);
        if (LZ4F_isError(compress_size)) {
            message->issueMsg("UTIL", 30, kError,
                                             LZ4F_getErrorName(compress_size));
            return kWriteFail;
        }
        fwrite(dst_buffer_, 1, compress_size, fp_);
        write_size += src_buffer_size_ - src_filled_size_;
        src_filled_size_ = 0;
    }

    memcpy(reinterpret_cast<char*>(src_buffer_) + src_filled_size_,
           reinterpret_cast<char*>(buffer) + write_size,
           size - write_size);
    src_filled_size_ += size - write_size;
    write_size = size - write_size;
    return write_size;
}

/// @brief seek Seek offset from origin.
///
/// @param offset
/// @param origin
///
/// @return
int Lz4::seek(int64_t offset, int origin) {
    return fseek(fp_, offset, origin);
}

/// @brief flush Flush data in buffer to file.
///
/// @return
int Lz4::flush() {
    if (kWrite != mode_type_) {
        return 0;
    }
    int compress_size = LZ4F_compressUpdate(com_context_,
                                      dst_buffer_, dst_buffer_size_,
                                      src_buffer_, src_filled_size_, NULL);
    if (LZ4F_isError(compress_size)) {
        message->issueMsg("UTIL", 30, kError, LZ4F_getErrorName(compress_size));
        return kWriteFail;
    }
    fwrite(dst_buffer_, 1, compress_size, fp_);
    src_filled_size_ = 0;
    return fflush(fp_);
}

/// @brief tell Tell current position.
///
/// @return
int64_t Lz4::tell() {
    return ftell(fp_);
}

/// @brief close Close Lz4 file.
void Lz4::close() {
    if (nullptr == fp_) {
        return;
    }
    if (kWrite == mode_type_) {
        int compress_size = LZ4F_compressUpdate(com_context_,
                                      dst_buffer_, dst_buffer_size_,
                                      src_buffer_, src_filled_size_, NULL);
        if (LZ4F_isError(compress_size)) {
            message->issueMsg("UTIL", 30, kError,
                                            LZ4F_getErrorName(compress_size));
            return;
        }
        fwrite(dst_buffer_, 1, compress_size, fp_);
        src_filled_size_ = 0;
        int end_size = LZ4F_compressEnd(com_context_,
                                        dst_buffer_, dst_buffer_size_, NULL);
        if (LZ4F_isError(end_size)) {
            message->issueMsg("UTIL", 31, kError, LZ4F_getErrorName(end_size));
            return;
        }
        fwrite(dst_buffer_, 1, end_size, fp_);
    }
    fclose(fp_);
    fp_ = nullptr;
}

/*
| MagicNb | F. Descriptor | Block | (...) | EndMark | C. Checksum |
|:-------:|:-------------:| ----- | ----- | ------- | ----------- |
| 4 bytes |  3-15 bytes   |       |       | 4 bytes | 0-4 bytes   |
*/

/// @brief read_header Read Lz4 header and get size for next read.
///
/// @return
int Lz4::read_header() {
    char magic_number_buffer[kMagicNumberSize];
    uint32_t magic_number = 0;
    int number_read_bytes = fread(magic_number_buffer, 1,
                                                 kMagicNumberSize, fp_);
    if (kMagicNumberSize != number_read_bytes) {
        message->issueMsg("UTIL", 22, kError);
        return kReadFail;
    }

    magic_number = readLE32(magic_number_buffer);
    if (kLz4MagicNumber != magic_number) {
        message->issueMsg("UTIL", 24, kError, magic_number, kLz4MagicNumber);
        return kReadFail;
    }

    size_t dst_size = 0;
    size_t src_size = kMagicNumberSize;
    writeLE32(src_buffer_, kLz4MagicNumber);
    next_to_load_ = LZ4F_decompress(decom_context_,
                                    dst_buffer_, &dst_size,
                                    src_buffer_, &src_size, NULL);
    if (LZ4F_isError(next_to_load_)) {
        message->issueMsg("UTIL", 26, kError, LZ4F_getErrorName(next_to_load_));
        return kReadFail;
    }

    return kReadSuccess;
}

/// @brief read Read data to buffer from Lz4 file.
///
/// @param buffer
/// @param size
///
/// @return
int Lz4::read(void *buffer, int size) {
    if (!buffer) {
        message->issueMsg("UTIL", 29, kError);
        return kReadFail;
    }

    int copied_size = 0;
    int src_readed_size_ = 0;

    // dst buffer has enough data to read
    if (dst_filled_size_ - dst_done_size_ >= size) {
        memcpy(buffer, reinterpret_cast<char*>(dst_buffer_) + dst_done_size_,
                                                                 (size_t)size);
        dst_done_size_ += size;
        return size;
    }

    // dst buffer has not enough data to read
    copied_size = dst_filled_size_ - dst_done_size_;
    memcpy(buffer, reinterpret_cast<char*>(dst_buffer_) + dst_done_size_,
                                                                  copied_size);
    dst_done_size_ += copied_size;
    dst_filled_size_ = 0;

    while (copied_size < size) {
        // src buffer has not data
        if (src_done_size_ >= src_filled_size_) {
            src_filled_size_ = 0;
            src_done_size_ = 0;
            if (next_to_load_ > src_buffer_size_) {
                next_to_load_ = src_buffer_size_;
            }
            // read data from file
            src_filled_size_ = fread(src_buffer_, 1, next_to_load_, fp_);
            if (!src_filled_size_) {
                break;
            }
        }

        // decode src buffer in loop
        while (src_done_size_ < src_filled_size_) {
            size_t remaining = src_filled_size_ - src_done_size_;
            dst_filled_size_ = dst_buffer_size_;
            dst_done_size_ = 0;
            next_to_load_ = LZ4F_decompress(decom_context_,
                      dst_buffer_, reinterpret_cast<size_t*>(&dst_filled_size_),
                      reinterpret_cast<char*>(src_buffer_) + src_done_size_,
                      &remaining, NULL);
            if (LZ4F_isError(next_to_load_)) {
                message->issueMsg("UTIL", 26, kError,
                                        LZ4F_getErrorName(next_to_load_));
                return kReadFail;
            }
            src_done_size_ += remaining;

            // copy dst buffer to output-buffer
            if (dst_filled_size_ < size - copied_size) {
                memcpy(reinterpret_cast<char*>(buffer) + copied_size,
                       dst_buffer_, dst_filled_size_);
                copied_size += dst_filled_size_;
                dst_done_size_ = 0;
                dst_filled_size_ = 0;
            } else {
                memcpy(reinterpret_cast<char*>(buffer) + copied_size,
                       dst_buffer_, size - copied_size);
                dst_done_size_ += size - copied_size;
                copied_size += size - copied_size;
                break;
            }
            if (!next_to_load_) {
                break;
            }
        }
    }
    return copied_size;
}

/// @brief readLE32 Read little endian 4 bytes integer.
///
/// @param src
///
/// @return
uint32_t Lz4::readLE32(const void* src) {
    const char* const srcPtr = (const char*)src;
    uint32_t value32 = srcPtr[0];
    value32 += ((uint32_t)srcPtr[1]) <<  8;
    value32 += ((uint32_t)srcPtr[2]) << 16;
    value32 += ((uint32_t)srcPtr[3]) << 24;
    return value32;
}

/// @brief writeLE32 Write 4 bytes integer to buffer in little endian
///
/// @param dst
/// @param value32
void Lz4::writeLE32(void* dst, uint32_t value32) {
    char* const dstPtr = reinterpret_cast<char*>(dst);
    dstPtr[0] = static_cast<char>(value32);
    dstPtr[1] = static_cast<char>(value32 >> 8);
    dstPtr[2] = static_cast<char>(value32 >> 16);
    dstPtr[3] = static_cast<char>(value32 >> 24);
}

}  // namespace util
}  // namespace open_edi
