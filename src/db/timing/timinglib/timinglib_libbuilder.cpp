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
#include "db/timing/timinglib/libset.h"
#include "db/timing/timinglib/timinglib_cell.h"
#include "db/timing/timinglib/timinglib_lib.h"
#include "db/timing/timinglib/timinglib_libanalysis.h"
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
    __getObjectsFromTopStack(&objects);          \
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
      setResistanceUnit_(false),
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

    __buildGroupMap();
    __buildAttrMap();
}

LibBuilder::~LibBuilder() {
    group_builder_map_.clear();
    attr_builder_map_.clear();

    while (!objects_stack_.empty()) {
        ObjectList *objects = objects_stack_.top();
        if (objects != nullptr) __deleteObjectList(objects);
        objects_stack_.pop();
    }
    while (!groupname_stack_.empty()) {
        groupname_stack_.pop();
    }
    scaling_factors_str_map_.clear();
}

void LibBuilder::setLibSyn(LibSyn *s) { libsyn_ = s; }
void LibBuilder::__buildGroupMap(void) {
    group_builder_map_ = {
        // library
        {"library", BINDG(__buildLibrary)},
        {"operating_conditions", BINDG(__buildOperatingConditions)},
        {"wire_load", BINDG(__buildWireLoad)},
        {"wire_load_table", BINDG(__buildWireLoadTable)},
        {"wire_load_selection", BINDG(__buildWireLoadSelection)},
        {"lu_table_template", BINDG(__buildLuTableTemplate)},

        // cell
        {"cell", BINDG(__buildCell)},
        {"pin", BINDG(__buildPin)},
        {"pg_pin", BINDG(__buildPgPin)},
        {"ff", BINDG(__buildFf)},
        {"latch", BINDG(__buildLatch)},
        {"statetable", BINDG(__buildStatetable)},
        {"bus", BINDG(__buildBus)},
        {"bundle", BINDG(__buildBundle)},

        // cell or library
        {"type", BINDG(__buildType)},

        // bus, bundle or pin
        {"timing", BINDG(__buildTiming)},
        {"cell_rise", BINDG(__buildCellRise)},
        {"cell_fall", BINDG(__buildCellFall)},
        {"rise_transition", BINDG(__buildRiseTransition)},
        {"fall_transition", BINDG(__buildFallTransition)},
        {"rise_constraint", BINDG(__buildRiseConstraint)},
        {"fall_constraint", BINDG(__buildFallConstraint)},
    };
}

void LibBuilder::__buildAttrMap(void) {
    attr_builder_map_ = {
        // library
        {"default_operating_conditions", BINDA(__buildDefaultOc)},
        {"time_unit", BINDA(__buildTimeUnit)},
        {"pulling_resistance_unit", BINDA(__buildPullingResistanceUnit)},
        {"resistance_unit", BINDA(__buildResistanceUnit)},
        {"capacitive_load_unit", BINDA(__buildCapacitiveLoadUnit)},
        {"voltage_unit", BINDA(__buildVoltageUnit)},
        {"current_unit", BINDA(__buildCurrentUnit)},
        {"leakage_power_unit", BINDA(__buildLeakagePowerUnit)},
        {"distance_unit", BINDA(__buildDistanceUnit)},
        {"nom_temperature", BINDA(__buildNomTemperature)},
        {"nom_voltage", BINDA(__buildNomVoltage)},
        {"nom_process", BINDA(__buildNomProcess)},
        {"input_threshold_pct_fall", BINDA(__buildInputThresholdPctFall)},
        {"input_threshold_pct_rise", BINDA(__buildInputThresholdPctRise)},
        {"output_threshold_pct_fall", BINDA(__buildOutputThresholdPctFall)},
        {"output_threshold_pct_rise", BINDA(__buildOutputThresholdPctRise)},
        {"slew_lower_threshold_pct_fall",
         BINDA(__buildSlewLowerThresholdPctFall)},
        {"slew_lower_threshold_pct_rise",
         BINDA(__buildSlewLowerThresholdPctRise)},
        {"slew_upper_threshold_pct_fall",
         BINDA(__buildSlewUpperThresholdPctFall)},
        {"slew_upper_threshold_pct_rise",
         BINDA(__buildSlewUpperThresholdPctRise)},
        {"slew_derate_from_library", BINDA(__buildSlewDerateFromLibrary)},
        {"default_wire_load", BINDA(__buildDefaultWireLoad)},
        {"default_wire_load_area", BINDA(__buildDefaultWireLoadArea)},
        {"default_wire_load_mode", BINDA(__buildDefaultWireLoadMode)},
        {"default_wire_load_capacitance",
         BINDA(__buildDefaultWireLoadCapacitance)},
        {"default_wire_load_resistance",
         BINDA(__buildDefaultWireLoadResistance)},
        {"default_wire_load_selection", BINDA(__buildDefaultWireLoadSelection)},
        {"default_input_pin_cap", BINDA(__buildDefaultInputPinCap)},
        {"default_output_pin_cap", BINDA(__buildDefaultOutputPinCap)},
        {"default_inout_pin_cap", BINDA(__buildDefaultInoutPinCap)},
        {"default_max_capacitance", BINDA(__buildDefaultMaxCapacitance)},
        {"default_max_fanout", BINDA(__buildDefaultMaxFanout)},
        {"default_max_transition", BINDA(__buildDefaultMaxTransition)},
        {"default_fanout_load", BINDA(__buildDefaultFanoutLoad)},
        {"default_cell_leakage_power", BINDA(__buildDefaultCellLeakagePower)},
        {"voltage_map", BINDA(__buildVoltageMap)},

        // operating conditions
        {"process", BINDA(__buildProcess)},
        {"temperature", BINDA(__buildTemperature)},
        {"voltage", BINDA(__buildVoltage)},
        {"tree_type", BINDA(__buildTreeType)},

        // cell
        {"area", BINDA(__buildArea)},
        {"cell_footprint", BINDA(__buildCellFootprint)},
        {"dont_touch", BINDA(__buildDontTouch)},
        {"dont_use", BINDA(__buildDontUse)},
        {"always_on", BINDA(__buildAlwaysOn)},
        {"is_macro_cell", BINDA(__buildIsMacroCell)},
        {"is_pad", BINDA(__buildIsPad)},
        {"is_decap_cell", BINDA(__buildIsDecapCell)},
        {"is_filler_cell", BINDA(__buildIsFillerCell)},
        {"is_tap_cell", BINDA(__buildIsTapCell)},
        {"is_clock_gating_cell", BINDA(__buildIsClockGatingCell)},
        {"is_clock_isolation_cell", BINDA(__buildIsClockIsolationCell)},
        {"is_isolation_cell", BINDA(__buildIsIsolationCell)},
        {"is_no_enable", BINDA(__buildIsNoEnable)},
        {"clock_gating_integrated_cell",
         BINDA(__buildClockGatingIntegratedCell)},
        {"antenna_diode_type", BINDA(__buildAntennaDiodeType)},
        {"is_level_shifter", BINDA(__buildIsLevelShifter)},
        {"cell_leakage_power", BINDA(__buildCellLeakagePower)},
        {"switch_cell_type", BINDA(__buildSwitchCellType)},
        {"retention_cell", BINDA(__buildRetentionCell)},

        // pin
        {"related_ground_pin", BINDA(__buildRelatedGroundPin)},
        {"related_power_pin", BINDA(__buildRelatedPowerPin)},
        {"max_transition", BINDA(__buildMaxTransition)},
        {"fall_capacitance", BINDA(__buildFallCapacitance)},
        {"rise_capacitance", BINDA(__buildRiseCapacitance)},
        {"three_state", BINDA(__buildThreeState)},
        {"capacitance", BINDA(__buildCapacitance)},
        {"max_capacitance", BINDA(__buildMaxCapacitance)},
        {"min_capacitance", BINDA(__buildMinCapacitance)},
        {"direction", BINDA(__buildDirection)},
        {"max_fanout", BINDA(__buildMaxFanout)},
        {"min_fanout", BINDA(__buildMinFanout)},
        {"min_transition", BINDA(__buildMinTransition)},
        {"min_period", BINDA(__buildMinPeriod)},
        {"min_pulse_width_high", BINDA(__buildMinPulseWidthHigh)},
        {"min_pulse_width_low", BINDA(__buildMinPulseWidthLow)},
        {"clock", BINDA(__buildClock)},
        {"clock_gate_clock_pin", BINDA(__buildClockGateClockPin)},
        {"clock_gate_enable_pin", BINDA(__buildClockGateEnablePin)},
        {"function", BINDA(__buildFunction)},

        // bundle
        {"members", BINDA(__buildMembers)},

        // bus
        {"bus_type", BINDA(__buildBusType)},

        // pg_pin
        {"pg_type", BINDA(__buildPgType)},
        {"voltage_name", BINDA(__buildVoltageName)},

        // type
        {"bit_width", BINDA(__buildBitWidth)},
        {"bit_from", BINDA(__buildBitFrom)},
        {"bit_to", BINDA(__buildBitTo)},
        {"downto", BINDA(__buildDownto)},

        // timing
        {"timing_sense", BINDA(__buildTimingSense)},
        {"timing_type", BINDA(__buildTimingType)},
        {"when", BINDA(__buildWhen)},
        {"related_pin", BINDA(__buildRelatedPin)},

        // wire_load
        {"resistance", BINDA(__buildResistance)},
        {"slope", BINDA(__buildSlope)},
        {"fanout_length", BINDA(__buildFanoutLength)},

        // wire_load_table
        {"fanout_capacitance", BINDA(__buildFanoutCapacitance)},
        {"fanout_resistance", BINDA(__buildFanoutResistance)},
        {"fanout_area", BINDA(__buildFanoutArea)},

        // wire_load_selection
        {"wire_load_from_area", BINDA(__buildWireLoadFromArea)},

        // lu_table_template
        {"variable_1", BINDA(__buildVariable1)},
        {"variable_2", BINDA(__buildVariable2)},
        {"variable_3", BINDA(__buildVariable3)},
        {"index_1", BINDA(__buildIndex1)},
        {"index_2", BINDA(__buildIndex2)},
        {"index_3", BINDA(__buildIndex3)},

        // cell_rise cell_fall rise_transition
        // fall_transition rise_constraint fall_constraint
        {"values", BINDA(__buildValues)},
    };
    __buildScalingFactorsAttrs();
}

