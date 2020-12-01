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

std::ostream &operator<<(std::ostream &os, SetCaseAnalysis &rhs) {
    switch (rhs.value_) {
        case CaseAnalysisValue::kZero:
            os << "zero";
            break;
        case CaseAnalysisValue::kOne:
            os << "one";
            break;
        case CaseAnalysisValue::kRise:
            os << "rise";
            break;
        case CaseAnalysisValue::kFall:
            os << "fall";
            break;
        defalut:
            os << "unknown";
    }
    return os; 
}

void SetCaseAnalysis::setValue(std::string& input) {
    if (input=="0" or input=="zero") {
        value_ = CaseAnalysisValue::kZero;
    } else if (input=="1" or input=="one") {
        value_ = CaseAnalysisValue::kOne;
    } else if (input=="rising" or input=="rise") {
        value_ = CaseAnalysisValue::kRise;
    } else if (input=="falling" or input=="fall") {
        value_ = CaseAnalysisValue::kFall;
    } else {
        value_ = CaseAnalysisValue::kUnknown;
    }
}

}
}
