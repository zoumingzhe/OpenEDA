/**
 * @file timinglib_libbuilder.h
 * @date 2020-08-26
 * @brief build the data structure
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBBUILDER_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBBUILDER_H_

#include <functional>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "db/timing/timinglib/timinglib_commondef.h"
#include "db/timing/timinglib/timinglib_structs.h"

namespace open_edi {
namespace db {
class LibSet;
class Object;
class TCell;
class TTerm;
class TPgTerm;
class TTermAttr;
class TimingArc;
class TableAxis;
}  // namespace db
}  // namespace open_edi

template <typename T>
using str_omap_t = std::unordered_map<std::string, T>;

namespace Timinglib {

class LibSyn;

#define tb_namespace open_edi::db
#define BUILD_PARAM const std::string &name, timinglib_attribute_value *v

typedef struct ObjectList {
    tb_namespace::Object *object = nullptr;
    struct ObjectList *owner = nullptr;
    struct ObjectList *next = nullptr;
} ObjectList;

typedef struct TypeGroup {
    int bit_width = 0;
    int bit_from = -1;
    int bit_to = -1;
    bool downto = false;
} TypeGroup;

class LibBuilder {
  public:
    explicit LibBuilder(tb_namespace::LibSet *libset);
    ~LibBuilder();

    // set
    void set_lib_syn(LibSyn *s);

    void begin_group(timinglib_head *);
    void end_group();

    void build_attribute(BUILD_PARAM);

  private:
    void __build_group_map();
    void __build_library(timinglib_head *);
    void __build_cell(timinglib_head *);
    void __build_pin(timinglib_head *);
    void __build_pg_pin(timinglib_head *);
    void __build_operating_conditions(timinglib_head *);
    void __build_wire_load(timinglib_head *);
    void __build_wire_load_table(timinglib_head *);
    void __build_wire_load_selection(timinglib_head *);
    void __build_lu_table_template(timinglib_head *);
    void __build_ff(timinglib_head *);
    void __build_latch(timinglib_head *);
    void __build_statetable(timinglib_head *);
    void __build_bus(timinglib_head *);
    void __build_bundle(timinglib_head *);
    void __build_type(timinglib_head *);
    void __build_timing(timinglib_head *);
    void __build_cell_rise(timinglib_head *);
    void __build_cell_fall(timinglib_head *);
    void __build_rise_transition(timinglib_head *);
    void __build_fall_transition(timinglib_head *);
    void __build_rise_constraint(timinglib_head *);
    void __build_fall_constraint(timinglib_head *);

    void __build_attr_map();
    void __build_scaling_factors_attrs();

    // library
    void __build_default_oc(BUILD_PARAM);
    void __build_time_unit(BUILD_PARAM);
    void __build_pulling_resistance_unit(BUILD_PARAM);
    void __build_resistance_unit(BUILD_PARAM);
    void __build_capacitive_load_unit(BUILD_PARAM);
    void __build_voltage_unit(BUILD_PARAM);
    void __build_current_unit(BUILD_PARAM);
    void __build_leakage_power_unit(BUILD_PARAM);
    void __build_distance_unit(BUILD_PARAM);
    void __build_nom_temperature(BUILD_PARAM);
    void __build_nom_voltage(BUILD_PARAM);
    void __build_nom_process(BUILD_PARAM);
    void __build_input_threshold_pct_fall(BUILD_PARAM);
    void __build_input_threshold_pct_rise(BUILD_PARAM);
    void __build_output_threshold_pct_fall(BUILD_PARAM);
    void __build_output_threshold_pct_rise(BUILD_PARAM);
    void __build_slew_lower_threshold_pct_fall(BUILD_PARAM);
    void __build_slew_lower_threshold_pct_rise(BUILD_PARAM);
    void __build_slew_upper_threshold_pct_fall(BUILD_PARAM);
    void __build_slew_upper_threshold_pct_rise(BUILD_PARAM);
    void __build_slew_derate_from_library(BUILD_PARAM);
    void __build_scaling_factors_attr(BUILD_PARAM);
    void __build_default_wire_load(BUILD_PARAM);
    void __build_default_wire_load_area(BUILD_PARAM);
    void __build_default_wire_load_mode(BUILD_PARAM);
    void __build_default_wire_load_capacitance(BUILD_PARAM);
    void __build_default_wire_load_resistance(BUILD_PARAM);
    void __build_default_wire_load_selection(BUILD_PARAM);
    void __build_default_input_pin_cap(BUILD_PARAM);
    void __build_default_output_pin_cap(BUILD_PARAM);
    void __build_default_inout_pin_cap(BUILD_PARAM);
    void __build_default_max_capacitance(BUILD_PARAM);
    void __build_default_max_fanout(BUILD_PARAM);
    void __build_default_max_transition(BUILD_PARAM);
    void __build_default_fanout_load(BUILD_PARAM);
    void __build_default_cell_leakage_power(BUILD_PARAM);
    void __build_voltage_map(BUILD_PARAM);

    // operating conditions
    void __build_process(BUILD_PARAM);
    void __build_temperature(BUILD_PARAM);
    void __build_voltage(BUILD_PARAM);
    void __build_tree_type(BUILD_PARAM);

    // cell
    void __build_area(BUILD_PARAM);
    void __build_cell_footprint(BUILD_PARAM);
    void __build_dont_touch(BUILD_PARAM);
    void __build_dont_use(BUILD_PARAM);
    void __build_always_on(BUILD_PARAM);
    void __build_is_macro_cell(BUILD_PARAM);
    void __build_is_pad(BUILD_PARAM);
    void __build_is_decap_cell(BUILD_PARAM);
    void __build_is_filler_cell(BUILD_PARAM);
    void __build_is_tap_cell(BUILD_PARAM);
    void __build_is_clock_gating_cell(BUILD_PARAM);
    void __build_is_clock_isolation_cell(BUILD_PARAM);
    void __build_is_isolation_cell(BUILD_PARAM);
    void __build_is_no_enable(BUILD_PARAM);
    void __build_clock_gating_integrated_cell(BUILD_PARAM);
    void __build_antenna_diode_type(BUILD_PARAM);
    void __build_is_level_shifter(BUILD_PARAM);
    void __build_cell_leakage_power(BUILD_PARAM);
    void __build_switch_cell_type(BUILD_PARAM);
    void __build_retention_cell(BUILD_PARAM);

    // pin
    void __build_related_ground_pin(BUILD_PARAM);
    void __build_related_power_pin(BUILD_PARAM);
    void __build_max_transition(BUILD_PARAM);
    void __build_fall_capacitance(BUILD_PARAM);
    void __build_rise_capacitance(BUILD_PARAM);
    void __build_three_state(BUILD_PARAM);
    void __build_capacitance(BUILD_PARAM);
    void __build_max_capacitance(BUILD_PARAM);
    void __build_min_capacitance(BUILD_PARAM);
    void __build_direction(BUILD_PARAM);
    void __build_max_fanout(BUILD_PARAM);
    void __build_min_fanout(BUILD_PARAM);
    void __build_min_transition(BUILD_PARAM);
    void __build_min_period(BUILD_PARAM);
    void __build_min_pulse_width_high(BUILD_PARAM);
    void __build_min_pulse_width_low(BUILD_PARAM);
    void __build_clock(BUILD_PARAM);
    void __build_clock_gate_clock_pin(BUILD_PARAM);
    void __build_clock_gate_enable_pin(BUILD_PARAM);
    void __build_function(BUILD_PARAM);
    void __build_members(BUILD_PARAM);
    void __build_bus_type(BUILD_PARAM);

    // pg_pin
    void __build_pg_type(BUILD_PARAM);
    void __build_voltage_name(BUILD_PARAM);

    // type
    void __build_bit_width(BUILD_PARAM);
    void __build_bit_from(BUILD_PARAM);
    void __build_bit_to(BUILD_PARAM);
    void __build_downto(BUILD_PARAM);

    // timing
    void __build_timing_sense(BUILD_PARAM);
    void __build_timing_type(BUILD_PARAM);
    void __build_when(BUILD_PARAM);
    void __build_related_pin(BUILD_PARAM);

    // wire_load
    void __build_resistance(BUILD_PARAM);
    void __build_slope(BUILD_PARAM);
    void __build_fanout_length(BUILD_PARAM);

    // wire_load_table
    void __build_fanout_capacitance(BUILD_PARAM);
    void __build_fanout_resistance(BUILD_PARAM);
    void __build_fanout_area(BUILD_PARAM);

    // wire_load_selection
    void __build_wire_load_from_area(BUILD_PARAM);

    // lu_table_template
    void __build_variable_1(BUILD_PARAM);
    void __build_variable_2(BUILD_PARAM);
    void __build_variable_3(BUILD_PARAM);
    void __build_index_1(BUILD_PARAM);
    void __build_index_2(BUILD_PARAM);
    void __build_index_3(BUILD_PARAM);

    // cell_rise cell_fall rise_transition
    // fall_transition rise_constraint fall_constraint
    void __build_values(BUILD_PARAM);

    void __get_terms_from_top_stack(
        std::vector<tb_namespace::TTerm *> *libterms);
    void __get_pg_terms_from_top_stack(
        std::vector<tb_namespace::TPgTerm *> *libpgterms);
    void __get_timingarcs_from_top_stack(
        std::vector<tb_namespace::TimingArc *> *libtimingarcs);
    void __get_objects_from_top_stack(
        std::vector<tb_namespace::Object *> *objects);
    void __delete_object_list(ObjectList *objects);
    float __get_time_unit_multiply(const std::string &u);
    float __get_resistance_unit_multiply(const std::string &u);
    float __get_capacitive_unit_multiply(const std::string &u);
    float __get_voltage_unit_multiply(const std::string &u);
    float __get_current_unit_multiply(const std::string &u);
    float __get_power_unit_multiply(const std::string &u);
    float __get_distance_unit_multiply(const std::string &u);
    void __get_pin_names_from_bus_range(const char *str,
                                        std::vector<std::string> *pinNames);
    tb_namespace::TCell *__get_tcell_by_timingarc(tb_namespace::TimingArc *t);

    bool __is_int_type(timinglib_attribute_value *);
    bool __is_double_type(timinglib_attribute_value *);
    bool __is_string_type(timinglib_attribute_value *);
    bool __is_bool_type(timinglib_attribute_value *);
    float __get_double_value(timinglib_attribute_value *);
    void __get_string_float_list(char *str, std::vector<double> *values);
    void __split_string_by_delim(char *str, const char *delim,
                                 std::vector<std::string> *rets);

    str_omap_t<std::function<void(timinglib_head *)>> group_builder_map_;
    str_omap_t<
        std::function<void(const std::string &, timinglib_attribute_value *)>>
        attr_builder_map_;

    tb_namespace::LibSet *libset_;
    std::stack<ObjectList *> objects_stack_;
    std::stack<std::string> groupname_stack_;
    bool push_status_;

    str_omap_t<
        std::tuple<tb_namespace::ScaleFactorType, tb_namespace::ScaleFactorPvt,
                   tb_namespace::ScaleFactorRiseFall>>
        scaling_factors_str_map_;

    LibSyn *libsyn_;

    bool set_resistance_unit_;
    str_omap_t<TypeGroup> type_group_lib_map_;
    str_omap_t<TypeGroup> type_group_cell_map_;
    str_omap_t<std::vector<tb_namespace::TTerm *>>
        bus_or_bundle_member_pins_map_;
    std::string cur_type_group_name_;
    std::string cur_bus_or_bundle_name_;

    tb_namespace::TableAxis *axis_[3];
};
}  // namespace Timinglib

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBBUILDER_H_
