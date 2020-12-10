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
#include "util/io_manager.h"
#include "util/message.h"
#include "util/enums.h"
#include "util/util.h"

namespace open_edi {
namespace util {

IOManager::IOManager() {
    fp_ = nullptr;
    gzfp_ = nullptr;
    compress_type_ = kCompressNull;
    compress_level_ = kCompressLevelMin;
}

IOManager::~IOManager() {
    if (fp_) {
        fclose(fp_);
        fp_ = nullptr;
    }
    if (gzfp_) {
        gzclose(gzfp_);
        gzfp_ = nullptr;
    }
}

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

    if (name_len > 3 && (0 == strcmp(".gz", &copy_file_name[name_len - 3]))) {
        gzfp_ = gzopen(file_name, mode);
        if (nullptr == gzfp_) {
            message->issueMsg("UTIL", 6, kError, file_name, mode);
            delete [] copy_file_name;
            return false;
        }
        compress_type_ = kCompressGz;
    } else if (name_len > 4 &&
               (0 == strcmp(".lz4", &copy_file_name[name_len - 4]))) {
        fp_ = fopen(file_name, mode);
        if (nullptr == fp_) {
            message->issueMsg("UTIL", 6, kError, file_name, mode);
            delete [] copy_file_name;
            return false;
        }
        compress_type_ = kCompressLz4;
    } else if (name_len > 4 &&
               (0 == strcmp(".zip", &copy_file_name[name_len - 4]))) {
        // TODO(haoqingsheng)
        delete [] copy_file_name;
        return true;
    } else {
        fp_ = fopen(file_name, mode);
        if (nullptr == fp_) {
            message->issueMsg("UTIL", 6, kError, file_name, mode);
            delete [] copy_file_name;
            return false;
        }
        compress_type_ = kCompressNull;
    }
    delete [] copy_file_name;
    return true;
}

bool IOManager::open(const char *file_name, const char *mode,
                                                       CompressLevel level) {
    if (level < kCompressLevelMin && level > kCompressLevelMax) {
        message->issueMsg("UTIL", 8, kError, level, kCompressLevelMin,
                                                           kCompressLevelMax);
        return false;
    }
    if (false == open(file_name, mode)) {
        return false;
    }
    if (compress_type_ == kCompressNull) {
        message->issueMsg("UTIL", 7, kError, file_name, mode, level);
        return false;
    }
    compress_level_ = level;
    return true;
}

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
            break;
        case kCompressGz:
            read_result = gzread(gzfp_, buffer, size);
            break;
        case kCompressZip:
            break;
        default:
            // should not in the case.
            return kReadFail;
    }
    return read_result;
}

IOBuffer *IOManager::read(uint32_t size) {
    IOBuffer *io_buffer = new IOBuffer(size);
    if (nullptr == io_buffer) {
        message->issueMsg("UTIL", 11, kError, size);
        return nullptr;
    }
    int read_result = read(io_buffer->getBuffer(), size);
    if (read_result <= 0) {
        delete io_buffer;
    }
    return nullptr;
}

CompressBlock *IOManager::readCompressBlock() {
    uint32_t total_number = 0;
    uint32_t max_decompressed_size = 0;
    int read_result = kReadFail;

    CompressHandler *ch = new CompressHandler(compress_type_, this);
    return ch->decompress();
}

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
            break;
        case kCompressGz:
            write_result = gzwrite(gzfp_, buffer, size);
            break;
        case kCompressZip:
            break;
        default:
            // should not in the case.
            break;
    }
    return write_result;
}

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

int IOManager::write(std::string str) {
    uint32_t size = str.length();
    IOBuffer *io_buffer = new IOBuffer(size);
    int write_result = write(io_buffer);
    delete io_buffer;

    return write_result;
}

int IOManager::writeCompressBlock(CompressBlock &compress_block) {
    CompressHandler *ch = new CompressHandler(compress_type_, this);
    int write_result = ch->compress(compress_block);
    return write_result;
}