void LibBuilder::__buildScalingFactorsAttrs(void) {
    for (int i = 0;
         i < static_cast<int>(tb_namespace::ScaleFactorType::kUnknown); ++i) {
        tb_namespace::ScaleFactorType type =
            static_cast<tb_namespace::ScaleFactorType>(i);
        for (int j = 0;
             j < static_cast<int>(tb_namespace::ScaleFactorPvt::kUnknown);
             ++j) {
            tb_namespace::ScaleFactorPvt pvt =
                static_cast<tb_namespace::ScaleFactorPvt>(j);
            if (tb_namespace::ScaleFactors::isRiseFallSuffix(type)) {
                for (int k = 0;
                     k < static_cast<int>(
                             tb_namespace::ScaleFactorRiseFall::kUnknown);
                     ++k) {
                    tb_namespace::ScaleFactorRiseFall rf =
                        static_cast<tb_namespace::ScaleFactorRiseFall>(k);
                    std::string name =
                        stringFormat("k_%s_%s_%s", toString(pvt).c_str(),
                                     toString(type).c_str(), toString(rf));
                    scaling_factors_str_map_[name] =
                        std::make_tuple(type, pvt, rf);
                }
            } else if (tb_namespace::ScaleFactors::isRiseFallPrefix(type)) {
                for (int k = 0;
                     k < static_cast<int>(
                             tb_namespace::ScaleFactorRiseFall::kUnknown);
                     ++k) {
                    tb_namespace::ScaleFactorRiseFall rf =
                        static_cast<tb_namespace::ScaleFactorRiseFall>(k);
                    std::string name =
                        stringFormat("k_%s_%s_%s", toString(pvt).c_str(),
                                     toString(rf), toString(type).c_str());
                    scaling_factors_str_map_[name] =
                        std::make_tuple(type, pvt, rf);
                }
            } else if (tb_namespace::ScaleFactors::isHighLowSuffix(type)) {
                for (int k = 0;
                     k < static_cast<int>(
                             tb_namespace::ScaleFactorRiseFall::kUnknown);
                     ++k) {
                    tb_namespace::ScaleFactorRiseFall rf =
                        static_cast<tb_namespace::ScaleFactorRiseFall>(k);
                    if (rf == tb_namespace::ScaleFactorRiseFall::kRise) {
                        std::string name =
                            stringFormat("k_%s_%s_%s", toString(pvt).c_str(),
                                         "high", toString(type).c_str());
                        scaling_factors_str_map_[name] =
                            std::make_tuple(type, pvt, rf);
                    } else if (rf == tb_namespace::ScaleFactorRiseFall::kFall) {
                        std::string name =
                            stringFormat("k_%s_%s_%s", toString(pvt).c_str(),
                                         "low", toString(type).c_str());
                        scaling_factors_str_map_[name] =
                            std::make_tuple(type, pvt, rf);
                    }
                }
            } else {
                std::string name = stringFormat(
                    "k_%s_%s", toString(pvt).c_str(), toString(type).c_str());
                scaling_factors_str_map_[name] = std::make_tuple(
                    type, pvt, tb_namespace::ScaleFactorRiseFall::kUnknown);
            }
        }
    }
    for (auto &p : scaling_factors_str_map_) {
        attr_builder_map_[p.first] = BINDA(__buildScalingFactorsAttr);
    }
}

