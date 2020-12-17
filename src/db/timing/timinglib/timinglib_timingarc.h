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
#include <vector>

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
    void setDisabled(bool b);
    void setTimingSense(TimingSense ts);
    void setTimingType(TimingType tt);
    TFunction* setWhen(const std::string& str);
    static TimingTable* createTimingTable(ObjectType type);
    void setCellRise(ObjectId id);
    void setCellFall(ObjectId id);
    void setRiseTransition(ObjectId id);
    void setFallTransition(ObjectId id);
    void setRiseConstraint(ObjectId id);
    void setFallConstraint(ObjectId id);
    void addRelatedPin(ObjectId id);

    /// get
    bool isDisabled(void);
    TimingSense getTimingSense(void);
    TimingType getTimingType(void);
    TFunction* getWhen(void);
    TimingTable* getCellRise(void);
    TimingTable* getCellFall(void);
    TimingTable* getRiseTransition(void);
    TimingTable* getFallTransition(void);
    TimingTable* getRiseConstraint(void);
    TimingTable* getFallConstraint(void);
    TTerm* getRelatedPin(const std::string& name);
    TTerm* getRelatedPin(ObjectId id);
    std::vector<TTerm*> getRelatedPins(void);

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
