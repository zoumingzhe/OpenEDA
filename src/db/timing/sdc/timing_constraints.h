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
#inlcude "db/timing/sdc/timing_exception.h"

namespace open_edi {
namespace db {

class CreateClock {
  private:
    std::string comment_ = "";
    std::bitset<1> flags_;

  public:
    COMMAND_GET_SET_VAR(comment, Comment)
    COMMAND_GET_SET_FLAG(flags, 0, add, Add)
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
    std::bitset<3> flags_;

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
    COMMAND_GET_SET_FLAG(flags, 0, invert, Invert)
    COMMAND_GET_SET_FLAG(flags, 1, add, Add)
    COMMAND_GET_SET_FLAG(flags, 2, combinational, Combinational)
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
    std::bitset<1> flags_;
  public:
    COMMAND_GET_SET_VAR(comment, Comment)
    COMMAND_GET_SET_VAR(name, Name)
    COMMAND_GET_SET_VAR(weight, Weight)
    COMMAND_GET_SET_VAR(from, From)
    COMMAND_GET_SET_VAR(to, To)
    COMMAND_GET_SET_VAR(through, Through)
    COMMAND_GET_SET_FLAG(flags, 0, default_value, DefaultValue)
};
using GroupPathPtr = std::shared_ptr<GroupPath>; 

class SetClockGatingCheck {
  private:
    float setup_ = 0.0;
    float hold_ = 0.0;
    std::bitset<4> flags_;
  public:
    COMMAND_GET_SET_VAR(setup, Setup)
    COMMAND_GET_SET_VAR(hold, Hold)
    COMMAND_GET_SET_FLAG(flags, 0, rise, Rise)
    COMMAND_GET_SET_FLAG(flags, 1, fall, Fall)
    COMMAND_GET_SET_FLAG(flags, 2, high, High)
    COMMAND_GET_SET_FLAG(flags, 3, low, Low)
};
using SetClockGatingCheckPtr = std::shared_ptr<SetClockGatingCheck>;

//TODO change the data structure for better data fetch, clock to get another clocks
class SetClockGroups {
  private:
    string name_ = "";
    string comment_ = "";
    std::vector<ClockGroupId> groups_;
    std::bitset<4> flags_;

  public:
    COMMAND_GET_SET_VAR(name, Name)
    COMMAND_GET_SET_VAR(comment, Comment)
    COMMAND_GET_SET_FLAG(flags, 0, physically_exclusive, PhysicallyExclusive)
    COMMAND_GET_SET_FLAG(flags, 1, logically_exclusive, LogicallyExclusive)
    COMMAND_GET_SET_FLAG(flags, 2, asynchronous, Asynchronous)
    COMMAND_GET_SET_FLAG(flags, 3, allow_paths, AllowPaths)
};
using SetClockGroupsPtr = std::shared_ptr<SetClockGroups>;

class SetClockLatency {
  private:
    float delay_ = 0.0;
    std::bitset<8> flags_;

  public:
    COMMAND_GET_SET_VAR(delay, Delay)
    COMMAND_GET_SET_FLAG(flags, 0, rise, Rise)
    COMMAND_GET_SET_FLAG(flags, 1, fall, Fall)
    COMMAND_GET_SET_FLAG(flags, 2, min, Min)
    COMMAND_GET_SET_FLAG(flags, 3, max, Max)
    COMMAND_GET_SET_FLAG(flags, 4, dynamic, Dynamic)
    COMMAND_GET_SET_FLAG(flags, 5, source, Source)
    COMMAND_GET_SET_FLAG(flags, 6, early, Early)
    COMMAND_GET_SET_FLAG(flags, 7, late, Late)
};
using SetClockLatencyPtr = std::shared_ptr<SetClockLatency>;

enum class DataType = { kClock=0, kData, kUnknown };
enum class PulseType = { kRiseTriggeredHighPulse, kRiseTriggeredLowPulse, kFallTriggeredHighPulse, kFallTriggeredLowPulse, kUnknown}; 
class SetSense {
  private:
    DataType type_ = DataType::kUnknown;
    PulseType pulse_ = PulseType::kUnknown;  
    std::bitset<5> flags_;

  public:
    COMMAND_GET_SET_VAR(type, Type)
    COMMAND_GET_SET_VAR(pulse, Pulse)
    COMMAND_GET_SET_FLAG(flags, 0, non_unate, NonUnate)
    COMMAND_GET_SET_FLAG(flags, 1, positive, Positive)
    COMMAND_GET_SET_FLAG(flags, 2, negative, Negative)
    COMMAND_GET_SET_FLAG(flags, 3, clock_leaf, ClockLeaf)
    COMMAND_GET_SET_FLAG(flags, 4, stop_propagation, StopPropation)
}; 
using SetSencePtr = std::shared_ptr<SetSence>;

class SetClockTransition {
  private:
    float transition_ = 0.0;
    std::bitset<4> flags_;

