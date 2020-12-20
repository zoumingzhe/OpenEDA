/** 
 * @file sdc_common.cpp
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


#include "sdc_common.h"

namespace open_edi {
namespace db {

Pin* getPinByFullName(const std::string &full_name) {
    //TODO
    //Maybe separator is not "/"
    const auto &pos = full_name.find_last_of('/');
    const auto &top_cell = getTopCell();
    if (pos == std::string::npos) { // port
        return top_cell->getIOPin(full_name);
    }
    const auto &inst_name = full_name.substr(0, pos);
    const auto &pin_name = full_name.substr(pos+1);
    const auto &inst = top_cell->getInstance(inst_name);
    if (!inst) {
        return nullptr;
    }
    return inst->getPin(pin_name);
}

std::string getPinFullName(const ObjectId &pin_id) {
    //TODO 
    //Maybe separator is not "/"
    Pin* pin = Object::addr<Pin>(pin_id);
    if (!pin) {
        // error messages
        return "";
    }
    const auto &pin_name = pin->getName();
    const auto &inst = pin->getInst();
    if (!inst) {
        // error messages
        return "";
    }
    const auto &inst_name = inst->getName();
    auto full_name = inst_name + "/" + pin_name;
    return full_name;
}



}
}
