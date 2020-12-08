/** 
 * @file multivoltage_power_commands.h
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

#ifndef EDI_DB_TIMING_SDC_MULTIVOLTAGE_POWER_COMMANDS_H_
#define EDI_DB_TIMING_SDC_MULTIVOLTAGE_POWER_COMMANDS_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "db/core/object.h"

namespace open_edi {
namespace db {


class CreateVoltageArea {
  private:
    std::string name_ = "";
    geo::Box coordinate_;
    geo::Point<float> polygons_;
    int guard_band_x_ = 0;
    int guard_band_y_ = 0;
  
  public:
    COMMAND_GET_SET_VAR(name, Name)
    COMMAND_GET_SET_VAR(coordinate, Coordinate)
    COMMAND_GET_SET_VAR(polygons, Polygons)
    COMMAND_GET_SET_VAR(guard_band_x, GuardBandX)
    COMMAND_GET_SET_VAR(guard_band_y, GuardBandY)
};
using CreateVoltageAreaPtr = std::shared_ptr<CreateVoltageArea>;

enum class LevelShifterStrategy {kAll, kLowToHigh, kHighToLow, kUnknown};
class SetLevelShifterStrategy {
  private:
    LevelShifterStrategy rule_ = LevelShifterStrategy::kUnknown;
  public:
    COMMAND_GET_SET_VAR(rule, Rule)
}; 
using SetLevelShifterStrategyPtr = std::shared_ptr<SetLevelShifterStrategy>;

class SetLevelShifterThreshold {
  private:
    float voltage_ = 0.0;
    float percent_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(voltage, Voltage)
    COMMAND_GET_SET_VAR(percent, Percent)
};
using SetLevelShifterThresholdPtr = std::shared_ptr<SetLevelShifterThreshold>;

class SetMaxDynamicPower {
  private:
    float power_value_ = 0.0; 

  public:
    COMMAND_GET_SET_VAR(power_value, PowerValue)
};
using SetMaxDynamicPowerPtr = std::shared_ptr<SetMaxDynamicPower>;

class SetMaxLeakagePower {
  private:
    float leakage_power_  = 0.0;

  public:
    COMMAND_GET_SET_VAR(leakage_power, LeakagePower)
};



}
}
#endif //EDI_DB_TIMING_SDC_MULTIVOLTAGE_POWER_COMMANDS_H_
