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
#include "util/util.h"

namespace open_edi {
namespace db {

using UInt32 = open_edi::util::UInt32;

enum class CaseAnalysisValue { kZero, kOne, kRise, kFall, kUnknown };

class SetCaseAnalysis {
  public:
    friend std::ostream &operator<<(std::ostream &os, SetCaseAnalysis &rhs);
    void setValue(std::string& input);

  private:
    CaseAnalysisValue value_ = CaseAnalysisValue::kUnknown;

  public:
    COMMAND_GET_SET_VAR(value, Value)
};
using SetCaseAnalysisPtr = std::shared_ptr<SetCaseAnalysis>;

class SetDrive {
  private:
    float resistance_  = 0.0;

  public:
    COMMAND_GET_SET_VAR(resistance, Resistance)
    COMMAND_GET_SET_FLAG(rise, Rise)
    COMMAND_GET_SET_FLAG(fall, Fall)
    COMMAND_GET_SET_FLAG(min, Min)
    COMMAND_GET_SET_FLAG(max, Max)
};
using SetDrivePtr = std::shared_ptr<SetDrive>;

class SetDrivingCell {
  private:
    ObjectId from_term_;
    ObjectId to_term_;

  public:
    COMMAND_GET_SET_VAR(from_term, FromTerm)
    COMMAND_GET_SET_VAR(to_term, ToTerm)
    COMMAND_GET_SET_FLAG(rise, Rise)
    COMMAND_GET_SET_FLAG(fall, Fall)
    COMMAND_GET_SET_FLAG(dont_scale, DontScale)
    COMMAND_GET_SET_FLAG(no_design_rule, NoDesignRule)
    COMMAND_GET_SET_FLAG(min, Min)
    COMMAND_GET_SET_FLAG(max, Max)
    COMMAND_GET_SET_FLAG(clock_fall, ClockFall)
};
using SetDrivingCellPtr = std::shared_ptr<SetDrivingCell>;

class SetFanoutLoad {
  private:
    float value_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(value, Value)
}; 
using SetFanoutLoadPtr = std::shared_ptr<SetFanoutLoad>;

class SetInputTransition {
  private:
    float transition_ = 0.0;
    std::vector<ClockId> clocks_;

  public:
    COMMAND_GET_SET_VAR(transition, Transition)
    COMMAND_GET_SET_VAR(clocks, Clocks)
    COMMAND_GET_SET_FLAG(rise, Rise)
    COMMAND_GET_SET_FLAG(fall, Fall)
    COMMAND_GET_SET_FLAG(min, Min)
    COMMAND_GET_SET_FLAG(max, Max)
    COMMAND_GET_SET_FLAG(clock_fall, ClockFall)
}; 
using SetInputTransitionPtr = std::shared_ptr<SetInputTransition>;

class SetLoad {
  private:
    float cap_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(cap, Cap)
    COMMAND_GET_SET_FLAG(min, Min)
    COMMAND_GET_SET_FLAG(max, Max)
    COMMAND_GET_SET_FLAG(substract_pin_load, SubstractPinLoad)
    COMMAND_GET_SET_FLAG(pin_load, PinLoad)
    COMMAND_GET_SET_FLAG(wire_load, WireLoad)
};
using SetLoadPtr = std::shared_ptr<SetLoad>;

enum class LogicValue {kZero, kOne, kDontCare, kUnknown};
class SetLogic {
  private:
    LogicValue value_ = LogicValue::kUnknown;

  public:
    COMMAND_GET_SET_VAR(value, Value)
};
using SetLogicPtr = std::shared_ptr<SetLogic>;

class SetMaxArea {
  private:
    float area_value_ = 0.0;
  public:
    COMMAND_GET_SET_VAR(area_value, AreaValue)
};
using SetMaxAreaPtr= std::shared_ptr<SetMaxArea>;

class SetMaxCapacitance {
  private:
    float cap_value_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(cap_value, CapValue)
};
using SetMaxCapacitancePtr = std::shared_ptr<SetMaxCapacitance>;

class SetMaxFanout {
  private:
    float fanout_value_ = 0.0; 

