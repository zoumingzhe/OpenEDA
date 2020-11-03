/**
 * @file  VectorObjectVarVar.h
 * @date  08/24/2020 06:45:29 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_DB_UTIL_VECTOR_OBJECT_VAR_H_
#define EDI_DB_UTIL_VECTOR_OBJECT_VAR_H_

#include "db/core/object.h"
#include "util/util.h"

namespace open_edi {
namespace db {

enum VectorSizeEnum {
    kSize_8 = 8,
    kSize_16 = 16,
    kSize_32 = 32,
    kSize_64 = 64,
    kSize_128 = 128,
    kSize_256 = 256,
    kSizeHighest = 503
};

template <VectorSizeEnum kArraySize>
class VectorObjectVar : public Object {
  public:
    using BaseType = Object;
    /// @brief default constructor
    VectorObjectVar();

    /// @brief constructor
    VectorObjectVar(Object *owner, uint32_t id);

    /// @brief copy constructor
    VectorObjectVar(VectorObjectVar<kArraySize> const &rhs);

    /// @brief move constructor
    VectorObjectVar(VectorObjectVar<kArraySize> &&rhs) noexcept;

    /// @brief copy assignment
    VectorObjectVar<kArraySize> &operator=(
        VectorObjectVar<kArraySize> const &rhs);

    /// @brief move constructor
    VectorObjectVar<kArraySize> &operator=(
        VectorObjectVar<kArraySize> &&rhs) noexcept;
    /// @brief clone current object for extensibility.
    /// Users need to manage the pointer to avoid memory leakage.
    /// @return the pointer to the new object
    VectorObjectVar<kArraySize> *clone() const;

    /// @brief summarize memory usage of the object in bytes
    uint32_t memory() const;

    // Get:
    uint32_t totalCapacity() const;
    uint32_t totalSize() const;
    ObjectId operator[](int i) const;
    // Set:
    ObjectId &operator[](int i);
    void push_back(ObjectId id);
    void push_back(ObjectId cell_id, ObjectId id);
    ObjectId pop_back();
    bool removeObjectIdFromVector(ObjectId id);
    void reserve(uint32_t size);
    // DB vector object creation/deletion:
    static VectorObjectVar<kArraySize> *createDBVectorObjectVar(bool is_header);
    static void deleteDBVectorObjectVar(ObjectId id);
    // Iterator:
    class iterator {
      public:
        iterator(ObjectId *p = nullptr) : ptr_(p), start_(p) {}
        ObjectId &operator*() const { return *ptr_; }
        ObjectId *operator->() const { return ptr_; }
        iterator &operator++() {
            ++ptr_;
            if ((ptr_ - start_ + 1) == kArraySize) {
                ObjectId next_id = *ptr_;
                // TODO: next_id==0: should we mark this iterator as illegal for
                // next ++?
                if (next_id != 0) {
                    VectorObjectVar<kArraySize> *next_object =
                        MemPool::getObjectPtr<VectorObjectVar<kArraySize> >(
                            next_id);
                    ptr_ = next_object->object_id_vector_;
                    start_ = ptr_;
                }
            }
            return *this;
        }
        iterator operator++(int) {
            ObjectId *tmp = ptr_;
            ++(*this);
            return iterator(tmp);
        }
        bool operator==(const iterator &rhs) const {
            return rhs.ptr_ == this->ptr_;
        }
        bool operator!=(const iterator &rhs) const {
            return rhs.ptr_ != this->ptr_;
        }

      private:
        ObjectId *ptr_;
        ObjectId *start_;
    };

    iterator begin() const { return iterator((ObjectId *)object_id_vector_); }
    iterator end() const {
        if (tail_count_ == 0 || tail_id_ == 0) {
            return iterator((ObjectId *)&(object_id_vector_[tail_size_]));
        }

        VectorObjectVar<kArraySize> *tail_object =
            MemPool::getObjectPtr<VectorObjectVar<kArraySize> >(tail_id_);
        if (tail_object) {
            return iterator((ObjectId *)(tail_object->object_id_vector_) +
                            tail_size_);
        }
        // return iterator((ObjectId *)tail_vector_ptr_ + tail_size_);
        return iterator(nullptr);
    }

  protected:
    /// @brief copy object
    void copy(VectorObjectVar const &rhs);
    /// @brief move object
    void move(VectorObjectVar &&rhs);
    /// @brief overload output stream
    template <VectorSizeEnum size>
    friend OStreamBase &operator<<(OStreamBase &os,
                                   VectorObjectVar<size> const &rhs);
    /// @brief overload input stream
    // friend IStreamBase &operator>>(IStreamBase &is, VectorObjectVar &rhs) {}
  private:
    ObjectId __getHeaderId() const;
    void __setHeaderId(ObjectId id);
    uint32_t __maxSizePerVector() const;
    bool __hasTail() const;
    bool __isEnd(ObjectId vector_object_id, uint32_t index) const;
    const ObjectId *__getElementInTailVectorObjectVar(uint32_t index) const;
    const ObjectId *__getElementInTailVectorObjectVar(ObjectId cell_id, uint32_t index) const;
    const ObjectId *__getElementByIndex(uint32_t index) const;
    VectorObjectVar<kArraySize> *__appendVectorObjectVar();

    // DATA
    ObjectId header_id_;  // The first vector object ID
    ObjectId tail_id_;    // The last vector object ID
    ObjectId prev_id_;    // The prev vector object ID
    uint32_t
        tail_count_;  // The count of the vector objects following after header
    uint32_t tail_size_;  // The size of the object elements in the last vector
                          // object.
    // ObjectId *tail_vector_ptr_;
    ObjectId object_id_vector_[kArraySize];  // The container of the object IDs.
};

template <VectorSizeEnum kArraySize>
VectorObjectVar<kArraySize>::VectorObjectVar()
    : BaseType(),
      header_id_(0),
      tail_id_(0),
      prev_id_(0),
      tail_count_(0),
      tail_size_(0) {
    // tail_vector_ptr_(nullptr) {
    memset(object_id_vector_, 0, sizeof(ObjectId) * kArraySize);
}

template <VectorSizeEnum kArraySize>
VectorObjectVar<kArraySize>::VectorObjectVar(Object *owner, uint32_t id)
    : BaseType(owner, id) {
    VectorObjectVar();
}

template <VectorSizeEnum kArraySize>
VectorObjectVar<kArraySize>::VectorObjectVar(VectorObjectVar const &rhs) {
    copy(rhs);
}

template <VectorSizeEnum kArraySize>
VectorObjectVar<kArraySize>::VectorObjectVar(VectorObjectVar &&rhs) noexcept {
    move(std::move(rhs));
}

template <VectorSizeEnum kArraySize>
VectorObjectVar<kArraySize> &VectorObjectVar<kArraySize>::operator=(
    VectorObjectVar const &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

template <VectorSizeEnum kArraySize>
VectorObjectVar<kArraySize> &VectorObjectVar<kArraySize>::operator=(
    VectorObjectVar &&rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

template <VectorSizeEnum kArraySize>
uint32_t VectorObjectVar<kArraySize>::memory() const {
    uint32_t ret = this->BaseType::memory();

    ret += sizeof(header_id_);
    ret += sizeof(tail_id_);
    ret += sizeof(prev_id_);
    ret += sizeof(tail_count_);
    ret += sizeof(tail_size_);
    // ret += sizeof (tail_vector_ptr_);
    ret += sizeof(object_id_vector_);

    uint32_t size = ret;  //

    if (tail_count_) {
        ret += size * tail_count_;
    }
    return ret;
}

template <VectorSizeEnum kArraySize>
void VectorObjectVar<kArraySize>::copy(VectorObjectVar const &rhs) {
    this->BaseType::copy(rhs);
    header_id_ = rhs.header_id_;
    tail_id_ = rhs.tail_id_;
    tail_count_ = rhs.tail_count_;
    for (int i = 0; i < kArraySize; ++i) {
        object_id_vector_[i] = rhs.object_id_vector_[i];
    }
}

template <VectorSizeEnum kArraySize>
void VectorObjectVar<kArraySize>::move(VectorObjectVar &&rhs) {
    this->BaseType::move(std::move(rhs));
    header_id_ = std::move(rhs.header_id_);
    tail_id_ = std::move(rhs.tail_id_);
    tail_count_ = std::move(rhs.tail_count_);
    for (int i = 0; i < kArraySize; ++i) {
        object_id_vector_[i] = rhs.object_id_vector_[i];
    }
}

template <VectorSizeEnum kArraySize>
ObjectId VectorObjectVar<kArraySize>::__getHeaderId() const {
    ObjectId header_object_id = this->header_id_;
    if (!header_object_id) {
        return this->getId();
    }
    return header_object_id;
}

template <VectorSizeEnum kArraySize>
bool VectorObjectVar<kArraySize>::__hasTail() const {
    return (tail_count_ > 0) ? true : false;
}

template <VectorSizeEnum kArraySize>
uint32_t VectorObjectVar<kArraySize>::__maxSizePerVector() const {
    return (kArraySize - 1);
}

// @brief
// calculate the capacity of the vector (using for header object).
template <VectorSizeEnum kArraySize>
uint32_t VectorObjectVar<kArraySize>::totalCapacity() const {
    return ((tail_count_ + 1) * __maxSizePerVector());
}

// @brief
// calculate the total size of the vector (using for header object).
template <VectorSizeEnum kArraySize>
uint32_t VectorObjectVar<kArraySize>::totalSize() const {
    return (tail_count_ * __maxSizePerVector() + tail_size_);
}

// @brief
// whether the index inside the specified vector_object indicates the last
// element of the vector
template <VectorSizeEnum kArraySize>
bool VectorObjectVar<kArraySize>::__isEnd(ObjectId vector_object_id,
                                          uint32_t index) const {
    if (tail_count_ > 0 &&
        vector_object_id !=
            tail_id_) {  // the vector_object is between header and tail.
        return false;
    }
    return (index == (tail_size_ - 1));
}

// @brief
// set header Id with the specified Object ID.
// Note that this should be called after Object ID is calculated.
template <VectorSizeEnum kArraySize>
void VectorObjectVar<kArraySize>::__setHeaderId(ObjectId id) {
    header_id_ = id;
}

template <VectorSizeEnum kArraySize>
VectorObjectVar<kArraySize>
    *VectorObjectVar<kArraySize>::createDBVectorObjectVar(bool is_header) {
    VectorObjectVar<kArraySize> *new_object =
        Object::createVectorObject<VectorObjectVar<kArraySize> >();

    if (!new_object) return nullptr;
    if (is_header) {
        new_object->__setHeaderId(new_object->getId());
    }
    return new_object;
}

template <VectorSizeEnum kArraySize>
void VectorObjectVar<kArraySize>::deleteDBVectorObjectVar(ObjectId id) {
    VectorObjectVar<kArraySize> *vector_object =
        addr<VectorObjectVar<kArraySize> >(id);

    if (!vector_object) return;

    if (vector_object->__hasTail()) {
        ObjectId tail_id = vector_object->tail_id_;
        while (tail_id != id) {
            VectorObjectVar<kArraySize> *tail_object =
                addr<VectorObjectVar<kArraySize> >(tail_id);
            if (tail_object == nullptr) {
                return;
            }
            ObjectId prev_id = tail_object->prev_id_;
            Object::deleteVectorObject<VectorObjectVar<kArraySize> >(
                tail_object);
            tail_id = prev_id;
        }
    }
    Object::deleteVectorObject<VectorObjectVar<kArraySize> >(vector_object);
}

// @brief
// append a new vector object to the tail.
template <VectorSizeEnum kArraySize>
VectorObjectVar<kArraySize>
    *VectorObjectVar<kArraySize>::__appendVectorObjectVar() {
    const uint32_t kLastIndex = __maxSizePerVector();
    ObjectId header_object_id = __getHeaderId();

    VectorObjectVar<kArraySize> *header_object =
        addr<VectorObjectVar<kArraySize> >(header_object_id);

    VectorObjectVar<kArraySize> *newvector_object =
        Object::createVectorObject<VectorObjectVar<kArraySize> >();
    ObjectId newtail_id = newvector_object->getId();
    newvector_object->__setHeaderId(header_object_id);

    if (header_object->tail_id_ != 0) {
        VectorObjectVar<kArraySize> *tail_object =
            addr<VectorObjectVar<kArraySize> >(
                header_object->tail_id_);
        tail_object->object_id_vector_[kLastIndex] = newtail_id;
        newvector_object->prev_id_ = tail_id_;
    } else {
        header_object->object_id_vector_[kLastIndex] = newtail_id;
        newvector_object->prev_id_ = header_object_id;
    }
    // header_object->tail_vector_ptr_ = (ObjectId
    // *)newvector_object->object_id_vector_;
    header_object->tail_id_ = newtail_id;
    header_object->tail_count_ += 1;
    header_object->tail_size_ =
        1;  // Set the tail_size to 1; implicitly expect to add one element into
            // the newvector_object.
    return newvector_object;
}

// @brief
// fetch element address with the specified index in tail object.
template <VectorSizeEnum kArraySize>
const ObjectId *VectorObjectVar<kArraySize>::__getElementInTailVectorObjectVar(
    uint32_t index) const {
    ObjectId header_object_id = __getHeaderId();
    VectorObjectVar<kArraySize> *header_object =
        addr<VectorObjectVar<kArraySize> >(header_object_id);
    VectorObjectVar<kArraySize> *tail_object;

    if (header_object->tail_count_ > 0) {
        tail_object = addr<VectorObjectVar<kArraySize> >(
            header_object->tail_id_);
    } else {
        tail_object = header_object;
    }
    return &(tail_object->object_id_vector_[index]);
}

// @brief
// fetch element address with the specified index.
// @return
// the element address
template <VectorSizeEnum kArraySize>
const ObjectId *VectorObjectVar<kArraySize>::__getElementByIndex(
    uint32_t index) const {
    if (index > totalSize()) {  // TODO warning message
        return &(object_id_vector_[0]);
    }
    const uint32_t kLastIndex = __maxSizePerVector();
    uint32_t tail_index = index / kLastIndex;
    uint32_t offset_in_vector = index % kLastIndex;

    if (tail_index > 0 && tail_index == tail_count_) {  // the tail object:
        return __getElementInTailVectorObjectVar(offset_in_vector);
    } else if (tail_index > 0) {  // in the middle of the list:
        ObjectId vector_object_id = this->object_id_vector_[kLastIndex];
        VectorObjectVar<kArraySize> *vector_object;
        // runtime bottleneck:
        for (int i = 0; i < tail_index; ++i) {
            vector_object = addr<VectorObjectVar<kArraySize> >(
                vector_object_id);
            vector_object_id = vector_object->object_id_vector_[kLastIndex];
        }
        return &(vector_object->object_id_vector_[offset_in_vector]);
    } else {  // the header object:
        return &(this->object_id_vector_[index]);
    }
}

// @brief
// operator[]: set element data with index.
// Note that the index should be equal to or lower than the vector size.
template <VectorSizeEnum kArraySize>
ObjectId &VectorObjectVar<kArraySize>::operator[](int index) {
    if (index > totalSize()) {
        // TODO error message.
        return object_id_vector_[0];
    }
    uint32_t tail_index = index / (uint32_t)__maxSizePerVector();
    if (tail_index > tail_count_) {
        VectorObjectVar<kArraySize> *newvector_object =
            __appendVectorObjectVar();
        if (!newvector_object) {
            return object_id_vector_[0];
        }
        return (newvector_object->object_id_vector_)[0];
    } else {
        if (index == totalSize()) {  // inserted new object id into the vector.
            tail_size_ += 1;
        }
        return const_cast<ObjectId &>(*(__getElementByIndex(index)));
    }
}

// @brief
// operator[]: get element data with index.
// Note that the index should be lower than the vector size.
template <VectorSizeEnum kArraySize>
ObjectId VectorObjectVar<kArraySize>::operator[](int index) const {
    if (index >= totalSize()) {
        // TODO error message.
        return object_id_vector_[0];
    }
    ObjectId ret = *(__getElementByIndex(index));
    return ret;
}

// @brief
// push back new Object ID into the vector.
template <VectorSizeEnum kArraySize>
void VectorObjectVar<kArraySize>::push_back(ObjectId id) {
    if (tail_size_ < kArraySize - 1) {
        ObjectId *id_ptr = const_cast<ObjectId *>(
            __getElementInTailVectorObjectVar(tail_size_));
        *id_ptr = id;
        tail_size_ += 1;
    } else {
        VectorObjectVar<kArraySize> *newvector_object =
            __appendVectorObjectVar();
        (newvector_object->object_id_vector_)[0] = id;
    }
}
// @brief
// pop the last element out of the vector.
template <VectorSizeEnum kArraySize>
ObjectId VectorObjectVar<kArraySize>::pop_back() {
    ObjectId ret = 0;

    if (tail_size_ > 0) {
        ObjectId *id_ptr = const_cast<ObjectId *>(
            __getElementInTailVectorObjectVar(tail_size_ - 1));
        ret = *id_ptr;
        *id_ptr = 0;
        tail_size_ -= 1;
    }

    if (tail_size_ == 0 && tail_count_ > 0) {
        // remove the last VectorObjectVar
        VectorObjectVar<kArraySize> *tail_object =
            addr<VectorObjectVar<kArraySize> >(tail_id_);
        ObjectId prev_id = tail_object->prev_id_;

        // TODO: DBObjectOp::deleteDBVectorObjectVar(tail_id_);
        // update the header's information accordingly:
        tail_count_ -= 1;
        tail_id_ = prev_id;
        tail_size_ = __maxSizePerVector();
        // delete:
        Object::deleteVectorObject<VectorObjectVar<kArraySize> >(tail_object);
    }
    return ret;
}

// @brief
// remove element from vector object with the specified Object ID.
// @return
// true if succeeded. false if failed.
template <VectorSizeEnum kArraySize>
bool VectorObjectVar<kArraySize>::removeObjectIdFromVector(ObjectId id) {
    ObjectId vector_object_id = 0;
    uint32_t max_index, index;
    ObjectId header_id = __getHeaderId();
    bool found = false;
    bool is_end = false;
    VectorObjectVar<kArraySize> *vector_object = nullptr;

    if (tail_count_ == 0) {
        vector_object_id = header_id;
    } else {
        vector_object_id = tail_id_;
    }
    while (!found && vector_object_id) {
        vector_object = addr<VectorObjectVar<kArraySize> >(
            vector_object_id);
        if (!vector_object) {
            return false;
        }

        if (vector_object_id == tail_id_) {
            max_index = tail_size_;
        } else {
            max_index = __maxSizePerVector();
        }

        for (index = 0; index < max_index; ++index) {
            ObjectId element_id = vector_object->object_id_vector_[index];
            if (element_id == id) {
                found = true;
                is_end = __isEnd(vector_object_id, index);
                break;
            }
        }
        vector_object_id = vector_object->prev_id_;
    }
    if (found) {
        ObjectId lastObjectId = this->pop_back();
        if (!is_end) {
            vector_object->object_id_vector_[index] = lastObjectId;
        }
    }
    return found;
}

// @brief
// reserve the size for vector_object.
// Note that this function is not recommended to be combined with
// push_back/pop_back().
template <VectorSizeEnum kArraySize>
void VectorObjectVar<kArraySize>::reserve(uint32_t size) {
    if (size < totalCapacity()) return;
    uint32_t tail_count = (size - totalCapacity()) / __maxSizePerVector() + 1;
    ObjectId tail_id_orig = tail_id_;

    while (tail_count) {
        VectorObjectVar<kArraySize> *newvector_object =
            __appendVectorObjectVar();
        --tail_count;
    }
    tail_size_ = 0;  // reset to 0;
}

// @brief
// output overload
template <VectorSizeEnum kArraySize>
OStreamBase &operator<<(OStreamBase &os,
                        VectorObjectVar<kArraySize> const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    os << DataFieldName("header_id_") << rhs.header_id_ << DataDelimiter(" ");
    os << DataFieldName("tail_id_") << rhs.tail_id_ << DataDelimiter(" ");
    os << DataFieldName("prev_id_") << rhs.prev_id_ << DataDelimiter(" ");
    os << DataFieldName("tail_count_") << rhs.tail_count_ << DataDelimiter(" ");
    os << DataFieldName("tail_size_") << rhs.tail_size_ << DataDelimiter(" ");
    if (rhs.tail_count_ > 0) {
        // os << DataFieldName("last_object_id") << *((ObjectId
        // *)rhs.tail_vector_ptr_) << DataDelimiter(" ");
    }
    uint32_t size = rhs.totalSize();
    os << DataFieldName("object_id_vector_") << size << DataBegin("[");
    auto delimiter = DataDelimiter("");
    ObjectId id = rhs.getId();
    uint32_t max_size = rhs.__maxSizePerVector();
    for (int i = 0; i < max_size && i < size; ++i) {
        ObjectId object_id = rhs.object_id_vector_[i];
        os << delimiter << object_id;
        delimiter = DataDelimiter();
    }

    ObjectId next_id = rhs.object_id_vector_[max_size];
    uint32_t tail_count = rhs.tail_count_;

    while (tail_count > 0 && next_id > 0) {
        VectorObjectVar<kArraySize> *vector_object =
            MemPool::getObjectPtr<VectorObjectVar<kArraySize> >(next_id);
        if (!vector_object) break;
        if (tail_count == 1) {  // printing the last vector_object
            max_size = rhs.tail_size_;
        } else {
            next_id = vector_object->object_id_vector_[max_size];
        }
        for (int i = 0; i < max_size; ++i) {
            ObjectId object_id = vector_object->object_id_vector_[i];
            os << delimiter << object_id;
        }
        --tail_count;
    }

    os << DataEnd("]");
    os << DataDelimiter();

    os << DataEnd(")");
    return os;
}

typedef VectorObjectVar<kSize_8> VectorObjectMin;
typedef VectorObjectVar<kSizeHighest> VectorObjectMax;
typedef VectorObjectVar<kSize_8> VectorObject8;
typedef VectorObjectVar<kSize_16> VectorObject16;
typedef VectorObjectVar<kSize_32> VectorObject32;
typedef VectorObjectVar<kSize_64> VectorObject64;
typedef VectorObjectVar<kSize_128> VectorObject128;
typedef VectorObjectVar<kSize_256> VectorObject256;

}  // namespace db
}  // namespace open_edi

#endif
