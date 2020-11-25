/**
 * @file  root.h
 * @date  11/19/2020 01:38:36 PM CST
 * @brief Class Root and StorageUtil
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_DB_CORE_ROOT_H_
#define SRC_DB_CORE_ROOT_H_

#include <string>
#include <vector>

#include "db/core/cell.h"
#include "db/tech/tech.h"
#include "db/core/timing.h"

#include "db/util/symbol_table.h"
#include "util/polygon_table.h"
#include "util/util.h"

namespace open_edi {
namespace db {
#if 0
class Timing;
class Cell;
class Tech;
class ObjectId;
class SymbolTable;
class PolygonTable;
class MemPagePool;
#endif
class Timing;

class StorageUtil {
  public:
    StorageUtil();
    StorageUtil(ObjectId id);

    ~StorageUtil();

    void setSymbolTable(SymbolTable *stb);
    SymbolTable *getSymbolTable() const;
    void setPolygonTable(PolygonTable *pt);
    PolygonTable *getPolygonTable() const;
    void setPool(MemPagePool *p);
    MemPagePool *getPool() const;

  private:
    MemPagePool *pool_;  ///< use the memory pool to allocate object
    SymbolTable *symtbl_;
    PolygonTable *polytbl_;
};

/// @brief root class: runtime
class Root {
  public:
    static Root & getInstance() {
        static Root root_inst;
        return root_inst;
    }
    void setTechLib(Tech *v);
    Tech* getTechLib() const;
    void setTimingLib(Timing *v);
    Timing* getTimingLib() const;
    void setTopCell(Cell *v);
    Cell* getTopCell() const;
    
  private:
    Root();
    ~Root();

    Tech *tech_;
    Timing *timing_;
    Cell *top_cell_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_CORE_ROOT_H_
