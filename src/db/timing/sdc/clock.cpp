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

std::ostream &operator<<(std::ostream &os, Clock &rhs) {
    os << "Clock name: " << rhs.name_ << " period: " << rhs.period_ << " id: " << rhs.id_ << "\n";
    os << "waveform: {";
    copy(rhs.waveform_.begin(), rhs.waveform_.end(), std::ostream_iterator<float>(os, " "));
    os << " }" << "\n";
    return os; 
}

}
}
