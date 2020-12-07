/**
 * @file  cell.cpp
 * @date  09/18/2020 01:38:04 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "db/core/cell.h"

#include <vector>
#include "db/core/db.h"
#include "db/util/array.h"

namespace open_edi {
namespace db {
void SitePattern::setStepPattern(StepPattern *v) {
    sp_.setxCount(v->getxCount());
    sp_.setyCount(v->getyCount());
    sp_.setxStep(v->getxStep());
    sp_.setyStep(v->getyStep());
}

std::string Foreign::getName() const {
    Cell *top_cell = getOwnerCell();
    return top_cell->getSymbolTable()->getSymbolByIndex(name_index_);
}

void Foreign::setName(const char *v) {
    Cell *top_cell = getOwnerCell();
    if (!top_cell) {
        message->issueMsg(kError, "Cannot find top cell \n");
        return;
    }
    name_index_ = top_cell->getOrCreateSymbol(v);
    // top_cell->addSymbolReference(name_index_, this->getId());//? need opt
}

std::string Density::getName() const {
    Cell *top_cell = getOwnerCell();
    return top_cell->getSymbolTable()->getSymbolByIndex(name_index_);
}

void Density::setName(const char *v) {
    Cell *top_cell = getOwnerCell();
    if (!top_cell) {
        message->issueMsg(kError, "Cannot find top cell \n");
        return;
    }
    name_index_ = top_cell->getOrCreateSymbol(v);
    // top_cell->addSymbolReference(name_index_, this->getId());//? need opt
}

void Density::addDensityLayer(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    Cell *top_cell = getOwnerCell();

    if (density_layers_ == 0) {
        vct = top_cell->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        density_layers_ = vct->getId();
        vct->setPool(top_cell->getPool());
        vct->reserve(32);
    } else {
        vct = addr<ArrayObject<ObjectId>>(density_layers_);
    }

    if (vct) vct->pushBack(id);
}

DensityLayer *Density::getDensityLayer(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (density_layers_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(density_layers_);
    }
    if (vct) {
        DensityLayer *obj_data = addr<DensityLayer>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

int Density::getDensityLayerNum() const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (density_layers_ == 0) {
        return 0;
    } else {
        vct = addr<ArrayObject<ObjectId>>(density_layers_);
    }
    if (vct) {
        return vct->getSize();
    }
    return 0;
}

// HierData
void HierData::__init() {
    storage_util_ = nullptr;
    // tech_lib_ = 0;
    fills_ = 0;
    scan_chains_ = 0;
    cells_ = 0;
    buses_ = 0;
    instances_ = 0;
    nets_ = 0;
    special_nets_ = 0;
    io_pins_ = 0;

    floor_plan_ = 0;
    groups_ = 0;
    fills_ = 0;
    scan_chains_ = 0;
    regions_ = 0;
}

StorageUtil* HierData::getStorageUtil() const { return storage_util_; }

void HierData::setStorageUtil(StorageUtil *v) { storage_util_ = v; }

/// @brief setPool set memory pool to a cell
void HierData::setPool(MemPagePool *p) { storage_util_->setPool(p); }

/// @brief getPool get memory pool of a cell
MemPagePool *HierData::getPool() { return storage_util_->getPool(); }

/// @brief getSymbolTable
///
/// @return
SymbolTable *HierData::getSymbolTable() { return storage_util_->getSymbolTable(); }

/// @brief setSymbolTable
///
/// @param stb
void HierData::setSymbolTable(SymbolTable *stb) { storage_util_->setSymbolTable(stb); }

PolygonTable *HierData::getPolygonTable() { return storage_util_->getPolygonTable(); }

void HierData::setPolygonTable(PolygonTable *pt) { storage_util_->setPolygonTable(pt); }

// TODO (ly): remove: 
// void HierData::setTechLibId(ObjectId v) { tech_lib_ = v; }
// ObjectId HierData::getTechLibId() const { return tech_lib_; }

void HierData::setFloorplanId(ObjectId v) { floor_plan_ = v; }

ObjectId HierData::getFloorplanId() const { return floor_plan_; }

// Get object vector:
ObjectId HierData::getCells() const { return cells_; }

ObjectId HierData::getInstances() const { return instances_; }

ObjectId HierData::getNets() const { return nets_; }

ObjectId HierData::getBuses() const { return buses_; }

ObjectId HierData::getSpecialNets() const { return special_nets_; }

ObjectId HierData::getIOPins() const { return io_pins_; }

ObjectId HierData::getGroups() const { return groups_; }

ObjectId HierData::getFills() const { return fills_; }

ObjectId HierData::getScanChains() const { return scan_chains_; }

ObjectId HierData::getRegions() const { return regions_; }

// Set object vector:
void HierData::setCells(ObjectId v) { cells_ = v; }

void HierData::setInstances(ObjectId v) { instances_ = v; }

void HierData::setNets(ObjectId v) { nets_ = v; }

void HierData::setBuses(ObjectId v) { buses_ = v; }

void HierData::setSpecialNets(ObjectId v) { special_nets_ = v; }

void HierData::setIOPins(ObjectId v) { io_pins_ = v; }

void HierData::setGroups(ObjectId v) { groups_ = v; }

void HierData::setFills(ObjectId v) { fills_ = v; }

void HierData::setScanChains(ObjectId v) { scan_chains_ = v; }

void HierData::setRegions(ObjectId v) { regions_ = v; }
// End of HierData

void Cell::__init() {
    name_index_ = -1;
    cell_type_ = CellType::kUnknown;
    hier_data_id_ = 0;
    originX_ = 0;
    originY_ = 0;
    sizeX_ = 0;
    sizeY_ = 0;
    terms_ = 0;
    eeq_index_ = -1;
    site_ = 0;
    site_patterns_ = 0;
    foreigns_ = 0;
    densities_ = 0;
    obses_ = 0;
    memset(mask_shift_layers_, '0', sizeof(ObjectId) * max_layer_num);
    num_mask_shift_layers_ = 0;
    // flag:
    has_origin_ = 0;
    has_eeq_ = 0;
    has_size_ = 0;
    has_x_symmetry_ = 0;
    has_y_symmetry_ = 0;
    has_90_symmetry_ = 0;
    has_site_name_ = 0;
    is_fixed_mask_ = 0;
    sub_class_type_ = kNoSubClass;
    class_type_ = kNoClass;
}

/// @brief Cell default constructor
Cell::Cell() : Cell::BaseType() {
    __init();
}

/// @brief Cell constructor with owner and id, once id is given, assume pool is
/// already there.
Cell::Cell(Object *owner, ObjectId id) : Cell::BaseType(owner, id) { __init(); }

/// @brief ~Cell default deconstructor
Cell::~Cell() {
    HierData *hier_data = __getHierData();
    if (hier_data != nullptr) {
        delete hier_data->getStorageUtil();
    }
}

/// @brief true if this is a hierarchical cell.
bool Cell::isHierCell() const {
    if (cell_type_ == CellType::kHierCell) {
        return true;
    }
    return false;
}

const HierData *Cell::__getConstHierData() const {
    if (hier_data_id_ == 0 || !isHierCell()) {
        return nullptr;
    }
    return addr<HierData>(hier_data_id_);
}

HierData *Cell::__getHierData() {
    if (hier_data_id_ == 0 || !isHierCell()) {
        return nullptr;
    }
    return addr<HierData>(hier_data_id_);
}

/// @brief setCellType set  a cell
void Cell::setCellType(CellType const &v) {
    cell_type_ = v;
}

/// @brief Initialize HierData in the cell with the specified storageUtil
//          and set cell type to kHierCell.
void Cell::initHierData(StorageUtil *v) {
    if (hier_data_id_ != 0 || v == nullptr) {
        return;
    }
    MemPagePool *pool = v->getPool();
    // Temporarily set the pool to parent Cell's pool.
    if (pool == nullptr) {
        pool = MemPool::getPagePoolByObjectId(this->getId());
        // Cell *parent = getOwnerCell();
        // pool = parent->getPool();
    }
    ediAssert(pool != nullptr);
    HierData *hier_data = pool->allocate<HierData>(
                  kObjecTypeHierData, hier_data_id_);
    hier_data->setStorageUtil(v);
    setCellType(CellType::kHierCell);
}

/// @brief get storage_util of a cell
StorageUtil *Cell::getStorageUtil() {
    if (getOwnerId() == this->getId()) {// this is a top cell:
        ediAssert(__getHierData() != nullptr);
        return __getHierData()->getStorageUtil();
    }
    // when a cell is a leaf cell, it doesn't have HierData
    // fetch the data from its owner cell
    if (__getConstHierData() == nullptr) {
        StorageUtil *storage_util = getStorageUtilById(getOwnerId());
        return storage_util;
    }
    return __getHierData()->getStorageUtil();
}

/// @brief set storage_util to a cell
void Cell::setStorageUtil(StorageUtil *v) {
    HierData * hier_data = __getHierData();
    if (hier_data) {
        hier_data->setStorageUtil(v);
    }
}

/// @brief setPool set memory pool to a cell
void Cell::setPool(MemPagePool *pool) {
    HierData * hier_data = __getHierData();
    if (hier_data) {
        hier_data->setPool(pool);
    }
}

/// @brief getPool get memory pool of a cell
MemPagePool *Cell::getPool() {
    if (getOwnerId() == this->getId()) {// this is a top cell:
        ediAssert(__getHierData() != nullptr);
        return __getHierData()->getPool();
    }
    // when a cell is a leaf cell, it doesn't have HierData
    // fetch the data from its owner cell
    if (__getConstHierData() == nullptr) {
        StorageUtil *storage_util = getStorageUtilById(getOwnerId());
        if (storage_util) {
            return storage_util->getPool();
        } else {
            return nullptr;
        }
    }

    return __getHierData()->getPool();
}

/// @brief setPolygonTable set polygon table to a cell
void Cell::setPolygonTable(PolygonTable *pt) {
    HierData * hier_data = __getHierData();
    if (hier_data) {
        hier_data->setPolygonTable(pt);
    }
}

/// @brief getPolygonTable get polygon table of a cell
PolygonTable *Cell::getPolygonTable() {
    if (getOwnerId() == this->getId()) {// this is a top cell:
        ediAssert(__getHierData() != nullptr);
        return __getHierData()->getPolygonTable();
    }
    // when a cell is a leaf cell, it doesn't have HierData
    // fetch the data from its owner cell
    if (__getConstHierData() == nullptr) {
        StorageUtil *storage_util = getStorageUtilById(getOwnerId());
        if (storage_util) {
            return storage_util->getPolygonTable();
        } else {
            return nullptr;
        }
    }

    return __getHierData()->getPolygonTable();
}

/// @brief getParentOrTopSymbolTable
SymbolTable *Cell::getParentOrTopSymbolTable() {
    if (getOwnerId() == this->getId()) {// this is a top cell:
        ediAssert(__getHierData() != nullptr);
        return __getHierData()->getSymbolTable();
    }
    StorageUtil *storage_util = getStorageUtilById(getOwnerId());
    if (storage_util) {
        return storage_util->getSymbolTable();
    }
    return nullptr;
}

/// @brief setSymbolTable
void Cell::setSymbolTable(SymbolTable *stb) {
    HierData * hier_data = __getHierData();
    if (hier_data) {
        hier_data->setSymbolTable(stb);
    }
}

/// @brief getSymbolTable
SymbolTable *Cell::getSymbolTable() {
    if (getOwnerId() == this->getId()) {// this is a top cell:
        ediAssert(__getHierData() != nullptr);
        return __getHierData()->getSymbolTable();
    }
    // when a cell is a leaf cell, it doesn't have HierData
    // fetch the data from its owner cell
    if (__getConstHierData() == nullptr) {
        StorageUtil *storage_util = getStorageUtilById(getOwnerId());
        if (storage_util) {
            return storage_util->getSymbolTable();
        } else {
            return nullptr;
        }
    }
    return __getHierData()->getSymbolTable();
}

/// @brief getOrCreateSymbol
SymbolIndex Cell::getOrCreateSymbol(const char *name) {
    SymbolTable *sym_table = getSymbolTable();
    if (sym_table == nullptr) {
        return kInvalidSymbolIndex;
    }
    return sym_table->getOrCreateSymbol(name);
}

/// @brief getOrCreateSymbol
///
/// @param name
///
/// @return
SymbolIndex Cell::getOrCreateSymbol(std::string &name) {
    return getOrCreateSymbol(name.c_str());
}

/// @brief addSymbolReference
///
/// @param index
/// @param owner
///
/// @return
bool Cell::addSymbolReference(SymbolIndex index, ObjectId owner) {
    return getSymbolTable()->addReference(index, owner);
}

/// @brief getSymbolByIndex
///
/// @param index
///
/// @return
std::string &Cell::getSymbolByIndex(SymbolIndex index) {
    return getSymbolTable()->getSymbolByIndex(index);
}

/// @brief getNameIndex
///
/// @return
SymbolIndex Cell::getNameIndex() { return name_index_; }

/// @brief getName
///
/// @return
std::string const &Cell::getName() {
    return getParentOrTopSymbolTable()->getSymbolByIndex(name_index_);
}

/// @brief setName
///
/// @param v
void Cell::setName(std::string &v) {
    // TODO (luoying): whether to use parent cell?
    // We need to use the parent Cell's symboltable for Cell's name.
    SymbolTable *sym_table = getParentOrTopSymbolTable();
    // SymbolTable *sym_table = getSymbolTable();
    ediAssert(sym_table != nullptr);
    name_index_ = sym_table->getOrCreateSymbol(v.c_str());
    if (name_index_ != kInvalidSymbolIndex) {
        sym_table->addReference(name_index_, this->getId());
    }
}

/// @brief getTechLib
///
/// @return
Tech *Cell::getTechLib() {
    return getRoot()->getTechLib();
}

Layer *Cell::getLayerByLayerId(Int32 id) {
    Tech *tech_lib = getTechLib();
    if (!tech_lib) {
        message->issueMsg(kError, "Cannot find Tech LEF .\n");
        return nullptr;
    }

    return tech_lib->getLayer(id);
}

/// @brief setFloorplan
///
/// @param fp
void Cell::setFloorplan(Floorplan *fp) {
    if (isHierCell()) {
        __getHierData()->setFloorplanId(fp->getId());
    }
}

/// @brief getFloorplan
///
/// @return
Floorplan *Cell::getFloorplan() {
    // when a cell is a leaf cell, it doesn't have HierData
    // fetch the data from its owner cell
    if (__getConstHierData() == nullptr) {
        Cell *owner_cell = addr<Cell>(getOwnerId());
        if (owner_cell) {
            return owner_cell->getFloorplan();
        } else {
            return nullptr;
        }
    }
    ObjectId id = __getHierData()->getFloorplanId();
    return addr<Floorplan>(id);
}

Floorplan *Cell::createFloorplan() {
    Floorplan *floorplan = createObject<Floorplan>(kObjectTypeFloorplan);
    if (!floorplan) {
        message->issueMsg(kError, "create floorplan failed.\n");
        return nullptr;
    }
    setFloorplan(floorplan);
    return floorplan;
}

/// @brief added a cell into array cells_
/// @return none
void Cell::addCell(ObjectId id) {
    if (__getConstHierData() == nullptr) {
        return;
    }
    ArrayObject<ObjectId> *vct = nullptr;
    ObjectId cell_array = __getHierData()->getCells();
    if (cell_array == 0) {
        vct = createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(256);
        __getHierData()->setCells(vct->getId());
    } else {
        vct = addr<ArrayObject<ObjectId>>(cell_array);
    }

    if (vct) vct->pushBack(id);
}

/// @brief createCell create a sub-cell in a cell
/// @return the cell created
Cell *Cell::createCell(std::string &name, bool isHier) {
    if (getCell(name) != nullptr) {
        message->issueMsg(kError,
            "create cell %s failed due to name conflicts.\n", name.c_str());
        return nullptr;
    }
    Cell *cell = createObject<Cell>(kObjectTypeCell);
    if (!cell) {
        message->issueMsg(kError, "create cell %s failed.\n", name.c_str());
        return nullptr;
    }

    if (isHier) {
        StorageUtil *storage_util = new StorageUtil(cell->getId());
        cell->initHierData(storage_util);
        cell->setCellType(CellType::kHierCell);
    } else {
        // TODO(ly): consolidate enum CellType with macro-class
        cell->setCellType(CellType::kCell);
    }
    cell->setName(name);

    addCell(cell->getId());
    return cell;
}

void Cell::deleteCell(Cell *cell) {
// TODO(ly): No implementation yet.
#if 0
    for ( Term *term : terms_) {
        deleteObject<Term>(term);
    }
    for ( Inst *instance : instances_) {
        ArrayObject<Pin*> pins = instance->getPins();
        for (Pin *pin : pins) {
            deleteObject<Pin>(pin);
        }
        deleteObject<Inst>(instance);
    }
    for ( Net *net : nets_) {
        deleteObject<Net>(net);
    }

    deleteObject<Cell>(cell);
#endif
}

/// @brief added a term into array terms_
/// @return none
void Cell::addTerm(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;

    if (terms_ == 0) {
        vct = createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(32);
        terms_ = vct->getId();
    } else {
        vct = addr<ArrayObject<ObjectId>>(terms_);
    }

    if (vct) vct->pushBack(id);
}

/// @brief createTerm create a term in a cell
/// @return the term created
Term *Cell::createTerm(std::string &name) {
    if (getTerm(name) != nullptr) {
        message->issueMsg(kError,
            "create term %s failed due to name conflicts.\n", name.c_str());
        return nullptr;
    }
    Term *term = createObject<Term>(kObjectTypeTerm);
    if (!term) {
        message->issueMsg(kError, "create term %s failed.\n", name.c_str());
        return nullptr;
    }
    term->setName(name);
    term->setCellId(this->getId());
    addTerm(term->getId());
    return term;
}

/// @brief added a bus into array buses_
/// @return none
void Cell::addBus(ObjectId id) {
    if (__getConstHierData() == nullptr) {
        return;
    }
    ArrayObject<ObjectId> *vct = nullptr;
    if (getBuses() == 0) {
        vct = createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(32);
        __getHierData()->setBuses(vct->getId());
    } else {
        vct = addr<ArrayObject<ObjectId>>(getBuses());
    }

    if (vct) vct->pushBack(id);
}

/// @brief createBus create a bus in a cell
/// @return the bus created
Bus *Cell::createBus(std::string &name) {
    if (getBus(name) != nullptr) {
        message->issueMsg(kError,
            "create bus %s failed due to name conflicts.\n", name.c_str());
        return nullptr;
    }
    Bus *bus = createObject<Bus>(kObjectTypeBus);
    if (!bus) {
        message->issueMsg(kError, "create bus %s failed.\n", name.c_str());
        return nullptr;
    }
    bus->setName(name);
    addBus(bus->getId());
    return bus;
}

/// @brief added a net into array nets_
/// @return none
void Cell::addNet(ObjectId id) {
    if (__getConstHierData() == nullptr) {
        return;
    }
    ArrayObject<ObjectId> *vct = nullptr;
    if (getNets() == 0) {
        vct = createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(256);
        __getHierData()->setNets(vct->getId());
    } else {
        vct = addr<ArrayObject<ObjectId>>(getNets());
    }

    if (vct) vct->pushBack(id);
}

Net *Cell::createNet(std::string &name) {
    if (getNet(name) != nullptr) {
        message->issueMsg(kError,
            "create net %s failed due to name conflicts.\n", name.c_str());
        return nullptr;
    }
    Net *net = createObject<Net>(kObjectTypeNet);
    if (!net) {
        message->issueMsg(kError, "create net %s failed.\n", name.c_str());
        return nullptr;
    }
    net->setCell(getId());
    net->setName(name);
    addNet(net->getId());
    return net;
}

/// @brief added a special_net into array special_nets_
/// @return none
void Cell::addSpecialNet(ObjectId id) {
    if (__getConstHierData() == nullptr) return;

    ArrayObject<ObjectId> *vct = nullptr;

    if (getSpecialNets() == 0) {
        vct = createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(256);
        __getHierData()->setSpecialNets(vct->getId());
    } else {
        vct = addr<ArrayObject<ObjectId>>(getSpecialNets());
    }

    if (vct) vct->pushBack(id);
}

SpecialNet *Cell::createSpecialNet(std::string &name) {
    SpecialNet *net = createObject<SpecialNet>(kObjectTypeSpecialNet);
    if (!net) {
        message->issueMsg(kError, "create cell %s failed.\n", name.c_str());
        return nullptr;
    }
    net->setName(name);
    addSpecialNet(net->getId());
    return net;
}

/// @brief added a instance into array instances_
/// @return none
void Cell::addInstance(ObjectId id) {
    if (__getConstHierData() == nullptr) return;

    ArrayObject<ObjectId> *vct = nullptr;

    if (getInstances() == 0) {
        vct = createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(256);
        __getHierData()->setInstances(vct->getId());
    } else {
        vct = addr<ArrayObject<ObjectId>>(getInstances());
    }

    if (vct) vct->pushBack(id);
}

Inst *Cell::createInstance(std::string &name) {
    if (getInstance(name) != nullptr) {
        message->issueMsg(kError,
            "create instance %s failed due to name conflicts.\n", name.c_str());
        return nullptr;
    }
    Inst *inst = createObject<Inst>(kObjectTypeInst);
    if (!inst) {
        message->issueMsg(kError, "create instance %s failed.\n", name.c_str());
        return nullptr;
    }
    inst->setName(name);
    addInstance(inst->getId());
    return inst;
}

/// @brief added a io_pin into array io_pins_
/// @return none
void Cell::addIOPin(ObjectId id) {
    if (__getConstHierData() == nullptr) return;

    ArrayObject<ObjectId> *vct = nullptr;

    if (getIOPins() == 0) {
        vct = createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(64);
        __getHierData()->setIOPins(vct->getId());
    } else {
        vct = addr<ArrayObject<ObjectId>>(getIOPins());
    }

    if (vct) vct->pushBack(id);
}

Pin *Cell::createIOPin(std::string &name) {
    // TODO(ly): naming conflicts check.
    Pin *pin = createObject<Pin>(kObjectTypePin);
    pin->setIsPrimary(true);
    pin->setName(name);
    addIOPin(pin->getId());
    return pin;
}

uint64_t Cell::getNumOfCells() const {
    if (__getConstHierData() == nullptr) {
        return 0;
    }
    ObjectId id = __getConstHierData()->getCells();
    if (id == 0) {
        return 0;
    }
    ArrayObject<ObjectId> *obj_vector = addr<ArrayObject<ObjectId>>(id);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfTerms() const {
    ObjectId id = getTerms();
    if (id == 0) {
        return 0;
    }
    ArrayObject<ObjectId> *obj_vector = addr<ArrayObject<ObjectId>>(id);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfSitePatterns() const {
    ObjectId id = site_patterns_;
    if (id == 0) {
        return 0;
    }
    ArrayObject<ObjectId> *obj_vector = addr<ArrayObject<ObjectId>>(id);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfForeigns() const {
    ObjectId id = getForeigns();
    if (id == 0) {
        return 0;
    }
    ArrayObject<ObjectId> *obj_vector = addr<ArrayObject<ObjectId>>(id);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfBuses() const {
    if (__getConstHierData() == nullptr) {
        return 0;
    }
    ObjectId id = __getConstHierData()->getBuses();
    if (id == 0) {
        return 0;
    }
    ArrayObject<ObjectId> *obj_vector = addr<ArrayObject<ObjectId>>(id);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfScanChains() const {
    if (__getConstHierData() == nullptr) {
        return 0;
    }
    ObjectId id = __getConstHierData()->getScanChains();
    if (id == 0) {
        return 0;
    }
    ArrayObject<ObjectId> *obj_vector = addr<ArrayObject<ObjectId>>(id);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfFills() const {
    if (__getConstHierData() == nullptr) {
        return 0;
    }
    ObjectId id = __getConstHierData()->getFills();
    if (id == 0) {
        return 0;
    }
    ArrayObject<ObjectId> *obj_vector = addr<ArrayObject<ObjectId>>(id);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfGroups() const {
    if (__getConstHierData() == nullptr) {
        return 0;
    }
    ObjectId id = __getConstHierData()->getGroups();
    if (id == 0) {
        return 0;
    }
    ArrayObject<ObjectId> *obj_vector = addr<ArrayObject<ObjectId>>(id);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfIOPins() const {
    if (__getConstHierData() == nullptr) {
        return 0;
    }
    ObjectId id = __getConstHierData()->getIOPins();
    if (id == 0) {
        return 0;
    }
    ArrayObject<ObjectId> *obj_vector = addr<ArrayObject<ObjectId>>(id);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfInsts() const {
    if (__getConstHierData() == nullptr) {
        return 0;
    }
    ObjectId id = __getConstHierData()->getInstances();
    if (id == 0) {
        return 0;
    }
    ArrayObject<ObjectId> *obj_vector = addr<ArrayObject<ObjectId>>(id);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfNets() const {
    if (__getConstHierData() == nullptr) {
        return 0;
    }
    ObjectId id = __getConstHierData()->getNets();
    if (id == 0) {
        return 0;
    }
    ArrayObject<ObjectId> *obj_vector = addr<ArrayObject<ObjectId>>(id);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfSpecialNets() const {
    if (__getConstHierData() == nullptr) {
        return 0;
    }
    ObjectId id = __getConstHierData()->getSpecialNets();
    if (id == 0) {
        return 0;
    }
    ArrayObject<ObjectId> *obj_vector = addr<ArrayObject<ObjectId>>(id);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

ObjectId Cell::getCells() const {
    if (__getConstHierData() == nullptr) {
        return 0;
    }
    return __getConstHierData()->getCells();
}

ObjectId Cell::getInstances() const {
    if (__getConstHierData() == nullptr) {
        return 0;
    }
    return __getConstHierData()->getInstances();
}

ObjectId Cell::getTerms() const { return terms_; }

ObjectId Cell::getBuses() const {
    if (__getConstHierData() == nullptr) {
        return 0;
    }
    return __getConstHierData()->getBuses();
}

ObjectId Cell::getNets() const {
    if (__getConstHierData() == nullptr) {
        return 0;
    }
    return __getConstHierData()->getNets();
}

ObjectId Cell::getSpecialNets() const {
    if (__getConstHierData() == nullptr) {
        return 0;
    }
    return __getConstHierData()->getSpecialNets();
}

ObjectId Cell::getIOPins() const {
    if (__getConstHierData() == nullptr) {
        return 0;
    }
    return __getConstHierData()->getIOPins();
}

ObjectId Cell::getGroups() const {
    if (__getConstHierData() == nullptr) {
        return 0;
    }
    return __getConstHierData()->getGroups();
}

ObjectId Cell::getFills() const {
    if (__getConstHierData() == nullptr) {
        return 0;
    }
    return __getConstHierData()->getFills();
}

ObjectId Cell::getScanChains() const {
    if (__getConstHierData() == nullptr) {
        return 0;
    }
    return __getConstHierData()->getScanChains();
}

Cell *Cell::getCellFromTechLib(std::string name) {
    SymbolIndex symbol_index = getTechLib()->getOrCreateSymbol(name.c_str());
    if (symbol_index == kInvalidSymbolIndex) return nullptr;

    std::vector<ObjectId> object_vector =
          getTechLib()->getSymbolTable()->getReferences(symbol_index);
    for (auto iter = object_vector.begin(); iter != object_vector.end();
        iter++) {
        Cell *target = addr<Cell>(*iter);
        if (target && (target->getObjectType() == kObjectTypeCell))
            return target;
    }
    return nullptr;
}

Cell *Cell::getCell(std::string name) {
    if (getCells() != 0) {
        SymbolIndex symbol_index = this->getOrCreateSymbol(name.c_str());
        if (symbol_index == kInvalidSymbolIndex) return nullptr;

        std::vector<ObjectId> object_vector =
              this->getSymbolTable()->getReferences(symbol_index);
        for (auto iter = object_vector.begin(); iter != object_vector.end();
            iter++) {
            Cell *target = addr<Cell>(*iter);
            if (target && (target->getObjectType() == kObjectTypeCell))
                return target;
        }
    }

    return getCellFromTechLib(name);
}

ArrayObject<ObjectId> *Cell::getCellArray() const {
    ObjectId id = getCells();
    if (id != 0) {
        ArrayObject<ObjectId> *cell_array = addr<ArrayObject<ObjectId>>(id);
        return cell_array;
    } else {
        return nullptr;
    }
}

ArrayObject<ObjectId> *Cell::getInstanceArray() const {
    ObjectId id = getInstances();
    if (id != 0) {
        ArrayObject<ObjectId> *instance_array = addr<ArrayObject<ObjectId>>(id);
        return instance_array;
    } else {
        return nullptr;
    }
}

ArrayObject<ObjectId> *Cell::getTermArray() const {
    if (terms_ != 0) {
        ArrayObject<ObjectId> *term_array = addr<ArrayObject<ObjectId>>(terms_);
        return term_array;
    } else {
        return nullptr;
    }
}

ArrayObject<ObjectId> *Cell::getBusArray() const {
    ObjectId id = getBuses();
    if (id != 0) {
        ArrayObject<ObjectId> *bus_array = addr<ArrayObject<ObjectId>>(id);
        return bus_array;
    } else {
        return nullptr;
    }
}

ArrayObject<ObjectId> *Cell::getNetArray() const {
    ObjectId id = getNets();
    if (id != 0) {
        ArrayObject<ObjectId> *net_array = addr<ArrayObject<ObjectId>>(id);
        return net_array;
    } else {
        return nullptr;
    }
}

ArrayObject<ObjectId> *Cell::getSpecialNetArray() const {
    ObjectId id = getSpecialNets();
    if (id != 0) {
        ArrayObject<ObjectId> *special_net_array =
            addr<ArrayObject<ObjectId>>(id);
        return special_net_array;
    } else {
        return nullptr;
    }
}  // namespace db

ArrayObject<ObjectId> *Cell::getGroupArray() const {
    ObjectId id = getGroups();
    if (id != 0) {
        ArrayObject<ObjectId> *group_array = addr<ArrayObject<ObjectId>>(id);
        return group_array;
    } else {
        return nullptr;
    }
}

Term *Cell::getTerm(std::string name) {
    if (getTerms() != 0) {
        SymbolIndex symbol_index = this->getOrCreateSymbol(name.c_str());
        if (symbol_index == kInvalidSymbolIndex) return nullptr;

        std::vector<ObjectId> &object_vector =
            this->getSymbolTable()->getReferences(symbol_index);
        for (auto iter = object_vector.begin(); iter != object_vector.end();
             iter++) {
            Term *target = addr<Term>(*iter);
            if (target && (target->getObjectType() == kObjectTypeTerm)) {
                if (target->getCellId() == this->getId()) {
                    return target;
                }
            }
        }
    }
    return nullptr;
}

Bus *Cell::getBus(std::string name) {
    if (getBuses() == 0) return nullptr;
    SymbolIndex symbol_index = this->getOrCreateSymbol(name.c_str());
    if (symbol_index == kInvalidSymbolIndex) return nullptr;

    std::vector<ObjectId> &object_vector =
        this->getSymbolTable()->getReferences(symbol_index);
    for (auto iter = object_vector.begin(); iter != object_vector.end();
         iter++) {
        Bus *target = Object::addr<Bus>(*iter);
        if (target && (target->getObjectType() == kObjectTypeBus))
            return target;
    }
    return nullptr;
}

Inst *Cell::getInstance(std::string name) {
    if (getInstances() == 0) return nullptr;
    SymbolIndex symbol_index = this->getOrCreateSymbol(name.c_str());
    if (symbol_index == kInvalidSymbolIndex) return nullptr;

    std::vector<ObjectId> object_vector =
        this->getSymbolTable()->getReferences(symbol_index);
    for (auto iter = object_vector.begin(); iter != object_vector.end();
         iter++) {
        Inst *target = addr<Inst>(*iter);
        if (target && (target->getObjectType() == kObjectTypeInst))
            return target;
    }

    return nullptr;
}

Pin *Cell::getIOPin(const std::string &name) {
    if (getIOPins() == 0) return nullptr;
    SymbolIndex symbol_index = this->getOrCreateSymbol(name.c_str());
    if (symbol_index == kInvalidSymbolIndex) return nullptr;

    std::vector<ObjectId> object_vector =
        this->getSymbolTable()->getReferences(symbol_index);
    for (auto iter = object_vector.begin(); iter != object_vector.end();
         iter++) {
        Pin *target = addr<Pin>(*iter);
        if (target && (target->getObjectType() == kObjectTypePin))
            return target;
    }
    return nullptr;
}

Net *Cell::getNet(std::string name) {
    if (getNets() == 0) return nullptr;

    SymbolIndex symbol_index = this->getOrCreateSymbol(name.c_str());
    if (symbol_index == kInvalidSymbolIndex) return nullptr;

    std::vector<ObjectId> object_vector =
        this->getSymbolTable()->getReferences(symbol_index);
    for (auto iter = object_vector.begin(); iter != object_vector.end();
         iter++) {
        Net *target = addr<Net>(*iter);
        if (target && (target->getObjectType() == kObjectTypeNet))
            return target;
    }
    return nullptr;
}

SpecialNet *Cell::getSpecialNet(std::string name) {
    if (getSpecialNets() == 0) return nullptr;

    SymbolIndex symbol_index = this->getOrCreateSymbol(name.c_str());
    if (symbol_index == kInvalidSymbolIndex) return nullptr;

    std::vector<ObjectId> object_vector =
        this->getSymbolTable()->getReferences(symbol_index);
    for (auto iter = object_vector.begin(); iter != object_vector.end();
         iter++) {
        SpecialNet *target = addr<SpecialNet>(*iter);
        if (target && (target->getObjectType() == kObjectTypeSpecialNet))
            return target;
    }
    return nullptr;
}

Inst *Cell::getInstance(ObjectId obj_id) const { return addr<Inst>(obj_id); }

Pin *Cell::getIOPinById(ObjectId obj_id) {
    if (obj_id == 0) {
        return nullptr;
    }
    return addr<Pin>(obj_id);
}

Pin *Cell::getIOPin(size_t idx) {
    if (getIOPins() == 0) return nullptr;
    ArrayObject<ObjectId> *obj_vector =
        addr<ArrayObject<ObjectId>>(getIOPins());
    if (obj_vector == nullptr) return nullptr;
    ObjectId pin_id = (*obj_vector)[idx];
    return (addr<Pin>(pin_id));
}

Cell *Cell::getCell(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    ObjectId cell_array = getCells();
    if (cell_array == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(cell_array);
    }
    if (vct) {
        Cell *obj_data = addr<Cell>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

Term *Cell::getTerm(size_t idx) const {
    if (getTerms() == 0) return nullptr;
    ArrayObject<ObjectId> *obj_vector =
      addr< ArrayObject<ObjectId> >(getTerms());
    if (obj_vector == nullptr) return nullptr;
    ObjectId object_id = (*obj_vector)[idx];
    return (addr<Term>(object_id));
}

/// @brief added a group into array groups_
/// @return none
void Cell::addGroup(ObjectId id) {
    if (__getConstHierData() == nullptr) return;

    ArrayObject<ObjectId> *vct = nullptr;

    if (getGroups() == 0) {
        vct = createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(32);
        __getHierData()->setGroups(vct->getId());
    } else {
        vct = addr<ArrayObject<ObjectId>>(getGroups());
    }

    if (vct) vct->pushBack(id);
}

Group *Cell::createGroup(std::string &name) {
    Group *group = createObject<Group>(kObjectTypeGroup);
    if (!group) {
        message->issueMsg(kError, "create group %s failed.\n", name.c_str());
        return nullptr;
    }
    group->setName(name);
    addGroup(group->getId());
    return group;
}

Group *Cell::getGroup(size_t idx) const {
    if (getGroups() == 0) return nullptr;
    ArrayObject<ObjectId> *obj_vector =
        addr<ArrayObject<ObjectId>>(getGroups());
    if (obj_vector == nullptr) return nullptr;
    ObjectId object_id = (*obj_vector)[idx];
    return (addr<Group>(object_id));
}

Group *Cell::getGroup(std::string &name) {
    if (getGroups() == 0) return nullptr;
    SymbolIndex symbol_index = this->getOrCreateSymbol(name.c_str());
    if (symbol_index == kInvalidSymbolIndex) return nullptr;

    std::vector<ObjectId> object_vector =
        this->getSymbolTable()->getReferences(symbol_index);
    for (auto iter = object_vector.begin(); iter != object_vector.end();
         iter++) {
        Group *target = addr<Group>(*iter);
        if (target && (target->getObjectType() == kObjectTypeGroup))
            return target;
    }
    return nullptr;
}

/// @brief added a fill into array fills_
/// @return none
void Cell::addFill(ObjectId id) {
    if (__getConstHierData() == nullptr) return;

    ArrayObject<ObjectId> *vct = nullptr;

    if (getFills() == 0) {
        vct = createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(64);
        __getHierData()->setFills(vct->getId());
    } else {
        vct = addr<ArrayObject<ObjectId>>(getFills());
    }

    if (vct) vct->pushBack(id);
}

Fill *Cell::createFill() {
    Fill *fill = createObject<Fill>(kObjectTypeFill);
    addFill(fill->getId());
    return fill;
}

Fill *Cell::getFill(size_t idx) const {
    if (getFills() == 0) return nullptr;
    ArrayObject<ObjectId> *obj_vector = addr<ArrayObject<ObjectId>>(getFills());
    if (obj_vector == nullptr) return nullptr;
    ObjectId fill_id = (*obj_vector)[idx];
    return (addr<Fill>(fill_id));
}

/// @brief added a scan_chain into array scan_chains_
/// @return none
void Cell::addScanChain(ObjectId id) {
    if (__getConstHierData() == nullptr) return;
    ArrayObject<ObjectId> *vct = nullptr;
    if (getScanChains() == 0) {
        vct = createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(64);
        __getHierData()->setScanChains(vct->getId());
    } else {
        vct = addr<ArrayObject<ObjectId>>(getScanChains());
    }

    if (vct) vct->pushBack(id);
}

ScanChain *Cell::createScanChain(std::string &name) {
    ScanChain *scan_chain = createObject<ScanChain>(kObjectTypeScanChain);
    scan_chain->setChainName(name.c_str());
    addScanChain(scan_chain->getId());
    return scan_chain;
}

ScanChain *Cell::getScanChain(size_t idx) const {
    if (getScanChains() == 0) return nullptr;
    ArrayObject<ObjectId> *obj_vector =
        addr<ArrayObject<ObjectId>>(getScanChains());
    if (obj_vector == nullptr) return nullptr;
    ObjectId sc_id = (*obj_vector)[idx];
    return (addr<ScanChain>(sc_id));
}

void Cell::resetTerms(const std::vector<Term *> &terms) {
    if (terms.empty() && terms_ == UNINIT_OBJECT_ID) return;
    ArrayObject<ObjectId> *p = nullptr;
    if (terms_ == UNINIT_OBJECT_ID) {
        p = createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (p) {
            terms_ = p->getId();
            p->setPool(getPool());
            p->reserve(32);
        }
    } else {
        p = addr<ArrayObject<ObjectId>>(terms_);
    }
    if (p != nullptr) {
        IndexType cur_size = terms.size();
        IndexType orig_size = p->getSize();
        if (cur_size < orig_size) {
            p->adjustSize(cur_size);

            IndexType index = 0;
            for (auto iter = p->begin(); iter != p->end(); ++iter) {
                auto &term = *iter;
                term = terms[index]->getId();
                ++index;
            }
        } else {
            IndexType index = 0;
            for (auto iter = p->begin(); iter != p->end(); ++iter) {
                auto &term = *iter;
                term = terms[index]->getId();
                ++index;
            }
            for (IndexType i = orig_size; i < cur_size; ++i) {
                auto &term = terms[i];
                p->pushBack(term->getId());
            }
        }
    }
}

std::string const Cell::getClassString() {
    //return getSymbolByIndex(class_index_);
    std::string tmp = std::string("");
    switch (class_type_) {
        case kCover:
            tmp = "COVER";
            break;
        case kRing:
            tmp = "RING";
            break;
        case kBlock:
            tmp = "BLOCK";
            break;
        case kPad:
            tmp = "PAD";
            break;
        case kCore:
            tmp = "CORE";
            break;
        case kEndcap:
            tmp = "ENDCAP";
            break;
        default:
            break;
    }
    if (sub_class_type_ == kNoSubClass) 
        return tmp;

    switch (sub_class_type_) {
        case kCoverBump:
            tmp += " BUMP";
            break;
        case kBlockBox:
            tmp += " BLACKBOX";
            break;
        case kBlockSoft:
            tmp += " SOFT";
            break;
        case kPadInput:
            tmp += " INPUT";
            break;
        case kPadOutput:
            tmp += " OUTPUT";
            break;
        case kPadInout:
            tmp += " INOUT";
            break;
        case kPadPower:
            tmp += " POWER";
            break;
        case kPadSpacer:
            tmp += " SPACER";
            break;
        case kPadAreaIO:
            tmp += " AREAIO";
            break;
        case kCoreFeedthru:
            tmp += " FEEDTHRU";
            break;
        case kCoreTiehigh:
            tmp += " TIEHIGH";
            break;
        case kCoreTielow:
            tmp += " TIELOW";
            break;
        case kCoreSpacer:
            tmp += " SPACER";
            break;
        case kCoreAntennaCell:
            tmp += " ANTENNACELL";
            break;
        case kCoreWelltap:
            tmp += " WELLTAP";
            break;
        case kEndcapPre:
            tmp += " PRE";
            break;
        case kEndcapPost:
            tmp += " POST";
            break;
        case kEndcapTopLeft:
            tmp += " TOPLEFT";
            break;
        case kEndcapTopRight:
            tmp += " TOPRIGHT";
            break;
        case kEndcapBottomLeft:
            tmp += " BOTTOMLEFT";
            break;
        case kEndcapBottomRight:
            tmp += " BOTTOMRIGHT";
            break; 
        default:
            break;
    }
    return tmp;
}

void Cell::setClass(const char *v) {
    if (v == nullptr)
        return;
    std::string tmp = v;
    std::string class_string;
    std::string sub_class_string;
    bool has_sub_class = false;
    std::string::size_type pos = tmp.find(" ");
    if (pos != std::string::npos) {
        has_sub_class = true;
        class_string = tmp.substr(0,pos);
        sub_class_string = tmp.substr(pos + 1);
    } else {
        class_string = tmp;
    }
    if (class_string == "COVER") {
        class_type_ = kCover;
    } else if (class_string == "RING") {
        class_type_ = kRing;
    } else if (class_string == "BLOCK") {
        class_type_ = kBlock;
    } else if (class_string == "PAD") {
        class_type_ = kPad;
    } else if (class_string == "CORE") {
        class_type_ = kCore;
    } else if (class_string == "ENDCAP") {
        class_type_ = kEndcap;
    } else {
        message->issueMsg(kError, "Class type %s not supported.\n", sub_class_string.c_str());
    }

    if (has_sub_class) {
        if (sub_class_string == "BUMP") {
            sub_class_type_ = CellSubClassType::kCoverBump;
        } else if (sub_class_string == "BLACKBOX") {
            sub_class_type_ = CellSubClassType::kBlockBox;
        } else if (sub_class_string == "SOFT") {
            sub_class_type_ = CellSubClassType::kBlockSoft;
        } else if (sub_class_string == "INPUT") {
            sub_class_type_ = CellSubClassType::kPadInput;
        } else if (sub_class_string == "OUTPUT") {
            sub_class_type_ = CellSubClassType::kPadOutput;
        }  else if (sub_class_string == "INOUT") {
            sub_class_type_ = CellSubClassType::kPadInout;
        }else if (sub_class_string == "POWER") {
            sub_class_type_ = CellSubClassType::kPadPower;
        } else if (sub_class_string == "SPACER") {
            if (class_type_ == kPad) {
                sub_class_type_ = CellSubClassType::kPadSpacer;
            }
            else if (class_type_ == kCore) {
                sub_class_type_ = CellSubClassType::kCoreSpacer;
            }
        } else if (sub_class_string == "AREAIO") {
            sub_class_type_ = CellSubClassType::kPadAreaIO;
        } else if (sub_class_string == "FEEDTHRU") {
            sub_class_type_ = CellSubClassType::kCoreFeedthru;
        } else if (sub_class_string == "TIEHIGH") {
            sub_class_type_ = CellSubClassType::kCoreTiehigh;
        } else if (sub_class_string == "TIELOW") {
            sub_class_type_ = CellSubClassType::kCoreTielow;
        } else if (sub_class_string == "ANTENNACELL") {
            sub_class_type_ = CellSubClassType::kCoreAntennaCell;
        } else if (sub_class_string == "WELLTAP") {
            sub_class_type_ = CellSubClassType::kCoreWelltap;
        } else if (sub_class_string == "PRE") {
            sub_class_type_ = CellSubClassType::kEndcapPre;
        } else if (sub_class_string == "POST") {
            sub_class_type_ = CellSubClassType::kEndcapPost;
        } else if (sub_class_string == "TOPLEFT") {
            sub_class_type_ = CellSubClassType::kEndcapTopLeft;
        } else if (sub_class_string == "TOPRIGHT") {
            sub_class_type_ = CellSubClassType::kEndcapTopRight;
        } else if (sub_class_string == "BOTTOMLEFT") {
            sub_class_type_ = CellSubClassType::kEndcapBottomLeft;
        } else if (sub_class_string == "BOTTOMRIGHT") {
            sub_class_type_ = CellSubClassType::kEndcapBottomRight;
        } else {
            message->issueMsg(kError, "sub class type %s not supported.\n", sub_class_string.c_str());
        }
    }
}

std::string const &Cell::getEEQ() {
    return getSymbolTable()->getSymbolByIndex(eeq_index_);
    // return getSymbolTable()->getSymbolByIndex(eeq_index_);
}

void Cell::setEEQ(const char *v) {
    SymbolIndex idx = getOrCreateSymbol(v);
    if (idx != kInvalidSymbolIndex) {
        eeq_index_ = idx;
    }
}

std::string Cell::getSiteName() {
    Site *target = addr<Site>(site_);
    return target->getName();
}

Site *Cell::getSite() { return addr<Site>(site_); }

ObjectId Cell::getForeigns() const { return foreigns_; }

void Cell::setForeigns(ObjectId v) { foreigns_ = v; }

void Cell::setNumMaskShiftLayers(uint8_t num) { num_mask_shift_layers_ = num; }

uint8_t Cell::getNumMaskShiftLayers() { return num_mask_shift_layers_; }

bool Cell::addMaskShiftLayer(ObjectId layer_id) {
    if (num_mask_shift_layers_ == max_layer_num) {
        return false;
    }

    mask_shift_layers_[num_mask_shift_layers_] = layer_id;
    num_mask_shift_layers_++;
    return true;
}

ObjectId Cell::getMaskShiftLayer(uint8_t index) {
    if (index >= num_mask_shift_layers_) {
        message->issueMsg(kError,
                          "index %d is more than %d, the number of component "
                          "mask shift layers.\n",
                          index, num_mask_shift_layers_);
        return false;
    }
    return mask_shift_layers_[index];
}

std::string SitePattern::getName() const {
    Site *target = addr<Site>(site_);
    return target->getName();
}

void SitePattern::setSiteID(ObjectId v) { site_ = v; }

Site *SitePattern::getSite() { return addr<Site>(site_); }

void Cell::addOBS(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;

    if (obses_ == 0) {
        vct = createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        obses_ = vct->getId();
        vct->setPool(getPool());
        vct->reserve(32);
    } else {
        vct = addr<ArrayObject<ObjectId>>(obses_);
    }

    if (vct) vct->pushBack(id);
}

LayerGeometry *Cell::getOBS(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (obses_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(obses_);
    }
    if (vct) {
        LayerGeometry *obj_data = addr<LayerGeometry>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

void Cell::addForeign(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    ObjectId foreign_array = getForeigns();
    if (foreign_array == 0) {
        vct = createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(32);
        setForeigns(vct->getId());
    } else {
        vct = addr<ArrayObject<ObjectId>>(foreign_array);
    }

    if (vct) vct->pushBack(id);
}

Foreign *Cell::getForeign(int i) const {
    if (foreigns_ == 0) {
        return nullptr;
    }

    ArrayObject<ObjectId> *vct = addr<ArrayObject<ObjectId>>(foreigns_);
    if (vct) {
        Foreign *obj_data = addr<Foreign>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

void Cell::addSitePattern(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;

    if (site_patterns_ == 0) {
        vct = createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        site_patterns_ = vct->getId();
        vct->setPool(getPool());
        vct->reserve(32);
    } else {
        vct = addr<ArrayObject<ObjectId>>(site_patterns_);
    }

    if (vct) vct->pushBack(id);
}

SitePattern *Cell::getSitePattern(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (site_patterns_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(site_patterns_);
    }
    if (vct) {
        SitePattern *obj_data = addr<SitePattern>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

void Cell::addDensity(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;

    if (densities_ == 0) {
        vct = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        if (vct == nullptr) return;
        densities_ = vct->getId();
        vct->setPool(getPool());
        vct->reserve(32);
    } else {
        vct = addr<ArrayObject<ObjectId>>(densities_);
    }

    if (vct) vct->pushBack(id);
}

Density *Cell::getDensity(int index) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (densities_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(densities_);
    }
    if (vct) {
        Density *obj_data = addr<Density>((*vct)[index]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

#if 0
void Cell::print() {
    Tech *lib = getOwnerCell()->getTechLib();
    message->info("MACRO %s \n", getName().c_str());
    message->info("CLASS %s ;\n", getClassString().c_str());
    if (getIsFixedMask()) {
        message->info("   FIXEDMASK ;\n");
    }
    if (numForeigns() > 0) {
        for (int i = 0; i < numForeigns(); i++) {
            Foreign *foreign = getForeign(i);
            message->info("   FOREIGN %s ", foreign->getName().c_str());
            if (foreign->hasForeignPoint()) {
                message->info(" %g %g ",
                              lib->dbuToMicrons(foreign->getforeignX()),
                              lib->dbuToMicrons(foreign->getforeignX()));
                if (foreign->hasForeignOrient()) {
                    message->info("%s ", foreign->foreignOrientStr());
                }
            }
            message->info(";\n");
        }
    }
    if (hasOrigin()) {
        message->info("   ORIGIN %g %g ;\n", lib->dbuToMicrons(getOriginX()),
                      lib->dbuToMicrons(getOriginY()));
    }
    if (hasEEQ()) {
        message->info("   EEQ %s ;\n", getEEQ().c_str());
    }
    if (hasSize()) {
        message->info("   SIZE %g BY %g ;\n", lib->dbuToMicrons(getSizeX()),
                      lib->dbuToMicrons(getSizeY()));
    }
    if (hasXSymmetry() || hasYSymmetry() || has90Symmetry()) {
        message->info("   SYMMETRY");
        if (hasXSymmetry()) message->info(" X");
        if (hasYSymmetry()) message->info(" Y");
        if (has90Symmetry()) message->info(" R90");
        message->info("  ;\n");
    }

    if (hasSiteName()) message->info("   SITE %s ;\n", getSiteName().c_str());
    if (getNumSites() > 0) {
        for (int i = 0; i < getNumSites(); i++) {
            SitePattern *pattern = getSitePattern(i);
            if (pattern) {
                if (pattern->getStepPattern().getxCount() > 0) {
                    message->info(
                        "   SITE %s %g %g %s DO %d BY %d STEP %g %g ;\n",
                        pattern->getName().c_str(),
                        lib->dbuToMicrons(pattern->getxOrigin()),
                        lib->dbuToMicrons(pattern->getyOrigin()),
                        toString(pattern->getOrient()).c_str(),
                        pattern->getStepPattern().getxCount(),
                        pattern->getStepPattern().getyCount(),
                        lib->dbuToMicrons(pattern->getStepPattern().getxStep()),
                        lib->dbuToMicrons(
                            pattern->getStepPattern().getyStep()));
                } else {
                    message->info("   SITE %s %g %g %s ;\n",
                                  pattern->getName().c_str(),
                                  lib->dbuToMicrons(pattern->getxOrigin()),
                                  lib->dbuToMicrons(pattern->getyOrigin()),
                                  toString(pattern->getOrient()).c_str());
                }
            }
        }
    }

    if (getNumOfTerms() > 0) {
        for (int i = 0; i < getNumOfTerms(); i++) getTerm(i)->print();
    }

    ArrayObject<ObjectId> *vct = nullptr;
    if (obses_ != 0) {
        vct = addr<ArrayObject<ObjectId>>(obses_);
        if (vct) {
            for (int i = 0; i < vct->getSize(); i++) {
                LayerGeometry *obj_data = getOBS(i);
                if (obj_data) {
                    message->info("   OBS\n");
                    obj_data->print();
                    message->info("   END\n");
                }
            }
        }
    }

    if (densities_ != 0) {
        vct = addr<ArrayObject<ObjectId>>(densities_);
        if (vct) {
            message->info("   DENSITY\n");
            for (int i = 0; i < vct->getSize(); i++) {
                Density *obj_data = getDensity(i);
                if (obj_data) {
                    message->info("      LAYER %s ;\n",
                                  obj_data->getName().c_str());
                    for (int j = 0; j < obj_data->getDensityLayerNum(); j++) {
                        DensityLayer *layer = obj_data->getDensityLayer(j);
                        message->info(
                            "         RECT %g %g %g %g %g ;\n",
                            lib->dbuToMicrons(layer->getRect().getLLX()),
                            lib->dbuToMicrons(layer->getRect().getLLY()),
                            lib->dbuToMicrons(layer->getRect().getURX()),
                            lib->dbuToMicrons(layer->getRect().getURY()),
                            layer->getDensity());
                    }
                }
            }
            message->info("   END\n");
        }
    }

    message->info("END %s\n", getName().c_str());
}
#endif

void Cell::printLEF(std::ofstream &ofs) {
    Tech *lib = getTechLib();
    ofs << "MACRO"
        << " " << getName().c_str() << " \n";
    if (getClassString().size() > 0) {
        ofs << "   CLASS"
            << " " << getClassString().c_str() << " ;\n";
    }
    if (getIsFixedMask()) {
        ofs << "   FIXEDMASK ;\n";
    }
    if (getNumOfForeigns() > 0) {
        for (int i = 0; i < getNumOfForeigns(); i++) {
            Foreign *foreign = getForeign(i);
            ofs << "   FOREIGN"
                << " " << foreign->getName().c_str() << " ";
            if (foreign->hasForeignPoint()) {
                ofs << lib->dbuToMicrons(foreign->getforeignX()) << " "
                    << lib->dbuToMicrons(foreign->getforeignY());
                if (foreign->hasForeignOrient()) {
                    ofs << " " << toString(foreign->getforeignOrient()).c_str();
                }
            }
            ofs << " ;\n";
        }
    }
    if (hasOrigin()) {
        ofs << "   ORIGIN"
            << " " << lib->dbuToMicrons(getOriginX()) << " "
            << lib->dbuToMicrons(getOriginY()) << " ;\n";
    }
    if (hasEEQ()) {
        ofs << "   EEQ "
            << " " << getEEQ().c_str() << " ;\n";
    }
    if (hasSize()) {
        ofs << "   SIZE"
            << " " << lib->dbuToMicrons(getSizeX()) << " BY "
            << lib->dbuToMicrons(getSizeY()) << " ;\n";
    }
    if (hasXSymmetry() || hasYSymmetry() || has90Symmetry()) {
        ofs << "   SYMMETRY";
        if (hasXSymmetry()) ofs << " X";
        if (hasYSymmetry()) ofs << " Y";
        if (has90Symmetry()) ofs << " R90";
        ofs << " ;\n";
    }

    if (hasSiteName())
        ofs << "   SITE"
            << " " << getSiteName().c_str() << " ;\n";
    if (getNumOfSitePatterns() > 0) {
        for (int i = 0; i < getNumOfSitePatterns(); i++) {
            SitePattern *pattern = getSitePattern(i);
            if (pattern) {
                if (pattern->getStepPattern().getxCount() > 0) {
                    ofs << "   SITE"
                        << " " << pattern->getName().c_str() << " "
                        << lib->dbuToMicrons(pattern->getxOrigin()) << " "
                        << lib->dbuToMicrons(pattern->getyOrigin()) << " "
                        << toString(pattern->getOrient()).c_str() << " DO "
                        << pattern->getStepPattern().getxCount() << " BY "
                        << pattern->getStepPattern().getyCount() << " STEP "
                        << lib->dbuToMicrons(
                               pattern->getStepPattern().getxStep())
                        << " "
                        << lib->dbuToMicrons(
                               pattern->getStepPattern().getyStep())
                        << " ;\n";
                } else {
                    ofs << "   SITE"
                        << " " << pattern->getName().c_str() << " "
                        << lib->dbuToMicrons(pattern->getxOrigin()) << " "
                        << lib->dbuToMicrons(pattern->getyOrigin()) << " "
                        << toString(pattern->getOrient()).c_str() << " ;\n";
                }
            }
        }
    }

    if (getNumOfTerms() > 0) {
        for (int i = 0; i < getNumOfTerms(); i++) getTerm(i)->printLEF(ofs);
    }

    ArrayObject<ObjectId> *vct = nullptr;
    if (obses_ != 0) {
        vct = addr<ArrayObject<ObjectId>>(obses_);
        if (vct) {
            for (int i = 0; i < vct->getSize(); i++) {
                LayerGeometry *obj_data = getOBS(i);
                if (obj_data) {
                    ofs << "   OBS\n";
                    obj_data->printLEF(ofs);
                    ofs << "   END\n";
                }
            }
        }
    }

    if (densities_ != 0) {
        vct = addr<ArrayObject<ObjectId>>(densities_);
        if (vct) {
            ofs << "   DENSITY\n";
            for (int i = 0; i < vct->getSize(); i++) {
                Density *obj_data = getDensity(i);
                if (obj_data) {
                    ofs << "      LAYER"
                        << " " << obj_data->getName().c_str() << " ;\n";
                    for (int j = 0; j < obj_data->getDensityLayerNum(); j++) {
                        DensityLayer *layer = obj_data->getDensityLayer(j);
                        ofs << "         RECT"
                            << " "
                            << lib->dbuToMicrons(layer->getRect().getLLX())
                            << " "
                            << lib->dbuToMicrons(layer->getRect().getLLY())
                            << " "
                            << lib->dbuToMicrons(layer->getRect().getURX())
                            << " "
                            << lib->dbuToMicrons(layer->getRect().getURY())
                            << " " << layer->getDensity() << " ;\n";
                    }
                }
            }
            ofs << ("   END\n");
        }
    }

    ofs << "END"
        << " " << getName().c_str() << "\n\n";
}

}  // namespace db
}  // namespace open_edi
