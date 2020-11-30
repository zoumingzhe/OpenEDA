/**
 * @file timinglib_libbuilder.cpp
 * @date 2020-08-26
 * @brief build the data structure
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/timing/timinglib/timinglib_libbuilder.h"

#include <regex.h>
#include <string.h>

#include <algorithm>

#include "db/core/db.h"
#include "db/timing/timinglib/timinglib_cell.h"
#include "db/timing/timinglib/timinglib_lib.h"
#include "db/timing/timinglib/timinglib_libanalysis.h"
#include "db/timing/timinglib/libset.h"
#include "db/timing/timinglib/timinglib_libsyn.h"
#include "db/timing/timinglib/timinglib_opcond.h"
#include "db/timing/timinglib/timinglib_pgterm.h"
#include "db/timing/timinglib/timinglib_scalefactors.h"
#include "db/timing/timinglib/timinglib_tabletemplate.h"
#include "db/timing/timinglib/timinglib_term.h"
#include "db/timing/timinglib/timinglib_timingarc.h"
#include "db/timing/timinglib/timinglib_timingtable.h"
#include "db/timing/timinglib/timinglib_units.h"
#include "db/timing/timinglib/timinglib_wireload.h"
#include "util/enums.h"

namespace Timinglib {

#define BINDG(T) std::bind(&LibBuilder::T, this, std::placeholders::_1)
#define BINDA(T)                                           \
    std::bind(&LibBuilder::T, this, std::placeholders::_1, \
              std::placeholders::_2)
#define GOBJECTS                                 \
    std::vector<tb_namespace::Object *> objects; \
    __get_objects_from_top_stack(&objects);      \
    if (objects.empty()) return;

LibBuilder::LibBuilder(tb_namespace::LibSet *libs)
    : group_builder_map_(),
      attr_builder_map_(),
      libset_(libs),
      objects_stack_(),
      groupname_stack_(),
      push_status_(0),
      scaling_factors_str_map_(),
      libsyn_(nullptr),
      set_resistance_unit_(false),
      type_group_lib_map_(),
      type_group_cell_map_(),
      bus_or_bundle_member_pins_map_(),
      cur_type_group_name_(""),
      cur_bus_or_bundle_name_("") {
    ObjectList *objects = new ObjectList();
    objects->object = libset_;
    objects_stack_.push(objects);

    axis_[0] = nullptr;
    axis_[1] = nullptr;
    axis_[2] = nullptr;

    __build_group_map();
    __build_attr_map();
}

LibBuilder::~LibBuilder() {
    group_builder_map_.clear();
    attr_builder_map_.clear();

    while (!objects_stack_.empty()) {
        ObjectList *objects = objects_stack_.top();
        if (objects != nullptr) __delete_object_list(objects);
        objects_stack_.pop();
    }
    while (!groupname_stack_.empty()) {
        groupname_stack_.pop();
    }
    scaling_factors_str_map_.clear();
}

void LibBuilder::set_lib_syn(LibSyn *s) { libsyn_ = s; }
void LibBuilder::__build_group_map(void) {
    group_builder_map_ = {
        // library
        {"library", BINDG(__build_library)},
        {"operating_conditions", BINDG(__build_operating_conditions)},
        {"wire_load", BINDG(__build_wire_load)},
        {"wire_load_table", BINDG(__build_wire_load_table)},
        {"wire_load_selection", BINDG(__build_wire_load_selection)},
        {"lu_table_template", BINDG(__build_lu_table_template)},

        // cell
        {"cell", BINDG(__build_cell)},
        {"pin", BINDG(__build_pin)},
        {"pg_pin", BINDG(__build_pg_pin)},
        {"ff", BINDG(__build_ff)},
        {"latch", BINDG(__build_latch)},
        {"statetable", BINDG(__build_statetable)},
        {"bus", BINDG(__build_bus)},
        {"bundle", BINDG(__build_bundle)},

        // cell or library
        {"type", BINDG(__build_type)},

        // bus, bundle or pin
        {"timing", BINDG(__build_timing)},
        {"cell_rise", BINDG(__build_cell_rise)},
        {"cell_fall", BINDG(__build_cell_fall)},
        {"rise_transition", BINDG(__build_rise_transition)},
        {"fall_transition", BINDG(__build_fall_transition)},
        {"rise_constraint", BINDG(__build_rise_constraint)},
        {"fall_constraint", BINDG(__build_fall_constraint)},
    };
}

void LibBuilder::__build_attr_map(void) {
    attr_builder_map_ = {
        // library
        {"default_operating_conditions", BINDA(__build_default_oc)},
        {"time_unit", BINDA(__build_time_unit)},
        {"pulling_resistance_unit", BINDA(__build_pulling_resistance_unit)},
        {"resistance_unit", BINDA(__build_resistance_unit)},
        {"capacitive_load_unit", BINDA(__build_capacitive_load_unit)},
        {"voltage_unit", BINDA(__build_voltage_unit)},
        {"current_unit", BINDA(__build_current_unit)},
        {"leakage_power_unit", BINDA(__build_leakage_power_unit)},
        {"distance_unit", BINDA(__build_distance_unit)},
        {"nom_temperature", BINDA(__build_nom_temperature)},
        {"nom_voltage", BINDA(__build_nom_voltage)},
        {"nom_process", BINDA(__build_nom_process)},
        {"input_threshold_pct_fall", BINDA(__build_input_threshold_pct_fall)},
        {"input_threshold_pct_rise", BINDA(__build_input_threshold_pct_rise)},
        {"output_threshold_pct_fall", BINDA(__build_output_threshold_pct_fall)},
        {"output_threshold_pct_rise", BINDA(__build_output_threshold_pct_rise)},
        {"slew_lower_threshold_pct_fall",
         BINDA(__build_slew_lower_threshold_pct_fall)},
        {"slew_lower_threshold_pct_rise",
         BINDA(__build_slew_lower_threshold_pct_rise)},
        {"slew_upper_threshold_pct_fall",
         BINDA(__build_slew_upper_threshold_pct_fall)},
        {"slew_upper_threshold_pct_rise",
         BINDA(__build_slew_upper_threshold_pct_rise)},
        {"slew_derate_from_library", BINDA(__build_slew_derate_from_library)},
        {"default_wire_load", BINDA(__build_default_wire_load)},
        {"default_wire_load_area", BINDA(__build_default_wire_load_area)},
        {"default_wire_load_mode", BINDA(__build_default_wire_load_mode)},
        {"default_wire_load_capacitance",
         BINDA(__build_default_wire_load_capacitance)},
        {"default_wire_load_resistance",
         BINDA(__build_default_wire_load_resistance)},
        {"default_wire_load_selection",
         BINDA(__build_default_wire_load_selection)},
        {"default_input_pin_cap", BINDA(__build_default_input_pin_cap)},
        {"default_output_pin_cap", BINDA(__build_default_output_pin_cap)},
        {"default_inout_pin_cap", BINDA(__build_default_inout_pin_cap)},
        {"default_max_capacitance", BINDA(__build_default_max_capacitance)},
        {"default_max_fanout", BINDA(__build_default_max_fanout)},
        {"default_max_transition", BINDA(__build_default_max_transition)},
        {"default_fanout_load", BINDA(__build_default_fanout_load)},
        {"default_cell_leakage_power",
         BINDA(__build_default_cell_leakage_power)},
        {"voltage_map", BINDA(__build_voltage_map)},

        // operating conditions
        {"process", BINDA(__build_process)},
        {"temperature", BINDA(__build_temperature)},
        {"voltage", BINDA(__build_voltage)},
        {"tree_type", BINDA(__build_tree_type)},

        // cell
        {"area", BINDA(__build_area)},
        {"cell_footprint", BINDA(__build_cell_footprint)},
        {"dont_touch", BINDA(__build_dont_touch)},
        {"dont_use", BINDA(__build_dont_use)},
        {"always_on", BINDA(__build_always_on)},
        {"is_macro_cell", BINDA(__build_is_macro_cell)},
        {"is_pad", BINDA(__build_is_pad)},
        {"is_decap_cell", BINDA(__build_is_decap_cell)},
        {"is_filler_cell", BINDA(__build_is_filler_cell)},
        {"is_tap_cell", BINDA(__build_is_tap_cell)},
        {"is_clock_gating_cell", BINDA(__build_is_clock_gating_cell)},
        {"is_clock_isolation_cell", BINDA(__build_is_clock_isolation_cell)},
        {"is_isolation_cell", BINDA(__build_is_isolation_cell)},
        {"is_no_enable", BINDA(__build_is_no_enable)},
        {"clock_gating_integrated_cell",
         BINDA(__build_clock_gating_integrated_cell)},
        {"antenna_diode_type", BINDA(__build_antenna_diode_type)},
        {"is_level_shifter", BINDA(__build_is_level_shifter)},
        {"cell_leakage_power", BINDA(__build_cell_leakage_power)},
        {"switch_cell_type", BINDA(__build_switch_cell_type)},
        {"retention_cell", BINDA(__build_retention_cell)},

        // pin
        {"related_ground_pin", BINDA(__build_related_ground_pin)},
        {"related_power_pin", BINDA(__build_related_power_pin)},
        {"max_transition", BINDA(__build_max_transition)},
        {"fall_capacitance", BINDA(__build_fall_capacitance)},
        {"rise_capacitance", BINDA(__build_rise_capacitance)},
        {"three_state", BINDA(__build_three_state)},
        {"capacitance", BINDA(__build_capacitance)},
        {"max_capacitance", BINDA(__build_max_capacitance)},
        {"min_capacitance", BINDA(__build_min_capacitance)},
        {"direction", BINDA(__build_direction)},
        {"max_fanout", BINDA(__build_max_fanout)},
        {"min_fanout", BINDA(__build_min_fanout)},
        {"min_transition", BINDA(__build_min_transition)},
        {"min_period", BINDA(__build_min_period)},
        {"min_pulse_width_high", BINDA(__build_min_pulse_width_high)},
        {"min_pulse_width_low", BINDA(__build_min_pulse_width_low)},
        {"clock", BINDA(__build_clock)},
        {"clock_gate_clock_pin", BINDA(__build_clock_gate_clock_pin)},
        {"clock_gate_enable_pin", BINDA(__build_clock_gate_enable_pin)},
        {"function", BINDA(__build_function)},

        // bundle
        {"members", BINDA(__build_members)},

        // bus
        {"bus_type", BINDA(__build_bus_type)},

        // pg_pin
        {"pg_type", BINDA(__build_pg_type)},
        {"voltage_name", BINDA(__build_voltage_name)},

        // type
        {"bit_width", BINDA(__build_bit_width)},
        {"bit_from", BINDA(__build_bit_from)},
        {"bit_to", BINDA(__build_bit_to)},
        {"downto", BINDA(__build_downto)},

        // timing
        {"timing_sense", BINDA(__build_timing_sense)},
        {"timing_type", BINDA(__build_timing_type)},
        {"when", BINDA(__build_when)},
        {"related_pin", BINDA(__build_related_pin)},

        // wire_load
        {"resistance", BINDA(__build_resistance)},
        {"slope", BINDA(__build_slope)},
        {"fanout_length", BINDA(__build_fanout_length)},

        // wire_load_table
        {"fanout_capacitance", BINDA(__build_fanout_capacitance)},
        {"fanout_resistance", BINDA(__build_fanout_resistance)},
        {"fanout_area", BINDA(__build_fanout_area)},

        // wire_load_selection
        {"wire_load_from_area", BINDA(__build_wire_load_from_area)},

        // lu_table_template
        {"variable_1", BINDA(__build_variable_1)},
        {"variable_2", BINDA(__build_variable_2)},
        {"variable_3", BINDA(__build_variable_3)},
        {"index_1", BINDA(__build_index_1)},
        {"index_2", BINDA(__build_index_2)},
        {"index_3", BINDA(__build_index_3)},

        // cell_rise cell_fall rise_transition
        // fall_transition rise_constraint fall_constraint
        {"values", BINDA(__build_values)},
    };
    __build_scaling_factors_attrs();
}

void LibBuilder::__build_scaling_factors_attrs(void) {
    for (int i = 0;
         i < static_cast<int>(tb_namespace::ScaleFactorType::kUnknown); ++i) {
        tb_namespace::ScaleFactorType type =
            static_cast<tb_namespace::ScaleFactorType>(i);
        for (int j = 0;
             j < static_cast<int>(tb_namespace::ScaleFactorPvt::kUnknown);
             ++j) {
            tb_namespace::ScaleFactorPvt pvt =
                static_cast<tb_namespace::ScaleFactorPvt>(j);
            if (tb_namespace::ScaleFactors::is_rise_fall_suffix(type)) {
                for (int k = 0;
                     k < static_cast<int>(
                             tb_namespace::ScaleFactorRiseFall::kUnknown);
                     ++k) {
                    tb_namespace::ScaleFactorRiseFall rf =
                        static_cast<tb_namespace::ScaleFactorRiseFall>(k);
                    std::string name =
                        string_format("k_%s_%s_%s", toString(pvt).c_str(),
                                      toString(type).c_str(), toString(rf));
                    scaling_factors_str_map_[name] =
                        std::make_tuple(type, pvt, rf);
                }
            } else if (tb_namespace::ScaleFactors::is_rise_fall_prefix(type)) {
                for (int k = 0;
                     k < static_cast<int>(
                             tb_namespace::ScaleFactorRiseFall::kUnknown);
                     ++k) {
                    tb_namespace::ScaleFactorRiseFall rf =
                        static_cast<tb_namespace::ScaleFactorRiseFall>(k);
                    std::string name =
                        string_format("k_%s_%s_%s", toString(pvt).c_str(),
                                      toString(rf), toString(type).c_str());
                    scaling_factors_str_map_[name] =
                        std::make_tuple(type, pvt, rf);
                }
            } else if (tb_namespace::ScaleFactors::is_high_low_suffix(type)) {
                for (int k = 0;
                     k < static_cast<int>(
                             tb_namespace::ScaleFactorRiseFall::kUnknown);
                     ++k) {
                    tb_namespace::ScaleFactorRiseFall rf =
                        static_cast<tb_namespace::ScaleFactorRiseFall>(k);
                    if (rf == tb_namespace::ScaleFactorRiseFall::kRise) {
                        std::string name =
                            string_format("k_%s_%s_%s", toString(pvt).c_str(),
                                          "high", toString(type).c_str());
                        scaling_factors_str_map_[name] =
                            std::make_tuple(type, pvt, rf);
                    } else if (rf == tb_namespace::ScaleFactorRiseFall::kFall) {
                        std::string name =
                            string_format("k_%s_%s_%s", toString(pvt).c_str(),
                                          "low", toString(type).c_str());
                        scaling_factors_str_map_[name] =
                            std::make_tuple(type, pvt, rf);
                    }
                }
            } else {
                std::string name = string_format(
                    "k_%s_%s", toString(pvt).c_str(), toString(type).c_str());
                scaling_factors_str_map_[name] = std::make_tuple(
                    type, pvt, tb_namespace::ScaleFactorRiseFall::kUnknown);
            }
        }
    }
    for (auto &p : scaling_factors_str_map_) {
        attr_builder_map_[p.first] = BINDA(__build_scaling_factors_attr);
    }
}

void LibBuilder::begin_group(timinglib_head *h) {
    push_status_ = 0;
    std::string str = h->name;
    auto findIt = group_builder_map_.find(str);
    if (findIt != group_builder_map_.end()) (findIt->second)(h);
    if (push_status_ == 0) {
        ObjectList *objects = new ObjectList();
        if (!objects_stack_.empty()) objects->owner = objects_stack_.top();
        objects_stack_.push(objects);
    }

    groupname_stack_.push(str);
}

void LibBuilder::end_group() {
    if (groupname_stack_.top() == "library") {
        if (set_resistance_unit_ == false) {
            std::vector<tb_namespace::Object *> objects;
            __get_objects_from_top_stack(&objects);
            for (auto &object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTLib) {
                    tb_namespace::TLib *lib =
                        static_cast<tb_namespace::TLib *>(object);
                    tb_namespace::TUnits *units = lib->get_units();
                    if (units) {
                        units->get_resistance_unit() =
                            units->get_pulling_resistance_unit();
                    }
                }
            }
        }
        type_group_lib_map_.clear();
    } else if (groupname_stack_.top() == "cell") {
        type_group_cell_map_.clear();
        bus_or_bundle_member_pins_map_.clear();
    } else if (groupname_stack_.top() == "bus" ||
               groupname_stack_.top() == "bundle") {
        cur_bus_or_bundle_name_ = "";
    } else if (groupname_stack_.top() == "type") {
        cur_type_group_name_ = "";
    } else if (groupname_stack_.top() == "cell_rise" ||
               groupname_stack_.top() == "cell_fall" ||
               groupname_stack_.top() == "rise_transition" ||
               groupname_stack_.top() == "fall_transition" ||
               groupname_stack_.top() == "rise_constraint" ||
               groupname_stack_.top() == "fall_constraint") {
        axis_[0] = nullptr;
        axis_[1] = nullptr;
        axis_[2] = nullptr;
    }

    objects_stack_.pop();
    groupname_stack_.pop();
}

void LibBuilder::build_attribute(BUILD_PARAM) {
    auto findIt = attr_builder_map_.find(name);
    if (findIt != attr_builder_map_.end()) (findIt->second)(name, v);
}

void LibBuilder::__build_library(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__is_string_type(v)) {
            std::string str = v->u.string_val;
            tb_namespace::TLib *lib = libset_->addTLib(h->filename, str);
            // lib->getAttr()->set_filename(h->filename);

            std::string upper;
            std::transform(str.begin(), str.end(), std::back_inserter(upper),
                           ::toupper);

            if (upper.find("NLDM") != std::string::npos)
                lib->getAttr()->set_timing_model_type(
                    tb_namespace::TimingModel::kNLDM);
            else if (upper.find("CSS") != std::string::npos)
                lib->getAttr()->set_timing_model_type(
                    tb_namespace::TimingModel::kCCS);
            else if (upper.find("ECSM") != std::string::npos)
                lib->getAttr()->set_timing_model_type(
                    tb_namespace::TimingModel::kECSM);

            ObjectList *objects = new ObjectList();
            objects->object = lib;
            if (!objects_stack_.empty()) objects->owner = objects_stack_.top();
            objects_stack_.push(objects);
            push_status_ = 1;
        }
    }
}

void LibBuilder::__build_cell(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__is_string_type(v)) {
            GOBJECTS
            for (auto object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTLib) {
                    tb_namespace::TCell *cell =
                        static_cast<tb_namespace::TLib *>(object)
                            ->add_timing_cell(v->u.string_val);

                    ObjectList *objects = new ObjectList();
                    objects->object = cell;
                    if (!objects_stack_.empty())
                        objects->owner = objects_stack_.top();
                    objects_stack_.push(objects);
                    push_status_ = 1;
                }
            }
        }
    }
}

void LibBuilder::__build_pin(timinglib_head *h) {
    bool is_bus = false;
    bool is_bundle = false;
    if (!groupname_stack_.empty()) {
        std::string lastgroup_name = groupname_stack_.top();
        if (lastgroup_name == "bus")
            is_bus = true;
        else if (lastgroup_name == "bundle")
            is_bundle = true;
    }

    std::vector<std::string> terms;
    for (auto v = h->list; v; v = v->next) {
        if (__is_string_type(v)) {
#if 1
            if (is_bus) {
                char *content = v->u.string_val;
                __get_pin_names_from_bus_range(content, &terms);
                continue;
            }
#endif
            terms.emplace_back(v->u.string_val);
        }
    }
    if (!terms.empty()) {
        ObjectList *pinList = nullptr;
        ObjectList *l = nullptr;

        std::vector<tb_namespace::Object *> objects;
        __get_objects_from_top_stack(&objects);
        if (!objects.empty()) {
            tb_namespace::Object *object = objects[0];
            if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell) {
                for (auto str : terms) {
                    tb_namespace::TTerm *term =
                        static_cast<tb_namespace::TCell *>(object)
                            ->get_or_create_term(str);
                    if (term) {
                        tb_namespace::TTerm::AttrType *attr = term->getAttr();
                        if (attr) {
                            attr->set_bus(is_bus);
                            attr->set_bundle(is_bundle);
                        }
                    }
                    if (pinList == nullptr) {
                        pinList = new ObjectList();
                        pinList->object = term;
                        l = pinList;
                    } else {
                        l = new ObjectList();
                        l->object = term;
                    }
                    l = l->next;
                }
            }
        }
        if (pinList != nullptr) {
            if (!objects_stack_.empty()) {
                for (ObjectList *v = pinList; v; v = v->next) {
                    v->owner = objects_stack_.top();
                }
            }
            objects_stack_.push(pinList);
            push_status_ = 1;
        }
    }
}
void LibBuilder::__build_pg_pin(timinglib_head *h) {
    std::vector<std::string> terms;
    for (auto v = h->list; v; v = v->next) {
        terms.emplace_back(v->u.string_val);
    }
    if (!terms.empty()) {
        ObjectList *pinList = nullptr;
        ObjectList *l = nullptr;

        std::vector<tb_namespace::Object *> objects;
        __get_objects_from_top_stack(&objects);
        if (!objects.empty()) {
            tb_namespace::Object *object = objects[0];
            if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell) {
                for (auto &str : terms) {
                    tb_namespace::TPgTerm *term =
                        static_cast<tb_namespace::TCell *>(object)
                            ->get_or_create_pgTerm(str);
                    if (pinList == nullptr) {
                        pinList = new ObjectList();
                        pinList->object = term;
                        l = pinList;
                    } else {
                        l = new ObjectList();
                        l->object = term;
                    }
                    l = l->next;
                }
            }
        }
        if (pinList != nullptr) {
            if (!objects_stack_.empty()) {
                for (ObjectList *v = pinList; v; v = v->next) {
                    v->owner = objects_stack_.top();
                }
            }
            objects_stack_.push(pinList);
            push_status_ = 1;
        }
    }
}

void LibBuilder::__build_operating_conditions(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__is_string_type(v)) {
            GOBJECTS
            for (auto object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTLib) {
                    tb_namespace::TLib *p =
                        static_cast<tb_namespace::TLib *>(object);
                    tb_namespace::OperatingConditions *oc =
                        p->get_operating_conditions(v->u.string_val);
                    if (oc == nullptr)
                        oc = p->add_operating_conditions(v->u.string_val);

                    ObjectList *objects = new ObjectList();
                    objects->object = oc;
                    if (!objects_stack_.empty())
                        objects->owner = objects_stack_.top();
                    objects_stack_.push(objects);
                    push_status_ = 1;
                }
            }
        }
    }
}
void LibBuilder::__build_wire_load(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__is_string_type(v)) {
            GOBJECTS
            for (auto object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTLib) {
                    tb_namespace::TLib *lib =
                        static_cast<tb_namespace::TLib *>(object);
                    auto p = lib->get_wire_load(v->u.string_val);
                    if (p == nullptr) p = lib->add_wire_load(v->u.string_val);

                    ObjectList *objects = new ObjectList();
                    objects->object = p;
                    if (!objects_stack_.empty())
                        objects->owner = objects_stack_.top();
                    objects_stack_.push(objects);
                    push_status_ = 1;
                }
            }
        }
    }
}
void LibBuilder::__build_wire_load_table(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__is_string_type(v)) {
            GOBJECTS
            for (auto object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTLib) {
                    tb_namespace::TLib *lib =
                        static_cast<tb_namespace::TLib *>(object);
                    auto p = lib->get_wire_load_table(v->u.string_val);
                    if (p == nullptr)
                        p = lib->add_wire_load_table(v->u.string_val);

                    ObjectList *objects = new ObjectList();
                    objects->object = p;
                    if (!objects_stack_.empty())
                        objects->owner = objects_stack_.top();
                    objects_stack_.push(objects);
                    push_status_ = 1;
                }
            }
        }
    }
}
void LibBuilder::__build_wire_load_selection(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__is_string_type(v)) {
            GOBJECTS
            for (auto object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTLib) {
                    tb_namespace::TLib *lib =
                        static_cast<tb_namespace::TLib *>(object);
                    auto p = lib->get_wire_load_selection(v->u.string_val);
                    if (p == nullptr)
                        p = lib->add_wire_load_selection(v->u.string_val);

                    ObjectList *objects = new ObjectList();
                    objects->object = p;
                    if (!objects_stack_.empty())
                        objects->owner = objects_stack_.top();
                    objects_stack_.push(objects);
                    push_status_ = 1;
                }
            }
        }
    }
}
void LibBuilder::__build_lu_table_template(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__is_string_type(v)) {
            GOBJECTS
            for (auto object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTLib) {
                    tb_namespace::TLib *lib =
                        static_cast<tb_namespace::TLib *>(object);
                    auto p = lib->get_table_template(v->u.string_val);
                    if (p == nullptr)
                        p = lib->add_table_template(v->u.string_val);

                    ObjectList *objects = new ObjectList();
                    objects->object = p;
                    if (!objects_stack_.empty())
                        objects->owner = objects_stack_.top();
                    objects_stack_.push(objects);
                    push_status_ = 1;
                }
            }
        }
    }
}
void LibBuilder::__build_ff(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__is_string_type(v)) {
            GOBJECTS
            for (auto object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTCell) {
                    tb_namespace::TCell *cell =
                        static_cast<tb_namespace::TCell *>(object);
                    if (cell->getAttr()) cell->getAttr()->set_sequential(true);

                    ObjectList *objects = new ObjectList();
                    objects->object = nullptr;
                    if (!objects_stack_.empty())
                        objects->owner = objects_stack_.top();
                    objects_stack_.push(objects);
                    push_status_ = 1;
                }
            }
        }
    }
}
void LibBuilder::__build_latch(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__is_string_type(v)) {
            GOBJECTS
            for (auto object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTCell) {
                    tb_namespace::TCell *cell =
                        static_cast<tb_namespace::TCell *>(object);
                    if (cell->getAttr()) cell->getAttr()->set_sequential(true);

                    ObjectList *objects = new ObjectList();
                    objects->object = nullptr;
                    if (!objects_stack_.empty())
                        objects->owner = objects_stack_.top();
                    objects_stack_.push(objects);
                    push_status_ = 1;
                }
            }
        }
    }
}
void LibBuilder::__build_statetable(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__is_string_type(v)) {
            GOBJECTS
            for (auto object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTCell) {
                    tb_namespace::TCell *cell =
                        static_cast<tb_namespace::TCell *>(object);
                    if (cell->getAttr()) cell->getAttr()->set_sequential(true);

                    ObjectList *objects = new ObjectList();
                    objects->object = nullptr;
                    if (!objects_stack_.empty())
                        objects->owner = objects_stack_.top();
                    objects_stack_.push(objects);
                    push_status_ = 1;
                }
            }
        }
    }
}
void LibBuilder::__build_bus(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__is_string_type(v)) {
            cur_bus_or_bundle_name_ = v->u.string_val;
        }
    }
}
void LibBuilder::__build_bundle(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__is_string_type(v)) {
            cur_bus_or_bundle_name_ = v->u.string_val;
        }
    }
}
void LibBuilder::__build_type(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__is_string_type(v)) {
            cur_type_group_name_ = v->u.string_val;
        }
    }
}
void LibBuilder::__build_timing(timinglib_head *h) {
    std::vector<tb_namespace::TTerm *> libpins;
    bool flag = false;
    if (cur_bus_or_bundle_name_ != "") {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            libpins = vec;
            flag = true;
        }
    }
    tb_namespace::Timing *timing_lib = tb_namespace::getTimingLib();
    if (!flag) {
        if (timing_lib != nullptr) __get_terms_from_top_stack(&libpins);
    }

    ObjectList *pinList = nullptr;
    ObjectList *l = nullptr;
    for (auto &p : libpins) {
        auto t = tb_namespace::Object::createObject<tb_namespace::TimingArc>(
            tb_namespace::ObjectType::kObjectTypeTimingArc,
            timing_lib->getId());
        if (t) {
            t->setOwner(p);
            p->add_timingarc(t->getId());

            if (pinList == nullptr) {
                pinList = new ObjectList();
                pinList->object = t;
                l = pinList;
            } else {
                l = new ObjectList();
                l->object = t;
            }
            l = l->next;
        }
    }
    if (pinList != nullptr) {
        if (!objects_stack_.empty()) {
            for (ObjectList *v = pinList; v; v = v->next) {
                v->owner = objects_stack_.top();
            }
        }
        objects_stack_.push(pinList);
        push_status_ = 1;
    }
}
void LibBuilder::__build_cell_rise(timinglib_head *h) {
    tb_namespace::Timing *timing_lib = tb_namespace::getTimingLib();
    auto v = h->list;
    if (v && __is_string_type(v)) {
        std::vector<tb_namespace::TimingArc *> timingarcs;
        __get_timingarcs_from_top_stack(&timingarcs);
        if (timingarcs.empty()) return;
        tb_namespace::TCell *cell = __get_tcell_by_timingarc(timingarcs[0]);
        if (cell == nullptr) return;
        tb_namespace::ObjectId owner = cell->getOwnerId();
        tb_namespace::TLib *lib =
            tb_namespace::Object::addr<tb_namespace::TLib>(owner);
        if (lib == nullptr) return;
        tb_namespace::TableTemplate *tt =
            lib->get_table_template(v->u.string_val);
        if (tt == nullptr) return;
        auto axis1 = tt->get_axis1();
        if (axis1) {
            axis_[0] = tb_namespace::Object::createObject<tb_namespace::TableAxis>(
                tb_namespace::ObjectType::kObjectTypeTableAxis,
                timing_lib->getId());
            if (axis_[0]) *(axis_[0]) = *axis1;
        }
        auto axis2 = tt->get_axis2();
        if (axis2) {
            axis_[1] = tb_namespace::Object::createObject<tb_namespace::TableAxis>(
                tb_namespace::ObjectType::kObjectTypeTableAxis,
                timing_lib->getId());
            if (axis_[1]) *(axis_[1]) = *axis2;
        }
        auto axis3 = tt->get_axis3();
        if (axis3) {
            axis_[2] = tb_namespace::Object::createObject<tb_namespace::TableAxis>(
                tb_namespace::ObjectType::kObjectTypeTableAxis,
                timing_lib->getId());
            if (axis_[2]) *(axis_[2]) = *axis3;
        }
    }
}
void LibBuilder::__build_cell_fall(timinglib_head *h) { __build_cell_rise(h); }
void LibBuilder::__build_rise_transition(timinglib_head *h) {
    __build_cell_rise(h);
}
void LibBuilder::__build_fall_transition(timinglib_head *h) {
    __build_cell_rise(h);
}
void LibBuilder::__build_rise_constraint(timinglib_head *h) {
    __build_cell_rise(h);
}
void LibBuilder::__build_fall_constraint(timinglib_head *h) {
    __build_cell_rise(h);
}

void LibBuilder::__build_default_oc(BUILD_PARAM) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __is_string_type(v)) {
            tb_namespace::TLib *p = static_cast<tb_namespace::TLib *>(object);
            tb_namespace::OperatingConditions *oc =
                p->get_operating_conditions(v->u.string_val);
            if (oc == nullptr) {
                oc = p->add_operating_conditions(v->u.string_val);
            }
            if (oc != nullptr) p->set_default_operating_conditions(oc->getId());
        }
    }
}
void LibBuilder::__build_time_unit(BUILD_PARAM) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __is_string_type(v)) {
            tb_namespace::TLib *p = static_cast<tb_namespace::TLib *>(object);
            tb_namespace::TUnits *units = p->get_or_create_units();
            if (units != nullptr) {
                int d = 0;
                char suffix[10] = {'\0'};
                sscanf(v->u.string_val, "%d%s", &d, suffix);
                tb_namespace::TUnit &unit = units->get_time_unit();
                unit.digits = d;
                unit.suffix = suffix;
                unit.scale = __get_time_unit_multiply(unit.suffix);
            }
        }
    }
}

void LibBuilder::__build_pulling_resistance_unit(BUILD_PARAM) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __is_string_type(v)) {
            tb_namespace::TLib *p = static_cast<tb_namespace::TLib *>(object);
            tb_namespace::TUnits *units = p->get_or_create_units();
            if (units != nullptr) {
                int d = 0;
                char suffix[10] = {'\0'};
                sscanf(v->u.string_val, "%d%s", &d, suffix);
                tb_namespace::TUnit &unit =
                    units->get_pulling_resistance_unit();
                unit.digits = d;
                unit.suffix = suffix;
                unit.scale = __get_resistance_unit_multiply(unit.suffix);
            }
        }
    }
}
void LibBuilder::__build_resistance_unit(BUILD_PARAM) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __is_string_type(v)) {
            tb_namespace::TLib *p = static_cast<tb_namespace::TLib *>(object);
            tb_namespace::TUnits *units = p->get_or_create_units();
            if (units != nullptr) {
                int d = 0;
                char suffix[10] = {'\0'};
                sscanf(v->u.string_val, "%d%s", &d, suffix);
                tb_namespace::TUnit &unit = units->get_resistance_unit();
                unit.digits = d;
                unit.suffix = suffix;
                unit.scale = __get_resistance_unit_multiply(unit.suffix);
                set_resistance_unit_ = true;
            }
        }
    }
}
void LibBuilder::__build_capacitive_load_unit(BUILD_PARAM) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __is_int_type(v)) {
            tb_namespace::TLib *p = static_cast<tb_namespace::TLib *>(object);
            tb_namespace::TUnits *units = p->get_or_create_units();
            if (units != nullptr) {
                int d = v->u.int_val;
                tb_namespace::TUnit &unit = units->get_capacitance_unit();
                unit.digits = d;
                if (v->next != nullptr && __is_string_type(v->next)) {
                    unit.suffix = v->next->u.string_val;
                    unit.scale = __get_capacitive_unit_multiply(unit.suffix);
                }
            }
        }
    }
}
void LibBuilder::__build_voltage_unit(BUILD_PARAM) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __is_string_type(v)) {
            tb_namespace::TLib *p = static_cast<tb_namespace::TLib *>(object);
            tb_namespace::TUnits *units = p->get_or_create_units();
            if (units != nullptr) {
                int d = 0;
                char suffix[10] = {'\0'};
                sscanf(v->u.string_val, "%d%s", &d, suffix);
                tb_namespace::TUnit &unit = units->get_volatge_unit();
                unit.digits = d;
                unit.suffix = suffix;
                unit.scale = __get_voltage_unit_multiply(unit.suffix);
            }
        }
    }
}
void LibBuilder::__build_current_unit(BUILD_PARAM) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __is_string_type(v)) {
            tb_namespace::TLib *p = static_cast<tb_namespace::TLib *>(object);
            tb_namespace::TUnits *units = p->get_or_create_units();
            if (units != nullptr) {
                int d = 0;
                char suffix[10] = {'\0'};
                sscanf(v->u.string_val, "%d%s", &d, suffix);
                tb_namespace::TUnit &unit = units->get_current_unit();
                unit.digits = d;
                unit.suffix = suffix;
                unit.scale = __get_current_unit_multiply(unit.suffix);
            }
        }
    }
}
void LibBuilder::__build_leakage_power_unit(BUILD_PARAM) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __is_string_type(v)) {
            tb_namespace::TLib *p = static_cast<tb_namespace::TLib *>(object);
            tb_namespace::TUnits *units = p->get_or_create_units();
            if (units != nullptr) {
                int d = 0;
                char suffix[10] = {'\0'};
                sscanf(v->u.string_val, "%d%s", &d, suffix);
                tb_namespace::TUnit &unit = units->get_power_unit();
                unit.digits = d;
                unit.suffix = suffix;
                unit.scale = __get_power_unit_multiply(unit.suffix);
            }
        }
    }
}
void LibBuilder::__build_distance_unit(BUILD_PARAM) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __is_string_type(v)) {
            tb_namespace::TLib *p = static_cast<tb_namespace::TLib *>(object);
            tb_namespace::TUnits *units = p->get_or_create_units();
            if (units != nullptr) {
                int d = 0;
                char suffix[10] = {'\0'};
                sscanf(v->u.string_val, "%d%s", &d, suffix);
                tb_namespace::TUnit &unit = units->get_distance_unit();
                unit.digits = d;
                unit.suffix = suffix;
                unit.scale = __get_distance_unit_multiply(unit.suffix);
            }
        }
    }
}
void LibBuilder::__build_nom_temperature(BUILD_PARAM) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            if (static_cast<tb_namespace::TLib *>(object)->getAttr() != nullptr)
                static_cast<tb_namespace::TLib *>(object)
                    ->getAttr()
                    ->set_nominal_temperature(__get_double_value(v));
        }
    }
}

void LibBuilder::__build_nom_voltage(BUILD_PARAM) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            if (static_cast<tb_namespace::TLib *>(object)->getAttr() != nullptr)
                static_cast<tb_namespace::TLib *>(object)
                    ->getAttr()
                    ->set_nominal_voltage(__get_double_value(v));
        }
    }
}

void LibBuilder::__build_nom_process(BUILD_PARAM) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            if (static_cast<tb_namespace::TLib *>(object)->getAttr() != nullptr)
                static_cast<tb_namespace::TLib *>(object)
                    ->getAttr()
                    ->set_nominal_process(__get_double_value(v));
        }
    }
}

void LibBuilder::__build_input_threshold_pct_fall(BUILD_PARAM) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            if (static_cast<tb_namespace::TLib *>(object)->getAttr() != nullptr)
                static_cast<tb_namespace::TLib *>(object)
                    ->getAttr()
                    ->set_input_threshold_pct_fall(__get_double_value(v));
        }
    }
}

void LibBuilder::__build_input_threshold_pct_rise(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            if (static_cast<tb_namespace::TLib *>(object)->getAttr() != nullptr)
                static_cast<tb_namespace::TLib *>(object)
                    ->getAttr()
                    ->set_input_threshold_pct_rise(__get_double_value(v));
        }
    }
}

void LibBuilder::__build_output_threshold_pct_fall(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            if (static_cast<tb_namespace::TLib *>(object)->getAttr() != nullptr)
                static_cast<tb_namespace::TLib *>(object)
                    ->getAttr()
                    ->set_output_threshold_pct_fall(__get_double_value(v));
        }
    }
}

void LibBuilder::__build_output_threshold_pct_rise(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            if (static_cast<tb_namespace::TLib *>(object)->getAttr() != nullptr)
                static_cast<tb_namespace::TLib *>(object)
                    ->getAttr()
                    ->set_output_threshold_pct_rise(__get_double_value(v));
        }
    }
}

void LibBuilder::__build_slew_lower_threshold_pct_fall(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            if (static_cast<tb_namespace::TLib *>(object)->getAttr() != nullptr)
                static_cast<tb_namespace::TLib *>(object)
                    ->getAttr()
                    ->set_slew_lower_threshold_pct_fall(__get_double_value(v));
        }
    }
}

void LibBuilder::__build_slew_lower_threshold_pct_rise(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            if (static_cast<tb_namespace::TLib *>(object)->getAttr() != nullptr)
                static_cast<tb_namespace::TLib *>(object)
                    ->getAttr()
                    ->set_slew_lower_threshold_pct_rise(__get_double_value(v));
        }
    }
}

void LibBuilder::__build_slew_upper_threshold_pct_fall(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            if (static_cast<tb_namespace::TLib *>(object)->getAttr() != nullptr)
                static_cast<tb_namespace::TLib *>(object)
                    ->getAttr()
                    ->set_slew_upper_threshold_pct_fall(__get_double_value(v));
        }
    }
}

void LibBuilder::__build_slew_upper_threshold_pct_rise(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            if (static_cast<tb_namespace::TLib *>(object)->getAttr() != nullptr)
                static_cast<tb_namespace::TLib *>(object)
                    ->getAttr()
                    ->set_slew_upper_threshold_pct_rise(__get_double_value(v));
        }
    }
}

void LibBuilder::__build_slew_derate_from_library(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            if (static_cast<tb_namespace::TLib *>(object)->getAttr() != nullptr)
                static_cast<tb_namespace::TLib *>(object)
                    ->getAttr()
                    ->set_slew_derate_from_library(__get_double_value(v));
        }
    }
}

void LibBuilder::__build_scaling_factors_attr(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            auto findIt = scaling_factors_str_map_.find(name);
            if (findIt != scaling_factors_str_map_.end()) {
                tb_namespace::TLib *lib =
                    static_cast<tb_namespace::TLib *>(object);
                tb_namespace::ScaleFactors *sf =
                    lib->get_or_create_scaling_factors("");
                if (sf) {
                    auto &q = findIt->second;
                    tb_namespace::ScaleFactorType type = std::get<0>(q);
                    tb_namespace::ScaleFactorPvt pvt = std::get<1>(q);
                    tb_namespace::ScaleFactorRiseFall rf = std::get<2>(q);
                    if (rf == tb_namespace::ScaleFactorRiseFall::kUnknown)
                        sf->add_scale(type, pvt, __get_double_value(v));
                    else
                        sf->add_scale(type, pvt, rf, __get_double_value(v));
                }
            }
        }
    }
}
void LibBuilder::__build_default_wire_load(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __is_string_type(v)) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            auto p = lib->get_wire_load(v->u.string_val);
            if (p == nullptr) p = lib->add_wire_load(v->u.string_val);
            if (p) lib->set_default_wire_load(p->getId());
        }
    }
}
void LibBuilder::__build_default_wire_load_area(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            auto p = lib->getAttr();
            if (p) p->set_default_wire_load_area(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_default_wire_load_mode(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __is_string_type(v)) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            auto p = lib->getAttr();
            if (p)
                p->set_default_wire_load_mode(
                    tb_namespace::toEnumByString<tb_namespace::WireLoadMode>(
                        v->u.string_val));
        }
    }
}
void LibBuilder::__build_default_wire_load_capacitance(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            auto p = lib->getAttr();
            if (p) p->set_default_wire_load_capacitance(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_default_wire_load_resistance(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            auto p = lib->getAttr();
            if (p) p->set_default_wire_load_resistance(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_default_wire_load_selection(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __is_string_type(v)) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            auto p = lib->get_default_wire_load_selection();
            if (p == nullptr) p = lib->add_wire_load_selection(v->u.string_val);
            if (p) lib->set_default_wire_load_selection(p->getId());
        }
    }
}
void LibBuilder::__build_default_input_pin_cap(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            auto p = lib->getAttr();
            if (p) p->set_default_input_pin_cap(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_default_output_pin_cap(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            auto p = lib->getAttr();
            if (p) p->set_default_output_pin_cap(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_default_inout_pin_cap(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            auto p = lib->getAttr();
            if (p) p->set_default_inout_pin_cap(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_default_max_capacitance(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            auto p = lib->getAttr();
            if (p) p->set_default_max_capacitance(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_default_max_fanout(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            auto p = lib->getAttr();
            if (p) p->set_default_max_fanout(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_default_max_transition(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            auto p = lib->getAttr();
            if (p) p->set_default_max_transition(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_default_fanout_load(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            auto p = lib->getAttr();
            if (p) p->set_default_fanout_load(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_default_cell_leakage_power(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__is_int_type(v) || __is_double_type(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            auto p = lib->getAttr();
            if (p) p->set_default_cell_leakage_power(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_voltage_map(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __is_string_type(v)) {
            if (v->next &&
                (__is_int_type(v->next) || __is_double_type(v->next))) {
                tb_namespace::TLib *lib =
                    static_cast<tb_namespace::TLib *>(object);
                tb_namespace::TLib::AttrType *attr = lib->getAttr();
                if (attr)
                    attr->add_supply_voltage(v->u.string_val,
                                             __get_double_value(v->next));
            }
        }
    }
}

void LibBuilder::__build_process(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeOperatingConditions &&
            (__is_int_type(v) || __is_double_type(v))) {
            static_cast<tb_namespace::OperatingConditions *>(object)
                ->set_process(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_temperature(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeOperatingConditions &&
            (__is_int_type(v) || __is_double_type(v))) {
            static_cast<tb_namespace::OperatingConditions *>(object)
                ->set_temperature(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_voltage(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeOperatingConditions &&
            (__is_int_type(v) || __is_double_type(v))) {
            static_cast<tb_namespace::OperatingConditions *>(object)
                ->set_voltage(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_tree_type(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeOperatingConditions &&
            __is_string_type(v)) {
            if (!strcmp(v->u.string_val, "best_case_tree"))
                static_cast<tb_namespace::OperatingConditions *>(object)
                    ->set_wire_load_tree(tb_namespace::WireLoadTree::kBestCase);
            else if (!strcmp(v->u.string_val, "balanced_tree"))
                static_cast<tb_namespace::OperatingConditions *>(object)
                    ->set_wire_load_tree(tb_namespace::WireLoadTree::kBalanced);
            else if (!strcmp(v->u.string_val, "worst_case_tree"))
                static_cast<tb_namespace::OperatingConditions *>(object)
                    ->set_wire_load_tree(
                        tb_namespace::WireLoadTree::kWorstCase);
        }
    }
}

void LibBuilder::__build_area(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if ((__is_int_type(v) || __is_double_type(v))) {
            if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell) {
                if (static_cast<tb_namespace::TCell *>(object)->getAttr() !=
                    nullptr)
                    static_cast<tb_namespace::TCell *>(object)
                        ->getAttr()
                        ->set_area(__get_double_value(v));
            } else if (object->getObjectType() ==
                       tb_namespace::ObjectType::kObjectTypeWireLoad) {
                static_cast<tb_namespace::WireLoad *>(object)->set_area(
                    __get_double_value(v));
            }
        }
    }
}
void LibBuilder::__build_cell_footprint(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __is_string_type(v)) {
            if (static_cast<tb_namespace::TCell *>(object)->getAttr() !=
                nullptr)
                static_cast<tb_namespace::TCell *>(object)
                    ->getAttr()
                    ->set_cell_footprint(v->u.string_val);
        }
    }
}
void LibBuilder::__build_dont_touch(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __is_bool_type(v)) {
            if (static_cast<tb_namespace::TCell *>(object)->getAttr() !=
                nullptr)
                static_cast<tb_namespace::TCell *>(object)
                    ->getAttr()
                    ->set_dont_touch(v->u.bool_val);
        }
    }
}
void LibBuilder::__build_dont_use(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __is_bool_type(v)) {
            if (static_cast<tb_namespace::TCell *>(object)->getAttr() !=
                nullptr)
                static_cast<tb_namespace::TCell *>(object)
                    ->getAttr()
                    ->set_dont_use(v->u.bool_val);
        }
    }
}
void LibBuilder::__build_always_on(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (__is_bool_type(v)) {
            if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell) {
                auto attr =
                    static_cast<tb_namespace::TCell *>(object)->getAttr();
                if (attr != nullptr) attr->set_always_on(v->u.bool_val);
            } else if (object->getObjectType() ==
                       tb_namespace::ObjectType::kObjectTypeTTerm) {
                auto attr =
                    static_cast<tb_namespace::TTerm *>(object)->getAttr();
                if (attr != nullptr) attr->set_always_on(true);
            }
        }
    }
}
void LibBuilder::__build_is_macro_cell(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __is_bool_type(v)) {
            if (static_cast<tb_namespace::TCell *>(object)->getAttr() !=
                nullptr)
                static_cast<tb_namespace::TCell *>(object)
                    ->getAttr()
                    ->set_macro_cell(v->u.bool_val);
        }
    }
}
void LibBuilder::__build_is_pad(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __is_bool_type(v)) {
            if (v->u.bool_val) {
                if (static_cast<tb_namespace::TCell *>(object)->getAttr() !=
                    nullptr)
                    static_cast<tb_namespace::TCell *>(object)
                        ->getAttr()
                        ->set_pad(true);
            }
        }
    }
}
void LibBuilder::__build_is_decap_cell(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __is_bool_type(v)) {
            if (v->u.bool_val) {
                if (static_cast<tb_namespace::TCell *>(object)->getAttr() !=
                    nullptr)
                    static_cast<tb_namespace::TCell *>(object)
                        ->getAttr()
                        ->set_decap_cell(true);
            }
        }
    }
}
void LibBuilder::__build_is_filler_cell(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __is_bool_type(v)) {
            if (v->u.bool_val) {
                if (static_cast<tb_namespace::TCell *>(object)->getAttr() !=
                    nullptr)
                    static_cast<tb_namespace::TCell *>(object)
                        ->getAttr()
                        ->set_filler_cell(true);
            }
        }
    }
}
void LibBuilder::__build_is_tap_cell(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __is_bool_type(v)) {
            if (v->u.bool_val) {
                if (static_cast<tb_namespace::TCell *>(object)->getAttr() !=
                    nullptr)
                    static_cast<tb_namespace::TCell *>(object)
                        ->getAttr()
                        ->set_tap_cell(true);
            }
        }
    }
}
void LibBuilder::__build_is_clock_gating_cell(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __is_bool_type(v)) {
            if (v->u.bool_val) {
                if (static_cast<tb_namespace::TCell *>(object)->getAttr() !=
                    nullptr)
                    static_cast<tb_namespace::TCell *>(object)
                        ->getAttr()
                        ->set_clock_gating_cell(true);
            }
        }
    }
}
void LibBuilder::__build_is_clock_isolation_cell(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __is_bool_type(v)) {
            if (v->u.bool_val) {
                if (static_cast<tb_namespace::TCell *>(object)->getAttr() !=
                    nullptr)
                    static_cast<tb_namespace::TCell *>(object)
                        ->getAttr()
                        ->set_clock_isolation_cell(true);
            }
        }
    }
}
void LibBuilder::__build_is_isolation_cell(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __is_bool_type(v)) {
            if (v->u.bool_val) {
                if (static_cast<tb_namespace::TCell *>(object)->getAttr() !=
                    nullptr)
                    static_cast<tb_namespace::TCell *>(object)
                        ->getAttr()
                        ->set_isolation_cell(true);
            }
        }
    }
}
void LibBuilder::__build_is_no_enable(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __is_bool_type(v)) {
            if (v->u.bool_val) {
                if (static_cast<tb_namespace::TCell *>(object)->getAttr() !=
                    nullptr)
                    static_cast<tb_namespace::TCell *>(object)
                        ->getAttr()
                        ->set_enable_level_shifter(true);
            }
        }
    }
}
void LibBuilder::__build_clock_gating_integrated_cell(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __is_string_type(v)) {
            tb_namespace::TCell::AttrType *attr =
                static_cast<tb_namespace::TCell *>(object)->getAttr();
            if (attr != nullptr) {
                attr->set_integrated_clock_gating_cell(true);
                attr->set_clock_gating_integrated_cell(
                    tb_namespace::toEnumByString<
                        tb_namespace::ClockGateIntegratedType>(
                        v->u.string_val));
            }
        }
    }
}
void LibBuilder::__build_antenna_diode_type(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __is_string_type(v)) {
            tb_namespace::TCell::AttrType *attr =
                static_cast<tb_namespace::TCell *>(object)->getAttr();
            if (attr == nullptr) continue;

            if (!strcmp(v->u.string_val, "power"))
                attr->set_antenna_diode_type(
                    tb_namespace::AntennaDiodeType::kPower);
            else if (!strcmp(v->u.string_val, "ground"))
                attr->set_antenna_diode_type(
                    tb_namespace::AntennaDiodeType::kGround);
            else if (!strcmp(v->u.string_val, "power_and_ground"))
                attr->set_antenna_diode_type(
                    tb_namespace::AntennaDiodeType::kPowerAndGround);

            attr->set_diode_cell(true);
        }
    }
}
void LibBuilder::__build_is_level_shifter(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __is_bool_type(v)) {
            if (v->u.bool_val) {
                if (static_cast<tb_namespace::TCell *>(object)->getAttr() !=
                    nullptr)
                    static_cast<tb_namespace::TCell *>(object)
                        ->getAttr()
                        ->set_level_shifter(true);
            }
        }
    }
}
void LibBuilder::__build_cell_leakage_power(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            (__is_int_type(v) || __is_double_type(v))) {
            if (static_cast<tb_namespace::TCell *>(object)->getAttr() !=
                nullptr)
                static_cast<tb_namespace::TCell *>(object)
                    ->getAttr()
                    ->set_cell_leakage_power(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_switch_cell_type(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __is_string_type(v)) {
            tb_namespace::TCell::AttrType *attr =
                static_cast<tb_namespace::TCell *>(object)->getAttr();
            if (attr != nullptr) {
                attr->set_switch_cell_type(
                    tb_namespace::toEnumByString<tb_namespace::SwitchCellType>(
                        v->u.string_val));
                attr->set_power_switch(true);
            }
        }
    }
}
void LibBuilder::__build_retention_cell(BUILD_PARAM) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __is_string_type(v)) {
            tb_namespace::TCell::AttrType *attr =
                static_cast<tb_namespace::TCell *>(object)->getAttr();
            if (attr != nullptr) {
                attr->set_retention_cell(v->u.string_val);
                attr->set_retention_cell(true);
            }
        }
    }
}
void LibBuilder::__build_related_ground_pin(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" && __is_string_type(v)) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                tb_namespace::ObjectId owner = term->getOwnerId();
                tb_namespace::TCell *cell = nullptr;
                if (owner != UNINIT_OBJECT_ID)
                    cell = tb_namespace::Object::addr<
                        tb_namespace::TCell>(owner);
                if (cell) {
                    auto p = cell->get_or_create_pgTerm(v->u.string_val);
                    if (p) term->set_related_ground_pin(p->getId());
                }
            }
            return;
        }
    }
    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if (__is_string_type(v)) {
        for (auto term : libpins) {
            if (term) {
                tb_namespace::ObjectId ownerId = term->getOwnerId();
                if (ownerId != UNINIT_OBJECT_ID) {
                    tb_namespace::TCell *cell =
                        tb_namespace::Object::addr<
                            tb_namespace::TCell>(ownerId);
                    if (cell) {
                        auto p = cell->get_or_create_pgTerm(v->u.string_val);
                        if (p) term->set_related_ground_pin(p->getId());
                    }
                }
            }
        }
    }
}
void LibBuilder::__build_related_power_pin(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" && __is_string_type(v)) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                tb_namespace::ObjectId ownerId = term->getOwnerId();
                tb_namespace::TCell *cell = nullptr;
                if (ownerId != UNINIT_OBJECT_ID)
                    cell = tb_namespace::Object::addr<
                        tb_namespace::TCell>(ownerId);
                if (cell) {
                    auto p = cell->get_or_create_pgTerm(v->u.string_val);
                    if (p) term->set_related_power_pin(p->getId());
                }
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if (__is_string_type(v)) {
        for (auto term : libpins) {
            if (term) {
                tb_namespace::ObjectId ownerId = term->getOwnerId();
                if (ownerId != UNINIT_OBJECT_ID) {
                    tb_namespace::TCell *cell =
                        tb_namespace::Object::addr<
                            tb_namespace::TCell>(ownerId);
                    if (cell) {
                        auto p = cell->get_or_create_pgTerm(v->u.string_val);
                        if (p) term->set_related_power_pin(p->getId());
                    }
                }
            }
        }
    }
}
void LibBuilder::__build_max_transition(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__is_int_type(v) || __is_double_type(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term->getAttr())
                    term->getAttr()->set_max_transition(__get_double_value(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if ((__is_int_type(v) || __is_double_type(v))) {
        for (auto term : libpins) {
            if (term->getAttr())
                term->getAttr()->set_max_transition(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_fall_capacitance(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__is_int_type(v) || __is_double_type(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term->getAttr())
                    term->getAttr()->set_fall_capacitance(
                        __get_double_value(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if ((__is_int_type(v) || __is_double_type(v))) {
        for (auto term : libpins) {
            if (term->getAttr())
                term->getAttr()->set_fall_capacitance(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_rise_capacitance(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__is_int_type(v) || __is_double_type(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term->getAttr())
                    term->getAttr()->set_rise_capacitance(
                        __get_double_value(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if ((__is_int_type(v) || __is_double_type(v))) {
        for (auto term : libpins) {
            if (term->getAttr())
                term->getAttr()->set_rise_capacitance(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_three_state(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" && __is_string_type(v)) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term->getAttr()) term->getAttr()->set_three_state(true);
                tb_namespace::ObjectId owner = term->getOwnerId();
                if (owner != UNINIT_OBJECT_ID) {
                    auto cell = tb_namespace::Object::addr<
                        tb_namespace::TCell>(owner);
                    if (cell && cell->getAttr())
                        cell->getAttr()->set_three_state(true);
                }
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if (__is_string_type(v)) {
        for (auto term : libpins) {
            if (term->getAttr()) term->getAttr()->set_three_state(true);
            tb_namespace::ObjectId owner = term->getOwnerId();
            if (owner != UNINIT_OBJECT_ID) {
                auto cell =
                    tb_namespace::Object::addr<tb_namespace::TCell>(
                        owner);
                if (cell && cell->getAttr())
                    cell->getAttr()->set_three_state(true);
            }
        }
    }
}
void LibBuilder::__build_capacitance(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__is_int_type(v) || __is_double_type(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term->getAttr())
                    term->getAttr()->set_capacitance(__get_double_value(v));
            }
            return;
        }
    }
    GOBJECTS
    if ((__is_int_type(v) || __is_double_type(v))) {
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTTerm) {
                auto term = static_cast<tb_namespace::TTerm *>(obj);
                if (term->getAttr())
                    term->getAttr()->set_capacitance(__get_double_value(v));
            } else if (obj->getObjectType() ==
                       tb_namespace::ObjectType::kObjectTypeWireLoad) {
                auto wire_load = static_cast<tb_namespace::WireLoad *>(obj);
                wire_load->set_capacitance(__get_double_value(v));
            }
        }
    }
}
void LibBuilder::__build_max_capacitance(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__is_int_type(v) || __is_double_type(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term->getAttr())
                    term->getAttr()->set_max_capacitance(__get_double_value(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if ((__is_int_type(v) || __is_double_type(v))) {
        for (auto term : libpins) {
            if (term->getAttr())
                term->getAttr()->set_max_capacitance(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_min_capacitance(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__is_int_type(v) || __is_double_type(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term->getAttr())
                    term->getAttr()->set_min_capacitance(__get_double_value(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if ((__is_int_type(v) || __is_double_type(v))) {
        for (auto term : libpins) {
            if (term->getAttr())
                term->getAttr()->set_min_capacitance(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_direction(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" && __is_string_type(v)) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                auto attr = term->getAttr();
                if (attr)
                    attr->set_direction(
                        tb_namespace::toEnumByString<
                            tb_namespace::PinDirection>(v->u.string_val));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if (__is_string_type(v)) {
        for (auto term : libpins) {
            auto attr = term->getAttr();
            if (attr)
                attr->set_direction(
                    tb_namespace::toEnumByString<tb_namespace::PinDirection>(
                        v->u.string_val));
        }
    }
}
void LibBuilder::__build_max_fanout(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__is_int_type(v) || __is_double_type(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term->getAttr())
                    term->getAttr()->set_max_fanout(__get_double_value(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if ((__is_int_type(v) || __is_double_type(v))) {
        for (auto term : libpins) {
            if (term->getAttr())
                term->getAttr()->set_max_fanout(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_min_fanout(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__is_int_type(v) || __is_double_type(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term->getAttr())
                    term->getAttr()->set_min_fanout(__get_double_value(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if ((__is_int_type(v) || __is_double_type(v))) {
        for (auto term : libpins) {
            if (term->getAttr())
                term->getAttr()->set_min_fanout(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_min_transition(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__is_int_type(v) || __is_double_type(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term->getAttr())
                    term->getAttr()->set_min_transition(__get_double_value(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if ((__is_int_type(v) || __is_double_type(v))) {
        for (auto term : libpins) {
            if (term->getAttr())
                term->getAttr()->set_min_transition(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_min_period(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__is_int_type(v) || __is_double_type(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term->getAttr())
                    term->getAttr()->set_min_period(__get_double_value(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if ((__is_int_type(v) || __is_double_type(v))) {
        for (auto term : libpins) {
            if (term->getAttr())
                term->getAttr()->set_min_period(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_min_pulse_width_high(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__is_int_type(v) || __is_double_type(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term->getAttr())
                    term->getAttr()->set_min_pulse_width_high(
                        __get_double_value(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if ((__is_int_type(v) || __is_double_type(v))) {
        for (auto term : libpins) {
            if (term->getAttr())
                term->getAttr()->set_min_pulse_width_high(
                    __get_double_value(v));
        }
    }
}
void LibBuilder::__build_min_pulse_width_low(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__is_int_type(v) || __is_double_type(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term->getAttr())
                    term->getAttr()->set_min_pulse_width_low(
                        __get_double_value(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if ((__is_int_type(v) || __is_double_type(v))) {
        for (auto term : libpins) {
            if (term->getAttr())
                term->getAttr()->set_min_pulse_width_low(__get_double_value(v));
        }
    }
}
void LibBuilder::__build_clock(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" && __is_bool_type(v)) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term->getAttr()) term->getAttr()->set_clock(v->u.bool_val);
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if (__is_bool_type(v)) {
        for (auto term : libpins) {
            if (term->getAttr()) term->getAttr()->set_clock(v->u.bool_val);
        }
    }
}
void LibBuilder::__build_clock_gate_clock_pin(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" && __is_bool_type(v)) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term->getAttr())
                    term->getAttr()->set_clock_gate_clock_pin(v->u.bool_val);
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if (__is_bool_type(v)) {
        for (auto term : libpins) {
            if (term->getAttr())
                term->getAttr()->set_clock_gate_clock_pin(v->u.bool_val);
        }
    }
}
void LibBuilder::__build_clock_gate_enable_pin(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" && __is_bool_type(v)) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term->getAttr())
                    term->getAttr()->set_clock_gate_enable_pin(v->u.bool_val);
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if (__is_bool_type(v)) {
        for (auto term : libpins) {
            if (term->getAttr())
                term->getAttr()->set_clock_gate_enable_pin(v->u.bool_val);
        }
    }
}
void LibBuilder::__build_function(BUILD_PARAM) {
    if (cur_bus_or_bundle_name_ != "" && __is_string_type(v)) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                term->set_function(v->u.string_val);
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __get_terms_from_top_stack(&libpins);
    if (libpins.empty()) return;

    if (__is_string_type(v)) {
        for (auto term : libpins) {
            term->set_function(v->u.string_val);
        }
    }
}

void LibBuilder::__build_members(BUILD_PARAM) {
    std::vector<std::string> terms;
    for (auto p = v; p; p = p->next) {
        if (__is_string_type(p)) {
            terms.emplace_back(p->u.string_val);
        }
    }
    if (!terms.empty()) {
        std::vector<tb_namespace::TTerm *> member_pins;
        std::vector<tb_namespace::Object *> objects;
        __get_objects_from_top_stack(&objects);
        if (!objects.empty()) {
            tb_namespace::Object *object = objects[0];
            if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell) {
                for (auto str : terms) {
                    tb_namespace::TTerm *term =
                        static_cast<tb_namespace::TCell *>(object)
                            ->get_or_create_term(str);
                    if (term) {
                        tb_namespace::TTerm::AttrType *attr = term->getAttr();
                        if (attr) {
                            attr->set_bundle(true);
                        }
                        member_pins.emplace_back(term);
                    }
                }
            }
        }
        for (auto &p : member_pins) {
            for (auto &q : member_pins) {
                p->add_member_pin(q->getId());
            }
        }
        if (cur_bus_or_bundle_name_ != "")
            bus_or_bundle_member_pins_map_[cur_bus_or_bundle_name_] =
                member_pins;
    }
}
void LibBuilder::__build_bus_type(BUILD_PARAM) {
    if (v == nullptr || !__is_string_type(v)) return;
    std::string bus_name = v->u.string_val;
    auto findIt = type_group_cell_map_.find(bus_name);
    if (findIt == type_group_cell_map_.end()) {
        findIt = type_group_lib_map_.find(bus_name);
        if (findIt == type_group_lib_map_.end()) return;
    }
    TypeGroup tg = findIt->second;
    int from = -1;
    int to = -1;
    if (tg.bit_from != -1 && tg.bit_to != -1) {
        if (tg.bit_from > tg.bit_to) {
            to = tg.bit_from;
            from = tg.bit_from;
        } else {
            from = tg.bit_from;
            to = tg.bit_to;
        }
    } else if (tg.bit_from != -1) {
        if (tg.downto == true) {
            to = tg.bit_from;
            from = to - tg.bit_width + 1;
        } else {
            from = to = tg.bit_from;
        }
    } else if (tg.bit_to != -1) {
        if (tg.downto == false) {
            to = tg.bit_to;
            from = tg.bit_to - tg.bit_width + 1;
        } else {
            from = to = tg.bit_to;
        }
    }
    if (from == -1 && to == -1)
        return;
    else if (from != -1 && to == -1)
        to = from;
    else if (from == -1 && to != -1)
        from = to;

    std::vector<tb_namespace::Object *> objects;
    __get_objects_from_top_stack(&objects);
    if (!objects.empty()) {
        tb_namespace::Object *object = objects[0];
        if (object->getObjectType() ==
            tb_namespace::ObjectType::kObjectTypeTCell) {
            tb_namespace::TCell *cell =
                static_cast<tb_namespace::TCell *>(object);
            std::vector<tb_namespace::TTerm *> member_pins;
            for (int i = from; i <= to; ++i) {
                std::string str = bus_name + std::to_string(i);
                auto p = cell->get_or_create_term(str);
                if (p) {
                    if (p->getAttr()) p->getAttr()->set_bus(true);
                    member_pins.emplace_back(p);
                }
            }
            for (auto &p : member_pins) {
                for (auto &q : member_pins) {
                    p->add_member_pin(q->getId());
                }
            }
            if (cur_bus_or_bundle_name_ != "")
                bus_or_bundle_member_pins_map_[cur_bus_or_bundle_name_] =
                    member_pins;
        }
    }
}

void LibBuilder::__build_pg_type(BUILD_PARAM) {
    std::vector<tb_namespace::TPgTerm *> libpgpins;
    __get_pg_terms_from_top_stack(&libpgpins);
    if (libpgpins.empty()) return;

    if (__is_string_type(v)) {
        for (auto pgterm : libpgpins) {
            pgterm->set_pg_type(
                tb_namespace::toEnumByString<tb_namespace::PGType>(
                    v->u.string_val));
        }
    }
}
void LibBuilder::__build_voltage_name(BUILD_PARAM) {
    std::vector<tb_namespace::TPgTerm *> libpgpins;
    __get_pg_terms_from_top_stack(&libpgpins);
    if (libpgpins.empty()) return;

    if (__is_string_type(v)) {
        for (auto pgterm : libpgpins) {
            pgterm->set_voltage_name(v->u.string_val);
        }
    }
}
void LibBuilder::__build_bit_width(BUILD_PARAM) {
    GOBJECTS;
    tb_namespace::Object *obj = objects[0];
    if (cur_type_group_name_ != "" && __is_int_type(v)) {
        if (obj->getObjectType() == tb_namespace::ObjectType::kObjectTypeTCell)
            type_group_cell_map_[cur_type_group_name_].bit_width = v->u.int_val;
        else if (obj->getObjectType() ==
                 tb_namespace::ObjectType::kObjectTypeTLib)
            type_group_lib_map_[cur_type_group_name_].bit_width = v->u.int_val;
    }
}
void LibBuilder::__build_bit_from(BUILD_PARAM) {
    GOBJECTS;
    tb_namespace::Object *obj = objects[0];
    if (cur_type_group_name_ != "" && __is_int_type(v)) {
        if (obj->getObjectType() == tb_namespace::ObjectType::kObjectTypeTCell)
            type_group_cell_map_[cur_type_group_name_].bit_from = v->u.int_val;
        else if (obj->getObjectType() ==
                 tb_namespace::ObjectType::kObjectTypeTLib)
            type_group_lib_map_[cur_type_group_name_].bit_from = v->u.int_val;
    }
}
void LibBuilder::__build_bit_to(BUILD_PARAM) {
    GOBJECTS;
    tb_namespace::Object *obj = objects[0];
    if (cur_type_group_name_ != "" && __is_int_type(v)) {
        if (obj->getObjectType() == tb_namespace::ObjectType::kObjectTypeTCell)
            type_group_cell_map_[cur_type_group_name_].bit_to = v->u.int_val;
        else if (obj->getObjectType() ==
                 tb_namespace::ObjectType::kObjectTypeTLib)
            type_group_lib_map_[cur_type_group_name_].bit_to = v->u.int_val;
    }
}
void LibBuilder::__build_downto(BUILD_PARAM) {
    GOBJECTS;
    tb_namespace::Object *obj = objects[0];
    if (cur_type_group_name_ != "" && __is_bool_type(v)) {
        if (obj->getObjectType() == tb_namespace::ObjectType::kObjectTypeTCell)
            type_group_cell_map_[cur_type_group_name_].downto = v->u.bool_val;
        else if (obj->getObjectType() ==
                 tb_namespace::ObjectType::kObjectTypeTLib)
            type_group_lib_map_[cur_type_group_name_].downto = v->u.bool_val;
    }
}

void LibBuilder::__build_timing_sense(BUILD_PARAM) {
    if (v && __is_string_type(v)) {
        std::vector<tb_namespace::TimingArc *> timingarcs;
        __get_timingarcs_from_top_stack(&timingarcs);
        for (auto &p : timingarcs) {
            p->set_timing_sense(
                tb_namespace::toEnumByString<tb_namespace::TimingSense>(
                    v->u.string_val));
        }
    }
}
void LibBuilder::__build_timing_type(BUILD_PARAM) {
    if (v && __is_string_type(v)) {
        std::vector<tb_namespace::TimingArc *> timingarcs;
        __get_timingarcs_from_top_stack(&timingarcs);
        for (auto &p : timingarcs) {
            p->set_timing_type(
                tb_namespace::toEnumByString<tb_namespace::TimingType>(
                    v->u.string_val));
        }
    }
}
void LibBuilder::__build_when(BUILD_PARAM) {
    if (v && __is_string_type(v)) {
        std::vector<tb_namespace::TimingArc *> timingarcs;
        __get_timingarcs_from_top_stack(&timingarcs);
        for (auto &p : timingarcs) {
            p->set_when(v->u.string_val);
        }
    }
}
void LibBuilder::__build_related_pin(BUILD_PARAM) {
    std::vector<std::string> strs;
    for (auto p = v; p; p = p->next) {
        if (__is_string_type(p)) {
            __split_string_by_delim(p->u.string_val, " ", &strs);
        }
    }
    if (strs.empty()) return;

    std::vector<std::string> pins;
    for (auto &s : strs) {
        __get_pin_names_from_bus_range(s.c_str(), &pins);
    }
    if (pins.empty()) return;

    std::vector<tb_namespace::TimingArc *> timingarcs;
    __get_timingarcs_from_top_stack(&timingarcs);
    if (timingarcs.empty()) return;

    std::vector<tb_namespace::TTerm *> terms_owner;
    for (auto &p : timingarcs) {
        tb_namespace::ObjectId owner = p->getOwnerId();
        if (owner != UNINIT_OBJECT_ID) {
            auto term =
                tb_namespace::Object::addr<tb_namespace::TTerm>(owner);
            if (term) terms_owner.emplace_back(term);
        }
    }
    tb_namespace::TCell *cell = nullptr;
    tb_namespace::TTerm *term = terms_owner[0];
    if (term) {
        tb_namespace::ObjectId owner = term->getOwnerId();
        if (owner != UNINIT_OBJECT_ID)
            cell =
                tb_namespace::Object::addr<tb_namespace::TCell>(owner);
    }
    if (cell == nullptr) return;

    size_t term_size = terms_owner.size();
    std::vector<tb_namespace::TTerm *> related_terms;
    for (auto &s : pins) {
        auto findIt = bus_or_bundle_member_pins_map_.find(s);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            if (term_size == vec.size()) {
                for (int i = 0; i < term_size; i++) {
                    timingarcs[i]->add_related_pin(vec[i]->getId());
                }
            } else {
                related_terms.insert(related_terms.end(), vec.begin(),
                                     vec.end());
            }
        } else {
            auto term = cell->get_or_create_term(s);
            if (term) related_terms.emplace_back(term);
        }
    }
    if (related_terms.empty()) return;

    for (auto &t : timingarcs) {
        for (auto &p : related_terms) {
            t->add_related_pin(p->getId());
        }
    }
}

void LibBuilder::__build_resistance(BUILD_PARAM) {
    if (v && (__is_int_type(v) || __is_double_type(v))) {
        GOBJECTS
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeWireLoad) {
                auto wire_load = static_cast<tb_namespace::WireLoad *>(obj);
                wire_load->set_resistance(__get_double_value(v));
            }
        }
    }
}
void LibBuilder::__build_slope(BUILD_PARAM) {
    if (v && (__is_int_type(v) || __is_double_type(v))) {
        GOBJECTS
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeWireLoad) {
                auto wire_load = static_cast<tb_namespace::WireLoad *>(obj);
                wire_load->set_slope(__get_double_value(v));
            }
        }
    }
}
void LibBuilder::__build_fanout_length(BUILD_PARAM) {
    if (v == nullptr || v->next == nullptr) return;
    GOBJECTS;
    if (__is_int_type(v) &&
        (__is_int_type(v->next) || __is_double_type(v->next))) {
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeWireLoad) {
                auto wire_load = static_cast<tb_namespace::WireLoad *>(obj);
                wire_load->add_fanout_length(v->u.int_val,
                                             __get_double_value(v->next));
            } else if (obj->getObjectType() ==
                       tb_namespace::ObjectType::kObjectTypeWireLoadTable) {
                auto wire_load_table =
                    static_cast<tb_namespace::WireLoadTable *>(obj);
                wire_load_table->set_fanout_length(v->u.int_val,
                                                   __get_double_value(v->next));
            }
        }
    }
}

void LibBuilder::__build_fanout_capacitance(BUILD_PARAM) {
    if (v == nullptr || v->next == nullptr) return;
    GOBJECTS;
    if (__is_int_type(v) &&
        (__is_int_type(v->next) || __is_double_type(v->next))) {
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeWireLoadTable) {
                auto wire_load_table =
                    static_cast<tb_namespace::WireLoadTable *>(obj);
                wire_load_table->set_fanout_capacitance(
                    v->u.int_val, __get_double_value(v->next));
            }
        }
    }
}
void LibBuilder::__build_fanout_resistance(BUILD_PARAM) {
    if (v == nullptr || v->next == nullptr) return;
    GOBJECTS;
    if (__is_int_type(v) &&
        (__is_int_type(v->next) || __is_double_type(v->next))) {
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeWireLoadTable) {
                auto wire_load_table =
                    static_cast<tb_namespace::WireLoadTable *>(obj);
                wire_load_table->set_fanout_resistance(
                    v->u.int_val, __get_double_value(v->next));
            }
        }
    }
}
void LibBuilder::__build_fanout_area(BUILD_PARAM) {
    if (v == nullptr || v->next == nullptr) return;
    GOBJECTS;
    if (__is_int_type(v) &&
        (__is_int_type(v->next) || __is_double_type(v->next))) {
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeWireLoadTable) {
                auto wire_load_table =
                    static_cast<tb_namespace::WireLoadTable *>(obj);
                wire_load_table->set_fanout_area(v->u.int_val,
                                                 __get_double_value(v->next));
            }
        }
    }
}

void LibBuilder::__build_wire_load_from_area(BUILD_PARAM) {
    if (v == nullptr || v->next == nullptr || v->next->next == nullptr) return;
    tb_namespace::Timing *timing_lib = tb_namespace::getTimingLib();
    if (timing_lib == nullptr) return;
    GOBJECTS
    auto p1 = v;
    auto p2 = v->next;
    auto p3 = v->next->next;
    if ((__is_int_type(p1) || __is_double_type(p1)) &&
        (__is_int_type(p2) || __is_double_type(p2)) && __is_string_type(p3)) {
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeWireLoadSelection) {
                auto p = static_cast<tb_namespace::WireLoadSelection *>(obj);
                tb_namespace::ObjectId owner = p->getOwnerId();
                tb_namespace::TLib *lib = nullptr;
                if (owner != UNINIT_OBJECT_ID)
                    lib =
                        tb_namespace::Object::addr<tb_namespace::TLib>(
                            owner);
                if (lib == nullptr) continue;
                auto wire_load = lib->get_wire_load(p3->u.string_val);
                if (wire_load == nullptr) continue;
                auto wlr = tb_namespace::Object::createObject<tb_namespace::WireLoadForArea>(
                    tb_namespace::ObjectType::kObjectTypeWireLoadForArea,
                    timing_lib->getId());
                if (wlr) {
                    wlr->set_min_area(__get_double_value(p1));
                    wlr->set_max_area(__get_double_value(p2));
                    wlr->set_wireload(wire_load->getId());
                    wlr->setOwner(p);
                    p->add_wire_load_for_area(wlr->getId());
                }
            }
        }
    }
}

void LibBuilder::__build_variable_1(BUILD_PARAM) {
    if (v && __is_string_type(v)) {
        GOBJECTS
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTableTemplate) {
                auto tt = static_cast<tb_namespace::TableTemplate *>(obj);
                auto axis1 = tt->create_axis1();
                if (axis1) {
                    axis1->set_variable(
                        tb_namespace::toEnumByString<
                            tb_namespace::TableAxisVariable>(v->u.string_val));
                }
            }
        }
    }
}
void LibBuilder::__build_variable_2(BUILD_PARAM) {
    if (v && __is_string_type(v)) {
        GOBJECTS
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTableTemplate) {
                auto tt = static_cast<tb_namespace::TableTemplate *>(obj);
                auto axis2 = tt->create_axis2();
                if (axis2) {
                    axis2->set_variable(
                        tb_namespace::toEnumByString<
                            tb_namespace::TableAxisVariable>(v->u.string_val));
                }
            }
        }
    }
}
void LibBuilder::__build_variable_3(BUILD_PARAM) {
    if (v && __is_string_type(v)) {
        GOBJECTS
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTableTemplate) {
                auto tt = static_cast<tb_namespace::TableTemplate *>(obj);
                auto axis3 = tt->create_axis3();
                if (axis3) {
                    axis3->set_variable(
                        tb_namespace::toEnumByString<
                            tb_namespace::TableAxisVariable>(v->u.string_val));
                }
            }
        }
    }
}
void LibBuilder::__build_index_1(BUILD_PARAM) {
    GOBJECTS
    for (auto &obj : objects) {
        if (obj->getObjectType() ==
            tb_namespace::ObjectType::kObjectTypeTableTemplate) {
            auto tt = static_cast<tb_namespace::TableTemplate *>(obj);
            auto axis = tt->get_axis1();
            if (axis == nullptr) continue;
            for (auto p = v; p; p = p->next) {
                if (__is_int_type(p) || __is_double_type(p)) {
                    axis->add_value(__get_double_value(p));
                } else if (__is_string_type(p)) {
                    std::vector<double> values;
                    __get_string_float_list(p->u.string_val, &values);
                    for (auto f : values) axis->add_value(f);
                }
            }
        }
    }
}
void LibBuilder::__build_index_2(BUILD_PARAM) {
    GOBJECTS
    for (auto &obj : objects) {
        if (obj->getObjectType() ==
            tb_namespace::ObjectType::kObjectTypeTableTemplate) {
            auto tt = static_cast<tb_namespace::TableTemplate *>(obj);
            auto axis = tt->get_axis2();
            if (axis == nullptr) continue;
            for (auto p = v; p; p = p->next) {
                if ((__is_int_type(p) || __is_double_type(p))) {
                    axis->add_value(__get_double_value(p));
                } else if (__is_string_type(p)) {
                    std::vector<double> values;
                    __get_string_float_list(p->u.string_val, &values);
                    for (auto f : values) axis->add_value(f);
                }
            }
        }
    }
}
void LibBuilder::__build_index_3(BUILD_PARAM) {
    GOBJECTS
    for (auto &obj : objects) {
        if (obj->getObjectType() ==
            tb_namespace::ObjectType::kObjectTypeTableTemplate) {
            auto tt = static_cast<tb_namespace::TableTemplate *>(obj);
            auto axis = tt->get_axis3();
            if (axis == nullptr) continue;
            for (auto p = v; p; p = p->next) {
                if ((__is_int_type(p) || __is_double_type(p))) {
                    axis->add_value(__get_double_value(p));
                } else if (__is_string_type(p)) {
                    std::vector<double> values;
                    __get_string_float_list(p->u.string_val, &values);
                    for (auto f : values) axis->add_value(f);
                }
            }
        }
    }
}

void LibBuilder::__build_values(BUILD_PARAM) {
    tb_namespace::Timing *timing_lib = tb_namespace::getTimingLib();
    if (timing_lib == nullptr) return;
    if (groupname_stack_.empty()) return;

    std::string groupname = groupname_stack_.top();
    if (groupname == "cell_rise" || groupname == "cell_fall" ||
        groupname == "rise_transition" || groupname == "fall_transition" ||
        groupname == "rise_constraint" || groupname == "fall_constraint") {
    } else {
        return;
    }

    std::vector<double> values;
    for (auto p = v; p; p = p->next) {
        if (__is_int_type(p) || __is_double_type(p)) {
            values.emplace_back(__get_double_value(p));
        } else if (__is_string_type(p)) {
            __get_string_float_list(p->u.string_val, &values);
        }
    }
    if (values.empty()) return;

    GOBJECTS
    for (auto &p : objects) {
        if (p->getObjectType() ==
            tb_namespace::ObjectType::kObjectTypeTimingArc) {
            auto timingarc = static_cast<tb_namespace::TimingArc *>(p);

            tb_namespace::TimingTable *tt = nullptr;
            if (axis_[0] && axis_[1] && axis_[2]) {
                auto p = tb_namespace::Object::createObject<tb_namespace::TimingTable3>(
                    tb_namespace::ObjectType::kObjectTypeTimingTable3,
                    timing_lib->getId());
                if (p) {
                    axis_[0]->setOwner(p);
                    p->set_axis1(axis_[0]->getId());
                    axis_[1]->setOwner(p);
                    p->set_axis2(axis_[1]->getId());
                    axis_[2]->setOwner(p);
                    p->set_axis3(axis_[2]->getId());
                    for (auto &v : values) p->add_value(v);
                    tt = p;
                }
            } else if (axis_[0] && axis_[1]) {
                auto p = tb_namespace::Object::createObject<tb_namespace::TimingTable2>(
                    tb_namespace::ObjectType::kObjectTypeTimingTable2,
                    timing_lib->getId());
                if (p) {
                    axis_[0]->setOwner(p);
                    p->set_axis1(axis_[0]->getId());
                    axis_[1]->setOwner(p);
                    p->set_axis2(axis_[1]->getId());
                    for (auto &v : values) p->add_value(v);
                    tt = p;
                }
            } else if (axis_[0]) {
                auto p = tb_namespace::Object::createObject<tb_namespace::TimingTable1>(
                    tb_namespace::ObjectType::kObjectTypeTimingTable1,
                    timing_lib->getId());
                if (p) {
                    axis_[0]->setOwner(p);
                    p->set_axis1(axis_[0]->getId());
                    for (auto &v : values) p->add_value(v);
                    tt = p;
                }
            } else {
                auto p = tb_namespace::Object::createObject<tb_namespace::TimingTable0>(
                    tb_namespace::ObjectType::kObjectTypeTimingTable0,
                    timing_lib->getId());
                if (p) p->set_value(values[0]);
                tt = p;
            }
            if (tt) {
                tt->setOwner(timingarc);
                if (groupname == "cell_rise")
                    timingarc->set_cell_rise(tt->getId());
                else if (groupname == "cell_fall")
                    timingarc->set_cell_fall(tt->getId());
                else if (groupname == "rise_transition")
                    timingarc->set_rise_transition(tt->getId());
                else if (groupname == "fall_transition")
                    timingarc->set_fall_transition(tt->getId());
                else if (groupname == "rise_constraint")
                    timingarc->set_rise_constraint(tt->getId());
                else if (groupname == "fall_constraint")
                    timingarc->set_fall_constraint(tt->getId());
            }
        }
    }
}

void LibBuilder::__get_terms_from_top_stack(
    std::vector<tb_namespace::TTerm *> *libterms) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
            tb_namespace::ObjectType::kObjectTypeTTerm) {
            libterms->emplace_back(static_cast<tb_namespace::TTerm *>(object));
        }
    }
}
void LibBuilder::__get_pg_terms_from_top_stack(
    std::vector<tb_namespace::TPgTerm *> *libpgterms) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
            tb_namespace::ObjectType::kObjectTypeTPgTerm) {
            libpgterms->emplace_back(
                static_cast<tb_namespace::TPgTerm *>(object));
        }
    }
}
void LibBuilder::__get_timingarcs_from_top_stack(
    std::vector<tb_namespace::TimingArc *> *libtimingarcs) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
            tb_namespace::ObjectType::kObjectTypeTimingArc) {
            libtimingarcs->emplace_back(
                static_cast<tb_namespace::TimingArc *>(object));
        }
    }
}
void LibBuilder::__get_objects_from_top_stack(
    std::vector<tb_namespace::Object *> *objects) {
    if (objects_stack_.empty()) return;

    ObjectList *objs = objects_stack_.top();
    while (objs != nullptr && objs->object == nullptr) {
        objs = objs->owner;
    }
    while (objs != nullptr) {
        if (objs->object != nullptr) objects->emplace_back(objs->object);
        objs = objs->next;
    }
}

void LibBuilder::__delete_object_list(ObjectList *objects) {
    if (objects->next == nullptr)
        delete objects;
    else
        __delete_object_list(objects->next);
}

float LibBuilder::__get_time_unit_multiply(const std::string &u) {
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
float LibBuilder::__get_resistance_unit_multiply(const std::string &u) {
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
float LibBuilder::__get_capacitive_unit_multiply(const std::string &u) {
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
float LibBuilder::__get_voltage_unit_multiply(const std::string &u) {
    std::string str;
    std::transform(u.begin(), u.end(), std::back_inserter(str), ::tolower);
    if (str == "v") return 1.0f;
    if (str == "mv")
        return 1e-3f;
    else
        return 0.0f;
}
float LibBuilder::__get_current_unit_multiply(const std::string &u) {
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
float LibBuilder::__get_power_unit_multiply(const std::string &u) {
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
float LibBuilder::__get_distance_unit_multiply(const std::string &u) {
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
void LibBuilder::__get_pin_names_from_bus_range(
    const char *str, std::vector<std::string> *pinNames) {
    const char *pattern =
        "\\s*([a-zA-Z]+)\\s*\\[\\s*([0-9]+)\\s*(:\\s*([0-9]+)\\s*){"
        "0,1}"
        "\\]\\s*";
    regex_t compiled;
    int error = regcomp(&compiled, pattern, REG_EXTENDED);
    if (error != 0) {
        pinNames->emplace_back(str);
        regfree(&compiled);
        return;
    }

    regmatch_t pmatch[5];
    error = regexec(&compiled, str, 5, pmatch, 0);
    if (error == 0 && compiled.re_nsub >= 2) {
        std::string bus_name = "";
        int from = -1;
        int to = -1;
        if (pmatch[1].rm_so != -1) {
            int len = pmatch[1].rm_eo - pmatch[1].rm_so;
            char *value = reinterpret_cast<char *>(malloc(len + 1));
            if (value != NULL) {
                memset(value, 0, len + 1);
                memcpy(value, str + pmatch[1].rm_so, len);
                bus_name = value;
                delete value;
            }
        }
        if (compiled.re_nsub >= 3) {
            if (pmatch[2].rm_so != -1) {
                int len = pmatch[2].rm_eo - pmatch[2].rm_so;
                char *value = reinterpret_cast<char *>(malloc(len + 1));
                if (value != NULL) {
                    memset(value, 0, len + 1);
                    memcpy(value, str + pmatch[2].rm_so, len);
                    from = atoi(value);
                    delete value;
                }
            }
        }
        if (compiled.re_nsub >= 4) {
            if (pmatch[3].rm_so != -1) {
                int len = pmatch[4].rm_eo - pmatch[4].rm_so;
                char *value = reinterpret_cast<char *>(malloc(len + 1));
                if (value != NULL) {
                    memset(value, 0, len + 1);
                    memcpy(value, str + pmatch[4].rm_so, len);
                    to = atoi(value);
                    delete value;
                }
            }
        }

        if (from == -1) {
            pinNames->emplace_back(str);
        } else if (to == -1) {
            std::string str = bus_name + std::to_string(from);
            pinNames->emplace_back(str);
        } else {
            if (from > to) {
                from = from + to;
                to = from - to;
                from = from - to;
            }
            for (int i = from; i <= to; ++i) {
                std::string str = bus_name + std::to_string(i);
                pinNames->emplace_back(str);
            }
        }

    } else {
        pinNames->emplace_back(str);
    }
    regfree(&compiled);
}
tb_namespace::TCell *LibBuilder::__get_tcell_by_timingarc(
    tb_namespace::TimingArc *t) {
    if (t == nullptr) return nullptr;
    tb_namespace::ObjectId owner = t->getOwnerId();
    tb_namespace::TTerm *p = nullptr;
    if (owner != UNINIT_OBJECT_ID)
        p = tb_namespace::Object::addr<tb_namespace::TTerm>(owner);
    if (p == nullptr) return nullptr;
    owner = p->getOwnerId();
    if (owner != UNINIT_OBJECT_ID)
        return tb_namespace::Object::addr<tb_namespace::TCell>(owner);
    else
        return nullptr;
}

bool LibBuilder::__is_int_type(timinglib_attribute_value *v) {
    if (v->type == timinglib_attribute_value_type::kTIMINGLIB__VAL_INT)
        return true;
    return false;
}
bool LibBuilder::__is_double_type(timinglib_attribute_value *v) {
    if (v->type == timinglib_attribute_value_type::kTIMINGLIB__VAL_DOUBLE)
        return true;
    return false;
}
bool LibBuilder::__is_string_type(timinglib_attribute_value *v) {
    if (v->type == timinglib_attribute_value_type::kTIMINGLIB__VAL_STRING)
        return true;
    return false;
}
bool LibBuilder::__is_bool_type(timinglib_attribute_value *v) {
    if (v->type == timinglib_attribute_value_type::kTIMINGLIB__VAL_BOOLEAN)
        return true;
    return false;
}
float LibBuilder::__get_double_value(timinglib_attribute_value *v) {
    if (__is_double_type(v))
        return v->u.double_val;
    else if (__is_int_type(v))
        return v->u.int_val;
    else
        return 0.0f;
}
void LibBuilder::__get_string_float_list(char *str,
                                         std::vector<double> *values) {
    if (values == nullptr) return;
    char *p;
    char *t;
    p = str;
    double x = 0;
    while ((x = strtod(p, &t)), t != p) {
        values->emplace_back(x);
        if (t && *t == 0) break;

        /* skip over intervening stuff to the next number */
        while (*t == ',' || *t == ' ' || *t == '\t' || *t == '\n' ||
               *t == '\r' || *t == '\\') {
            if (*t == '\\' && *(t + 1) == '\n') /*skip over line escapes */
                t++;
            t++;
        }
        p = t;
    }
}
void LibBuilder::__split_string_by_delim(char *str, const char *delim,
                                         std::vector<std::string> *rets) {
    if (rets == nullptr) return;

    char temp[200] = {'\0'};
    snprintf(temp, sizeof(temp), "%s", str);
    char *saveptr;
    char *ptr;
    ptr = strtok_r(temp, delim, &saveptr);
    while (ptr != nullptr) {
        rets->emplace_back(ptr);
        ptr = strtok_r(nullptr, delim, &saveptr);
    }
}
}  // namespace Timinglib
