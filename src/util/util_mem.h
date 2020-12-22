/** 
 * @file  util_mem.hpp
 * @date  Aug 2020
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include <assert.h>
#include <map>
#include <vector>
#include <forward_list>
#include <limits.h>
#include <mutex>
#include "util/namespace.h"
#include "util/io_manager.h"

#ifndef EDI_UITIL_MEM_HPP_
#define EDI_UITIL_MEM_HPP_

namespace open_edi {
namespace util {

using Bits = uint64_t;

class MemException {
  public:
    MemException(uint64_t sz) : size_(sz) {}
    int getSize() {return size_;}
  private:
    uint64_t size_;
};

// Mega bytes
#define MEM_POOL_MAX    (1 << 6)        // max pool number we can handle
#define MEM_MEGA_BYTE   ((1 << 10)*(1 << 10)) // 1MB
#define MEM_ALIGN_BIT   3     // align addr with 8
// Each page is 1MB, use lower 20bits as offset
// Page number uses 30bits, total 1e6GB memory
// Total id occupies 56bits
// Assume average 100Bytes per object, approximately 10,000 Billion objects per pool
#define MEM_PAGE_MAX_BIT  50  // 30bits for page number
#define MEM_PAGE_SIZE_BIT 20  // 1MB per page, also used when compile/de-compile id
#define MEM_PAGE_NUM_INIT 32  // fixed increamental
// Object id is composed of three parts: pool number, page number & offset in page
#define POOL_INDEX_MASK  0x00FC000000000000  // first 6bits of total 56bits
#define PAGE_INDEX_MASK  0x0003FFFFFFF00000  // following 30bits of total 56bits
#define PAGE_OFFSET_MASK 0x00000000000FFFFD  // rest 20bits

class MemPage {
  public:
    MemPage();
    MemPage(size_t size);
    ~MemPage() {};
    void    setPageSize(size_t s);
    void    setFrame(char *ptr) {frame_ = ptr; free_ = ptr;}
    char*   getFrame() {return frame_;}
    void    setPageNo(uint64_t v) {page_no_ = v;}
    uint64_t    getPageNo() {return page_no_;}
    float       printPageUsage(bool display = false);
    bool        isFree() {return size_avail_ == size_total_;} 
    void        adjustFree() {free_ += size_total_ - size_avail_;}
    template<class T> T* allocate(uint32_t &offset);
    template<class T> T* allocate(uint64_t num, uint32_t &offset);

  private:
    void    reset();
    uint32_t    __getAvail() {return size_avail_;}
    inline void __align(uint32_t &size) {
        size = ((size+(1<<MEM_ALIGN_BIT)-1)>>MEM_ALIGN_BIT)<<MEM_ALIGN_BIT;
    }

  private:
    Bits dirty_:1; /* Bit 0 */
    Bits size_avail_:21; 
    Bits size_total_:21;
    Bits page_size_:21;
    uint32_t alloc_num_;
    uint64_t page_no_;
    char *frame_;
    char *free_;
};

/// @brief allocate 
///
/// @tparam T
/// @param offset
///
/// @return 
template <class T>
T *MemPage::allocate(uint32_t &offset)
{
    T *obj = nullptr;

    uint32_t r_size = 0;    // 1. aligned required size

    r_size = sizeof(T);
    __align(r_size);

    if (size_avail_ < r_size) {
        return nullptr;
    }

    offset = free_ - frame_;    // 2. compute new object's offset
    obj = new((T*)free_)T;      // 3. placement new to call T's constructor
    free_ += r_size;            // 4. move free_ to next available addr
    size_avail_ -= r_size;      // 5. update size_avail_
    alloc_num_++;               // 6. increase alloc number

    return obj;
}

template <class T>
T *MemPage::allocate(uint64_t num, uint32_t &offset)
{
    T *obj = nullptr;

    uint32_t r_size = 0;    // 1. aligned required size

    r_size = sizeof(T) * num;
    __align(r_size);

    if (size_avail_ < r_size) {
        return nullptr;
    }

    offset = free_ - frame_;    // 2. compute new object's offset
    obj = new((T*)free_)T[num]; // 3. placement new to call T's constructor
    // placement new operator seems to work for basic data types,
    // but let's assert here in case there's a problem
    assert(free_ == (char *)obj);
    free_ += r_size;            // 4. move free_ to next available addr
    size_avail_ -= r_size;      // 5. update size_avail_
    alloc_num_++;               // 6. increase alloc number

    return obj;
}

class MemChunk {
  public:
    MemChunk();
    MemChunk(size_t size);

    ~MemChunk();
    
    void setSize(size_t size) { size_ = size; }
    size_t getSize() const { return size_;}
    void *getChunk() const { return chunk_;}

  private:
    size_t size_;
    void *chunk_;
};

class MemPagePool {
  public:
    MemPagePool();
    ~MemPagePool();

