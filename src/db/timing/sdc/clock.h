/** 
 * @file clock.h
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

#ifndef EDI_DB_TIMING_SDC_CLOCK_H_
#define EDI_DB_TIMING_SDC_CLOCK_H_

#include <memory>

#include "db/core/object.h"
#include "util/util.h"

namespace open_edi {
namespace db {

using ClockId = open_edi::util::ULong;
static const ClockId invalid_clock_id = -1;
class Clock {
  public:
    void setPeriod(const float period) { period_ = period; }
    void setClockId(const ClockId& id) { id_ = id; }
    void setWaveform(const float& edge) { waveform_.emplace_back(edge); }
    void setName(const std::string& name) { name_ = name; }

    const float getPeriod() { return period_; } 
    const ClockId getId() { return id_; }
    const std::vector<float> &getWaveform() { return waveform_; }
    const std::string &getName() { return name_; } 

    friend std::ostream &operator<<(std::ostream &os, const Clock &rhs);

  private:
    float period_ = 0.0;
    ClockId id_ = invalid_clock_id;
    std::vector<float> waveform_;
    std::string name_ = "";
};
using ClockPtr = std::shared_ptr<Clock>;

class ClockGroup {
  public:
    void addClock(const ClockId& id) { clock_ids_.emplace_back(id); }
    void setClockIds(const std::vector<ClockId> &ids) { clock_ids_ = ids; }

    const std::vector<ClockId> &getClockIds() { return clock_ids_; }
  private:
    std::vector<ClockId> clock_ids_;
};
using ClockGroupPtr = std::shared_ptr<ClockGroup>;

class ClockPinPair {
  public:
    ClockPinPair(ClockId clock_id, ObjectId pin_id) : clock_id_(clock_id), pin_id_(pin_id) {}
    //deconstructor
    //operator ==
    void SetClockId(const ClockId &id) { clock_id_ = id; } 
    void SetPinId(const ObjectId &id) { pin_id_ = id; } 

    const ClockId& getClockId() { return clock_id_; }
    const ClockId& getPinId() { return pin_id_; }

  private:
    ClockId clock_id_ = invalid_clock_id;
    ObjectId pin_id_;
};
using ClockPinPairPtr = std::shared_ptr<ClockPinPair>;

class ClockPair {
  public:
    ClockPair(ClockId first = invalid_clock_id, ClockId second = invalid_clock_id) : first_id_(first), second_id_(second) {}
    //deconstructor
    //operator ==
    void SetFirstId(const ClockId &id) { first_id_ = id; } 
    void setSecondId(const ClockId &id) { second_id_ = id; }

    const ClockId &getFirstId() { return first_id_; }
    const ClockId &getSecondId() { return second_id_; }

  private:
    ClockId first_id_ = invalid_clock_id;
    ClockId second_id_ = invalid_clock_id;
};
using ClockPairPtr = std::shared_ptr<ClockPair>;

//TODO invalied object id?
class PinPair {
  public:
    PinPair(ObjectId first, ObjectId second) : first_id_(first), second_id_(second) {} 
    void SetFirstId(const ObjectId &id) { first_id_ = id; }
    void SetSecondId(const ObjectId &id) { second_id_ = id; }

    const ObjectId &getFirstId() { return first_id_; }
    const ObjectId &getSecondId() { return second_id_; }

  private:
    ObjectId first_id_;
    ObjectId second_id_;
};
using PinPairPtr = std::shared_ptr<PinPair>;



}
}

#endif // EDI_DB_TIMING_SDC_CLOCK_H_
