/** 
 * @file sdc_common.h
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

#ifndef EDI_DB_TIMING_SDC_SDC_COMMON_H_
#define EDI_DB_TIMING_SDC_SDC_COMMON_H_

#include "db/core/object.h"
#include "db/core/db.h"
#include "util/data_traits.h"
#include "util/util.h"
#include <cmath>
#include <limits>

namespace open_edi {
namespace db {

template <typename T>
bool ediEqual(const T &value1, const T &value2, T eps = std::numeric_limits<T>::epsilon()) {
    return fabs(value1 - value2) < eps;
}


















}
}

#endif //EDI_DB_TIMING_SDC_SDC_COMMON_H_ 
