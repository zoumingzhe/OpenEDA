/**
 * @file timinglib_libattr.cpp
 * @date 2020-08-25
 * @brief
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/timing/timinglib/timinglib_libattr.h"

#include <map>
#include <string>

#include "db/core/db.h"

namespace open_edi {
namespace db {

TLibAttr::TLibAttr()
    : TLibAttr::ObjectAttr(),
      name_(0),
      timing_model_type_(TimingModel::kNLDM),
      nominal_voltage_(0.0),
      nominal_process_(0.0),
      nominal_temperature_(0.0),
      input_threshold_pct_fall_(0.0),
      input_threshold_pct_rise_(0.0),
      output_threshold_pct_fall_(0.0),
      output_threshold_pct_rise_(0.0),
      slew_lower_threshold_pct_fall_(0.0),
      slew_lower_threshold_pct_rise_(0.0),
      slew_upper_threshold_pct_fall_(0.0),
      slew_upper_threshold_pct_rise_(0.0),
      slew_derate_from_library_(0.0),
      default_wire_load_area_(0.0),
      default_wire_load_mode_(WireLoadMode::kUnknown),
      default_wire_load_capacitance_(0.0),
      default_wire_load_resistance_(0.0),
      default_input_pin_cap_(0.0),
      default_output_pin_cap_(0.0),
      default_inout_pin_cap_(0.0),
      default_max_capacitance_(0.0),
      default_max_fanout_(0.0),
      default_max_transition_(0.0),
      default_fanout_load_(0.0),
      default_cell_leakage_power_(0.0),
      supply_voltage_map_() {}

TLibAttr::TLibAttr(TLibAttr const &rhs) { copy(rhs); }

TLibAttr::TLibAttr(TLibAttr &&rhs) noexcept { move(std::move(rhs)); }

TLibAttr &TLibAttr::operator=(TLibAttr const &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TLibAttr &TLibAttr::operator=(TLibAttr &&rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

/// set
void TLibAttr::set_name(const std::string &name) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(name.c_str());
        if (index != kInvalidSymbolIndex) {
            name_ = index;
            // timing_lib->addSymbolReference(name_, this->getId());
        }
    }
}
void TLibAttr::set_timing_model_type(TimingModel tm) {
    timing_model_type_ = tm;
}
void TLibAttr::set_nominal_voltage(float f) { nominal_voltage_ = f; }
void TLibAttr::set_nominal_process(float f) { nominal_process_ = f; }
void TLibAttr::set_nominal_temperature(float f) { nominal_temperature_ = f; }
void TLibAttr::set_input_threshold_pct_fall(float f) {
    input_threshold_pct_fall_ = f;
}
void TLibAttr::set_input_threshold_pct_rise(float f) {
    input_threshold_pct_rise_ = f;
}
void TLibAttr::set_output_threshold_pct_fall(float f) {
    output_threshold_pct_fall_ = f;
}
void TLibAttr::set_output_threshold_pct_rise(float f) {
    output_threshold_pct_rise_ = f;
}
void TLibAttr::set_slew_lower_threshold_pct_fall(float f) {
    slew_lower_threshold_pct_fall_ = f;
}
void TLibAttr::set_slew_lower_threshold_pct_rise(float f) {
    slew_lower_threshold_pct_rise_ = f;
}
void TLibAttr::set_slew_upper_threshold_pct_fall(float f) {
    slew_upper_threshold_pct_fall_ = f;
}
void TLibAttr::set_slew_upper_threshold_pct_rise(float f) {
    slew_upper_threshold_pct_rise_ = f;
}
void TLibAttr::set_slew_derate_from_library(float f) {
    slew_derate_from_library_ = f;
}
void TLibAttr::set_default_wire_load_area(float f) {
    default_wire_load_area_ = f;
}
void TLibAttr::set_default_wire_load_mode(WireLoadMode w) {
    default_wire_load_mode_ = w;
}
void TLibAttr::set_default_wire_load_capacitance(float f) {
    default_wire_load_capacitance_ = f;
}
void TLibAttr::set_default_wire_load_resistance(float f) {
    default_wire_load_resistance_ = f;
}
void TLibAttr::set_default_input_pin_cap(float f) {
    default_input_pin_cap_ = f;
}
void TLibAttr::set_default_output_pin_cap(float f) {
    default_output_pin_cap_ = f;
}
void TLibAttr::set_default_inout_pin_cap(float f) {
    default_inout_pin_cap_ = f;
}
void TLibAttr::set_default_max_capacitance(float f) {
    default_max_capacitance_ = f;
}
void TLibAttr::set_default_max_fanout(float f) { default_max_fanout_ = f; }
void TLibAttr::set_default_max_transition(float f) {
    default_max_transition_ = f;
}
void TLibAttr::set_default_fanout_load(float f) { default_fanout_load_ = f; }
void TLibAttr::set_default_cell_leakage_power(float f) {
    default_cell_leakage_power_ = f;
}
void TLibAttr::add_supply_voltage(const std::string &name, float f) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(name.c_str());
        if (index != kInvalidSymbolIndex) {
            name_ = index;
            supply_voltage_map_[name_] = f;
            // timing_lib->addSymbolReference(name_, this->getId());
        }
    }
}

/// get
std::string TLibAttr::get_name(void) const {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(name_);
    }
    return "";
}
TimingModel TLibAttr::get_timing_model_type(void) { return timing_model_type_; }
float TLibAttr::get_nominal_voltage(void) { return nominal_voltage_; }
float TLibAttr::get_nominal_process(void) { return nominal_process_; }
float TLibAttr::get_nominal_temperature(void) { return nominal_temperature_; }
float TLibAttr::get_input_threshold_pct_fall(void) {
    return input_threshold_pct_fall_;
}
float TLibAttr::get_input_threshold_pct_rise(void) {
    return input_threshold_pct_rise_;
}
float TLibAttr::get_output_threshold_pct_fall(void) {
    return output_threshold_pct_fall_;
}
float TLibAttr::get_output_threshold_pct_rise(void) {
    return output_threshold_pct_rise_;
}
float TLibAttr::get_slew_lower_threshold_pct_fall(void) {
    return slew_lower_threshold_pct_fall_;
}
float TLibAttr::get_slew_lower_threshold_pct_rise(void) {
    return slew_lower_threshold_pct_rise_;
}
float TLibAttr::get_slew_upper_threshold_pct_fall(void) {
    return slew_upper_threshold_pct_fall_;
}
float TLibAttr::get_slew_upper_threshold_pct_rise(void) {
    return slew_upper_threshold_pct_rise_;
}
float TLibAttr::get_slew_derate_from_library(void) {
    return slew_derate_from_library_;
}
float TLibAttr::get_default_wire_load_area(void) {
    return default_wire_load_area_;
}
WireLoadMode TLibAttr::get_default_wire_load_mode(void) {
    return default_wire_load_mode_;
}
float TLibAttr::get_default_wire_load_capacitance(void) {
    return default_wire_load_capacitance_;
}
float TLibAttr::get_default_wire_load_resistance(void) {
    return default_wire_load_resistance_;
}
float TLibAttr::get_default_input_pin_cap(void) {
    return default_input_pin_cap_;
}
float TLibAttr::get_default_output_pin_cap(void) {
    return default_output_pin_cap_;
}
float TLibAttr::get_default_inout_pin_cap(void) {
    return default_inout_pin_cap_;
}
float TLibAttr::get_default_max_capacitance(void) {
    return default_max_capacitance_;
}
float TLibAttr::get_default_max_fanout(void) { return default_max_fanout_; }
float TLibAttr::get_default_max_transition(void) {
    return default_max_transition_;
}
float TLibAttr::get_default_fanout_load(void) { return default_fanout_load_; }
float TLibAttr::get_default_cell_leakage_power(void) {
    return default_cell_leakage_power_;
}
float TLibAttr::get_supply_voltage(const std::string &name) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(name.c_str());
        if (index != kInvalidSymbolIndex) {
            auto it = supply_voltage_map_.find(index);
            if (it != supply_voltage_map_.end()) return it->second;
        }
    }
    return 0.0;
}

TLibAttr::IndexType TLibAttr::memory() const {
    IndexType ret = this->ObjectAttr::memory();

    ret += sizeof(name_);
    ret += sizeof(timing_model_type_);
    ret += sizeof(nominal_voltage_);
    ret += sizeof(nominal_process_);
    ret += sizeof(nominal_temperature_);
    ret += sizeof(input_threshold_pct_fall_);
    ret += sizeof(input_threshold_pct_rise_);
    ret += sizeof(output_threshold_pct_fall_);
    ret += sizeof(output_threshold_pct_rise_);
    ret += sizeof(slew_lower_threshold_pct_fall_);
    ret += sizeof(slew_lower_threshold_pct_rise_);
    ret += sizeof(slew_upper_threshold_pct_fall_);
    ret += sizeof(slew_upper_threshold_pct_rise_);
    ret += sizeof(slew_derate_from_library_);
    ret += sizeof(default_wire_load_area_);
    ret += sizeof(default_wire_load_mode_);
    ret += sizeof(default_wire_load_capacitance_);
    ret += sizeof(default_wire_load_resistance_);
    ret += sizeof(default_input_pin_cap_);
    ret += sizeof(default_output_pin_cap_);
    ret += sizeof(default_inout_pin_cap_);
    ret += sizeof(default_max_capacitance_);
    ret += sizeof(default_max_fanout_);
    ret += sizeof(default_max_transition_);
    ret += sizeof(default_fanout_load_);
    ret += sizeof(default_cell_leakage_power_);

    return ret;
}

TLibAttr *TLibAttr::clone() const { return new TLibAttr(*this); }

void TLibAttr::copy(TLibAttr const &rhs) {
    name_ = rhs.name_;
    timing_model_type_ = rhs.timing_model_type_;
    nominal_voltage_ = rhs.nominal_voltage_;
    nominal_process_ = rhs.nominal_process_;
    nominal_temperature_ = rhs.nominal_temperature_;
    input_threshold_pct_fall_ = rhs.input_threshold_pct_fall_;
    input_threshold_pct_rise_ = rhs.input_threshold_pct_rise_;
    output_threshold_pct_fall_ = rhs.output_threshold_pct_fall_;
    output_threshold_pct_rise_ = rhs.output_threshold_pct_rise_;
    slew_lower_threshold_pct_fall_ = rhs.slew_lower_threshold_pct_fall_;
    slew_lower_threshold_pct_rise_ = rhs.slew_lower_threshold_pct_rise_;
    slew_upper_threshold_pct_fall_ = rhs.slew_upper_threshold_pct_fall_;
    slew_upper_threshold_pct_rise_ = rhs.slew_upper_threshold_pct_rise_;
    slew_derate_from_library_ = rhs.slew_derate_from_library_;
    default_wire_load_area_ = rhs.default_wire_load_area_;
    default_wire_load_mode_ = rhs.default_wire_load_mode_;
    default_wire_load_capacitance_ = rhs.default_wire_load_capacitance_;
    default_wire_load_resistance_ = rhs.default_wire_load_resistance_;
    default_input_pin_cap_ = rhs.default_input_pin_cap_;
    default_output_pin_cap_ = rhs.default_output_pin_cap_;
    default_inout_pin_cap_ = rhs.default_inout_pin_cap_;
    default_max_capacitance_ = rhs.default_max_capacitance_;
    default_max_fanout_ = rhs.default_max_fanout_;
    default_max_transition_ = rhs.default_max_transition_;
    default_fanout_load_ = rhs.default_fanout_load_;
    default_cell_leakage_power_ = rhs.default_cell_leakage_power_;
    supply_voltage_map_ = rhs.supply_voltage_map_;
}

void TLibAttr::move(TLibAttr &&rhs) {
    name_ = std::move(rhs.name_);
    timing_model_type_ = std::move(rhs.timing_model_type_);
    nominal_voltage_ = std::move(rhs.nominal_voltage_);
    nominal_process_ = std::move(rhs.nominal_process_);
    nominal_temperature_ = std::move(rhs.nominal_temperature_);
    input_threshold_pct_fall_ = std::move(rhs.input_threshold_pct_fall_);
    input_threshold_pct_rise_ = std::move(rhs.input_threshold_pct_rise_);
    output_threshold_pct_fall_ = std::move(rhs.output_threshold_pct_fall_);
    output_threshold_pct_rise_ = std::move(rhs.output_threshold_pct_rise_);
    slew_lower_threshold_pct_fall_ =
        std::move(rhs.slew_lower_threshold_pct_fall_);
    slew_lower_threshold_pct_rise_ =
        std::move(rhs.slew_lower_threshold_pct_rise_);
    slew_upper_threshold_pct_fall_ =
        std::move(rhs.slew_upper_threshold_pct_fall_);
    slew_upper_threshold_pct_rise_ =
        std::move(rhs.slew_upper_threshold_pct_rise_);
    slew_derate_from_library_ = std::move(rhs.slew_derate_from_library_);
    default_wire_load_area_ = std::move(rhs.default_wire_load_area_);
    default_wire_load_mode_ = std::move(rhs.default_wire_load_mode_);
    default_wire_load_capacitance_ =
        std::move(rhs.default_wire_load_capacitance_);
    default_wire_load_resistance_ =
        std::move(rhs.default_wire_load_resistance_);
    default_input_pin_cap_ = std::move(rhs.default_input_pin_cap_);
    default_output_pin_cap_ = std::move(rhs.default_output_pin_cap_);
    default_inout_pin_cap_ = std::move(rhs.default_inout_pin_cap_);
    default_max_capacitance_ = std::move(rhs.default_max_capacitance_);
    default_max_fanout_ = std::move(rhs.default_max_fanout_);
    default_max_transition_ = std::move(rhs.default_max_transition_);
    default_fanout_load_ = std::move(rhs.default_fanout_load_);
    default_cell_leakage_power_ = std::move(rhs.default_cell_leakage_power_);
    supply_voltage_map_ = std::move(rhs.supply_voltage_map_);
}

OStreamBase &operator<<(OStreamBase &os, TLibAttr const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    os << DataFieldName("name_") << rhs.get_name() << DataDelimiter();
    os << DataFieldName("timing_model_type_") << rhs.timing_model_type_
       << DataDelimiter();
    os << DataFieldName("nominal_voltage_") << rhs.nominal_voltage_
       << DataDelimiter();
    os << DataFieldName("nominal_process_") << rhs.nominal_process_
       << DataDelimiter();
    os << DataFieldName("nominal_temperature_") << rhs.nominal_temperature_
       << DataDelimiter();
    os << DataFieldName("input_threshold_pct_fall_")
       << rhs.input_threshold_pct_fall_ << DataDelimiter();
    os << DataFieldName("input_threshold_pct_rise_")
       << rhs.input_threshold_pct_rise_ << DataDelimiter();
    os << DataFieldName("output_threshold_pct_fall_")
       << rhs.output_threshold_pct_fall_ << DataDelimiter();
    os << DataFieldName("output_threshold_pct_rise_")
       << rhs.output_threshold_pct_rise_ << DataDelimiter();
    os << DataFieldName("slew_lower_threshold_pct_fall_")
       << rhs.slew_lower_threshold_pct_fall_ << DataDelimiter();
    os << DataFieldName("slew_lower_threshold_pct_rise_")
       << rhs.slew_lower_threshold_pct_rise_ << DataDelimiter();
    os << DataFieldName("slew_upper_threshold_pct_fall_")
       << rhs.slew_upper_threshold_pct_fall_ << DataDelimiter();
    os << DataFieldName("slew_upper_threshold_pct_rise_")
       << rhs.slew_upper_threshold_pct_rise_ << DataDelimiter();
    os << DataFieldName("slew_derate_from_library_")
       << rhs.slew_derate_from_library_ << DataDelimiter();
    os << DataFieldName("default_wire_load_area_")
       << rhs.default_wire_load_area_ << DataDelimiter();
    os << DataFieldName("default_wire_load_mode_")
       << rhs.default_wire_load_mode_ << DataDelimiter();
    os << DataFieldName("default_wire_load_capacitance_")
       << rhs.default_wire_load_capacitance_ << DataDelimiter();
    os << DataFieldName("default_wire_load_resistance_")
       << rhs.default_wire_load_resistance_ << DataDelimiter();
    os << DataFieldName("default_input_pin_cap_") << rhs.default_input_pin_cap_
       << DataDelimiter();
    os << DataFieldName("default_output_pin_cap_")
       << rhs.default_output_pin_cap_ << DataDelimiter();
    os << DataFieldName("default_inout_pin_cap_") << rhs.default_inout_pin_cap_
       << DataDelimiter();
    os << DataFieldName("default_max_capacitance_")
       << rhs.default_max_capacitance_ << DataDelimiter();
    os << DataFieldName("default_max_fanout_") << rhs.default_max_fanout_
       << DataDelimiter();
    os << DataFieldName("default_max_transition_")
       << rhs.default_max_transition_ << DataDelimiter();
    os << DataFieldName("default_fanout_load_") << rhs.default_fanout_load_
       << DataDelimiter();
    os << DataFieldName("default_cell_leakage_power_")
       << rhs.default_cell_leakage_power_ << DataDelimiter();
    os << DataFieldName("supply_voltages_");

    std::map<std::string, float> sorted_map;
    Timing *timing_lib = getTimingLib();
    for (auto it = rhs.supply_voltage_map_.begin();
         it != rhs.supply_voltage_map_.end(); it++) {
        if (timing_lib) {
            std::string &str = timing_lib->getSymbolByIndex(it->first);
            if (str != "") sorted_map[str] = it->second;
        }
    }
    os << sorted_map.size();
    os << DataBegin("[");
    auto delimiter = DataDelimiter("");
    for (auto it = sorted_map.begin(); it != sorted_map.end(); ++it) {
        os << delimiter << it->first << DataDelimiter() << it->second;
        delimiter = DataDelimiter();
    }
    os << DataEnd("]");

    os << DataEnd(")");
    return os;
}
}  // namespace db
}  // namespace open_edi
