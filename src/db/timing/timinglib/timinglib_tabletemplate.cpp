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
void TableAxis::set_variable(TableAxisVariable tv) { variable_ = tv; }
void TableAxis::add_value(float f) {
    Cell* topCell = getTopCell();
    if (topCell != nullptr) {
        ArrayObject<float>* object_array = nullptr;
        if (values_ == UNINIT_OBJECT_ID) {
            object_array =
                topCell->createObject<ArrayObject<float>>(kObjectTypeArray);
            if (object_array != nullptr) {
                object_array->setPool(topCell->getPool());
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
TableAxisVariable TableAxis::get_variable(void) { return variable_; }
ArrayObject<float>* TableAxis::get_values(void) {
    if (values_ == UNINIT_OBJECT_ID)
        return nullptr;
    else
        return Object::addr<ArrayObject<float>>(values_);
}
TableAxis::IndexType TableAxis::get_size(void) {
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
    Cell* topCell = getTopCell();
    if (topCell != nullptr) {
        if (axis1_ != UNINIT_OBJECT_ID) {
            TableAxis* t = Object::addr<TableAxis>(axis1_);
            if (t != nullptr) topCell->deleteObject<TableAxis>(t);
        }
        if (axis2_ != UNINIT_OBJECT_ID) {
            TableAxis* t = Object::addr<TableAxis>(axis2_);
            if (t != nullptr) topCell->deleteObject<TableAxis>(t);
        }
        if (axis3_ != UNINIT_OBJECT_ID) {
            TableAxis* t = Object::addr<TableAxis>(axis3_);
            if (t != nullptr) topCell->deleteObject<TableAxis>(t);
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
void TableTemplate::set_name(const std::string& name) {
    Cell* topCell = getTopCell();
    if (topCell) {
        int64_t index = topCell->getOrCreateSymbol(name.c_str());
        if (index != -1) {
            name_ = index;
            topCell->addSymbolReference(name_, this->getId());
        }
    }
}
TableAxis* TableTemplate::create_axis1() {
    Cell* topCell = getTopCell();
    if (topCell != nullptr) {
        TableAxis* t = topCell->createObject<TableAxis>(kObjectTypeTableAxis);
        if (t != nullptr) {
            t->setOwner(this);
            axis1_ = t->getId();
        }
        return t;
    }
    return nullptr;
}
TableAxis* TableTemplate::create_axis2() {
    Cell* topCell = getTopCell();
    if (topCell != nullptr) {
        TableAxis* t = topCell->createObject<TableAxis>(kObjectTypeTableAxis);
        if (t != nullptr) {
            t->setOwner(this);
            axis2_ = t->getId();
        }
        return t;
    }
    return nullptr;
}
TableAxis* TableTemplate::create_axis3() {
    Cell* topCell = getTopCell();
    if (topCell != nullptr) {
        TableAxis* t = topCell->createObject<TableAxis>(kObjectTypeTableAxis);
        if (t != nullptr) {
            t->setOwner(this);
            axis3_ = t->getId();
        }
        return t;
    }
    return nullptr;
}

/// get
std::string TableTemplate::get_name(void) const {
    Cell* topCell = getTopCell();
    if (topCell) {
        return topCell->getSymbolByIndex(name_);
    }
    return "";
}
SymbolIndex TableTemplate::get_name_index(void) { return name_; }
TableAxis* TableTemplate::get_axis1(void) {
    if (axis1_ != UNINIT_OBJECT_ID)
        return Object::addr<TableAxis>(axis1_);
    else
        return nullptr;
}
TableAxis* TableTemplate::get_axis2(void) {
    if (axis2_ != UNINIT_OBJECT_ID)
        return Object::addr<TableAxis>(axis2_);
    else
        return nullptr;
}
TableAxis* TableTemplate::get_axis3(void) {
    if (axis3_ != UNINIT_OBJECT_ID)
        return Object::addr<TableAxis>(axis3_);
    else
        return nullptr;
}

OStreamBase& operator<<(OStreamBase& os, TableTemplate const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    TableTemplate::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("name_") << rhs.get_name() << DataDelimiter();
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
