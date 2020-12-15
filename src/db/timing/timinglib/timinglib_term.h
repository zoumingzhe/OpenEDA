/**
 * @file timinglib_term.h
 * @date 2020-08-26
 * @brief TTerm Class
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TERM_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TERM_H_

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>

#include "db/core/object.h"
#include "db/timing/timinglib/timinglib_commondef.h"
#include "db/util/array.h"

namespace open_edi {
namespace db {

class TFunction;
class TPgTerm;
class TimingArc;

class TTerm : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    TTerm();

    /// @brief destructor
    ~TTerm();

    /// @brief constructor
    TTerm(Object *owner, IndexType id);

    /// @brief copy constructor
    TTerm(TTerm const &rhs);

    /// @brief move constructor
    TTerm(TTerm &&rhs) noexcept;

    /// @brief copy assignment
    TTerm &operator=(TTerm const &rhs);

    /// @brief move constructor
    TTerm &operator=(TTerm &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    void setAlwaysOn(bool b);
    void setClock(bool b);
    void setClockGateClockPin(bool b);
    void setClockGateEnablePin(bool b);
    void setThreeState(bool b);
    void setBundle(bool b);
    void setBus(bool b);
    void setDummy(bool b);
    void setDirection(PinDirection d);
    void setCapacitance(float f);
    void setRiseCapacitance(float f);
    void setFallCapacitance(float f);
    void setMaxCapacitance(float f);
    void setMinCapacitance(float f);
    void setMaxFanout(float f);
    void setMinFanout(float f);
    void setMaxTransition(float f);
    void setMinTransition(float f);
    void setMinPeriod(float f);
    void setMinPulseWidthHigh(float f);
    void setMinPulseWidthLow(float f);
    void setName(const std::string &name);
    void setRelatedPowerPin(ObjectId id);
    void setRelatedGroundPin(ObjectId id);
    TFunction *setFunction(const std::string &str);
    void addMemberPin(ObjectId id);
    void addTimingarc(ObjectId id);

    bool isAlwaysOn(void);
    bool isClock(void);
    bool isClockGateClockPin(void);
    bool isClockGateEnablePin(void);
    bool isThreeState(void);
    bool isBundle(void);
    bool isBus(void);
    bool isDummy(void);
    PinDirection getDirection(void);
    float getCapacitance(void);
    float getRiseCapacitance(void);
    float getFallCapacitance(void);
    float getMaxCapacitance(void);
    float getMinCapacitance(void);
    float getMaxFanout(void);
    float getMinFanout(void);
    float getMaxTransition(void);
    float getMinTransition(void);
    float getMinPeriod(void);
    float getMinPulseWidthHigh(void);
    float getMinPulseWidthLow(void);
    std::string getName(void) const;
    SymbolIndex getNameIndex(void);
    TFunction *getFunction(void);
    TimingArc *getTimingarc(ObjectId id);
    TPgTerm *getRelatedPowerPin(void) const;
    TPgTerm *getRelatedGroundPin(void) const;

    /// @brief output the information
    void print(std::ostream &stream);

  protected:
    /// @brief copy object
    void copy(TTerm const &rhs);
    /// @brief move object
    void move(TTerm &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TTerm const &rhs);

  private:
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
    ObjectId related_power_pin_;
    ObjectId related_ground_pin_;
    ObjectId function_;
    ObjectId member_pins_;
    ObjectId timing_arcs_;
    std::unordered_map<SymbolIndex, ObjectId> member_pins_map_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TERM_H_
