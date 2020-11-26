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
Root::Root() : tech_(nullptr), timing_(nullptr), top_cell_(nullptr) {}

Root::~Root() {
    if (top_cell_ != nullptr) {
        Object::deleteObject<Cell>(top_cell_);
    }
    if (timing_ != nullptr) {
        Object::deleteObject<Timing>(timing_);
    }
    if (tech_ != nullptr) {
        Object::deleteObject<Tech>(tech_);
    }
}

void Root::initTopCell() {
    MemPool::initMemPool();
    // Top cell:
    StorageUtil *top_cell_storage = new StorageUtil(0);
    MemPagePool *top_cell_pool = top_cell_storage->getPool();
    if (top_cell_pool == nullptr) return;
    ObjectId cell_id = 0;
    top_cell_ = top_cell_pool->allocate<Cell>(kObjectTypeCell, cell_id);
    top_cell_->setId(cell_id);
    top_cell_->setOwner(cell_id);
    top_cell_->setObjectType(kObjectTypeCell);
    top_cell_->initHierData(top_cell_storage);
    top_cell_->setCellType(CellType::kHierCell);
    MemPool::insertPagePool(cell_id, top_cell_pool);
}

void Root::initTechLib() {
    MemPool::initMemPool();
    // Tech:
    StorageUtil *tech_lib_storage = new StorageUtil(0);
    MemPagePool *tech_lib_pool = tech_lib_storage->getPool();
    if (tech_lib_pool == nullptr) return;
    ObjectId tech_id;
    tech_ = tech_lib_pool->allocate<Tech>(kObjectTypeTech, tech_id);
    tech_->setId(tech_id);
    tech_->setOwner(tech_id);
    tech_->setObjectType(kObjectTypeTech);
    tech_->setStorageUtil(tech_lib_storage);
    MemPool::insertPagePool(tech_id, tech_lib_pool);
}

void Root::initTimingLib() {
    MemPool::initMemPool();
    // Timing:
    StorageUtil *timing_lib_storage = new StorageUtil(0);
    MemPagePool *timing_lib_pool = timing_lib_storage->getPool();
    if (timing_lib_pool == nullptr) return;
    ObjectId timing_id;
    timing_ = timing_lib_pool->allocate<Timing>(kObjectTypeTiming, timing_id);
    timing_->setId(timing_id);
    timing_->setOwner(timing_id);
    timing_->setObjectType(kObjectTypeTiming);
    timing_->setStorageUtil(timing_lib_storage);
    MemPool::insertPagePool(timing_id, timing_lib_pool);
}

void Root::reset() {
    setTimingLib(nullptr);
    setTechLib(nullptr);
    setTopCell(nullptr);
}

void Root::setTechLib(Tech *v) {
    if (tech_ != nullptr) {
        Object::deleteObject<Tech>(tech_);
    }
    tech_ = v;
}

Tech* Root::getTechLib() const {
    return tech_;
}

void Root::setTimingLib(Timing *v) {
    if (timing_ != nullptr) {
        Object::deleteObject<Timing>(timing_);
    }
    timing_ = v;
}

Timing* Root::getTimingLib() const {
    return timing_;
}

void Root::setTopCell(Cell *v) {
    if (top_cell_ != nullptr) {
        Object::deleteObject<Cell>(top_cell_);
    }
    top_cell_ = v;
}

Cell* Root::getTopCell() const {
    return top_cell_;
}

// Class StorageUtil (runtime object):
StorageUtil::StorageUtil() : 
  pool_(nullptr), symtbl_(nullptr), polytbl_(nullptr) {}

StorageUtil::StorageUtil(uint64_t cell_id) {
    initPool(cell_id);
    initSymbolTable();
    initPolygonTable();
}

StorageUtil::~StorageUtil() {
    if (polytbl_ != nullptr) {
        delete polytbl_;
    }
    if (symtbl_ != nullptr) {
        delete symtbl_;
    }
    // TODO (ly): pool_ ?
}

void StorageUtil::initPool(uint64_t cell_id) {
    if (cell_id == 0) {
        pool_ = MemPool::newPagePool();
    } else {
        pool_ = MemPool::newPagePool(cell_id);
    }
}

void StorageUtil::initSymbolTable() {
    symtbl_ = new SymbolTable;
}

void StorageUtil::initPolygonTable() {
    polytbl_ = new PolygonTable();
}

void StorageUtil::setSymbolTable(SymbolTable *stb) {
    if (stb != nullptr && symtbl_ != nullptr) {
        delete symtbl_;
    }
    symtbl_ = stb;
}

SymbolTable *StorageUtil::getSymbolTable() const {
    return symtbl_;
}

void StorageUtil::setPolygonTable(PolygonTable *pt) {
    if (pt != nullptr && polytbl_ != nullptr) {
        delete polytbl_;
    }
    polytbl_ = pt;
}

PolygonTable *StorageUtil::getPolygonTable() const {
    return polytbl_;
}

void StorageUtil::setPool(MemPagePool *p) {
    if (p != nullptr && pool_ != nullptr) {
        // TODO (ly) free a memory pool?
    }
    pool_ = p;
}

MemPagePool *StorageUtil::getPool() const {
    return pool_;
}

}  // namespace db
}  // namespace open_edi
