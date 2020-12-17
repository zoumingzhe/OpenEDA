/** 
 * @file clock.cpp
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

#include "db/timing/sdc/clock.h"

namespace open_edi {
namespace db {

Clock::Clock(ClockId id) : name_(""), waveform_() {
    period_ = 0.0;
    id_ = kInvalidClockId;
    is_generated_ = true;
}

Clock::Clock() : Clock(kInvalidClockId) {}

Clock::Clock(const Clock &rhs) {
    copy(rhs);
}

Clock::Clock(Clock &&rhs) noexcept {
    move(std::move(rhs));
}

Clock &Clock::operator=(const Clock &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

Clock &Clock::operator=(Clock &&rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void Clock::copy(const Clock &rhs) {
    name_ = rhs.name_;
    waveform_ = rhs.waveform_;
    period_ = rhs.period_;
    id_ = rhs.id_;
    is_generated_ = rhs.is_generated_;
}

void Clock::move(Clock &&rhs) {
    name_ = std::move(rhs.name_);
    waveform_ = std::move(rhs.waveform_);
    period_ = rhs.period_;
    id_ = rhs.id_;
    is_generated_ = rhs.is_generated_;
}

bool Clock::operator==(const Clock &rhs) const {
    return ((name_ == rhs.getName()) and (id_ == rhs.getId()));
}

std::ostream &operator<<(std::ostream &os, Clock &rhs) {
    os << "Clock name: " << rhs.name_ << " period: " << rhs.period_ << " id: " << rhs.id_ << "\n";
    os << "waveform: {";
    copy(rhs.waveform_.begin(), rhs.waveform_.end(), std::ostream_iterator<float>(os, " "));
    os << " }" << "\n";
    return os; 
}

}
}
