/* @file  data_traits.hpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_UTIL_DATA_TRAITS_H_
#define EDI_UTIL_DATA_TRAITS_H_

#include <cxxabi.h>

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "util/namespace.h"

namespace open_edi {
namespace util {

using Bits   = uint64_t;
using Bits8  = uint8_t;
using Bits16 = uint16_t;
using Bits32 = uint32_t;
using Bits64 = uint64_t;

using ObjectId = uint64_t;
using SymbolIndex = uint64_t; // Symbol (string) index,
                              // consistent with SymbolTable
using ObjectIndex = uint32_t; // Polygon, layer, rule, bus etc.
using LocType = int32_t;

//typedef uint32_t  Bits; //conflict...
#if 0
typedef int32_t   Int32;
typedef uint32_t  UInt32;
typedef uint64_t  UInt64;
typedef int64_t   Long;
typedef uint64_t  ULong;
#endif
using Int32 = int32_t;
using UInt32 = uint32_t;
using UInt64 = uint64_t;
using Long = int64_t;
using ULong = uint64_t;

/// @brief Data traits.
/// define a template class of data traits
/// which will make it easier for generic change of data type
template <typename T>
struct CoordinateTraits;

/// @brief Data traits specialization for int32_t
template <>
struct CoordinateTraits<int32_t> {
  using CoordinateType = int32_t;
  using EuclidieanDistanceType = float;
  using ManhattanDistanceType = int64_t;
  using AreaType = int64_t;
  using IndexType = uint64_t;  ///< index (id)
  using WeightType = float;    ///< type for net or node weights
  using FactorType = uint32_t;   ///< type for factor using in tech
};
/// @brief Data traits specialization for float
template <>
struct CoordinateTraits<float> {
  using CoordinateType = float;
  using EuclidieanDistanceType = double;
  using ManhattanDistanceType = double;
  using AreaType = double;
  using IndexType = uint64_t;  ///< index (id)
  using WeightType = float;    ///< type for net or node weights
  using FactorType = uint32_t;   ///< type for factor using in tech
};
/// @brief Data traits specialization for double
template <>
struct CoordinateTraits<double> {
  using CoordinateType = double;
  using EuclidieanDistanceType = long double;
  using ManhattanDistanceType = long double;
  using AreaType = long double;
  using IndexType = uint64_t;  ///< index (id)
  using WeightType = double;   ///< type for net or node weights
  using FactorType = uint64_t;   ///< type for factor using in tech
};

/// @brief A helper function to get the demangled class name of a type
template <typename T>
inline const std::string classNameImpl() {
  int32_t status;
  auto demangled =
      std::string(abi::__cxa_demangle(typeid(T).name(), 0, 0, &status));

  std::string remove_space;
  remove_space.reserve(demangled.size());
  for (auto const& v : demangled) {
    if (!std::isspace(v)) {
      remove_space.append(1, v);
    }
  }
  return remove_space;
}

/// @brief A helper function to get the demangled class name of an object
template <typename T>
inline const std::string className(T const&) {
  return classNameImpl<T>();
}

}  // namespace util
}  // namespace open_edi

#endif
