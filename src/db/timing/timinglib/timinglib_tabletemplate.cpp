/**
 * @file timinglib_tabletemplate.cpp
 * @date 2020-10-9
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
#include "db/timing/timinglib/timinglib_tabletemplate.h"

#include "db/core/db.h"

namespace open_edi {
namespace db {

TableAxis::TableAxis()
    : TableAxis::BaseType(),
      variable_(TableAxisVariable::kUnknown),
      values_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeTableAxis);
}

TableAxis::~TableAxis() { values_ = UNINIT_OBJECT_ID; }

TableAxis::TableAxis(Object* owner, TableAxis::IndexType id)
    : TableAxis::BaseType(owner, id),
      variable_(TableAxisVariable::kUnknown),
      values_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeTableAxis);
}

TableAxis::TableAxis(TableAxis const& rhs) { copy(rhs); }

TableAxis::TableAxis(TableAxis&& rhs) noexcept { move(std::move(rhs)); }

TableAxis& TableAxis::operator=(TableAxis const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TableAxis& TableAxis::operator=(TableAxis&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void TableAxis::copy(TableAxis const& rhs) {
    this->BaseType::copy(rhs);

    variable_ = rhs.variable_;
    values_ = rhs.values_;
}

void TableAxis::move(TableAxis&& rhs) {
    this->BaseType::move(std::move(rhs));

    variable_ = std::move(rhs.variable_);
    values_ = std::move(rhs.values_);
}

TableAxis::IndexType TableAxis::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(variable_);
    ret += sizeof(values_);

    return ret;
}

/// set
void TableAxis::setVariable(TableAxisVariable tv) { variable_ = tv; }
void TableAxis::addValue(float f) {
    Timing* timing_lib = getTimingLib();
    if (timing_lib != nullptr) {
        ArrayObject<float>* object_array = nullptr;
        if (values_ == UNINIT_OBJECT_ID) {
            object_array = Object::createObject<ArrayObject<float>>(
                kObjectTypeArray, timing_lib->getId());
            if (object_array != nullptr) {
                object_array->setPool(timing_lib->getPool());
                object_array->reserve(32);
                values_ = object_array->getId();
            }
        } else {
            object_array = Object::addr<ArrayObject<float>>(values_);
        }
        if (object_array != nullptr) object_array->pushBack(f);
    }
}

/// get
TableAxisVariable TableAxis::getVariable(void) { return variable_; }
ArrayObject<float>* TableAxis::getValues(void) {
    if (values_ == UNINIT_OBJECT_ID)
        return nullptr;
    else
        return Object::addr<ArrayObject<float>>(values_);
}
TableAxis::IndexType TableAxis::getSize(void) {
    if (values_ != UNINIT_OBJECT_ID) {
        auto p = Object::addr<ArrayObject<float>>(values_);
        return p->getSize();
    }
    return 0;
}

OStreamBase& operator<<(OStreamBase& os, TableAxis const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    TableAxis::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("variable_") << rhs.variable_ << DataDelimiter();
    os << DataFieldName("values_");
    ArrayObject<float>* object_array = nullptr;
    if (rhs.values_ != UNINIT_OBJECT_ID)
        object_array = Object::addr<ArrayObject<float>>(rhs.values_);
    if (object_array != nullptr)
        os << object_array->getSize();
    else
        os << "0";
    os << DataBegin("[");
    if (object_array) {
        auto delimiter = DataDelimiter("");
        for (int64_t i = 0; i < object_array->getSize(); ++i) {
            os << delimiter << (*object_array)[i];
            delimiter = DataDelimiter();
        }
    }
    os << DataEnd("]");

    os << DataEnd(")");
    return os;
}

TableTemplate::TableTemplate()
    : TableTemplate::BaseType(),
      name_(0),
      axis1_(UNINIT_OBJECT_ID),
      axis2_(UNINIT_OBJECT_ID),
      axis3_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeTableTemplate);
}

TableTemplate::~TableTemplate() {
#if 0
    Timing* timing_lib = getTimingLib();
    if (timing_lib != nullptr) {
        if (axis1_ != UNINIT_OBJECT_ID) {
            TableAxis* t = Object::addr<TableAxis>(axis1_);
            if (t != nullptr) timing_lib->deleteObject<TableAxis>(t);
        }
        if (axis2_ != UNINIT_OBJECT_ID) {
            TableAxis* t = Object::addr<TableAxis>(axis2_);
            if (t != nullptr) timing_lib->deleteObject<TableAxis>(t);
        }
        if (axis3_ != UNINIT_OBJECT_ID) {
            TableAxis* t = Object::addr<TableAxis>(axis3_);
            if (t != nullptr) timing_lib->deleteObject<TableAxis>(t);
        }
    }
#endif
}

TableTemplate::TableTemplate(Object* owner, TableTemplate::IndexType id)
    : TableTemplate::BaseType(owner, id),
      name_(0),
      axis1_(UNINIT_OBJECT_ID),
      axis2_(UNINIT_OBJECT_ID),
      axis3_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeTableTemplate);
}

TableTemplate::TableTemplate(TableTemplate const& rhs) { copy(rhs); }

TableTemplate::TableTemplate(TableTemplate&& rhs) noexcept {
    move(std::move(rhs));
}

TableTemplate& TableTemplate::operator=(TableTemplate const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TableTemplate& TableTemplate::operator=(TableTemplate&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void TableTemplate::copy(TableTemplate const& rhs) {
    this->BaseType::copy(rhs);

    name_ = rhs.name_;
    axis1_ = rhs.axis1_;
    axis2_ = rhs.axis2_;
    axis3_ = rhs.axis3_;
}

void TableTemplate::move(TableTemplate&& rhs) {
    this->BaseType::move(std::move(rhs));

    name_ = std::move(rhs.name_);
    rhs.name_ = 0;
    axis1_ = std::move(rhs.axis1_);
    rhs.axis1_ = UNINIT_OBJECT_ID;
    axis2_ = std::move(rhs.axis2_);
    rhs.axis2_ = UNINIT_OBJECT_ID;
    axis3_ = std::move(rhs.axis3_);
    rhs.axis3_ = UNINIT_OBJECT_ID;
}

TableTemplate::IndexType TableTemplate::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(name_);
    ret += sizeof(axis1_);
    ret += sizeof(axis2_);
    ret += sizeof(axis3_);

    return ret;
}

/// set
void TableTemplate::setName(const std::string& name) {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(name.c_str());
        if (index != kInvalidSymbolIndex) {
            name_ = index;
            timing_lib->addSymbolReference(name_, this->getId());
        }
    }
}
TableAxis* TableTemplate::createAxis1() {
    Timing* timing_lib = getTimingLib();
    if (timing_lib != nullptr) {
        TableAxis* t = Object::createObject<TableAxis>(kObjectTypeTableAxis,
                                                       timing_lib->getId());
        if (t != nullptr) {
            t->setOwner(this);
            axis1_ = t->getId();
        }
        return t;
    }
    return nullptr;
}
TableAxis* TableTemplate::createAxis2() {
    Timing* timing_lib = getTimingLib();
    if (timing_lib != nullptr) {
        TableAxis* t = Object::createObject<TableAxis>(kObjectTypeTableAxis,
                                                       timing_lib->getId());
        if (t != nullptr) {
            t->setOwner(this);
            axis2_ = t->getId();
        }
        return t;
    }
    return nullptr;
}
TableAxis* TableTemplate::createAxis3() {
    Timing* timing_lib = getTimingLib();
    if (timing_lib != nullptr) {
        TableAxis* t = Object::createObject<TableAxis>(kObjectTypeTableAxis,
                                                       timing_lib->getId());
        if (t != nullptr) {
            t->setOwner(this);
            axis3_ = t->getId();
        }
        return t;
    }
    return nullptr;
}

/// get
std::string TableTemplate::getName(void) const {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(name_);
    }
    return "";
}
SymbolIndex TableTemplate::getNameIndex(void) { return name_; }
TableAxis* TableTemplate::getAxis1(void) {
    if (axis1_ != UNINIT_OBJECT_ID)
        return Object::addr<TableAxis>(axis1_);
    else
        return nullptr;
}
TableAxis* TableTemplate::getAxis2(void) {
    if (axis2_ != UNINIT_OBJECT_ID)
        return Object::addr<TableAxis>(axis2_);
    else
        return nullptr;
}
TableAxis* TableTemplate::getAxis3(void) {
    if (axis3_ != UNINIT_OBJECT_ID)
        return Object::addr<TableAxis>(axis3_);
    else
        return nullptr;
}

OStreamBase& operator<<(OStreamBase& os, TableTemplate const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    TableTemplate::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("name_") << rhs.getName() << DataDelimiter();
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
    os << DataEnd("]") << DataDelimiter();

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