void LibBuilder::beginGroup(timinglib_head *h) {
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

void LibBuilder::endGroup() {
    if (groupname_stack_.top() == "library") {
        if (setResistanceUnit_ == false) {
            std::vector<tb_namespace::Object *> objects;
            __getObjectsFromTopStack(&objects);
            for (auto &object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTLib) {
                    tb_namespace::TLib *lib =
                        static_cast<tb_namespace::TLib *>(object);
                    tb_namespace::TUnits *units = lib->getUnits();
                    if (units) {
                        units->getResistanceUnit() =
                            units->getPullingResistanceUnit();
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

void LibBuilder::buildAttribute(buildParam) {
    auto findIt = attr_builder_map_.find(name);
    if (findIt != attr_builder_map_.end()) (findIt->second)(name, v);
}

void LibBuilder::__buildLibrary(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__isStringType(v)) {
            std::string str = v->u.string_val;
            tb_namespace::TLib *lib = libset_->addTLib(h->filename, str);
            // lib->setFilename(h->filename);

            std::string upper;
            std::transform(str.begin(), str.end(), std::back_inserter(upper),
                           ::toupper);

            if (upper.find("NLDM") != std::string::npos)
                lib->setTimingModelType(tb_namespace::TimingModel::kNLDM);
            else if (upper.find("CSS") != std::string::npos)
                lib->setTimingModelType(tb_namespace::TimingModel::kCCS);
            else if (upper.find("ECSM") != std::string::npos)
                lib->setTimingModelType(tb_namespace::TimingModel::kECSM);

            ObjectList *objects = new ObjectList();
            objects->object = lib;
            if (!objects_stack_.empty()) objects->owner = objects_stack_.top();
            objects_stack_.push(objects);
            push_status_ = 1;
        }
    }
}

void LibBuilder::__buildCell(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__isStringType(v)) {
            GOBJECTS
            for (auto object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTLib) {
                    tb_namespace::TCell *cell =
                        static_cast<tb_namespace::TLib *>(object)
                            ->addTimingCell(v->u.string_val);

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

void LibBuilder::__buildPin(timinglib_head *h) {
    bool isBus = false;
    bool isBundle = false;
    if (!groupname_stack_.empty()) {
        std::string lastgroup_name = groupname_stack_.top();
        if (lastgroup_name == "bus")
            isBus = true;
        else if (lastgroup_name == "bundle")
            isBundle = true;
    }

    std::vector<std::string> terms;
    for (auto v = h->list; v; v = v->next) {
        if (__isStringType(v)) {
#if 1
            if (isBus) {
                char *content = v->u.string_val;
                __getPinNamesFromBusRange(content, &terms);
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
        __getObjectsFromTopStack(&objects);
        if (!objects.empty()) {
            tb_namespace::Object *object = objects[0];
            if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell) {
                for (auto str : terms) {
                    tb_namespace::TTerm *term =
                        static_cast<tb_namespace::TCell *>(object)
                            ->getOrCreateTerm(str);
                    if (term) {
                        term->setBus(isBus);
                        term->setBundle(isBundle);
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
void LibBuilder::__buildPgPin(timinglib_head *h) {
    std::vector<std::string> terms;
    for (auto v = h->list; v; v = v->next) {
        terms.emplace_back(v->u.string_val);
    }
    if (!terms.empty()) {
        ObjectList *pinList = nullptr;
        ObjectList *l = nullptr;

        std::vector<tb_namespace::Object *> objects;
        __getObjectsFromTopStack(&objects);
        if (!objects.empty()) {
            tb_namespace::Object *object = objects[0];
            if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell) {
                for (auto &str : terms) {
                    tb_namespace::TPgTerm *term =
                        static_cast<tb_namespace::TCell *>(object)
                            ->getOrCreatePgTerm(str);
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

void LibBuilder::__buildOperatingConditions(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__isStringType(v)) {
            GOBJECTS
            for (auto object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTLib) {
                    tb_namespace::TLib *p =
                        static_cast<tb_namespace::TLib *>(object);
                    tb_namespace::OperatingConditions *oc =
                        p->getOperatingConditions(v->u.string_val);
                    if (oc == nullptr)
                        oc = p->addOperatingConditions(v->u.string_val);

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
void LibBuilder::__buildWireLoad(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__isStringType(v)) {
            GOBJECTS
            for (auto object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTLib) {
                    tb_namespace::TLib *lib =
                        static_cast<tb_namespace::TLib *>(object);
                    auto p = lib->getWireLoad(v->u.string_val);
                    if (p == nullptr) p = lib->addWireLoad(v->u.string_val);

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
void LibBuilder::__buildWireLoadTable(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__isStringType(v)) {
            GOBJECTS
            for (auto object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTLib) {
                    tb_namespace::TLib *lib =
                        static_cast<tb_namespace::TLib *>(object);
                    auto p = lib->getWireLoadTable(v->u.string_val);
                    if (p == nullptr)
                        p = lib->addWireLoadTable(v->u.string_val);

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
void LibBuilder::__buildWireLoadSelection(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__isStringType(v)) {
            GOBJECTS
            for (auto object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTLib) {
                    tb_namespace::TLib *lib =
                        static_cast<tb_namespace::TLib *>(object);
                    auto p = lib->getWireLoadSelection(v->u.string_val);
                    if (p == nullptr)
                        p = lib->addWireLoadSelection(v->u.string_val);

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
void LibBuilder::__buildLuTableTemplate(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__isStringType(v)) {
            GOBJECTS
            for (auto object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTLib) {
                    tb_namespace::TLib *lib =
                        static_cast<tb_namespace::TLib *>(object);
                    auto p = lib->getTableTemplate(v->u.string_val);
                    if (p == nullptr)
                        p = lib->addTableTemplate(v->u.string_val);

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
void LibBuilder::__buildFf(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__isStringType(v)) {
            GOBJECTS
            for (auto object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTCell) {
                    tb_namespace::TCell *cell =
                        static_cast<tb_namespace::TCell *>(object);
                    if (cell) cell->setSequential(true);

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
void LibBuilder::__buildLatch(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__isStringType(v)) {
            GOBJECTS
            for (auto object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTCell) {
                    tb_namespace::TCell *cell =
                        static_cast<tb_namespace::TCell *>(object);
                    if (cell) cell->setSequential(true);

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
void LibBuilder::__buildStatetable(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__isStringType(v)) {
            GOBJECTS
            for (auto object : objects) {
                if (object->getObjectType() ==
                    tb_namespace::ObjectType::kObjectTypeTCell) {
                    tb_namespace::TCell *cell =
                        static_cast<tb_namespace::TCell *>(object);
                    if (cell) cell->setSequential(true);

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
void LibBuilder::__buildBus(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__isStringType(v)) {
            cur_bus_or_bundle_name_ = v->u.string_val;
        }
    }
}
void LibBuilder::__buildBundle(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__isStringType(v)) {
            cur_bus_or_bundle_name_ = v->u.string_val;
        }
    }
}
void LibBuilder::__buildType(timinglib_head *h) {
    auto v = h->list;
    if (v != nullptr) {
        if (__isStringType(v)) {
            cur_type_group_name_ = v->u.string_val;
        }
    }
}
void LibBuilder::__buildTiming(timinglib_head *h) {
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
        if (timing_lib != nullptr) __getTermsFromTopStack(&libpins);
    }

    ObjectList *pinList = nullptr;
    ObjectList *l = nullptr;
    for (auto &p : libpins) {
        auto t = tb_namespace::Object::createObject<tb_namespace::TimingArc>(
            tb_namespace::ObjectType::kObjectTypeTimingArc,
            timing_lib->getId());
        if (t) {
            t->setOwner(p);
            p->addTimingarc(t->getId());

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
void LibBuilder::__buildCellRise(timinglib_head *h) {
    tb_namespace::Timing *timing_lib = tb_namespace::getTimingLib();
    auto v = h->list;
    if (v && __isStringType(v)) {
        std::vector<tb_namespace::TimingArc *> timingarcs;
        __getTimingarcsFromTopStack(&timingarcs);
        if (timingarcs.empty()) return;
        tb_namespace::TCell *cell = __getTcellByTimingarc(timingarcs[0]);
        if (cell == nullptr) return;
        tb_namespace::ObjectId owner = cell->getOwnerId();
        tb_namespace::TLib *lib =
            tb_namespace::Object::addr<tb_namespace::TLib>(owner);
        if (lib == nullptr) return;
        tb_namespace::TableTemplate *tt =
            lib->getTableTemplate(v->u.string_val);
        if (tt == nullptr) return;
        auto axis1 = tt->getAxis1();
        if (axis1) {
            axis_[0] =
                tb_namespace::Object::createObject<tb_namespace::TableAxis>(
                    tb_namespace::ObjectType::kObjectTypeTableAxis,
                    timing_lib->getId());
            if (axis_[0]) *(axis_[0]) = *axis1;
        }
        auto axis2 = tt->getAxis2();
        if (axis2) {
            axis_[1] =
                tb_namespace::Object::createObject<tb_namespace::TableAxis>(
                    tb_namespace::ObjectType::kObjectTypeTableAxis,
                    timing_lib->getId());
            if (axis_[1]) *(axis_[1]) = *axis2;
        }
        auto axis3 = tt->getAxis3();
        if (axis3) {
            axis_[2] =
                tb_namespace::Object::createObject<tb_namespace::TableAxis>(
                    tb_namespace::ObjectType::kObjectTypeTableAxis,
                    timing_lib->getId());
            if (axis_[2]) *(axis_[2]) = *axis3;
        }
    }
}
void LibBuilder::__buildCellFall(timinglib_head *h) { __buildCellRise(h); }
void LibBuilder::__buildRiseTransition(timinglib_head *h) {
    __buildCellRise(h);
}
void LibBuilder::__buildFallTransition(timinglib_head *h) {
    __buildCellRise(h);
}
void LibBuilder::__buildRiseConstraint(timinglib_head *h) {
    __buildCellRise(h);
}
void LibBuilder::__buildFallConstraint(timinglib_head *h) {
    __buildCellRise(h);
}

void LibBuilder::__buildDefaultOc(buildParam) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __isStringType(v)) {
            tb_namespace::TLib *p = static_cast<tb_namespace::TLib *>(object);
            tb_namespace::OperatingConditions *oc =
                p->getOperatingConditions(v->u.string_val);
            if (oc == nullptr) {
                oc = p->addOperatingConditions(v->u.string_val);
            }
            if (oc != nullptr) p->setDefaultOperatingConditions(oc->getId());
        }
    }
}
void LibBuilder::__buildTimeUnit(buildParam) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __isStringType(v)) {
            tb_namespace::TLib *p = static_cast<tb_namespace::TLib *>(object);
            tb_namespace::TUnits *units = p->getOrCreateUnits();
            if (units != nullptr) {
                int d = 0;
                char suffix[10] = {'\0'};
                sscanf(v->u.string_val, "%d%s", &d, suffix);
                tb_namespace::TUnit &unit = units->getTimeUnit();
                unit.digits = d;
                unit.suffix = suffix;
                unit.scale = tb_namespace::UnitMultiply::getTimeUnitMultiply(unit.suffix);
            }
        }
    }
}

void LibBuilder::__buildPullingResistanceUnit(buildParam) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __isStringType(v)) {
            tb_namespace::TLib *p = static_cast<tb_namespace::TLib *>(object);
            tb_namespace::TUnits *units = p->getOrCreateUnits();
            if (units != nullptr) {
                int d = 0;
                char suffix[10] = {'\0'};
                sscanf(v->u.string_val, "%d%s", &d, suffix);
                tb_namespace::TUnit &unit = units->getPullingResistanceUnit();
                unit.digits = d;
                unit.suffix = suffix;
                unit.scale = tb_namespace::UnitMultiply::getResistanceUnitMultiply(unit.suffix);
            }
        }
    }
}
void LibBuilder::__buildResistanceUnit(buildParam) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __isStringType(v)) {
            tb_namespace::TLib *p = static_cast<tb_namespace::TLib *>(object);
            tb_namespace::TUnits *units = p->getOrCreateUnits();
            if (units != nullptr) {
                int d = 0;
                char suffix[10] = {'\0'};
                sscanf(v->u.string_val, "%d%s", &d, suffix);
                tb_namespace::TUnit &unit = units->getResistanceUnit();
                unit.digits = d;
                unit.suffix = suffix;
                unit.scale = tb_namespace::UnitMultiply::getResistanceUnitMultiply(unit.suffix);
                setResistanceUnit_ = true;
            }
        }
    }
}
void LibBuilder::__buildCapacitiveLoadUnit(buildParam) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __isIntType(v)) {
            tb_namespace::TLib *p = static_cast<tb_namespace::TLib *>(object);
            tb_namespace::TUnits *units = p->getOrCreateUnits();
            if (units != nullptr) {
                int d = v->u.int_val;
                tb_namespace::TUnit &unit = units->getCapacitanceUnit();
                unit.digits = d;
                if (v->next != nullptr && __isStringType(v->next)) {
                    unit.suffix = v->next->u.string_val;
                    unit.scale = tb_namespace::UnitMultiply::getCapacitiveUnitMultiply(unit.suffix);
                }
            }
        }
    }
}
void LibBuilder::__buildVoltageUnit(buildParam) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __isStringType(v)) {
            tb_namespace::TLib *p = static_cast<tb_namespace::TLib *>(object);
            tb_namespace::TUnits *units = p->getOrCreateUnits();
            if (units != nullptr) {
                int d = 0;
                char suffix[10] = {'\0'};
                sscanf(v->u.string_val, "%d%s", &d, suffix);
                tb_namespace::TUnit &unit = units->getVolatgeUnit();
                unit.digits = d;
                unit.suffix = suffix;
                unit.scale = tb_namespace::UnitMultiply::getVoltageUnitMultiply(unit.suffix);
            }
        }
    }
}
void LibBuilder::__buildCurrentUnit(buildParam) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __isStringType(v)) {
            tb_namespace::TLib *p = static_cast<tb_namespace::TLib *>(object);
            tb_namespace::TUnits *units = p->getOrCreateUnits();
            if (units != nullptr) {
                int d = 0;
                char suffix[10] = {'\0'};
                sscanf(v->u.string_val, "%d%s", &d, suffix);
                tb_namespace::TUnit &unit = units->getCurrentUnit();
                unit.digits = d;
                unit.suffix = suffix;
                unit.scale = tb_namespace::UnitMultiply::getCurrentUnitMultiply(unit.suffix);
            }
        }
    }
}
void LibBuilder::__buildLeakagePowerUnit(buildParam) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __isStringType(v)) {
            tb_namespace::TLib *p = static_cast<tb_namespace::TLib *>(object);
            tb_namespace::TUnits *units = p->getOrCreateUnits();
            if (units != nullptr) {
                int d = 0;
                char suffix[10] = {'\0'};
                sscanf(v->u.string_val, "%d%s", &d, suffix);
                tb_namespace::TUnit &unit = units->getPowerUnit();
                unit.digits = d;
                unit.suffix = suffix;
                unit.scale = tb_namespace::UnitMultiply::getPowerUnitMultiply(unit.suffix);
            }
        }
    }
}
void LibBuilder::__buildDistanceUnit(buildParam) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __isStringType(v)) {
            tb_namespace::TLib *p = static_cast<tb_namespace::TLib *>(object);
            tb_namespace::TUnits *units = p->getOrCreateUnits();
            if (units != nullptr) {
                int d = 0;
                char suffix[10] = {'\0'};
                sscanf(v->u.string_val, "%d%s", &d, suffix);
                tb_namespace::TUnit &unit = units->getDistanceUnit();
                unit.digits = d;
                unit.suffix = suffix;
                unit.scale = tb_namespace::UnitMultiply::getDistanceUnitMultiply(unit.suffix);
            }
        }
    }
}
void LibBuilder::__buildNomTemperature(buildParam) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            if (static_cast<tb_namespace::TLib *>(object) != nullptr)
                static_cast<tb_namespace::TLib *>(object)

                    ->setNominalTemperature(__getDoubleValue(v));
        }
    }
}

void LibBuilder::__buildNomVoltage(buildParam) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            if (static_cast<tb_namespace::TLib *>(object) != nullptr)
                static_cast<tb_namespace::TLib *>(object)

                    ->setNominalVoltage(__getDoubleValue(v));
        }
    }
}