    /// @brief allocate mem & initialize object id
    template<class T> T *allocate(int type, uint64_t &id); 
    template<typename T> T *allocateArray(int64_t size, uint64_t &id); 
    template<class T> void free(const int type, T *o);
    template<class T> T *getObjectPtr(uint64_t id);

    MemPage*    getPage(uint64_t pid) {return pages_.empty()?nullptr:pages_[pid];}
    MemPage*    getCurrentPage() {return getPage(curr_page_id_);}
    void        setPoolNo(size_t n) {pool_no_ = n;}
    size_t      getPoolNo() {return pool_no_;}
    void        printUsage();
    void        writeHeaderToFile(IOManager & io_manager, bool debug = false);
    void        writeContentToFile(IOManager & io_manager, bool debug = false);
    void        readFromFile(IOManager & io_manager, bool debug = false);

  private:
    void        __reset();
    void        __release();
    bool        __allocatePages(); // allocate page chunks, re-alloc page array;
    template<class T> T* __allocateFromFreeList(const int type);
    template<class T> T* __allocateFromPages(uint32_t &offset);
    template<class T> T* __allocateFromPages(uint64_t num, uint32_t &offset);
    MemPage*    __nextPage();

    inline uint64_t __computeObjectId(MemPage *p, size_t of) 
    {
        // pool number starts from 1, id is non-zero.
        return ((getPoolNo() << MEM_PAGE_MAX_BIT) | (p->getPageNo() << MEM_PAGE_SIZE_BIT) | of);
    }

    bool __pageEnd() {
        return (pages_.empty() || curr_page_id_ == (pages_.size()-1));
    }

    inline void __align(uint64_t &size) {
        size = ((size+(1<<MEM_ALIGN_BIT)-1)>>MEM_ALIGN_BIT)<<MEM_ALIGN_BIT;
    }
    
    void __writeChunkSizeInfo(IOManager & io_manager, bool debug = false);
    void __readChunkSizeInfo(IOManager & io_manager, bool debug = false);  
    void __writePageInfo(IOManager & io_manager, bool debug = false);
    void __readPageInfo(IOManager & io_manager, bool debug = false);
    void __writeFreeListInfo(IOManager & io_manager, bool debug = false);
    void __readFreeListInfo(IOManager & io_manager, bool debug = false);
    void __writeChunks(IOManager & io_manager, bool debug = false);
    void __readChunks(IOManager & io_manager, bool debug = false);
  private:
    std::mutex mutex_;

    size_t pool_no_;
    size_t page_size_;
    uint64_t num_pages_;
    uint64_t curr_page_id_;
    uint64_t mem_used_;  // used pages
    uint64_t mem_free_;  // free memory
    uint64_t chunk_size_;
    uint64_t num_chunks_;
    std::vector<MemPage *> pages_;
    std::map<int, std::forward_list<void *>*> free_list_;
    std::vector<MemChunk *> chunks_;
};

/// @brief free an object & put it in free list
template<class T>
void MemPagePool::free(int type, T *obj)
{
    obj->~T(); // de-construct

    std::lock_guard<std::mutex> sg(mutex_);
    uint64_t size = sizeof(T);
    __align(size);

    auto it = free_list_.find(type); 
    if (it == free_list_.end()) {
        std::forward_list<void*> *fl = new std::forward_list<void*>;
        fl->push_front((void*)obj);
        free_list_[type] = fl;
    } else {
        it->second->push_front((void*)obj);
    }
    mem_free_ += size*sizeof(char);
}

/// @brief __allocateFromFreeList 
///
/// @tparam T
/// @param type
///
/// @return 
template<class T>
T* MemPagePool::__allocateFromFreeList(const int type)
{
    T *ptr = nullptr;

    if (free_list_.empty()) return nullptr;

    auto it = free_list_.find(type); 
    if (it != free_list_.end() && !it->second->empty()) {
        ptr = (T*)(it->second->front());
        it->second->erase_after(it->second->before_begin());
    }
    return ptr;
}

/// @brief __allocateFromPages 
///
/// @tparam T
/// @param offset
///
/// @return 
template<class T>
T* MemPagePool::__allocateFromPages(uint32_t &offset)
{
    T *obj = nullptr;
    MemPage *p = nullptr;

    uint64_t size = sizeof(T);
    __align(size);

    if (size <= mem_free_) {
        p = getCurrentPage();
        while (p) {
            obj = p->allocate<T>(offset);
            // success, or no avail pages
            if (obj != nullptr || __pageEnd()) break;
            p = __nextPage();
        }
    }

    return obj;
}

template<class T>
T* MemPagePool::__allocateFromPages(uint64_t num, uint32_t &offset)
{
    T *obj = nullptr;
    MemPage *p = nullptr;

    uint64_t size = sizeof(T) * num;
    __align(size);

    if (size <= mem_free_) {
        p = getCurrentPage();
        while (p) {
            obj = p->allocate<T>(num, offset);
            // success, or no avail pages
            if (obj != nullptr || __pageEnd()) break;
            p = __nextPage();
        }
    }

    return obj;
}

 /// @brief allocate 
 ///
 /// @param type
 /// @param id
 ///
 /// @return nullptr upon failure, pointer to desired class upon success.
