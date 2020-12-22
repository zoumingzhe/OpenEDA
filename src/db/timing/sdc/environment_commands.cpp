/** 
 * @file environment_commands.cpp
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

#include "db/timing/sdc/environment_commands.h"

namespace open_edi {
namespace db {

std::string to_string(const CaseAnalysisValue &value) {
    switch (value) {
        case CaseAnalysisValue::kZero:
            return "zero";
        case CaseAnalysisValue::kOne:
            return "one";
        case CaseAnalysisValue::kRise:
            return "rise";
        case CaseAnalysisValue::kFall:
            return "fall";
        defalut:
            return "unknown";
    }
    return "unknown";
}

bool SetCaseAnalysis::setValue(std::string& input) {
    if (input=="0" or input=="zero") {
        value_ = CaseAnalysisValue::kZero;
        return true;
    }
    if (input=="1" or input=="one") {
        value_ = CaseAnalysisValue::kOne;
        return true;
    }
    if (input=="rising" or input=="rise") {
        value_ = CaseAnalysisValue::kRise;
        return true;
    }
    if (input=="falling" or input=="fall") {
        value_ = CaseAnalysisValue::kFall;
        return true;
    }
    value_ = CaseAnalysisValue::kUnknown;
    return false;
}

bool CaseAnalysisCotnainerData::add(const std::string &pin_name, const SetCaseAnalysis &case_analysis) {
    const auto &pin = getPinByFullName(pin_name);
    if (!pin) {
        return false;
    }
    const auto &pin_id = pin->getId();
    pin_to_case_analysis_.emplace(pin_id, case_analysis);
}

std::string to_string(const LogicValue &value) {
    switch (value) {
        case LogicValue::kZero:
            return "zero";
        case LogicValue::kOne:
            return "one";
        case LogicValue::kDontCare:
            return "dontcare";
        defalut:
            return "unknown";
    }
    return "unknown";
}

}
}
