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

using ClockId = open_edi::util::UInt32;
static const ClockId kInvalidClockId = -1;
class Clock {
  public:
    Clock();
    Clock(ClockId id);
    Clock(const Clock &rhs);
    Clock(Clock &&rhs) noexcept;
    Clock &operator=(const Clock &rhs);
    Clock &operator=(Clock &&rhs) noexcept;
    ~Clock() {};

    bool operator==(const Clock &rhs) const;

  public:
    void addWaveform(const float &edge) { waveform_.emplace_back(edge); }

    void setName(const std::string &name) { name_ = name; }
    void setWaveform(const std::vector<float> &edge) { waveform_ = edge; }
    void setPeriod(const float &period) { period_ = period; }
    void setId(const ClockId &id) { id_ = id; }
    void setGenerated() { is_generated_ = true; }
    void setVirtual() { is_virtual_ = true; }
    void setAdd() { is_add_ = true; }

    const std::string &getName() const { return name_; }
    const std::vector<float> &getWaveform() const { return waveform_; }
    const float &getPeriod() const { return period_; }
    const ClockId &getId() const { return id_; }
    bool isGenerated() const { return is_generated_; }
    bool isVirtual() const { return is_virtual_; }
    bool isAdd() const { return is_add_; }

    friend std::ostream &operator<<(std::ostream &os, Clock &rhs);
    //TODO hash function

  private:
    void copy(const Clock &rhs);
    void move(Clock &&rhs);
    std::string name_ = "";
    std::vector<float> waveform_;
    float period_ = 0.0;
    ClockId id_ = kInvalidClockId;
    bool is_generated_ : 1;
    bool is_virtual_ : 1;
    bool is_add_ : 1;
};
using ClockPtr = std::shared_ptr<Clock>;

class ClockPinPair {
  public:
    ClockPinPair(ClockId clock_id, ObjectId pin_id) : clock_id_(clock_id), pin_id_(pin_id) {}
    //deconstructor
    //operator ==
    void SetClockId(const ClockId &id) { clock_id_ = id; } 
    void SetPinId(const ObjectId &id) { pin_id_ = id; } 

    const ClockId &getClockId() const { return clock_id_; }
    const ObjectId &getPinId() const { return pin_id_; }

  private:
    ClockId clock_id_ = kInvalidClockId;
    ObjectId pin_id_ = UNINIT_OBJECT_ID;
};
using ClockPinPairPtr = std::shared_ptr<ClockPinPair>;

class ClockPair {
  public:
    ClockPair(ClockId first = kInvalidClockId, ClockId second = kInvalidClockId) : first_id_(first), second_id_(second) {}
    //deconstructor
    //operator ==
    void SetFirstId(const ClockId &id) { first_id_ = id; } 
    void setSecondId(const ClockId &id) { second_id_ = id; }

    const ClockId &getFirstId() { return first_id_; }
    const ClockId &getSecondId() { return second_id_; }

  private:
    ClockId first_id_ = kInvalidClockId;
    ClockId second_id_ = kInvalidClockId;
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
