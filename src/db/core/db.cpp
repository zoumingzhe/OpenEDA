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

#include "db/core/object.h"
#include "db/core/root.h"
#include "db/util/array.h"
#include "db/util/symbol_table.h"
#include "util/polygon_table.h"

namespace open_edi {
namespace db {

using Version = open_edi::util::Version;

// top cell contains all objects in library and design
static Cell *kTopCell = nullptr;
static bool kIsTopCellInitialized = false;
static Version kCurrentVersion;
static Root &kRoot = Root::getInstance();

/// @brief resetTopCell:
//    Note currently the root is also reset. might be changed someday.
void resetTopCell() {
    // MemPool::initMemPool();
    kRoot.reset();
    kTopCell = nullptr;
    kCurrentVersion.reset();
}

/// @brief initTopCell
///
/// @return
bool initTopCell() {
    if (kIsTopCellInitialized) return true;

    kRoot.initTopCell();
    kRoot.initTechLib();
    kRoot.initTimingLib();
    kTopCell = kRoot.getTopCell();

    Floorplan *floorplan = kTopCell->createFloorplan();
    if (!floorplan) {
        message->issueMsg(
            kError, "Create floorplan failed when initializing top cell.\n");
        return false;
    }

    kIsTopCellInitialized = true;
    kCurrentVersion.init();

    return true;
}

/// @brief getRoot
///
/// @return
Root *getRoot() { return &kRoot; }

/// @brief getTopCell
///
/// @return
Cell *getTopCell() { return kTopCell; }

/// @brief getTechLib
///
/// @return
Tech *getTechLib() { return kRoot.getTechLib(); }

/// @brief getTimingLib
///
/// @return
Timing *getTimingLib() { return kRoot.getTimingLib(); }

/// @brief  setTopCell
///
/// @return
void setTopCell(ObjectId top_cell_id) {
    if (top_cell_id == 0) return;
    kTopCell = (Cell *)Object::addr<Cell>(top_cell_id);
    kRoot.setTopCell(kTopCell);
}

/// @brief  setTechLib
///
/// @return
void setTechLib(ObjectId tech_lib_id) {
    if (tech_lib_id == 0) return;
    Tech *tech = (Tech *)Object::addr<Tech>(tech_lib_id);
    kRoot.setTechLib(tech);
}

/// @brief  setTimingLib
///
/// @return
void setTimingLib(ObjectId timing_lib_id) {
    if (timing_lib_id == 0) return;
    Timing *timing = (Timing *)Object::addr<Timing>(timing_lib_id);
    kRoot.setTimingLib(timing);
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

}  // namespace db
}  // namespace open_edi
