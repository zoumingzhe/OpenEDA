/**
 * @file timinglib_cellattr.h
 * @date 2020-08-26
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
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_CELLATTR_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_CELLATTR_H_

#include <algorithm>
#include <string>
#include <utility>

#include "db/core/object_attr.h"
#include "db/timing/timinglib/timinglib_commondef.h"

namespace open_edi {
namespace db {

class TCellAttr : public ObjectAttr {
  public:
    /// @brief default constructor
    TCellAttr();

    /// @brief copy constructor
    TCellAttr(TCellAttr const &rhs);

    /// @brief move constructor
    TCellAttr(TCellAttr &&rhs) noexcept;

    /// @brief copy assignment
    TCellAttr &operator=(TCellAttr const &rhs);

    /// @brief move constructor
    TCellAttr &operator=(TCellAttr &&rhs) noexcept;

    /// set
    void set_always_on(bool b);
    void set_dont_touch(bool b);
    void set_dont_use(bool b);
    void set_pad(bool b);
    void set_macro_cell(bool b);
    void set_clock_gating_cell(bool b);
    void set_clock_isolation_cell(bool b);
    void set_isolation_cell(bool b);
    void set_level_shifter(bool b);
    void set_enable_level_shifter(bool b);
    void set_decap_cell(bool b);
    void set_filler_cell(bool b);
    void set_tap_cell(bool b);
    void set_diode_cell(bool b);
    void set_power_switch(bool b);
    void set_retention_cell(bool b);
    void set_sequential(bool b);
    // void set_physical_only(bool b);
    // void set_combinational(bool b);
    // void set_has_multi_power_rails(bool b);
    void set_integrated_clock_gating_cell(bool b);
    void set_three_state(bool b);
    void set_antenna_diode_type(AntennaDiodeType t);
    void set_switch_cell_type(SwitchCellType t);
    void set_threshold_votage_group(ThresholdVotageGroup t);
    void set_clock_gating_integrated_cell(ClockGateIntegratedType t);
    void set_name(const std::string &name);
    void set_cell_footprint(const std::string &cf);
    void set_retention_cell(const std::string &rc);
    void set_area(float f);
    void set_cell_leakage_power(float f);

    /// get
    bool is_always_on(void);
    bool is_dont_touch(void);
    bool is_dont_use(void);
    bool is_pad(void);
    bool is_macro_cell(void);
    bool is_clock_gating_cell(void);
    bool is_clock_isolation_cell(void);
    bool is_isolation_cell(void);
    bool is_level_shifter(void);
    bool is_enable_level_shifter(void);
    bool is_decap_cell(void);
    bool is_filler_cell(void);
    bool is_tap_cell(void);
    bool is_diode_cell(void);
    bool is_power_switch(void);
    bool is_retention_cell(void);
    bool is_sequential(void);
    bool is_integrated_clock_gating_cell(void);
    bool is_three_state(void);
    AntennaDiodeType get_antenna_diode_type(void);
    SwitchCellType get_switch_cell_type(void);
    ThresholdVotageGroup get_threshold_votage_group(void);
    ClockGateIntegratedType get_clock_gating_integrated_cell(void);
    std::string get_name(void) const;
    SymbolIndex get_name_index(void);
    std::string get_cell_footprint(void) const;
    std::string get_retention_cell(void) const;
    float get_area(void);
    float get_cell_leakage_power(void);

    /// @brief clone current object for extensibility.
    /// Users need to manage the pointer to avoid memory leakage.
    /// @return the pointer to the new object
    TCellAttr *clone() const;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

  protected:
    /// @brief copy object
    void copy(TCellAttr const &rhs);
    /// @brief move object
    void move(TCellAttr &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TCellAttr const &rhs);

    bool always_on_;
    bool dont_touch_;
    bool dont_use_;
    bool is_pad_;
    bool is_macro_cell_;
    bool is_clock_gating_cell_;
    bool is_clock_isolation_cell_;
    bool is_isolation_cell_;
    bool is_level_shifter_;
    bool is_enable_level_shifter_;
    bool is_decap_cell_;
    bool is_filler_cell_;
    bool is_tap_cell_;
    bool is_diode_cell_;
    bool is_power_switch_;
    bool is_retention_cell_;
    bool is_sequential_;
    // bool is_physical_only_;
    // bool is_combinational_;
    // bool has_multi_power_rails_;
    bool is_integrated_clock_gating_cell_;
    bool is_three_state_;
    AntennaDiodeType antenna_diode_type_;
    SwitchCellType switch_cell_type_;
    ThresholdVotageGroup threshold_votage_group_;
    ClockGateIntegratedType clock_gating_integrated_cell_;
    SymbolIndex name_;
    SymbolIndex cell_footprint_;
    SymbolIndex retention_cell_;
    float area_;
    float cell_leakage_power_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_CELLATTR_H_
