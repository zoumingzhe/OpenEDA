/**
 * @file timinglib_scalefactors.h
 * @date 2020-10-09
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
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_SCALEFACTORS_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_SCALEFACTORS_H_

#include <algorithm>
#include <string>
#include <utility>

#include "db/core/object.h"
#include "db/timing/timinglib/timinglib_commondef.h"

namespace open_edi {
namespace db {

class ScaleFactors : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    ScaleFactors();

    /// @brief destructor
    ~ScaleFactors();

    /// @brief constructor
    ScaleFactors(Object *owner, IndexType id);

    /// @brief copy constructor
    ScaleFactors(ScaleFactors const &rhs);

    /// @brief move constructor
    ScaleFactors(ScaleFactors &&rhs) noexcept;

    /// @brief copy assignment
    ScaleFactors &operator=(ScaleFactors const &rhs);

    /// @brief move assignment
    ScaleFactors &operator=(ScaleFactors &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    void set_name(const std::string &name);
    void add_scale(ScaleFactorType t, ScaleFactorPvt p, float f);
    void add_scale(ScaleFactorType t, ScaleFactorPvt p, ScaleFactorRiseFall rf,
                   float f);

    /// get
    std::string get_name(void) const;
    float get_scale(ScaleFactorType t, ScaleFactorPvt p);
    float get_scale(ScaleFactorType t, ScaleFactorPvt p,
                    ScaleFactorRiseFall rf);

    static bool is_rise_fall_suffix(ScaleFactorType type);
    static bool is_rise_fall_prefix(ScaleFactorType type);
    static bool is_high_low_suffix(ScaleFactorType type);

  protected:
    /// @brief copy object
    void copy(ScaleFactors const &rhs);
    /// @brief move object
    void move(ScaleFactors &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, ScaleFactors const &rhs);

  private:
    void __initValue();

    SymbolIndex name_;
    float scales_[static_cast<int>(ScaleFactorType::kUnknown)]
                 [static_cast<int>(ScaleFactorPvt::kUnknown)]
                 [static_cast<int>(ScaleFactorRiseFall::kUnknown)];
};
}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_SCALEFACTORS_H_
