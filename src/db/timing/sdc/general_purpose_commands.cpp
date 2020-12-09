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
