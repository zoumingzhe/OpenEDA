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
#include <boost/functional/hash.hpp>

namespace open_edi {
namespace db {

template <typename T>
bool ediEqual(const T &value1, const T &value2, T eps = std::numeric_limits<T>::epsilon()) {
    return fabs(value1 - value2) < eps;
}

template<typename T, typename U>
class UnorderedPair {
  public:
    UnorderedPair() {}
    UnorderedPair(T first_value, U second_value) : first(first_value), second(second_value) {}
    bool operator==(const UnorderedPair<T, U> &rhs) const;
    template<T, U>
    friend std::size_t hash_value(const UnorderedPair<T, U> &p);

    //keep the same usage as std::pair
    T first;
    U second;
};

template<typename T, typename U>
bool UnorderedPair<T,U>::operator==(const UnorderedPair<T, U> &rhs) const {
    if ((this->first == rhs.first) and (this->second == rhs.second)) {
        return true;
    }
    if ((this->first == rhs.second) and (this->second == rhs.first )) {
        return true;
    }
    return false;
}

template<typename T, typename U>
std::size_t hash_value(const UnorderedPair<T, U> &p) {
    std::size_t seed1 = 0;
    boost::hash_combine(seed1, p.first);
    std::size_t seed2 = 0;
    boost::hash_combine(seed2, p.second);
    std::size_t seed = (seed1 xor seed2) + p.first + p.second;
    return seed;
}

Pin* getPinByFullName(const std::string &full_name);
std::string getPinFullName(const ObjectId &pin_id);



}
}

#endif //EDI_DB_TIMING_SDC_SDC_COMMON_H_ 
