/** 
* @file sdc.cpp
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

#include "db/timing/sdc/sdc.h"

namespace open_edi {
namespace db {

std::ostream &operator<<(std::ostream &os, SdcCaseAnalysisContainer &rhs) {
    for (auto& pin_case_analysis : rhs.pin_to_case_analysis_) {
        auto& pin_id = pin_case_analysis.first;
        auto& case_analysis = *(pin_case_analysis.second);
        os << "set_case_analysis " << pin_id << " " << case_analysis;
    }
    return os;
}




}
}
