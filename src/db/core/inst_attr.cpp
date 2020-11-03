/* @file  inst_attr.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "db/core/inst_attr.h"

namespace open_edi {
namespace db {

InstAttr::InstAttr() : InstAttr::BaseType(), loc_() {}

InstAttr::InstAttr(InstAttr const &rhs) { copy(rhs); }

InstAttr::InstAttr(InstAttr &&rhs) noexcept { move(std::move(rhs)); }

InstAttr &InstAttr::operator=(InstAttr const &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

InstAttr &InstAttr::operator=(InstAttr &&rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

InstAttr::IndexType InstAttr::memory() const {
    IndexType ret = this->BaseType::memory();
    ret += sizeof(loc_);  // using util/Point instead of geo/Point.
    // ret +=  loc_.memory();
    return ret;
}

InstAttr *InstAttr::clone() const { return new InstAttr(*this); }

void InstAttr::copy(InstAttr const &rhs) {
    name_ = rhs.name_;
    loc_ = rhs.loc_;
}

void InstAttr::move(InstAttr &&rhs) {
    name_ = std::move(rhs.name_);
    loc_ = std::move(rhs.loc_);
}

OStreamBase &operator<<(OStreamBase &os, InstAttr const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    os << DataFieldName("name_") << rhs.name_ << DataDelimiter();
    os << DataFieldName("loc_") << rhs.loc_.getX() << DataDelimiter()
       << rhs.loc_.getY();
    ;

    os << DataEnd(")");
    return os;
}

IStreamBase &operator>>(IStreamBase &is, InstAttr &rhs) {
    is >> DataTypeName(className(rhs)) >> DataBegin("(");

    // read name_
    is >> DataFieldName("name_") >> rhs.name_ >> DataDelimiter();

    // read loc_
    int x = 0;
    int y = 0;

    is >> DataFieldName("loc_") >> x >> DataDelimiter() >> y;
    rhs.loc_.setX(x);
    rhs.loc_.setY(y);

    is >> DataEnd(")");
    return is;
}

}  // namespace db
}  // namespace open_edi
