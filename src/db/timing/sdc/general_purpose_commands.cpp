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
#include <regex>

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

const std::string SetHierarchySeparator::legal_chars_ = "/@^#.|";
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

void SetUnits::splitUnit(float &value, std::string &suffix, const std::string &target) {
     std::regex re("([0-9]*\\.?[0-9]*)?([a-zA-Z]+)");
     std::smatch sm;
     std::regex_match(target, sm, re);
     if (!sm.ready() or (sm.size()!=3)) {
         //TODO error messages
         return;
     }
     try {
         if (sm[1].str().size()) {
             value = std::stof(sm[1].str());
         }
         suffix = sm[2].str();
     } catch (std::exception &ex) {
         //TODO error messages
     }
}

void SetUnits::setAndCheckCapacitance(const std::string &cap) {
    float value = 0.0;
    std::string suffix = ""; 
    splitUnit(value, suffix, cap);
    capacitance_unit_value_ = value * UnitMultiply::getCapacitiveUnitMultiply(suffix);
    TUnits* units = Object::addr<TUnits>(liberty_units_id_);
    if (units == nullptr) {
        //TODO error messages
        return;
    }
    const auto& lib_cap_unit = units->getCapacitanceUnit();
    float lib_cap_value = (lib_cap_unit.digits)*(lib_cap_unit.scale);
    if (capacitance_unit_value_ != lib_cap_value) {
        //TODO error messages
        return;
    }
}

void SetUnits::setAndCheckResistance(const std::string &res) {
    float value = 0.0;
    std::string suffix = ""; 
    splitUnit(value, suffix, res);
    resistance_unit_value_ = value * UnitMultiply::getCapacitiveUnitMultiply(suffix);
    TUnits* units = Object::addr<TUnits>(liberty_units_id_);
    if (units == nullptr) {
        //TODO error messages
        return;
    }
    const auto& lib_res_unit = units->getResistanceUnit();
    float lib_res_value = (lib_res_unit.digits)*(lib_res_unit.scale);
    if (resistance_unit_value_ != lib_res_value) {
        //TODO error messages
        return;
    }
}

void SetUnits::setAndCheckTime(const std::string &time) {
    float value = 0.0;
    std::string suffix = ""; 
    splitUnit(value, suffix, time);
    time_unit_value_ = value * UnitMultiply::getCapacitiveUnitMultiply(suffix);
    TUnits* units = Object::addr<TUnits>(liberty_units_id_);
    if (units == nullptr) {
        //TODO error messages
        return;
    }
    const auto& lib_time_unit = units->getTimeUnit();
    float lib_time_value = (lib_time_unit.digits)*(lib_time_unit.scale);
    if (time_unit_value_ != lib_time_value) {
        //TODO error messages
        return;
    }
}

void SetUnits::setAndCheckVoltage(const std::string &voltage) {
    float value = 0.0;
    std::string suffix = ""; 
    splitUnit(value, suffix, voltage);
    voltage_unit_value_ = value * UnitMultiply::getCapacitiveUnitMultiply(suffix);
    TUnits* units = Object::addr<TUnits>(liberty_units_id_);
    if (units == nullptr) {
        //TODO error messages
        return;
    }
    //TODO need solo fix the spile issue
    //const auto& lib_vol_unit = units->getVoltageUnit();
    const auto& lib_vol_unit = units->getVoltageUnit();
    float lib_vol_value = (lib_vol_unit.digits)*(lib_vol_unit.scale);
    if (voltage_unit_value_ != lib_vol_value) {
        //TODO error messages
        return;
    }
}

void SetUnits::setAndCheckCurrent(const std::string &current) {
    float value = 0.0;
    std::string suffix = ""; 
    splitUnit(value, suffix, current);
    current_unit_value_ = value * UnitMultiply::getCapacitiveUnitMultiply(suffix);
    TUnits* units = Object::addr<TUnits>(liberty_units_id_);
    if (units == nullptr) {
        //TODO error messages
        return;
    }
    const auto& lib_current_unit = units->getCurrentUnit();
    float lib_current_value = (lib_current_unit.digits)*(lib_current_unit.scale);
    if (current_unit_value_ != lib_current_value) {
        //TODO error messages
        return;
    }
}

void SetUnits::setAndCheckPower(const std::string &power) {
    float value = 0.0;
    std::string suffix = ""; 
    splitUnit(value, suffix, power);
    power_unit_value_ = value * UnitMultiply::getCapacitiveUnitMultiply(suffix);
    TUnits* units = Object::addr<TUnits>(liberty_units_id_);
    if (units == nullptr) {
        //TODO error messages
        return;
    }
    const auto& lib_power_unit = units->getPowerUnit();
    float lib_power_value = (lib_power_unit.digits)*(lib_power_unit.scale);
    if (power_unit_value_ != lib_power_value) {
        //TODO error messages
        return;
    }
}



}
}
