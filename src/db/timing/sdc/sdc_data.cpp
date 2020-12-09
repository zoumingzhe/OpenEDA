/** 
 * @file sdc_data.cpp
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

#include "db/timing/sdc/sdc_data.h"

namespace open_edi {
namespace db {

//general purpose commands
std::string SdcCurrentInstanceContainer::getName() {
    const ObjectId &inst_id = current_instance_->getInstanceId();
    return "test";

}

// environment commands
// create_clock
/*
void SdcClockContainer::add(const CreateClock &create_clock) {
    auto& count = clock_ids_.size();
    create_clocks_.emplace(count, create_clock);
    clock_ids_.emplace_back(count+1);
    


}
*/







// set_case_analysis
void SdcCaseAnalysisContainer::add(const ObjectId &pin_id, const SetCaseAnalysisPtr &case_analysis_ptr) {
    assert(case_analysis_ptr);
    pin_to_case_analysis_.emplace(pin_id, case_analysis_ptr);
}

CaseAnalysisValue SdcCaseAnalysisContainer::getPinValue(const ObjectId &pin_id) {
    auto found = pin_to_case_analysis_.find(pin_id); 
    if (found != pin_to_case_analysis_.end()) {
        auto case_analysis_ptr = found->second;
        assert(case_analysis_ptr);
        if (case_analysis_ptr != nullptr) {
            return case_analysis_ptr->getValue(); 
        }
    }
    return CaseAnalysisValue::kUnknown;
}

void SdcCaseAnalysisContainer::resetPinValue(const ObjectId &pin_id, const CaseAnalysisValue &value) {
    auto found = pin_to_case_analysis_.find(pin_id); 
    if (found != pin_to_case_analysis_.end()) {
        auto case_analysis_ptr = found->second;
        assert(case_analysis_ptr);
        if (case_analysis_ptr != nullptr) {
            case_analysis_ptr->setValue(value); 
        }
    }
}

bool SdcCaseAnalysisContainer::isPinConstZero(const ObjectId &pin_id) {
    const auto& value = getPinValue(pin_id); 
    return (value == CaseAnalysisValue::kZero); 
} 

bool SdcCaseAnalysisContainer::isPinConstOne(const ObjectId &pin_id) {
    const auto& value = getPinValue(pin_id); 
    return (value == CaseAnalysisValue::kOne); 
}

std::ostream &operator<<(std::ostream &os, SdcCaseAnalysisContainer &rhs) {
    for ( auto& pin_value : rhs.pin_to_case_analysis_ ) {
        auto& pin_id = pin_value.first;
        Pin* pin_ptr = Object::addr<Pin>(pin_id); 
        assert(pin_ptr);
        if (pin_ptr == nullptr) {
            //TODO messages
            continue;
        }
        auto& case_analysis_ptr = pin_value.second;
        assert(case_analysis_ptr);
        if (case_analysis_ptr == nullptr) {
            //TODO messages
            continue;
        }
        os << "set_case_analysis " << *case_analysis_ptr << " " << pin_ptr->getName() << "\n";
    }
    return os;
}


//
}
}