  public:
    COMMAND_GET_SET_VAR(fanout_value, FanoutValue)
};
using SetMaxFanoutPtr = std::shared_ptr<SetMaxFanout>;

class SetMaxTransition {
  private:
    float transition_value_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(transition_value, TransitionValue)
    COMMAND_GET_SET_FLAG(clock_path, ClockPath)
    COMMAND_GET_SET_FLAG(fall, Fall)
    COMMAND_GET_SET_FLAG(rise, Rise)
}; 
using SetMaxTransitionPtr = std::shared_ptr<SetMaxTransition>;

class SetMinCapacitance {
  private:
    float cap_value_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(cap_value, CapValue)
};
using SetMinCapacitancePtr = std::shared_ptr<SetMinCapacitance>;

enum class AnalysisType {kSingle = 0, kBcWc, kOnChipVariation, kUnknown};
class SetOperatingConditions {
  private:
    AnalysisType analysis_type_ = AnalysisType::kUnknown;
    OperatingConditions condition_;
};
using SetOperatingConditionsPtr = std::shared_ptr<SetOperatingConditions>;

class SetPortFanoutNumber {
  private:
    UInt32 fanout_number_ = 0;

  public:
    COMMAND_GET_SET_VAR(fanout_number, FanoutNumber)
};
using SetPortFanoutNumberPtr = std::shared_ptr<SetPortFanoutNumber>;

class SetResistance {
  private:
    float value_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(value, Value)
    COMMAND_GET_SET_FLAG(min, Min)
    COMMAND_GET_SET_FLAG(max, Max)
};
using SetResistancePtr = std::shared_ptr<SetResistance>;

class SetTimingDerate {
  private:
    float derate_value_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(derate_value, DerateValue)
    COMMAND_GET_SET_FLAG(min, Min)
    COMMAND_GET_SET_FLAG(max, Max)
    COMMAND_GET_SET_FLAG(rise, Rise)
    COMMAND_GET_SET_FLAG(fall, Fall)
    COMMAND_GET_SET_FLAG(early, Early)
    COMMAND_GET_SET_FLAG(late, Late)
    COMMAND_GET_SET_FLAG(static_type, StaticType)
    COMMAND_GET_SET_FLAG(dynamic_type, DynamicType)
    COMMAND_GET_SET_FLAG(increment, Increment)
    COMMAND_GET_SET_FLAG(clock, Clock)
    COMMAND_GET_SET_FLAG(data, Data)
    COMMAND_GET_SET_FLAG(net_delay, NetDelay)
    COMMAND_GET_SET_FLAG(cell_delay, CellDelay)
    COMMAND_GET_SET_FLAG(cell_check, CellCheck)
};
using SetTimingDeratePtr = std::shared_ptr<SetTimingDerate>;

class SetVoltage {
  private:
    float max_case_voltage_ = 0.0;
    float min_case_voltage_ = 0.0;
  public:
    COMMAND_GET_SET_VAR(max_case_voltage, MaxCaseVoltage)
    COMMAND_GET_SET_VAR(min_case_voltage, MinCaseVoltage)
};
using SetVoltagePtr = std::shared_ptr<SetVoltage>; 

class SetWireLoadMode {
  private:
    WireLoadMode mode_ = WireLoadMode::kUnknown; 

  public:
    COMMAND_GET_SET_VAR(mode, Mode)
};
using SetWireLoadModePtr = std::shared_ptr<SetWireLoadMode>;

class SetWireLoadModel {
  private:
    WireLoad wire_load_;

  public:
    COMMAND_GET_SET_VAR(wire_load, WireLoad)
    COMMAND_GET_SET_FLAG(min, Min)
    COMMAND_GET_SET_FLAG(max, Max)
};
using SetWireLoadModelPtr = std::shared_ptr<SetWireLoadModel>;

class SetWireLoadSelectionGroup {
  private:
    WireLoadSelection selection_;

  public:
    COMMAND_GET_SET_VAR(selection, Selection)
    COMMAND_GET_SET_FLAG(min, Min)
    COMMAND_GET_SET_FLAG(max, Max)
};
using SetWireLoadSelectionGroupPtr = std::shared_ptr<SetWireLoadSelectionGroup>;


}
}
#endif // EDI_DB_TIMING_SDC_ENVIRONMENT_COMMANDS_H_
