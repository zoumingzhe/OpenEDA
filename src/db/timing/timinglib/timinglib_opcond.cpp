/**
 * @file timinglib_opcond.cpp
 * @date 2020-08-26
 * @brief OperatingConditions Class
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/timing/timinglib/timinglib_opcond.h"

#include "db/core/db.h"

namespace open_edi {
namespace db {

OperatingConditions::OperatingConditions()
    : OperatingConditions::BaseType(),
      name_(0),
      wire_load_tree_(WireLoadTree::kUnknown) {
    setObjectType(ObjectType::kObjectTypeOperatingConditions);
}

OperatingConditions::~OperatingConditions() {}

OperatingConditions::OperatingConditions(Object* owner,
                                         OperatingConditions::IndexType id)
    : OperatingConditions::BaseType(owner, id),
      name_(0),
      wire_load_tree_(WireLoadTree::kUnknown) {
    setObjectType(ObjectType::kObjectTypeOperatingConditions);
}

OperatingConditions::OperatingConditions(OperatingConditions const& rhs) {
    copy(rhs);
}

OperatingConditions::OperatingConditions(OperatingConditions&& rhs) noexcept {
    move(std::move(rhs));
}

OperatingConditions& OperatingConditions::operator=(
    OperatingConditions const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

OperatingConditions& OperatingConditions::operator=(
    OperatingConditions&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void OperatingConditions::print(std::ostream& stream) {
    stream << "OperatingConditions: name: " << get_name()
           << " wire_load_tree: " << toString(wire_load_tree_);
    this->BaseType::print(stream);
}

void OperatingConditions::copy(OperatingConditions const& rhs) {
    this->BaseType::copy(rhs);
    name_ = rhs.name_;
    wire_load_tree_ = rhs.wire_load_tree_;
}

void OperatingConditions::move(OperatingConditions&& rhs) {
    this->BaseType::move(std::move(rhs));
    name_ = std::move(rhs.name_);
    wire_load_tree_ = std::move(rhs.wire_load_tree_);
}

OperatingConditions::IndexType OperatingConditions::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(name_);
    ret += sizeof(wire_load_tree_);

    return ret;
}

void OperatingConditions::set_name(const std::string& name) {
    Cell* topCell = getTopCell();
    if (topCell) {
        SymbolIndex index = topCell->getOrCreateSymbol(name.c_str());
        if (index != kInvalidSymbolIndex) {
            name_ = index;
            topCell->addSymbolReference(name_, this->getId());
        }
    }
}
void OperatingConditions::set_wire_load_tree(WireLoadTree ty) {
    wire_load_tree_ = ty;
}
std::string OperatingConditions::get_name(void) const {
    Cell* topCell = getTopCell();
    if (topCell) return topCell->getSymbolByIndex(name_);
    return "";
}
SymbolIndex OperatingConditions::get_name_index(void) { return name_; }
WireLoadTree OperatingConditions::get_wire_load_tree(void) {
    return wire_load_tree_;
}

OStreamBase& operator<<(OStreamBase& os, OperatingConditions const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    OperatingConditions::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("name_") << rhs.get_name() << DataDelimiter();
    os << DataFieldName("wire_load_tree_") << rhs.wire_load_tree_;

    os << DataEnd(")");
    return os;
}
}  // namespace db
}  // namespace open_edi
