/**
 * @file timinglib_timingtable.cpp
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
#include "db/timing/timinglib/timinglib_timingtable.h"

#include "db/core/db.h"
#include "db/timing/timinglib/timinglib_tabletemplate.h"

namespace open_edi {
namespace db {

TimingTable::TimingTable() : TimingTable::BaseType() {
    setObjectType(ObjectType::kObjectTypeTimingTable);
}

TimingTable::~TimingTable() {}

TimingTable::TimingTable(Object* owner, TimingTable::IndexType id)
    : TimingTable::BaseType(owner, id) {
    setObjectType(ObjectType::kObjectTypeTimingTable);
}

TimingTable::TimingTable(TimingTable const& rhs) { copy(rhs); }

TimingTable::TimingTable(TimingTable&& rhs) noexcept { move(std::move(rhs)); }

TimingTable& TimingTable::operator=(TimingTable const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TimingTable& TimingTable::operator=(TimingTable&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void TimingTable::copy(TimingTable const& rhs) { this->BaseType::copy(rhs); }

void TimingTable::move(TimingTable&& rhs) {
    this->BaseType::move(std::move(rhs));
}

TimingTable::IndexType TimingTable::memory() const {
    IndexType ret = this->BaseType::memory();

    return ret;
}

TableAxis* TimingTable::getAxis1(void) { return nullptr; }
TableAxis* TimingTable::getAxis2(void) { return nullptr; }
TableAxis* TimingTable::getAxis3(void) { return nullptr; }

OStreamBase& operator<<(OStreamBase& os, TimingTable const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    TimingTable::BaseType const& base = rhs;
    os << base;

    os << DataEnd(")");
    return os;
}

TimingTable0::TimingTable0() : TimingTable0::BaseType(), value_(0.0) {
    setObjectType(ObjectType::kObjectTypeTimingTable0);
}

TimingTable0::~TimingTable0() {}

TimingTable0::TimingTable0(Object* owner, TimingTable0::IndexType id)
    : TimingTable0::BaseType(owner, id), value_(0.0) {
    setObjectType(ObjectType::kObjectTypeTimingTable0);
}

TimingTable0::TimingTable0(TimingTable0 const& rhs) { copy(rhs); }

TimingTable0::TimingTable0(TimingTable0&& rhs) noexcept {
    move(std::move(rhs));
}

TimingTable0& TimingTable0::operator=(TimingTable0 const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TimingTable0& TimingTable0::operator=(TimingTable0&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void TimingTable0::copy(TimingTable0 const& rhs) {
    this->BaseType::copy(rhs);
    value_ = rhs.value_;
}

void TimingTable0::move(TimingTable0&& rhs) {
    this->BaseType::move(std::move(rhs));
    value_ = std::move(rhs.value_);
}

TimingTable0::IndexType TimingTable0::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(value_);

    return ret;
}
void TimingTable0::setValue(float f) { value_ = f; }
float TimingTable0::getValue(void) { return value_; }

OStreamBase& operator<<(OStreamBase& os, TimingTable0 const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    TimingTable0::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("value_") << rhs.value_;

    os << DataEnd(")");
    return os;
}

TimingTable1::TimingTable1()
    : TimingTable1::BaseType(),
      values_(UNINIT_OBJECT_ID),
      axis1_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeTimingTable1);
}

TimingTable1::~TimingTable1() {
#if 0
    Timing* timing_lib = getTimingLib();
    if (timing_lib && axis1_ != UNINIT_OBJECT_ID) {
        TableAxis* t = addr<TableAxis>(axis1_);
        if (t) timing_lib->deleteObject<TableAxis>(t);
    }
#endif
}

TimingTable1::TimingTable1(Object* owner, TimingTable1::IndexType id)
    : TimingTable1::BaseType(owner, id),
      values_(UNINIT_OBJECT_ID),
      axis1_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeTimingTable1);
}

TimingTable1::TimingTable1(TimingTable1 const& rhs) { copy(rhs); }

TimingTable1::TimingTable1(TimingTable1&& rhs) noexcept {
    move(std::move(rhs));
}

TimingTable1& TimingTable1::operator=(TimingTable1 const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TimingTable1& TimingTable1::operator=(TimingTable1&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void TimingTable1::copy(TimingTable1 const& rhs) {
    this->BaseType::copy(rhs);
    values_ = rhs.values_;
    axis1_ = rhs.axis1_;
}

void TimingTable1::move(TimingTable1&& rhs) {
    this->BaseType::move(std::move(rhs));
    values_ = std::move(rhs.values_);
    axis1_ = std::move(rhs.axis1_);
}

TimingTable1::IndexType TimingTable1::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(values_);
    ret += sizeof(axis1_);

    return ret;
}

void TimingTable1::addValue(float f) {
    ArrayObject<float>* p = nullptr;
    if (values_ == UNINIT_OBJECT_ID) {
        Timing* timing_lib = getTimingLib();
        if (timing_lib != nullptr) {
            p = Object::createObject<ArrayObject<float>>(kObjectTypeArray,
                                                         timing_lib->getId());
            if (p != nullptr) {
                values_ = p->getId();
                p->setPool(timing_lib->getPool());
                p->reserve(32);
            }
        }
    } else {
        p = addr<ArrayObject<float>>(values_);
    }
    if (p != nullptr) p->pushBack(f);
}

void TimingTable1::setAxis1(ObjectId id) { axis1_ = id; }

std::vector<float> TimingTable1::getValues(void) {
    std::vector<float> values;
    if (values_ != UNINIT_OBJECT_ID) {
        ArrayObject<float>* p = addr<ArrayObject<float>>(values_);
        if (p != nullptr) {
            for (int64_t i = 0; i < p->getSize(); ++i)
                values.emplace_back((*p)[i]);
        }
    }

    return values;
}

TableAxis* TimingTable1::getAxis1(void) {
    if (axis1_ != UNINIT_OBJECT_ID)
        return addr<TableAxis>(axis1_);
    else
        return nullptr;
}

OStreamBase& operator<<(OStreamBase& os, TimingTable1 const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    TimingTable1::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("values_");
    ArrayObject<float>* p = nullptr;
    if (rhs.values_ != UNINIT_OBJECT_ID)
        p = Object::addr<ArrayObject<float>>(rhs.values_);
    if (p != nullptr)
        os << p->getSize();
    else
        os << 0;
    os << DataBegin("[");
    if (p != nullptr) {
        auto delimiter = DataDelimiter("");
        for (int64_t i = 0; i < p->getSize(); ++i) {
            os << delimiter << (*p)[i];
            delimiter = DataDelimiter();
        }
    }
    os << DataEnd("]") << DataDelimiter();

    os << DataFieldName("axis1_") << rhs.axis1_;
    os << DataBegin("[");
    if (rhs.axis1_ != UNINIT_OBJECT_ID) {
        TableAxis* t = Object::addr<TableAxis>(rhs.axis1_);
        if (t) os << *t;
    }
    os << DataEnd("]");

    os << DataEnd(")");
    return os;
}

TimingTable2::TimingTable2()
    : TimingTable2::BaseType(),
      values_(UNINIT_OBJECT_ID),
      axis1_(UNINIT_OBJECT_ID),
      axis2_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeTimingTable2);
}

TimingTable2::~TimingTable2() {
#if 0
    Timing* timing_lib = getTimingLib();
    if (timing_lib && axis1_ != UNINIT_OBJECT_ID) {
        TableAxis* t = addr<TableAxis>(axis1_);
        if (t) timing_lib->deleteObject<TableAxis>(t);
    }
    if (timing_lib && axis2_ != UNINIT_OBJECT_ID) {
        TableAxis* t = addr<TableAxis>(axis2_);
        if (t) timing_lib->deleteObject<TableAxis>(t);
    }
#endif
}

TimingTable2::TimingTable2(Object* owner, TimingTable2::IndexType id)
    : TimingTable2::BaseType(owner, id),
      values_(UNINIT_OBJECT_ID),
      axis1_(UNINIT_OBJECT_ID),
      axis2_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeTimingTable2);
}

TimingTable2::TimingTable2(TimingTable2 const& rhs) { copy(rhs); }

TimingTable2::TimingTable2(TimingTable2&& rhs) noexcept {
    move(std::move(rhs));
}

TimingTable2& TimingTable2::operator=(TimingTable2 const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TimingTable2& TimingTable2::operator=(TimingTable2&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void TimingTable2::copy(TimingTable2 const& rhs) {
    this->BaseType::copy(rhs);
    values_ = rhs.values_;
    axis1_ = rhs.axis1_;
    axis2_ = rhs.axis2_;
}

void TimingTable2::move(TimingTable2&& rhs) {
    this->BaseType::move(std::move(rhs));
    values_ = std::move(rhs.values_);
    axis1_ = std::move(rhs.axis1_);
    axis2_ = std::move(rhs.axis2_);
}

TimingTable2::IndexType TimingTable2::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(values_);
    ret += sizeof(axis1_);
    ret += sizeof(axis2_);

    return ret;
}

void TimingTable2::addValue(float f) {
    ArrayObject<float>* p = nullptr;
    if (values_ == UNINIT_OBJECT_ID) {
        Timing* timing_lib = getTimingLib();
        if (timing_lib != nullptr) {
            p = Object::createObject<ArrayObject<float>>(kObjectTypeArray,
                                                         timing_lib->getId());
            if (p != nullptr) {
                values_ = p->getId();
                p->setPool(timing_lib->getPool());
                p->reserve(32);
            }
        }
    } else {
        p = addr<ArrayObject<float>>(values_);
    }
    if (p != nullptr) p->pushBack(f);
}
void TimingTable2::setAxis1(ObjectId id) { axis1_ = id; }
void TimingTable2::setAxis2(ObjectId id) { axis2_ = id; }

float TimingTable2::getValue(IndexType index1, IndexType index2) {
    TableAxis* t = getAxis1();
    if (t) {
        ArrayObject<float>* p = nullptr;
        if (values_ != UNINIT_OBJECT_ID) p = addr<ArrayObject<float>>(values_);
        if (p != nullptr) return (*p)[index1 * t->getSize() + index2];
    }
    return 0.0;
}

TableAxis* TimingTable2::getAxis1(void) {
    if (axis1_ != UNINIT_OBJECT_ID)
        return addr<TableAxis>(axis1_);
    else
        return nullptr;
}

TableAxis* TimingTable2::getAxis2(void) {
    if (axis2_ != UNINIT_OBJECT_ID)
        return addr<TableAxis>(axis2_);
    else
        return nullptr;
}

OStreamBase& operator<<(OStreamBase& os, TimingTable2 const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    TimingTable2::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("values_");
    {
        ArrayObject<float>* p = nullptr;
        if (rhs.values_ != UNINIT_OBJECT_ID)
            p = Object::addr<ArrayObject<float>>(rhs.values_);
        if (p != nullptr)
            os << p->getSize();
        else
            os << 0;
        os << DataBegin("[");
        if (p != nullptr) {
            auto delimiter = DataDelimiter("");
            for (int64_t i = 0; i < p->getSize(); ++i) {
                os << delimiter << (*p)[i];
                delimiter = DataDelimiter();
            }
        }
        os << DataEnd("]") << DataDelimiter();
    }

    os << DataFieldName("axis1_") << rhs.axis1_;
    os << DataBegin("[");
    if (rhs.axis1_ != UNINIT_OBJECT_ID) {
        TableAxis* t = Object::addr<TableAxis>(rhs.axis1_);
        if (t) os << *t;
    }
    os << DataEnd("]") << DataDelimiter();

    os << DataFieldName("axis2_") << rhs.axis2_;
    os << DataBegin("[");
    if (rhs.axis2_ != UNINIT_OBJECT_ID) {
        TableAxis* t = Object::addr<TableAxis>(rhs.axis2_);
        if (t) os << *t;
    }
    os << DataEnd("]");

    os << DataEnd(")");
    return os;
}

TimingTable3::TimingTable3()
    : TimingTable3::BaseType(), axis3_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeTimingTable3);
}

TimingTable3::~TimingTable3() {
#if 0
    Timing* timing_lib = getTimingLib();
    if (timing_lib && axis3_ != UNINIT_OBJECT_ID) {
        TableAxis* t = addr<TableAxis>(axis3_);
        if (t) timing_lib->deleteObject<TableAxis>(t);
    }
#endif
}

TimingTable3::TimingTable3(Object* owner, TimingTable3::IndexType id)
    : TimingTable3::BaseType(owner, id), axis3_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeTimingTable3);
}

TimingTable3::TimingTable3(TimingTable3 const& rhs) { copy(rhs); }

TimingTable3::TimingTable3(TimingTable3&& rhs) noexcept {
    move(std::move(rhs));
}

TimingTable3& TimingTable3::operator=(TimingTable3 const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TimingTable3& TimingTable3::operator=(TimingTable3&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void TimingTable3::copy(TimingTable3 const& rhs) {
    this->BaseType::copy(rhs);
    axis3_ = rhs.axis3_;
}

void TimingTable3::move(TimingTable3&& rhs) {
    this->BaseType::move(std::move(rhs));
    axis3_ = std::move(rhs.axis3_);
}

TimingTable3::IndexType TimingTable3::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(axis3_);

    return ret;
}

void TimingTable3::setAxis3(ObjectId id) { axis3_ = id; }
float TimingTable3::getValue(IndexType index1, IndexType index2,
                             IndexType index3) {
    TableAxis* t1 = getAxis1();
    TableAxis* t2 = getAxis2();
    if (t1 && t2) {
        ArrayObject<float>* p = nullptr;
        if (values_ != UNINIT_OBJECT_ID) p = addr<ArrayObject<float>>(values_);
        if (p != nullptr)
            return (
                *p)[(index1 * t1->getSize() + index2) * t2->getSize() + index3];
    }

    return 0.0f;
}

TableAxis* TimingTable3::getAxis3(void) {
    if (axis3_ != UNINIT_OBJECT_ID)
        return addr<TableAxis>(axis3_);
    else
        return nullptr;
}

OStreamBase& operator<<(OStreamBase& os, TimingTable3 const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    TimingTable3::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("axis3_") << rhs.axis3_;
    os << DataBegin("[");
    if (rhs.axis3_ != UNINIT_OBJECT_ID) {
        TableAxis* t = Object::addr<TableAxis>(rhs.axis3_);
        if (t) os << *t;
    }
    os << DataEnd("]");

    os << DataEnd(")");
    return os;
}
}  // namespace db
}  // namespace open_edi
