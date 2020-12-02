/**
 * @file timinglib_lib.h
 * @date 2020-08-26
 * @brief TLib class
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIB_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIB_H_

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "db/core/object.h"
#include "db/timing/timinglib/timinglib_commondef.h"
#include "db/util/array.h"

namespace open_edi {
namespace db {

class OperatingConditions;
class TCell;
class WireLoad;
class WireLoadTable;
class WireLoadSelection;
class TableTemplate;
class TUnits;
class ScaleFactors;

typedef struct supply_voltage__pair {
    SymbolIndex name = 0;
    float value = 0.0f;
} SupplyVoltagePair;

class TLib : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    TLib();

    /// @brief destructor
    ~TLib();

    /// @brief constructor
    TLib(Object *owner, IndexType id);

    /// @brief copy constructor
    TLib(TLib const &rhs);

    /// @brief move constructor
    TLib(TLib &&rhs) noexcept;

    /// @brief copy assignment
    TLib &operator=(TLib const &rhs);

    /// @brief move constructor
    TLib &operator=(TLib &&rhs) noexcept;

    void setName(const std::string &name);
    void setTimingModelType(TimingModel tm);
    void setNominalVoltage(float f);
    void setNominalProcess(float f);
    void setNominalTemperature(float f);
    void setInputThresholdPctFall(float f);
    void setInputThresholdPctRise(float f);
    void setOutputThresholdPctFall(float f);
    void setOutputThresholdPctRise(float f);
    void setSlewLowerThresholdPctFall(float f);
    void setSlewLowerThresholdPctRise(float f);
    void setSlewUpperThresholdPctFall(float f);
    void setSlewUpperThresholdPctRise(float f);
    void setSlewDerateFromLibrary(float f);
    void setDefaultWireLoadArea(float f);
    void setDefaultWireLoadMode(WireLoadMode w);
    void setDefaultWireLoadCapacitance(float f);
    void setDefaultWireLoadResistance(float f);
    void setDefaultInputPinCap(float f);
    void setDefaultOutputPinCap(float f);
    void setDefaultInoutPinCap(float f);
    void setDefaultMaxCapacitance(float f);
    void setDefaultMaxFanout(float f);
    void setDefaultMaxTransition(float f);
    void setDefaultFanoutLoad(float f);
    void setDefaultCellLeakagePower(float f);
    void addSupplyVoltage(const std::string &name, float f);

    /// @brief number of cells
    IndexType numTCells() const;

    /// @brief number of cells
    IndexType numOperatingConditions() const;

    /// @brief add a instance tcell
    TCell *addTimingCell(const std::string &name);

    /// @brief add a instance operatingconditions
    OperatingConditions *addOperatingConditions(const std::string &name);
    WireLoad *addWireLoad(const std::string &name);
    WireLoadTable *addWireLoadTable(const std::string &name);
    WireLoadSelection *addWireLoadSelection(const std::string &name);
    TableTemplate *addTableTemplate(const std::string &name);

    void setDefaultOperatingConditions(ObjectId id);
    ScaleFactors *getOrCreateScalingFactors(const std::string &name);
    TUnits *getOrCreateUnits();
    void setDefaultWireLoad(ObjectId id);
    void setDefaultWireLoadSelection(ObjectId id);

    std::string getName(void) const;
    TimingModel getTimingModelType(void);
    float getNominalVoltage(void);
    float getNominalProcess(void);
    float getNominalTemperature(void);
    float getInputThresholdPctFall(void);
    float getInputThresholdPctRise(void);
    float getOutputThresholdPctFall(void);
    float getOutputThresholdPctRise(void);
    float getSlewLowerThresholdPctFall(void);
    float getSlewLowerThresholdPctRise(void);
    float getSlewUpperThresholdPctFall(void);
    float getSlewUpperThresholdPctRise(void);
    float getSlewDerateFromLibrary(void);
    float getDefaultWireLoadArea(void);
    WireLoadMode getDefaultWireLoadMode(void);
    float getDefaultWireLoadCapacitance(void);
    float getDefaultWireLoadResistance(void);
    float getDefaultInputPinCap(void);
    float getDefaultOutputPinCap(void);
    float getDefaultInoutPinCap(void);
    float getDefaultMaxCapacitance(void);
    float getDefaultMaxFanout(void);
    float getDefaultMaxTransition(void);
    float getDefaultFanoutLoad(void);
    float getDefaultCellLeakagePower(void);
    float getSupplyVoltage(const std::string &name);
    OperatingConditions *getDefaultOperatingConditions(void) const;
    ScaleFactors *getScalingFactors(void);
    TUnits *getUnits(void);
    WireLoad *getDefaultWireLoad(void) const;
    WireLoadSelection *getDefaultWireLoadSelection(void) const;
    OperatingConditions *getOperatingConditions(const std::string &name) const;
    WireLoad *getWireLoad(const std::string &name) const;
    WireLoadTable *getWireLoadTable(const std::string &name);
    WireLoadSelection *getWireLoadSelection(const std::string &name) const;
    TableTemplate *getTableTemplate(const std::string &name);
    TCell *getTimingCell(const std::string &name);
    std::vector<TCell *> getTimingCells(void);

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// @brief output the information
    void print(std::ostream &stream);

  protected:
    /// @brief copy object
    void copy(TLib const &rhs);
    /// @brief move object
    void move(TLib &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TLib const &rhs);

  private:
    /// @brief add one tcell
    /// @return the added tcell
    TCell *__addTCellImpl();

    /// @brief add one operatingconditions
    /// @return the added operatingconditions
    OperatingConditions *__addOperatingConditionsImpl();

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
    ObjectId supply_voltages_;
    ObjectId default_operating_conditions_;
    ObjectId scaling_factors_;
    ObjectId units_;
    ObjectId default_wire_load_;
    ObjectId default_wire_load_selection_;
    ObjectId operating_conditions_;
    ObjectId wire_loads_;
    ObjectId wire_load_tables_;
    ObjectId wire_load_selections_;
    ObjectId table_templates_;
    ObjectId timing_cells_;
    std::unordered_map<SymbolIndex, float> supply_voltage_map_;
    std::unordered_map<SymbolIndex, ObjectId> operating_conditions_map_;
    std::unordered_map<SymbolIndex, ObjectId> wire_loads_map_;
    std::unordered_map<SymbolIndex, ObjectId> wire_load_tables_map_;
    std::unordered_map<SymbolIndex, ObjectId> wire_load_selections_map_;
    std::unordered_map<SymbolIndex, ObjectId> table_templates_map_;
    std::unordered_map<SymbolIndex, ObjectId> timing_cells_map_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIB_H_
