/* @file  compress.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "util/compress.h"
#include "util/message.h"

namespace open_edi {
namespace util {

Compressor::~Compressor() {
}

void Compressor::preRun() {
}

void Compressor::postRun() {
}

void* Compressor::runMapper() {
    CompressInput* input = reinterpret_cast<CompressInput*>(input_);
    std::vector<MemChunk*> *src_chunks = input_->getSrcChunks();
    std::vector<MemChunk*> *dst_chunks = input_->getDstChunks();
    std::vector<int>       *compressed_sizes = input->getCompressedSizes();

    for (int i = 0; i < src_chunks->size(); ++i) {
        CompressTask* task = new CompressTask((*src_chunks)[i],(*dst_chunks)[i],
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
        MemChunk *src_chunk = task->getSrcChunk();
        MemChunk *dst_chunk = task->getDstChunk();

        switch (compress_type_) {
            case kLz4:
                task->setCompressedSize(LZ4_compress_default(
                                                (char *)src_chunk->getChunk(),
                                                (char *)dst_chunk->getChunk(),
                                                src_chunk->getSize(),
                                                dst_chunk->getSize()));
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

}  // util
}  // open_edi
