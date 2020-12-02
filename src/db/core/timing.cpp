/**
 * @file  timing.cpp
 * @date  11/19/2020 01:38:36 PM CST
 * @brief Class Timing
 *
 * Copyright (C) 2020 NiiCEDA Inc.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "db/core/timing.h"

#include "db/timing/timinglib/analysis_corner.h"
#include "db/timing/timinglib/analysis_mode.h"
#include "db/timing/timinglib/analysis_view.h"
#include "db/util/symbol_table.h"
#include "util/polygon_table.h"

namespace open_edi {
namespace db {
Timing::Timing() {}
Timing::~Timing() {}

// Functions for accessing runtime StorageUtil.
MemPagePool *Timing::getPool() const { return storage_util_->getPool(); }
void Timing::setPool(MemPagePool *p) { storage_util_->setPool(p); }

SymbolTable *Timing::getSymbolTable() {
    return storage_util_->getSymbolTable();
}
void Timing::setSymbolTable(SymbolTable *stb) {
    storage_util_->setSymbolTable(stb);
}

PolygonTable *Timing::getPolygonTable() {
    return storage_util_->getPolygonTable();
}
void Timing::setPolygonTable(PolygonTable *pt) {
    storage_util_->setPolygonTable(pt);
}

StorageUtil *Timing::getStorageUtil() const { return storage_util_; }
void Timing::setStorageUtil(StorageUtil *v) { storage_util_ = v; }

/// @brief getSymbolByIndex
/// @param index
/// @return
std::string &Timing::getSymbolByIndex(SymbolIndex index) {
    return getSymbolTable()->getSymbolByIndex(index);
}

/// @brief getOrCreateSymbol
SymbolIndex Timing::getOrCreateSymbol(const char *name) {
    SymbolTable *sym_table = getSymbolTable();
    if (sym_table == nullptr) {
        return kInvalidSymbolIndex;
    }
    return sym_table->getOrCreateSymbol(name);
}

/// @brief getOrCreateSymbol
/// @param name
/// @return
SymbolIndex Timing::getOrCreateSymbol(std::string &name) {
    return getOrCreateSymbol(name.c_str());
}

/// @brief addSymbolReference
/// @param index
/// @param owner
/// @return
bool Timing::addSymbolReference(SymbolIndex index, ObjectId owner) {
    return getSymbolTable()->addReference(index, owner);
}

uint64_t Timing::getNumOfAnalysisViews() const {
    if (analysis_views_ == UNINIT_OBJECT_ID) return 0;
    ArrayObject<ObjectId> *p = addr<ArrayObject<ObjectId>>(analysis_views_);
    if (p == nullptr) return 0;
    return p->getSize();
}

AnalysisMode *Timing::createAnalysisMode(std::string &name) {
    AnalysisMode *analysis_mode =
        createObject<AnalysisMode>(kObjectTypeAnalysisMode, this->getId());
    if (analysis_mode == nullptr) return nullptr;
    analysis_mode->setName(name);
    if (analysis_modes_ == 0) {
        ArrayObject<ObjectId> *am_vector = createObject<ArrayObject<ObjectId>>(
            kObjectTypeArray, this->getId());
        analysis_modes_ = am_vector->getId();
        am_vector->setPool(getPool());
        am_vector->reserve(32);
    }
    ArrayObject<ObjectId> *am_vector =
        addr<ArrayObject<ObjectId>>(analysis_modes_);
    am_vector->pushBack(analysis_mode->getId());
    return analysis_mode;
}

AnalysisCorner *Timing::createAnalysisCorner(std::string &name) {
    AnalysisCorner *analysis_corner =
        createObject<AnalysisCorner>(kObjectTypeAnalysisCorner, this->getId());
    if (analysis_corner == nullptr) return nullptr;
    analysis_corner->setName(name);
    if (analysis_corners_ == 0) {
        ArrayObject<ObjectId> *ac_vector = createObject<ArrayObject<ObjectId>>(
            kObjectTypeArray, this->getId());
        analysis_corners_ = ac_vector->getId();
        ac_vector->setPool(getPool());
        ac_vector->reserve(32);
    }
    ArrayObject<ObjectId> *ac_vector =
        addr<ArrayObject<ObjectId>>(analysis_corners_);
    ac_vector->pushBack(analysis_corner->getId());
    return analysis_corner;
}

AnalysisView *Timing::createAnalysisView(std::string &name) {
    AnalysisView *analysis_view =
        createObject<AnalysisView>(kObjectTypeAnalysisView, this->getId());
    if (analysis_view == nullptr) return nullptr;
    analysis_view->setName(name);
    if (analysis_views_ == 0) {
        ArrayObject<ObjectId> *av_vector = createObject<ArrayObject<ObjectId>>(
            kObjectTypeArray, this->getId());
        analysis_views_ = av_vector->getId();
        av_vector->setPool(getPool());
        av_vector->reserve(32);
    }
    ArrayObject<ObjectId> *av_vector =
        addr<ArrayObject<ObjectId>>(analysis_views_);
    av_vector->pushBack(analysis_view->getId());
    return analysis_view;
}

AnalysisMode *Timing::getAnalysisMode(std::string name) {
    if (analysis_modes_ == 0 || name == "") return nullptr;
    ArrayObject<ObjectId> *object_vector =
        addr<ArrayObject<ObjectId>>(analysis_modes_);
    if (object_vector == nullptr) return nullptr;
    for (auto iter = object_vector->begin(); iter != object_vector->end();
         iter++) {
        AnalysisMode *target = addr<AnalysisMode>(*iter);
        if (target && target->getName() == name) return target;
    }
    return nullptr;
}

AnalysisCorner *Timing::getAnalysisCorner(std::string name) {
    if (analysis_corners_ == 0 || name == "") return nullptr;
    ArrayObject<ObjectId> *object_vector =
        addr<ArrayObject<ObjectId>>(analysis_corners_);
    if (object_vector == nullptr) return nullptr;
    for (auto iter = object_vector->begin(); iter != object_vector->end();
         iter++) {
        AnalysisCorner *target = addr<AnalysisCorner>(*iter);
        if (target && target->getName() == name) return target;
    }
    return nullptr;
}

AnalysisView *Timing::getAnalysisView(std::string name) {
    if (analysis_views_ == 0 || name == "") return nullptr;
    ArrayObject<ObjectId> *object_vector =
        addr<ArrayObject<ObjectId>>(analysis_views_);
    if (object_vector == nullptr) return nullptr;
    for (auto iter = object_vector->begin(); iter != object_vector->end();
         iter++) {
        AnalysisView *target = addr<AnalysisView>(*iter);
        if (target && target->getName() == name) return target;
    }
    return nullptr;
}

AnalysisView *Timing::getAnalysisView(size_t idx) const {
    if (analysis_views_ == 0) return nullptr;
    ArrayObject<ObjectId> *object_vector =
        addr<ArrayObject<ObjectId>>(analysis_views_);
    if (object_vector == nullptr) return nullptr;
    ObjectId id = (*object_vector)[idx];
    return addr<AnalysisView>(id);
}

void Timing::addActiveSetupView(ObjectId id) {
    if (id == 0) return;
    if (active_setup_views_ == 0) {
        ArrayObject<ObjectId> *asv_vector = createObject<ArrayObject<ObjectId>>(
            kObjectTypeArray, this->getId());
        active_setup_views_ = asv_vector->getId();
        asv_vector->setPool(getPool());
        asv_vector->reserve(32);
    }
    ArrayObject<ObjectId> *asv_vector =
        addr<ArrayObject<ObjectId>>(active_setup_views_);
    asv_vector->pushBack(id);
}

void Timing::addActiveHoldView(ObjectId id) {
    if (id == 0) return;
    if (active_hold_views_ == 0) {
        ArrayObject<ObjectId> *ahv_vector = createObject<ArrayObject<ObjectId>>(
            kObjectTypeArray, this->getId());
        active_hold_views_ = ahv_vector->getId();
        ahv_vector->setPool(getPool());
        ahv_vector->reserve(32);
    }
    ArrayObject<ObjectId> *ahv_vector =
        addr<ArrayObject<ObjectId>>(active_hold_views_);
    ahv_vector->pushBack(id);
}

}  // namespace db
}  // namespace open_edi