template<class T>
T *MemPagePool::allocate(const int type, uint64_t &id)
{
    id = ULONG_MAX;
    T *obj = nullptr;

    uint64_t size = sizeof(T);
    __align(size);
    assert(size <= page_size_);

    std::lock_guard<std::mutex> sg(mutex_);

    // free list first.
    if (obj = __allocateFromFreeList<T>(type)) {
        mem_free_ -= size*sizeof(char);
        id = obj->getId();
        return new(obj)T;
    }

    uint32_t offset = 0;
    MemPage *p = nullptr;

    // second, from current available page
    obj = __allocateFromPages<T>(offset);

    // third, if free mem on current page is not adequate 
    // then allocate new pages, and allocate from new pages
    if (obj == nullptr && __pageEnd()) {
        try {
            __allocatePages();
        } catch (MemException &e) {
            return (T*)obj;
        }
        obj = __allocateFromPages<T>(offset);
    }

    if (obj == nullptr) return (T*)nullptr;
    p = getCurrentPage();
    
    mem_free_ -= size*sizeof(char);
    id = __computeObjectId(p, offset); // compute id

    return obj;
}


template<typename T>
T *MemPagePool::allocateArray(int64_t num, uint64_t &id)
{
    id = ULONG_MAX;
    T *obj = nullptr;

    std::lock_guard<std::mutex> sg(mutex_);

    uint64_t size = sizeof(T) * num;
    __align(size);

    assert(size<=page_size_);

    uint32_t offset = 0;
    MemPage *p = nullptr;

    // second, from current available page
    obj = __allocateFromPages<T>(num, offset);

    // third, if free mem on current page is not adequate 
    // then allocate new pages, and allocate from new pages
    if (obj == nullptr && __pageEnd()) {
        try {
            __allocatePages();
        } catch (MemException &e) {
            return (T*)obj;
        }
        obj = __allocateFromPages<T>(num, offset);
    }

    if (obj == nullptr) return (T*)nullptr;
    p = getCurrentPage();
    
    mem_free_ -= size*sizeof(char);
    id = __computeObjectId(p, offset); // compute id

    return obj;
}

/// @brief getObjectPtr 
///
/// @tparam T
/// @param id
///
/// @return 
template <class T>
T *MemPagePool::getObjectPtr(uint64_t id) {
    uint32_t idx;
    char *ptr;

    // get page with page no given by middle 35 bits
    idx = ((id & PAGE_INDEX_MASK) >> MEM_PAGE_SIZE_BIT);
    ptr = getPage(idx)->getFrame();

    // get real addr with offset given by last 10 bits
    ptr += (id & PAGE_OFFSET_MASK);

    return (T*)ptr;
}

class MemPool
{
  public:
    enum PagePoolType {
        kPagePoolTypeLogic=1,
        kPagePoolTypeLib,
        kPagePoolTypeMax
    };
    static bool initMemPool();
    static bool destroyMemPool();
    static MemPagePool *newPagePool();
    static MemPagePool *newPagePool(uint64_t cell_id);
    static void insertPagePool(uint64_t cell_id, MemPagePool *pool);
    static MemPagePool *getPagePool(uint64_t cell_id);
    static void setCurrentPagePool(MemPagePool *current_pool); // TODO: move to private
    static MemPagePool *getPagePoolByObjectId(uint64_t obj_id);
    static MemPagePool *getCurrentPagePool(); // TODO: move to private

    template<class T> static T *getObjectPtr(uint64_t obj_id);
    template<class T> static T *getObjectPtr(uint64_t cell_id, uint64_t obj_id);

  private:
    static std::mutex mutex_;
    static bool initialized_;
    static uint32_t pool_no_;
    static std::array<MemPagePool *, MEM_POOL_MAX> indexed_page_pools_;
    static std::map<uint64_t, MemPagePool *> page_pools_;
    static MemPagePool *current_pool_;
    static uint64_t current_id_;
};

/// @brief getObjectPtr 
///
/// @tparam T
/// @param obj_id
///
/// @return 
template <class T>
T *MemPool::getObjectPtr(uint64_t obj_id) {
    if (!initialized_) return nullptr;

    MemPagePool *pp = getPagePoolByObjectId(obj_id);
    if (nullptr == pp) return nullptr;
    return pp->getObjectPtr<T>(obj_id);
}

/// @brief getObjectPtr 
///
/// @tparam T
/// @param cell_id
/// @param obj_id
///
/// @return 
template <class T>
T *MemPool::getObjectPtr(uint64_t cell_id, uint64_t obj_id) {
    if (!initialized_) return nullptr;
    // get pool
    MemPagePool *pp = getPagePool(cell_id);
    return pp->getObjectPtr<T>(obj_id);
}

}  // namespace util
}  // namespace open_edi

#endif
