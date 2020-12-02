/**
 * @file timinglib_cell.h
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
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_CELL_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_CELL_H_

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
class TPgTerm;

class TCell : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    TCell();

    /// @brief destructor
    ~TCell();

    /// @brief constructor
    TCell(Object *owner, IndexType id);

    /// @brief copy constructor
    TCell(TCell const &rhs);

    /// @brief move constructor
    TCell(TCell &&rhs) noexcept;

    /// @brief copy assignment
    TCell &operator=(TCell const &rhs);

    /// @brief move constructor
    TCell &operator=(TCell &&rhs) noexcept;

    /// @brief number of terms
    IndexType numTerms() const;

    IndexType numPgTerms() const;

    /// @brief get or create a instance terminal
    TTerm *getOrCreateTerm(const std::string &name);
    TTerm *getTerm(const std::string &name);
    std::vector<TTerm *> getTerms(void);
    void resetTerms(const std::vector<TTerm *> &terms);

    TPgTerm *getOrCreatePgTerm(const std::string &name);
    TPgTerm *getPgTerm(const std::string &name);
    std::vector<TPgTerm *> getPgTerms(void);

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    void setAlwaysOn(bool b);
    void setDontTouch(bool b);
    void setDontUse(bool b);
    void setPad(bool b);
    void setMacroCell(bool b);
    void setClockGatingCell(bool b);
    void setClockIsolationCell(bool b);
    void setIsolationCell(bool b);
    void setLevelShifter(bool b);
    void setEnableLevelShifter(bool b);
    void setDecapCell(bool b);
    void setFillerCell(bool b);
    void setTapCell(bool b);
    void setDiodeCell(bool b);
    void setPowerSwitch(bool b);
    void setRetentionCell(bool b);
    void setSequential(bool b);
    // void setPhysicalOnly(bool b);
    // void setCombinational(bool b);
    // void setHasMultiPowerRails(bool b);
    void setIntegratedClockGatingCell(bool b);
    void setThreeState(bool b);
    void setAntennaDiodeType(AntennaDiodeType t);
    void setSwitchCellType(SwitchCellType t);
    void setThresholdVotageGroup(ThresholdVotageGroup t);
    void setClockGatingIntegratedCell(ClockGateIntegratedType t);
    void setName(const std::string &name);
    void setCellFootprint(const std::string &cf);
    void setRetentionCell(const std::string &rc);
    void setArea(float f);
    void setCellLeakagePower(float f);
    bool isPhysicalOnly(void);
    bool isCombinational(void);
    bool hasMultiPowerRails(void);

    void print(std::ostream &stream);

    bool isAlwaysOn(void);
    bool isDontTouch(void);
    bool isDontUse(void);
    bool isPad(void);
    bool isMacroCell(void);
    bool isClockGatingCell(void);
    bool isClockIsolationCell(void);
    bool isIsolationCell(void);
    bool isLevelShifter(void);
    bool isEnableLevelShifter(void);
    bool isDecapCell(void);
    bool isFillerCell(void);
    bool isTapCell(void);
    bool isDiodeCell(void);
    bool isPowerSwitch(void);
    bool isRetentionCell(void);
    bool isSequential(void);
    bool isIntegratedClockGatingCell(void);
    bool isThreeState(void);
    AntennaDiodeType getAntennaDiodeType(void);
    SwitchCellType getSwitchCellType(void);
    ThresholdVotageGroup getThresholdVotageGroup(void);
    ClockGateIntegratedType getClockGatingIntegratedCell(void);
    std::string getName(void) const;
    SymbolIndex getNameIndex(void);
    std::string getCellFootprint(void) const;
    std::string getRetentionCell(void) const;
    float getArea(void);
    float getCellLeakagePower(void);
    std::string getFullName(void);
    IndexType getNumPins(void);

  protected:
    /// @brief copy object
    void copy(TCell const &rhs);
    /// @brief move object
    void move(TCell &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TCell const &rhs);

  private:
    /// @brief add one term
    /// @return the added term
    TTerm *__addTermImpl();
    TPgTerm *__addPgTermImpl();

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
    ObjectId tterms_;
    ObjectId tpg_terms_;
    std::unordered_map<SymbolIndex, ObjectId> term_map_;
    std::unordered_map<SymbolIndex, ObjectId> pg_term_map_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_CELL_H_
