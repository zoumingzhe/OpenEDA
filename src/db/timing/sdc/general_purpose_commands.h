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

#include "db/core/object.h"
#include "db/timing/sdc/command_get_set_property.h"

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
    void setSeparator(const std::string& input);
    const std::string& getSeparator() const { return separator_; }

  private:
    std::string separator_ = "/";
};
using SetHierarchySeparatorPtr = std::shared_ptr<SetHierarchySeparator>; 

class SetUnits {
  public:
    //TODO unit check and set

  private:
    float capacitance_unit_;
    float resistance_unit_;
    float time_unit_;
    float voltage_unit_;
    float current_unit_;
    float power_unit_; 
};
using SetUnitsPtr = std::shared_ptr<SetUnits>;

}
}
#endif //EDI_DB_TIMING_SDC_GENERAL_PURPOSE_COMMANDS_H_
