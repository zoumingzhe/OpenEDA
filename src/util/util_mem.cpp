/**
 * @file  util_mem.cpp
 * @date  Aug 2020
 * @brief Page memory implementation.
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "string.h"

#include <unistd.h>
#include <fstream>
#include <iostream>

#include "util/util_mem.h"
#include "util/message.h"

namespace open_edi {
namespace util {

using namespace std;

/// @brief set page size and relevant fields, point frame correctly
void MemPage::reset() {
    page_size_ = 0;
    page_no_ = 0;
    dirty_ = 0;
    size_total_ = 0;
    size_avail_ = 0;
    frame_ = nullptr;
    free_ = nullptr;
    alloc_num_ = 0;
}

/// @brief set page size and relevant fields, point frame correctly
MemPage::MemPage() { reset(); }

/// @brief set page size and relevant fields, point frame correctly
MemPage::MemPage(size_t size) {
    reset();
    setPageSize(size);
}

/// @brief set page size and relevant fields, point frame correctly
void MemPage::setPageSize(size_t size) {
    page_size_ = size;
    size_total_ = size;
    size_avail_ = size;
}

/// @brief printPageUsage
///
/// @param display
///
/// @return
float MemPage::printPageUsage(bool display) {
    float rate = (float)(size_total_ - size_avail_) / size_total_;
    if (display) {
        cout << "MEMINFO: "
             << "Page addr- " << this << " Page No- " << page_no_ << " frm- "
             << frame_ << ":"
             << " alloc num- " << alloc_num_ << " total size- " << size_total_
             << " available size- " << size_avail_ << " utilize rate " << rate
             << endl;
    }
    return rate;
}

/// @brief constructor of MemChunk
MemChunk::MemChunk() { MemChunk(0); }

/// @brief MemChunk
///
/// @param size
MemChunk::MemChunk(size_t size) : size_(size) {
    if (size > 0) {
        chunk_ = new char[size];
    } else {
        chunk_ = nullptr;
    }
}

/// @brief destructor of MemChunk
MemChunk::~MemChunk() {
    if (size_ > 0) {
        delete[](char *) chunk_;
    }
}

/// @brief init pool
void MemPagePool::__reset() {
    num_pages_ = MEM_PAGE_NUM_INIT;
    page_size_ = 1 << MEM_PAGE_SIZE_BIT;
    chunk_size_ = num_pages_ * page_size_ * sizeof(char);
    num_chunks_ = 0;
    mem_free_ = 0;
    curr_page_id_ = 0;
    mem_used_ = 0;
}

/// @brief release memory
void MemPagePool::__release() {
    pages_.clear();

    for (auto &fl : free_list_) {
        delete fl.second;
    }
    free_list_.clear();

    for (auto &chunk : chunks_) {
        delete chunk;
    }
    chunks_.clear();

    __reset();
}

MemPagePool::MemPagePool() { __reset(); }

MemPagePool::~MemPagePool() {
    std::lock_guard<std::mutex> sg(mutex_);
    __release();
}

MemPage *MemPagePool::__nextPage() {
    if (curr_page_id_ < (num_pages_ - 1)) {
        return pages_[++curr_page_id_];
    } else {
        return nullptr;
    }
}

/// @brief allocate new memory chunk
bool MemPagePool::__allocatePages() {
    MemChunk *mem_chunk = nullptr;
    char *chunk = nullptr;
    MemPage *page = nullptr;

    try {
        mem_chunk = new MemChunk(chunk_size_);
    } catch (std::bad_alloc &ba) {
        throw MemException(chunk_size_);
        return false;
    }

    if (mem_chunk == nullptr) return false;

    chunk = (char *)mem_chunk->getChunk();
    memset(chunk, 0, chunk_size_);

    uint64_t i = pages_.size();
    uint64_t pn = chunk_size_ / (page_size_ * sizeof(char));

    num_pages_ = i + pn;
    pages_.resize(num_pages_, nullptr);

    for (size_t j = 0; j < pn; j++) {
        page = new MemPage(page_size_);
        pages_[i] = page;
        page->setPageNo(i++);
        page->setFrame(&(chunk[j * page_size_ * sizeof(char)]));
    }

    num_chunks_++;
    chunks_.resize(num_chunks_, nullptr);
    chunks_[num_chunks_ - 1] = mem_chunk;

    mem_free_ += chunk_size_;

    return true;
}

/// @brief print poo usage
void MemPagePool::printUsage() {
    float ur = 0.0;

    for (auto &page : pages_) {
        if (!page->isFree()) ur += page->printPageUsage();
    }

    cout << "MEMINFO: "
         << "Number of chunks- " << num_chunks_ << endl;
    cout << "MEMINFO: "
         << "Number of pages- " << num_pages_ << endl;
    cout << "MEMINFO: "
         << "Current (available) page- " << curr_page_id_ << endl;
    cout << "MEMINFO: "
         << "Total memory allocated- "
         << (float)num_pages_ * page_size_ * sizeof(char) / MEM_MEGA_BYTE
         << "MB" << endl;
    cout << "MEMINFO: "
         << "Free memory- " << (float)mem_free_ / MEM_MEGA_BYTE << "MB" << endl;
    cout << "MEMINFO: "
         << "Utilize rate- "
         << 1 - (float)mem_free_ / (num_pages_ * page_size_ * sizeof(char))
         << endl;
    cout << "MEMINFO: "
         << "Avg page utilize rate- " << ur / num_pages_ << endl;
}

void MemPagePool::__writeChunkSizeInfo(std::ofstream &outfile, bool debug) {
    if (debug) cout << "RWDBGINFO: write size " << num_chunks_ << endl;
    outfile.write((char *)&(num_chunks_), sizeof(uint64_t));
    int i = 0;
    for (auto &mem_chunk : chunks_) {
        uint64_t size = mem_chunk->getSize();
        if (debug) cout << "RWDBGINFO: write chunk_size " << size << endl;
        outfile.write((char *)&size, sizeof(uint64_t));
    }
}

void MemPagePool::__readChunkSizeInfo(std::ifstream &infile, bool debug) {
    uint64_t size = 0;
    infile.read((char *)(&size), sizeof(uint64_t));
    if (debug) cout << "RWDBGINFO: read size " << size << endl;

    uint64_t *buffer = new uint64_t[size];

    infile.read((char *)buffer, sizeof(uint64_t) * size);
    num_chunks_ = size;
    chunk_size_ = buffer[size - 1];
    chunks_.resize(num_chunks_, nullptr);
    for (int i = 0; i < size; ++i) {
        if (debug) cout << "RWDBGINFO: read chunk_size " << buffer[i] << endl;
        MemChunk *mem_chunk = new MemChunk(buffer[i]);
        chunks_[i] = mem_chunk;
    }
    delete[] buffer;
}

void MemPagePool::__writePageInfo(std::ofstream &outfile, bool debug) {
    outfile.write((char *)&(curr_page_id_), sizeof(size_t));
    outfile.write((char *)&(page_size_), sizeof(size_t));
    outfile.write((char *)&(num_pages_), sizeof(uint64_t));
    if (debug)
        cout << "RWDBGINFO: write page size " << sizeof(MemPage) << " * num "
             << num_pages_ << " current_page_id " << curr_page_id_ << endl;

    for (auto &page : pages_) {
        if (debug) {
            page->printPageUsage(true);
        }
        outfile.write(reinterpret_cast<char *>(page), sizeof(MemPage));
    }
}

void MemPagePool::__readPageInfo(std::ifstream &infile, bool debug) {
    infile.read((char *)&(curr_page_id_), sizeof(size_t));
    infile.read((char *)&(page_size_), sizeof(size_t));
    infile.read((char *)&(num_pages_), sizeof(uint64_t));
    if (debug)
        cout << "RWDBGINFO: read pagesize " << page_size_ << " pagenum "
             << num_pages_ << " current_page_id " << curr_page_id_ << endl;

    pages_.resize(num_pages_, nullptr);

    uint64_t chunk_index = 0;
    size_t offset_in_chunk = 0;

    for (uint64_t i = 0; i < num_pages_; ++i) {
        MemPage *mem_page = new MemPage(page_size_);
        infile.read(reinterpret_cast<char *>(mem_page), sizeof(MemPage));

        char *chunk_data = (char *)(chunks_[chunk_index]->getChunk());
        size_t chunk_size = chunks_[chunk_index]->getSize();

        if (offset_in_chunk >= chunk_size) {  // use the new chunk:
            ++chunk_index;
            offset_in_chunk = 0;
            chunk_data = (char *)chunks_[chunk_index]->getChunk();
        }
        // set the right chunk data to page's frame
        mem_page->setFrame(&(chunk_data[offset_in_chunk * sizeof(char)]));
        mem_page->adjustFree();
        offset_in_chunk += page_size_;
        pages_[i] = mem_page;
        if (debug) {
            mem_page->printPageUsage(true);
        }
    }
}

void MemPagePool::__writeFreeListInfo(std::ofstream &outfile, bool debug) {
    outfile.write((char *)&(mem_free_), sizeof(uint64_t));

    size_t size = free_list_.size();
    outfile.write((char *)&(size), sizeof(size_t));
    if (debug)
        cout << "RWDBGINFO: write freelist size " << size << " and mem_free "
             << mem_free_ << endl;

    for (auto &fl : free_list_) {
        int obj_type_id = fl.first;
        outfile.write((char *)&(obj_type_id), sizeof(int));

        size_t freeobjs_size = 0;
        for (std::forward_list<void *>::iterator iter = fl.second->begin();
             iter != fl.second->end(); ++iter) {
            ++freeobjs_size;
        }
        outfile.write((char *)&(freeobjs_size), sizeof(size_t));

        if (debug)
            cout << "RWDBGINFO: write freelist objtype_id " << obj_type_id
                 << " freeobjs_size " << freeobjs_size << endl;
        for (std::forward_list<void *>::iterator iter = fl.second->begin();
             iter != fl.second->end(); ++iter) {
            uint64_t freeobj_ptr = (long)(char *)(*iter);
            outfile.write((char *)(&freeobj_ptr), sizeof(uint64_t));
            if (debug)
                cout << "RWDBGINFO: write freelist obj_id " << freeobj_ptr
                     << endl;
        }
    }
}

void MemPagePool::__readFreeListInfo(std::ifstream &infile, bool debug) {
    infile.read((char *)&(mem_free_), sizeof(uint64_t));

    size_t size = 0;
    infile.read((char *)&(size), sizeof(size_t));
    if (debug)
        cout << "RWDBGINFO: read freelist size " << size << " and mem_free "
             << mem_free_ << endl;
    for (int i = 0; i < size; ++i) {
        int obj_type_id = 0;
        size_t freeobjs_size = 0;
        infile.read((char *)&(obj_type_id), sizeof(int));
        infile.read((char *)&(freeobjs_size), sizeof(size_t));
        if (debug)
            cout << "RWDBGINFO: read freelist objtype_id " << obj_type_id
                 << " freeobjs_size " << freeobjs_size << endl;
        for (int j = 0; j < freeobjs_size; ++j) {
            uint64_t freeobj_ptr = 0;
            infile.read((char *)&(freeobj_ptr), sizeof(uint64_t));
            if (debug)
                cout << "RWDBGINFO: read freelist obj_id " << freeobj_ptr
                     << endl;

            if (j == 0) {
                std::forward_list<void *> *fl = new std::forward_list<void *>;
                fl->push_front((void *)freeobj_ptr);
                free_list_[obj_type_id] = fl;
            } else {
                free_list_[obj_type_id]->push_front((void *)freeobj_ptr);
            }
        }
    }
}

void MemPagePool::__writeChunks(std::ofstream &outfile, bool debug) {
    int i = 0;
    for (auto &mem_chunk : chunks_) {
        if (debug)
            cout << "RWDBGINFO: write chunk#" << i << " with size "
                 << mem_chunk->getSize() << endl;
        outfile.write((char *)(mem_chunk->getChunk()), mem_chunk->getSize());
        ++i;
    }
}

void MemPagePool::__readChunks(std::ifstream &infile, bool debug) {
    for (uint64_t i = 0; i < num_chunks_; ++i) {
        void *chunk = chunks_[i]->getChunk();
        size_t size = chunks_[i]->getSize();
        if (debug)
            cout << "RWDBGINFO: read chunk#" << i << " with size "
                 << chunks_[i]->getSize() << endl;
        if (debug)
            cout << "RWDBGINFO: read (before) chunk_data "
                 << ((long *)(chunk))[0] << endl;

        infile.read((char *)(chunk), size);
        if (debug)
            cout << "RWDBGINFO: read chunk_data " << ((long *)(chunk))[0]
                 << endl;
    }
}

/// @brief write header to a file
void MemPagePool::writeHeaderToFile(std::ofstream &outfile, bool debug) {
    if (!outfile) {
        return;
    }
    // 2. write num_chunk & chunk_size
    __writeChunkSizeInfo(outfile, debug);
    // 3. write num_pages & page_info
    __writePageInfo(outfile, debug);
    // 4. write num_free_list & typeid+free_object_ids
    __writeFreeListInfo(outfile, debug);
}

/// @brief write chunk/content to a file
void MemPagePool::writeContentToFile(std::ofstream &outfile, bool debug) {
    if (!outfile) {
        return;
    }
    // 5. write chunks
    __writeChunks(outfile, debug);
    // close-file moved to the UI callback.
    // outfile.close();
}

/// @brief read from file:
void MemPagePool::readFromFile(std::ifstream &infile, bool debug) {
    if (!infile) {
        return;
    }

    // 2. read num_chunk & chunk_size
    __readChunkSizeInfo(infile, debug);
    // 3. read num_pages & page_info
    __readPageInfo(infile, debug);
    // 4. read num_free_list & typeid+free_object_ids
    __readFreeListInfo(infile, debug);
    // 5. read chunks
    __readChunks(infile, debug);
    // close-file moved to UI callback.
    // infile.close();
    if (debug) {
        printUsage();
    }
}

/// @brief MemPool
uint32_t MemPool::pool_no_;
std::array<MemPagePool *, MEM_POOL_MAX> MemPool::indexed_page_pools_;
std::map<uint64_t, MemPagePool *> MemPool::page_pools_;
std::mutex MemPool::mutex_;
bool MemPool::initialized_;
MemPagePool *MemPool::current_pool_;
uint64_t MemPool::current_id_;

/// @brief initMemPool
///
/// @return
bool MemPool::initMemPool() {
    std::lock_guard<std::mutex> sg(mutex_);

    if (initialized_) return true;

    indexed_page_pools_.fill(nullptr);
    pool_no_ = 1;
    current_id_ = 0;
    current_pool_ = nullptr;
    initialized_ = 1;

    return true;
}

/// @brief destroyMemPool
///
/// @return
bool MemPool::destroyMemPool() {
    std::lock_guard<std::mutex> sg(mutex_);

    if (!initialized_) return true;

    for (auto pool : page_pools_) {
        if (pool.second != nullptr) delete pool.second;
    }

    indexed_page_pools_.fill(nullptr);
    page_pools_.clear();
    initialized_ = 0;
    //initMemPool();
    return true;
}

/// @brief newPagePool
///
/// @return
MemPagePool *MemPool::newPagePool() {
    std::lock_guard<std::mutex> sg(mutex_);

    if (!initialized_) {
        message->issueMsg(kError, "Cannot new Page pool when Memory pool"
                                  "is not initialized.\n");
        return nullptr;
    }
    if (pool_no_ == MEM_POOL_MAX) {
        message->issueMsg(kError, "page pool is full. %d\n", pool_no_);
        return nullptr;
    }

    MemPagePool *p = new MemPagePool;
    p->setPoolNo(pool_no_);
    indexed_page_pools_[pool_no_] = p;
    pool_no_++;
    current_pool_ = p; // TODO: for paired use with getCurrentPagePool
    return p;
}

/// @brief newPagePool 
///
/// @param cell_id
///
/// @return 
MemPagePool *MemPool::newPagePool(uint64_t cell_id) {
    std::lock_guard<std::mutex> sg(mutex_);

    if (!initialized_) {
        message->issueMsg(kError, "Cannot new Page pool when Memory pool"
                                  "is not initialized.\n");
        return nullptr;
    }
    if (pool_no_ == MEM_POOL_MAX) {
        message->issueMsg(kError, "page pool is full. %d\n", pool_no_);
        return nullptr;
    }

    MemPagePool *p = new MemPagePool;
    insertPagePool(cell_id, p);
    p->setPoolNo(pool_no_);
    indexed_page_pools_[pool_no_] = p;
    current_id_ = cell_id;
    current_pool_ = p;
    pool_no_++;
    return p;
}
/// @brief setCurrentPagePool
///
/// @return
void MemPool::setCurrentPagePool(MemPagePool *current_pool) {
    current_pool_ = current_pool;
}


/// @brief getCurrentPagePool, return value could be nullptr
///
/// @return
MemPagePool *MemPool::getCurrentPagePool() {
    if (!initialized_) return nullptr;
    return current_pool_;
}

/// @brief insertPagePool 
///
/// @param cell_id
/// @param pool
void MemPool::insertPagePool(uint64_t cell_id, MemPagePool *pool) {
    page_pools_.insert(std::pair<uint64_t, MemPagePool *>(cell_id, pool));
}

/// @brief getPagePool
///
/// @param idx
///
/// @return
MemPagePool *MemPool::getPagePool(uint64_t cell_id) {
    if (!initialized_) return nullptr;
    if (cell_id == current_id_ && current_pool_) return current_pool_;

    auto iter = page_pools_.find(cell_id);
    if (iter != page_pools_.end()) {
        current_id_ = cell_id;
        current_pool_ = iter->second;
        return current_pool_;
    }

    return nullptr;
}

/// @brief getPagePoolByObjectId 
///
/// @param obj_id
///
/// @return 
MemPagePool *MemPool::getPagePoolByObjectId(uint64_t obj_id)
{
    if (!initialized_) return nullptr;

    uint32_t idx;
    idx = ((obj_id & POOL_INDEX_MASK) >> MEM_PAGE_MAX_BIT);
    return indexed_page_pools_[idx];
}

}  // namespace util
}  // namespace open_edi
