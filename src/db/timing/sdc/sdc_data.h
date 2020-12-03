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
    void add(const ClockId& clock_id, const CreateClock& create_clock);
    void add(const ClockId& clock_id, const CreateGeneratedClock& create_generated_clock);

    const std::vector<Clock>& getClocks() { return clocks_; }
    std::string getClockName(const ClockId id);
    const Clock& getClock(const ClockId id);
    const Clock& getClock(const std::string &name);
    const ClockId getClockId(const std::string &name);

    friend std::ostream &operator<<(std::ostream &os, SdcCaseAnalysisContainer &rhs);

  private:
    std::vector<ClockId> clock_ids_;
    std::vector<std::string> clock_names_;
    std::vector<Clock> clocks_;
    std::unordered_map<ClockId, CreateClock> create_clocks_;
    std::unordered_map<ClockId, CreateGeneratedClock> create_generated_clocks_;
    static const Clock invaild_clock_;
};
using SdcClockContainerPtr = std::shared_ptr<SdcClockContainer>;

class SdcGroupPathContainer {
  public:
    void add(const GroupPath& path) { paths_.emplace_back(path); }
    const std::vector<GroupPath> &get() { return paths_; }

  private:
    std::vector<GroupPath> paths_;
};
using SdcGroupPathContinerPtr = std::shared_ptr<SdcGroupPathContainer>;

class SdcClockGatingCheckContainer {
  public:
    void addToPin(const ObjectId pin_id, const SetClockGatingCheck &check) { pin_clock_gating_check_.emplace(pin_id, check); }
    void addToInstance(const ObjectId inst_id, const SetClockGatingCheck &check) { inst_clock_gating_check_.emplace(pin_id, check); }
    void addToClock(const ObjectId pin_id, const SetClockGatingCheck &check) { clock_to_gating_check_.emplace(pin_id, check); }

  private:
    std::unordered_map<ObjectId, SetClockGatingCheck> pin_clock_gating_check_;
    std::unordered_map<ObjectId, SetClockGatingCheck> inst_clock_gating_check_;
    std::unordered_map<ClockId, SetClockGatingCheck> clock_to_gating_check_; 
};
using SdcClockGatingCheckContainerPtr = std::shared_ptr<SdcClockGatingCheckContainer>;

//TODO change the data structure
class SdcClockGroupsContainer {
  public:
    void add(const SetClocKGroups &groups);

  private:
    std::vector<ClockGroupId> clock_groups_;
    std::unordered_map<ClockId, std::vector<ClockGroupId> > clock_in_groups_;
    std::unordered_map<ClockGroupId, ClockGroupId> physically_exclusive_groups_;
};
using SdcClockGroupsContainerPtr = std::shared_ptr<SdcClockGroupsContainer>; 

class SdcClockLatencyContainer {
  public:
    void add(const SetClockLatency &latency);

  private:
    std::unordered_map<ObjectId, std::vector<SetClockLatency> > pin_clock_latency_;
    std::unordered_map<ClockId, std::vector<SetClockLatency> > clock_latency_;
}
using SdcClockLatencyContainerPtr = std::shared_ptr<SdcClockLatencyContainer>;

class SdcSenceContainer {

  private:
    std::unordered_map<ClockPinPairPtr, SetSense> clock_pin_sense_; 
};
using SdcSenceContainerPtr = std::shared_ptr<SdcSenceContainer>;

class SdcClockTransitionContainer {

  private:
    std::unordered_map<ClockId, SetClockTransitionPtr> clock_transitions_;
};
using SdcClockTransitionContainerPtr = std::shared_ptr<SdcClockTransitionContainer>;

class SdcClockUncertaintyContainer {

  private:
    std::unordered_map<ClockPair, SetClockUncertaintyPtr> clock_to_clock_uncertainty_;
    std::unordered_map<ObjectId, SetClockUncertaintyPtr> pin_to_uncertainty_;
};
using SdcClockUncertaintyContainerPtr = std::shared_ptr<SdcClockUncertaintyContainer>;

class SdcDataCheckContainer {

  private:
    std::unordered_map<PinPairPtr, SetDataCheckPtr> pin_to_pin_data_check_;
};
using SdcDataCheckContainerPtr = std::shared_ptr<SdcDataCheckContainer>;

class SdcDisableTimingContinaer {
  public:
    void add(const SetDiableTiming &disable_timing);

  private:
    std::unordered_map<ObjectId, ObjectId> pin_to_pin_;
    std::unordered_map<ObjectId, ObjectId> term_to_term_;
};
using SdcDisableTimingContainerPtr = std::shared_ptr<SdcDisableTimingContinaer>;

class SdcFalsePathContainer {
  public:
    void add(const SetFalsePath& path);
  private:
    std::vector<SetFalsePathPtr> false_paths_;
};
using SdcFalsePathContainerPtr = std::shared_ptr<SdcFalsePathContainer>; 





//environment commands
using PinCaseAnalysisMap = std::unordered_map<ObjectId, SetCaseAnalysisPtr>;
class SdcCaseAnalysisContainer {
  public:
    void add(const ObjectId &pin_id, const SetCaseAnalysisPtr &case_analysis_ptr);
    const PinCaseAnalysisMap& get() {return pin_to_case_analysis_;}

    CaseAnalysisValue getPinValue(const ObjectId &pin_id);
    void resetPinValue(const ObjectId &pin_id, const CaseAnalysisValue &value);

    bool isPinConstZero(const ObjectId &pin_id);
    bool isPinConstOne(const ObjectId &pin_id);

    friend std::ostream &operator<<(std::ostream &os, SdcCaseAnalysisContainer &rhs);

  private:
    PinCaseAnalysisMap pin_to_case_analysis_;
};
using SdcCaseAnalysisContainerPtr = std::shared_ptr<SdcCaseAnalysisContainer>;

;//multivoltage power commands


}
}
#endif //EDI_DB_TIMING_SDC_SDC_DATA_H_ 
