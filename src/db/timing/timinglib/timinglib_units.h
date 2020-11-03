/**
 * @file timinglib_units.h
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
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_UNITS_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_UNITS_H_

#include <algorithm>
#include <string>
#include <utility>

#include "db/core/object.h"
#include "db/tech/type_def.h"
#include "util/util.h"

namespace open_edi {
namespace db {

typedef struct TUnit {
    float scale = 1.0;
    int digits = 0;
    std::string suffix = "";

    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TUnit const &rhs) {
        os << DataTypeName(className(rhs)) << DataBegin("(");

        os << DataFieldName("scale") << rhs.scale << DataDelimiter();
        os << DataFieldName("digits") << rhs.digits << DataDelimiter();
        os << DataFieldName("suffix") << rhs.suffix;
        os << DataEnd(")");
        return os;
    }
} TUnit;

/// @brief class unit.
class TUnits : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    TUnits();

    /// @brief copy constructor
    TUnits(TUnits const &rhs);

    /// @brief move constructor
    TUnits(TUnits &&rhs) noexcept;

    /// @brief copy assignment
    TUnits &operator=(TUnits const &rhs);

    /// @brief move constructor
    TUnits &operator=(TUnits &&rhs) noexcept;

    /// @brief clone current object for extensibility.
    /// Users need to manage the pointer to avoid memory leakage.
    /// @return the pointer to the new object
    TUnits *clone() const;

    /// @brief summarize memory usage of the object in bytes
    UInt32 memory() const;

    /// Access APIs:
    TUnit &get_time_unit(void);
    TUnit &get_capacitance_unit(void);
    TUnit &get_volatge_unit(void);
    TUnit &get_resistance_unit(void);
    TUnit &get_pulling_resistance_unit(void);
    TUnit &get_current_unit(void);
    TUnit &get_power_unit(void);
    TUnit &get_distance_unit(void);
    TUnit &get_scalar_unit(void);

    // Set:
    void set_time_unit(const TUnit &unit);
    void set_capacitance_unit(const TUnit &unit);
    void set_volatge_unit(const TUnit &unit);
    void set_resistance_unit(const TUnit &unit);
    void set_pulling_resistance_unit(const TUnit &unit);
    void set_current_unit(const TUnit &unit);
    void set_power_unit(const TUnit &unit);
    void set_distance_unit(const TUnit &unit);
    void set_scalar_unit(const TUnit &unit);

  protected:
    /// @brief copy object
    void copy(TUnits const &rhs);
    /// @brief move object
    void move(TUnits &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TUnits const &rhs);

  private:
    void initValues();

    TUnit time_unit_;
    TUnit capacitance_unit_;
    TUnit volatge_unit_;
    TUnit resistance_unit_;
    TUnit pulling_resistance_unit_;
    TUnit current_unit_;
    TUnit power_unit_;
    TUnit distance_unit_;
    TUnit scalar_unit_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_UNITS_H_
