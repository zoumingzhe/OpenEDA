/* @file  io_manager.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_UTIL_IO_HANDLER_H_
#define SRC_UTIL_IO_HANDLER_H_

#include <stdint.h>

#include <string>

#include "zlib.h"
#include "lz4.h"
#include "lz4frame.h"

#include "util/map_reduce.h"
#include "util/message.h"

namespace open_edi {
namespace util {

const uint32_t kDefaultSize = 1024;
const uint8_t  kEOF = 0;
const uint8_t  kReadFail = -1;
const uint8_t  kWriteFail = -1;

class IOBuffer;
class CompressBlock;

enum CompressType {
    kCompressNull,
    kCompressLz4,
    kCompressGz,
    kCompressZip
};

enum CompressLevel {
    kCompressLevelMin = 1, // default of LZ4
    kCompressLevel2,
    kCompressLevel3,
    kCompressLevel4,
    kCompressLevel5,
    kCompressLevel6, // default of GZIP
    kCompressLevel7,
    kCompressLevel8,
    kCompressLevel9,
    kCompressLevel10,
    kCompressLevel11,
    kCompressLevel12,
    kCompressLevel13,
    kCompressLevel14,
    kCompressLevel15,
    kCompressLevelMax,
};

class IOManager {
  public:
    IOManager();
    ~IOManager();
    bool open(const char *file_name, const char *mode);
    bool open(const char *file_name, const char *mode, CompressLevel level);

    int read(void *buffer, uint32_t size);
    // The buffer will be reused when read(uint32_t size) is called next time.
    IOBuffer *read(uint32_t size = kDefaultSize);
    // The buffers should be freed by applications.
    CompressBlock *readCompressBlock();
    // comprees_block is allocated by caller
    bool readCompressBlock(CompressBlock &compress_block);

    int write(IOBuffer *buffer);
    int write(void *buffer, uint32_t size);
    int write(std::string);
    int writeCompressBlock(CompressBlock &compress_block);

    int seek(long int offset, int origin);
    void flush();
    int64_t tell();
    void close();

  private:
    CompressType compress_type_;
    CompressLevel compress_level_;

    FILE *fp_;
    gzFile gzfp_;
};

class IOBuffer {
  public:
    IOBuffer() { buffer_ = nullptr; }
    IOBuffer(uint32_t size) {
        size_ = size;
        buffer_ = new char[size]();
    }
    ~IOBuffer() {
        delete buffer_; 
        buffer_ = nullptr;
    }
    void setBuffer(char *buffer) { buffer_ = buffer; }
    char *getBuffer() {return buffer_;}
    void setSize(uint32_t size) { size_ = size; }
    uint32_t getSize() {return size_;};


  private:
    uint32_t size_; // the length of valid characters in buffer.
    char *buffer_;
};

class CompressBlock {
  public:
    CompressBlock(std::vector<IOBuffer*> *io_buffers) {
        io_buffers_ = io_buffers;
    }
    ~CompressBlock() {
        for (auto io_buffer : *io_buffers_) {
            delete io_buffer;
        }
    }
    void setTotalNumber(uint32_t total_number) { total_number_ = total_number; }
    uint32_t getTotalNumber() { return total_number_; }

    void setMaxBufferSize(uint32_t max_buffer_size) {
        max_buffer_size_ = max_buffer_size;
    }
    uint32_t getMaxBufferSize() { return max_buffer_size_; }

    void setIOBuffers(std::vector<IOBuffer*> *io_buffers) {
        io_buffers_ = io_buffers;
    }
    std::vector<IOBuffer*> *getIOBuffers() { return io_buffers_; }
  private:
    uint32_t total_number_;
    uint32_t max_buffer_size_;
    std::vector<IOBuffer*> *io_buffers_;
};

class CompressHandler {
  public:
    CompressHandler(CompressType compress_type, IOManager *io_manager);
    bool compress(CompressBlock &compress_block); 
    CompressBlock *decompress();
    bool decompress(CompressBlock &compress_block);

  private:
    void freeBuffers(std::vector<IOBuffer*> *buffers);

    CompressType compress_type_;
    IOManager *io_manager_;
    std::vector<IOBuffer*> src_buffers_;
    std::vector<IOBuffer*> dst_buffers_;

};


class CompressInput : public MTAppInput {
  public:
    CompressInput(std::vector<IOBuffer*> *src_buffers,
                  std::vector<IOBuffer*> *dst_buffers,
                  std::vector<int>       *compressed_sizes) {
        src_buffers_ = src_buffers;
        dst_buffers_ = dst_buffers;
        compressed_sizes_ = compressed_sizes;
    }
    void setSrcChunks(std::vector<IOBuffer*> *src_buffers) {
        src_buffers_ = src_buffers;
    }
    std::vector<IOBuffer*> *getSrcChunks() {
        return src_buffers_;
    }
    void setDstChunks(std::vector<IOBuffer*> *dst_buffers) {
        dst_buffers_ = dst_buffers;
    }
    std::vector<IOBuffer*> *getDstChunks() {
        return dst_buffers_;
    }
    void setCompressedSizes(std::vector<int> *compressed_sizes) {
        compressed_sizes_ = compressed_sizes;
    }
    std::vector<int> *getCompressedSizes() {
        return compressed_sizes_;
    }

  private:
    std::vector<IOBuffer*> *src_buffers_;
    std::vector<IOBuffer*> *dst_buffers_;
    std::vector<int>       *compressed_sizes_;
};

class CompressTask : public MTTask {
  public:
    CompressTask(IOBuffer *src_buffer, IOBuffer *dst_buffer,
            int *compressed_size) {
        ediAssert(src_buffer != nullptr);
        ediAssert(dst_buffer != nullptr);
        src_buffer_ = src_buffer;
        dst_buffer_ = dst_buffer;
        compressed_size_ = compressed_size;
    }
    void setSrcChunk(IOBuffer *src_buffer) {
        src_buffer_ = src_buffer;
    }
    void setDstChunk(IOBuffer *dst_buffer) {
        dst_buffer_ = dst_buffer;
    }
    IOBuffer *getSrcChunk() {
        return src_buffer_;
    }
    IOBuffer *getDstChunk() {
        return dst_buffer_;
    }
    void setCompressedSize(int32_t compressed_size) {
        *compressed_size_ = compressed_size;
    }
    int32_t* getCompressedSize() {
        return compressed_size_;
    }
  private:
    IOBuffer *src_buffer_;
    IOBuffer *dst_buffer_;
    int32_t* compressed_size_;
};  // class CompressTask

class Compressor : public MTMRApp {
  public:
    
    ~Compressor();

    void setCompressType(CompressType compress_type) {
        compress_type_ = compress_type;
    }
    CompressType getCompressType() {
        return compress_type_;
    }
    void setInput(CompressInput *input) {
        input_ = input;
    };

    virtual void preRun();
    virtual void postRun();
  private:
    CompressType  compress_type_;
    virtual void* runMapper();
    virtual void* runWorker();
    virtual void* runReducer();

    CompressInput *input_;
};

/// @brief Define decompress input for map reducer
class DecompressInput : public MTAppInput {
  public:
    DecompressInput(std::vector<IOBuffer*> *src_chunks,
                    std::vector<IOBuffer*> *dst_chunks,
                    std::vector<int>       *decompressed_sizes) {
        src_chunks_ = src_chunks;
        dst_chunks_ = dst_chunks;
        decompressed_sizes_ = decompressed_sizes;
    }
    void setSrcChunks(std::vector<IOBuffer*> *src_chunks) {
        src_chunks_ = src_chunks;
    }
    std::vector<IOBuffer*> *getSrcChunks() {
        return src_chunks_;
    }
    void setDstChunks(std::vector<IOBuffer*> *dst_chunks) {
        dst_chunks_ = dst_chunks;
    }
    std::vector<IOBuffer*> *getDstChunks() {
        return dst_chunks_;
    }
    void setDecompressedSizes(std::vector<int> *decompressed_sizes) {
        decompressed_sizes_ = decompressed_sizes;
    }
    std::vector<int> *getDecompressedSizes() {
        return decompressed_sizes_;
    }

  private:
    std::vector<IOBuffer*> *src_chunks_;
    std::vector<IOBuffer*> *dst_chunks_;
    std::vector<int>       *decompressed_sizes_;
};

/// @brief Define decompress task for map reducer
class DecompressTask : public MTTask {
  public:
    DecompressTask(IOBuffer *src_chunk, IOBuffer *dst_chunk,
                   int *decompressed_size) {
        ediAssert(src_chunk != nullptr);
        ediAssert(dst_chunk != nullptr);
        src_chunk_ = src_chunk;
        dst_chunk_ = dst_chunk;
        decompressed_size_ = decompressed_size;
    }
    void setSrcChunk(IOBuffer *src_chunk) {
        src_chunk_ = src_chunk;
    }
    void setDstChunk(IOBuffer *dst_chunk) {
        dst_chunk_ = dst_chunk;
    }
    IOBuffer *getSrcChunk() {
        return src_chunk_;
    }
    IOBuffer *getDstChunk() {
        return dst_chunk_;
    }
    void setDecompressedSize(int32_t decompressed_size) {
        *decompressed_size_ = decompressed_size;
    }
    int32_t* getDecompressedSize() {
        return decompressed_size_;
    }
  private:
    IOBuffer *src_chunk_;
    IOBuffer *dst_chunk_;
    int32_t* decompressed_size_;
};  // class DecompressTask

/// @brief Define the decompressor
class Decompressor : public MTMRApp {
  public:
    Decompressor() {};
    Decompressor(CompressType decompress_type,
                 char *src_buffer, uint32_t src_sizes);
    Decompressor(CompressType decompress_type,
                 std::vector<char *> *src_buffers,
                 std::vector<uint32_t *> *src_siezes);
    ~Decompressor() {};

    void getResult(char **dst_buffer, uint32_t dst_size);
    void getResult(std::vector<char *> &dst_buffer,
                   std::vector<uint32_t> &dst_sizes);

    void setDecompressType(CompressType decompress_type) {
        decompress_type_ = decompress_type;
    }
    CompressType getDecompressType() {
        return decompress_type_;
    }
    void setInput(DecompressInput *input) {
        input_ = input;
    };
    virtual void preRun() {};
    virtual void postRun() {};
  private:
    CompressType  decompress_type_;
    virtual void* runMapper();
    virtual void* runWorker();
    virtual void* runReducer();

    DecompressInput *input_;
};

}  // util
}  // open_edi

#endif  // SRC_UTIL_IO_HANDLER_H_