  public:
    COMMAND_GET_SET_VAR(transition, Transition)
    COMMAND_GET_SET_FLAG(flags, 0, rise, Rise)
    COMMAND_GET_SET_FLAG(flags, 1, fall, Fall)
    COMMAND_GET_SET_FLAG(flags, 2, min, Min)
    COMMAND_GET_SET_FLAG(flags, 3, max, Max)
};
using SetClockTransitionPtr = std::shared_ptr<SetClockTransition>;

class SetClockUncertainty {
  private:
    float uncertainty_ = 0.0;
    std::bitset<4> flags_;

  public:
    COMMAND_GET_SET_VAR(transition, Transition)
    COMMAND_GET_SET_FLAG(flags, 0, consider_rise_from, ConsiderRiseFrom)
    COMMAND_GET_SET_FLAG(flags, 1, consider_fall_from, ConsiderFallFrom)
    COMMAND_GET_SET_FLAG(flags, 2, consider_rise_to, ConsiderRiseTo)
    COMMAND_GET_SET_FLAG(flags, 3, consider_fall_to, ConsiderFallTo)
    COMMAND_GET_SET_FLAG(flags, 4, setup, Setup)
    COMMAND_GET_SET_FLAG(flags, 5, hold, Hold)
};
using SetClockUncertaintyPtr = std::shared_ptr<SetClockUncertainty>;

class SetDataCheck {
  public:
    void addClock(const ClockId &id) { clocks_.emplace_back(id); };

  private:
    float value_ = 0.0;
    std::vector<ClockId> clocks_;
    std::bitset<6> flags_;

  public:
    COMMAND_GET_SET_VAR(value, Value)
    COMMAND_GET_SET_VAR(clocks, Clocks)
    COMMAND_GET_SET_FLAG(flags, 0, consider_rise_from, ConsiderRiseFrom)
    COMMAND_GET_SET_FLAG(flags, 1, consider_fall_from, ConsiderFallFrom)
    COMMAND_GET_SET_FLAG(flags, 2, consider_rise_to, ConsiderRiseTo)
    COMMAND_GET_SET_FLAG(flags, 3, consider_fall_to, ConsiderFallTo)
    COMMAND_GET_SET_FLAG(flags, 4, setup, Setup)
    COMMAND_GET_SET_FLAG(flags, 5, hold, Hold)
};
using SetDataCheckPtr = std::shared_ptr<SetDataCheck>;

class SetDiableTiming {
  private:
    ObjectId from_;
    ObjectId to_;
    std::bitset<1> flags_;
  public:
    COMMAND_GET_SET_VAR(from, From)
    COMMAND_GET_SET_VAR(to, To)
    COMMAND_GET_SET_FLAG(flags, 0, io_pin, IoPin)
}; 
using SetDisableTimingPtr = std::shared_ptr<SetDisableTiming>;

class SetFalsePath : public ExceptionPath {
  private:
    std::string comment_ = ""; 
    std::bitset<2> flags_;

  public:
    COMMAND_GET_SET_VAR(comment, Comment)
    COMMAND_GET_SET_FLAG(flags, 0, setup, Setup)
    COMMAND_GET_SET_FLAG(flags, 1, hold, Hold)

}; 
using SetFalsePathPtr = std::shared_ptr<SetFalsePath>;

class SetIdealLatency {
  private:
    float delay_;
    std::bitset<4> flags_;

  public:
    COMMAND_GET_SET_VAR(delay, Delay)
    COMMAND_GET_SET_FLAG(flags, 0, rise, Rise)
    COMMAND_GET_SET_FLAG(flags, 1, fall, Fall)
    COMMAND_GET_SET_FLAG(flags, 2, min, Min)
    COMMAND_GET_SET_FLAG(flags, 3, max, Max)
};
using SetIdealLatencyPtr = std::shared_ptr<SetIdealLatency>;

class SetIdealNetwork {
  public:
    void addPin(ObjectId &pin_id) { pins_.emplace_back(pin_id); };
    void addNet(ObjectId &net_id) { nets_.emplace_back(net_id); };

  private:
    std::vector<ObjectId> pins_;
    std::vector<ObjectId> nets_;
    std::bitset<1> flags_;

