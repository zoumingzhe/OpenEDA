/**
 * @file timinglib_lib.cpp
 * @date 2020-08-26
 * @brief TLib Class
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/timing/timinglib/timinglib_lib.h"

#include <map>
#include <string>

#include "db/core/db.h"
#include "db/timing/timinglib/timinglib_cell.h"
#include "db/timing/timinglib/timinglib_opcond.h"
#include "db/timing/timinglib/timinglib_scalefactors.h"
#include "db/timing/timinglib/timinglib_tabletemplate.h"
#include "db/timing/timinglib/timinglib_timingtable.h"
#include "db/timing/timinglib/timinglib_units.h"
#include "db/timing/timinglib/timinglib_wireload.h"

namespace open_edi {
namespace db {

TLib::TLib()
    : TLib::BaseType(),
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
      supply_voltages_(UNINIT_OBJECT_ID),
      default_operating_conditions_(UNINIT_OBJECT_ID),
      scaling_factors_(UNINIT_OBJECT_ID),
      units_(UNINIT_OBJECT_ID),
      default_wire_load_(UNINIT_OBJECT_ID),
      default_wire_load_selection_(UNINIT_OBJECT_ID),
      operating_conditions_(UNINIT_OBJECT_ID),
      wire_loads_(UNINIT_OBJECT_ID),
      wire_load_tables_(UNINIT_OBJECT_ID),
      wire_load_selections_(UNINIT_OBJECT_ID),
      table_templates_(UNINIT_OBJECT_ID),
      timing_cells_(UNINIT_OBJECT_ID),
      supply_voltage_map_(),
      operating_conditions_map_(),
      wire_loads_map_(),
      wire_load_tables_map_(),
      wire_load_selections_map_(),
      table_templates_map_(),
      timing_cells_map_() {
    setObjectType(ObjectType::kObjectTypeTLib);
}

TLib::~TLib() {
    supply_voltages_ = UNINIT_OBJECT_ID;
    default_operating_conditions_ = UNINIT_OBJECT_ID;
    scaling_factors_ = UNINIT_OBJECT_ID;
    units_ = UNINIT_OBJECT_ID;
    default_wire_load_ = UNINIT_OBJECT_ID;
    default_wire_load_selection_ = UNINIT_OBJECT_ID;
    operating_conditions_ = UNINIT_OBJECT_ID;
    wire_loads_ = UNINIT_OBJECT_ID;
    wire_load_tables_ = UNINIT_OBJECT_ID;
    wire_load_selections_ = UNINIT_OBJECT_ID;
    table_templates_ = UNINIT_OBJECT_ID;
    timing_cells_ = UNINIT_OBJECT_ID;
    supply_voltage_map_.clear();
    operating_conditions_map_.clear();
    wire_loads_map_.clear();
    wire_load_tables_map_.clear();
    wire_load_selections_map_.clear();
    table_templates_map_.clear();
    timing_cells_map_.clear();
}

TLib::TLib(Object *owner, TLib::IndexType id)
    : TLib::BaseType(owner, id),
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
      supply_voltages_(UNINIT_OBJECT_ID),
      default_operating_conditions_(UNINIT_OBJECT_ID),
      scaling_factors_(UNINIT_OBJECT_ID),
      units_(UNINIT_OBJECT_ID),
      default_wire_load_(UNINIT_OBJECT_ID),
      default_wire_load_selection_(UNINIT_OBJECT_ID),
      wire_loads_(UNINIT_OBJECT_ID),
      wire_load_tables_(UNINIT_OBJECT_ID),
      wire_load_selections_(UNINIT_OBJECT_ID),
      table_templates_(UNINIT_OBJECT_ID),
      timing_cells_(UNINIT_OBJECT_ID),
      supply_voltage_map_(),
      operating_conditions_map_(),
      wire_loads_map_(),
      wire_load_tables_map_(),
      wire_load_selections_map_(),
      table_templates_map_(),
      timing_cells_map_() {
    setObjectType(ObjectType::kObjectTypeTLib);
}

TLib::TLib(TLib const &rhs) { copy(rhs); }

TLib::TLib(TLib &&rhs) noexcept { move(std::move(rhs)); }

TLib &TLib::operator=(TLib const &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TLib &TLib::operator=(TLib &&rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void TLib::setName(const std::string &name) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(name.c_str());
        if (index != kInvalidSymbolIndex) {
            name_ = index;
            timing_lib->addSymbolReference(name_, this->getId());
        }
    }
}
void TLib::setTimingModelType(TimingModel tm) { timing_model_type_ = tm; }
void TLib::setNominalVoltage(float f) { nominal_voltage_ = f; }
void TLib::setNominalProcess(float f) { nominal_process_ = f; }
void TLib::setNominalTemperature(float f) { nominal_temperature_ = f; }
void TLib::setInputThresholdPctFall(float f) { input_threshold_pct_fall_ = f; }
void TLib::setInputThresholdPctRise(float f) { input_threshold_pct_rise_ = f; }
void TLib::setOutputThresholdPctFall(float f) {
    output_threshold_pct_fall_ = f;
}
void TLib::setOutputThresholdPctRise(float f) {
    output_threshold_pct_rise_ = f;
}
void TLib::setSlewLowerThresholdPctFall(float f) {
    slew_lower_threshold_pct_fall_ = f;
}
void TLib::setSlewLowerThresholdPctRise(float f) {
    slew_lower_threshold_pct_rise_ = f;
}
void TLib::setSlewUpperThresholdPctFall(float f) {
    slew_upper_threshold_pct_fall_ = f;
}
void TLib::setSlewUpperThresholdPctRise(float f) {
    slew_upper_threshold_pct_rise_ = f;
}
void TLib::setSlewDerateFromLibrary(float f) { slew_derate_from_library_ = f; }
void TLib::setDefaultWireLoadArea(float f) { default_wire_load_area_ = f; }
void TLib::setDefaultWireLoadMode(WireLoadMode w) {
    default_wire_load_mode_ = w;
}
void TLib::setDefaultWireLoadCapacitance(float f) {
    default_wire_load_capacitance_ = f;
}
void TLib::setDefaultWireLoadResistance(float f) {
    default_wire_load_resistance_ = f;
}
void TLib::setDefaultInputPinCap(float f) { default_input_pin_cap_ = f; }
void TLib::setDefaultOutputPinCap(float f) { default_output_pin_cap_ = f; }
void TLib::setDefaultInoutPinCap(float f) { default_inout_pin_cap_ = f; }
void TLib::setDefaultMaxCapacitance(float f) { default_max_capacitance_ = f; }
void TLib::setDefaultMaxFanout(float f) { default_max_fanout_ = f; }
void TLib::setDefaultMaxTransition(float f) { default_max_transition_ = f; }
void TLib::setDefaultFanoutLoad(float f) { default_fanout_load_ = f; }
void TLib::setDefaultCellLeakagePower(float f) {
    default_cell_leakage_power_ = f;
}
void TLib::addSupplyVoltage(const std::string &name, float f) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(name.c_str());
        if (index != kInvalidSymbolIndex) {
            name_ = index;
            supply_voltage_map_[name_] = f;
            timing_lib->addSymbolReference(name_, this->getId());

            ArrayObject<SupplyVoltagePair> *p = nullptr;
            if (supply_voltages_ == UNINIT_OBJECT_ID) {
                p = Object::createObject<ArrayObject<SupplyVoltagePair>>(
                    kObjectTypeArray, timing_lib->getId());
                if (p != nullptr) {
                    p->setPool(timing_lib->getPool());
                    p->reserve(32);
                    supply_voltages_ = p->getId();
                }
            } else {
                p = addr<ArrayObject<SupplyVoltagePair>>(supply_voltages_);
            }
            if (p != nullptr) {
                SupplyVoltagePair sp;
                sp.name = index;
                sp.value = f;
                p->pushBack(sp);
            }
        }
    }
}

TLib::IndexType TLib::numTCells() const {
    if (timing_cells_ == UNINIT_OBJECT_ID) return 0;
    auto p = addr<ArrayObject<ObjectId>>(timing_cells_);
    if (p != nullptr)
        return p->getSize();
    else
        return 0;
}

TLib::IndexType TLib::numOperatingConditions() const {
    return operating_conditions_map_.size();
}

TCell *TLib::addTimingCell(const std::string &name) {
    auto cell = __addTCellImpl();
    cell->setName(name);

    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(name.c_str());
        if (index != kInvalidSymbolIndex) {
            timing_cells_map_[index] = cell->getId();
            timing_lib->addSymbolReference(index, cell->getId());
        }
    }

    return cell;
}

OperatingConditions *TLib::addOperatingConditions(const std::string &name) {
    auto oc = __addOperatingConditionsImpl();
    oc->setName(name);

    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(name.c_str());
        if (index != kInvalidSymbolIndex) {
            operating_conditions_map_[index] = oc->getId();
            timing_lib->addSymbolReference(index, oc->getId());
        }
    }

    return oc;
}

WireLoad *TLib::addWireLoad(const std::string &name) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        ArrayObject<ObjectId> *object_array = nullptr;
        if (wire_loads_ == UNINIT_OBJECT_ID) {
            object_array = timing_lib->createObject<ArrayObject<ObjectId>>(
                kObjectTypeArray, timing_lib->getId());
            if (object_array != nullptr) {
                wire_loads_ = object_array->getId();
                object_array->setPool(timing_lib->getPool());
                object_array->reserve(32);
            }
        } else {
            object_array = addr<ArrayObject<ObjectId>>(wire_loads_);
        }
        if (object_array != nullptr) {
            auto p = timing_lib->createObject<WireLoad>(kObjectTypeWireLoad,
                                                        timing_lib->getId());
            if (p) {
                p->setOwner(this);
                p->setName(name);
                ObjectId id = p->getId();
                object_array->pushBack(id);
                wire_loads_map_[p->getNameIndex()] = id;
                return p;
            }
        }
    }
    return nullptr;
}
WireLoadTable *TLib::addWireLoadTable(const std::string &name) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        ArrayObject<ObjectId> *object_array = nullptr;
        if (wire_load_tables_ == UNINIT_OBJECT_ID) {
            object_array = timing_lib->createObject<ArrayObject<ObjectId>>(
                kObjectTypeArray, timing_lib->getId());
            if (object_array != nullptr) {
                wire_load_tables_ = object_array->getId();
                object_array->setPool(timing_lib->getPool());
                object_array->reserve(32);
            }
        } else {
            object_array = addr<ArrayObject<ObjectId>>(wire_load_tables_);
        }
        if (object_array != nullptr) {
            auto p = timing_lib->createObject<WireLoadTable>(
                kObjectTypeWireLoadTable, timing_lib->getId());
            if (p) {
                p->setOwner(this);
                p->setName(name);
                ObjectId id = p->getId();
                object_array->pushBack(id);
                wire_load_tables_map_[p->getNameIndex()] = id;
                return p;
            }
        }
    }
    return nullptr;
}
WireLoadSelection *TLib::addWireLoadSelection(const std::string &name) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        ArrayObject<ObjectId> *object_array = nullptr;
        if (wire_load_selections_ == UNINIT_OBJECT_ID) {
            object_array = timing_lib->createObject<ArrayObject<ObjectId>>(
                kObjectTypeArray, timing_lib->getId());
            if (object_array != nullptr) {
                wire_load_selections_ = object_array->getId();
                object_array->setPool(timing_lib->getPool());
                object_array->reserve(32);
            }
        } else {
            object_array = addr<ArrayObject<ObjectId>>(wire_load_selections_);
        }
        if (object_array != nullptr) {
            auto p = timing_lib->createObject<WireLoadSelection>(
                kObjectTypeWireLoadSelection, timing_lib->getId());
            if (p) {
                p->setOwner(this);
                p->setName(name);
                ObjectId id = p->getId();
                object_array->pushBack(id);
                wire_load_selections_map_[p->getNameIndex()] = id;
                return p;
            }
        }
    }
    return nullptr;
}
TableTemplate *TLib::addTableTemplate(const std::string &name) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        ArrayObject<ObjectId> *object_array = nullptr;
        if (table_templates_ == UNINIT_OBJECT_ID) {
            object_array = timing_lib->createObject<ArrayObject<ObjectId>>(
                kObjectTypeArray, timing_lib->getId());
            if (object_array != nullptr) {
                table_templates_ = object_array->getId();
                object_array->setPool(timing_lib->getPool());
                object_array->reserve(32);
            }
        } else {
            object_array = addr<ArrayObject<ObjectId>>(table_templates_);
        }
        if (object_array != nullptr) {
            auto p = timing_lib->createObject<TableTemplate>(
                kObjectTypeTableTemplate, timing_lib->getId());
            if (p) {
                p->setOwner(this);
                p->setName(name);
                ObjectId id = p->getId();
                object_array->pushBack(id);
                table_templates_map_[p->getNameIndex()] = id;
                return p;
            }
        }
    }
    return nullptr;
}

void TLib::setDefaultOperatingConditions(ObjectId id) {
    default_operating_conditions_ = id;
}
ScaleFactors *TLib::getOrCreateScalingFactors(const std::string &name) {
    if (scaling_factors_ != UNINIT_OBJECT_ID) {
        return addr<ScaleFactors>(scaling_factors_);
    } else {
        Timing *timing_lib = getTimingLib();
        auto p = timing_lib->createObject<ScaleFactors>(kObjectTypeScaleFactors,
                                                        timing_lib->getId());
        if (p) {
            p->setOwner(this);
            p->setName(name);
            scaling_factors_ = p->getId();
            return p;
        }
    }
    return nullptr;
}
TUnits *TLib::getOrCreateUnits() {
    if (units_ != UNINIT_OBJECT_ID) {
        return addr<TUnits>(units_);
    } else {
        Timing *timing_lib = getTimingLib();
        auto p = timing_lib->createObject<TUnits>(kObjectTypeTUnits,
                                                  timing_lib->getId());
        if (p) {
            p->setOwner(this);
            units_ = p->getId();
            return p;
        }
    }
    return nullptr;
}
void TLib::setDefaultWireLoad(ObjectId id) { default_wire_load_ = id; }
void TLib::setDefaultWireLoadSelection(ObjectId id) {
    default_wire_load_selection_ = id;
}

std::string TLib::getName(void) const {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(name_);
    }
    return "";
}
TimingModel TLib::getTimingModelType(void) { return timing_model_type_; }
float TLib::getNominalVoltage(void) { return nominal_voltage_; }
float TLib::getNominalProcess(void) { return nominal_process_; }
float TLib::getNominalTemperature(void) { return nominal_temperature_; }
float TLib::getInputThresholdPctFall(void) { return input_threshold_pct_fall_; }
float TLib::getInputThresholdPctRise(void) { return input_threshold_pct_rise_; }
float TLib::getOutputThresholdPctFall(void) {
    return output_threshold_pct_fall_;
}
float TLib::getOutputThresholdPctRise(void) {
    return output_threshold_pct_rise_;
}
float TLib::getSlewLowerThresholdPctFall(void) {
    return slew_lower_threshold_pct_fall_;
}
float TLib::getSlewLowerThresholdPctRise(void) {
    return slew_lower_threshold_pct_rise_;
}
float TLib::getSlewUpperThresholdPctFall(void) {
    return slew_upper_threshold_pct_fall_;
}
float TLib::getSlewUpperThresholdPctRise(void) {
    return slew_upper_threshold_pct_rise_;
}
float TLib::getSlewDerateFromLibrary(void) { return slew_derate_from_library_; }
float TLib::getDefaultWireLoadArea(void) { return default_wire_load_area_; }
WireLoadMode TLib::getDefaultWireLoadMode(void) {
    return default_wire_load_mode_;
}
float TLib::getDefaultWireLoadCapacitance(void) {
    return default_wire_load_capacitance_;
}
float TLib::getDefaultWireLoadResistance(void) {
    return default_wire_load_resistance_;
}
float TLib::getDefaultInputPinCap(void) { return default_input_pin_cap_; }
float TLib::getDefaultOutputPinCap(void) { return default_output_pin_cap_; }
float TLib::getDefaultInoutPinCap(void) { return default_inout_pin_cap_; }
float TLib::getDefaultMaxCapacitance(void) { return default_max_capacitance_; }
float TLib::getDefaultMaxFanout(void) { return default_max_fanout_; }
float TLib::getDefaultMaxTransition(void) { return default_max_transition_; }
float TLib::getDefaultFanoutLoad(void) { return default_fanout_load_; }
float TLib::getDefaultCellLeakagePower(void) {
    return default_cell_leakage_power_;
}
float TLib::getSupplyVoltage(const std::string &name) {
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

OperatingConditions *TLib::getDefaultOperatingConditions(void) const {
    if (default_operating_conditions_ != UNINIT_OBJECT_ID)
        return addr<OperatingConditions>(default_operating_conditions_);
    else
        return nullptr;
}
ScaleFactors *TLib::getScalingFactors(void) {
    if (scaling_factors_ != UNINIT_OBJECT_ID)
        return addr<ScaleFactors>(scaling_factors_);
    else
        return nullptr;
}
TUnits *TLib::getUnits(void) {
    if (units_ != UNINIT_OBJECT_ID)
        return addr<TUnits>(units_);
    else
        return nullptr;
}
WireLoad *TLib::getDefaultWireLoad(void) const {
    if (default_wire_load_)
        return addr<WireLoad>(default_wire_load_);
    else
        return nullptr;
}
WireLoadSelection *TLib::getDefaultWireLoadSelection(void) const {
    if (default_wire_load_selection_ != UNINIT_OBJECT_ID)
        return addr<WireLoadSelection>(default_wire_load_selection_);
    else
        return nullptr;
}
OperatingConditions *TLib::getOperatingConditions(
    const std::string &name) const {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex idx = timing_lib->getOrCreateSymbol(name.c_str());
        if (idx != kInvalidSymbolIndex) {
            auto it = operating_conditions_map_.find(idx);
            if (it != operating_conditions_map_.end())
                return addr<OperatingConditions>(it->second);
        }
    }
    return nullptr;
}
WireLoad *TLib::getWireLoad(const std::string &name) const {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex idx = timing_lib->getOrCreateSymbol(name.c_str());
        if (idx != kInvalidSymbolIndex) {
            auto it = wire_loads_map_.find(idx);
            if (it != wire_loads_map_.end()) return addr<WireLoad>(it->second);
        }
    }
    return nullptr;
}
WireLoadTable *TLib::getWireLoadTable(const std::string &name) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex idx = timing_lib->getOrCreateSymbol(name.c_str());
        if (idx != kInvalidSymbolIndex) {
            auto it = wire_load_tables_map_.find(idx);
            if (it != wire_load_tables_map_.end())
                return addr<WireLoadTable>(it->second);
        }
    }
    return nullptr;
}
WireLoadSelection *TLib::getWireLoadSelection(const std::string &name) const {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex idx = timing_lib->getOrCreateSymbol(name.c_str());
        if (idx != kInvalidSymbolIndex) {
            auto it = wire_load_selections_map_.find(idx);
            if (it != wire_load_selections_map_.end())
                return addr<WireLoadSelection>(it->second);
        }
    }
    return nullptr;
}
TableTemplate *TLib::getTableTemplate(const std::string &name) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex idx = timing_lib->getOrCreateSymbol(name.c_str());
        if (idx != kInvalidSymbolIndex) {
            auto it = table_templates_map_.find(idx);
            if (it != table_templates_map_.end())
                return addr<TableTemplate>(it->second);
        }
    }
    return nullptr;
}
TCell *TLib::getTimingCell(const std::string &name) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex idx = timing_lib->getOrCreateSymbol(name.c_str());
        if (idx != kInvalidSymbolIndex) {
            auto it = timing_cells_map_.find(idx);
            if (it != timing_cells_map_.end()) return addr<TCell>(it->second);
        }
    }
    return nullptr;
}
std::vector<TCell *> TLib::getTimingCells(void) {
    std::vector<TCell *> cells;
    for (auto p : timing_cells_map_) {
        auto cell = addr<TCell>(p.second);
        if (cell != nullptr) cells.emplace_back(cell);
    }
    return cells;
}
TLib::IndexType TLib::memory() const {
    IndexType ret = this->BaseType::memory();

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
    ret += sizeof(supply_voltages_);
    ret += sizeof(default_operating_conditions_);
    ret += sizeof(scaling_factors_);
    ret += sizeof(units_);
    ret += sizeof(default_wire_load_);
    ret += sizeof(default_wire_load_selection_);
    ret += sizeof(operating_conditions_);
    ret += sizeof(wire_loads_);
    ret += sizeof(wire_load_tables_);
    ret += sizeof(wire_load_selections_);
    ret += sizeof(table_templates_);
    ret += sizeof(timing_cells_);

    // TODO(liwei)

    return ret;
}

void TLib::print(std::ostream &stream) {
    stream << "TLib name: " << getName() << " id: " << getId() << std::endl;

    if (timing_cells_ != UNINIT_OBJECT_ID) {
        auto p = addr<ArrayObject<ObjectId>>(timing_cells_);
        if (p != nullptr) {
            for (int64_t i = 0; i < p->getSize(); ++i) {
                auto c = addr<TCell>((*p)[i]);
                if (c) c->print(stream);
            }
        }
    }
    for (auto oc : operating_conditions_map_) {
        auto c = addr<OperatingConditions>(oc.second);
        if (c) c->print(stream);
    }
}

void TLib::copy(TLib const &rhs) {
    this->BaseType::copy(rhs);

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
    supply_voltages_ = rhs.supply_voltages_;
    default_operating_conditions_ = rhs.default_operating_conditions_;
    scaling_factors_ = rhs.scaling_factors_;
    units_ = rhs.units_;
    default_wire_load_ = rhs.default_wire_load_;
    default_wire_load_selection_ = rhs.default_wire_load_selection_;
    operating_conditions_ = rhs.operating_conditions_;
    wire_loads_ = rhs.wire_loads_;
    wire_load_tables_ = rhs.wire_load_tables_;
    wire_load_selections_ = rhs.wire_load_selections_;
    table_templates_ = rhs.table_templates_;
    timing_cells_ = rhs.timing_cells_;
    supply_voltage_map_ = rhs.supply_voltage_map_;
    operating_conditions_map_ = rhs.operating_conditions_map_;
    wire_loads_map_ = rhs.wire_loads_map_;
    wire_load_tables_map_ = rhs.wire_load_tables_map_;
    wire_load_selections_map_ = rhs.wire_load_selections_map_;
    table_templates_map_ = rhs.table_templates_map_;
    timing_cells_map_ = rhs.timing_cells_map_;
}

void TLib::move(TLib &&rhs) {
    this->BaseType::move(std::move(rhs));

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
    supply_voltages_ = std::move(rhs.supply_voltages_);
    default_operating_conditions_ =
        std::move(rhs.default_operating_conditions_);
    scaling_factors_ = std::move(rhs.scaling_factors_);
    units_ = std::move(rhs.units_);
    default_wire_load_ = std::move(rhs.default_wire_load_);
    default_wire_load_selection_ = std::move(rhs.default_wire_load_selection_);
    operating_conditions_ = std::move(rhs.operating_conditions_);
    wire_loads_ = std::move(rhs.wire_loads_);
    wire_load_tables_ = std::move(rhs.wire_load_tables_);
    wire_load_selections_ = std::move(rhs.wire_load_selections_);
    table_templates_ = std::move(rhs.table_templates_);
    timing_cells_ = std::move(rhs.timing_cells_);
    supply_voltage_map_ = std::move(rhs.supply_voltage_map_);
    operating_conditions_map_ = std::move(rhs.operating_conditions_map_);
    wire_loads_map_ = std::move(rhs.wire_loads_map_);
    wire_load_tables_map_ = std::move(rhs.wire_load_tables_map_);
    wire_load_selections_map_ = std::move(rhs.wire_load_selections_map_);
    table_templates_map_ = std::move(rhs.table_templates_map_);
    timing_cells_map_ = std::move(rhs.timing_cells_map_);

    rhs.supply_voltages_ = UNINIT_OBJECT_ID;
    rhs.default_operating_conditions_ = UNINIT_OBJECT_ID;
    rhs.scaling_factors_ = UNINIT_OBJECT_ID;
    rhs.units_ = UNINIT_OBJECT_ID;
    rhs.default_wire_load_ = UNINIT_OBJECT_ID;
    rhs.default_wire_load_selection_ = UNINIT_OBJECT_ID;
    operating_conditions_ = UNINIT_OBJECT_ID;
    wire_loads_ = UNINIT_OBJECT_ID;
    wire_load_tables_ = UNINIT_OBJECT_ID;
    wire_load_selections_ = UNINIT_OBJECT_ID;
    table_templates_ = UNINIT_OBJECT_ID;
    timing_cells_ = UNINIT_OBJECT_ID;
    rhs.timing_cells_ = UNINIT_OBJECT_ID;
    rhs.operating_conditions_map_.clear();
    rhs.wire_loads_map_.clear();
    rhs.wire_load_tables_map_.clear();
    rhs.wire_load_selections_map_.clear();
    rhs.table_templates_map_.clear();
    rhs.timing_cells_map_.clear();
}

template <typename T>
void output_map(const std::unordered_map<SymbolIndex, ObjectId> &origmap,
                OStreamBase *os) {
    Timing *timing_lib = getTimingLib();
    std::map<std::string, T *> sorted_map;
    for (auto &v : origmap) {
        std::string str = timing_lib->getSymbolByIndex(v.first);
        auto p = Object::addr<T>(v.second);
        if (str != "" && p != nullptr) sorted_map[str] = p;
    }
    *os << sorted_map.size();
    *os << DataBegin("[");
    auto delimiter = DataDelimiter("");
    for (auto const &v : sorted_map) {
        *os << delimiter << *(v.second);
        delimiter = DataDelimiter();
    }
    *os << DataEnd("]");
}
template <typename T>
void output_vec(const ObjectId &origvec, OStreamBase *os) {
    ArrayObject<ObjectId> *p = nullptr;
    if (origvec != UNINIT_OBJECT_ID)
        p = Object::addr<ArrayObject<ObjectId>>(origvec);
    if (p == nullptr)
        *os << 0;
    else
        *os << p->getSize();
    *os << DataBegin("[");
    if (p != nullptr) {
        auto delimiter = DataDelimiter("");
        for (int64_t i = 0; i < p->getSize(); ++i) {
            auto q = Object::addr<T>((*p)[i]);
            if (q) {
                *os << delimiter << *q;
                delimiter = DataDelimiter();
            }
        }
    }
    *os << DataEnd("]");
}
OStreamBase &operator<<(OStreamBase &os, TLib const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    TLib::BaseType const &base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("name_") << rhs.getName() << DataDelimiter();
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

    os << DataFieldName("default_operating_conditions_");
    {
        auto p = rhs.getDefaultOperatingConditions();
        if (p != nullptr)
            os << p->getName();
        else
            os << "";
        os << DataDelimiter();
    }
    os << DataFieldName("scaling_factors_") << rhs.scaling_factors_;
    {
        os << DataBegin("[");
        if (rhs.scaling_factors_ != UNINIT_OBJECT_ID) {
            auto p = Object::addr<ScaleFactors>(rhs.scaling_factors_);
            if (p) os << *p;
        }
        os << DataEnd("]") << DataDelimiter();
    }
    os << DataFieldName("units_") << rhs.units_;
    {
        os << DataBegin("[");
        if (rhs.units_ != UNINIT_OBJECT_ID) {
            auto p = Object::addr<TUnits>(rhs.units_);
            if (p) os << *p;
        }
        os << DataEnd("]") << DataDelimiter();
    }
    os << DataFieldName("default_wire_load_");
    {
        auto p = rhs.getDefaultWireLoad();
        if (p != nullptr)
            os << p->getName();
        else
            os << "";
        os << DataDelimiter();
    }
    os << DataFieldName("default_wire_load_selection_");
    {
        auto p = rhs.getDefaultWireLoadSelection();
        if (p != nullptr)
            os << p->getName();
        else
            os << "";
        os << DataDelimiter();
    }
    os << DataFieldName("operating_conditions_");
    {
        output_map<OperatingConditions>(rhs.operating_conditions_map_, &os);
        os << DataDelimiter();
    }
    os << DataFieldName("wire_loads_");
    {
        output_map<WireLoad>(rhs.wire_loads_map_, &os);
        os << DataDelimiter();
    }
    os << DataFieldName("wire_load_tables_");
    {
        output_map<WireLoadTable>(rhs.wire_load_tables_map_, &os);
        os << DataDelimiter();
    }
    os << DataFieldName("wire_load_selections_");
    {
        output_map<WireLoadSelection>(rhs.wire_load_selections_map_, &os);
        os << DataDelimiter();
    }
    os << DataFieldName("table_templates_");
    {
        output_map<TableTemplate>(rhs.table_templates_map_, &os);
        os << DataDelimiter();
    }
    os << DataFieldName("timing_cells_");
    { output_vec<TCell>(rhs.timing_cells_, &os); }

    os << DataEnd(")");
    return os;
}

TCell *TLib::__addTCellImpl() {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        ArrayObject<ObjectId> *p = nullptr;
        if (timing_cells_ == UNINIT_OBJECT_ID) {
            p = Object::createObject<ArrayObject<ObjectId>>(
                kObjectTypeArray, timing_lib->getId());
            if (p != nullptr) {
                timing_cells_ = p->getId();
                p->setPool(timing_lib->getPool());
                p->reserve(32);
            }
        } else {
            p = addr<ArrayObject<ObjectId>>(timing_cells_);
        }
        if (p != nullptr) {
            auto c = Object::createObject<TCell>(kObjectTypeTCell,
                                                 timing_lib->getId());
            if (c) {
                c->setOwner(this);
                ObjectId id = c->getId();
                p->pushBack(id);
                return c;
            }
        }
    }
    return nullptr;
}

OperatingConditions *TLib::__addOperatingConditionsImpl() {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        ArrayObject<ObjectId> *object_array = nullptr;
        if (operating_conditions_ == UNINIT_OBJECT_ID) {
            object_array = Object::createObject<ArrayObject<ObjectId>>(
                kObjectTypeArray, timing_lib->getId());
            if (object_array != nullptr) {
                operating_conditions_ = object_array->getId();
                object_array->setPool(timing_lib->getPool());
                object_array->reserve(32);
            }
        } else {
            object_array = addr<ArrayObject<ObjectId>>(operating_conditions_);
        }
        if (object_array != nullptr) {
            auto c = timing_lib->createObject<OperatingConditions>(
                kObjectTypeOperatingConditions, timing_lib->getId());
            if (c) {
                c->setOwner(this);
                ObjectId id = c->getId();
                object_array->pushBack(id);
                return c;
            }
        }
    }
    return nullptr;
}
}  // namespace db
}  // namespace open_edi
