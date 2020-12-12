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
#include "db/timing/sdc/command_get_set_property.h"

namespace open_edi {
namespace db {

class BoxCoordinate {
  private:
    float llx_ = 0.0;
    float lly_ = 0.0;
    float urx_ = 0.0;
    float ury_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(llx, LLX)
    COMMAND_GET_SET_VAR(lly, LLY)
    COMMAND_GET_SET_VAR(urx, URX)
    COMMAND_GET_SET_VAR(ury, URY)
};
using BoxCoordinatePtr = std::shared_ptr<BoxCoordinate>;

class CreateVoltageArea {
  public:
    void add(BoxCoordinatePtr &box) { coordinates_.emplace_back(box); }

  private:
    std::string name_ = "";
    std::vector<BoxCoordinatePtr> coordinates_;
    int guard_band_x_ = 0;
    int guard_band_y_ = 0;
  
  public:
    COMMAND_GET_SET_VAR(name, Name)
    COMMAND_GET_SET_VAR(coordinates, Coordinates)
    COMMAND_GET_SET_VAR(guard_band_x, GuardBandX)
    COMMAND_GET_SET_VAR(guard_band_y, GuardBandY)
};
using CreateVoltageAreaPtr = std::shared_ptr<CreateVoltageArea>;

class VoltageAreaContainerData {
  public:
    void add(ObjectId &cell_id, CreateVoltageAreaPtr &voltage_area) { cell_voltage_area_.emplace(cell_id, voltage_area); } 

  private:
    std::unordered_map<ObjectId, CreateVoltageAreaPtr> cell_voltage_area_;

  public:
    COMMAND_GET_SET_VAR(cell_voltage_area, CellVoltageArea)
};
using VoltageAreaContainerDataPtr = std::shared_ptr<VoltageAreaContainerData>; 

enum class LevelShifterStrategy : Bits8 {kAll, kLowToHigh, kHighToLow, kUnknown};
inline std::string toString(const LevelShifterStrategy &strategy) {
    switch (strategy) {
        case LevelShifterStrategy::kAll:
            return "all";
        case LevelShifterStrategy::kLowToHigh:
            return "low_to_high";
        case LevelShifterStrategy::kHighToLow:
            return "high_to_low";
        default:
            return "Unknown";
    }
}

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
  public:
    void set(const float &value, const std::string &unit);

  private:
    float power_value_ = 0.0; 

  public:
    COMMAND_GET_SET_VAR(power_value, PowerValue)
};
using SetMaxDynamicPowerPtr = std::shared_ptr<SetMaxDynamicPower>;

class MaxDynamicPowerContainerData {
  public:
    void set(const ObjectId &current_cell, const SetMaxDynamicPowerPtr &power) { dynamic_power_.emplace(current_cell, power); }

  private:
    std::unordered_map<ObjectId, SetMaxDynamicPowerPtr> dynamic_power_;

  public:
    COMMAND_GET_SET_VAR(dynamic_power, DynamicPower)
};
using MaxDynamicPowerContainerDataPtr = std::shared_ptr<MaxDynamicPowerContainerData>;

class SetMaxLeakagePower {
  public:
    void set(const float &value, const std::string &unit);

  private:
    float power_value_  = 0.0;

  public:
    COMMAND_GET_SET_VAR(power_value, PowerValue)
};
using SetMaxLeakagePowerPtr = std::shared_ptr<SetMaxLeakagePower>;



}
}
#endif //EDI_DB_TIMING_SDC_MULTIVOLTAGE_POWER_COMMANDS_H_
