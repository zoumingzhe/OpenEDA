 /**
 * @file  root.cpp
 * @date  11/19/2020 01:38:36 PM CST
 * @brief Class Root and StorageUtil.
 *
 * Copyright (C) 2020 NiiCEDA Inc.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "db/core/cell.h"
#include "db/tech/tech.h"
#include "db/core/timing.h"

#include "db/util/symbol_table.h"
#include "util/polygon_table.h"
#include "util/util.h"
#include "db/core/root.h"

namespace open_edi {
namespace db {
// Class Root (runtime object):
Root::Root() {
    MemPool::initMemPool();
    // Top cell:
    StorageUtil *top_cell_storage = new StorageUtil;
    MemPagePool *top_cell_pool = top_cell_storage->getPool();
    if (top_cell_pool == nullptr) return;
    ObjectId cell_id = 0;
    top_cell_ = top_cell_pool->allocate<Cell>(kObjectTypeCell, cell_id);
    top_cell_->setId(cell_id);
    top_cell_->setOwner(cell_id);
    top_cell_->setObjectType(kObjectTypeCell);
    top_cell_->initHierData(top_cell_storage);
    top_cell_->setCellType(CellType::kHierCell);
    // MemPool::insertPagePool(cell_id, top_cell_pool);
    
    // Tech:
    StorageUtil *tech_lib_storage = new StorageUtil;
    MemPagePool *tech_lib_pool = tech_lib_storage->getPool();
    if (tech_lib_pool == nullptr) return;
    ObjectId tech_id;
    tech_ = tech_lib_pool->allocate<Tech>(kObjectTypeTech, tech_id);
    tech_->setId(tech_id);
    tech_->setOwner(tech_id);
    tech_->setObjectType(kObjectTypeTech);
    tech_->setStorageUtil(tech_lib_storage);
    // MemPool::insertPagePool(tech_id, tech_lib_pool);

    // Timing:
    StorageUtil *timing_lib_storage = new StorageUtil;
    MemPagePool *timing_lib_pool = timing_lib_storage->getPool();
    if (timing_lib_pool == nullptr) return;
    ObjectId timing_id;
    timing_ = timing_lib_pool->allocate<Timing>(kObjectTypeTiming, timing_id);
    timing_->setId(timing_id);
    timing_->setOwner(timing_id);
    timing_->setObjectType(kObjectTypeTiming);
    timing_->setStorageUtil(timing_lib_storage);
    // MemPool::insertPagePool(timing_id, timing_lib_pool);
}

Root::~Root() {
    delete top_cell_;
    delete timing_;
    delete tech_;
}

void Root::setTechLib(Tech *v) {
    ediAssert(v != nullptr);
    if (tech_ != nullptr) {
        delete tech_;
    }
    tech_ = v;
}

Tech* Root::getTechLib() const {
    return tech_;
}

void Root::setTimingLib(Timing *v) {
    ediAssert(v != nullptr);
    if (timing_ != nullptr) {
        delete timing_;
    }
    timing_ = v;
}

Timing* Root::getTimingLib() const {
    return timing_;
}

void Root::setTopCell(Cell *v) {
    ediAssert(v != nullptr);
    if (top_cell_ != nullptr) {
        delete top_cell_;
    }
    top_cell_ = v;  
}

Cell* Root::getTopCell() const {
    return top_cell_;
}

// Class StorageUtil (runtime object):
StorageUtil::StorageUtil() {
    pool_ = MemPool::newPagePool();
    symtbl_ = new SymbolTable;
    polytbl_ = new PolygonTable();
}

StorageUtil::StorageUtil(uint64_t cell_id) {
    pool_ = MemPool::newPagePool(cell_id);
    symtbl_ = new SymbolTable;
    polytbl_ = new PolygonTable();
}

StorageUtil::~StorageUtil() {
    // TODO (ly): pool_ ?
    delete symtbl_;
    delete polytbl_;
}

// TODO: delete it before set.
void StorageUtil::setSymbolTable(SymbolTable *stb) {
    symtbl_ = stb;
}

SymbolTable *StorageUtil::getSymbolTable() const {
    return symtbl_;
}

void StorageUtil::setPolygonTable(PolygonTable *pt) {
    polytbl_ = pt;
}

PolygonTable *StorageUtil::getPolygonTable() const {
    return polytbl_;
}

void StorageUtil::setPool(MemPagePool *p) {
    pool_ = p;
}

MemPagePool *StorageUtil::getPool() const {
    return pool_;
}

}  // namespace db
}  // namespace open_edi
