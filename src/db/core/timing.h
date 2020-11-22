/**
 * @file  timinglib.h
 * @date  11/19/2020 01:38:36 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_DB_CORE_TIMING_H_
#define SRC_DB_CORE_TIMING_H_

#include <string>
#include <vector>

#include "db/core/cell.h"
#include "db/tech/tech.h"
#include "db/core/term.h"

#include "db/util/symbol_table.h"
#include "util/polygon_table.h"
#include "util/util.h"

namespace open_edi {
namespace db {

class AnalysisView;
class AnalysisMode;
class AnalysisCorner;
class StorageUtil;

/// @brief class Timing for timingLib
class Timing : public Object {
  public:
    Timing();
    ~Timing();

    MemPagePool *getPool() const;
    void setPool(MemPagePool *p);
    
    SymbolTable *getSymbolTable();
    void setSymbolTable(SymbolTable *stb);

    PolygonTable *getPolygonTable();
    void setPolygonTable(PolygonTable *pt);

    StorageUtil* getStorageUtil() const;
    void setStorageUtil(StorageUtil *v);

    std::string &getSymbolByIndex(SymbolIndex index);
    SymbolIndex getOrCreateSymbol(const char *name);
    SymbolIndex getOrCreateSymbol(std::string &name);
    bool addSymbolReference(SymbolIndex index, ObjectId owner);

    // timinglib
    AnalysisMode *getAnalysisMode(std::string name);
    AnalysisCorner *getAnalysisCorner(std::string name);
    AnalysisView *getAnalysisView(std::string name);
    AnalysisView *getAnalysisView(size_t idx) const;
    void addActiveSetupView(ObjectId id);
    void addActiveHoldView(ObjectId id);
    AnalysisMode *createAnalysisMode(std::string &name);
    AnalysisCorner *createAnalysisCorner(std::string &name);
    AnalysisView *createAnalysisView(std::string &name);
    // timinglib
    
  private:
    // timinglib
    ObjectId analysis_modes_;
    ObjectId analysis_corners_;
    ObjectId analysis_views_;
    ObjectId active_setup_views_;
    ObjectId active_hold_views_;
    // timinglib
    StorageUtil *storage_util_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_CORE_TIMING_H_
