/* @file  attr_object.hpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_DB_ATTR_OBJECT_HPP_
#define EDI_DB_ATTR_OBJECT_HPP_

#include "db/core/object.h"
#include "db/core/object_attr.h"

namespace open_edi {
namespace db {

/// @brief Base class for object with attributes
template <typename T>
class AttrObject : public Object {
  public:
    using BaseType = Object;
    using AttrType = T;

    /// @brief default constructor
    AttrObject() : BaseType(), attr_(nullptr) {}

    /// @brief constructor
    AttrObject(Object* owner, IndexType id)
        : BaseType(owner, id), attr_(nullptr) {}

    /// @brief copy constructor
    AttrObject(AttrObject const& rhs) { copy(rhs); }

    /// @brief move constructor
    AttrObject(AttrObject&& rhs) { move(std::move(rhs)); }

    /// @brief copy assignment
    AttrObject& operator=(AttrObject const& rhs) {
        if (this != &rhs) {
            copy(rhs);
        }
        return *this;
    }

    /// @brief move assignment
    AttrObject& operator=(AttrObject&& rhs) {
        if (this != &rhs) {
            move(std::move(rhs));
        }
        return *this;
    }

    /// @brief destructor
    ~AttrObject() { clear(); }

    /// @brief getter for attributes
    AttrType const* getAttr() const { return attr_; }

    /// @brief getter for attributes
    AttrType* getAttr() { return attr_; }

    /// @brief setter for attributes.
    void setAttr(AttrType* attr) {
        if (attr_) {
            delete attr_;
        }
        // we need to new a attr here, not just assignment:
        // attr_ = attr;
        attr_ = new AttrType(*attr);
    }

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const { return sizeof(attr_) + attr_->memory(); }

  protected:
    /// @brief copy object
    void copy(AttrObject const& rhs) {
        clear();
        this->BaseType::copy(rhs);
        attr_ = rhs.attr_->clone();
    }
    /// @brief move object
    void move(AttrObject&& rhs) {
        this->BaseType::move(std::move(rhs));
        attr_ = std::exchange(rhs.attr_, nullptr);
    }
    /// @brief clear object
    void clear() {
        if (attr_) {
            delete attr_;
        }
    }
    OStreamBase& writeAttr(OStreamBase& os) const {
        os << DataFieldName("attr_");
        // put an indicator for whether attribute exists or not
        // use '(nil)' to indicate attr_ doesn't exist. -- luoying
        if (this->attr_) {
            // os << (uint8_t)1;
            os << *this->attr_;
        } else {
            os << "(nil)";
            // os << (uint8_t)0;
        }

        return os;
    }
    IStreamBase& readAttr(IStreamBase& is) {
        // add current context to AttrObject
        is.addContext(this);

        is >> DataFieldName("attr_");
        uint8_t has_attr = 0;
        is >> has_attr;
        if (has_attr) {
            this->clear();
            this->attr_ = new AttrObject::AttrType;
            is >> *this->attr_;
        }

        // pop current context
        is.popContext();

        return is;
    }
    /// @brief overload output stream
    friend OStreamBase& operator<<(OStreamBase& os, AttrObject const& rhs) {
        os << DataTypeName(className(rhs)) << DataBegin("(");

        AttrObject::BaseType const& base = rhs;
        os << base << DataDelimiter();
        rhs.writeAttr(os);

        os << DataEnd(")");
        return os;
    }
    /// @brief overload input stream
    friend IStreamBase& operator>>(IStreamBase& is, AttrObject& rhs) {
        is >> DataTypeName(className(rhs)) >> DataBegin("(");

        AttrObject::BaseType& base = rhs;
        is >> base >> DataDelimiter();
        rhs.readAttr(is);

        is >> DataEnd(")");
        return is;
    }

    AttrType* attr_;  ///< attributes of the object
};

}  // namespace db
}  // namespace open_edi

#endif
