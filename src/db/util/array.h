/**
 * @file  array_object.hpp
 * @date  09/26/2020 11:20:19 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef _EDI_UTIL_ARRAY_OBJECT_HPP_
#define _EDI_UTIL_ARRAY_OBJECT_HPP_

#include "db/core/object.h"
#include "util/util.h"
#include "util/util_mem.h"

namespace open_edi {
namespace db {

// segment is used to store data
class ArraySegment : public Object {
  public:
    /// @brief ArraySegment
    ArraySegment() {
        next_ = 0;
        array_id_ = 0;
    }
    /// @brief ~ArraySegment
    ~ArraySegment() {
        next_ = 0;
        array_id_ = 0;
        seg_no_ = 0;
    }

    /// @brief setNext
    ///
    /// @param id
    void setNext(ObjectId id) { next_ = id; }

    /// @brief getNext
    ///
    /// @return
    ObjectId getNext() { return next_; }

    /// @brief setArrayId
    ///
    /// @param id
    void setArrayId(ObjectId id) { array_id_ = id; }

    /// @brief getArrayId
    ///
    /// @return
    ObjectId getArrayId() { return array_id_; }

    /// @brief getNo 
    ///
    /// @return 
    uint64_t getNo() {return seg_no_;}

    /// @brief setNo 
    ///
    /// @param no
    void setNo(uint64_t no) {seg_no_ = no;}

  private:
    uint64_t seg_no_;
    ObjectId next_;  // id of next ArraySegment object
    ObjectId array_id_;
};

// array object
// Usage:
//      setPool(pool);
//      reserve(size);
//      pushBack(T &e);
//      a[index] = e;
template <class T>
class ArrayObject : public Object {
  public:
    void initArrayObject() {
        pool_ = nullptr;
        is_initialized_ = false;
        current_avail_ = 0;
        current_seg_ = nullptr;
        size_ = 0;
    }
    /// @brief ArrayObject
    ///
    /// @param pool
    ArrayObject() { initArrayObject(); }
    /// @brief ~ArrayObject
    ~ArrayObject();

    /// @brief setPool
    ///
    /// @param pool
    void setPool(MemPagePool *pool) { pool_ = pool; }

    /// @brief getPool
    ///
    /// @return
    MemPagePool* getPool() {
        if (nullptr != pool_) return pool_;
        pool_ = MemPool::getPagePoolByObjectId(getId());
        return pool_;
    }

    /// @brief reserve
    ///
    /// @param size
    ///
    /// @return
    bool reserve(int64_t size) {
        if (nullptr == getPool()) return false;

        if (size >= kSegmentSize) {
            segment_size_ = kSegmentSize;
        } else {
            segment_size_ = size;
        }

        uint64_t seg_no = 1;
        ObjectId seg_id = 0;
        ArraySegment *seg_ptr = createSegment(segment_size_, seg_id);
        if (nullptr == seg_ptr) {
            return false;
        }
        seg_ptr->setNo(seg_no);
        this->setFirstSegmentId(seg_id);
        segment_num_ = 1;
        current_avail_ = 0;
        current_seg_ = seg_ptr;
        size_ = segment_size_;

        // This is actual array size, not index
        // Since getSegmentNumber() assumes the parameter as index
        // Decrease by 1 before passing into
        int32_t seg_num = getSegmentNumber(size - 1);  // total segments needed

        ArraySegment *new_ptr = nullptr;
        while (--seg_num) {  // ignore the first one
            ObjectId id;
            new_ptr = createSegment(segment_size_, id);
            if (nullptr == new_ptr) {
                return false;
            }
            seg_no++;
            new_ptr->setNo(seg_no);
            seg_ptr->setNext(id);
            seg_ptr = new_ptr;
            segment_num_++;
            size_ += segment_size_;
        }
        is_initialized_ = true;
        return true;
    }

    /// @brief operator[]
    ///
    /// @param index
    ///
    /// @return
    T &operator[](uint64_t index) {
        int32_t seg_idx = getSegmentIndex(index);

        ArraySegment *seg_ptr = nullptr;
        seg_ptr = getSegmentByIndex(index);

        ediAssert(nullptr != seg_ptr);
        MemPagePool* pool = getPool();
        T *array = pool->getObjectPtr<T>(seg_ptr->getArrayId());
        return array[seg_idx];
    }

    /// @brief pushBack
    ///
    /// @param ele
    ///
    /// @return
    bool pushBack(const T &ele) {
        int32_t index = getSegmentIndex(current_avail_);

        ArraySegment *seg_ptr = nullptr;
        seg_ptr = getSegmentByIndex(current_avail_);

        if (nullptr != seg_ptr) {
            MemPagePool* pool = getPool();
            T *array = pool->getObjectPtr<T>(seg_ptr->getArrayId());
            array[index] = ele;
            current_avail_++;
            return true;
        }

        return false;
    }

    /// @brief getArraySize returns total allocated array size.
    ///
    /// @return
    int64_t getArraySize() { return size_; }

    /// @brief getSize returns used size. less than or equal to total size.
    ///
    /// @return 
    int64_t getSize() { return current_avail_; }

    /// @brief adjustSize adjusts used size.
    ///
    /// @return true if the given size is set as expected, otherwise false.
    bool adjustSize(int64_t size) { 
        if (size > size_) {
            increaseArraySize(size);
        }
        current_avail_ = size;
        return true; 
    }

    class iterator {
      public:
        iterator() {};

        /// @brief iterator 
        ///
        /// @param array_ptr
        iterator(ArrayObject<T> *array_ptr) {
            setArray(array_ptr);
        }

        /// @brief operator== 
        ///
        /// @param iter
        ///
        /// @return 
        bool operator==(iterator iter) {return !(getIndex() - iter.getIndex());}

        /// @brief operator!= 
        ///
        /// @param iter
        ///
        /// @return 
        bool operator!=(iterator iter) {return (getIndex() - iter.getIndex());}

        /// @brief operator++ 
        ///
        /// @param i
        ///
        /// @return 
        iterator operator++(int) {
            iterator tmp_iter = *this;
            curr_index_++;
            return tmp_iter;
        }

        /// @brief operator++ 
        ///
        /// @return 
        iterator& operator++() {
            curr_index_++;
            return *this;
        }

        /// @brief operator 
        ///
        /// @param iter
        ///
        /// @return 
        T& operator*() {return (*array_)[curr_index_];}

        /// @brief operator= 
        ///
        /// @param iter
        void operator=(iterator iter) {
            array_ = iter.array_;
            curr_index_ = iter.curr_index_;
        }

        /// @brief setIndex 
        ///
        /// @param index
        void setIndex(int32_t index) {curr_index_ = index;}

       
      private:
        void setArray(ArrayObject<T> *array_ptr) {
            array_ = array_ptr;
            curr_index_ = 0;
        }
        int32_t getIndex() {return curr_index_;}

      private:
        ArrayObject<T> *array_;
        int32_t curr_index_;
    };

    /// @brief begin 
    ///
    /// @return 
    iterator begin() {
        iterator iter = iterator(this);
        return iter;
    }

    /// @brief end 
    ///
    /// @return 
    iterator end() {
        iterator iter = iterator(this);
        iter.setIndex(getSize());
        return iter; 
    }

  private:
    /// @brief getFirstSegmentId
    ///
    /// @return
    ObjectId getFirstSegmentId() { return segments_; }
    /// @brief setFirstSegmentId
    ///
    /// @param seg_id
    void setFirstSegmentId(ObjectId seg_id) { segments_ = seg_id; }

    /// @brief getSegmentNumber
    ///
    /// @param index
    ///
    /// @return
    int32_t getSegmentNumber(int64_t index) {
        // if you call this fucntion with total size, you need to convert to
        // index number say, if you want to know how many segments needed for
        // totol N elements, call: getSegmentNumber(N-1);
        return (index + segment_size_) / segment_size_;
    }

    /// @brief getSegmentIndex
    ///
    /// @param index
    ///
    /// @return
    int32_t getSegmentIndex(int64_t index) { return index % segment_size_; }

    ArraySegment *increaseArraySize(int64_t to_size);

    /// @brief createSegment
    ///
    /// @param segment_size
    /// @param seg_id
    ///
    /// @return
    ArraySegment *createSegment(int32_t segment_size, uint64_t &seg_id) {
        T *array_ptr = nullptr;
        uint64_t array_id;
        MemPagePool* pool = getPool();
        if (!pool) return nullptr;

        ArraySegment *seg_ptr =
            pool->allocate<ArraySegment>(kObjectTypeArraySegment, seg_id);
        if (nullptr == seg_ptr) return nullptr;
        seg_ptr->setId(seg_id);
        seg_ptr->setObjectType(kObjectTypeArraySegment);
        array_ptr = pool->allocateArray<T>(segment_size, array_id);
        if (nullptr == array_ptr) {
            pool->free<ArraySegment>(kObjectTypeArraySegment, seg_ptr);
            return nullptr;
        }
        seg_ptr->setArrayId(array_id);
        return seg_ptr;
    }

    /// @brief getSegmentByIndex
    ///
    /// @param index
    ///
    /// @return
    ArraySegment *getSegmentByIndex(int32_t index) {
        int32_t seg_num = getSegmentNumber(index);  // get needed segment number

        if (seg_num > segment_num_) {
            current_seg_ = increaseArraySize(index);
            return current_seg_;
        }

        int start_i = 0;
        
        if (index >= ((current_seg_->getNo()-1) * segment_size_)) {
            if (index < (current_seg_->getNo() * segment_size_)) {
                return current_seg_;
            } else {
                start_i = current_seg_->getNo();
            }
        }

        ObjectId seg_id;
        ArraySegment *seg_ptr = nullptr;
        MemPagePool* pool = getPool();

        if (start_i == 0) {
            seg_id = getFirstSegmentId();
        } else {
            seg_id = current_seg_->getNext();
        }
        for (; start_i < seg_num; start_i++) {
            seg_ptr = pool->getObjectPtr<ArraySegment>(seg_id);
            seg_id = seg_ptr->getNext();
        }
        current_seg_ = seg_ptr;
        return seg_ptr;
    }

  private:
    const static int kSegmentSize = 32;

    MemPagePool *pool_;
    int64_t size_;           // array size
    int32_t segment_size_;   // size of each segment
    int32_t segment_num_;    // number of segments
    ObjectId segments_;      // id for first ArraySegment object
    int64_t current_avail_;  // index for next available element
    ArraySegment* current_seg_;  // index for next available element
    bool is_initialized_;
};

/// @brief increaseArraySize
///
/// @tparam T
/// @param index
///
/// @return
template <class T>
ArraySegment *ArrayObject<T>::increaseArraySize(int64_t index) {
    int32_t seg_num = getSegmentNumber(index);  // get needed segment number

    ArraySegment *seg_ptr = nullptr;
    seg_ptr = getSegmentByIndex(segment_size_ * segment_num_ - 1);  // last one

    seg_num -= segment_num_;
    // now seg_ptr points to the last segment
    while (seg_num--) {
        ObjectId id;
        ArraySegment *new_ptr = nullptr;
        new_ptr = createSegment(segment_size_, id);
        if (nullptr == new_ptr) {
            return nullptr;
        }
        new_ptr->setNo(seg_ptr->getNo()+1);
        seg_ptr->setNext(id);
        segment_num_++;
        size_ += segment_size_;
        seg_ptr = new_ptr;
    }

    return seg_ptr;
}

/// @brief ~ArrayObject
///
/// @tparam T
template <class T>
ArrayObject<T>::~ArrayObject() {
    if (!is_initialized_) return;
    MemPagePool* pool = getPool();
    if (!pool) return;

    ArraySegment *seg_ptr = pool->getObjectPtr<ArraySegment>(segments_);
    ArraySegment *pre_ptr = seg_ptr;
    while (segment_num_--) {
        seg_ptr = pool->getObjectPtr<ArraySegment>(seg_ptr->getNext());
        pool->free(kObjectTypeArraySegment, pre_ptr);
        pre_ptr = seg_ptr;
    }
    initArrayObject();
}

}  // namespace db
}  // namespace open_edi
#endif
