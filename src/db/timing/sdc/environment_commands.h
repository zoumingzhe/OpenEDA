/** 
 * @file environment_commands.h
 * @date 2020-11-25
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

#ifndef EDI_DB_TIMING_SDC_ENVIRONMENT_COMMANDS_H_
#define EDI_DB_TIMING_SDC_ENVIRONMENT_COMMANDS_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>

#include "db/core/object.h"
#include "db/timing/timinglib/timinglib_wireload.h"
#include "db/timing/timinglib/timinglib_opcond.h"
#include "db/timing/sdc/command_get_set_property.h"
#include "db/timing/sdc/clock.h"
#include "db/timing/sdc/sdc_common.h"

namespace open_edi {
namespace db {

using UInt32 = open_edi::util::UInt32;
using Bits8 = open_edi::util::Bits8;

enum class CaseAnalysisValue : Bits8 { kZero, kOne, kRise, kFall, kUnknown };
std::string to_string(const CaseAnalysisValue &value);
class SetCaseAnalysis {
  public:
    bool setValue(std::string& input);

  private:
    CaseAnalysisValue value_ = CaseAnalysisValue::kUnknown;

  public:
    COMMAND_GET_SET_VAR(value, Value)
};
using SetCaseAnalysisPtr = std::shared_ptr<SetCaseAnalysis>;

class CaseAnalysisCotnainerData {
  public:
    bool add(const std::string &pin_name, const SetCaseAnalysis &case_analysis);

  private:
    std::unordered_map<ObjectId, SetCaseAnalysis> pin_to_case_analysis_;

  public:
    COMMAND_GET_SET_VAR(pin_to_case_analysis, PinToCaseAnalysis)
};
using CaseAnalysisCotnainerDataPtr = std::shared_ptr<CaseAnalysisCotnainerData>;

class SetDrive {
  private:
    float resistance_  = 0.0;

  public:
    COMMAND_GET_SET_VAR(resistance, Resistance)
    COMMAND_GET_SET_FLAG(rise_, Rise)
    COMMAND_GET_SET_FLAG(fall_, Fall)
    COMMAND_GET_SET_FLAG(min_, Min)
    COMMAND_GET_SET_FLAG(max_, Max)
};
using SetDrivePtr = std::shared_ptr<SetDrive>;

class DriveContainerData {
  public:
    void add(const ObjectId &pin_id, const SetDrivePtr &drive) { pin_drives_.emplace(pin_id, drive); }

  private:
    //TODO multi map
    std::unordered_map<ObjectId, SetDrivePtr> pin_drives_;

  public:
    COMMAND_GET_SET_VAR(pin_drives, PinDrives)
};
using DriveContainerDataPtr = std::shared_ptr<DriveContainerData>;

class SetDrivingCell {
  private:
    ObjectId from_term_;
    ObjectId to_term_;

  public:
    COMMAND_GET_SET_VAR(from_term, FromTerm)
    COMMAND_GET_SET_VAR(to_term, ToTerm)
    COMMAND_GET_SET_FLAG(rise_, Rise)
    COMMAND_GET_SET_FLAG(fall_, Fall)
    COMMAND_GET_SET_FLAG(dont_scale_, DontScale)
    COMMAND_GET_SET_FLAG(no_design_rule_, NoDesignRule)
    COMMAND_GET_SET_FLAG(min_, Min)
    COMMAND_GET_SET_FLAG(max_, Max)
    COMMAND_GET_SET_FLAG(clock_fall_, ClockFall)
};
using SetDrivingCellPtr = std::shared_ptr<SetDrivingCell>;

class DrivingCellData {
  public:
    void add(const ObjectId &pin_id, const SetDrivingCellPtr &cell) { pin_driving_cell_.emplace(pin_id, cell); }

  private:
    //TODO multi map
    std::unordered_map<ObjectId, SetDrivingCellPtr> pin_driving_cell_;

  public:
    COMMAND_GET_SET_VAR(pin_driving_cell, PinDrivingCell)
};
using DrivingCellDataPtr = std::shared_ptr<DrivingCellData>;

class SetFanoutLoad {
  private:
    float value_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(value, Value)
}; 
using SetFanoutLoadPtr = std::shared_ptr<SetFanoutLoad>;

class FanoutLoadContainerData {
  public:
    void add(const ObjectId &pin_id, const SetFanoutLoadPtr &fanout) { pin_fanout_load_.emplace(pin_id, fanout); }

  private:
    std::unordered_map<ObjectId, SetFanoutLoadPtr> pin_fanout_load_;

  public:
    COMMAND_GET_SET_VAR(pin_fanout_load, PinFanoutLoad)
};
using FanoutLoadContainerDataPtr = std::shared_ptr<FanoutLoadContainerData>;

class SetInputTransition {
  private:
    float transition_ = 0.0;
    std::vector<ClockId> clocks_;

  public:
    COMMAND_GET_SET_VAR(transition, Transition)
    COMMAND_GET_SET_VAR(clocks, Clocks)
    COMMAND_GET_SET_FLAG(rise_, Rise)
    COMMAND_GET_SET_FLAG(fall_, Fall)
    COMMAND_GET_SET_FLAG(min_, Min)
    COMMAND_GET_SET_FLAG(max_, Max)
    COMMAND_GET_SET_FLAG(clock_fall_, ClockFall)
}; 
using SetInputTransitionPtr = std::shared_ptr<SetInputTransition>;

class InputTransitionData {
  public:
    void add(const ObjectId &pin_id, const SetInputTransitionPtr&transition) { pin_input_transitions_.emplace(pin_id, transition); }

  private:
    std::unordered_map<ObjectId, SetInputTransitionPtr> pin_input_transitions_;

  public:
    COMMAND_GET_SET_VAR(pin_input_transitions, PinInputTransitions)
};
using InputTransitionDataPtr = std::shared_ptr<InputTransitionData>;

class SetLoad {
  private:
    float cap_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(cap, Cap)
    COMMAND_GET_SET_FLAG(min_, Min)
    COMMAND_GET_SET_FLAG(max_, Max)
    COMMAND_GET_SET_FLAG(substract_pin_load_, SubstractPinLoad)
    COMMAND_GET_SET_FLAG(pin_load_, PinLoad) //default is pin_load
    COMMAND_GET_SET_FLAG(wire_load_, WireLoad)
};
using SetLoadPtr = std::shared_ptr<SetLoad>;

class LoadContainerData {
  public:
    void addToPin(const ObjectId &pin_id, const SetLoadPtr &load) { pin_load_caps_.emplace(pin_id, load); }
    void addToNet(const ObjectId &wire_id, const SetLoadPtr &load) { wire_load_caps_.emplace(wire_id, load); }

  private:
    //TODO multi map
    std::unordered_map<ObjectId, SetLoadPtr> pin_load_caps_;
    std::unordered_map<ObjectId, SetLoadPtr> wire_load_caps_;

  public:
    COMMAND_GET_SET_VAR(pin_load_caps, PinLoadCaps)
    COMMAND_GET_SET_VAR(wire_load_caps, WireLoadCaps)
};
using LoadContainerDataPtr = std::shared_ptr<LoadContainerData>;

enum class LogicValue : Bits8 {kZero, kOne, kDontCare, kUnknown};
std::string to_string(const LogicValue &value);
class SetLogic {
  private:
    LogicValue value_ = LogicValue::kUnknown;

  public:
    COMMAND_GET_SET_VAR(value, Value)
};
using SetLogicPtr = std::shared_ptr<SetLogic>;

class LogicContainerData {
  public:
    void add(const ObjectId &pin_id, const SetLogic &logic) { pin_logic_value_.emplace(pin_id, logic); };

  private:
    std::unordered_map<ObjectId, SetLogic> pin_logic_value_;

  public:
    COMMAND_GET_SET_VAR(pin_logic_value, PinLogicValue)
};
using LogicContainerDataPtr = std::shared_ptr<LogicContainerData>;

class SetMaxArea {
  private:
    float area_value_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(area_value, AreaValue)
};
using SetMaxAreaPtr= std::shared_ptr<SetMaxArea>;

class MaxAreaContainerData {
  public:
    void add(const ObjectId &cell_id, const SetMaxAreaPtr &area) { cell_max_area_.emplace(cell_id, area); }

  private:
    std::unordered_map<ObjectId, SetMaxAreaPtr> cell_max_area_;

  public:
    COMMAND_GET_SET_VAR(cell_max_area, CellMaxArea)
};
using MaxAreaContainerDataPtr = std::shared_ptr<MaxAreaContainerData>;

class SetMaxCapacitance {
  private:
    float cap_value_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(cap_value, CapValue)
};
using SetMaxCapacitancePtr = std::shared_ptr<SetMaxCapacitance>;

class MaxCapacitanceContainerData {
  public:
    void addToPin(const ObjectId &pin_id, const SetMaxCapacitancePtr &cap) { pin_max_caps_.emplace(pin_id, cap); }
    void addToCell(const ObjectId &cell_id, const SetMaxCapacitancePtr &cap) { cell_max_caps_.emplace(cell_id, cap); }

  private:
    std::unordered_map<ObjectId, SetMaxCapacitancePtr> pin_max_caps_; 
    std::unordered_map<ObjectId, SetMaxCapacitancePtr> cell_max_caps_; 

  public:
    COMMAND_GET_SET_VAR(pin_max_caps, PinMaxCaps)
    COMMAND_GET_SET_VAR(cell_max_caps, CellMaxCaps)
};
using MaxCapacitanceContainerDataPtr = std::shared_ptr<MaxCapacitanceContainerData>;

class SetMaxFanout {
  private:
    float fanout_value_ = 0.0; 

  public:
    COMMAND_GET_SET_VAR(fanout_value, FanoutValue)
};
using SetMaxFanoutPtr = std::shared_ptr<SetMaxFanout>;

class MaxFanoutContainerData {
  public:
    void addToPin(const ObjectId &pin_id, const SetMaxFanoutPtr &fanout) { pin_max_fanout_.emplace(pin_id, fanout); }
    void addToCell(const ObjectId &cell_id, const SetMaxFanoutPtr &fanout) { cell_max_fanout_.emplace(cell_id, fanout); }

  private:
    std::unordered_map<ObjectId, SetMaxFanoutPtr> pin_max_fanout_;
    std::unordered_map<ObjectId, SetMaxFanoutPtr> cell_max_fanout_;

  public:
    COMMAND_GET_SET_VAR(pin_max_fanout, PinMaxFanout)
    COMMAND_GET_SET_VAR(cell_max_fanout, CellMaxFanout)
};
using MaxFanoutContainerDataPtr = std::shared_ptr<MaxFanoutContainerData>;

class SetMaxTransition {
  private:
    float transition_value_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(transition_value, TransitionValue)
    COMMAND_GET_SET_FLAG(clock_path_, ClockPath)
    COMMAND_GET_SET_FLAG(fall_, Fall)
    COMMAND_GET_SET_FLAG(rise_, Rise)
}; 
using SetMaxTransitionPtr = std::shared_ptr<SetMaxTransition>;

class MaxTransitionContainerData {
  public:
    void addToClock(const ClockId &clock_id, const SetMaxTransitionPtr &transition) { clock_max_transition_.emplace(clock_id, transition); }
    void addToCell(const ObjectId &cell_id, const SetMaxTransitionPtr &transition) { cell_max_transition_.emplace(cell_id, transition); }
    void addToPin(const ObjectId &pin_id, const SetMaxTransitionPtr &transition) { pin_max_transition_.emplace(pin_id, transition); }

  private:
    std::unordered_map<ClockId, SetMaxTransitionPtr> clock_max_transition_;
    std::unordered_map<ObjectId, SetMaxTransitionPtr> cell_max_transition_;
    std::unordered_map<ObjectId, SetMaxTransitionPtr> pin_max_transition_;

  public:
    COMMAND_GET_SET_VAR(clock_max_transition, ClockMaxTransition)
    COMMAND_GET_SET_VAR(cell_max_transition, CellMaxTransition)
    COMMAND_GET_SET_VAR(pin_max_transition, PinMaxTransition)
};
using MaxTransitionContainerDataPtr = std::shared_ptr<MaxTransitionContainerData>;

class SetMinCapacitance {
  private:
    float cap_value_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(cap_value, CapValue)
};
using SetMinCapacitancePtr = std::shared_ptr<SetMinCapacitance>;

class MinCapacitanceContainerData {
  public:
    void addToPin(const ObjectId &pin_id, const SetMinCapacitancePtr &cap) { pin_min_caps_.emplace(pin_id, cap); }
    void addToCell(const ObjectId &cell_id, const SetMinCapacitancePtr &cap) { cell_min_caps_.emplace(cell_id, cap); }

  private:
    std::unordered_map<ObjectId, SetMinCapacitancePtr> pin_min_caps_; 
    std::unordered_map<ObjectId, SetMinCapacitancePtr> cell_min_caps_; 

  public:
    COMMAND_GET_SET_VAR(pin_min_caps, PinMinCaps)
    COMMAND_GET_SET_VAR(cell_min_caps, cellMinCaps)
};
using MinCapacitanceContainerDataPtr = std::shared_ptr<MinCapacitanceContainerData>;

enum class AnalysisType {kSingle = 0, kBcWc, kOnChipVariation, kUnknown};
class SetOperatingConditions {
  private:
    AnalysisType analysis_type_ = AnalysisType::kUnknown;
    OperatingConditions min_condition_;
    OperatingConditions max_condition_;

  public:
    COMMAND_GET_SET_VAR(analysis_type, AnalysisType)
    COMMAND_GET_SET_VAR(min_condition, MinCondition)
    COMMAND_GET_SET_VAR(max_condition, MaxCondition)
};
using SetOperatingConditionsPtr = std::shared_ptr<SetOperatingConditions>;

class OperatingConditionsContainerData {
  public:
    void add(const ObjectId &cell_id, const SetOperatingConditionsPtr &condition) { cell_operating_conditions_.emplace(cell_id, condition); }

  private:
    std::unordered_map<ObjectId, SetOperatingConditionsPtr> cell_operating_conditions_;

  public:
    COMMAND_GET_SET_VAR(cell_operating_conditions, CellOperatingConditions);
};
using OperatingConditionsContainerDataPtr = std::shared_ptr<OperatingConditionsContainerData>;

class SetPortFanoutNumber {
  private:
    UInt32 fanout_number_ = 0;

  public:
    //rise fall ?
    COMMAND_GET_SET_VAR(fanout_number, FanoutNumber)
};
using SetPortFanoutNumberPtr = std::shared_ptr<SetPortFanoutNumber>;

class PortFanoutNumberContainerData {
  public:
    void add(const ObjectId &pin_id, const SetPortFanoutNumberPtr &fanout) { pin_fanout_number_.emplace(pin_id, fanout); }

  private:
    // multi map if introduce rise and fall flag
    std::unordered_map<ObjectId, SetPortFanoutNumberPtr> pin_fanout_number_;

  public:
    COMMAND_GET_SET_VAR(pin_fanout_number, PinFanoutNumber)
};
using PortFanoutNumberContainerDataPtr = std::shared_ptr<PortFanoutNumberContainerData>;

class SetResistance {
  private:
    float value_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(value, Value)
    COMMAND_GET_SET_FLAG(min_, Min)
    COMMAND_GET_SET_FLAG(max_, Max)
};
using SetResistancePtr = std::shared_ptr<SetResistance>;

class ResistanceContainerData {
  public:
    void add(const ObjectId &net_id, const SetResistancePtr &res) { net_resistance_.emplace(net_id, res); }

  private:
    //multi map
    std::unordered_map<ObjectId, SetResistancePtr> net_resistance_;

  public:
    COMMAND_GET_SET_VAR(net_resistance, NetResistance)
};
using ResistanceContainerDataPtr = std::shared_ptr<ResistanceContainerData>;

class SetTimingDerate {
  private:
    float derate_value_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(derate_value, DerateValue)
    COMMAND_GET_SET_FLAG(min_, Min)
    COMMAND_GET_SET_FLAG(max_, Max)
    COMMAND_GET_SET_FLAG(rise_, Rise)
    COMMAND_GET_SET_FLAG(fall_, Fall)
    COMMAND_GET_SET_FLAG(early_, Early)
    COMMAND_GET_SET_FLAG(late_, Late)
    COMMAND_GET_SET_FLAG(static_type_, StaticType)
    COMMAND_GET_SET_FLAG(dynamic_type_, DynamicType)
    COMMAND_GET_SET_FLAG(increment_, Increment)
    COMMAND_GET_SET_FLAG(clock_, Clock)
    COMMAND_GET_SET_FLAG(data_, Data)
    COMMAND_GET_SET_FLAG(net_delay_, NetDelay)
    COMMAND_GET_SET_FLAG(cell_delay_, CellDelay)
    COMMAND_GET_SET_FLAG(cell_check_, CellCheck)
};
using SetTimingDeratePtr = std::shared_ptr<SetTimingDerate>;

class TimingDerateContainerData {
  public:
    void addToNet(const ObjectId &net_id, const SetTimingDeratePtr &derate) { net_timing_derate_.emplace(net_id, derate); }
    void addToInst(const ObjectId &inst_id, const SetTimingDeratePtr &derate) { inst_timing_derate_.emplace(inst_id, derate); }
    void addToCell(const ObjectId &cell_id, const SetTimingDeratePtr &derate) { cell_timing_derate_.emplace(cell_id, derate); }

  private:
    std::unordered_map<ObjectId, SetTimingDeratePtr> net_timing_derate_;
    std::unordered_map<ObjectId, SetTimingDeratePtr> inst_timing_derate_;
    std::unordered_map<ObjectId, SetTimingDeratePtr> cell_timing_derate_;

  public:
    COMMAND_GET_SET_VAR(net_timing_derate, NetTimingDerate)
    COMMAND_GET_SET_VAR(inst_timing_derate, InstTimingDerate)
    COMMAND_GET_SET_VAR(cell_timing_derate, CellTimingDerate)
};
using TimingDerateContainerDataPtr = std::shared_ptr<TimingDerateContainerData>;

class SetVoltage {
  private:
    float max_case_voltage_ = 0.0;
    float min_case_voltage_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(max_case_voltage, MaxCaseVoltage)
    COMMAND_GET_SET_VAR(min_case_voltage, MinCaseVoltage)
};
using SetVoltagePtr = std::shared_ptr<SetVoltage>; 

class VoltageContainerData {
  public:
    void addToNet(const ObjectId &net_id, const SetVoltagePtr &voltage) { net_voltage_.emplace(net_id, voltage); }
    void addToPin(const ObjectId &pin_id, const SetVoltagePtr &voltage) { pin_voltage_.emplace(pin_id, voltage); }

  private:
    std::unordered_map<ObjectId, SetVoltagePtr> net_voltage_;
    std::unordered_map<ObjectId, SetVoltagePtr> pin_voltage_;

  public:
    COMMAND_GET_SET_VAR(net_voltage, NetVoltage)
    COMMAND_GET_SET_VAR(pin_voltage, PinVoltage)
};
using VoltageContainerDataPtr = std::shared_ptr<VoltageContainerData>;

class SetWireLoadMinBlockSize {
  private:
    float block_size_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(block_size, BlockSize)
};
using SetWireLoadMinBlockSizePtr = std::shared_ptr<SetWireLoadMinBlockSize>;

class WireLoadMinBlockSizeContainerData {
  public:
    void add(const ObjectId &cell_id, const SetWireLoadMinBlockSizePtr &size) { cell_min_block_size_.emplace(cell_id, size); }

  private:
    std::unordered_map<ObjectId, SetWireLoadMinBlockSizePtr> cell_min_block_size_;

  public:
    COMMAND_GET_SET_VAR(cell_min_block_size, CellMinBlockSize)
};
using WireLoadMinBlockSizeContainerDataPtr = std::shared_ptr<WireLoadMinBlockSizeContainerData>;

class SetWireLoadMode {
  private:
    WireLoadMode mode_ = WireLoadMode::kUnknown; 

  public:
    COMMAND_GET_SET_VAR(mode, Mode)
};
using SetWireLoadModePtr = std::shared_ptr<SetWireLoadMode>;

class WireLoadModeContainerData {
  public:
    void add(const ObjectId &cell_id, const SetWireLoadMode &mode) { cell_wire_load_mode_.emplace(cell_id, mode); }

  private:
    std::unordered_map<ObjectId, SetWireLoadMode> cell_wire_load_mode_;

  public:
    COMMAND_GET_SET_VAR(cell_wire_load_mode, CellWireLoadMode)
};
using WireLoadModeContainerDataPtr = std::shared_ptr<WireLoadModeContainerData>; 

class SetWireLoadModel {
  private:
    WireLoad wire_load_;

  public:
    COMMAND_GET_SET_VAR(wire_load, WireLoad)
    COMMAND_GET_SET_FLAG(min_, Min)
    COMMAND_GET_SET_FLAG(max_, Max)
};
using SetWireLoadModelPtr = std::shared_ptr<SetWireLoadModel>;

class WireLoadModelContainerData {
  public:
    void addToCell(const ObjectId &cell_id, const SetWireLoadModelPtr &model) { cell_wire_load_model_.emplace(cell_id, model); }
    void addToPin(const ObjectId &pin_id, const SetWireLoadModelPtr &model) { pin_wire_load_model_.emplace(pin_id, model); }

  private:
    std::unordered_map<ObjectId, SetWireLoadModelPtr> cell_wire_load_model_;
    std::unordered_map<ObjectId, SetWireLoadModelPtr> pin_wire_load_model_;

  public:
    COMMAND_GET_SET_VAR(cell_wire_load_model, CellWireLoadModel)
    COMMAND_GET_SET_VAR(pin_wire_load_model, PinWireLoadModel)
};
using WireLoadModelContainerDataPtr = std::shared_ptr<WireLoadModelContainerData>;

class SetWireLoadSelectionGroup {
  private:
    WireLoadSelection selection_;

  public:
    COMMAND_GET_SET_VAR(selection, Selection)
    COMMAND_GET_SET_FLAG(min_, Min)
    COMMAND_GET_SET_FLAG(max_, Max)
};
using SetWireLoadSelectionGroupPtr = std::shared_ptr<SetWireLoadSelectionGroup>;

class WireLoadSelectionGroupContainerData {
  public:
    void add(const ObjectId &cell_id, const SetWireLoadSelectionGroupPtr &group) { cell_wire_selection_group_.emplace(cell_id, group); }

  private:
    std::unordered_map<ObjectId, SetWireLoadSelectionGroupPtr> cell_wire_selection_group_;

  public:
    COMMAND_GET_SET_VAR(cell_wire_selection_group, CellWireSelectionGroup)
};
using WireLoadSelectionGroupContainerDataPtr = std::shared_ptr<WireLoadSelectionGroupContainerData>;


}
}
#endif // EDI_DB_TIMING_SDC_ENVIRONMENT_COMMANDS_H_
