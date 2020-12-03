/**
 * @file analysis_view.h
 * @date 2020-09-08
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
#ifndef SRC_DB_TIMING_TIMINGLIB_ANALYSIS_VIEW_H_
#define SRC_DB_TIMING_TIMINGLIB_ANALYSIS_VIEW_H_

#include <algorithm>
#include <string>
#include <utility>

#include "db/core/object.h"
#include "util/data_traits.h"

namespace open_edi {
namespace db {

class AnalysisCorner;
class AnalysisMode;
class AnalysisView : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    AnalysisView();

    /// @brief destructor
    ~AnalysisView();

    /// @brief constructor
    AnalysisView(Object *owner, IndexType id);

    /// @brief copy constructor
    AnalysisView(AnalysisView const &rhs);

    /// @brief move constructor
    AnalysisView(AnalysisView &&rhs) noexcept;

    /// @brief copy assignment
    AnalysisView &operator=(AnalysisView const &rhs);

    /// @brief move assignment
    AnalysisView &operator=(AnalysisView &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    void setName(const std::string &name);

    void setAnalysisCorner(ObjectId id);
    void setAnalysisMode(ObjectId id);
    void setActive(bool b);
    void setSetup(bool b);
    void setHold(bool b);
    void setMaxCap(bool b);
    void setMaxTran(bool b);
    void setMinCap(bool b);
    void setLeakagePower(bool b);
    void setDynamicPower(bool b);
    void setCellEm(bool b);
    void setSignalEm(bool b);

    /// get
    SymbolIndex getNameIndex(void);
    std::string getName(void) const;

    AnalysisCorner *getAnalysisCorner(void);
    AnalysisMode *getAnalysisMode(void);
    bool isActive(void);
    bool isSetup(void);
    bool isHold(void);
    bool isMaxCap(void);
    bool isMaxTran(void);
    bool isMinCap(void);
    bool isLeakagePower(void);
    bool isDynamicPower(void);
    bool isCellEm(void);
    bool isSignalEm(void);

  protected:
    /// @brief copy object
    void copy(AnalysisView const &rhs);
    /// @brief move object
    void move(AnalysisView &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, AnalysisView const &rhs);

  private:
    SymbolIndex name_;
    ObjectId analysis_corner_;
    ObjectId analysis_mode_;
    bool is_active_;
    bool is_setup_;
    bool is_hold_;
    bool is_max_cap_;
    bool is_max_tran_;
    bool is_min_cap_;
    bool is_leakage_power_;
    bool is_dynamic_power_;
    bool is_cell_em_;
    bool is_signal_em_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_ANALYSIS_VIEW_H_
