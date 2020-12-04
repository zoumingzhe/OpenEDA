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

// not used
class SetFanoutLoad {

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


}
}
#endif // EDI_DB_TIMING_SDC_ENVIRONMENT_COMMANDS_H_
