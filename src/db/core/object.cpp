/* @file  object.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "db/core/object.h"

namespace open_edi {
namespace db {

/// @brief Object 
Object::Object() {
    setId(UNINIT_OBJECT_ID);
    owner_ = 0;
}

/// @brief Object 
///
/// @param o
/// @param id
Object::Object(Object* o, IndexType id) : id_(id) {
    owner_ = o->getId();
}

/// @brief Object 
///
/// @param rhs
Object::Object(Object const& rhs) { copy(rhs); }

/// @brief Object 
///
/// @param rhs
Object::Object(Object&& rhs) { move(std::move(rhs)); }

/// @brief operator= 
///
/// @param rhs
///
/// @return 
Object& Object::operator=(Object const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

/// @brief operator= 
///
/// @param rhs
///
/// @return 
Object& Object::operator=(Object&& rhs) {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

/// @brief ~Object 
Object::~Object() {}

/// @brief memory 
///
/// @return 
Object::IndexType Object::memory() const { return sizeof(*this); }

/// @brief copy 
///
/// @param rhs
void Object::copy(Object const& rhs) {
    owner_ = rhs.owner_;
    id_ = rhs.id_;
}

/// @brief move 
///
/// @param rhs
void Object::move(Object&& rhs) {
    owner_ = std::move(rhs.owner_);
    // owner_ = std::exchange(rhs.owner_, nullptr);
    id_ = std::move(static_cast<unsigned long>(rhs.id_));
}

/// @brief setIsValid 
///
/// @param v
void Object::setIsValid(int v) {is_valid_ = v;}

/// @brief getIsValid 
///
/// @return 
int Object::getIsValid() {return is_valid_;}

/// @brief setIsSelected
///
/// @param v
void Object::setIsSelected(int v) {is_selected_ = v;}

/// @brief getIsSelected
///
/// @return 
int Object::getIsSelected() {return is_selected_;}

/// @brief setIsHighlight 
///
/// @param v
void Object::setIsHighlight(int v) {is_highlight_ = v;}

/// @brief getIsHighlight 
///
/// @return 
int Object::getIsHighlight() {return is_highlight_;}

/// @brief setIsModified 
///
/// @param v
void Object::setIsModified(int v) {is_modified_ = v;}

/// @brief getIsModified 
///
/// @return 
int Object::getIsModified() {return is_modified_;}

/// @brief setIsMarked 
///
/// @param v
void Object::setIsMarked(int v) {is_marked_ = v;}

/// @brief getIsMarked 
///
/// @return 
int Object::getIsMarked() {return is_marked_;}

/// @brief operator<< 
///
/// @param os
/// @param rhs
///
/// @return 
OStreamBase& operator<<(OStreamBase& os, Object const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    os << DataFieldName("owner_");
    if (rhs.owner_) {
        os << rhs.owner_;
        // os << rhs.owner_->getId();
    } else {
        os << UNINIT_OBJECT_ID;
    }
    os << DataDelimiter();
    os << DataFieldName("id_") << rhs.id_ << DataDelimiter();

    os << DataEnd(")");
    return os;
}

void Object::deleteSparseObject(ObjectType type) {
    kSparseMap.erase(IdType(getId(), type));
}

void Object::deleteSparseObject(ObjectType type, ObjectId obj_id) {
    kSparsePair = kSparseMap.equal_range(IdType(getId(), type));
    for (kSparseIt = kSparsePair.first; kSparseIt != kSparsePair.second;
                                                             ++kSparseIt) {
        if (obj_id == kSparseIt->second) {
            kSparseMap.erase(kSparseIt);
            break;
        }
    }
}

/// @brief operator>> 
///
/// @param is
/// @param rhs
///
/// @return 
IStreamBase& operator>>(IStreamBase& is, Object& rhs) {
    is >> DataTypeName(className(rhs)) >> DataBegin("(");

    auto context = is.getContext();

    Object::IndexType owner_id = rhs.owner_;
    Object::IndexType id = rhs.id_;
    is >> DataFieldName("owner_") >> owner_id >> DataDelimiter();
    is >> DataFieldName("id_") >> id >> DataDelimiter();

    if (context) {
        ediAssertMsg(owner_id == context->getId(),
                     "owner id %u mismatch %u in object %u", owner_id,
                     context->getId(), rhs.getId());
    } else {
        ediAssertMsg(owner_id == UNINIT_OBJECT_ID,
                     "owner %u mismatch nullptr in object %u", owner_id,
                     rhs.getId());
    }

    // read owner_
    rhs.owner_ = context->getId();

    is >> DataEnd(")");
    return is;
}

// Store sparse data for all Object
SparseMap kSparseMap;
SparsePair kSparsePair;
SparseMap::iterator kSparseIt;


}  // namespace db
}  // namespace open_edi
