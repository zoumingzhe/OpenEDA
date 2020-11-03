
/**
 * @file timinglib_termattr.cpp
 * @date 2020-08-26
 * @brief TTerm Attributes
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/timing/timinglib/timinglib_termattr.h"

#include "db/core/db.h"

namespace open_edi {
namespace db {

TTermAttr::TTermAttr()
    : TTermAttr::ObjectAttr(),
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
      name_(0) {}

TTermAttr::TTermAttr(TTermAttr const &rhs) { copy(rhs); }

TTermAttr::TTermAttr(TTermAttr &&rhs) noexcept { move(std::move(rhs)); }

TTermAttr &TTermAttr::operator=(TTermAttr const &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TTermAttr &TTermAttr::operator=(TTermAttr &&rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

/// set
void TTermAttr::set_always_on(bool b) { is_always_on_ = b; }
void TTermAttr::set_clock(bool b) { is_clock_ = b; }
void TTermAttr::set_clock_gate_clock_pin(bool b) {
    is_clock_gate_clock_pin_ = b;
}
void TTermAttr::set_clock_gate_enable_pin(bool b) {
    is_clock_gate_enable_pin_ = b;
}
void TTermAttr::set_three_state(bool b) { is_three_state_ = b; }
void TTermAttr::set_bundle(bool b) { is_bundle_ = b; }
void TTermAttr::set_bus(bool b) { is_bus_ = b; }
void TTermAttr::set_dummy(bool b) { is_dummy_ = b; }
void TTermAttr::set_direction(PinDirection d) { direction_ = d; }
void TTermAttr::set_capacitance(float f) { capacitance_ = f; }
void TTermAttr::set_rise_capacitance(float f) { rise_capacitance_ = f; }
void TTermAttr::set_fall_capacitance(float f) { fall_capacitance_ = f; }
void TTermAttr::set_max_capacitance(float f) { max_capacitance_ = f; }
void TTermAttr::set_min_capacitance(float f) { min_capacitance_ = f; }
void TTermAttr::set_max_fanout(float f) { max_fanout_ = f; }
void TTermAttr::set_min_fanout(float f) { min_fanout_ = f; }
void TTermAttr::set_max_transition(float f) { max_transition_ = f; }
void TTermAttr::set_min_transition(float f) { min_transition_ = f; }
void TTermAttr::set_min_period(float f) { min_period_ = f; }
void TTermAttr::set_min_pulse_width_high(float f) { min_pulse_width_high_ = f; }
void TTermAttr::set_min_pulse_width_low(float f) { min_pulse_width_low_ = f; }
void TTermAttr::set_name(const std::string &name) {
    Cell *topCell = getTopCell();
    if (topCell) {
        int64_t index = topCell->getOrCreateSymbol(name.c_str());
        if (index != -1) {
            name_ = index;
            // topCell->addSymbolReference(name_, this->getId());
        }
    }
}

/// get
bool TTermAttr::is_always_on(void) { return is_always_on_; }
bool TTermAttr::is_clock(void) { return is_clock_; }
bool TTermAttr::is_clock_gate_clock_pin(void) {
    return is_clock_gate_clock_pin_;
}
bool TTermAttr::is_clock_gate_enable_pin(void) {
    return is_clock_gate_enable_pin_;
}
bool TTermAttr::is_three_state(void) { return is_three_state_; }
bool TTermAttr::is_bundle(void) { return is_bundle_; }
bool TTermAttr::is_bus(void) { return is_bus_; }
bool TTermAttr::is_dummy(void) { return is_dummy_; }
PinDirection TTermAttr::get_direction(void) { return direction_; }
float TTermAttr::get_capacitance(void) { return capacitance_; }
float TTermAttr::get_rise_capacitance(void) { return rise_capacitance_; }
float TTermAttr::get_fall_capacitance(void) { return fall_capacitance_; }
float TTermAttr::get_max_capacitance(void) { return max_capacitance_; }
float TTermAttr::get_min_capacitance(void) { return min_capacitance_; }
float TTermAttr::get_max_fanout(void) { return max_fanout_; }
float TTermAttr::get_min_fanout(void) { return min_fanout_; }
float TTermAttr::get_max_transition(void) { return max_transition_; }
float TTermAttr::get_min_transition(void) { return min_transition_; }
float TTermAttr::get_min_period(void) { return min_period_; }
float TTermAttr::get_min_pulse_width_high(void) {
    return min_pulse_width_high_;
}
float TTermAttr::get_min_pulse_width_low(void) { return min_pulse_width_low_; }
std::string TTermAttr::get_name(void) const {
    Cell *topCell = getTopCell();
    if (topCell) {
        return topCell->getSymbolByIndex(name_);
    }
    return "";
}
SymbolIndex TTermAttr::get_name_index(void) { return name_; }

TTermAttr::IndexType TTermAttr::memory() const {
    IndexType ret = this->ObjectAttr::memory();

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

    return ret;
}

TTermAttr *TTermAttr::clone() const { return new TTermAttr(*this); }

void TTermAttr::copy(TTermAttr const &rhs) {
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
}

void TTermAttr::move(TTermAttr &&rhs) {
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
}

OStreamBase &operator<<(OStreamBase &os, TTermAttr const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

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
    os << DataFieldName("name_") << rhs.get_name();

    os << DataEnd(")");
    return os;
}
}  // namespace db
}  // namespace open_edi