int IOManager::seek(long int offset, int origin) {
    int result = 0;
    switch (compress_type_) {
        case kCompressNull:
            result = fseek(fp_, offset, origin);
            break;
        case kCompressLz4:
            break;
        case kCompressGz:
            result = gzseek(gzfp_, offset, origin);
            break;
        case kCompressZip:
            break;
        default:
            // should not in the case.
            break;
    }
    return result;
}

void IOManager::flush() {
    switch (compress_type_) {
        case kCompressNull:
            fflush(fp_);
            break;
        case kCompressLz4:
            break;
        case kCompressGz:
            gzflush(gzfp_, Z_FULL_FLUSH);
            break;
        case kCompressZip:
            break;
        default:
            // should not in the case.
            break;
    }
}

int64_t IOManager::tell() {
    int64_t result = -1;
    switch (compress_type_) {
        case kCompressNull:
            result = ftell(fp_);
            break;
        case kCompressLz4:
            break;
        case kCompressGz:
            result = gztell(gzfp_);
            break;
        case kCompressZip:
            break;
        default:
            // should not in the case.
            break;
    }
    return result;
}

void IOManager::close() {
    switch (compress_type_) {
        case kCompressNull:
            if (fp_) {
                fclose(fp_);
                fp_ = nullptr;
            }
            break;
        case kCompressLz4:
            break;
        case kCompressGz:
            if (gzfp_) {
                gzclose(gzfp_);
                gzfp_ = nullptr;
            }
            break;
        case kCompressZip:
            break;
        default:
            // should not in the case.
            break;
    }
}

CompressHandler::CompressHandler(CompressType compress_type, IOManager *im) {
    compress_type_ = compress_type;
    io_manager_ = im;
}

void CompressHandler::freeBuffers(std::vector<IOBuffer*> *io_buffers) {
    for (auto io_buffer : *io_buffers) {
        delete io_buffer;
    }
}

bool CompressHandler::compress(CompressBlock &compress_block) {
    int i = 0;
    int total_number = compress_block.getTotalNumber();
    int num_thread = calcThreadNumber(total_number);
    std::vector<IOBuffer*> copy_src_buffers;
    std::vector<IOBuffer*> *src_buffers = compress_block.getIOBuffers();
    std::vector<IOBuffer*> dst_buffers;

    int max_buffer_size = compress_block.getMaxBufferSize();
    size_t dst_size = LZ4F_compressFrameBound(max_buffer_size, NULL);
    for (int j = 0; j < num_thread; ++j) {
        IOBuffer *new_buffer = new IOBuffer(dst_size);
        dst_buffers.push_back(new_buffer);
    }

    io_manager_->write((void *)&total_number, sizeof(uint32_t));
    io_manager_->write((void *)&max_buffer_size, sizeof(uint32_t));

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
                io_manager_->write((void*)&(compressed_sizes[k]), sizeof(uint32_t));
                io_manager_->write((void*)dst_buffers[k]->getBuffer(),
                                                          compressed_sizes[k]);
            } else {
                message->issueMsg("UTIL", 15, kError, compressed_sizes[k]);
                freeBuffers(&dst_buffers);
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
            io_manager_->write((void*)&(compressed_sizes[k]), sizeof(uint32_t));
            io_manager_->write((void*)(dst_buffers[k]->getBuffer()),
                                                      compressed_sizes[k]);
        } else {
            message->issueMsg("UTIL", 15, kError, compressed_sizes[k]);
            freeBuffers(&dst_buffers);
            dst_buffers.clear();
            return false;
        }
    }

    freeBuffers(&dst_buffers);
    dst_buffers.clear();
    return true;
}

