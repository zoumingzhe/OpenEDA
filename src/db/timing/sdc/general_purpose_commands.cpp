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

const std::string legal_chars_ = "/@^#.|";
void SetHierarchySeparator::setAndCheck(const std::string &input) {
    if (input.size() != 1) {
        //TODO error messages
        return;
    }
    auto found = legal_chars_.find(input); 
    if (found == std::string::npos) {
        //TODO error messages
        return;
    }
    separator_ = input.front();
}

/*
void setAndCheckCapacitance(const std::string &cap) {
    //TODO
    const auto &timing_lib = getTimingLib();
}

void setAndCheckResistance(const std::string &res) {
    //TODO
    const auto &timing_lib = getTimingLib();
}

void setAndCheckTime(const std::string &time) {
    //TODO
    const auto &timing_lib = getTimingLib();
}

void setAndCheckVoltage(const std::string &voltage) {
    //TODO
    const auto &timing_lib = getTimingLib();
}

void setAndCheckCurrent(const std::string &current) {
    //TODO
    const auto &timing_lib = getTimingLib();
}

void setAndCheckPower(const std::string &power) {
    //TODO
    const auto &timing_lib = getTimingLib();
}

*/


}
}
