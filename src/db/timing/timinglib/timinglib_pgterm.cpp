/**
 * @file timinglib_pgterm.cpp
 * @date 2020-10-09
 * @brief
 *
 * Copyright (C) 2020 NIIC EDA
 *
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 *
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/timing/timinglib/timinglib_pgterm.h"

#include "db/core/db.h"

namespace open_edi {
namespace db {

TPgTerm::TPgTerm()
    : TPgTerm::BaseType(),
      name_(0),
      pg_type_(PGType::kUnknown),
      voltage_name_(0) {
    setObjectType(ObjectType::kObjectTypeTPgTerm);
}

TPgTerm::~TPgTerm() {}

TPgTerm::TPgTerm(Object* owner, TPgTerm::IndexType id)
    : TPgTerm::BaseType(owner, id),
      name_(0),
      pg_type_(PGType::kUnknown),
      voltage_name_(0) {
    setObjectType(ObjectType::kObjectTypeTPgTerm);
}

TPgTerm::TPgTerm(TPgTerm const& rhs) { copy(rhs); }

TPgTerm::TPgTerm(TPgTerm&& rhs) noexcept { move(std::move(rhs)); }

TPgTerm& TPgTerm::operator=(TPgTerm const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TPgTerm& TPgTerm::operator=(TPgTerm&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void TPgTerm::copy(TPgTerm const& rhs) {
    this->BaseType::copy(rhs);
    name_ = rhs.name_;
    pg_type_ = rhs.pg_type_;
    voltage_name_ = rhs.voltage_name_;
}

void TPgTerm::move(TPgTerm&& rhs) {
    this->BaseType::move(std::move(rhs));

    name_ = std::move(rhs.name_);
    pg_type_ = std::move(rhs.pg_type_);
    voltage_name_ = std::move(rhs.voltage_name_);
}

TPgTerm::IndexType TPgTerm::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(name_);
    ret += sizeof(pg_type_);
    ret += sizeof(voltage_name_);

    return ret;
}

void TPgTerm::print(std::ostream& stream) {
    stream << "TPgTerm name: " << getName() << std::endl;
}

/// set
void TPgTerm::setName(const std::string& name) {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(name.c_str());
        if (index != kInvalidSymbolIndex) {
            name_ = index;
            timing_lib->addSymbolReference(name_, this->getId());
        }
    }
}

void TPgTerm::setPgType(PGType t) { pg_type_ = t; }
void TPgTerm::setVoltageName(const std::string& name) {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(name.c_str());
        if (index != kInvalidSymbolIndex) {
            voltage_name_ = index;
            timing_lib->addSymbolReference(voltage_name_, this->getId());
        }
    }
}

/// get
std::string TPgTerm::getName(void) const {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(name_);
    }
    return "";
}
SymbolIndex TPgTerm::getNameIndex(void) { return name_; }
PGType TPgTerm::getPgType(void) { return pg_type_; }
std::string TPgTerm::getVoltageName(void) const {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(voltage_name_);
    }
    return "";
}

OStreamBase& operator<<(OStreamBase& os, TPgTerm const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    TPgTerm::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("name_") << rhs.getName() << DataDelimiter();
    os << DataFieldName("pg_type_") << rhs.pg_type_ << DataDelimiter();
    os << DataFieldName("voltage_name_") << rhs.getVoltageName();

    os << DataEnd(")");
    return os;
}
}  // namespace db
}  // namespace open_edi
