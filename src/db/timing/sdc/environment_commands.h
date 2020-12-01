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
    COMMAND_GET_SET_VAR(value, Value);
};
using SetCaseAnalysisPtr = std::shared_ptr<SetCaseAnalysis>;

}
}
#endif // EDI_DB_TIMING_SDC_ENVIRONMENT_COMMANDS_H_
