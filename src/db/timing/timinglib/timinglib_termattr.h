/**
 * @file timinglib_termattr.h
 * @date 2020-08-26
 * @brief TTerm Attributes
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TERMATTR_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TERMATTR_H_

#include <algorithm>
#include <string>
#include <utility>

#include "db/core/object_attr.h"
#include "db/timing/timinglib/timinglib_commondef.h"

namespace open_edi {
namespace db {

class TTermAttr : public ObjectAttr {
  public:
    /// @brief default constructor
    TTermAttr();

    /// @brief copy constructor
    TTermAttr(TTermAttr const &rhs);

    /// @brief move constructor
    TTermAttr(TTermAttr &&rhs) noexcept;

    /// @brief copy assignment
    TTermAttr &operator=(TTermAttr const &rhs);

    /// @brief move constructor
    TTermAttr &operator=(TTermAttr &&rhs) noexcept;

    /// set
    void set_always_on(bool b);
    void set_clock(bool b);
    void set_clock_gate_clock_pin(bool b);
    void set_clock_gate_enable_pin(bool b);
    void set_three_state(bool b);
    void set_bundle(bool b);
    void set_bus(bool b);
    void set_dummy(bool b);
    void set_direction(PinDirection d);
    void set_capacitance(float f);
    void set_rise_capacitance(float f);
    void set_fall_capacitance(float f);
    void set_max_capacitance(float f);
    void set_min_capacitance(float f);
    void set_max_fanout(float f);
    void set_min_fanout(float f);
    void set_max_transition(float f);
    void set_min_transition(float f);
    void set_min_period(float f);
    void set_min_pulse_width_high(float f);
    void set_min_pulse_width_low(float f);
    void set_name(const std::string &name);

    /// get
    bool is_always_on(void);
    bool is_clock(void);
    bool is_clock_gate_clock_pin(void);
    bool is_clock_gate_enable_pin(void);
    bool is_three_state(void);
    bool is_bundle(void);
    bool is_bus(void);
    bool is_dummy(void);
    PinDirection get_direction(void);
    float get_capacitance(void);
    float get_rise_capacitance(void);
    float get_fall_capacitance(void);
    float get_max_capacitance(void);
    float get_min_capacitance(void);
    float get_max_fanout(void);
    float get_min_fanout(void);
    float get_max_transition(void);
    float get_min_transition(void);
    float get_min_period(void);
    float get_min_pulse_width_high(void);
    float get_min_pulse_width_low(void);
    std::string get_name(void) const;
    SymbolIndex get_name_index(void);

    /// @brief clone current object for extensibility.
    /// Users need to manage the pointer to avoid memory leakage.
    /// @return the pointer to the new object
    TTermAttr *clone() const;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

  protected:
    /// @brief copy object
    void copy(TTermAttr const &rhs);
    /// @brief move object
    void move(TTermAttr &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TTermAttr const &rhs);

    bool is_always_on_;
    bool is_clock_;
    bool is_clock_gate_clock_pin_;
    bool is_clock_gate_enable_pin_;
    bool is_three_state_;
    bool is_bundle_;
    bool is_bus_;
    bool is_dummy_;
    PinDirection direction_;
    float capacitance_;
    float rise_capacitance_;
    float fall_capacitance_;
    float max_capacitance_;
    float min_capacitance_;
    float max_fanout_;
    float min_fanout_;
    float max_transition_;
    float min_transition_;
    float min_period_;
    float min_pulse_width_high_;
    float min_pulse_width_low_;
    SymbolIndex name_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TERMATTR_H_
