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

void ClockContainerData::addClock(ClockPtr &clock, CreateClockPtr &create_clock) {
    const ClockId &clock_id = clock->getId();
    const std::string &clock_name = clock->getName();
    clock_names_.emplace_back(clock_name);
    clocks_.emplace_back(clock);
    name_to_id_.emplace(clock_name, clock_id);
    create_clocks_.emplace(clock_id, create_clock);
}

void ClockContainerData::addClock(ClockPtr &clock, CreateGeneratedClockPtr &create_generated_clock) {
    const ClockId &clock_id = clock->getId();
    const std::string &clock_name = clock->getName();
    clock_names_.emplace_back(clock_name);
    clocks_.emplace_back(clock);
    name_to_id_.emplace(clock_name, clock_id);
    create_generated_clocks_.emplace(clock_id, create_generated_clock);
}


}
}
