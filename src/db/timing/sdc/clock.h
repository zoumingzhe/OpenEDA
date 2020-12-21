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
    ~Clock()=default;

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
    void setPropagated() { is_propagated_ = true; }

    const std::string &getName() const { return name_; }
    const std::vector<float> &getWaveform() const { return waveform_; }
    const float &getPeriod() const { return period_; }
    const ClockId &getId() const { return id_; }
    bool isGenerated() const { return is_generated_; }
    bool isVirtual() const { return is_virtual_; }
    bool isAdd() const { return is_add_; }
    bool isPropagated() const { return is_propagated_; }

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
    bool is_propagated_ : 1;
};
using ClockPtr = std::shared_ptr<Clock>;

}
}

#endif // EDI_DB_TIMING_SDC_CLOCK_H_
