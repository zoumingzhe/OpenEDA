/** 
 * @file multivoltage_power_commands.cpp
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

#include "db/timing/sdc/multivoltage_power_commands.h"
#include "db/timing/timinglib/timinglib_units.h"

namespace open_edi {
namespace db {

void SetMaxDynamicPower::set(const float &value, const std::string &unit) {
    power_value_ = value * UnitMultiply::getCapacitiveUnitMultiply(unit);
}

void SetMaxLeakagePower::set(const float &value, const std::string &unit) {
    power_value_ = value * UnitMultiply::getCapacitiveUnitMultiply(unit);
}


}
}
