/* @file  decompress.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef SRC_UTIL_DECOMPRESS_H_
#define SRC_UTIL_DECOMPRESS_H_
#include <lz4.h>
#include <lz4frame.h>

#include "util/util_mem.h"
#include "util/message.h"
#include "util/map_reduce.h"

namespace open_edi {
namespace util {

enum DecompressType {
    kDecompressLz4,
    kDecompressGz,
    kDecompressZip
};

class DecompressInput : public MTAppInput {
  public:
    DecompressInput(std::vector<MemChunk*> *src_chunks,
                    std::vector<MemChunk*> *dst_chunks,
                    std::vector<int>       *decompressed_sizes) {
        src_chunks_ = src_chunks;
        dst_chunks_ = dst_chunks;
        decompressed_sizes_ = decompressed_sizes;
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
    void setDecompressedSizes(std::vector<int> *decompressed_sizes) {
        decompressed_sizes_ = decompressed_sizes;
    }
    std::vector<int> *getDecompressedSizes() {
        return decompressed_sizes_;
    }

  private:
    std::vector<MemChunk*> *src_chunks_;
    std::vector<MemChunk*> *dst_chunks_;
    std::vector<int>       *decompressed_sizes_;
};

class DecompressTask : public MTTask {
  public:
    DecompressTask(MemChunk *src_chunk, MemChunk *dst_chunk,
                   int *decompressed_size) {
        ediAssert(src_chunk != nullptr);
        ediAssert(dst_chunk != nullptr);
        src_chunk_ = src_chunk;
        dst_chunk_ = dst_chunk;
        decompressed_size_ = decompressed_size;
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
    void setDecompressedSize(int32_t decompressed_size) {
        *decompressed_size_ = decompressed_size;
    }
    int32_t* getDecompressedSize() {
        return decompressed_size_;
    }
  private:
    MemChunk *src_chunk_;
    MemChunk *dst_chunk_;
    int32_t* decompressed_size_;
};  // class DecompressTask

class Decompressor : public MTMRApp {
  public:
    
    ~Decompressor();

    void setDecompressType(DecompressType decompress_type) {
        decompress_type_ = decompress_type;
    }
    DecompressType getDecompressType() {
        return decompress_type_;
    }
    void setInput(DecompressInput *input) {
        input_ = input;
    };

    virtual void preRun();
    virtual void postRun();
  private:
    DecompressType  decompress_type_;
    virtual void* runMapper();
    virtual void* runWorker();
    virtual void* runReducer();

    DecompressInput *input_;
};

}  // util
}  // open_edi
#endif  // SRC_UTIL_DECOMPRESS_H_
