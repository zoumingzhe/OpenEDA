/**
 * @file timinglib_units.cpp
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
#include "db/timing/timinglib/timinglib_units.h"

namespace open_edi {
namespace db {

TUnits::TUnits() : TUnits::BaseType() {
    setObjectType(ObjectType::kObjectTypeTUnits);
    initValues();
}

void TUnits::initValues() {
    time_unit_ = {1.0e-9f, 1, "s"};
    capacitance_unit_ = {1.0e-12f, 1, "F"};
    voltage_unit_ = {1.0f, 1, "V"};
    resistance_unit_ = {1.0f, 1, "ohm"};
    pulling_resistance_unit_ = {1.0f, 1, "ohm"};
    current_unit_ = {1.0e-3f, 1, "A"};
    power_unit_ = {1.0f, 1, "W"};
    distance_unit_ = {1.0e-6f, 1, "m"};
    scalar_unit_ = {1.0f, 1, ""};
}

TUnits::TUnits(TUnits const &rhs) { copy(rhs); }

TUnits::TUnits(TUnits &&rhs) noexcept { move(std::move(rhs)); }

TUnits &TUnits::operator=(TUnits const &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TUnits &TUnits::operator=(TUnits &&rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

UInt32 TUnits::memory() const {
    UInt32 ret = this->BaseType::memory();

    ret += sizeof(time_unit_);
    ret += sizeof(capacitance_unit_);
    ret += sizeof(voltage_unit_);
    ret += sizeof(resistance_unit_);
    ret += sizeof(pulling_resistance_unit_);
    ret += sizeof(current_unit_);
    ret += sizeof(power_unit_);
    ret += sizeof(distance_unit_);
    ret += sizeof(scalar_unit_);

    return ret;
}

/// Access APIs:
TUnit &TUnits::getTimeUnit(void) { return time_unit_; }
TUnit &TUnits::getCapacitanceUnit(void) { return capacitance_unit_; }
TUnit &TUnits::getVoltageUnit(void) { return voltage_unit_; }
TUnit &TUnits::getResistanceUnit(void) { return resistance_unit_; }
TUnit &TUnits::getPullingResistanceUnit(void) {
    return pulling_resistance_unit_;
}
TUnit &TUnits::getCurrentUnit(void) { return current_unit_; }
TUnit &TUnits::getPowerUnit(void) { return power_unit_; }
TUnit &TUnits::getDistanceUnit(void) { return distance_unit_; }
TUnit &TUnits::getScalarUnit(void) { return scalar_unit_; }

// Set:
void TUnits::setTimeUnit(const TUnit &unit) { time_unit_ = unit; }
void TUnits::setCapacitanceUnit(const TUnit &unit) { capacitance_unit_ = unit; }
void TUnits::setVolatgeUnit(const TUnit &unit) { voltage_unit_ = unit; }
void TUnits::setResistanceUnit(const TUnit &unit) { resistance_unit_ = unit; }
void TUnits::setPullingResistanceUnit(const TUnit &unit) {
    pulling_resistance_unit_ = unit;
}
void TUnits::setCurrentUnit(const TUnit &unit) { current_unit_ = unit; }
void TUnits::setPowerUnit(const TUnit &unit) { power_unit_ = unit; }
void TUnits::setDistanceUnit(const TUnit &unit) { distance_unit_ = unit; }
void TUnits::setScalarUnit(const TUnit &unit) { scalar_unit_ = unit; }

TUnits *TUnits::clone() const { return new TUnits(*this); }

void TUnits::copy(TUnits const &rhs) {
    this->BaseType::copy(rhs);

    time_unit_ = rhs.time_unit_;
    capacitance_unit_ = rhs.capacitance_unit_;
    voltage_unit_ = rhs.voltage_unit_;
    resistance_unit_ = rhs.resistance_unit_;
    pulling_resistance_unit_ = rhs.pulling_resistance_unit_;
    current_unit_ = rhs.current_unit_;
    power_unit_ = rhs.power_unit_;
    distance_unit_ = rhs.distance_unit_;
    scalar_unit_ = rhs.scalar_unit_;
}

void TUnits::move(TUnits &&rhs) {
    this->BaseType::move(std::move(rhs));

    time_unit_ = std::move(rhs.time_unit_);
    capacitance_unit_ = std::move(rhs.capacitance_unit_);
    voltage_unit_ = std::move(rhs.voltage_unit_);
    resistance_unit_ = std::move(rhs.resistance_unit_);
    pulling_resistance_unit_ = std::move(rhs.pulling_resistance_unit_);
    current_unit_ = std::move(rhs.current_unit_);
    power_unit_ = std::move(rhs.power_unit_);
    distance_unit_ = std::move(rhs.distance_unit_);
    scalar_unit_ = std::move(rhs.scalar_unit_);
}

OStreamBase &operator<<(OStreamBase &os, TUnits const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    TUnits::BaseType const &base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("time_unit_") << rhs.time_unit_ << DataDelimiter();
    os << DataFieldName("capacitance_unit_") << rhs.capacitance_unit_
       << DataDelimiter();
    os << DataFieldName("voltage_unit_") << rhs.voltage_unit_
       << DataDelimiter();
    os << DataFieldName("resistance_unit_") << rhs.resistance_unit_
       << DataDelimiter();
    os << DataFieldName("pulling_resistance_unit_")
       << rhs.pulling_resistance_unit_ << DataDelimiter();
    os << DataFieldName("current_unit_") << rhs.current_unit_
       << DataDelimiter();
    os << DataFieldName("power_unit_") << rhs.power_unit_ << DataDelimiter();
    os << DataFieldName("distance_unit_") << rhs.distance_unit_
       << DataDelimiter();
    os << DataFieldName("scalar_unit_") << rhs.scalar_unit_;
    os << DataEnd(")");
    return os;
}
}  // namespace db
}  // namespace open_edi
