/* @file  db.cpp
 * @date  Aug 2020
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NiiCEDA Inc.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "db/core/db.h"

#include "db/util/symbol_table.h"
#include "util/polygon_table.h"

namespace open_edi {
namespace db {

using Version = open_edi::util::Version;

// top cell contains all objects in library and design
static Cell *kTopCell = nullptr;
static bool kIsTopCellInitialized = false;
static Version kCurrentVersion;

/// @brief resetTopCell
void resetTopCell() {
    MemPool::initMemPool();
    kTopCell = nullptr;
    kCurrentVersion.reset();
}

/// @brief initTopCell
///
/// @return
bool initTopCell() {
    if (kIsTopCellInitialized) return true;

    MemPagePool *pool = MemPool::newPagePool();
    if (pool == nullptr) return false;

    ObjectId cell_id = 0;
    kTopCell = pool->allocate<Cell>(kObjectTypeCell, cell_id);
    kTopCell->setId(cell_id);
    kTopCell->setOwner(cell_id);
    kTopCell->setObjectType(kObjectTypeCell);
    kTopCell->setCellType(CellType::kHierCell);
    kTopCell->setPool(pool);
    MemPool::insertPagePool(cell_id, pool);

    Tech *tech_lib = kTopCell->createObject<Tech>(kObjectTypeTech);

    if (nullptr == tech_lib) {
        resetTopCell();
        return false;
    }
    kTopCell->setTechLib(tech_lib);

    Floorplan *floorplan = kTopCell->createFloorplan();
    if (!floorplan) {
        message->issueMsg(
            kError, "Create floorplan failed when initializing top cell.\n");
        return false;
    }
    floorplan->setCell(cell_id);

    SymbolTable *st = new SymbolTable;
    if (nullptr == st) {
        resetTopCell();
        return false;
    }
    kTopCell->setSymbolTable(st);
    PolygonTable *pt = new PolygonTable();
    kTopCell->setPolygonTable(pt);

    kIsTopCellInitialized = true;
    kCurrentVersion.init();

    return true;
}

/// @brief getTopCell
///
/// @return
Cell *getTopCell() { return kTopCell; }

/// @brief  setTopCell
///
/// @return
void setTopCell(ObjectId top_cell_id) {
    if (top_cell_id == 0) return;
    kTopCell = (Cell *)Object::addr<Cell>(top_cell_id);
}

/// @brief getCurrentVersion
///
/// @return
Version &getCurrentVersion() { return kCurrentVersion; }

/// @brief  setCurrentVersion
///
/// @return
void setCurrentVersion(Version &v) { kCurrentVersion.set(v); }

/// @brief  getCell
///
/// @return
Cell *getCell() {
    Cell *cell = getTopCell();
    return cell;
}

/// @brief  creatBox
///
/// @return
Box *creatBox() {
    Box *box = getCell()->createObject<Box>(kObjectTypeBox);
    return box;
}

/// @brief  creatBox
///
/// @return
Box *creatBox(int llx, int lly, int urx, int ury) {
    Box *box = getCell()->createObject<Box>(kObjectTypeBox);
    box->setLLX(llx);
    box->setLLY(lly);
    box->setURX(urx);
    box->setURY(ury);

    return box;
}

}  // namespace db
}  // namespace open_edi
