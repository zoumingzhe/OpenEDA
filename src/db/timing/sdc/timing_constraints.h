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

#include "db/core/object.h"
#include "db/timing/sdc/clock.h"
#include "db/timing/sdc/timing_exception.h"
#include "db/timing/sdc/command_get_set_property.h"

namespace open_edi {
namespace db {

class CreateClock {
  private:
    std::string comment_ = "";

  public:
    COMMAND_GET_SET_VAR(comment, Comment)
    COMMAND_GET_SET_FLAG(add_, Add)
};
using CreateClockPtr = std::shared_ptr<CreateClock>;

class CreateGeneratedClock {
  public:
    void addEdgeShift(const float edge_shift) { edge_shifts_.emplace_back(edge_shift); }
    void addSourceObject(const ObjectId source_object) { source_objects_.emplace_back(source_object); }
    void addEdge(const int edge) { edges_.emplace_back(edge); }

  private:
    std::string comment_ = "";
    float duty_cycle_ = 0.0;
    ObjectId source_master_pin_;
    ObjectId master_clock_;
    int divided_by_ = 0;
    int multiply_by_ = 0;
    std::vector<float> edge_shifts_;
    std::vector<ObjectId> source_objects_;
    std::vector<int> edges_;

  public:
    COMMAND_GET_SET_VAR(comment, Comment)
    COMMAND_GET_SET_VAR(duty_cycle, DutyCycle)
    COMMAND_GET_SET_VAR(source_master_pin, SourceMasterPin)
    COMMAND_GET_SET_VAR(master_clock, MasterClock)
    COMMAND_GET_SET_VAR(divided_by, DividedBy)
    COMMAND_GET_SET_VAR(multiply_by, MultiplyBy)
    COMMAND_GET_SET_VAR(edge_shifts, EdgeShifts)
    COMMAND_GET_SET_VAR(source_objects, SourceObjects)
    COMMAND_GET_SET_VAR(edges, Edges)
    COMMAND_GET_SET_FLAG(invert_, Invert)
    COMMAND_GET_SET_FLAG(add_, Add)
    COMMAND_GET_SET_FLAG(combinational_, Combinational)
};
using CreateGeneratedClockPtr = std::shared_ptr<CreateGeneratedClock>;

class GroupPath {
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

class SetClockGatingCheck {
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

//TODO change the data structure for better data fetch, clock to get another clocks
/*
class SetClockGroups {
  private:
    std::string name_ = "";
    std::string comment_ = "";
    std::vector<ClockGroupId> groups_;

  public:
    COMMAND_GET_SET_VAR(name, Name)
    COMMAND_GET_SET_VAR(comment, Comment)
    COMMAND_GET_SET_FLAG(physically_exclusive_, PhysicallyExclusive)
    COMMAND_GET_SET_FLAG(logically_exclusive_, LogicallyExclusive)
    COMMAND_GET_SET_FLAG(asynchronous_, Asynchronous)
    COMMAND_GET_SET_FLAG(allow_paths_, AllowPaths)
};
using SetClockGroupsPtr = std::shared_ptr<SetClockGroups>;
*/

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

enum class DataType { kClock=0, kData, kUnknown };
enum class PulseType { kRiseTriggeredHighPulse, kRiseTriggeredLowPulse, kFallTriggeredHighPulse, kFallTriggeredLowPulse, kUnknown }; 
class SetSense {
  private:
    DataType type_ = DataType::kUnknown;
    PulseType pulse_ = PulseType::kUnknown;  

