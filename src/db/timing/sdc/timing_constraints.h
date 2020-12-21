/** 
 * @file timing_constraints.h
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

#ifndef EDI_DB_TIMING_SDC_TIMING_CONSTRAINTS_H_
#define EDI_DB_TIMING_SDC_TIMING_CONSTRAINTS_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>

#include "db/core/object.h"
#include "db/timing/sdc/clock.h"
#include "db/timing/sdc/command_get_set_property.h"
#include "util/data_traits.h"
#include <boost/functional/hash.hpp>
#include "db/timing/sdc/sdc_common.h"

namespace open_edi {
namespace db {

class CreateClock {
  private:
    std::string comment_ = "";

  public:
    COMMAND_GET_SET_VAR(comment, Comment)
};
using CreateClockPtr = std::shared_ptr<CreateClock>;

class CreateGeneratedClock {
  public:
    void addEdgeShift(const float& edge_shift) { edge_shifts_.emplace_back(edge_shift); }
    void addEdge(const int edge) { edges_.emplace_back(edge); }
    bool addSourceMasterPin(const std::string &pin_name);

  private:
    std::string comment_ = "";
    std::vector<float> edge_shifts_;
    std::vector<ObjectId> source_master_pins_;
    std::vector<int> edges_;
    float duty_cycle_ = 0.0;
    ClockId master_clock_;
    int divided_by_ = 0;
    int multiply_by_ = 0;

  public:
    COMMAND_GET_SET_VAR(comment, Comment)
    COMMAND_GET_SET_VAR(edge_shifts, EdgeShifts)
    COMMAND_GET_SET_VAR(source_master_pins, SourceMasterPins)
    COMMAND_GET_SET_VAR(edges, Edges)
    COMMAND_GET_SET_VAR(duty_cycle, DutyCycle)
    COMMAND_GET_SET_VAR(master_clock, MasterClock)
    COMMAND_GET_SET_VAR(divided_by, DividedBy)
    COMMAND_GET_SET_VAR(multiply_by, MultiplyBy)
    COMMAND_GET_SET_FLAG(invert_, Invert)
    COMMAND_GET_SET_FLAG(combinational_, Combinational)
};
using CreateGeneratedClockPtr = std::shared_ptr<CreateGeneratedClock>;

class ClockContainerData {
  public:
    const size_t getClockNum() const { return clock_ids_.size(); }
    void addClock(ClockPtr &clock, CreateClockPtr &create_clock);
    void addClock(ClockPtr &clock, CreateGeneratedClockPtr &create_generated_clock);
    bool addClockPin(const std::string &pin_name, const ClockPtr &clock);

  private:
    std::vector<ClockId> clock_ids_;
    std::vector<std::string> clock_names_;
    std::vector<ClockPtr> clocks_;
    std::unordered_map<std::string, ClockId> name_to_id_;
    std::unordered_map<ClockId, CreateClockPtr> create_clocks_;
    std::unordered_map<ClockId, CreateGeneratedClockPtr> create_generated_clocks_;
    std::unordered_multimap<ObjectId, ClockPtr> pin_clock_map_;

  public:
    COMMAND_GET_SET_VAR(clock_ids, ClockIds)
    COMMAND_GET_SET_VAR(clock_names, ClockNames)
    COMMAND_GET_SET_VAR(clocks, Clocks)
    COMMAND_GET_SET_VAR(name_to_id, NameToId)
    COMMAND_GET_SET_VAR(create_clocks, CreateClocks)
    COMMAND_GET_SET_VAR(create_generated_clocks, CreateGeneratedClocks)
    COMMAND_GET_SET_VAR(pin_clock_map, PinClockMap)
};
using ClockContainerDataPtr = std::shared_ptr<ClockContainerData>;

class PathNodes {
  public:
    void addPin(const ObjectId pin_id) { pins_.emplace_back(pin_id); }
    void addInst(const ObjectId inst_id) { insts_.emplace_back(inst_id); }
    void addClock(const ClockId clock_id) { clocks_.emplace_back(clock_id); }

  private:
    std::vector<ObjectId> pins_;
    std::vector<ObjectId> insts_;
    std::vector<ClockId> clocks_;

  public:
    COMMAND_GET_SET_VAR(pins, Pins)
    COMMAND_GET_SET_VAR(insts, Insts)
    COMMAND_GET_SET_VAR(clocks, Clocks)
    COMMAND_GET_SET_FLAG(rise_, Rise)
    COMMAND_GET_SET_FLAG(fall_, Fall)
};
using PathNodesPtr = std::shared_ptr<PathNodes>;

class PathThroughNodes : public PathNodes {
  public:
    void addNet(const ObjectId net_id) { nets_.emplace_back(net_id); };

  private:
    std::vector<ObjectId> nets_;

  public:
    COMMAND_GET_SET_VAR(nets, Nets)
};
using PathThroughNodesPtr = std::shared_ptr<PathThroughNodes>;

class ExceptionPath {
  public:
    ExceptionPath();

  private:
    PathNodesPtr from_;
    PathNodesPtr to_;
    PathThroughNodesPtr through_;

  public:
    COMMAND_GET_SET_VAR(from, From)
    COMMAND_GET_SET_VAR(to, To)
    COMMAND_GET_SET_VAR(through, Through)
};
using ExceptionPathPtr = std::shared_ptr<ExceptionPath>;

class GroupPath {
  public:
    GroupPath();

    bool addFromPinNode(const std::string &pin_name);
    bool addFromInstNode(const std::string &inst_name);
    void addFromClockNode(const ClockId &clock_id);
    void setRiseFrom();
    void setFallFrom();
    void setRiseFallFrom();

    bool addToPinNode(const std::string &pin_name);
    bool addToInstNode(const std::string &inst_name);
    void addToClockNode(const ClockId &clock_id);
    void setRiseTo();
    void setFallTo();
    void setRiseFallTo();

    bool addThroughPinNode(const std::string &pin_name);
    bool addThroughInstNode(const std::string &inst_name);
    void addThroughClockNode(const ClockId &clock_id);
    void setRiseThrough();
    void setFallThrough();
    void setRiseFallThrough();

  private:
    std::string comment_ = "";
    std::string name_ = "";
    float weight_ = 0.0;
    PathNodesPtr from_;
    PathNodesPtr to_;
    PathThroughNodesPtr through_;

  public:
    COMMAND_GET_SET_VAR(comment, Comment)
    COMMAND_GET_SET_VAR(name, Name)
    COMMAND_GET_SET_VAR(weight, Weight)
    COMMAND_GET_SET_VAR(from, From)
    COMMAND_GET_SET_VAR(to, To)
    COMMAND_GET_SET_VAR(through, Through)
    COMMAND_GET_SET_FLAG(default_value_, DefaultValue)
};
using GroupPathPtr = std::shared_ptr<GroupPath>; 

class GroupPathContainerData {
  public:
    void add(const GroupPathPtr &path) { group_paths_.emplace_back(path); }

  private:
    std::vector<GroupPathPtr> group_paths_;

  public:
    COMMAND_GET_SET_VAR(group_paths, GroupPaths)
};
using GroupPathContainerDataPtr = std::shared_ptr<GroupPathContainerData>;

class SetClockGatingCheck {
  public:
    void checkFlags();

  private:
    float setup_ = 0.0;
    float hold_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(setup, Setup)
    COMMAND_GET_SET_VAR(hold, Hold)
    COMMAND_GET_SET_FLAG(rise_, Rise)
    COMMAND_GET_SET_FLAG(fall_, Fall)
    COMMAND_GET_SET_FLAG(high_, High)
    COMMAND_GET_SET_FLAG(low_, Low)
};
using SetClockGatingCheckPtr = std::shared_ptr<SetClockGatingCheck>;

class ClockGatingCheckContainerData {
  public:
    bool addToPin(const std::string &pin_name, const SetClockGatingCheckPtr &check);
    bool addToInst(const std::string &inst_name, const SetClockGatingCheckPtr &check); 
    void addToClock(const ClockId &clock_id, const SetClockGatingCheckPtr &check) { clock_to_check_.emplace(clock_id, check); }
    void addToCurrentDesign(const ObjectId &cell_id, const SetClockGatingCheckPtr &check) { design_to_check_.emplace(cell_id, check); }

  private:
    std::unordered_multimap<ObjectId, SetClockGatingCheckPtr> pin_to_check_;
    std::unordered_multimap<ObjectId, SetClockGatingCheckPtr> inst_to_check_;
    std::unordered_multimap<ClockId, SetClockGatingCheckPtr> clock_to_check_; 
    std::unordered_map<ObjectId, SetClockGatingCheckPtr> design_to_check_;

  public:
    COMMAND_GET_SET_VAR(pin_to_check, PinToCheck)
    COMMAND_GET_SET_VAR(inst_to_check, InstToCheck)
    COMMAND_GET_SET_VAR(clock_to_check, ClockToCheck)
    COMMAND_GET_SET_VAR(design_to_check, DesignToCheck)
};
using ClockGatingCheckContainerDataPtr = std::shared_ptr<ClockGatingCheckContainerData>;

using ClockGroupId = open_edi::util::UInt32;
static const ClockGroupId kInvalidClockGroupId = -1;
enum class GroupRelationshipType : Bits8 {kAsynchronous=0, kLogicallyExclusive, kPhysicallyExclusive, kUnknown};
class ClockGroup {
  public:
    void addClock(const ClockId& clock_id) { clock_ids_.emplace_back(clock_id); }
    void sort() { std::sort(clock_ids_.begin(), clock_ids_.end()); }

  private:
    std::vector<ClockId> clock_ids_; //need to sort after setup
    ClockGroupId id_ = kInvalidClockGroupId;
    GroupRelationshipType type_ = GroupRelationshipType::kUnknown;

  public:
    COMMAND_GET_SET_VAR(clock_ids, ClockIds)
    COMMAND_GET_SET_VAR(id, Id)
    COMMAND_GET_SET_VAR(type, Type)
    COMMAND_GET_SET_FLAG(allow_paths_, AllowPaths)
    COMMAND_GET_SET_FLAG(link_to_right_, LinkToRight)
    COMMAND_GET_SET_FLAG(link_to_left_, LinkToLeft)
};
using ClockGroupPtr = std::shared_ptr<ClockGroup>;

class SetClockGroups {
  private:
    std::string name_ = "";
    std::string comment_ = "";
    std::vector<ClockGroupPtr> groups_;

  public:
    COMMAND_GET_SET_VAR(name, Name)
    COMMAND_GET_SET_VAR(comment, Comment)
    COMMAND_GET_SET_VAR(groups, Groups)
};
using SetClockGroupsPtr = std::shared_ptr<SetClockGroups>;

class ClockGroupsContainerData {
  public:
    void add(const ClockId &clock_id, const ClockGroupPtr &group) { clock_to_group_.emplace(clock_id, group); } 
    size_t getClockGroupNum() const { return group_ids_.size(); } 

  private:
    std::unordered_multimap<ClockId, ClockGroupPtr> clock_to_group_;
    std::unordered_map<ClockGroupId, SetClockGroupsPtr> set_clock_groups_;
    std::vector<ClockGroupId> group_ids_;
    std::vector<ClockGroupPtr> groups_;
 
   public:
    COMMAND_GET_SET_VAR(clock_to_group, ClockToGroup)
    COMMAND_GET_SET_VAR(set_clock_groups, SetClockGroups)
    COMMAND_GET_SET_VAR(group_ids, GroupIds)
    COMMAND_GET_SET_VAR(groups, Groups)
};
using ClockGroupsContainerDataPtr = std::shared_ptr<ClockGroupsContainerData>;

class SetClockLatency {
  private:
    float delay_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(delay, Delay)
    COMMAND_GET_SET_FLAG(rise_, Rise)
    COMMAND_GET_SET_FLAG(fall_, Fall)
    COMMAND_GET_SET_FLAG(min_, Min)
    COMMAND_GET_SET_FLAG(max_, Max)
    COMMAND_GET_SET_FLAG(dynamic_, Dynamic)
    COMMAND_GET_SET_FLAG(source_, Source)
    COMMAND_GET_SET_FLAG(early_, Early)
    COMMAND_GET_SET_FLAG(late_, Late)
};
using SetClockLatencyPtr = std::shared_ptr<SetClockLatency>;

class ClockLatencyOnPin : public SetClockLatency {
  private:
    std::vector<ClockId> pin_associated_clocks_;

  public:
    COMMAND_GET_SET_VAR(pin_associated_clocks, PinAssociatedClocks)
};
using ClockLatencyOnPinPtr = std::shared_ptr<ClockLatencyOnPin>;

class ClockLatencyContainerData {
  public:
    void addToPin(const ObjectId &pin_id, const ClockLatencyOnPinPtr &latency) { pin_to_latency_.emplace(pin_id, latency); }
    void addToClock(const ObjectId &clock_id, const SetClockLatencyPtr &latency) { clock_to_latency_.emplace(clock_id, latency); }

  private:
    std::unordered_multimap<ObjectId, ClockLatencyOnPinPtr> pin_to_latency_;
    std::unordered_multimap<ClockId, SetClockLatencyPtr> clock_to_latency_;

  public:
    COMMAND_GET_SET_VAR(pin_to_latency, PinToLatency);
    COMMAND_GET_SET_VAR(clock_to_latency, ClockToLatency);
};
using ClockLatencyContainerDataPtr = std::shared_ptr<ClockLatencyContainerData>;

enum class DataType : Bits8 { kClock=0, kData, kUnknown };
enum class PulseType : Bits8 { kRiseTriggeredHighPulse=0, kRiseTriggeredLowPulse, kFallTriggeredHighPulse, kFallTriggeredLowPulse, kUnknown }; 
class SetSense {
  private:
    DataType type_ = DataType::kUnknown;
    PulseType pulse_ = PulseType::kUnknown;  
    std::vector<ClockId> clocks_;

  public:
    COMMAND_GET_SET_VAR(type, Type)
    COMMAND_GET_SET_VAR(pulse, Pulse)
    COMMAND_GET_SET_VAR(clocks, Clocks)
    COMMAND_GET_SET_FLAG(non_unate_, NonUnate)
    COMMAND_GET_SET_FLAG(positive_, Positive)
    COMMAND_GET_SET_FLAG(negative_, Negative)
    COMMAND_GET_SET_FLAG(clock_leaf_, ClockLeaf)
    COMMAND_GET_SET_FLAG(stop_propagation_, StopPropation)
};
using SetSensePtr = std::shared_ptr<SetSense>;

class SenseContainerData {
  public:
    void add(const ObjectId &pin_id, const SetSensePtr &sense) { pin_sense_.emplace(pin_id, sense); }

  private:
    std::unordered_map<ObjectId, SetSensePtr> pin_sense_; 

  public:
    COMMAND_GET_SET_VAR(pin_sense, PinSense)
};
using SenseContainerDataPtr = std::shared_ptr<SenseContainerData>;

class SetClockTransition {
  private:
    float transition_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(transition, Transition)
    COMMAND_GET_SET_FLAG(rise_, Rise)
    COMMAND_GET_SET_FLAG(fall_, Fall)
    COMMAND_GET_SET_FLAG(min_, Min)
    COMMAND_GET_SET_FLAG(max_, Max)
};
using SetClockTransitionPtr = std::shared_ptr<SetClockTransition>;

class ClockTransitionContainerData {
  public:
    void add(const ClockId &clock_id, const SetClockTransitionPtr &transition) { clock_transitions_.emplace(clock_id, transition); }

  private:
    std::unordered_map<ClockId, SetClockTransitionPtr> clock_transitions_;

  public:
    COMMAND_GET_SET_VAR(clock_transitions, ClockTransitions)
};
using ClockTransitionContainerDataPtr = std::shared_ptr<ClockTransitionContainerData>;

class SetClockUncertainty {
  private:
    float uncertainty_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(uncertainty, Uncertainty)
    COMMAND_GET_SET_FLAG(setup_, Setup)
    COMMAND_GET_SET_FLAG(hold_, Hold)
};
using SetClockUncertaintyPtr = std::shared_ptr<SetClockUncertainty>;

class InterClockUncertainty : SetClockUncertainty {
  public:
    COMMAND_GET_SET_FLAG(rise_from_, RiseFrom)
    COMMAND_GET_SET_FLAG(fall_from_, FallFrom)
    COMMAND_GET_SET_FLAG(rise_to_, RiseTo)
    COMMAND_GET_SET_FLAG(fall_to_, FallTo)
};
using InterClockUncertaintyPtr = std::shared_ptr<InterClockUncertainty>;

using InterClockPair = std::pair<ClockId, ClockId>;
using InterClockPairPtr = std::shared_ptr<InterClockPair>;
class ClockUncertaintyContainerData {
    void addToInterClock(const InterClockPair &clocks, const InterClockUncertaintyPtr &uncertainty) { interclock_uncertainty_.emplace(clocks, uncertainty); }
    void addToPin(const ObjectId &pin_id, const SetClockUncertaintyPtr &uncertainty) { pin_uncertainty_.emplace(pin_id, uncertainty); }
    void addToClock(const ClockId &clock_id, const SetClockUncertaintyPtr &uncertainty) { clock_uncertainty_.emplace(clock_id, uncertainty); }

  private:
    //TODO multi map? move the rise/fall to key or use multi map
    std::unordered_map<InterClockPair, InterClockUncertaintyPtr, boost::hash<std::pair<ClockId, ClockId> > > interclock_uncertainty_;
    std::unordered_map<ObjectId, SetClockUncertaintyPtr> pin_uncertainty_;
    std::unordered_map<ClockId, SetClockUncertaintyPtr> clock_uncertainty_;

  public:
    COMMAND_GET_SET_VAR(interclock_uncertainty, InterClockUncertainty)
    COMMAND_GET_SET_VAR(pin_uncertainty, PinUncertainty)
    COMMAND_GET_SET_VAR(clock_uncertainty, ClockUncertainty)
};
using ClockUncertaintyContainerDataPtr = std::shared_ptr<ClockUncertaintyContainerData>;

class SetDataCheck {
  public:
    void addClock(const ClockId &id) { clocks_.emplace_back(id); };

  private:
    float value_ = 0.0;
    std::vector<ClockId> clocks_;

  public:
    COMMAND_GET_SET_VAR(value, Value)
    COMMAND_GET_SET_VAR(clocks, Clocks)
    COMMAND_GET_SET_FLAG(rise_from_, RiseFrom)
    COMMAND_GET_SET_FLAG(fall_from_, FallFrom)
    COMMAND_GET_SET_FLAG(rise_to_, RiseTo)
    COMMAND_GET_SET_FLAG(fall_to_, FallTo)
    COMMAND_GET_SET_FLAG(setup_, Setup)
    COMMAND_GET_SET_FLAG(hold_, Hold)
};
using SetDataCheckPtr = std::shared_ptr<SetDataCheck>;

using InterPinPair = std::pair<ObjectId, ObjectId>;
using InterPinPairPtr = std::shared_ptr<InterPinPair>;
class DataCheckContainerData {
  public:
    void add(const InterPinPair &pins, const SetDataCheckPtr &check) { interpin_data_check_.emplace(pins, check); };

  private:
    //TODO multi map? move the rise/fall to key or use multi map
    std::unordered_map<InterPinPair, SetDataCheckPtr, boost::hash<std::pair<ObjectId, ObjectId> > > interpin_data_check_;

  public:
    COMMAND_GET_SET_VAR(interpin_data_check, InterPinDataCheck)
};
using DataCheckContainerDataPtr = std::shared_ptr<DataCheckContainerData>;

using CellArc = std::pair<ObjectId, ObjectId>;
using CellArcPtr = std::shared_ptr<CellArc>;
class SetDisableTiming {
  private:
    ObjectId from_id_; //for cell_disable_arcs_, it is term; for inst_disable_arcs_, it is pin
    ObjectId to_id_; // for cell_disable_arcs_, it is term; for inst_disable_arcs_, it is pin

  public:
    COMMAND_GET_SET_VAR(from_id, FromId)
    COMMAND_GET_SET_VAR(to_id, ToId)
    COMMAND_GET_SET_FLAG(set_all_arcs_, SetAllArcs)
};
using SetDisableTimingPtr = std::shared_ptr<SetDisableTiming>;

class DisableTimingContainerData {
  public:
    void addToCell(const ObjectId &cell_id, const SetDisableTimingPtr &disable_arcs) { cell_disable_arcs_.emplace(cell_id, disable_arcs); }
    void addToInst(const ObjectId &inst_id, const SetDisableTimingPtr &disable_arcs) { inst_disable_arcs_.emplace(inst_id, disable_arcs); }

  private:
    std::unordered_map<ObjectId, SetDisableTimingPtr> cell_disable_arcs_;
    std::unordered_map<ObjectId, SetDisableTimingPtr> inst_disable_arcs_;

  public:
    COMMAND_GET_SET_VAR(cell_disable_arcs, CellDisableArcs)
    COMMAND_GET_SET_VAR(inst_disable_arcs, InstDisableArcs)
};
using DisableTimingContainerDataPtr = std::shared_ptr<DisableTimingContainerData>;

class SetFalsePath : public ExceptionPath {
  private:
    std::string comment_ = ""; 

  public:
    COMMAND_GET_SET_VAR(comment, Comment)
    COMMAND_GET_SET_FLAG(setup_, Setup)
    COMMAND_GET_SET_FLAG(hold_, Hold)
    COMMAND_GET_SET_FLAG(rise_, Rise)
    COMMAND_GET_SET_FLAG(fall_, Fall)
}; 
using SetFalsePathPtr = std::shared_ptr<SetFalsePath>;

class FalsePathContainerData {
  public:
    void add(const SetFalsePathPtr &path) { false_paths_.emplace_back(path); };

  private:
    std::vector<SetFalsePathPtr> false_paths_;

  public:
    COMMAND_GET_SET_VAR(false_paths, FalsePaths);
};
using FalsePathContainerDataPtr = std::shared_ptr<FalsePathContainerData>;

class SetIdealLatency {
  private:
    float delay_;

  public:
    COMMAND_GET_SET_VAR(delay, Delay)
    COMMAND_GET_SET_FLAG(rise_, Rise)
    COMMAND_GET_SET_FLAG(fall_, Fall)
    COMMAND_GET_SET_FLAG(min_, Min)
    COMMAND_GET_SET_FLAG(max_, Max)
};
using SetIdealLatencyPtr = std::shared_ptr<SetIdealLatency>;

class IdealLatencyContainerData {
  public:
    void add(const ObjectId &pin_id, const SetIdealLatencyPtr &latency) { pin_to_latency_.emplace(pin_id, latency); }

  private:
    std::unordered_map<ObjectId, SetIdealLatencyPtr> pin_to_latency_;

  public:
    COMMAND_GET_SET_VAR(pin_to_latency, PinToLatency)
};
using IdealLatencyContainerDataPtr = std::shared_ptr<IdealLatencyContainerData>;

class SetIdealNetwork {
  public:
    void addPin(ObjectId &pin_id) { pins_.emplace_back(pin_id); };
    void addNet(ObjectId &net_id) { nets_.emplace_back(net_id); };

  private:
    std::vector<ObjectId> pins_;
    std::vector<ObjectId> nets_;

  public:
    COMMAND_GET_SET_VAR(pins, Pins)
    COMMAND_GET_SET_VAR(nets, Nets)
    COMMAND_GET_SET_FLAG(no_propagate_, NoPropagate)
};
using SetIdealNetworkPtr = std::shared_ptr<SetIdealNetwork>;

class SetIdealTransition {
  private:
    float value_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(value, Value)
    COMMAND_GET_SET_FLAG(rise_, Rise)
    COMMAND_GET_SET_FLAG(fall_, Fall)
    COMMAND_GET_SET_FLAG(min_, Min)
    COMMAND_GET_SET_FLAG(max_, Max)
};
using SetIdealTransitionPtr = std::shared_ptr<SetIdealTransition>;

class IdealTransitionContainerData {
  public:
    void add(const ObjectId &pin_id, const SetIdealTransitionPtr &transition) { pin_ideal_transitions_.emplace(pin_id, transition); };

  private:
    std::unordered_map<ObjectId, SetIdealTransitionPtr> pin_ideal_transitions_;

  public:
    COMMAND_GET_SET_VAR(pin_ideal_transitions, PinIdealTransitions)
};
using IdealTransitionContainerDataPtr = std::shared_ptr<IdealTransitionContainerData>;

class PortDelay {
  private:
    float delay_value_ = 0.0;
    std::vector<ClockId> clocks_;
    std::vector<ObjectId> reference_pins_;

  public:
    COMMAND_GET_SET_VAR(delay_value, DelayValue)
    COMMAND_GET_SET_VAR(clocks, Clocks)
    COMMAND_GET_SET_VAR(reference_pins, ReferencePins)
    COMMAND_GET_SET_FLAG(clock_fall_, ClockFall)
    COMMAND_GET_SET_FLAG(level_sensitive_, LevelSensitive)
    COMMAND_GET_SET_FLAG(rise_, Rise)
    COMMAND_GET_SET_FLAG(fall_, Fall)
    COMMAND_GET_SET_FLAG(min_, Min)
    COMMAND_GET_SET_FLAG(max_, Max)
    COMMAND_GET_SET_FLAG(add_delay_, AddDelay)
    COMMAND_GET_SET_FLAG(network_latency_included_, NetworkLatencyIncluded)
    COMMAND_GET_SET_FLAG(source_latency_included_, SourceLatencyIncluded)
};
using PortDelayPtr = std::shared_ptr<PortDelay>;

class SetInputDelay : public PortDelay {

};
using SetInputDelayPtr = std::shared_ptr<SetInputDelay>;

class InputDelayContainerData {
  public:
    void add(const ObjectId &pin_id, const SetInputDelayPtr &delay) { pin_input_delays_.emplace(pin_id, delay); }

  private:
    //TODO need ref pin to input delay map?
    std::unordered_map<ObjectId, SetInputDelayPtr> pin_input_delays_;

  public:
    COMMAND_GET_SET_VAR(pin_input_delays, PinInputDelays)
};
using InputDelayContainerDataPtr = std::shared_ptr<InputDelayContainerData>;

class SetMaxDelay : public ExceptionPath {
  private:
    float delay_value_ = 0.0;
    std::string comment_ = "";

  public:
    COMMAND_GET_SET_VAR(delay_value, DelayValue)
    COMMAND_GET_SET_VAR(comment, Comment)
    COMMAND_GET_SET_FLAG(rise_, Rise)
    COMMAND_GET_SET_FLAG(fall_, Fall)
    COMMAND_GET_SET_FLAG(ignore_clock_latency_, IgnoreClockLatency)
};
using SetMaxDelayPtr = std::shared_ptr<SetMaxDelay>;

class MaxDelayContainerData {
  public:
    void add(const SetMaxDelayPtr &delay) { max_delays_.emplace_back(delay); } 

  private:
    std::vector<SetMaxDelayPtr> max_delays_;

  public:
    COMMAND_GET_SET_VAR(max_delays, MaxDelays)
};
using MaxDelayContainerDataPtr = std::shared_ptr<MaxDelayContainerData>;

class SetMaxTimeBorrow {
  private:
    float value_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(value, Value)
};
using SetMaxTimeBorrowPtr = std::shared_ptr<SetMaxTimeBorrow>;

class MaxTimeBorrowContainerData {
  public:
    void addToPin(const ObjectId &pin_id, const SetMaxTimeBorrowPtr &time_borrow) { pin_max_time_borrow_.emplace(pin_id, time_borrow); }
    void addToInst(const ObjectId &inst_id, const SetMaxTimeBorrowPtr &time_borrow) { inst_max_time_borrow_.emplace(inst_id, time_borrow); }
    void addToClock(const ClockId &clock_id, const SetMaxTimeBorrowPtr &time_borrow) { clock_max_time_borrow_.emplace(clock_id, time_borrow); }

  private:
    std::unordered_map<ObjectId, SetMaxTimeBorrowPtr> pin_max_time_borrow_;
    std::unordered_map<ObjectId, SetMaxTimeBorrowPtr> inst_max_time_borrow_;
    std::unordered_map<ClockId, SetMaxTimeBorrowPtr> clock_max_time_borrow_;

  public:
    COMMAND_GET_SET_VAR(pin_max_time_borrow, PinMaxTimeBorrow)
    COMMAND_GET_SET_VAR(inst_max_time_borrow, InstMaxTimeBorrow)
    COMMAND_GET_SET_VAR(clock_max_time_borrow, ClockMaxTimeBorrow)
};
using MaxTimeBorrowContainerDataPtr = std::shared_ptr<MaxTimeBorrowContainerData>;

class SetMinDelay : public ExceptionPath {
  private:
    float delay_value_ = 0.0;
    std::string comment_ = "";

  public:
    COMMAND_GET_SET_VAR(delay_value, DelayValue)
    COMMAND_GET_SET_VAR(comment, Comment)
    COMMAND_GET_SET_FLAG(rise_, Rise)
    COMMAND_GET_SET_FLAG(fall_, Fall)
    COMMAND_GET_SET_FLAG(ignore_clock_latency_, IgnoreClockLatency)
};
using SetMinDelayPtr = std::shared_ptr<SetMinDelay>;

class MinDelayContainerData {
  public:
    void add(const SetMinDelayPtr &delay) { min_delays_.emplace_back(delay); } 

  private:
    std::vector<SetMinDelayPtr> min_delays_;

  public:
    COMMAND_GET_SET_VAR(min_delays, MinDelays)
};
using MinDelayContainerDataPtr = std::shared_ptr<MinDelayContainerData>;

class SetMinPulseWidth {
  private:
    float value_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(value, Value)
    COMMAND_GET_SET_FLAG(low_, Low)
    COMMAND_GET_SET_FLAG(high_, High)
};
using SetMinPulseWidthPtr = std::shared_ptr<SetMinPulseWidth>;

class MinPulseWidthData {
  private:
    void addToPin(const ObjectId &pin_id, const SetMinPulseWidthPtr &pulse_width) { pin_min_pulse_width_.emplace(pin_id, pulse_width); } 
    void addToInst(const ObjectId &inst_id, const SetMinPulseWidthPtr &pulse_width) { inst_min_pulse_width_.emplace(inst_id, pulse_width); }
    void addToClock(const ClockId &clock_id, const SetMinPulseWidthPtr &pulse_width) { clock_min_pulse_width_.emplace(clock_id, pulse_width); }
    void addToCurrentDesign(const ObjectId &cell_id, const SetMinPulseWidthPtr &pulse_width) { cell_min_pulse_width_.emplace(cell_id, pulse_width); }

  private:
    std::unordered_map<ObjectId, SetMinPulseWidthPtr> pin_min_pulse_width_;
    std::unordered_map<ObjectId, SetMinPulseWidthPtr> inst_min_pulse_width_;
    std::unordered_map<ClockId, SetMinPulseWidthPtr> clock_min_pulse_width_;
    std::unordered_map<ObjectId, SetMinPulseWidthPtr> cell_min_pulse_width_; //no object specified, set to all the inst in current design

  public:
    COMMAND_GET_SET_VAR(pin_min_pulse_width, PinMinPulseWidth)
    COMMAND_GET_SET_VAR(inst_min_pulse_width, InstMinPulseWidth)
    COMMAND_GET_SET_VAR(clock_min_pulse_width, ClockMinPulseWidth)
    COMMAND_GET_SET_VAR(cell_min_pulse_width, CellMinPulseWidth)
};
using MinPulseWidthDataPtr = std::shared_ptr<MinPulseWidthData>;

class SetMulticyclePath : public ExceptionPath {
  private:
    std::string comment_ = "";
    UInt32 path_multiplier = 0;

  public:
    COMMAND_GET_SET_VAR(comment, Comment)
    COMMAND_GET_SET_FLAG(setup_, Setup)
    COMMAND_GET_SET_FLAG(hold_, Hold)
    COMMAND_GET_SET_FLAG(rise_, Rise)
    COMMAND_GET_SET_FLAG(fall_, Fall)
    COMMAND_GET_SET_FLAG(start_, Start)
    COMMAND_GET_SET_FLAG(end_, End)

};
using SetMulticyclePathPtr = std::shared_ptr<SetMulticyclePath>;

class MulticyclePathContainerData {
  public:
    void add(const SetMulticyclePathPtr &path) { multi_cycle_paths_.emplace_back(path); };

  private:
    std::vector<SetMulticyclePathPtr> multi_cycle_paths_;

  public:
    COMMAND_GET_SET_VAR(multi_cycle_paths, MultiCyclePaths)
};
using MulticyclePathContainerDataPtr = std::shared_ptr<MulticyclePathContainerData>;

class SetOutputDelay : public PortDelay {

};
using SetOutputDelayPtr = std::shared_ptr<SetOutputDelay>;

class OutputDelayContainerData {
  public:
    void add(const ObjectId &pin_id, const SetOutputDelayPtr &delay) { pin_output_delays_.emplace(pin_id, delay); }

  private:
    //TODO need ref pin to input delay map?
    std::unordered_map<ObjectId, SetOutputDelayPtr> pin_output_delays_;

  public:
    COMMAND_GET_SET_VAR(pin_output_delays, PinOutputDelays)
};
using OutputDelayContainerDataPtr = std::shared_ptr<OutputDelayContainerData>;

class SetPropagatedClock {
  public:
    bool addToPin(const std::string &pin_name);
    void addToClock(ClockPtr &clock) { clock->setPropagated(); }

  private:
    std::set<ObjectId> pins_;

  public:
    COMMAND_GET_SET_VAR(pins, Pins)
};
using SetPropagatedClockPtr = std::shared_ptr<SetPropagatedClock>;


}
}
#endif //EDI_DB_TIMING_SDC_TIMING_CONSTRAINTS_H_ 