bool CompressHandler::decompress(CompressBlock & compress_block) {
    std::vector<IOBuffer*> *io_buffers = compress_block.getIOBuffers();
    if (nullptr == io_buffers) {
        message->issueMsg("UTIL", 9, kError);
        return false;
    }
    int read_result = 0;
    uint32_t total_number = 0;
    uint32_t max_buffer_size = 0;

    read_result = io_manager_->read((void*)(&total_number), sizeof(uint32_t));
    if (read_result <= 0) {
        message->issueMsg("UTIL", 13, kError);
        return false;
    }
    read_result = io_manager_->read((void*)&max_buffer_size, sizeof(uint32_t));
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
    IOBuffer *mem_buffer = nullptr;
    for (i = 0; i + num_thread < total_number; i += num_thread) {
        for (int j = 0; j < num_thread; ++j) {
            mem_buffer = src_buffers[j];
            io_manager_->read((void *)&(size), sizeof(int));
            io_manager_->read((void *)(mem_buffer->getBuffer()), size);
            mem_buffer->setSize(size);

            dst_buffers.push_back((*io_buffers)[i + j]);
        }
        DecompressInput input(&src_buffers, &dst_buffers, &decompressed_sizes);
        decompressor.setInput(&input);
        decompressor.run(1, num_thread, 1);
        for (int k = 0; k < num_thread; ++k) {
            if (decompressed_sizes[k] < 0) {
                message->issueMsg("UTIL", 16, kError, decompressed_sizes[k]);
                freeBuffers(&src_buffers);
                freeBuffers(io_buffers);
                delete io_buffers;
                return false;
            }
        }
        decompressed_sizes.resize(num_thread, 0);
        dst_buffers.clear();
    }
    int num_last_buffers = total_number - i;
    for (int j = 0; j < num_last_buffers; ++j) {
        int size;
        IOBuffer *mem_buffer = src_buffers[j];
        io_manager_->read((char *)&(size), sizeof(int));
        io_manager_->read((char *)(mem_buffer->getBuffer()), size);
        mem_buffer->setSize(size);

        dst_buffers.push_back((*io_buffers)[i + j]);
    }
    DecompressInput input(&src_buffers, &dst_buffers, &decompressed_sizes);
    decompressor.setInput(&input);
    decompressor.run(1, num_last_buffers, 1);
    for (int k = 0; k < num_last_buffers; ++k) {
        if (decompressed_sizes[k] < 0) {
            message->issueMsg("UTIL", 16, kError, decompressed_sizes[k]);
            freeBuffers(&src_buffers);
            freeBuffers(io_buffers);
            delete io_buffers;
            return false;
        }
    }

    for (auto mem_buffer : src_buffers) {
        delete mem_buffer;
    }
    compress_block.setTotalNumber(total_number);
    compress_block.setMaxBufferSize(max_buffer_size);
    return true;
}

