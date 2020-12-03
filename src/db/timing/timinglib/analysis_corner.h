/**
 * @file analysis_corner.h
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
#ifndef SRC_DB_TIMING_TIMINGLIB_ANALYSIS_CORNER_H_
#define SRC_DB_TIMING_TIMINGLIB_ANALYSIS_CORNER_H_

#include <algorithm>
#include <string>
#include <utility>

#include "db/core/object.h"
#include "db/timing/spef/design_parasitics.h"
#include "util/data_traits.h"

namespace open_edi {
namespace db {

class LibSet;
class DesignParasitics;

class AnalysisCorner : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    AnalysisCorner();

    /// @brief destructor
    ~AnalysisCorner();

    /// @brief constructor
    AnalysisCorner(Object *owner, IndexType id);

    /// @brief copy constructor
    AnalysisCorner(AnalysisCorner const &rhs);

    /// @brief move constructor
    AnalysisCorner(AnalysisCorner &&rhs) noexcept;

    /// @brief copy assignment
    AnalysisCorner &operator=(AnalysisCorner const &rhs);

    /// @brief move assignment
    AnalysisCorner &operator=(AnalysisCorner &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    void setName(const std::string &name);
    void setRcTechFile(const std::string &file);
    void setLibset(ObjectId id);
    void setDesignParasitics(ObjectId id);

    /// get
    SymbolIndex getNameIndex(void);
    SymbolIndex getRcTechFileIndex(void);
    std::string getName(void) const;
    std::string getRcTechFile(void) const;

    LibSet *getLibset(void);
    DesignParasitics *getDesignParasitics(void);

  protected:
    /// @brief copy object
    void copy(AnalysisCorner const &rhs);
    /// @brief move object
    void move(AnalysisCorner &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, AnalysisCorner const &rhs);

  private:
    SymbolIndex name_;
    SymbolIndex rc_tech_file_;
    ObjectId libset_;
    ObjectId design_parasitics_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_ANALYSIS_CORNER_H_
