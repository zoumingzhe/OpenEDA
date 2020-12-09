/** 
 * @file timing_exception.h
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

#ifndef EDI_DB_TIMING_SDC_TIMING_EXCEPTION_H_
#define EDI_DB_TIMING_SDC_TIMING_EXCEPTION_H_

#include <string>
#include <vector>
#include <memory>

#include "db/core/object.h"
#include "db/timing/sdc/clock.h"
#include "db/timing/sdc/command_get_set_property.h"

namespace open_edi {
namespace db {

class PathNodes {
  public:
    void addPin(const ObjectId pin_id) { pins_.emplace_back(pin_id); }
    void addTerm(const ObjectId term_id) { terms_.emplace_back(term_id); }
    void addInstance(const ObjectId instance_id) { instances_.emplace_back(instance_id); }
    void addClock(const ClockId clock_id) { clocks_.emplace_back(clock_id); }

  private:
    std::vector<ObjectId> pins_;
    std::vector<ObjectId> terms_; 
    std::vector<ObjectId> instances_;
    std::vector<ClockId> clocks_;

  public:
    COMMAND_GET_SET_VAR(pins, Pins)
    COMMAND_GET_SET_VAR(terms, Terms)
    COMMAND_GET_SET_VAR(instances, Instances)
    COMMAND_GET_SET_VAR(clocks, Clocks)
    COMMAND_GET_SET_FLAG(rise, Rise)
    COMMAND_GET_SET_FLAG(fall, Fall)
};
using PathNodesPtr = std::shared_ptr<PathNodes>;

class PathThroughNodes : PathNodes {
  public:
    void addNet(const ObjectId net_id) { nets_.emplace_back(net_id); };

  private:
    std::vector<ObjectId> nets_;

  public:
    COMMAND_GET_SET_VAR(nets, Nets)
};
using PathThroughNodesPtr = std::shared_ptr<PathThroughNodes>;

class ExceptionPath {
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

}
}

#endif // EDI_DB_TIMING_SDC_TIMING_EXCEPTION_H_
