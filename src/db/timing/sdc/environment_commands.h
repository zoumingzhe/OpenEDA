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
#include "db/timing/sdc/command_get_set_property.h"

namespace open_edi {
namespace db {

enum class CaseAnalysisValue {kZero, kOne, kRise, kFall, kUnknown};

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
    std::bitset<4> flags_;

  public:
    COMMAND_GET_SET_VAR(resistance, Resistance)
    COMMAND_GET_SET_FLAG(flags, 0, rise, Rise)
    COMMAND_GET_SET_FLAG(flags, 1, fall, Fall)
    COMMAND_GET_SET_FLAG(flags, 2, min, Min)
    COMMAND_GET_SET_FLAG(flags, 3, max, Max)
};
using SetDrivePtr = std::shared_ptr<SetDrive>;

class SetDrivingCell {
  private:
    ObjectId from_term_;
    ObjectId to_term_;
    std::bitset<7> flags_;

  public:
    COMMAND_GET_SET_VAR(from_term, FromTerm)
    COMMAND_GET_SET_VAR(to_term, ToTerm)
    COMMAND_GET_SET_FLAG(flags, 0, rise, Rise)
    COMMAND_GET_SET_FLAG(flags, 1, fall, Fall)
    COMMAND_GET_SET_FLAG(flags, 2, dont_scale, DontScale)
    COMMAND_GET_SET_FLAG(flags, 3, no_design_rule, NoDesignRule)
    COMMAND_GET_SET_FLAG(flags, 4, min, Min)
    COMMAND_GET_SET_FLAG(flags, 5, max, Max)
    COMMAND_GET_SET_FLAG(flags, 6, clock_fall, ClockFall)
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
    std::bitset<5> flags_;

  public:
    COMMAND_GET_SET_VAR(transition, Transition)
    COMMAND_GET_SET_VAR(clocks, Clocks)
    COMMAND_GET_SET_FLAG(flags, 0, rise, Rise)
    COMMAND_GET_SET_FLAG(flags, 1, fall, Fall)
    COMMAND_GET_SET_FLAG(flags, 2, min, Min)
    COMMAND_GET_SET_FLAG(flags, 3, max, Max)
    COMMAND_GET_SET_FLAG(flags, 4, clock_fall, ClockFall)
}; 
using SetInputTransitionPtr = std::shared_ptr<SetInputTransition>;

class SetLoad {
  private:
    float cap_ = 0.0;
    std::bitset<5> flags_;

  public:
    COMMAND_GET_SET_VAR(cap, Cap)
    COMMAND_GET_SET_FLAG(flags, 0, min, Min)
    COMMAND_GET_SET_FLAG(flags, 1, max, Max)
    COMMAND_GET_SET_FLAG(flags, 2, substract_pin_load, SubstractPinLoad)
    COMMAND_GET_SET_FLAG(flags, 3, pin_load, PinLoad)
    COMMAND_GET_SET_FLAG(flags, 4, wire_load, WireLoad)
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
    std::bitset<3> flags_;

  public:
    COMMAND_GET_SET_VAR(transition_value, TransitionValue)
    COMMAND_GET_SET_FLAG(flags, 0, clock_path, ClockPath)
    COMMAND_GET_SET_FLAG(flags, 1, fall, Fall)
    COMMAND_GET_SET_FLAG(flags, 2, rise, Rise)
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
    Uint32 fanout_number_ = 0;

  public:
    COMMAND_GET_SET_VAR(fanout_number, FanoutNumber)
};
using SetPortFanoutNumberPtr = std::shared_ptr<SetPortFanoutNumber>;

class SetResistance {
  private:
    float value_ = 0.0;
    std::bitset<2> flags_;

  public:
    COMMAND_GET_SET_VAR(value, Value)
    COMMAND_GET_SET_FLAG(flags, 0, min, Min)
    COMMAND_GET_SET_FLAG(flags, 1, max, Max)
};
using SetResistancePtr = std::shared_ptr<SetResistance>;

class SetTimingDerate {
  private:
    float derate_value_ = 0.0;
    std::bitset<14> flags_;

  public:
    COMMAND_GET_SET_VAR(derate_value, DerateValue)
    COMMAND_GET_SET_FLAG(flags, 0, min, Min)
    COMMAND_GET_SET_FLAG(flags, 1, max, Max)
    COMMAND_GET_SET_FLAG(flags, 2, rise, Rise)
    COMMAND_GET_SET_FLAG(flags, 3, fall, Fall)
    COMMAND_GET_SET_FLAG(flags, 4, early, Early)
    COMMAND_GET_SET_FLAG(flags, 5, late, Late)
    COMMAND_GET_SET_FLAG(flags, 6, static_type, StaticType)
    COMMAND_GET_SET_FLAG(flags, 7, dynamic_type, DynamicType)
    COMMAND_GET_SET_FLAG(flags, 8, increment, Increment)
    COMMAND_GET_SET_FLAG(flags, 9, clock, Clock)
    COMMAND_GET_SET_FLAG(flags, 10, data, Data)
    COMMAND_GET_SET_FLAG(flags, 11, net_delay, NetDelay)
    COMMAND_GET_SET_FLAG(flags, 12, cell_delay, CellDelay)
    COMMAND_GET_SET_FLAG(flags, 13, cell_check, CellCheck)
};
using SetTimingDeratePtr = std::shared_ptr<SetTimingDerate>;

class SetVoltage {
  private:
    float value_ = 0.0;


};
using SetVoltagePtr = std::shared_ptr<SetVoltage>; 


}
}
#endif // EDI_DB_TIMING_SDC_ENVIRONMENT_COMMANDS_H_
