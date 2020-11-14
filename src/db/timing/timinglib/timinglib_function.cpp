/**
 * @file timinglib_function.cpp
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
#include "db/timing/timinglib/timinglib_function.h"

#include "db/core/db.h"
#include "db/timing/timinglib/timinglib_term.h"

namespace open_edi {
namespace db {

TFunction::TFunction()
    : TFunction::BaseType(),
      func_str_(0),
      op_(FuncOpType::kUnknown),
      tterm_(UNINIT_OBJECT_ID),
      left_(UNINIT_OBJECT_ID),
      right_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeTFunction);
}

TFunction::~TFunction() {}

TFunction::TFunction(Object* owner, TFunction::IndexType id)
    : TFunction::BaseType(owner, id),
      func_str_(0),
      op_(FuncOpType::kUnknown),
      tterm_(UNINIT_OBJECT_ID),
      left_(UNINIT_OBJECT_ID),
      right_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeTFunction);
}

TFunction::TFunction(TFunction const& rhs) { copy(rhs); }

TFunction::TFunction(TFunction&& rhs) noexcept { move(std::move(rhs)); }

TFunction& TFunction::operator=(TFunction const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TFunction& TFunction::operator=(TFunction&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void TFunction::print(std::ostream& stream) {}

void TFunction::copy(TFunction const& rhs) {
    this->BaseType::copy(rhs);

    func_str_ = rhs.func_str_;
    op_ = rhs.op_;
    tterm_ = rhs.tterm_;
    left_ = rhs.left_;
    right_ = rhs.right_;
}

void TFunction::move(TFunction&& rhs) {
    this->BaseType::move(std::move(rhs));

    func_str_ = std::move(rhs.func_str_);
    op_ = std::move(rhs.op_);
    tterm_ = std::move(rhs.tterm_);
    left_ = std::move(rhs.left_);
    right_ = std::move(rhs.right_);
}

TFunction::IndexType TFunction::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(func_str_);
    ret += sizeof(op_);
    ret += sizeof(tterm_);
    ret += sizeof(left_);
    ret += sizeof(right_);

    return ret;
}

/// set
void TFunction::set_func_str(const std::string& str) {
    Cell* topCell = getTopCell();
    if (topCell != nullptr) {
        SymbolIndex idx = topCell->getOrCreateSymbol(str.c_str());
        if (idx != kInvalidSymbolIndex) {
            func_str_ = idx;
            topCell->addSymbolReference(func_str_, this->getId());
        }
    }
}
void TFunction::set_tterm(ObjectId id) { tterm_ = id; }
void TFunction::set_op(FuncOpType t) { op_ = t; }
void TFunction::set_left(ObjectId id) { left_ = id; }
void TFunction::set_right(ObjectId id) { right_ = id; }

/// get
TTerm* TFunction::get_tterm(void) {
    if (tterm_ != UNINIT_OBJECT_ID)
        return addr<TTerm>(tterm_);
    else
        return nullptr;
}
FuncOpType TFunction::get_op(void) { return op_; }
TFunction* TFunction::get_left(void) {
    if (left_ != UNINIT_OBJECT_ID)
        return addr<TFunction>(left_);
    else
        return nullptr;
}
TFunction* TFunction::get_right(void) {
    if (right_ != UNINIT_OBJECT_ID)
        return addr<TFunction>(right_);
    else
        return nullptr;
}

OStreamBase& operator<<(OStreamBase& os, TFunction const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    TFunction::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("func_str_");
    {
        Cell* topCell = getTopCell();
        if (topCell != nullptr && rhs.func_str_ != 0) {
            os << topCell->getSymbolByIndex(rhs.func_str_);
        } else {
            os << "";
        }
        os << DataDelimiter();
    }
    os << DataFieldName("op_") << rhs.op_ << DataDelimiter();
    os << DataFieldName("tterm_") << rhs.tterm_ << DataDelimiter();

    os << DataFieldName("left_") << rhs.left_;
    {
        os << DataBegin("[");
        if (rhs.left_ != UNINIT_OBJECT_ID) {
            auto p = Object::addr<TTerm>(rhs.left_);
            if (p) os << *p;
        }
        os << DataEnd("]") << DataDelimiter();
    }
    os << DataFieldName("right_") << rhs.right_;
    {
        os << DataBegin("[");
        if (rhs.right_ != UNINIT_OBJECT_ID) {
            auto p = Object::addr<TTerm>(rhs.right_);
            if (p) os << *p;
        }
        os << DataEnd("]");
    }

    os << DataEnd(")");
    return os;
}
}  // namespace db
}  // namespace open_edi
