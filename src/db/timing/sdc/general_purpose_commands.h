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

namespace open_edi {
namespace db {

class SetHierarchySeparator {
  public:
    void setSeparator(const std::string& separator) { separator_ = separator; }
    const std::string& getSeparator() const { return separator_; }
    void checkSepartor();

  private:
    std::string separator_ = "/";
};
using SetHierarchySeparatorPtr = std::shared_ptr<SetHierarchySeparator>; 

}
}
#endif //EDI_DB_TIMING_SDC_GENERAL_PURPOSE_COMMANDS_H_