  public:
    COMMAND_GET_SET_VAR(pins, Pins)
    COMMAND_GET_SET_VAR(nets, Nets)
    COMMAND_GET_SET_FLAG(flags, 0, no_propagate, NoPropagate)
};
using SetIdealNetworkPtr = std::shared_ptr<SetIdealNetwork>;

class SetIdealTransition {
  private:
    float value_ = 0.0;
    std::bitset<4> flags_;

  public:
    COMMAND_GET_SET_VAR(value, Value)
    COMMAND_GET_SET_FLAG(flags, 0, rise, Rise)
    COMMAND_GET_SET_FLAG(flags, 1, fall, Fall)
    COMMAND_GET_SET_FLAG(flags, 2, min, Min)
    COMMAND_GET_SET_FLAG(flags, 3, max, Max)
};
using SetIdealTransitionPtr = std::shared_ptr<SetIdealTransition>;

class PortDelay {
  private:
    float delay_value_ = 0.0;
    std::vector<ClockId> clocks_;
    std::bitset<9> flags_;

  public:
    COMMAND_GET_SET_VAR(delay_value, DelayValue);
    COMMAND_GET_SET_VAR(clocks, Clocks)
    COMMAND_GET_SET_FLAG(flags, 0, clock_fall, ClockFall)
    COMMAND_GET_SET_FLAG(flags, 1, level_sensitive, LevelSensitive)
    COMMAND_GET_SET_FLAG(flags, 2, rise, Rise)
    COMMAND_GET_SET_FLAG(flags, 3, fall, Fall)
    COMMAND_GET_SET_FLAG(flags, 4, min, Min)
    COMMAND_GET_SET_FLAG(flags, 5, max, Max)
    COMMAND_GET_SET_FLAG(flags, 6, add_delay, AddDelay)
    COMMAND_GET_SET_FLAG(flags, 7, network_latency_included, NetworkLatencyIncluded)
    COMMAND_GET_SET_FLAG(flags, 8, source_latency_included, SourceLatencyIncluded)
};
using PortDelayPtr = std::shared_ptr<PortDelay>;

class SetInputDelay : public PortDelay {

};
using SetInputDelayPtr = std::shared_ptr<SetInputDelay>;

class SetMaxDelay : public ExceptionPath {
  private:
    float delay_value_ = 0.0;
    std::string comment = "";
    std::bitset<3> flags_;

  public:
    COMMAND_GET_SET_VAR(delay_value, DelayValue)
    COMMAND_GET_SET_VAR(comment, Comment)
    COMMAND_GET_SET_FLAG(flags, 0, rise, Rise)
    COMMAND_GET_SET_FLAG(flags, 1, fall, Fall)
    COMMAND_GET_SET_FLAG(flags, 2, ignore_clock_latency, IgnoreClockLatency)
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
    std::string comment = "";
    std::bitset<3> flags_;

  public:
    COMMAND_GET_SET_VAR(delay_value, DelayValue)
    COMMAND_GET_SET_VAR(comment, Comment)
    COMMAND_GET_SET_FLAG(flags, 0, rise, Rise)
    COMMAND_GET_SET_FLAG(flags, 1, fall, Fall)
    COMMAND_GET_SET_FLAG(flags, 2, ignore_clock_latency, IgnoreClockLatency)
};
using SetMinDelayPtr = std::shared_ptr<SetMinDelay>;

class SetMinPulseWidth {
  private:
    float value_ = 0.0;
    std::bitset<2> flags_;

  public:
    COMMAND_GET_SET_VAR(value, Value)
    COMMAND_GET_SET_FLAG(flags, 0, low, Low)
    COMMAND_GET_SET_FLAG(flags, 1, high, High)
};
using SetMinPulseWidthPtr = std::shared_ptr<SetMinPulseWidth>;

class SetMulticyclePath : public ExceptionPath {
  private:
    std::string comment_ = "";
    UInt32 path_multiplier = 0;
    std::bitset<6> flags_;

  public:
    COMMAND_GET_SET_VAR(comment, Comment)
    COMMAND_GET_SET_FLAG(flags, 0, setup, Setup)
    COMMAND_GET_SET_FLAG(flags, 1, hold, Hold)
    COMMAND_GET_SET_FLAG(flags, 2, rise, Rise)
    COMMAND_GET_SET_FLAG(flags, 3, fall, Fall)
    COMMAND_GET_SET_FLAG(flags, 4, start, Start)
    COMMAND_GET_SET_FLAG(flags, 5, end, End)

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
