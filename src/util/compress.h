/* @file  compress.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef SRC_UTIL_COMPRESS_H_
#define SRC_UTIL_COMPRESS_H_
#include <lz4.h>
#include <lz4frame.h>

#include "util/util_mem.h"
#include "util/message.h"
#include "util/map_reduce.h"

namespace open_edi {
namespace util {

enum CompressType {
    kCompressLz4,
    kCompressGz,
    kCompressZip
};

class CompressInput : public MTAppInput {
  public:
    CompressInput(std::vector<MemChunk*> *src_chunks,
                  std::vector<MemChunk*> *dst_chunks,
                  std::vector<int>       *compressed_sizes) {
        src_chunks_ = src_chunks;
        dst_chunks_ = dst_chunks;
        compressed_sizes_ = compressed_sizes;
    }
    void setSrcChunks(std::vector<MemChunk*> *src_chunks) {
        src_chunks_ = src_chunks;
    }
    std::vector<MemChunk*> *getSrcChunks() {
        return src_chunks_;
    }
    void setDstChunks(std::vector<MemChunk*> *dst_chunks) {
        dst_chunks_ = dst_chunks;
    }
    std::vector<MemChunk*> *getDstChunks() {
        return dst_chunks_;
    }
    void setCompressedSizes(std::vector<int> *compressed_sizes) {
        compressed_sizes_ = compressed_sizes;
    }
    std::vector<int> *getCompressedSizes() {
        return compressed_sizes_;
    }

  private:
    std::vector<MemChunk*> *src_chunks_;
    std::vector<MemChunk*> *dst_chunks_;
    std::vector<int>       *compressed_sizes_;
};

class CompressTask : public MTTask {
  public:
    CompressTask(MemChunk *src_chunk, MemChunk *dst_chunk,
            int *compressed_size) {
        ediAssert(src_chunk != nullptr);
        ediAssert(dst_chunk != nullptr);
        src_chunk_ = src_chunk;
        dst_chunk_ = dst_chunk;
        compressed_size_ = compressed_size;
    }
    void setSrcChunk(MemChunk *src_chunk) {
        src_chunk_ = src_chunk;
    }
    void setDstChunk(MemChunk *dst_chunk) {
        dst_chunk_ = dst_chunk;
    }
    MemChunk *getSrcChunk() {
        return src_chunk_;
    }
    MemChunk *getDstChunk() {
        return dst_chunk_;
    }
    void setCompressedSize(int32_t compressed_size) {
        *compressed_size_ = compressed_size;
    }
    int32_t* getCompressedSize() {
        return compressed_size_;
    }
  private:
    MemChunk *src_chunk_;
    MemChunk *dst_chunk_;
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

class FileManager {
  public:
};


}  // util
}  // open_edi
#endif  // SRC_UTIL_COMPRESS_H_
