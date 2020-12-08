/** 
 * @file object_access_commands.h
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

#ifndef EDI_DB_TIMING_SDC_OBJECT_ACCESS_COMMANDS_H_
#define EDI_DB_TIMING_SDC_OBJECT_ACCESS_COMMANDS_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "db/core/object.h"

namespace open_edi {
namespace db {

class AllClocks {
  private:
    std::vector<ClockId> clocks_;
  public:
    COMMAND_GET_SET_VAR(clocks, Clocks)
};
using AllClocksPtr = std::shared_ptr<AllClocks>;

class AllInputs {
  private:
    std::bitset<2> flags_;

  public:
    COMMAND_GET_SET_FLAG(flags, 0, level_sensitive, LevelSensitive)
    COMMAND_GET_SET_FLAG(flags, 1, edge_triggered, EdgeTriggered)
};
using AllInputsPtr = std::shared_ptr<AllInputs>;

class AllOutputs {
  private:
    std::bitset<2> flags_;

  public:
    COMMAND_GET_SET_FLAG(flags, 0, level_sensitive, LevelSensitive)
    COMMAND_GET_SET_FLAG(flags, 1, edge_triggered, EdgeTriggered)
};
using AllOutputsPtr = std::shared_ptr<AllOutputs>;

class AllRegisters {
  private:
    std::vector<ClockId> clocks_;
    std::vector<ClockId> rise_clocks_;
    std::vector<ClockId> fall_clocks_;
    std::bitset<10> flags_;

  public:
    COMMAND_GET_SET_VAR(clocks, Clocks)
    COMMAND_GET_SET_VAR(rise_clocks, RiseClocks)
    COMMAND_GET_SET_VAR(fall_clocks, FallClocks)
    COMMAND_GET_SET_FLAG(flags, 0, no_hierarchy, NoHierarchy)
    COMMAND_GET_SET_FLAG(flags, 1, cells, Cells)
    COMMAND_GET_SET_FLAG(flags, 2, data_pins, DataPins)
    COMMAND_GET_SET_FLAG(flags, 3, clock_pins, ClockPins)
    COMMAND_GET_SET_FLAG(flags, 4, slave_clock_pins, SlaveClockPins)
    COMMAND_GET_SET_FLAG(flags, 5, async_pins, AsyncPins)
    COMMAND_GET_SET_FLAG(flags, 6, output_pins, OutputPins)
    COMMAND_GET_SET_FLAG(flags, 7, level_sensitive, LevelSensitive)
    COMMAND_GET_SET_FLAG(flags, 8, edge_triggered, EdgeTriggered)
    COMMAND_GET_SET_FLAG(flags, 9, master_slave, MasterSlave)
};
using AllRegistersPtr = std::shared_ptr<AllRegisters>;

class CurrentDesign {
  private:
    ObjectId current_design_;

  public:
    COMMAND_GET_SET_VAR(current_design, CurrentDesign)
};
using CurrentDesignPtr = std::shared_ptr<CurrentDesign>; 

class GetCells {
  private:
    std::vector<ObjectId> of_objects_;
    std::string patterns_ = ".*";
    std::bitset<3> flags_;

  public:
    COMMAND_GET_SET_VAR(of_objects, OfObjects)
    COMMAND_GET_SET_VAR(patterns, Patterns)
    COMMAND_GET_SET_FLAG(flags, 0, hierarchical, Hierarchical)
    COMMAND_GET_SET_FLAG(flags, 1, regexp, Regexp)
    COMMAND_GET_SET_FLAG(flags, 2, nocase, NoCase)
};
using GetCellsPtr = std::shared_ptr<GetCells>;

class GetClocks {
  private:
    std::string patterns_ = ".*";
    std::bitset<3> flags_;

  public:
    COMMAND_GET_SET_VAR(patterns, Patterns)
    COMMAND_GET_SET_FLAG(flags, 0, regexp, Regexp)
    COMMAND_GET_SET_FLAG(flags, 1, nocase, NoCase)
}; 
using GetClocksPtr = std::shared_ptr<GetClocks>;

class GetLibCells {
  private:
    std::string patterns_ = ".*";
    std::bitset<2> flags_;

  public:
    COMMAND_GET_SET_VAR(patterns, Patterns)
    COMMAND_GET_SET_FLAG(flags, 0, regexp, Regexp)
    COMMAND_GET_SET_FLAG(flags, 1, nocase, NoCase)
};
using GetLibCellsPtr = std::shared_ptr<GetLibCells>;

class GetLibPins {
  private:
    std::string patterns_ = ".*";
    std::bitset<2> flags_;

  public:
    COMMAND_GET_SET_VAR(patterns, Patterns)
    COMMAND_GET_SET_FLAG(flags, 0, regexp, Regexp)
    COMMAND_GET_SET_FLAG(flags, 1, nocase, NoCase)
};
using GetLibPinsPtr = std::shared_ptr<GetLibPins>;

class GetLibs {
  private:
    std::string patterns_ = ".*";
    std::bitset<2> flags_;

  public:
    COMMAND_GET_SET_VAR(patterns, Patterns)
    COMMAND_GET_SET_FLAG(flags, 0, regexp, Regexp)
    COMMAND_GET_SET_FLAG(flags, 1, nocase, NoCase)
};
using GetLibsPtr = std::shared_ptr<GetLibs>;

class GetNets {
  private:
    std::vector<ObjectId> of_objects_;
    std::bitset<3> flags_;

  public:
    COMMAND_GET_SET_VAR(of_objects, OfObjects)
    COMMAND_GET_SET_FLAG(flags, 0, hierarchical, Hierarchical)
    COMMAND_GET_SET_FLAG(flags, 1, regexp, Regexp)
    COMMAND_GET_SET_FLAG(flags, 2, nocase, NoCase)
};
using GetNetsPtr = std::shared_ptr<GetNets>;

class GetPins {
  private:
    std::string patterns_ = ".*";
    std::vector<ObjectId> of_objects_;
    std::bitset<3> flags_;

  public:
    COMMAND_GET_SET_VAR(patterns, Patterns)
    COMMAND_GET_SET_VAR(of_objects, OfObjects)
    COMMAND_GET_SET_FLAG(flags, 0, hierarchical, Hierarchical)
    COMMAND_GET_SET_FLAG(flags, 1, regexp, Regexp)
    COMMAND_GET_SET_FLAG(flags, 2, nocase, NoCase)
};
using GetPinsPtr = std::shared_ptr<GetPins>;

class GetPorts {
  private:
    std::string patterns_ = ".*";
    std::bitset<2> flags_;

  public:
    COMMAND_GET_SET_VAR(patterns, Patterns)
    COMMAND_GET_SET_FLAG(flags, 0, regexp, Regexp)
    COMMAND_GET_SET_FLAG(flags, 1, nocase, NoCase)
};
using GetPortsPtr = std::shared_ptr<GetPorts>;


}
}

#endif //EDI_DB_TIMING_SDC_OBJECT_ACCESS_COMMANDS_H_
