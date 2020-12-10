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
    volatge_unit_ = {1.0f, 1, "V"};
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
    ret += sizeof(volatge_unit_);
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
TUnit &TUnits::getVolatgeUnit(void) { return volatge_unit_; }
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
void TUnits::setVolatgeUnit(const TUnit &unit) { volatge_unit_ = unit; }
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
    volatge_unit_ = rhs.volatge_unit_;
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
    volatge_unit_ = std::move(rhs.volatge_unit_);
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
    os << DataFieldName("volatge_unit_") << rhs.volatge_unit_
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

float UnitMultiply::getTimeUnitMultiply(const std::string &u) {
    std::string str;
    std::transform(u.begin(), u.end(), std::back_inserter(str), ::tolower);
    if (str == "s") return 1.0f;
    if (str == "ks")
        return 1.0e3f;
    else if (str == "ms")
        return 1e-3f;
    else if (str == "us")
        return 1e-6f;
    else if (str == "ns")
        return 1e-9f;
    else if (str == "ps")
        return 1e-12f;
    else if (str == "fs")
        return 1e-15f;
    else
        return 0.0f;
}
float UnitMultiply::getResistanceUnitMultiply(const std::string &u) {
    std::string str;
    std::transform(u.begin(), u.end(), std::back_inserter(str), ::tolower);
    if (str == "ohm") return 1.0f;
    if (str == "kohm")
        return 1e+3f;
    else if (str == "mohm")
        return 1e-3f;
    else if (str == "uohm")
        return 1e-6f;
    else if (str == "nohm")
        return 1e-9f;
    else if (str == "pohm")
        return 1e-12f;
    else if (str == "fohm")
        return 1e-15f;
    else
        return 0.0f;
}
float UnitMultiply::getCapacitiveUnitMultiply(const std::string &u) {
    std::string str;
    std::transform(u.begin(), u.end(), std::back_inserter(str), ::tolower);
    if (str == "f") return 1.0f;
    if (str == "ff")
        return 1e-15f;
    else if (str == "pf")
        return 1e-12f;
    else
        return 0.0f;
}
float UnitMultiply::getVoltageUnitMultiply(const std::string &u) {
    std::string str;
    std::transform(u.begin(), u.end(), std::back_inserter(str), ::tolower);
    if (str == "v") return 1.0f;
    if (str == "mv")
        return 1e-3f;
    else
        return 0.0f;
}
float UnitMultiply::getCurrentUnitMultiply(const std::string &u) {
    std::string str;
    std::transform(u.begin(), u.end(), std::back_inserter(str), ::tolower);
    if (str == "a")
        return 1.0f;
    else if (str == "ma")
        return 1e-3f;
    else if (str == "ua")
        return 1e-6f;
    else
        return 0.0f;
}
float UnitMultiply::getPowerUnitMultiply(const std::string &u) {
    std::string str;
    std::transform(u.begin(), u.end(), std::back_inserter(str), ::tolower);
    if (str == "w")
        return 1.0f;
    else if (str == "mw")
        return 1e-3f;
    else if (str == "nw")
        return 1e-9f;
    else if (str == "pw")
        return 1e-12f;
    else
        return 0.0f;
}
float UnitMultiply::getDistanceUnitMultiply(const std::string &u) {
    std::string str;
    std::transform(u.begin(), u.end(), std::back_inserter(str), ::tolower);
    if (str == "m")
        return 1.0f;
    else if (str == "mm")
        return 1e-3f;
    else if (str == "um")
        return 1e-6f;
    else
        return 0.0f;
}


}  // namespace db
}  // namespace open_edi