  public:
    COMMAND_GET_SET_VAR(type, Type)
    COMMAND_GET_SET_VAR(pulse, Pulse)
    COMMAND_GET_SET_FLAG(non_unate_, NonUnate)
    COMMAND_GET_SET_FLAG(positive_, Positive)
    COMMAND_GET_SET_FLAG(negative_, Negative)
    COMMAND_GET_SET_FLAG(clock_leaf_, ClockLeaf)
    COMMAND_GET_SET_FLAG(stop_propagation_, StopPropation)
}; 
using SetSensePtr = std::shared_ptr<SetSense>;

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

class SetClockUncertainty {
  private:
    float uncertainty_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(uncertainty, Uncertainty)
    COMMAND_GET_SET_FLAG(consider_rise_from_, ConsiderRiseFrom)
    COMMAND_GET_SET_FLAG(consider_fall_from_, ConsiderFallFrom)
    COMMAND_GET_SET_FLAG(consider_rise_to_, ConsiderRiseTo)
    COMMAND_GET_SET_FLAG(consider_fall_to_, ConsiderFallTo)
    COMMAND_GET_SET_FLAG(setup_, Setup)
    COMMAND_GET_SET_FLAG(hold_, Hold)
};
using SetClockUncertaintyPtr = std::shared_ptr<SetClockUncertainty>;

class SetDataCheck {
  public:
    void addClock(const ClockId &id) { clocks_.emplace_back(id); };

  private:
    float value_ = 0.0;
    std::vector<ClockId> clocks_;

  public:
    COMMAND_GET_SET_VAR(value, Value)
    COMMAND_GET_SET_VAR(clocks, Clocks)
    COMMAND_GET_SET_FLAG(consider_rise_from_, ConsiderRiseFrom)
    COMMAND_GET_SET_FLAG(consider_fall_from_, ConsiderFallFrom)
    COMMAND_GET_SET_FLAG(consider_rise_to_, ConsiderRiseTo)
    COMMAND_GET_SET_FLAG(consider_fall_to_, ConsiderFallTo)
    COMMAND_GET_SET_FLAG(setup_, Setup)
    COMMAND_GET_SET_FLAG(hold_, Hold)
};
using SetDataCheckPtr = std::shared_ptr<SetDataCheck>;

class SetDisableTiming {
  private:
    ObjectId from_;
    ObjectId to_;
  public:
    COMMAND_GET_SET_VAR(from, From)
    COMMAND_GET_SET_VAR(to, To)
    COMMAND_GET_SET_FLAG(io_pin_, IoPin)
}; 
using SetDisableTimingPtr = std::shared_ptr<SetDisableTiming>;

class SetFalsePath : public ExceptionPath {
  private:
    std::string comment_ = ""; 

  public:
    COMMAND_GET_SET_VAR(comment, Comment)
    COMMAND_GET_SET_FLAG(setup_, Setup)
    COMMAND_GET_SET_FLAG(hold_, Hold)

}; 
using SetFalsePathPtr = std::shared_ptr<SetFalsePath>;

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

class PortDelay {
  private:
    float delay_value_ = 0.0;
    std::vector<ClockId> clocks_;

  public:
    COMMAND_GET_SET_VAR(delay_value, DelayValue);
    COMMAND_GET_SET_VAR(clocks, Clocks)
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

class SetMaxTimeBorrow {
  private:
    float value_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(value, Value)
};
using SetMaxTimeBorrowPtr = std::shared_ptr<SetMaxTimeBorrow>;

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

class SetMinPulseWidth {
  private:
    float value_ = 0.0;

  public:
    COMMAND_GET_SET_VAR(value, Value)
    COMMAND_GET_SET_FLAG(low_, Low)
    COMMAND_GET_SET_FLAG(high_, High)
};
using SetMinPulseWidthPtr = std::shared_ptr<SetMinPulseWidth>;

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

class SetOutputDelay : public PortDelay {

};
using SetOutputDelayPtr = std::shared_ptr<SetOutputDelay>;

//not used
class SetPropagatedClock {


};
using SetPropagatedClockPtr = std::shared_ptr<SetPropagatedClock>;


}
}
#endif //EDI_DB_TIMING_SDC_TIMING_CONSTRAINTS_H_ 
