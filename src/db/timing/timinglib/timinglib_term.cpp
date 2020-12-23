/**
 * @file timinglib_term.cpp
 * @date 2020-08-26
 * @brief TTerm Class
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/timing/timinglib/timinglib_term.h"

#include "db/core/db.h"
#include "db/timing/timinglib/timinglib_function.h"
#include "db/timing/timinglib/timinglib_pgterm.h"
#include "db/timing/timinglib/timinglib_timingarc.h"

namespace open_edi {
namespace db {

TTerm::TTerm()
    : TTerm::BaseType(),
      is_always_on_(false),
      is_clock_(false),
      is_clock_gate_clock_pin_(false),
      is_clock_gate_enable_pin_(false),
      is_three_state_(false),
      is_bundle_(false),
      is_bus_(false),
      is_dummy_(false),
      direction_(PinDirection::kUnknown),
      capacitance_(0.0),
      rise_capacitance_(0.0),
      fall_capacitance_(0.0),
      max_capacitance_(0.0),
      min_capacitance_(0.0),
      max_fanout_(0.0),
      min_fanout_(0.0),
      max_transition_(0.0),
      min_transition_(0.0),
      min_period_(0.0),
      min_pulse_width_high_(0.0),
      min_pulse_width_low_(0.0),
      name_(0),
      related_power_pin_(UNINIT_OBJECT_ID),
      related_ground_pin_(UNINIT_OBJECT_ID),
      function_(UNINIT_OBJECT_ID),
      member_pins_(UNINIT_OBJECT_ID),
      timing_arcs_(UNINIT_OBJECT_ID),
      member_pins_map_() {
    setObjectType(ObjectType::kObjectTypeTTerm);
}

TTerm::~TTerm() {}

TTerm::TTerm(Object *owner, TTerm::IndexType id)
    : TTerm::BaseType(owner, id),
      is_always_on_(false),
      is_clock_(false),
      is_clock_gate_clock_pin_(false),
      is_clock_gate_enable_pin_(false),
      is_three_state_(false),
      is_bundle_(false),
      is_bus_(false),
      is_dummy_(false),
      direction_(PinDirection::kUnknown),
      capacitance_(0.0),
      rise_capacitance_(0.0),
      fall_capacitance_(0.0),
      max_capacitance_(0.0),
      min_capacitance_(0.0),
      max_fanout_(0.0),
      min_fanout_(0.0),
      max_transition_(0.0),
      min_transition_(0.0),
      min_period_(0.0),
      min_pulse_width_high_(0.0),
      min_pulse_width_low_(0.0),
      name_(0),
      related_power_pin_(UNINIT_OBJECT_ID),
      related_ground_pin_(UNINIT_OBJECT_ID),
      function_(UNINIT_OBJECT_ID),
      member_pins_(UNINIT_OBJECT_ID),
      timing_arcs_(UNINIT_OBJECT_ID),
      member_pins_map_() {
    setObjectType(ObjectType::kObjectTypeTTerm);
}

TTerm::TTerm(TTerm const &rhs) { copy(rhs); }

TTerm::TTerm(TTerm &&rhs) noexcept { move(std::move(rhs)); }

TTerm &TTerm::operator=(TTerm const &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TTerm &TTerm::operator=(TTerm &&rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

TTerm::IndexType TTerm::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(is_always_on_);
    ret += sizeof(is_clock_);
    ret += sizeof(is_clock_gate_clock_pin_);
    ret += sizeof(is_clock_gate_enable_pin_);
    ret += sizeof(is_three_state_);
    ret += sizeof(is_bundle_);
    ret += sizeof(is_bus_);
    ret += sizeof(is_dummy_);
    ret += sizeof(direction_);
    ret += sizeof(capacitance_);
    ret += sizeof(rise_capacitance_);
    ret += sizeof(fall_capacitance_);
    ret += sizeof(max_capacitance_);
    ret += sizeof(min_capacitance_);
    ret += sizeof(max_fanout_);
    ret += sizeof(min_fanout_);
    ret += sizeof(max_transition_);
    ret += sizeof(min_transition_);
    ret += sizeof(min_period_);
    ret += sizeof(min_pulse_width_high_);
    ret += sizeof(min_pulse_width_low_);
    ret += sizeof(name_);
    ret += sizeof(related_power_pin_);
    ret += sizeof(related_ground_pin_);
    ret += sizeof(function_);
    ret += sizeof(member_pins_);
    ret += sizeof(timing_arcs_);

    return ret;
}

void TTerm::setAlwaysOn(bool b) { is_always_on_ = b; }
void TTerm::setClock(bool b) { is_clock_ = b; }
void TTerm::setClockGateClockPin(bool b) { is_clock_gate_clock_pin_ = b; }
void TTerm::setClockGateEnablePin(bool b) { is_clock_gate_enable_pin_ = b; }
void TTerm::setThreeState(bool b) { is_three_state_ = b; }
void TTerm::setBundle(bool b) { is_bundle_ = b; }
void TTerm::setBus(bool b) { is_bus_ = b; }
void TTerm::setDummy(bool b) { is_dummy_ = b; }
void TTerm::setDirection(PinDirection d) { direction_ = d; }
void TTerm::setCapacitance(float f) { capacitance_ = f; }
void TTerm::setRiseCapacitance(float f) { rise_capacitance_ = f; }
void TTerm::setFallCapacitance(float f) { fall_capacitance_ = f; }
void TTerm::setMaxCapacitance(float f) { max_capacitance_ = f; }
void TTerm::setMinCapacitance(float f) { min_capacitance_ = f; }
void TTerm::setMaxFanout(float f) { max_fanout_ = f; }
void TTerm::setMinFanout(float f) { min_fanout_ = f; }
void TTerm::setMaxTransition(float f) { max_transition_ = f; }
void TTerm::setMinTransition(float f) { min_transition_ = f; }
void TTerm::setMinPeriod(float f) { min_period_ = f; }
void TTerm::setMinPulseWidthHigh(float f) { min_pulse_width_high_ = f; }
void TTerm::setMinPulseWidthLow(float f) { min_pulse_width_low_ = f; }
void TTerm::setName(const std::string &name) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(name.c_str());
        if (index != kInvalidSymbolIndex) {
            name_ = index;
            timing_lib->addSymbolReference(name_, this->getId());
        }
    }
}
void TTerm::setRelatedPowerPin(ObjectId id) { related_power_pin_ = id; }
void TTerm::setRelatedGroundPin(ObjectId id) { related_ground_pin_ = id; }
TFunction *TTerm::setFunction(const std::string &str) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        auto p = Object::createObject<TFunction>(kObjectTypeTFunction,
                                                 timing_lib->getId());
        if (p) {
            p->setOwner(this);
            function_ = p->getId();
            p->setFuncStr(str);
        }
        return p;
    }
    return nullptr;
}
void TTerm::addMemberPin(ObjectId id) {
    if (id == UNINIT_OBJECT_ID) return;
    auto pin = Object::addr<TTerm>(id);
    if (pin) {
        ArrayObject<ObjectId> *p = nullptr;
        if (member_pins_ == UNINIT_OBJECT_ID) {
            Timing *timing_lib = getTimingLib();
            if (timing_lib != nullptr) {
                p = Object::createObject<ArrayObject<ObjectId>>(
                    kObjectTypeArray, timing_lib->getId());
                if (p != nullptr) {
                    member_pins_ = p->getId();
                    p->setPool(timing_lib->getPool());
                    p->reserve(32);
                }
            }
        } else {
            p = Object::addr<ArrayObject<ObjectId>>(member_pins_);
        }
        if (p != nullptr) {
            p->pushBack(id);
            member_pins_map_[pin->getNameIndex()] = id;
        }
    }
}
void TTerm::addTimingarc(ObjectId id) {
    ArrayObject<ObjectId> *p = nullptr;
    if (timing_arcs_ == UNINIT_OBJECT_ID) {
        Timing *timing_lib = getTimingLib();
        if (timing_lib != nullptr) {
            p = Object::createObject<ArrayObject<ObjectId>>(
                kObjectTypeArray, timing_lib->getId());
            if (p != nullptr) {
                timing_arcs_ = p->getId();
                p->setPool(timing_lib->getPool());
                p->reserve(32);
            }
        }
    } else {
        p = Object::addr<ArrayObject<ObjectId>>(timing_arcs_);
    }
    p->pushBack(id);
}

bool TTerm::isAlwaysOn(void) { return is_always_on_; }
bool TTerm::isClock(void) { return is_clock_; }
bool TTerm::isClockGateClockPin(void) { return is_clock_gate_clock_pin_; }
bool TTerm::isClockGateEnablePin(void) { return is_clock_gate_enable_pin_; }
bool TTerm::isThreeState(void) { return is_three_state_; }
bool TTerm::isBundle(void) { return is_bundle_; }
bool TTerm::isBus(void) { return is_bus_; }
bool TTerm::isDummy(void) { return is_dummy_; }
PinDirection TTerm::getDirection(void) { return direction_; }
float TTerm::getCapacitance(void) { return capacitance_; }
float TTerm::getRiseCapacitance(void) { return rise_capacitance_; }
float TTerm::getFallCapacitance(void) { return fall_capacitance_; }
float TTerm::getMaxCapacitance(void) { return max_capacitance_; }
float TTerm::getMinCapacitance(void) { return min_capacitance_; }
float TTerm::getMaxFanout(void) { return max_fanout_; }
float TTerm::getMinFanout(void) { return min_fanout_; }
float TTerm::getMaxTransition(void) { return max_transition_; }
float TTerm::getMinTransition(void) { return min_transition_; }
float TTerm::getMinPeriod(void) { return min_period_; }
float TTerm::getMinPulseWidthHigh(void) { return min_pulse_width_high_; }
float TTerm::getMinPulseWidthLow(void) { return min_pulse_width_low_; }
std::string TTerm::getName(void) const {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(name_);
    }
    return "";
}
SymbolIndex TTerm::getNameIndex(void) { return name_; }
TFunction *TTerm::getFunction(void) {
    if (function_ != UNINIT_OBJECT_ID) {
        return Object::addr<TFunction>(function_);
    }
    return nullptr;
}
TimingArc *TTerm::getTimingArc(ObjectId id) {
    if (id != UNINIT_OBJECT_ID) {
        return Object::addr<TimingArc>(id);
    }
    return nullptr;
}
std::vector<TimingArc *> TTerm::getTimingArcs(void) {
    std::vector<TimingArc *> timingArcs;
    if (timing_arcs_ != UNINIT_OBJECT_ID) {
        Timing *timing_lib = getTimingLib();
        if (timing_lib != nullptr) {
            ArrayObject<ObjectId> *p =
                Object::addr<ArrayObject<ObjectId>>(timing_arcs_);
            if (p != nullptr) {
                int64_t count = p->getSize();
                timingArcs.reserve(count);
                for (int64_t i = 0; i < count; ++i) {
                    auto q = Object::addr<TimingArc>((*p)[i]);
                    if (q != nullptr) timingArcs.emplace_back(q);
                }
            }
        }
    }
    return timingArcs;
}
TPgTerm *TTerm::getRelatedPowerPin(void) const {
    if (related_power_pin_ != UNINIT_OBJECT_ID) {
        return Object::addr<TPgTerm>(related_power_pin_);
    }
    return nullptr;
}
TPgTerm *TTerm::getRelatedGroundPin(void) const {
    if (related_ground_pin_ != UNINIT_OBJECT_ID) {
        return Object::addr<TPgTerm>(related_ground_pin_);
    }
    return nullptr;
}
void TTerm::print(std::ostream &stream) {
    stream << "TTerm name: " << getName() << std::endl;
}

void TTerm::copy(TTerm const &rhs) {
    this->BaseType::copy(rhs);

    is_always_on_ = rhs.is_always_on_;
    is_clock_ = rhs.is_clock_;
    is_clock_gate_clock_pin_ = rhs.is_clock_gate_clock_pin_;
    is_clock_gate_enable_pin_ = rhs.is_clock_gate_enable_pin_;
    is_three_state_ = rhs.is_three_state_;
    is_bundle_ = rhs.is_bundle_;
    is_bus_ = rhs.is_bus_;
    is_dummy_ = rhs.is_dummy_;
    direction_ = rhs.direction_;
    capacitance_ = rhs.capacitance_;
    rise_capacitance_ = rhs.rise_capacitance_;
    fall_capacitance_ = rhs.fall_capacitance_;
    max_capacitance_ = rhs.max_capacitance_;
    min_capacitance_ = rhs.min_capacitance_;
    max_fanout_ = rhs.max_fanout_;
    min_fanout_ = rhs.min_fanout_;
    max_transition_ = rhs.max_transition_;
    min_transition_ = rhs.min_transition_;
    min_period_ = rhs.min_period_;
    min_pulse_width_high_ = rhs.min_pulse_width_high_;
    min_pulse_width_low_ = rhs.min_pulse_width_low_;
    name_ = rhs.name_;
    related_power_pin_ = rhs.related_power_pin_;
    related_ground_pin_ = rhs.related_ground_pin_;
    function_ = rhs.function_;
    member_pins_ = rhs.member_pins_;
    timing_arcs_ = rhs.timing_arcs_;
    member_pins_map_ = rhs.member_pins_map_;
}

void TTerm::move(TTerm &&rhs) {
    this->BaseType::move(std::move(rhs));

    is_always_on_ = std::move(rhs.is_always_on_);
    is_clock_ = std::move(rhs.is_clock_);
    is_clock_gate_clock_pin_ = std::move(rhs.is_clock_gate_clock_pin_);
    is_clock_gate_enable_pin_ = std::move(rhs.is_clock_gate_enable_pin_);
    is_three_state_ = std::move(rhs.is_three_state_);
    is_bundle_ = std::move(rhs.is_bundle_);
    is_bus_ = std::move(rhs.is_bus_);
    is_dummy_ = std::move(rhs.is_dummy_);
    direction_ = std::move(rhs.direction_);
    capacitance_ = std::move(rhs.capacitance_);
    rise_capacitance_ = std::move(rhs.rise_capacitance_);
    fall_capacitance_ = std::move(rhs.fall_capacitance_);
    max_capacitance_ = std::move(rhs.max_capacitance_);
    min_capacitance_ = std::move(rhs.min_capacitance_);
    max_fanout_ = std::move(rhs.max_fanout_);
    min_fanout_ = std::move(rhs.min_fanout_);
    max_transition_ = std::move(rhs.max_transition_);
    min_transition_ = std::move(rhs.min_transition_);
    min_period_ = std::move(rhs.min_period_);
    min_pulse_width_high_ = std::move(rhs.min_pulse_width_high_);
    min_pulse_width_low_ = std::move(rhs.min_pulse_width_low_);
    name_ = std::move(rhs.name_);
    related_power_pin_ = std::move(rhs.related_power_pin_);
    related_ground_pin_ = std::move(rhs.related_ground_pin_);
    function_ = std::move(rhs.function_);
    member_pins_ = std::move(rhs.member_pins_);
    timing_arcs_ = std::move(rhs.timing_arcs_);
    member_pins_map_ = std::move(rhs.member_pins_map_);
}

OStreamBase &operator<<(OStreamBase &os, TTerm const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    TTerm::BaseType const &base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("is_always_on_") << rhs.is_always_on_
       << DataDelimiter();
    os << DataFieldName("is_clock_") << rhs.is_clock_ << DataDelimiter();
    os << DataFieldName("is_clock_gate_clock_pin_")
       << rhs.is_clock_gate_clock_pin_ << DataDelimiter();
    os << DataFieldName("is_clock_gate_enable_pin_")
       << rhs.is_clock_gate_enable_pin_ << DataDelimiter();
    os << DataFieldName("is_three_state_") << rhs.is_three_state_
       << DataDelimiter();
    os << DataFieldName("is_bundle_") << rhs.is_bundle_ << DataDelimiter();
    os << DataFieldName("is_bus_") << rhs.is_bus_ << DataDelimiter();
    os << DataFieldName("is_dummy_") << rhs.is_dummy_ << DataDelimiter();
    os << DataFieldName("direction_") << rhs.direction_ << DataDelimiter();
    os << DataFieldName("capacitance_") << rhs.capacitance_ << DataDelimiter();
    os << DataFieldName("rise_capacitance_") << rhs.rise_capacitance_
       << DataDelimiter();
    os << DataFieldName("fall_capacitance_") << rhs.fall_capacitance_
       << DataDelimiter();
    os << DataFieldName("max_capacitance_") << rhs.max_capacitance_
       << DataDelimiter();
    os << DataFieldName("min_capacitance_") << rhs.min_capacitance_
       << DataDelimiter();
    os << DataFieldName("max_fanout_") << rhs.max_fanout_ << DataDelimiter();
    os << DataFieldName("min_fanout_") << rhs.min_fanout_ << DataDelimiter();
    os << DataFieldName("max_transition_") << rhs.max_transition_
       << DataDelimiter();
    os << DataFieldName("min_transition_") << rhs.min_transition_
       << DataDelimiter();
    os << DataFieldName("min_period_") << rhs.min_period_ << DataDelimiter();
    os << DataFieldName("min_pulse_width_high_") << rhs.min_pulse_width_high_
       << DataDelimiter();
    os << DataFieldName("min_pulse_width_low_") << rhs.min_pulse_width_low_
       << DataDelimiter();
    os << DataFieldName("name_") << rhs.getName();
    os << DataFieldName("related_power_pin_");
    {
        auto p = rhs.getRelatedPowerPin();
        if (p != nullptr)
            os << p->getName();
        else
            os << "";
        os << DataDelimiter();
    }

    os << DataFieldName("related_ground_pin_");
    {
        auto p = rhs.getRelatedGroundPin();
        if (p != nullptr)
            os << p->getName();
        else
            os << "";
        os << DataDelimiter();
    }

    os << DataFieldName("function_") << rhs.function_;
    {
        os << DataBegin("[");
        if (rhs.function_ != UNINIT_OBJECT_ID) {
            auto p = Object::addr<TFunction>(rhs.function_);
            if (p) os << *p;
        }
        os << DataEnd("]") << DataDelimiter();
    }
    os << DataFieldName("member_pins_");
    {
        ArrayObject<ObjectId> *p = nullptr;
        if (rhs.member_pins_ != UNINIT_OBJECT_ID) {
            p = Object::addr<ArrayObject<ObjectId>>(rhs.member_pins_);
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
        os << DataEnd("]") << DataDelimiter();
    }
    os << DataFieldName("timing_arcs_");
    {
        ArrayObject<ObjectId> *p = nullptr;
        if (rhs.timing_arcs_ != UNINIT_OBJECT_ID) {
            p = Object::addr<ArrayObject<ObjectId>>(rhs.timing_arcs_);
        }
        if (p != nullptr)
            os << p->getSize();
        else
            os << 0;
        os << DataBegin("[");
        if (p != nullptr) {
            auto delimiter = DataDelimiter("");
            for (int64_t i = 0; i < p->getSize(); ++i) {
                auto q = Object::addr<TimingArc>((*p)[i]);
                if (q) {
                    os << delimiter << *q;
                    delimiter = DataDelimiter();
                }
            }
        }
        os << DataEnd("]");
    }

    os << DataEnd(")");
    return os;
}
}  // namespace db
}  // namespace open_edi
