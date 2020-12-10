/** 
 * @file general_purpose_commands.h
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

#ifndef EDI_DB_TIMING_SDC_GENERAL_PURPOSE_COMMANDS_H_
#define EDI_DB_TIMING_SDC_GENERAL_PURPOSE_COMMANDS_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <set> 

#include "db/core/object.h"
#include "db/timing/sdc/command_get_set_property.h"
#include "db/timing/timinglib/timinglib_units.h"
#include "db/timing/timinglib/timinglib_libbuilder.h"

namespace open_edi {
namespace db {

class CurrentInstance {
  public:
    void cd(const std::string &dir);

  private:
    ObjectId instance_id_ = UNINIT_OBJECT_ID;

  public:
    COMMAND_GET_SET_VAR(instance_id, InstanceId)
};
using CurrentInstancePtr = std::shared_ptr<CurrentInstance>;

class SetHierarchySeparator {
  public:
    void setAndCheck(const std::string &input);

  private:
    static const std::string legal_chars_ ;
    char separator_ = '/';

  public:
    COMMAND_GET_SET_VAR(separator, Separator)
};
using SetHierarchySeparatorPtr = std::shared_ptr<SetHierarchySeparator>; 

class SetUnits {
  public:
    void setAndCheckCapacitance(const std::string &cap);
    void setAndCheckResistance(const std::string &res);
    void setAndCheckTime(const std::string &time);
    void setAndCheckVoltage(const std::string &voltage);
    void setAndCheckCurrent(const std::string &current);
    void setAndCheckPower(const std::string &power);

  private:
    TUnit capacitance_unit_ = {1.0e-3f, 1, "A"};
    TUnit resistance_unit_ = {1.0f, 1, "ohm"};
    TUnit time_unit_ = {1.0e-9f, 1, "s"};
    TUnit voltage_unit_ = {1.0f, 1, "V"};
    TUnit current_unit_ = {1.0e-3f, 1, "A"};
    TUnit power_unit_ = {1.0f, 1, "W"};

  public:
    COMMAND_GET_SET_VAR(capacitance_unit, CapacitanceUnit)
    COMMAND_GET_SET_VAR(resistance_unit, ResistanceUnit)
    COMMAND_GET_SET_VAR(time_unit, TimeUnit)
    COMMAND_GET_SET_VAR(voltage_unit, VoltageUnit)
    COMMAND_GET_SET_VAR(current_unit, CurrentUnit)
    COMMAND_GET_SET_VAR(power_unit, PowerUnit)
};
using SetUnitsPtr = std::shared_ptr<SetUnits>;





}
}
#endif //EDI_DB_TIMING_SDC_GENERAL_PURPOSE_COMMANDS_H_
