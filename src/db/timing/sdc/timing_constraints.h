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

namespace open_edi {
namespace db {

//TODO use get set macro

class CreateClock {
  public:
    void set_period(const float period) { period_ = period; }
    void set_add(const bool add) { add_ = add; }
    void set_name(const std::string& name) { name_ = name; }
    void set_comment(const std::string& comment) { comment_ = comment;  }
    void set_waveform(const float& edge) { waveform_.emplace_back(edge); }

    const float get_period() { return period_; } 
    const std::string& get_name() { return name_; } 
    const std::vector<float>& get_waveform() { return waveform_; }

    const bool get_add() { return add_; }

  private:
    float period_ = 0.0;
    std::string name_ = "";
    std::string comment_ = "";
    std::vector<float> waveform_;

    bool add_ = false;
};
using CreateClockPtr = std::shared_ptr<CreateClock>;

class CreateGeneratedClock {
  public:
    void set_duty_cycle(const float duty_cycle) { duty_cycle_ = duty_cycle_; }
    void set_source_master_pin(const ObjectId source) { source_master_pin_ = source; }
    void set_master_clock(const ObjectId master_clock) { master_clock_ = master_clock; }
    void set_divided_by(const int divided_by) { divided_by_ = divided_by; }
    void set_multiply_by(const int multiply_by) { multiply_by_ = multiply_by; }
    void set_invert(const bool invert) { invert_= invert; }
    void set_add(const bool add) { add_ = add; }
    void set_combinational(const bool combinational) { combinational_ = combinational; }
    void set_name(const std::string& name) { name_ = name; }
    void set_comment(const std::string& comment) { comment_ = comment; }
    void set_edge_shift(const float edge_shift) { edge_shifts_.emplace_back(edge_shift); }
    void set_source_objects(const ObjectId source_object) { source_objects_.emplace_back(source_object); }

    const float get_duty_cycle() { return duty_cycle_; }
    const ObjectId get_source_master_pin() { return source_master_pin_; }
    const ObjectId get_master_clock() { return master_clock_; }
    const int get_divided_by() { return divided_by_; }
    const int get_multiply_by() { return multiply_by_; }
    const std::string& get_name() { return name_; } 
    const std::string& get_comment() { return comment_; } 
    const std::vector<float>& get_edge_shifts() { return edge_shifts_; }
    const std::vector<int>& get_edges() { return edges_; } 
    const std::vector<ObjectId>& get_source_objects() { return source_objects_; }

    const bool get_invert() { return invert_; }
    const bool get_add() { return add_; }
    const bool get_combinational() { return combinational_; }

  private:
    float duty_cycle_ = 0.0;
    ObjectId source_master_pin_;
    ObjectId master_clock_;
    int divided_by_ = 0;
    int multiply_by_ = 0;
    std::string name_ = "";
    std::string comment_ = "";
    std::vector<float> edge_shifts_;
    std::vector<int> edges_;
    std::vector<ObjectId> source_objects_;

    bool invert_ = false;
    bool add_ = false;
    bool combinational_ = false;
};
using CreateGeneratedClockPtr = std::shared_ptr<CreateGeneratedClock>;


}
}
#endif //EDI_DB_TIMING_SDC_TIMING_CONSTRAINTS_H_ 
