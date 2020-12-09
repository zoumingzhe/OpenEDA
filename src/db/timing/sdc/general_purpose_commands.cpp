/** 
 * @file general_purpose_commands.cpp
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

#include "db/timing/sdc/general_purpose_commands.h"

namespace open_edi {
namespace db {

//current_instance
void CurrentInstance::cd(const std::string &dir) {
    //TODO
    // 1) dir == ""
    // 2) dir == "." --> current
    // 3) dir == ".."
    // 4) dir == "inst name"
    // 5) dir == "../inst name"
    // 6) dir == "../../inst name"
    // 7) dir == "\." --> top
}

//set_hierarchy_separator
void SetHierarchySeparator::setSeparator(const std::string& input) {
    if ((input == "/") or (input == "@") or (input == "^")  or
        (input == "#") or (input == ".") or (input == "|")) {
        separator_ = input;
    }
    //TODO messages
}

}
}
