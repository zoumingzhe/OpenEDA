/**
 * @file timinglib_cellattr.cpp
 * @date 2020-08-26
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
#include "db/timing/timinglib/timinglib_cellattr.h"

#include "db/core/db.h"
#include "util/enums.h"

namespace open_edi {
namespace db {

TCellAttr::TCellAttr()
    : TCellAttr::ObjectAttr(),
      always_on_(false),
      dont_touch_(false),
      dont_use_(false),
      is_pad_(false),
      is_macro_cell_(false),
      is_clock_gating_cell_(false),
      is_clock_isolation_cell_(false),
      is_isolation_cell_(false),
      is_level_shifter_(false),
      is_enable_level_shifter_(false),
      is_decap_cell_(false),
      is_filler_cell_(false),
      is_tap_cell_(false),
      is_diode_cell_(false),
      is_power_switch_(false),
      is_retention_cell_(false),
      is_sequential_(false),
      is_integrated_clock_gating_cell_(false),
      is_three_state_(false),
      antenna_diode_type_(AntennaDiodeType::kUnknown),
      switch_cell_type_(SwitchCellType::kUnknown),
      threshold_votage_group_(ThresholdVotageGroup::kUnknown),
      clock_gating_integrated_cell_(ClockGateIntegratedType::kUnknown),
      name_(0),
      cell_footprint_(0),
      retention_cell_(0),
      area_(0.0),
      cell_leakage_power_(0.0) {}

TCellAttr::TCellAttr(TCellAttr const &rhs) { copy(rhs); }

TCellAttr::TCellAttr(TCellAttr &&rhs) noexcept { move(std::move(rhs)); }

TCellAttr &TCellAttr::operator=(TCellAttr const &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TCellAttr &TCellAttr::operator=(TCellAttr &&rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void TCellAttr::set_always_on(bool b) { always_on_ = b; }
void TCellAttr::set_dont_touch(bool b) { dont_touch_ = b; }
void TCellAttr::set_dont_use(bool b) { dont_use_ = b; }
void TCellAttr::set_pad(bool b) { is_pad_ = b; }
void TCellAttr::set_macro_cell(bool b) { is_macro_cell_ = b; }
void TCellAttr::set_clock_gating_cell(bool b) { is_clock_gating_cell_ = b; }
void TCellAttr::set_clock_isolation_cell(bool b) {
    is_clock_isolation_cell_ = b;
}
void TCellAttr::set_isolation_cell(bool b) { is_isolation_cell_ = b; }
void TCellAttr::set_level_shifter(bool b) { is_level_shifter_ = b; }
void TCellAttr::set_enable_level_shifter(bool b) {
    is_enable_level_shifter_ = b;
}
void TCellAttr::set_decap_cell(bool b) { is_decap_cell_ = b; }
void TCellAttr::set_filler_cell(bool b) { is_filler_cell_ = b; }
void TCellAttr::set_tap_cell(bool b) { is_tap_cell_ = b; }
void TCellAttr::set_diode_cell(bool b) { is_diode_cell_ = b; }
void TCellAttr::set_power_switch(bool b) { is_power_switch_ = b; }
void TCellAttr::set_retention_cell(bool b) { is_retention_cell_ = b; }
void TCellAttr::set_sequential(bool b) { is_sequential_ = b; }
void TCellAttr::set_integrated_clock_gating_cell(bool b) {
    is_integrated_clock_gating_cell_ = b;
}
void TCellAttr::set_three_state(bool b) { is_three_state_ = b; }
void TCellAttr::set_antenna_diode_type(AntennaDiodeType t) {
    antenna_diode_type_ = t;
}
void TCellAttr::set_switch_cell_type(SwitchCellType t) {
    switch_cell_type_ = t;
}
void TCellAttr::set_threshold_votage_group(ThresholdVotageGroup t) {
    threshold_votage_group_ = t;
}
void TCellAttr::set_clock_gating_integrated_cell(ClockGateIntegratedType t) {
    clock_gating_integrated_cell_ = t;
}
void TCellAttr::set_name(const std::string &name) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(name.c_str());
        if (index != kInvalidSymbolIndex) {
            name_ = index;
            // timing_lib->addSymbolReference(name_, this->getId());
        }
    }
}
void TCellAttr::set_cell_footprint(const std::string &cf) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(cf.c_str());
        if (index != kInvalidSymbolIndex) {
            cell_footprint_ = index;
            // timing_lib->addSymbolReference(cell_footprint_, this->getId());
        }
    }
}
void TCellAttr::set_retention_cell(const std::string &rc) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(rc.c_str());
        if (index != kInvalidSymbolIndex) {
            retention_cell_ = index;
            // timing_lib->addSymbolReference(retention_cell_, this->getId());
        }
    }
}
void TCellAttr::set_area(float f) { area_ = f; }
void TCellAttr::set_cell_leakage_power(float f) { cell_leakage_power_ = f; }
bool TCellAttr::is_always_on(void) { return always_on_; }
bool TCellAttr::is_dont_touch(void) { return dont_touch_; }
bool TCellAttr::is_dont_use(void) { return dont_use_; }
bool TCellAttr::is_pad(void) { return is_pad_; }
bool TCellAttr::is_macro_cell(void) { return is_macro_cell_; }
bool TCellAttr::is_clock_gating_cell(void) { return is_clock_gating_cell_; }
bool TCellAttr::is_clock_isolation_cell(void) {
    return is_clock_isolation_cell_;
}
bool TCellAttr::is_isolation_cell(void) { return is_isolation_cell_; }
bool TCellAttr::is_level_shifter(void) { return is_level_shifter_; }
bool TCellAttr::is_enable_level_shifter(void) {
    return is_enable_level_shifter_;
}
bool TCellAttr::is_decap_cell(void) { return is_decap_cell_; }
bool TCellAttr::is_filler_cell(void) { return is_filler_cell_; }
bool TCellAttr::is_tap_cell(void) { return is_tap_cell_; }
bool TCellAttr::is_diode_cell(void) { return is_diode_cell_; }
bool TCellAttr::is_power_switch(void) { return is_power_switch_; }
bool TCellAttr::is_retention_cell(void) { return is_retention_cell_; }
bool TCellAttr::is_sequential(void) { return is_sequential_; }
bool TCellAttr::is_integrated_clock_gating_cell(void) {
    return is_integrated_clock_gating_cell_;
}
bool TCellAttr::is_three_state(void) { return is_three_state_; }
AntennaDiodeType TCellAttr::get_antenna_diode_type(void) {
    return antenna_diode_type_;
}
SwitchCellType TCellAttr::get_switch_cell_type(void) {
    return switch_cell_type_;
}
ThresholdVotageGroup TCellAttr::get_threshold_votage_group(void) {
    return threshold_votage_group_;
}
ClockGateIntegratedType TCellAttr::get_clock_gating_integrated_cell(void) {
    return clock_gating_integrated_cell_;
}
std::string TCellAttr::get_name(void) const {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(name_);
    }
    return "";
}
SymbolIndex TCellAttr::get_name_index(void) { return name_; }
std::string TCellAttr::get_cell_footprint(void) const {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(cell_footprint_);
    }
    return "";
}
std::string TCellAttr::get_retention_cell(void) const {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(retention_cell_);
    }
    return "";
}
float TCellAttr::get_area(void) { return area_; }
float TCellAttr::get_cell_leakage_power(void) { return cell_leakage_power_; }

TCellAttr::IndexType TCellAttr::memory() const {
    IndexType ret = this->ObjectAttr::memory();

    ret += sizeof(always_on_);
    ret += sizeof(dont_touch_);
    ret += sizeof(dont_use_);
    ret += sizeof(is_pad_);
    ret += sizeof(is_macro_cell_);
    ret += sizeof(is_clock_gating_cell_);
    ret += sizeof(is_clock_isolation_cell_);
    ret += sizeof(is_isolation_cell_);
    ret += sizeof(is_level_shifter_);
    ret += sizeof(is_enable_level_shifter_);
    ret += sizeof(is_decap_cell_);
    ret += sizeof(is_filler_cell_);
    ret += sizeof(is_tap_cell_);
    ret += sizeof(is_diode_cell_);
    ret += sizeof(is_power_switch_);
    ret += sizeof(is_retention_cell_);
    ret += sizeof(is_sequential_);
    ret += sizeof(is_integrated_clock_gating_cell_);
    ret += sizeof(is_three_state_);
    ret += sizeof(is_tap_cell_);
    ret += sizeof(antenna_diode_type_);
    ret += sizeof(switch_cell_type_);
    ret += sizeof(threshold_votage_group_);
    ret += sizeof(clock_gating_integrated_cell_);
    ret += sizeof(name_);
    ret += sizeof(cell_footprint_);
    ret += sizeof(retention_cell_);
    ret += sizeof(area_);
    ret += sizeof(cell_leakage_power_);

    ret += sizeof(ret);

    return ret;
}

TCellAttr *TCellAttr::clone() const { return new TCellAttr(*this); }

void TCellAttr::copy(TCellAttr const &rhs) {
    always_on_ = rhs.always_on_;
    dont_touch_ = rhs.dont_touch_;
    dont_use_ = rhs.dont_use_;
    is_pad_ = rhs.is_pad_;
    is_macro_cell_ = rhs.is_macro_cell_;
    is_clock_gating_cell_ = rhs.is_clock_gating_cell_;
    is_clock_isolation_cell_ = rhs.is_clock_isolation_cell_;
    is_isolation_cell_ = rhs.is_isolation_cell_;
    is_level_shifter_ = rhs.is_level_shifter_;
    is_enable_level_shifter_ = rhs.is_enable_level_shifter_;
    is_decap_cell_ = rhs.is_decap_cell_;
    is_filler_cell_ = rhs.is_filler_cell_;
    is_tap_cell_ = rhs.is_tap_cell_;
    is_diode_cell_ = rhs.is_diode_cell_;
    is_power_switch_ = rhs.is_power_switch_;
    is_retention_cell_ = rhs.is_retention_cell_;
    is_sequential_ = rhs.is_sequential_;
    is_integrated_clock_gating_cell_ = rhs.is_integrated_clock_gating_cell_;
    is_three_state_ = rhs.is_three_state_;
    is_tap_cell_ = rhs.is_tap_cell_;
    antenna_diode_type_ = rhs.antenna_diode_type_;
    switch_cell_type_ = rhs.switch_cell_type_;
    threshold_votage_group_ = rhs.threshold_votage_group_;
    clock_gating_integrated_cell_ = rhs.clock_gating_integrated_cell_;
    name_ = rhs.name_;
    cell_footprint_ = rhs.cell_footprint_;
    retention_cell_ = rhs.retention_cell_;
    area_ = rhs.area_;
    cell_leakage_power_ = rhs.cell_leakage_power_;
}

void TCellAttr::move(TCellAttr &&rhs) {
    always_on_ = std::move(rhs.always_on_);
    dont_touch_ = std::move(rhs.dont_touch_);
    dont_use_ = std::move(rhs.dont_use_);
    is_pad_ = std::move(rhs.is_pad_);
    is_macro_cell_ = std::move(rhs.is_macro_cell_);
    is_clock_gating_cell_ = std::move(rhs.is_clock_gating_cell_);
    is_clock_isolation_cell_ = std::move(rhs.is_clock_isolation_cell_);
    is_isolation_cell_ = std::move(rhs.is_isolation_cell_);
    is_level_shifter_ = std::move(rhs.is_level_shifter_);
    is_enable_level_shifter_ = std::move(rhs.is_enable_level_shifter_);
    is_decap_cell_ = std::move(rhs.is_decap_cell_);
    is_filler_cell_ = std::move(rhs.is_filler_cell_);
    is_tap_cell_ = std::move(rhs.is_tap_cell_);
    is_diode_cell_ = std::move(rhs.is_diode_cell_);
    is_power_switch_ = std::move(rhs.is_power_switch_);
    is_retention_cell_ = std::move(rhs.is_retention_cell_);
    is_sequential_ = std::move(rhs.is_sequential_);
    is_integrated_clock_gating_cell_ =
        std::move(rhs.is_integrated_clock_gating_cell_);
    is_three_state_ = std::move(rhs.is_three_state_);
    is_tap_cell_ = std::move(rhs.is_tap_cell_);
    antenna_diode_type_ = std::move(rhs.antenna_diode_type_);
    switch_cell_type_ = std::move(rhs.switch_cell_type_);
    threshold_votage_group_ = std::move(rhs.threshold_votage_group_);
    clock_gating_integrated_cell_ =
        std::move(rhs.clock_gating_integrated_cell_);
    name_ = std::move(rhs.name_);
    cell_footprint_ = std::move(rhs.cell_footprint_);
    retention_cell_ = std::move(rhs.retention_cell_);
    area_ = std::move(rhs.area_);
    cell_leakage_power_ = std::move(rhs.cell_leakage_power_);
}

OStreamBase &operator<<(OStreamBase &os, TCellAttr const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    os << DataFieldName("always_on_") << rhs.always_on_ << DataDelimiter();
    os << DataFieldName("dont_touch_") << rhs.dont_touch_ << DataDelimiter();
    os << DataFieldName("dont_use_") << rhs.dont_use_ << DataDelimiter();
    os << DataFieldName("is_pad_") << rhs.is_pad_ << DataDelimiter();
    os << DataFieldName("is_macro_cell_") << rhs.is_macro_cell_
       << DataDelimiter();
    os << DataFieldName("is_clock_gating_cell_") << rhs.is_clock_gating_cell_
       << DataDelimiter();
    os << DataFieldName("is_clock_isolation_cell_")
       << rhs.is_clock_isolation_cell_ << DataDelimiter();
    os << DataFieldName("is_isolation_cell_") << rhs.is_isolation_cell_
       << DataDelimiter();
    os << DataFieldName("is_level_shifter_") << rhs.is_level_shifter_
       << DataDelimiter();
    os << DataFieldName("is_enable_level_shifter_")
       << rhs.is_enable_level_shifter_ << DataDelimiter();
    os << DataFieldName("is_decap_cell_") << rhs.is_decap_cell_
       << DataDelimiter();
    os << DataFieldName("is_filler_cell_") << rhs.is_filler_cell_
       << DataDelimiter();
    os << DataFieldName("is_tap_cell_") << rhs.is_tap_cell_ << DataDelimiter();
    os << DataFieldName("is_diode_cell_") << rhs.is_diode_cell_
       << DataDelimiter();
    os << DataFieldName("is_power_switch_") << rhs.is_power_switch_
       << DataDelimiter();
    os << DataFieldName("is_retention_cell_") << rhs.is_retention_cell_
       << DataDelimiter();
    os << DataFieldName("is_sequential_") << rhs.is_sequential_
       << DataDelimiter();
    os << DataFieldName("is_integrated_clock_gating_cell_")
       << rhs.is_integrated_clock_gating_cell_ << DataDelimiter();
    os << DataFieldName("is_three_state_") << rhs.is_three_state_
       << DataDelimiter();
    os << DataFieldName("is_tap_cell_") << rhs.is_tap_cell_ << DataDelimiter();
    os << DataFieldName("antenna_diode_type_") << rhs.antenna_diode_type_
       << DataDelimiter();
    os << DataFieldName("switch_cell_type_") << rhs.switch_cell_type_
       << DataDelimiter();
    os << DataFieldName("threshold_votage_group_")
       << rhs.threshold_votage_group_ << DataDelimiter();
    os << DataFieldName("clock_gating_integrated_cell_")
       << rhs.clock_gating_integrated_cell_ << DataDelimiter();
    os << DataFieldName("name_") << rhs.get_name() << DataDelimiter();
    os << DataFieldName("cell_footprint_") << rhs.get_cell_footprint()
       << DataDelimiter();
    os << DataFieldName("retention_cell_") << rhs.get_retention_cell()
       << DataDelimiter();
    os << DataFieldName("area_") << rhs.area_ << DataDelimiter();
    os << DataFieldName("cell_leakage_power_") << rhs.cell_leakage_power_;

    os << DataEnd(")");
    return os;
}
}  // namespace db
}  // namespace open_edi