CompressBlock *CompressHandler::decompress() {
    int read_result = 0;
    uint32_t total_number = 0;
    uint32_t max_buffer_size = 0;

    read_result = io_manager_->read((void*)(&total_number), sizeof(uint32_t));
    if (read_result <= 0) {
        message->issueMsg("UTIL", 13, kError);
        return nullptr;
    }
    read_result = io_manager_->read((void*)&max_buffer_size, sizeof(uint32_t));
    if (read_result <= 0) {
        message->issueMsg("UTIL", 14, kError);
        return nullptr;
    }

    std::vector<IOBuffer*> *io_buffers = new std::vector<IOBuffer*>;

    for (uint32_t i = 0; i < total_number; ++i) {
        IOBuffer *new_buffer = new IOBuffer(max_buffer_size);
        io_buffers->push_back(new_buffer);
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
    IOBuffer *mem_buffer = nullptr;
    for (i = 0; i + num_thread < total_number; i += num_thread) {
        for (int j = 0; j < num_thread; ++j) {
            mem_buffer = src_buffers[j];
            io_manager_->read((void *)&(size), sizeof(int));
            io_manager_->read((void *)(mem_buffer->getBuffer()), size);
            mem_buffer->setSize(size);

            dst_buffers.push_back((*io_buffers)[i + j]);
        }
        DecompressInput input(&src_buffers, &dst_buffers, &decompressed_sizes);
        decompressor.setInput(&input);
        decompressor.run(1, num_thread, 1);
        for (int k = 0; k < num_thread; ++k) {
            if (decompressed_sizes[k] < 0) {
                message->issueMsg("UTIL", 16, kError, decompressed_sizes[k]);
                freeBuffers(&src_buffers);
                freeBuffers(io_buffers);
                delete io_buffers;
                return nullptr;
            }
        }
        decompressed_sizes.resize(num_thread, 0);
        dst_buffers.clear();
    }
    int num_last_buffers = total_number - i;
    for (int j = 0; j < num_last_buffers; ++j) {
        int size;
        IOBuffer *mem_buffer = src_buffers[j];
        io_manager_->read((char *)&(size), sizeof(int));
        io_manager_->read((char *)(mem_buffer->getBuffer()), size);
        mem_buffer->setSize(size);

        dst_buffers.push_back((*io_buffers)[i + j]);
    }
    DecompressInput input(&src_buffers, &dst_buffers, &decompressed_sizes);
    decompressor.setInput(&input);
    decompressor.run(1, num_last_buffers, 1);
    for (int k = 0; k < num_last_buffers; ++k) {
        if (decompressed_sizes[k] < 0) {
            message->issueMsg("UTIL", 16, kError, decompressed_sizes[k]);
            freeBuffers(&src_buffers);
            freeBuffers(io_buffers);
            delete io_buffers;
            return nullptr;
        }
    }

    for (auto mem_buffer : src_buffers) {
        delete mem_buffer;
    }
    CompressBlock *compress_block = new CompressBlock(io_buffers);
    compress_block->setTotalNumber(total_number);
    compress_block->setMaxBufferSize(max_buffer_size);
    return compress_block;
}


Compressor::~Compressor() {
}

void Compressor::preRun() {
}

void Compressor::postRun() {
}

void* Compressor::runMapper() {
    CompressInput* input = reinterpret_cast<CompressInput*>(input_);
    std::vector<IOBuffer*> *src_buffers = input_->getSrcChunks();
    std::vector<IOBuffer*> *dst_buffers = input_->getDstChunks();
    std::vector<int>       *compressed_sizes = input->getCompressedSizes();

    for (int i = 0; i < src_buffers->size(); ++i) {
        CompressTask* task = new CompressTask((*src_buffers)[i],(*dst_buffers)[i],
                                                       &(*compressed_sizes)[i]);
        task_queue_.push(task);
    }
    return nullptr;
}

void* Compressor::runWorker() {
    while (true) {
        CompressTask* task = static_cast<CompressTask*>(task_queue_.pop());
        if (!task) {
            break;
        }
        IOBuffer *src_buffer = task->getSrcChunk();
        IOBuffer *dst_buffer = task->getDstChunk();

        switch (compress_type_) {
            case kCompressLz4:
                task->setCompressedSize(LZ4_compress_default(
                                                (char *)src_buffer->getBuffer(),
                                                (char *)dst_buffer->getBuffer(),
                                                src_buffer->getSize(),
                                                dst_buffer->getSize()));
                break;
            default:
                break;
        }

        delete task;
    }

    return nullptr;
}

void* Compressor::runReducer() {
    return nullptr;
}

/// @brief runMapper Create decompress task for map reducer
///
/// @return 
void* Decompressor::runMapper() {
    DecompressInput* input = reinterpret_cast<DecompressInput*>(input_);
    std::vector<IOBuffer*> *src_buffers = input_->getSrcChunks();
    std::vector<IOBuffer*> *dst_buffers = input_->getDstChunks();
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
        IOBuffer *src_buffer = task->getSrcChunk();
        IOBuffer *dst_buffer = task->getDstChunk();

        switch (decompress_type_) {
            case kCompressLz4:
                task->setDecompressedSize(LZ4_decompress_safe(
                                                (char *)src_buffer->getBuffer(),
                                                (char *)dst_buffer->getBuffer(),
                                                src_buffer->getSize(),
                                                dst_buffer->getSize()));
                break;
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

}  // util
}  // open_edi
