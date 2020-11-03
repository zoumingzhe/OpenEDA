/**
 * @file timinglib_timingarc.h
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
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TIMINGARC_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TIMINGARC_H_

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>

#include "db/core/object.h"
#include "db/timing/timinglib/timinglib_commondef.h"

namespace open_edi {
namespace db {

class TTerm;
class TFunction;
class TimingTable;

class TimingArc : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    TimingArc();

    /// @brief destructor
    ~TimingArc();

    /// @brief constructor
    TimingArc(Object* owner, IndexType id);

    /// @brief copy constructor
    TimingArc(TimingArc const& rhs);

    /// @brief move constructor
    TimingArc(TimingArc&& rhs) noexcept;

    /// @brief copy assignment
    TimingArc& operator=(TimingArc const& rhs);

    /// @brief move assignment
    TimingArc& operator=(TimingArc&& rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    void set_disabled(bool b);
    void set_timing_sense(TimingSense ts);
    void set_timing_type(TimingType tt);
    TFunction* set_when(const std::string& str);
    static TimingTable* create_timing_table(ObjectType type);
    void set_cell_rise(ObjectId id);
    void set_cell_fall(ObjectId id);
    void set_rise_transition(ObjectId id);
    void set_fall_transition(ObjectId id);
    void set_rise_constraint(ObjectId id);
    void set_fall_constraint(ObjectId id);
    void add_related_pin(ObjectId id);

    /// get
    bool is_disabled(void);
    TimingSense get_timing_sense(void);
    TimingType get_timing_type(void);
    TFunction* get_when(void);
    TimingTable* get_cell_rise(void);
    TimingTable* get_cell_fall(void);
    TimingTable* get_rise_transition(void);
    TimingTable* get_fall_transition(void);
    TimingTable* get_rise_constraint(void);
    TimingTable* get_fall_constraint(void);
    TTerm* get_related_pin(const std::string& name);
    TTerm* get_related_pin(ObjectId id);

  protected:
    /// @brief copy object
    void copy(TimingArc const& rhs);
    /// @brief move object
    void move(TimingArc&& rhs);
    /// @brief overload output stream
    friend OStreamBase& operator<<(OStreamBase& os, TimingArc const& rhs);

  private:
    bool is_disabled_;
    TimingSense timing_sense_;
    TimingType timing_type_;
    ObjectId when_;
    ObjectId cell_rise_;
    ObjectId cell_fall_;
    ObjectId rise_transition_;
    ObjectId fall_transition_;
    ObjectId rise_constraint_;
    ObjectId fall_constraint_;
    ObjectId related_pins_;
    std::unordered_map<SymbolIndex, ObjectId> related_pins_map_;
};
}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TIMINGARC_H_
