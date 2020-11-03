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
    void set_name(const std::string &name);

    void set_analysis_corner(ObjectId id);
    void set_analysis_mode(ObjectId id);
    void set_active(bool b);
    void set_setup(bool b);
    void set_hold(bool b);
    void set_max_cap(bool b);
    void set_max_tran(bool b);
    void set_min_cap(bool b);
    void set_leakage_power(bool b);
    void set_dynamic_power(bool b);
    void set_cell_em(bool b);
    void set_signal_em(bool b);

    /// get
    SymbolIndex get_name_index(void);
    std::string get_name(void) const;

    AnalysisCorner *get_analysis_corner(void);
    AnalysisMode *get_analysis_mode(void);
    bool is_active(void);
    bool is_setup(void);
    bool is_hold(void);
    bool is_max_cap(void);
    bool is_max_tran(void);
    bool is_min_cap(void);
    bool is_leakage_power(void);
    bool is_dynamic_power(void);
    bool is_cell_em(void);
    bool is_signal_em(void);

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
