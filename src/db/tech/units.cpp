/**
 * @file  units.cpp
 * @date  08/21/2020 02:33:40 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/tech/units.h"

#include "db/core/db.h"

namespace open_edi {
namespace db {

Units::Units() : Units::BaseType() { __init(); }

void Units::__init() {
    from_def_ = false;
    capacitance_factor_ = 0;
    current_factor_ = 0;
    power_factor_ = 0;
    resistance_factor_ = 0;
    voltage_factor_ = 0;
    time_factor_ = 0;
    frequency_factor_ = 0;
    length_factor_ = 0;

    capacitance_unit_ = -1;
    current_unit_ = -1;
    power_unit_ = -1;
    resistance_unit_ = -1;
    voltage_unit_ = -1;
    time_unit_ = -1;
    frequency_unit_ = -1;
    length_unit_ = -1;
}

Units::Units(Units const &rhs) { copy(rhs); }

Units::Units(Units &&rhs) noexcept { move(std::move(rhs)); }

Units &Units::operator=(Units const &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

Units &Units::operator=(Units &&rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

UInt32 Units::memory() const {
    UInt32 ret = this->BaseType::memory();
    ret += sizeof(from_def_);
    ret += sizeof(capacitance_factor_);
    ret += sizeof(current_factor_);
    ret += sizeof(power_factor_);
    ret += sizeof(resistance_factor_);
    ret += sizeof(voltage_factor_);
    ret += sizeof(time_factor_);
    ret += sizeof(frequency_factor_);
    ret += sizeof(length_factor_);

    ret += sizeof(capacitance_unit_);
    ret += sizeof(current_unit_);
    ret += sizeof(power_unit_);
    ret += sizeof(resistance_unit_);
    ret += sizeof(voltage_unit_);
    ret += sizeof(time_unit_);
    ret += sizeof(frequency_unit_);
    ret += sizeof(length_unit_);
    return ret;
}

void Units::copy(Units const &rhs) {
    from_def_ = rhs.from_def_;
    capacitance_factor_ = rhs.capacitance_factor_;
    current_factor_ = rhs.current_factor_;
    power_factor_ = rhs.power_factor_;
    resistance_factor_ = rhs.resistance_factor_;
    voltage_factor_ = rhs.voltage_factor_;
    time_factor_ = rhs.time_factor_;
    frequency_factor_ = rhs.frequency_factor_;
    length_factor_ = rhs.length_factor_;

    capacitance_unit_ = rhs.capacitance_unit_;
    current_unit_ = rhs.current_unit_;
    power_unit_ = rhs.power_unit_;
    resistance_unit_ = rhs.resistance_unit_;
    voltage_unit_ = rhs.voltage_unit_;
    time_unit_ = rhs.time_unit_;
    frequency_unit_ = rhs.frequency_unit_;
    length_unit_ = rhs.length_unit_;
}

void Units::move(Units &&rhs) {
    from_def_ = std::move(rhs.from_def_);
    capacitance_factor_ = std::move(rhs.capacitance_factor_);
    current_factor_ = std::move(rhs.current_factor_);
    power_factor_ = std::move(rhs.power_factor_);
    resistance_factor_ = std::move(rhs.resistance_factor_);
    voltage_factor_ = std::move(rhs.voltage_factor_);
    time_factor_ = std::move(rhs.time_factor_);
    frequency_factor_ = std::move(rhs.frequency_factor_);
    length_factor_ = std::move(rhs.length_factor_);

    capacitance_unit_ = std::move(rhs.capacitance_unit_);
    current_unit_ = std::move(rhs.current_unit_);
    power_unit_ = std::move(rhs.power_unit_);
    resistance_unit_ = std::move(rhs.resistance_unit_);
    voltage_unit_ = std::move(rhs.voltage_unit_);
    time_unit_ = std::move(rhs.time_unit_);
    frequency_unit_ = std::move(rhs.frequency_unit_);
    length_unit_ = std::move(rhs.length_unit_);
}

const char *Units::getCapacitanceUnits() const {
    if (capacitance_unit_ < 0) return nullptr;

    return getTopCell()->getSymbolByIndex(capacitance_unit_).c_str();
}

const char *Units::getCurrentUnits() const {
    if (current_unit_ < 0) return nullptr;

    return getTopCell()->getSymbolByIndex(current_unit_).c_str();
}

const char *Units::getPowerUnits() const {
    if (power_unit_ < 0) return nullptr;

    return getTopCell()->getSymbolByIndex(power_unit_).c_str();
}

const char *Units::getResistanceUnits() const {
    if (resistance_unit_ < 0) return nullptr;

    return getTopCell()->getSymbolByIndex(resistance_unit_).c_str();
}

const char *Units::getVoltageUnits() const {
    if (voltage_unit_ < 0) return nullptr;

    return getTopCell()->getSymbolByIndex(voltage_unit_).c_str();
}

const char *Units::getTimeUnits() const {
    if (time_unit_ < 0) return nullptr;

    return getTopCell()->getSymbolByIndex(time_unit_).c_str();
}

const char *Units::getFrequencyUnits() const {
    if (frequency_unit_ < 0) return nullptr;

    return getTopCell()->getSymbolByIndex(frequency_unit_).c_str();
}

const char *Units::getLengthUnits() const {
    if (length_unit_ < 0) return nullptr;

    return getTopCell()->getSymbolByIndex(length_unit_).c_str();
}

void Units::setCapacitanceUnits(const char *v) {
    int64_t index = getTopCell()->getOrCreateSymbol(v);
    if (index == -1) return;

    capacitance_unit_ = index;
    getTopCell()->addSymbolReference(capacitance_unit_, this->getId());
}

void Units::setCurrentUnits(const char *v) {
    int64_t index = getTopCell()->getOrCreateSymbol(v);
    if (index == -1) return;

    current_unit_ = index;
    getTopCell()->addSymbolReference(current_unit_, this->getId());
}

void Units::setPowerUnits(const char *v) {
    int64_t index = getTopCell()->getOrCreateSymbol(v);
    if (index == -1) return;

    power_unit_ = index;
    getTopCell()->addSymbolReference(power_unit_, this->getId());
}

void Units::setResistanceUnits(const char *v) {
    int64_t index = getTopCell()->getOrCreateSymbol(v);
    if (index == -1) return;

    resistance_unit_ = index;
    getTopCell()->addSymbolReference(resistance_unit_, this->getId());
}

void Units::setVoltageUnits(const char *v) {
    int64_t index = getTopCell()->getOrCreateSymbol(v);
    if (index == -1) return;

    voltage_unit_ = index;
    getTopCell()->addSymbolReference(voltage_unit_, this->getId());
}

void Units::setTimeUnits(const char *v) {
    int64_t index = getTopCell()->getOrCreateSymbol(v);
    if (index == -1) return;

    time_unit_ = index;
    getTopCell()->addSymbolReference(time_unit_, this->getId());
}

void Units::setFrequencyUnits(const char *v) {
    int64_t index = getTopCell()->getOrCreateSymbol(v);
    if (index == -1) return;

    frequency_unit_ = index;
    getTopCell()->addSymbolReference(frequency_unit_, this->getId());
}

void Units::setLengthUnits(const char *v) {
    int64_t index = getTopCell()->getOrCreateSymbol(v);
    if (index == -1) return;

    length_unit_ = index;
    getTopCell()->addSymbolReference(length_unit_, this->getId());
}

void Units::printLEF(std::ofstream &ofs) const {
    ofs << "UNITS\n";
    if (time_unit_ >= 0)
        ofs << "   TIME " << getTimeUnits() << " " << time_factor_ << " ;\n";
    if (capacitance_unit_ >= 0)
        ofs << "   CAPACITANCE " << getCapacitanceUnits() << " "
            << capacitance_factor_ << " ;\n";
    if (resistance_unit_ >= 0)
        ofs << "   RESISTANCE " << getResistanceUnits() << " "
            << resistance_factor_ << " ;\n";
    if (power_unit_ >= 0)
        ofs << "   POWER " << getPowerUnits() << " " << power_factor_ << " ;\n";
    if (current_unit_ >= 0)
        ofs << "   CURRENT " << getCurrentUnits() << " " << current_factor_
            << " ;\n";
    if (voltage_unit_ >= 0)
        ofs << "   VOLTAGE " << getVoltageUnits() << " " << voltage_factor_
            << " ;\n";
    if (length_unit_ >= 0)
        ofs << "   DATABASE " << getLengthUnits() << " " << length_factor_
            << " ;\n";
    if (frequency_unit_ >= 0)
        ofs << "   FREQUENCY " << getFrequencyUnits() << " "
            << frequency_factor_ << " ;\n";
    ofs << "END UNITS\n\n";
}

OStreamBase &operator<<(OStreamBase &os, Units const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");
    os << DataFieldName("from_def_") << rhs.from_def_ << DataDelimiter();
    os << DataFieldName("capacitance_factor_") << rhs.capacitance_factor_
       << DataDelimiter();
    os << DataFieldName("current_factor_") << rhs.current_factor_
       << DataDelimiter();
    os << DataFieldName("power_factor_") << rhs.power_factor_
       << DataDelimiter();
    os << DataFieldName("resistance_factor_") << rhs.resistance_factor_
       << DataDelimiter();
    os << DataFieldName("voltage_factor_") << rhs.voltage_factor_
       << DataDelimiter();
    os << DataFieldName("time_factor_") << rhs.time_factor_ << DataDelimiter();
    os << DataFieldName("frequency_factor_") << rhs.frequency_factor_
       << DataDelimiter();
    os << DataFieldName("length_factor_") << rhs.length_factor_
       << DataDelimiter();
    os << DataFieldName("capacitance_unit_") << rhs.getCapacitanceUnits()
       << DataDelimiter();
    os << DataFieldName("current_unit_") << rhs.getCurrentUnits()
       << DataDelimiter();
    os << DataFieldName("power_unit_") << rhs.getPowerUnits()
       << DataDelimiter();
    os << DataFieldName("resistance_unit_") << rhs.getResistanceUnits()
       << DataDelimiter();
    os << DataFieldName("voltage_unit_") << rhs.getVoltageUnits()
       << DataDelimiter();
    os << DataFieldName("time_unit_") << rhs.getTimeUnits() << DataDelimiter();
    os << DataFieldName("frequency_unit_") << rhs.getFrequencyUnits()
       << DataDelimiter();
    os << DataFieldName("length_unit_") << rhs.getLengthUnits()
       << DataDelimiter();
    os << DataEnd(")");
    return os;
}

}  // namespace db
}  // namespace open_edi
