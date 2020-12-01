/** 
 * @file sdc_data.h
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

#ifndef EDI_DB_TIMING_SDC_SDC_DATA_H_
#define EDI_DB_TIMING_SDC_SDC_DATA_H_

#include "db/timing/sdc/general_purpose_commands.h"
#include "db/timing/sdc/object_access_commands.h"
#include "db/timing/sdc/timing_constraints.h"
#include "db/timing/sdc/environment_commands.h"
#include "db/timing/sdc/multivoltage_power_commands.h"
#include "db/core/db.h"

namespace open_edi {
namespace db {

//general purpose commands
using SdcHierarchySeparator = SetHierarchySeparator;
using SdcHierarchySeparatorPtr = SetHierarchySeparatorPtr;


//object access commands
using SdcCurrentDesign = CurrentDesign;
using SdcCurrentDesignPtr = CurrentDesignPtr;


//timing constraint commands
class SdcClockContainer {
  public:
    void add(const CreateClock& create_clock);
    void add(const CreateGeneratedClock& create_generated_clock);

  private:
    std::unordered_map<ObjectId, CreateClock> pin_clock_map_;
    std::unordered_map<ObjectId, CreateGeneratedClock> pin_generated_clock_map_;
};
using SdcClockContainerPtr = std::shared_ptr<SdcClockContainer>;

//environment commands
using PinCaseAnalysisMap = std::unordered_map<ObjectId, SetCaseAnalysisPtr>;
class SdcCaseAnalysisContainer {
  public:
    friend std::ostream &operator<<(std::ostream &os, SdcCaseAnalysisContainer &rhs);

    void add(const ObjectId &pin_id, const SetCaseAnalysisPtr &case_analysis_ptr);
    const PinCaseAnalysisMap& get() {return pin_to_case_analysis_;}

    CaseAnalysisValue getPinValue(const ObjectId &pin_id);
    void resetPinValue(const ObjectId &pin_id, const CaseAnalysisValue &value);

    bool isPinConstZero(const ObjectId &pin_id);
    bool isPinConstOne(const ObjectId &pin_id);

  private:
    PinCaseAnalysisMap pin_to_case_analysis_;
};
using SdcCaseAnalysisContainerPtr = std::shared_ptr<SdcCaseAnalysisContainer>;

//multivoltage power commands


}
}
#endif //EDI_DB_TIMING_SDC_SDC_DATA_H_ 
