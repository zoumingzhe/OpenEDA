/**
 * @file timinglib_timingarc.cpp
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
#include "db/timing/timinglib/timinglib_timingarc.h"

#include "db/core/db.h"
#include "db/timing/timinglib/timinglib_function.h"
#include "db/timing/timinglib/timinglib_term.h"
#include "db/timing/timinglib/timinglib_timingtable.h"

namespace open_edi {
namespace db {

TimingArc::TimingArc()
    : TimingArc::BaseType(),
      is_disabled_(false),
      timing_sense_(TimingSense::kUnknown),
      timing_type_(TimingType::kUnknown),
      when_(UNINIT_OBJECT_ID),
      cell_rise_(UNINIT_OBJECT_ID),
      cell_fall_(UNINIT_OBJECT_ID),
      rise_transition_(UNINIT_OBJECT_ID),
      fall_transition_(UNINIT_OBJECT_ID),
      rise_constraint_(UNINIT_OBJECT_ID),
      fall_constraint_(UNINIT_OBJECT_ID),
      related_pins_(UNINIT_OBJECT_ID),
      related_pins_map_() {
    setObjectType(ObjectType::kObjectTypeTimingArc);
}

TimingArc::~TimingArc() {}

TimingArc::TimingArc(Object* owner, TimingArc::IndexType id)
    : TimingArc::BaseType(owner, id),
      is_disabled_(false),
      timing_sense_(TimingSense::kUnknown),
      timing_type_(TimingType::kUnknown),
      when_(UNINIT_OBJECT_ID),
      cell_rise_(UNINIT_OBJECT_ID),
      cell_fall_(UNINIT_OBJECT_ID),
      rise_transition_(UNINIT_OBJECT_ID),
      fall_transition_(UNINIT_OBJECT_ID),
      rise_constraint_(UNINIT_OBJECT_ID),
      fall_constraint_(UNINIT_OBJECT_ID),
      related_pins_(UNINIT_OBJECT_ID),
      related_pins_map_() {
    setObjectType(ObjectType::kObjectTypeTimingArc);
}

TimingArc::TimingArc(TimingArc const& rhs) { copy(rhs); }

TimingArc::TimingArc(TimingArc&& rhs) noexcept { move(std::move(rhs)); }

TimingArc& TimingArc::operator=(TimingArc const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TimingArc& TimingArc::operator=(TimingArc&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void TimingArc::copy(TimingArc const& rhs) {
    this->BaseType::copy(rhs);

    is_disabled_ = rhs.is_disabled_;
    timing_sense_ = rhs.timing_sense_;
    timing_type_ = rhs.timing_type_;
    when_ = rhs.when_;
    cell_rise_ = rhs.cell_rise_;
    cell_fall_ = rhs.cell_fall_;
    rise_transition_ = rhs.rise_transition_;
    fall_transition_ = rhs.fall_transition_;
    rise_constraint_ = rhs.rise_constraint_;
    fall_constraint_ = rhs.fall_constraint_;
    related_pins_ = rhs.related_pins_;
    related_pins_map_ = rhs.related_pins_map_;
}

void TimingArc::move(TimingArc&& rhs) {
    this->BaseType::move(std::move(rhs));

    is_disabled_ = std::move(rhs.is_disabled_);
    timing_sense_ = std::move(rhs.timing_sense_);
    timing_type_ = std::move(rhs.timing_type_);
    when_ = std::move(rhs.when_);
    cell_rise_ = std::move(rhs.cell_rise_);
    cell_fall_ = std::move(rhs.cell_fall_);
    rise_transition_ = std::move(rhs.rise_transition_);
    fall_transition_ = std::move(rhs.fall_transition_);
    rise_constraint_ = std::move(rhs.rise_constraint_);
    fall_constraint_ = std::move(rhs.fall_constraint_);
    related_pins_ = std::move(rhs.related_pins_);
    related_pins_map_ = std::move(rhs.related_pins_map_);
}

TimingArc::IndexType TimingArc::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(is_disabled_);
    ret += sizeof(timing_sense_);
    ret += sizeof(timing_type_);
    ret += sizeof(when_);
    ret += sizeof(cell_rise_);
    ret += sizeof(cell_fall_);
    ret += sizeof(rise_transition_);
    ret += sizeof(fall_transition_);
    ret += sizeof(rise_constraint_);
    ret += sizeof(fall_constraint_);
    ret += sizeof(related_pins_);

    return ret;
}

/// set
void TimingArc::setDisabled(bool b) { is_disabled_ = b; }
void TimingArc::setTimingSense(TimingSense ts) { timing_sense_ = ts; }
void TimingArc::setTimingType(TimingType tt) {}
TFunction* TimingArc::setWhen(const std::string& str) {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        auto p = Object::createObject<TFunction>(kObjectTypeTFunction,
                                                 timing_lib->getId());
        if (p) {
            when_ = p->getId();
            p->setFuncStr(str);
            p->setOwner(this);
        }
        return p;
    }
    return nullptr;
}
TimingTable* TimingArc::createTimingTable(ObjectType type) {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        TimingTable* p = nullptr;
        if (type == kObjectTypeTimingTable)
            p = Object::createObject<TimingTable>(kObjectTypeTimingTable,
                                                  timing_lib->getId());
        else if (type == kObjectTypeTimingTable0)
            p = Object::createObject<TimingTable0>(kObjectTypeTimingTable0,
                                                   timing_lib->getId());
        else if (type == kObjectTypeTimingTable1)
            p = Object::createObject<TimingTable1>(kObjectTypeTimingTable1,
                                                   timing_lib->getId());
        else if (type == kObjectTypeTimingTable2)
            p = Object::createObject<TimingTable2>(kObjectTypeTimingTable2,
                                                   timing_lib->getId());
        else if (type == kObjectTypeTimingTable3)
            p = Object::createObject<TimingTable3>(kObjectTypeTimingTable3,
                                                   timing_lib->getId());
        return p;
    }
    return nullptr;
}
void TimingArc::setCellRise(ObjectId id) { cell_rise_ = id; }
void TimingArc::setCellFall(ObjectId id) { cell_fall_ = id; }
void TimingArc::setRiseTransition(ObjectId id) { rise_transition_ = id; }
void TimingArc::setFallTransition(ObjectId id) { fall_transition_ = id; }
void TimingArc::setRiseConstraint(ObjectId id) { rise_constraint_ = id; }
void TimingArc::setFallConstraint(ObjectId id) { fall_constraint_ = id; }
void TimingArc::addRelatedPin(ObjectId id) {
    if (id == UNINIT_OBJECT_ID) return;
    auto pin = Object::addr<TTerm>(id);
    if (pin) {
        ArrayObject<ObjectId>* p = nullptr;
        if (related_pins_ == UNINIT_OBJECT_ID) {
            Timing* timing_lib = getTimingLib();
            if (timing_lib != nullptr) {
                p = Object::createObject<ArrayObject<ObjectId>>(
                    kObjectTypeArray, timing_lib->getId());
                if (p != nullptr) {
                    related_pins_ = p->getId();
                    p->setPool(timing_lib->getPool());
                    p->reserve(32);
                }
            }
        } else {
            p = Object::addr<ArrayObject<ObjectId>>(related_pins_);
        }
        if (p != nullptr) {
            p->pushBack(id);
            related_pins_map_[pin->getNameIndex()] = id;
        }
    }
}

/// get
bool TimingArc::isDisabled(void) { return is_disabled_; }
TimingSense TimingArc::getTimingSense(void) { return timing_sense_; }
TimingType TimingArc::getTimingType(void) { return timing_type_; }
TFunction* TimingArc::getWhen(void) {
    if (when_ != UNINIT_OBJECT_ID)
        return Object::addr<TFunction>(when_);
    else
        return nullptr;
}
TimingTable* TimingArc::getCellRise(void) {
    if (cell_rise_ != UNINIT_OBJECT_ID)
        return Object::addr<TimingTable>(cell_rise_);
    else
        return nullptr;
}
TimingTable* TimingArc::getCellFall(void) {
    if (cell_fall_ != UNINIT_OBJECT_ID)
        return Object::addr<TimingTable>(cell_fall_);
    else
        return nullptr;
}
TimingTable* TimingArc::getRiseTransition(void) {
    if (rise_transition_ != UNINIT_OBJECT_ID)
        return Object::addr<TimingTable>(rise_transition_);
    else
        return nullptr;
}
TimingTable* TimingArc::getFallTransition(void) {
    if (fall_transition_ != UNINIT_OBJECT_ID)
        return Object::addr<TimingTable>(fall_transition_);
    else
        return nullptr;
}
TimingTable* TimingArc::getRiseConstraint(void) {
    if (rise_constraint_ != UNINIT_OBJECT_ID)
        return Object::addr<TimingTable>(rise_constraint_);
    else
        return nullptr;
}
TimingTable* TimingArc::getFallConstraint(void) {
    if (fall_constraint_ != UNINIT_OBJECT_ID)
        return Object::addr<TimingTable>(fall_constraint_);
    else
        return nullptr;
}
TTerm* TimingArc::getRelatedPin(const std::string& name) {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex id = timing_lib->getOrCreateSymbol(name.c_str());
        if (id != kInvalidSymbolIndex) {
            auto p = related_pins_map_.find(id);
            if (p != related_pins_map_.end()) return getRelatedPin(p->second);
        }
    }
    return nullptr;
}
TTerm* TimingArc::getRelatedPin(ObjectId id) {
    if (id != UNINIT_OBJECT_ID)
        return Object::addr<TTerm>(id);
    else
        return nullptr;
}
std::vector<TTerm*> TimingArc::getRelatedPins(void) {
    std::vector<TTerm*> relatedPins;
    if (related_pins_ != UNINIT_OBJECT_ID) {
        ArrayObject<ObjectId>* p = nullptr;
        p = Object::addr<ArrayObject<ObjectId>>(related_pins_);
        if (p != nullptr) {
            int64_t count = p->getSize();
            relatedPins.reserve(count);
            for (int64_t i = 0; i < count; ++i) {
                auto q = Object::addr<TTerm>((*p)[i]);
                if (q != nullptr) relatedPins.emplace_back(q);
            }
        }
    }
    return relatedPins;
}
ObjectType getTimingtableObjectType(const std::string& str) {
    if (str == "kObjectTypeTimingTable")
        return kObjectTypeTimingTable;
    else if (str == "kObjectTypeTimingTable0")
        return kObjectTypeTimingTable0;
    else if (str == "kObjectTypeTimingTable1")
        return kObjectTypeTimingTable1;
    else if (str == "kObjectTypeTimingTable2")
        return kObjectTypeTimingTable2;
    else if (str == "kObjectTypeTimingTable3")
        return kObjectTypeTimingTable3;
    return kObjectTypeMax;
}

std::string getTimingtableStr(ObjectType t) {
    switch (t) {
        case kObjectTypeTimingTable:
            return "kObjectTypeTimingTable";
        case kObjectTypeTimingTable0:
            return "kObjectTypeTimingTable0";
        case kObjectTypeTimingTable1:
            return "kObjectTypeTimingTable1";
        case kObjectTypeTimingTable2:
            return "kObjectTypeTimingTable2";
        case kObjectTypeTimingTable3:
            return "kObjectTypeTimingTable3";
        default:
            return "";
    }
}

void outputTimingTable(OStreamBase* os, ObjectId id) {
    *os << id;
    *os << DataBegin("[");
    if (id != UNINIT_OBJECT_ID) {
        auto p = Object::addr<TimingTable>(id);
        if (p) {
            if (p->getObjectType() == kObjectTypeTimingTable0) {
                auto q = dynamic_cast<TimingTable0*>(p);
                if (q) {
                    *os << getTimingtableStr(p->getObjectType())
                        << DataDelimiter();
                    *os << *q;
                }
            } else if (p->getObjectType() == kObjectTypeTimingTable1) {
                auto q = dynamic_cast<TimingTable1*>(p);
                if (q) {
                    *os << getTimingtableStr(p->getObjectType())
                        << DataDelimiter();
                    *os << *q;
                }
            } else if (p->getObjectType() == kObjectTypeTimingTable2) {
                auto q = dynamic_cast<TimingTable2*>(p);
                if (q) {
                    *os << getTimingtableStr(p->getObjectType())
                        << DataDelimiter();
                    *os << *q;
                }
            } else if (p->getObjectType() == kObjectTypeTimingTable3) {
                auto q = dynamic_cast<TimingTable3*>(p);
                if (q) {
                    *os << getTimingtableStr(p->getObjectType())
                        << DataDelimiter();
                    *os << *q;
                }
            } else {
                *os << getTimingtableStr(p->getObjectType()) << DataDelimiter();
                *os << *p;
            }
        }
    }
    *os << DataEnd("]");
}

OStreamBase& operator<<(OStreamBase& os, TimingArc const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    TimingArc::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("is_disabled_") << rhs.is_disabled_ << DataDelimiter();
    os << DataFieldName("timing_sense_") << rhs.timing_sense_
       << DataDelimiter();
    os << DataFieldName("timing_type_") << rhs.timing_type_ << DataDelimiter();
    os << DataFieldName("when_") << rhs.when_;
    {
        os << DataBegin("[");
        if (rhs.when_ != UNINIT_OBJECT_ID) {
            auto p = Object::addr<TFunction>(rhs.when_);
            if (p) os << *p;
        }
        os << DataEnd("]") << DataDelimiter();
    }
    os << DataFieldName("cell_rise_");
    outputTimingTable(&os, rhs.cell_rise_);
    os << DataDelimiter();

    os << DataFieldName("cell_fall_");
    outputTimingTable(&os, rhs.cell_fall_);
    os << DataDelimiter();

    os << DataFieldName("rise_transition_");
    outputTimingTable(&os, rhs.rise_transition_);
    os << DataDelimiter();

    os << DataFieldName("fall_transition_");
    outputTimingTable(&os, rhs.fall_transition_);
    os << DataDelimiter();

    os << DataFieldName("rise_constraint_");
    outputTimingTable(&os, rhs.rise_constraint_);
    os << DataDelimiter();

    os << DataFieldName("fall_constraint_");
    outputTimingTable(&os, rhs.fall_constraint_);
    os << DataDelimiter();

    os << DataFieldName("related_pins_");
    ArrayObject<ObjectId>* p = nullptr;
    if (rhs.related_pins_ != UNINIT_OBJECT_ID) {
        p = Object::addr<ArrayObject<ObjectId>>(rhs.related_pins_);
    }
    if (p != nullptr)
        os << p->getSize();
    else
        os << 0;
    os << DataBegin("[");
    if (p != nullptr) {
        auto delimiter = DataDelimiter("");
        for (int64_t i = 0; i < p->getSize(); ++i) {
            auto q = Object::addr<TTerm>((*p)[i]);
            if (q != nullptr)
                os << delimiter << q->getName();
            else
                os << delimiter << "";
            delimiter = DataDelimiter();
        }
    }
    os << DataEnd("]");

    os << DataEnd(")");
    return os;
}
}  // namespace db
}  // namespace open_edi
