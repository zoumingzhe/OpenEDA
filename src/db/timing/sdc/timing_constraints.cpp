/** 
 * @file timing_constraints.cpp
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

#include "db/timing/sdc/timing_constraints.h"

namespace open_edi {
namespace db {

bool CreateGeneratedClock::addSourceMasterPin(const std::string &pin_name) {
    const auto &pin = getPinByFullName(pin_name); 
    if (!pin) {
        return false;
    }
    const auto &pin_id = pin->getId();
    source_master_pins_.emplace_back(pin_id);
    return true;
}

void ClockContainerData::addClock(ClockPtr &clock, CreateClockPtr &create_clock) {
    const ClockId &clock_id = clock_ids_.size();
    clock->setId(clock_id);
    clock_ids_.emplace_back(clock_id);
    const std::string &clock_name = clock->getName();
    clock_names_.emplace_back(clock_name);
    name_to_id_.emplace(clock_name, clock_id);
    clocks_.emplace_back(clock);
    create_clocks_.emplace(clock_id, create_clock);
}

void ClockContainerData::addClock(ClockPtr &clock, CreateGeneratedClockPtr &create_generated_clock) {
    const ClockId &clock_id = clock->getId();
    const std::string &clock_name = clock->getName();
    clock_ids_.emplace_back(clock_id);
    clock_names_.emplace_back(clock_name);
    name_to_id_.emplace(clock_name, clock_id);
    clocks_.emplace_back(clock);
    create_generated_clocks_.emplace(clock_id, create_generated_clock);
}

bool ClockContainerData::addClockPin(const std::string &pin_name, const ClockPtr &clock) {
    const auto &pin = getPinByFullName(pin_name);
    if (!pin) {
        return false;
    }
    const auto &pin_id = pin->getId();
    if (!clock->isAdd()) {
        auto found = pin_clock_map_.find(pin_id);
        if (found != pin_clock_map_.end()) {
            found->second = clock;
            return true;
        }
    }
    pin_clock_map_.emplace(pin_id, clock);
    return true;
}


}
}
