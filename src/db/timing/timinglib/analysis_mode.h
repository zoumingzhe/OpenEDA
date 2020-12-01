
/**
 * @file analysis_mode.h
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
#ifndef SRC_DB_TIMING_TIMINGLIB_ANALYSIS_MODE_H_
#define SRC_DB_TIMING_TIMINGLIB_ANALYSIS_MODE_H_

#include <algorithm>
#include <string>
#include <utility>

#include "db/core/object.h"
#include "db/util/array.h"
#include "util/data_traits.h"
#include "db/timing/sdc/sdc.h"

namespace open_edi {
namespace db {

class AnalysisMode : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    AnalysisMode();

    /// @brief destructor
    ~AnalysisMode();

    /// @brief constructor
    AnalysisMode(Object *owner, IndexType id);

    /// @brief copy constructor
    AnalysisMode(AnalysisMode const &rhs);

    /// @brief move constructor
    AnalysisMode(AnalysisMode &&rhs) noexcept;

    /// @brief copy assignment
    AnalysisMode &operator=(AnalysisMode const &rhs);

    /// @brief move assignment
    AnalysisMode &operator=(AnalysisMode &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    void set_name(const std::string &name);
    void add_constraint_file(const std::string &file);
    void create_sdc();
    void set_sdc(SdcPtr sdc);

    /// get
    SymbolIndex get_name_index(void);
    std::string get_name(void) const;
    std::vector<std::string> get_constraint_files(void);
    std::string get_constraint_file(SymbolIndex index) const;
    SdcPtr get_sdc(void) const;

    int num_contraint_files(void) const;

  protected:
    /// @brief copy object
    void copy(AnalysisMode const &rhs);
    /// @brief move object
    void move(AnalysisMode &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, AnalysisMode const &rhs);

  private:
    SymbolIndex name_;
    ObjectId constraint_files_;
    SdcPtr sdc_;
};
}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_ANALYSIS_MODE_H_
