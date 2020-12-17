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
    TUnit &getTimeUnit(void);
    TUnit &getCapacitanceUnit(void);
    TUnit &getVoltageUnit(void);
    TUnit &getResistanceUnit(void);
    TUnit &getPullingResistanceUnit(void);
    TUnit &getCurrentUnit(void);
    TUnit &getPowerUnit(void);
    TUnit &getDistanceUnit(void);
    TUnit &getScalarUnit(void);

    // Set:
    void setTimeUnit(const TUnit &unit);
    void setCapacitanceUnit(const TUnit &unit);
    void setVolatgeUnit(const TUnit &unit);
    void setResistanceUnit(const TUnit &unit);
    void setPullingResistanceUnit(const TUnit &unit);
    void setCurrentUnit(const TUnit &unit);
    void setPowerUnit(const TUnit &unit);
    void setDistanceUnit(const TUnit &unit);
    void setScalarUnit(const TUnit &unit);

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
    TUnit voltage_unit_;
    TUnit resistance_unit_;
    TUnit pulling_resistance_unit_;
    TUnit current_unit_;
    TUnit power_unit_;
    TUnit distance_unit_;
    TUnit scalar_unit_;
};

class UnitMultiply {
  public:
    static float getTimeUnitMultiply(const std::string &u);
    static float getResistanceUnitMultiply(const std::string &u);
    static float getCapacitiveUnitMultiply(const std::string &u);
    static float getVoltageUnitMultiply(const std::string &u);
    static float getCurrentUnitMultiply(const std::string &u);
    static float getPowerUnitMultiply(const std::string &u);
    static float getDistanceUnitMultiply(const std::string &u);
};


}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_UNITS_H_
