/**
 * @file timinglib_libattr.h
 * @date 2020-08-25
 * @brief
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBATTR_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBATTR_H_

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>

#include "db/core/object_attr.h"
#include "db/timing/timinglib/timinglib_commondef.h"

namespace open_edi {
namespace db {

class OperatingConditions;

class TLibAttr : public ObjectAttr {
  public:
    /// @brief default constructor
    TLibAttr();

    /// @brief copy constructor
    TLibAttr(TLibAttr const &rhs);

    /// @brief move constructor
    TLibAttr(TLibAttr &&rhs) noexcept;

    /// @brief copy assignment
    TLibAttr &operator=(TLibAttr const &rhs);

    /// @brief move constructor
    TLibAttr &operator=(TLibAttr &&rhs) noexcept;

    /// set
    void set_name(const std::string &name);
    void set_timing_model_type(TimingModel tm);
    void set_nominal_voltage(float f);
    void set_nominal_process(float f);
    void set_nominal_temperature(float f);
    void set_input_threshold_pct_fall(float f);
    void set_input_threshold_pct_rise(float f);
    void set_output_threshold_pct_fall(float f);
    void set_output_threshold_pct_rise(float f);
    void set_slew_lower_threshold_pct_fall(float f);
    void set_slew_lower_threshold_pct_rise(float f);
    void set_slew_upper_threshold_pct_fall(float f);
    void set_slew_upper_threshold_pct_rise(float f);
    void set_slew_derate_from_library(float f);
    void set_default_wire_load_area(float f);
    void set_default_wire_load_mode(WireLoadMode w);
    void set_default_wire_load_capacitance(float f);
    void set_default_wire_load_resistance(float f);
    void set_default_input_pin_cap(float f);
    void set_default_output_pin_cap(float f);
    void set_default_inout_pin_cap(float f);
    void set_default_max_capacitance(float f);
    void set_default_max_fanout(float f);
    void set_default_max_transition(float f);
    void set_default_fanout_load(float f);
    void set_default_cell_leakage_power(float f);
    void add_supply_voltage(const std::string &name, float f);

    /// get
    std::string get_name(void) const;
    TimingModel get_timing_model_type(void);
    float get_nominal_voltage(void);
    float get_nominal_process(void);
    float get_nominal_temperature(void);
    float get_input_threshold_pct_fall(void);
    float get_input_threshold_pct_rise(void);
    float get_output_threshold_pct_fall(void);
    float get_output_threshold_pct_rise(void);
    float get_slew_lower_threshold_pct_fall(void);
    float get_slew_lower_threshold_pct_rise(void);
    float get_slew_upper_threshold_pct_fall(void);
    float get_slew_upper_threshold_pct_rise(void);
    float get_slew_derate_from_library(void);
    float get_default_wire_load_area(void);
    WireLoadMode get_default_wire_load_mode(void);
    float get_default_wire_load_capacitance(void);
    float get_default_wire_load_resistance(void);
    float get_default_input_pin_cap(void);
    float get_default_output_pin_cap(void);
    float get_default_inout_pin_cap(void);
    float get_default_max_capacitance(void);
    float get_default_max_fanout(void);
    float get_default_max_transition(void);
    float get_default_fanout_load(void);
    float get_default_cell_leakage_power(void);
    float get_supply_voltage(const std::string &name);

    /// @brief clone current object for extensibility.
    /// Users need to manage the pointer to avoid memory leakage.
    /// @return the pointer to the new object
    TLibAttr *clone() const;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

  protected:
    /// @brief copy object
    void copy(TLibAttr const &rhs);
    /// @brief move object
    void move(TLibAttr &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TLibAttr const &rhs);

    SymbolIndex name_;
    TimingModel timing_model_type_;
    float nominal_voltage_;
    float nominal_process_;
    float nominal_temperature_;
    float input_threshold_pct_fall_;
    float input_threshold_pct_rise_;
    float output_threshold_pct_fall_;
    float output_threshold_pct_rise_;
    float slew_lower_threshold_pct_fall_;
    float slew_lower_threshold_pct_rise_;
    float slew_upper_threshold_pct_fall_;
    float slew_upper_threshold_pct_rise_;
    float slew_derate_from_library_;
    float default_wire_load_area_;
    WireLoadMode default_wire_load_mode_;
    float default_wire_load_capacitance_;
    float default_wire_load_resistance_;
    float default_input_pin_cap_;
    float default_output_pin_cap_;
    float default_inout_pin_cap_;
    float default_max_capacitance_;
    float default_max_fanout_;
    float default_max_transition_;
    float default_fanout_load_;
    float default_cell_leakage_power_;
    std::unordered_map<SymbolIndex, float> supply_voltage_map_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBATTR_H_
