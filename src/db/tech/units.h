/* @file  units.h
 * @date  08/12/2020 06:33:49 PM CST
 * @brief Class Units.
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_DB_TECH_UNIT_H_
#define EDI_DB_TECH_UNIT_H_

#include "db/core/object.h"
#include "db/tech/type_def.h"
#include "util/util.h"

namespace open_edi {
namespace db {

/**
 * @class Units
 * @brief class Units is for: UNIT
 * [UNITS
 *      [TIME NANOSECONDS convertFactor ;]
 *      [CAPACITANCE PICOFARADS convertFactor ;]
 *      [RESISTANCE OHMS convertFactor ;]
 *      [POWER MILLIWATTS convertFactor ;]
 *      [CURRENT MILLIAMPS convertFactor ;]
 *      [VOLTAGE VOLTS convertFactor ;]
 *      [DATABASE MICRONS LEFconvertFactor ;]
 *      [FREQUENCY MEGAHERTZ convertFactor ;]
 *  END UNITS]
 * */

/// @brief class unit.
class Units : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    Units();

    /// @brief copy constructor
    Units(Units const &rhs);

    /// @brief move constructor
    Units(Units &&rhs) noexcept;

    /// @brief copy assignment
    Units &operator=(Units const &rhs);

    /// @brief move constructor
    Units &operator=(Units &&rhs) noexcept;

    /// @brief clone current object for extensibility.
    /// Users need to manage the pointer to avoid memory leakage.
    /// @return the pointer to the new object
    // Units *clone() const;

    /// @brief summarize memory usage of the object in bytes
    UInt32 memory() const;
    /// Access APIs:
    bool getFromDEF() const { return from_def_; }
    UInt32 getCapacitanceFactor() const { return capacitance_factor_; }
    UInt32 getCurrentFactor() const { return current_factor_; }
    UInt32 getPowerFactor() const { return power_factor_; }
    UInt32 getResistanceFactor() const { return resistance_factor_; }
    UInt32 getVoltageFactor() const { return voltage_factor_; }
    UInt32 getTimeFactor() const { return time_factor_; }
    UInt32 getFrequencyFactor() const { return frequency_factor_; }
    UInt32 getLengthFactor() const { return length_factor_; }
    double getMicronPerDBU() const { return microns_per_dbu_; }

    // Set:
    void setFromDEF(bool v) { from_def_ = v; }
    void setLengthFactor(UInt32 v) {
        length_factor_ = v;
        microns_per_dbu_ = 1.0 / length_factor_;
    }
    void setCapacitanceFactor(UInt32 v) { capacitance_factor_ = v; }
    void setCurrentFactor(UInt32 v) { current_factor_ = v; }
    void setPowerFactor(UInt32 v) { power_factor_ = v; }
    void setResistanceFactor(UInt32 v) { resistance_factor_ = v; }
    void setVoltageFactor(UInt32 v) { voltage_factor_ = v; }
    void setTimeFactor(UInt32 v) { time_factor_ = v; }
    void setFrequencyFactor(UInt32 v) { frequency_factor_ = v; }

    // Print:
    void printLEF(std::ofstream &ofs) const;

    // string related:
    SymbolIndex getCapacitanceUnitsSymIndex() const {
        return capacitance_unit_;
    }
    SymbolIndex getCurrentUnitsSymIndex() const { return current_unit_; }
    SymbolIndex getPowerUnitsSymIndex() const { return power_unit_; }
    SymbolIndex getResistanceUnitsSymIndex() const { return resistance_unit_; }
    SymbolIndex getVoltageUnitsSymIndex() const { return voltage_unit_; }
    SymbolIndex getTimeUnitsSymIndex() const { return time_unit_; }
    SymbolIndex getFrequencyUnitsSymIndex() const { return frequency_unit_; }
    SymbolIndex getLengthUnitsSymIndex() const { return length_unit_; }

    const char *getCapacitanceUnits() const;
    const char *getCurrentUnits() const;
    const char *getPowerUnits() const;
    const char *getResistanceUnits() const;
    const char *getVoltageUnits() const;
    const char *getTimeUnits() const;
    const char *getFrequencyUnits() const;
    const char *getLengthUnits() const;

    void setCapacitanceUnits(const char *v);
    void setCurrentUnits(const char *v);
    void setPowerUnits(const char *v);
    void setResistanceUnits(const char *v);
    void setVoltageUnits(const char *v);
    void setTimeUnits(const char *v);
    void setFrequencyUnits(const char *v);
    void setLengthUnits(const char *v);

  protected:
    /// @brief copy object
    void copy(Units const &rhs);
    /// @brief move object
    void move(Units &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, Units const &rhs);
    /// @brief overload input stream
    friend IStreamBase &operator>>(IStreamBase &is, Units &rhs) {return is;}

  private:
    void __init();

    bool from_def_;

    UInt32 capacitance_factor_;  /// In LEF’s UNITS section: CAPACITANCE
                                 /// PICOFARADS 10
    UInt32 current_factor_;  /// In LEF’s UNITS section: CURRENT MILLIAMPS 10000
    UInt32 power_factor_;  /// In LEF’s UNITS section: POWER MILLIWATTS 10000
    UInt32
        resistance_factor_;  /// In LEF’s UNITS section: RESISTANCE OHMS 10000
    UInt32 voltage_factor_;  /// In LEF’s UNITS section: VOLTAGE VOLTS 1000
    UInt32 time_factor_;     /// In LEF’s UNITS section: TIME NANOSECONDS 100
    UInt32
        frequency_factor_;  /// In LEF’s UNITS section: FREQUENCY MEGAHERTZ 10
    UInt32 length_factor_;  /// In LEF’s UNITS section: DATABASE MICRONS 20000
    double microns_per_dbu_;  // micron per DBU
    SymbolIndex capacitance_unit_;
    SymbolIndex current_unit_;
    SymbolIndex power_unit_;
    SymbolIndex resistance_unit_;
    SymbolIndex voltage_unit_;
    SymbolIndex time_unit_;
    SymbolIndex frequency_unit_;
    SymbolIndex length_unit_;
};

}  // namespace db
}  // namespace open_edi

#endif
