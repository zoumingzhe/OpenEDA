/** 
 * @file object_access_commands.h
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

#ifndef EDI_DB_TIMING_SDC_OBJECT_ACCESS_COMMANDS_H_
#define EDI_DB_TIMING_SDC_OBJECT_ACCESS_COMMANDS_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "db/core/object.h"

namespace open_edi {
namespace db {

class CurrentDesign {
  public:
    const std::string& get_current_design_name() const;
};
using CurrentDesignPtr = std::shared_ptr<CurrentDesign>; 

}
}

#endif //EDI_DB_TIMING_SDC_OBJECT_ACCESS_COMMANDS_H_
