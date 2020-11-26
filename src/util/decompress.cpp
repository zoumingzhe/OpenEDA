/* @file  decompress.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "util/decompress.h"
#include "util/message.h"

namespace open_edi {
namespace util {

Decompressor::~Decompressor() {
}

void Decompressor::preRun() {
}

void Decompressor::postRun() {
}

void* Decompressor::runMapper() {
    DecompressInput* input = reinterpret_cast<DecompressInput*>(input_);
    std::vector<MemChunk*> *src_chunks = input_->getSrcChunks();
    std::vector<MemChunk*> *dst_chunks = input_->getDstChunks();
    std::vector<int>       *decompressed_sizes = input->getDecompressedSizes();

    for (int i = 0; i < src_chunks->size(); ++i) {
        DecompressTask* task = new DecompressTask((*src_chunks)[i],
                                                  (*dst_chunks)[i],
                                                  &(*decompressed_sizes)[i]);
        task_queue_.push(task);
    }
    return nullptr;
}

void* Decompressor::runWorker() {
    while (true) {
        DecompressTask* task = static_cast<DecompressTask*>(task_queue_.pop());
        if (!task) {
            break;
        }
        MemChunk *src_chunk = task->getSrcChunk();
        MemChunk *dst_chunk = task->getDstChunk();

        switch (decompress_type_) {
            case kDecompressLz4:
                task->setDecompressedSize(LZ4_decompress_safe(
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

void* Decompressor::runReducer() {
    return nullptr;
}

}  // util
}  // open_edi