void LibBuilder::__buildNomProcess(buildParam) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            if (static_cast<tb_namespace::TLib *>(object) != nullptr)
                static_cast<tb_namespace::TLib *>(object)

                    ->setNominalProcess(__getDoubleValue(v));
        }
    }
}

void LibBuilder::__buildInputThresholdPctFall(buildParam) {
    if (objects_stack_.empty()) return;
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            if (static_cast<tb_namespace::TLib *>(object) != nullptr)
                static_cast<tb_namespace::TLib *>(object)

                    ->setInputThresholdPctFall(__getDoubleValue(v));
        }
    }
}

void LibBuilder::__buildInputThresholdPctRise(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            if (static_cast<tb_namespace::TLib *>(object) != nullptr)
                static_cast<tb_namespace::TLib *>(object)

                    ->setInputThresholdPctRise(__getDoubleValue(v));
        }
    }
}

void LibBuilder::__buildOutputThresholdPctFall(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            if (static_cast<tb_namespace::TLib *>(object) != nullptr)
                static_cast<tb_namespace::TLib *>(object)

                    ->setOutputThresholdPctFall(__getDoubleValue(v));
        }
    }
}

void LibBuilder::__buildOutputThresholdPctRise(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            if (static_cast<tb_namespace::TLib *>(object) != nullptr)
                static_cast<tb_namespace::TLib *>(object)

                    ->setOutputThresholdPctRise(__getDoubleValue(v));
        }
    }
}

void LibBuilder::__buildSlewLowerThresholdPctFall(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            if (static_cast<tb_namespace::TLib *>(object) != nullptr)
                static_cast<tb_namespace::TLib *>(object)

                    ->setSlewLowerThresholdPctFall(__getDoubleValue(v));
        }
    }
}

void LibBuilder::__buildSlewLowerThresholdPctRise(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            if (static_cast<tb_namespace::TLib *>(object) != nullptr)
                static_cast<tb_namespace::TLib *>(object)

                    ->setSlewLowerThresholdPctRise(__getDoubleValue(v));
        }
    }
}

void LibBuilder::__buildSlewUpperThresholdPctFall(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            if (static_cast<tb_namespace::TLib *>(object) != nullptr)
                static_cast<tb_namespace::TLib *>(object)

                    ->setSlewUpperThresholdPctFall(__getDoubleValue(v));
        }
    }
}

void LibBuilder::__buildSlewUpperThresholdPctRise(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            if (static_cast<tb_namespace::TLib *>(object) != nullptr)
                static_cast<tb_namespace::TLib *>(object)

                    ->setSlewUpperThresholdPctRise(__getDoubleValue(v));
        }
    }
}

void LibBuilder::__buildSlewDerateFromLibrary(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            if (static_cast<tb_namespace::TLib *>(object) != nullptr)
                static_cast<tb_namespace::TLib *>(object)

                    ->setSlewDerateFromLibrary(__getDoubleValue(v));
        }
    }
}

