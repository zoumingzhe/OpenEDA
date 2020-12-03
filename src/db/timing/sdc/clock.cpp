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

std::ostream &operator<<(std::ostream &os, const Clock &rhs) {
    //TODO 
    os << "Clock name: "<< rhs.name_ << " period: " << rhs.period_ << " id: " << rhs.id_ << "\n";
    return os; 
}

}
}