void LibBuilder::__buildScalingFactorsAttr(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            auto findIt = scaling_factors_str_map_.find(name);
            if (findIt != scaling_factors_str_map_.end()) {
                tb_namespace::TLib *lib =
                    static_cast<tb_namespace::TLib *>(object);
                tb_namespace::ScaleFactors *sf =
                    lib->getOrCreateScalingFactors("");
                if (sf) {
                    auto &q = findIt->second;
                    tb_namespace::ScaleFactorType type = std::get<0>(q);
                    tb_namespace::ScaleFactorPvt pvt = std::get<1>(q);
                    tb_namespace::ScaleFactorRiseFall rf = std::get<2>(q);
                    if (rf == tb_namespace::ScaleFactorRiseFall::kUnknown)
                        sf->addScale(type, pvt, __getDoubleValue(v));
                    else
                        sf->addScale(type, pvt, rf, __getDoubleValue(v));
                }
            }
        }
    }
}
void LibBuilder::__buildDefaultWireLoad(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __isStringType(v)) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            auto p = lib->getWireLoad(v->u.string_val);
            if (p == nullptr) p = lib->addWireLoad(v->u.string_val);
            if (p) lib->setDefaultWireLoad(p->getId());
        }
    }
}
void LibBuilder::__buildDefaultWireLoadArea(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            lib->setDefaultWireLoadArea(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildDefaultWireLoadMode(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __isStringType(v)) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            lib->setDefaultWireLoadMode(
                tb_namespace::toEnumByString<tb_namespace::WireLoadMode>(
                    v->u.string_val));
        }
    }
}
void LibBuilder::__buildDefaultWireLoadCapacitance(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            lib->setDefaultWireLoadCapacitance(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildDefaultWireLoadResistance(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            lib->setDefaultWireLoadResistance(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildDefaultWireLoadSelection(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __isStringType(v)) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            auto p = lib->getDefaultWireLoadSelection();
            if (p == nullptr) p = lib->addWireLoadSelection(v->u.string_val);
            if (p) lib->setDefaultWireLoadSelection(p->getId());
        }
    }
}
void LibBuilder::__buildDefaultInputPinCap(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            lib->setDefaultInputPinCap(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildDefaultOutputPinCap(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            lib->setDefaultOutputPinCap(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildDefaultInoutPinCap(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            lib->setDefaultInoutPinCap(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildDefaultMaxCapacitance(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            lib->setDefaultMaxCapacitance(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildDefaultMaxFanout(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            lib->setDefaultMaxFanout(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildDefaultMaxTransition(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            lib->setDefaultMaxTransition(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildDefaultFanoutLoad(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            lib->setDefaultFanoutLoad(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildDefaultCellLeakagePower(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            (__isIntType(v) || __isDoubleType(v))) {
            tb_namespace::TLib *lib = static_cast<tb_namespace::TLib *>(object);
            lib->setDefaultCellLeakagePower(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildVoltageMap(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTLib &&
            __isStringType(v)) {
            if (v->next && (__isIntType(v->next) || __isDoubleType(v->next))) {
                tb_namespace::TLib *lib =
                    static_cast<tb_namespace::TLib *>(object);
                lib->addSupplyVoltage(v->u.string_val,
                                      __getDoubleValue(v->next));
            }
        }
    }
}

void LibBuilder::__buildProcess(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeOperatingConditions &&
            (__isIntType(v) || __isDoubleType(v))) {
            static_cast<tb_namespace::OperatingConditions *>(object)
                ->setProcess(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildTemperature(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeOperatingConditions &&
            (__isIntType(v) || __isDoubleType(v))) {
            static_cast<tb_namespace::OperatingConditions *>(object)
                ->setTemperature(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildVoltage(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeOperatingConditions &&
            (__isIntType(v) || __isDoubleType(v))) {
            static_cast<tb_namespace::OperatingConditions *>(object)
                ->setVoltage(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildTreeType(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeOperatingConditions &&
            __isStringType(v)) {
            if (!strcmp(v->u.string_val, "best_case_tree"))
                static_cast<tb_namespace::OperatingConditions *>(object)
                    ->setWireLoadTree(tb_namespace::WireLoadTree::kBestCase);
            else if (!strcmp(v->u.string_val, "balanced_tree"))
                static_cast<tb_namespace::OperatingConditions *>(object)
                    ->setWireLoadTree(tb_namespace::WireLoadTree::kBalanced);
            else if (!strcmp(v->u.string_val, "worst_case_tree"))
                static_cast<tb_namespace::OperatingConditions *>(object)
                    ->setWireLoadTree(tb_namespace::WireLoadTree::kWorstCase);
        }
    }
}

void LibBuilder::__buildArea(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if ((__isIntType(v) || __isDoubleType(v))) {
            if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell) {
                if (static_cast<tb_namespace::TCell *>(object) != nullptr)
                    static_cast<tb_namespace::TCell *>(object)

                        ->setArea(__getDoubleValue(v));
            } else if (object->getObjectType() ==
                       tb_namespace::ObjectType::kObjectTypeWireLoad) {
                static_cast<tb_namespace::WireLoad *>(object)->setArea(
                    __getDoubleValue(v));
            }
        }
    }
}
void LibBuilder::__buildCellFootprint(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __isStringType(v)) {
            if (static_cast<tb_namespace::TCell *>(object) != nullptr)
                static_cast<tb_namespace::TCell *>(object)

                    ->setCellFootprint(v->u.string_val);
        }
    }
}
void LibBuilder::__buildDontTouch(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __isBoolType(v)) {
            if (static_cast<tb_namespace::TCell *>(object) != nullptr)
                static_cast<tb_namespace::TCell *>(object)

                    ->setDontTouch(v->u.bool_val);
        }
    }
}
void LibBuilder::__buildDontUse(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __isBoolType(v)) {
            if (static_cast<tb_namespace::TCell *>(object) != nullptr)
                static_cast<tb_namespace::TCell *>(object)

                    ->setDontUse(v->u.bool_val);
        }
    }
}
void LibBuilder::__buildAlwaysOn(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (__isBoolType(v)) {
            if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell) {
                auto attr = static_cast<tb_namespace::TCell *>(object);
                if (attr != nullptr) attr->setAlwaysOn(v->u.bool_val);
            } else if (object->getObjectType() ==
                       tb_namespace::ObjectType::kObjectTypeTTerm) {
                auto attr = static_cast<tb_namespace::TTerm *>(object);
                if (attr != nullptr) attr->setAlwaysOn(true);
            }
        }
    }
}
void LibBuilder::__buildIsMacroCell(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __isBoolType(v)) {
            if (static_cast<tb_namespace::TCell *>(object) != nullptr)
                static_cast<tb_namespace::TCell *>(object)

                    ->setMacroCell(v->u.bool_val);
        }
    }
}
void LibBuilder::__buildIsPad(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __isBoolType(v)) {
            if (v->u.bool_val) {
                if (static_cast<tb_namespace::TCell *>(object) != nullptr)
                    static_cast<tb_namespace::TCell *>(object)

                        ->setPad(true);
            }
        }
    }
}
void LibBuilder::__buildIsDecapCell(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __isBoolType(v)) {
            if (v->u.bool_val) {
                if (static_cast<tb_namespace::TCell *>(object) != nullptr)
                    static_cast<tb_namespace::TCell *>(object)

                        ->setDecapCell(true);
            }
        }
    }
}
void LibBuilder::__buildIsFillerCell(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __isBoolType(v)) {
            if (v->u.bool_val) {
                if (static_cast<tb_namespace::TCell *>(object) != nullptr)
                    static_cast<tb_namespace::TCell *>(object)

                        ->setFillerCell(true);
            }
        }
    }
}
void LibBuilder::__buildIsTapCell(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __isBoolType(v)) {
            if (v->u.bool_val) {
                if (static_cast<tb_namespace::TCell *>(object) != nullptr)
                    static_cast<tb_namespace::TCell *>(object)

                        ->setTapCell(true);
            }
        }
    }
}
void LibBuilder::__buildIsClockGatingCell(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __isBoolType(v)) {
            if (v->u.bool_val) {
                if (static_cast<tb_namespace::TCell *>(object) != nullptr)
                    static_cast<tb_namespace::TCell *>(object)

                        ->setClockGatingCell(true);
            }
        }
    }
}
void LibBuilder::__buildIsClockIsolationCell(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __isBoolType(v)) {
            if (v->u.bool_val) {
                if (static_cast<tb_namespace::TCell *>(object) != nullptr)
                    static_cast<tb_namespace::TCell *>(object)

                        ->setClockIsolationCell(true);
            }
        }
    }
}
void LibBuilder::__buildIsIsolationCell(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __isBoolType(v)) {
            if (v->u.bool_val) {
                if (static_cast<tb_namespace::TCell *>(object) != nullptr)
                    static_cast<tb_namespace::TCell *>(object)

                        ->setIsolationCell(true);
            }
        }
    }
}
void LibBuilder::__buildIsNoEnable(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __isBoolType(v)) {
            if (v->u.bool_val) {
                if (static_cast<tb_namespace::TCell *>(object) != nullptr)
                    static_cast<tb_namespace::TCell *>(object)

                        ->setEnableLevelShifter(true);
            }
        }
    }
}
void LibBuilder::__buildClockGatingIntegratedCell(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __isStringType(v)) {
            tb_namespace::TCell *cell =
                static_cast<tb_namespace::TCell *>(object);
            if (cell != nullptr) {
                cell->setIntegratedClockGatingCell(true);
                cell->setClockGatingIntegratedCell(
                    tb_namespace::toEnumByString<
                        tb_namespace::ClockGateIntegratedType>(
                        v->u.string_val));
            }
        }
    }
}
void LibBuilder::__buildAntennaDiodeType(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __isStringType(v)) {
            tb_namespace::TCell *cell =
                static_cast<tb_namespace::TCell *>(object);
            if (cell == nullptr) continue;

            if (!strcmp(v->u.string_val, "power"))
                cell->setAntennaDiodeType(
                    tb_namespace::AntennaDiodeType::kPower);
            else if (!strcmp(v->u.string_val, "ground"))
                cell->setAntennaDiodeType(
                    tb_namespace::AntennaDiodeType::kGround);
            else if (!strcmp(v->u.string_val, "power_and_ground"))
                cell->setAntennaDiodeType(
                    tb_namespace::AntennaDiodeType::kPowerAndGround);

            cell->setDiodeCell(true);
        }
    }
}
void LibBuilder::__buildIsLevelShifter(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __isBoolType(v)) {
            if (v->u.bool_val) {
                if (static_cast<tb_namespace::TCell *>(object) != nullptr)
                    static_cast<tb_namespace::TCell *>(object)

                        ->setLevelShifter(true);
            }
        }
    }
}
void LibBuilder::__buildCellLeakagePower(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            (__isIntType(v) || __isDoubleType(v))) {
            if (static_cast<tb_namespace::TCell *>(object) != nullptr)
                static_cast<tb_namespace::TCell *>(object)

                    ->setCellLeakagePower(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildSwitchCellType(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __isStringType(v)) {
            tb_namespace::TCell *cell =
                static_cast<tb_namespace::TCell *>(object);
            if (cell != nullptr) {
                cell->setSwitchCellType(
                    tb_namespace::toEnumByString<tb_namespace::SwitchCellType>(
                        v->u.string_val));
                cell->setPowerSwitch(true);
            }
        }
    }
}
void LibBuilder::__buildRetentionCell(buildParam) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell &&
            __isStringType(v)) {
            tb_namespace::TCell *cell =
                static_cast<tb_namespace::TCell *>(object);
            if (cell != nullptr) {
                cell->setRetentionCell(v->u.string_val);
                cell->setRetentionCell(true);
            }
        }
    }
}
void LibBuilder::__buildRelatedGroundPin(buildParam) {
    if (cur_bus_or_bundle_name_ != "" && __isStringType(v)) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                tb_namespace::ObjectId owner = term->getOwnerId();
                tb_namespace::TCell *cell = nullptr;
                if (owner != UNINIT_OBJECT_ID)
                    cell =
                        tb_namespace::Object::addr<tb_namespace::TCell>(owner);
                if (cell) {
                    auto p = cell->getOrCreatePgTerm(v->u.string_val);
                    if (p) term->setRelatedGroundPin(p->getId());
                }
            }
            return;
        }
    }
    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if (__isStringType(v)) {
        for (auto term : libpins) {
            if (term) {
                tb_namespace::ObjectId ownerId = term->getOwnerId();
                if (ownerId != UNINIT_OBJECT_ID) {
                    tb_namespace::TCell *cell =
                        tb_namespace::Object::addr<tb_namespace::TCell>(
                            ownerId);
                    if (cell) {
                        auto p = cell->getOrCreatePgTerm(v->u.string_val);
                        if (p) term->setRelatedGroundPin(p->getId());
                    }
                }
            }
        }
    }
}
void LibBuilder::__buildRelatedPowerPin(buildParam) {
    if (cur_bus_or_bundle_name_ != "" && __isStringType(v)) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                tb_namespace::ObjectId ownerId = term->getOwnerId();
                tb_namespace::TCell *cell = nullptr;
                if (ownerId != UNINIT_OBJECT_ID)
                    cell = tb_namespace::Object::addr<tb_namespace::TCell>(
                        ownerId);
                if (cell) {
                    auto p = cell->getOrCreatePgTerm(v->u.string_val);
                    if (p) term->setRelatedPowerPin(p->getId());
                }
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if (__isStringType(v)) {
        for (auto term : libpins) {
            if (term) {
                tb_namespace::ObjectId ownerId = term->getOwnerId();
                if (ownerId != UNINIT_OBJECT_ID) {
                    tb_namespace::TCell *cell =
                        tb_namespace::Object::addr<tb_namespace::TCell>(
                            ownerId);
                    if (cell) {
                        auto p = cell->getOrCreatePgTerm(v->u.string_val);
                        if (p) term->setRelatedPowerPin(p->getId());
                    }
                }
            }
        }
    }
}
void LibBuilder::__buildMaxTransition(buildParam) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__isIntType(v) || __isDoubleType(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term) term->setMaxTransition(__getDoubleValue(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if ((__isIntType(v) || __isDoubleType(v))) {
        for (auto term : libpins) {
            if (term) term->setMaxTransition(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildFallCapacitance(buildParam) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__isIntType(v) || __isDoubleType(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term) term->setFallCapacitance(__getDoubleValue(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if ((__isIntType(v) || __isDoubleType(v))) {
        for (auto term : libpins) {
            if (term) term->setFallCapacitance(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildRiseCapacitance(buildParam) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__isIntType(v) || __isDoubleType(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term) term->setRiseCapacitance(__getDoubleValue(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if ((__isIntType(v) || __isDoubleType(v))) {
        for (auto term : libpins) {
            if (term) term->setRiseCapacitance(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildThreeState(buildParam) {
    if (cur_bus_or_bundle_name_ != "" && __isStringType(v)) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term) term->setThreeState(true);
                tb_namespace::ObjectId owner = term->getOwnerId();
                if (owner != UNINIT_OBJECT_ID) {
                    auto cell =
                        tb_namespace::Object::addr<tb_namespace::TCell>(owner);
                    if (cell) cell->setThreeState(true);
                }
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if (__isStringType(v)) {
        for (auto term : libpins) {
            if (term) term->setThreeState(true);
            tb_namespace::ObjectId owner = term->getOwnerId();
            if (owner != UNINIT_OBJECT_ID) {
                auto cell =
                    tb_namespace::Object::addr<tb_namespace::TCell>(owner);
                if (cell) cell->setThreeState(true);
            }
        }
    }
}
void LibBuilder::__buildCapacitance(buildParam) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__isIntType(v) || __isDoubleType(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term) term->setCapacitance(__getDoubleValue(v));
            }
            return;
        }
    }
    GOBJECTS
    if ((__isIntType(v) || __isDoubleType(v))) {
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTTerm) {
                auto term = static_cast<tb_namespace::TTerm *>(obj);
                if (term) term->setCapacitance(__getDoubleValue(v));
            } else if (obj->getObjectType() ==
                       tb_namespace::ObjectType::kObjectTypeWireLoad) {
                auto wire_load = static_cast<tb_namespace::WireLoad *>(obj);
                wire_load->setCapacitance(__getDoubleValue(v));
            }
        }
    }
}
void LibBuilder::__buildMaxCapacitance(buildParam) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__isIntType(v) || __isDoubleType(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term) term->setMaxCapacitance(__getDoubleValue(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if ((__isIntType(v) || __isDoubleType(v))) {
        for (auto term : libpins) {
            if (term) term->setMaxCapacitance(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildMinCapacitance(buildParam) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__isIntType(v) || __isDoubleType(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term) term->setMinCapacitance(__getDoubleValue(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if ((__isIntType(v) || __isDoubleType(v))) {
        for (auto term : libpins) {
            if (term) term->setMinCapacitance(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildDirection(buildParam) {
    if (cur_bus_or_bundle_name_ != "" && __isStringType(v)) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term)
                    term->setDirection(
                        tb_namespace::toEnumByString<
                            tb_namespace::PinDirection>(v->u.string_val));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if (__isStringType(v)) {
        for (auto term : libpins) {
            if (term)
                term->setDirection(
                    tb_namespace::toEnumByString<tb_namespace::PinDirection>(
                        v->u.string_val));
        }
    }
}
void LibBuilder::__buildMaxFanout(buildParam) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__isIntType(v) || __isDoubleType(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term) term->setMaxFanout(__getDoubleValue(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if ((__isIntType(v) || __isDoubleType(v))) {
        for (auto term : libpins) {
            if (term) term->setMaxFanout(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildMinFanout(buildParam) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__isIntType(v) || __isDoubleType(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term) term->setMinFanout(__getDoubleValue(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if ((__isIntType(v) || __isDoubleType(v))) {
        for (auto term : libpins) {
            if (term) term->setMinFanout(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildMinTransition(buildParam) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__isIntType(v) || __isDoubleType(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term) term->setMinTransition(__getDoubleValue(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if ((__isIntType(v) || __isDoubleType(v))) {
        for (auto term : libpins) {
            if (term) term->setMinTransition(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildMinPeriod(buildParam) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__isIntType(v) || __isDoubleType(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term) term->setMinPeriod(__getDoubleValue(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if ((__isIntType(v) || __isDoubleType(v))) {
        for (auto term : libpins) {
            if (term) term->setMinPeriod(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildMinPulseWidthHigh(buildParam) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__isIntType(v) || __isDoubleType(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term) term->setMinPulseWidthHigh(__getDoubleValue(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if ((__isIntType(v) || __isDoubleType(v))) {
        for (auto term : libpins) {
            if (term) term->setMinPulseWidthHigh(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildMinPulseWidthLow(buildParam) {
    if (cur_bus_or_bundle_name_ != "" &&
        (__isIntType(v) || __isDoubleType(v))) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term) term->setMinPulseWidthLow(__getDoubleValue(v));
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if ((__isIntType(v) || __isDoubleType(v))) {
        for (auto term : libpins) {
            if (term) term->setMinPulseWidthLow(__getDoubleValue(v));
        }
    }
}
void LibBuilder::__buildClock(buildParam) {
    if (cur_bus_or_bundle_name_ != "" && __isBoolType(v)) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term) term->setClock(v->u.bool_val);
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if (__isBoolType(v)) {
        for (auto term : libpins) {
            if (term) term->setClock(v->u.bool_val);
        }
    }
}
void LibBuilder::__buildClockGateClockPin(buildParam) {
    if (cur_bus_or_bundle_name_ != "" && __isBoolType(v)) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term) term->setClockGateClockPin(v->u.bool_val);
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if (__isBoolType(v)) {
        for (auto term : libpins) {
            if (term) term->setClockGateClockPin(v->u.bool_val);
        }
    }
}
void LibBuilder::__buildClockGateEnablePin(buildParam) {
    if (cur_bus_or_bundle_name_ != "" && __isBoolType(v)) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                if (term) term->setClockGateEnablePin(v->u.bool_val);
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if (__isBoolType(v)) {
        for (auto term : libpins) {
            if (term) term->setClockGateEnablePin(v->u.bool_val);
        }
    }
}
void LibBuilder::__buildFunction(buildParam) {
    if (cur_bus_or_bundle_name_ != "" && __isStringType(v)) {
        auto findIt =
            bus_or_bundle_member_pins_map_.find(cur_bus_or_bundle_name_);
        if (findIt != bus_or_bundle_member_pins_map_.end()) {
            auto &vec = findIt->second;
            for (auto &term : vec) {
                term->setFunction(v->u.string_val);
            }
            return;
        }
    }

    std::vector<tb_namespace::TTerm *> libpins;
    __getTermsFromTopStack(&libpins);
    if (libpins.empty()) return;

    if (__isStringType(v)) {
        for (auto term : libpins) {
            term->setFunction(v->u.string_val);
        }
    }
}

void LibBuilder::__buildMembers(buildParam) {
    std::vector<std::string> terms;
    for (auto p = v; p; p = p->next) {
        if (__isStringType(p)) {
            terms.emplace_back(p->u.string_val);
        }
    }
    if (!terms.empty()) {
        std::vector<tb_namespace::TTerm *> member_pins;
        std::vector<tb_namespace::Object *> objects;
        __getObjectsFromTopStack(&objects);
        if (!objects.empty()) {
            tb_namespace::Object *object = objects[0];
            if (object->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTCell) {
                for (auto str : terms) {
                    tb_namespace::TTerm *term =
                        static_cast<tb_namespace::TCell *>(object)
                            ->getOrCreateTerm(str);
                    if (term) {
                        term->setBundle(true);
                        member_pins.emplace_back(term);
                    }
                }
            }
        }
        for (auto &p : member_pins) {
            for (auto &q : member_pins) {
                p->addMemberPin(q->getId());
            }
        }
        if (cur_bus_or_bundle_name_ != "")
            bus_or_bundle_member_pins_map_[cur_bus_or_bundle_name_] =
                member_pins;
    }
}
void LibBuilder::__buildBusType(buildParam) {
    if (v == nullptr || !__isStringType(v)) return;
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
    __getObjectsFromTopStack(&objects);
    if (!objects.empty()) {
        tb_namespace::Object *object = objects[0];
        if (object->getObjectType() ==
            tb_namespace::ObjectType::kObjectTypeTCell) {
            tb_namespace::TCell *cell =
                static_cast<tb_namespace::TCell *>(object);
            std::vector<tb_namespace::TTerm *> member_pins;
            for (int i = from; i <= to; ++i) {
                std::string str = bus_name + std::to_string(i);
                auto p = cell->getOrCreateTerm(str);
                if (p) {
                    p->setBus(true);
                    member_pins.emplace_back(p);
                }
            }
            for (auto &p : member_pins) {
                for (auto &q : member_pins) {
                    p->addMemberPin(q->getId());
                }
            }
            if (cur_bus_or_bundle_name_ != "")
                bus_or_bundle_member_pins_map_[cur_bus_or_bundle_name_] =
                    member_pins;
        }
    }
}

void LibBuilder::__buildPgType(buildParam) {
    std::vector<tb_namespace::TPgTerm *> libpgpins;
    __getPgTermsFromTopStack(&libpgpins);
    if (libpgpins.empty()) return;

    if (__isStringType(v)) {
        for (auto pgterm : libpgpins) {
            pgterm->setPgType(
                tb_namespace::toEnumByString<tb_namespace::PGType>(
                    v->u.string_val));
        }
    }
}
void LibBuilder::__buildVoltageName(buildParam) {
    std::vector<tb_namespace::TPgTerm *> libpgpins;
    __getPgTermsFromTopStack(&libpgpins);
    if (libpgpins.empty()) return;

    if (__isStringType(v)) {
        for (auto pgterm : libpgpins) {
            pgterm->setVoltageName(v->u.string_val);
        }
    }
}
void LibBuilder::__buildBitWidth(buildParam) {
    GOBJECTS;
    tb_namespace::Object *obj = objects[0];
    if (cur_type_group_name_ != "" && __isIntType(v)) {
        if (obj->getObjectType() == tb_namespace::ObjectType::kObjectTypeTCell)
            type_group_cell_map_[cur_type_group_name_].bit_width = v->u.int_val;
        else if (obj->getObjectType() ==
                 tb_namespace::ObjectType::kObjectTypeTLib)
            type_group_lib_map_[cur_type_group_name_].bit_width = v->u.int_val;
    }
}
void LibBuilder::__buildBitFrom(buildParam) {
    GOBJECTS;
    tb_namespace::Object *obj = objects[0];
    if (cur_type_group_name_ != "" && __isIntType(v)) {
        if (obj->getObjectType() == tb_namespace::ObjectType::kObjectTypeTCell)
            type_group_cell_map_[cur_type_group_name_].bit_from = v->u.int_val;
        else if (obj->getObjectType() ==
                 tb_namespace::ObjectType::kObjectTypeTLib)
            type_group_lib_map_[cur_type_group_name_].bit_from = v->u.int_val;
    }
}
void LibBuilder::__buildBitTo(buildParam) {
    GOBJECTS;
    tb_namespace::Object *obj = objects[0];
    if (cur_type_group_name_ != "" && __isIntType(v)) {
        if (obj->getObjectType() == tb_namespace::ObjectType::kObjectTypeTCell)
            type_group_cell_map_[cur_type_group_name_].bit_to = v->u.int_val;
        else if (obj->getObjectType() ==
                 tb_namespace::ObjectType::kObjectTypeTLib)
            type_group_lib_map_[cur_type_group_name_].bit_to = v->u.int_val;
    }
}
void LibBuilder::__buildDownto(buildParam) {
    GOBJECTS;
    tb_namespace::Object *obj = objects[0];
    if (cur_type_group_name_ != "" && __isBoolType(v)) {
        if (obj->getObjectType() == tb_namespace::ObjectType::kObjectTypeTCell)
            type_group_cell_map_[cur_type_group_name_].downto = v->u.bool_val;
        else if (obj->getObjectType() ==
                 tb_namespace::ObjectType::kObjectTypeTLib)
            type_group_lib_map_[cur_type_group_name_].downto = v->u.bool_val;
    }
}

void LibBuilder::__buildTimingSense(buildParam) {
    if (v && __isStringType(v)) {
        std::vector<tb_namespace::TimingArc *> timingarcs;
        __getTimingarcsFromTopStack(&timingarcs);
        for (auto &p : timingarcs) {
            p->setTimingSense(
                tb_namespace::toEnumByString<tb_namespace::TimingSense>(
                    v->u.string_val));
        }
    }
}
void LibBuilder::__buildTimingType(buildParam) {
    if (v && __isStringType(v)) {
        std::vector<tb_namespace::TimingArc *> timingarcs;
        __getTimingarcsFromTopStack(&timingarcs);
        for (auto &p : timingarcs) {
            p->setTimingType(
                tb_namespace::toEnumByString<tb_namespace::TimingType>(
                    v->u.string_val));
        }
    }
}
void LibBuilder::__buildWhen(buildParam) {
    if (v && __isStringType(v)) {
        std::vector<tb_namespace::TimingArc *> timingarcs;
        __getTimingarcsFromTopStack(&timingarcs);
        for (auto &p : timingarcs) {
            p->setWhen(v->u.string_val);
        }
    }
}
void LibBuilder::__buildRelatedPin(buildParam) {
    std::vector<std::string> strs;
    for (auto p = v; p; p = p->next) {
        if (__isStringType(p)) {
            __splitStringByDelim(p->u.string_val, " ", &strs);
        }
    }
    if (strs.empty()) return;

    std::vector<std::string> pins;
    for (auto &s : strs) {
        __getPinNamesFromBusRange(s.c_str(), &pins);
    }
    if (pins.empty()) return;

    std::vector<tb_namespace::TimingArc *> timingarcs;
    __getTimingarcsFromTopStack(&timingarcs);
    if (timingarcs.empty()) return;

    std::vector<tb_namespace::TTerm *> terms_owner;
    for (auto &p : timingarcs) {
        tb_namespace::ObjectId owner = p->getOwnerId();
        if (owner != UNINIT_OBJECT_ID) {
            auto term = tb_namespace::Object::addr<tb_namespace::TTerm>(owner);
            if (term) terms_owner.emplace_back(term);
        }
    }
    tb_namespace::TCell *cell = nullptr;
    tb_namespace::TTerm *term = terms_owner[0];
    if (term) {
        tb_namespace::ObjectId owner = term->getOwnerId();
        if (owner != UNINIT_OBJECT_ID)
            cell = tb_namespace::Object::addr<tb_namespace::TCell>(owner);
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
                    timingarcs[i]->addRelatedPin(vec[i]->getId());
                }
            } else {
                related_terms.insert(related_terms.end(), vec.begin(),
                                     vec.end());
            }
        } else {
            auto term = cell->getOrCreateTerm(s);
            if (term) related_terms.emplace_back(term);
        }
    }
    if (related_terms.empty()) return;

    for (auto &t : timingarcs) {
        for (auto &p : related_terms) {
            t->addRelatedPin(p->getId());
        }
    }
}

void LibBuilder::__buildResistance(buildParam) {
    if (v && (__isIntType(v) || __isDoubleType(v))) {
        GOBJECTS
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeWireLoad) {
                auto wire_load = static_cast<tb_namespace::WireLoad *>(obj);
                wire_load->setResistance(__getDoubleValue(v));
            }
        }
    }
}
void LibBuilder::__buildSlope(buildParam) {
    if (v && (__isIntType(v) || __isDoubleType(v))) {
        GOBJECTS
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeWireLoad) {
                auto wire_load = static_cast<tb_namespace::WireLoad *>(obj);
                wire_load->setSlope(__getDoubleValue(v));
            }
        }
    }
}
void LibBuilder::__buildFanoutLength(buildParam) {
    if (v == nullptr || v->next == nullptr) return;
    GOBJECTS;
    if (__isIntType(v) && (__isIntType(v->next) || __isDoubleType(v->next))) {
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeWireLoad) {
                auto wire_load = static_cast<tb_namespace::WireLoad *>(obj);
                wire_load->addFanoutLength(v->u.int_val,
                                           __getDoubleValue(v->next));
            } else if (obj->getObjectType() ==
                       tb_namespace::ObjectType::kObjectTypeWireLoadTable) {
                auto wire_load_table =
                    static_cast<tb_namespace::WireLoadTable *>(obj);
                wire_load_table->setFanoutLength(v->u.int_val,
                                                 __getDoubleValue(v->next));
            }
        }
    }
}

void LibBuilder::__buildFanoutCapacitance(buildParam) {
    if (v == nullptr || v->next == nullptr) return;
    GOBJECTS;
    if (__isIntType(v) && (__isIntType(v->next) || __isDoubleType(v->next))) {
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeWireLoadTable) {
                auto wire_load_table =
                    static_cast<tb_namespace::WireLoadTable *>(obj);
                wire_load_table->setFanoutCapacitance(
                    v->u.int_val, __getDoubleValue(v->next));
            }
        }
    }
}
void LibBuilder::__buildFanoutResistance(buildParam) {
    if (v == nullptr || v->next == nullptr) return;
    GOBJECTS;
    if (__isIntType(v) && (__isIntType(v->next) || __isDoubleType(v->next))) {
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeWireLoadTable) {
                auto wire_load_table =
                    static_cast<tb_namespace::WireLoadTable *>(obj);
                wire_load_table->setFanoutResistance(v->u.int_val,
                                                     __getDoubleValue(v->next));
            }
        }
    }
}
void LibBuilder::__buildFanoutArea(buildParam) {
    if (v == nullptr || v->next == nullptr) return;
    GOBJECTS;
    if (__isIntType(v) && (__isIntType(v->next) || __isDoubleType(v->next))) {
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeWireLoadTable) {
                auto wire_load_table =
                    static_cast<tb_namespace::WireLoadTable *>(obj);
                wire_load_table->setFanoutArea(v->u.int_val,
                                               __getDoubleValue(v->next));
            }
        }
    }
}

void LibBuilder::__buildWireLoadFromArea(buildParam) {
    if (v == nullptr || v->next == nullptr || v->next->next == nullptr) return;
    tb_namespace::Timing *timing_lib = tb_namespace::getTimingLib();
    if (timing_lib == nullptr) return;
    GOBJECTS
    auto p1 = v;
    auto p2 = v->next;
    auto p3 = v->next->next;
    if ((__isIntType(p1) || __isDoubleType(p1)) &&
        (__isIntType(p2) || __isDoubleType(p2)) && __isStringType(p3)) {
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeWireLoadSelection) {
                auto p = static_cast<tb_namespace::WireLoadSelection *>(obj);
                tb_namespace::ObjectId owner = p->getOwnerId();
                tb_namespace::TLib *lib = nullptr;
                if (owner != UNINIT_OBJECT_ID)
                    lib = tb_namespace::Object::addr<tb_namespace::TLib>(owner);
                if (lib == nullptr) continue;
                auto wire_load = lib->getWireLoad(p3->u.string_val);
                if (wire_load == nullptr) continue;
                auto wlr = tb_namespace::Object::createObject<
                    tb_namespace::WireLoadForArea>(
                    tb_namespace::ObjectType::kObjectTypeWireLoadForArea,
                    timing_lib->getId());
                if (wlr) {
                    wlr->setMinArea(__getDoubleValue(p1));
                    wlr->setMaxArea(__getDoubleValue(p2));
                    wlr->setWireload(wire_load->getId());
                    wlr->setOwner(p);
                    p->addWireLoadForArea(wlr->getId());
                }
            }
        }
    }
}

void LibBuilder::__buildVariable1(buildParam) {
    if (v && __isStringType(v)) {
        GOBJECTS
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTableTemplate) {
                auto tt = static_cast<tb_namespace::TableTemplate *>(obj);
                auto axis1 = tt->createAxis1();
                if (axis1) {
                    axis1->setVariable(
                        tb_namespace::toEnumByString<
                            tb_namespace::TableAxisVariable>(v->u.string_val));
                }
            }
        }
    }
}
void LibBuilder::__buildVariable2(buildParam) {
    if (v && __isStringType(v)) {
        GOBJECTS
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTableTemplate) {
                auto tt = static_cast<tb_namespace::TableTemplate *>(obj);
                auto axis2 = tt->createAxis2();
                if (axis2) {
                    axis2->setVariable(
                        tb_namespace::toEnumByString<
                            tb_namespace::TableAxisVariable>(v->u.string_val));
                }
            }
        }
    }
}
void LibBuilder::__buildVariable3(buildParam) {
    if (v && __isStringType(v)) {
        GOBJECTS
        for (auto &obj : objects) {
            if (obj->getObjectType() ==
                tb_namespace::ObjectType::kObjectTypeTableTemplate) {
                auto tt = static_cast<tb_namespace::TableTemplate *>(obj);
                auto axis3 = tt->createAxis3();
                if (axis3) {
                    axis3->setVariable(
                        tb_namespace::toEnumByString<
                            tb_namespace::TableAxisVariable>(v->u.string_val));
                }
            }
        }
    }
}
void LibBuilder::__buildIndex1(buildParam) {
    GOBJECTS
    for (auto &obj : objects) {
        if (obj->getObjectType() ==
            tb_namespace::ObjectType::kObjectTypeTableTemplate) {
            auto tt = static_cast<tb_namespace::TableTemplate *>(obj);
            auto axis = tt->getAxis1();
            if (axis == nullptr) continue;
            for (auto p = v; p; p = p->next) {
                if (__isIntType(p) || __isDoubleType(p)) {
                    axis->addValue(__getDoubleValue(p));
                } else if (__isStringType(p)) {
                    std::vector<double> values;
                    __getStringFloatList(p->u.string_val, &values);
                    for (auto f : values) axis->addValue(f);
                }
            }
        }
    }
}
void LibBuilder::__buildIndex2(buildParam) {
    GOBJECTS
    for (auto &obj : objects) {
        if (obj->getObjectType() ==
            tb_namespace::ObjectType::kObjectTypeTableTemplate) {
            auto tt = static_cast<tb_namespace::TableTemplate *>(obj);
            auto axis = tt->getAxis2();
            if (axis == nullptr) continue;
            for (auto p = v; p; p = p->next) {
                if ((__isIntType(p) || __isDoubleType(p))) {
                    axis->addValue(__getDoubleValue(p));
                } else if (__isStringType(p)) {
                    std::vector<double> values;
                    __getStringFloatList(p->u.string_val, &values);
                    for (auto f : values) axis->addValue(f);
                }
            }
        }
    }
}
void LibBuilder::__buildIndex3(buildParam) {
    GOBJECTS
    for (auto &obj : objects) {
        if (obj->getObjectType() ==
            tb_namespace::ObjectType::kObjectTypeTableTemplate) {
            auto tt = static_cast<tb_namespace::TableTemplate *>(obj);
            auto axis = tt->getAxis3();
            if (axis == nullptr) continue;
            for (auto p = v; p; p = p->next) {
                if ((__isIntType(p) || __isDoubleType(p))) {
                    axis->addValue(__getDoubleValue(p));
                } else if (__isStringType(p)) {
                    std::vector<double> values;
                    __getStringFloatList(p->u.string_val, &values);
                    for (auto f : values) axis->addValue(f);
                }
            }
        }
    }
}

void LibBuilder::__buildValues(buildParam) {
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
        if (__isIntType(p) || __isDoubleType(p)) {
            values.emplace_back(__getDoubleValue(p));
        } else if (__isStringType(p)) {
            __getStringFloatList(p->u.string_val, &values);
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
                auto p = tb_namespace::Object::createObject<
                    tb_namespace::TimingTable3>(
                    tb_namespace::ObjectType::kObjectTypeTimingTable3,
                    timing_lib->getId());
                if (p) {
                    axis_[0]->setOwner(p);
                    p->setAxis1(axis_[0]->getId());
                    axis_[1]->setOwner(p);
                    p->setAxis2(axis_[1]->getId());
                    axis_[2]->setOwner(p);
                    p->setAxis3(axis_[2]->getId());
                    for (auto &v : values) p->addValue(v);
                    tt = p;
                }
            } else if (axis_[0] && axis_[1]) {
                auto p = tb_namespace::Object::createObject<
                    tb_namespace::TimingTable2>(
                    tb_namespace::ObjectType::kObjectTypeTimingTable2,
                    timing_lib->getId());
                if (p) {
                    axis_[0]->setOwner(p);
                    p->setAxis1(axis_[0]->getId());
                    axis_[1]->setOwner(p);
                    p->setAxis2(axis_[1]->getId());
                    for (auto &v : values) p->addValue(v);
                    tt = p;
                }
            } else if (axis_[0]) {
                auto p = tb_namespace::Object::createObject<
                    tb_namespace::TimingTable1>(
                    tb_namespace::ObjectType::kObjectTypeTimingTable1,
                    timing_lib->getId());
                if (p) {
                    axis_[0]->setOwner(p);
                    p->setAxis1(axis_[0]->getId());
                    for (auto &v : values) p->addValue(v);
                    tt = p;
                }
            } else {
                auto p = tb_namespace::Object::createObject<
                    tb_namespace::TimingTable0>(
                    tb_namespace::ObjectType::kObjectTypeTimingTable0,
                    timing_lib->getId());
                if (p) p->setValue(values[0]);
                tt = p;
            }
            if (tt) {
                tt->setOwner(timingarc);
                if (groupname == "cell_rise")
                    timingarc->setCellRise(tt->getId());
                else if (groupname == "cell_fall")
                    timingarc->setCellFall(tt->getId());
                else if (groupname == "rise_transition")
                    timingarc->setRiseTransition(tt->getId());
                else if (groupname == "fall_transition")
                    timingarc->setFallTransition(tt->getId());
                else if (groupname == "rise_constraint")
                    timingarc->setRiseConstraint(tt->getId());
                else if (groupname == "fall_constraint")
                    timingarc->setFallConstraint(tt->getId());
            }
        }
    }
}

void LibBuilder::__getTermsFromTopStack(
    std::vector<tb_namespace::TTerm *> *libterms) {
    GOBJECTS
    for (auto object : objects) {
        if (object->getObjectType() ==
            tb_namespace::ObjectType::kObjectTypeTTerm) {
            libterms->emplace_back(static_cast<tb_namespace::TTerm *>(object));
        }
    }
}
void LibBuilder::__getPgTermsFromTopStack(
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
void LibBuilder::__getTimingarcsFromTopStack(
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
void LibBuilder::__getObjectsFromTopStack(
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

void LibBuilder::__deleteObjectList(ObjectList *objects) {
    if (objects->next == nullptr)
        delete objects;
    else
        __deleteObjectList(objects->next);
}

void LibBuilder::__getPinNamesFromBusRange(const char *str,
                                           std::vector<std::string> *pinNames) {
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
tb_namespace::TCell *LibBuilder::__getTcellByTimingarc(
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

bool LibBuilder::__isIntType(timinglib_attribute_value *v) {
    if (v->type == timinglib_attribute_value_type::kTIMINGLIB__VAL_INT)
        return true;
    return false;
}
bool LibBuilder::__isDoubleType(timinglib_attribute_value *v) {
    if (v->type == timinglib_attribute_value_type::kTIMINGLIB__VAL_DOUBLE)
        return true;
    return false;
}
bool LibBuilder::__isStringType(timinglib_attribute_value *v) {
    if (v->type == timinglib_attribute_value_type::kTIMINGLIB__VAL_STRING)
        return true;
    return false;
}
bool LibBuilder::__isBoolType(timinglib_attribute_value *v) {
    if (v->type == timinglib_attribute_value_type::kTIMINGLIB__VAL_BOOLEAN)
        return true;
    return false;
}
float LibBuilder::__getDoubleValue(timinglib_attribute_value *v) {
    if (__isDoubleType(v))
        return v->u.double_val;
    else if (__isIntType(v))
        return v->u.int_val;
    else
        return 0.0f;
}
void LibBuilder::__getStringFloatList(char *str, std::vector<double> *values) {
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
void LibBuilder::__splitStringByDelim(char *str, const char *delim,
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
