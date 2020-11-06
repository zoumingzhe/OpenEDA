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
#include "db/timing/timinglib/analysis_corner.h"
#include "db/timing/timinglib/analysis_mode.h"
#include "db/timing/timinglib/analysis_view.h"
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
    Cell *top_cell = getTopCell();
    return top_cell->getSymbolTable()->getSymbolByIndex(name_index_);
}

void Foreign::setName(const char *v) {
    Cell *top_cell = getTopCell();
    if (!top_cell) {
        message->issueMsg(kError, "Cannot find top cell \n");
        return;
    }
    name_index_ = top_cell->getOrCreateSymbol(v);
    // top_cell->addSymbolReference(name_index_, this->getId());//? need opt
}

std::string Density::getName() const {
    Cell *top_cell = getTopCell();
    return top_cell->getSymbolTable()->getSymbolByIndex(name_index_);
}

void Density::setName(const char *v) {
    Cell *top_cell = getTopCell();
    if (!top_cell) {
        message->issueMsg(kError, "Cannot find top cell \n");
        return;
    }
    name_index_ = top_cell->getOrCreateSymbol(v);
    // top_cell->addSymbolReference(name_index_, this->getId());//? need opt
}

void Density::addDensityLayer(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    Cell *top_cell = getTopCell();

    if (density_layers_ == 0) {
        vct = top_cell->createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        if (vct == nullptr) return;
        density_layers_ = vct->getId();
        vct->setPool(top_cell->getPool());
        vct->reserve(32);        
    } else {
        vct = addr< ArrayObject<ObjectId> >(density_layers_);
    }

    if (vct) vct->pushBack(id);
}

DensityLayer *Density::getDensityLayer(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (density_layers_ == 0) {
        return nullptr;
    } else {
        vct = addr< ArrayObject<ObjectId> >(density_layers_);
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
        vct = addr< ArrayObject<ObjectId> >(density_layers_);
    }
    if (vct) {
        return vct->getSize();
    }
    return 0;
}

/// @brief Cell default constructor
Cell::Cell() : Cell::BaseType() {
    name_index_ = -1;
    pool_ = nullptr;
    tech_lib_ = 0;
    fills_ = 0;
    scan_chains_ = 0;
    is_fixed_mask_    = 0;
    has_origin_       = 0;
    has_eeq_          = 0;
    has_size_         = 0;
    has_site_name_    = 0;
    has_x_symmetry_   = 0;
    has_y_symmetry_   = 0;
    has_90_symmetry_  = 0;

    // initialization of vector_cells/terms/instances/nets/pins/io_pins/views.
    cells_ = 0;
    terms_ = 0;
    instances_ = 0;
    nets_ = 0;
    special_nets_ = 0;
    io_pins_ = 0;
    site_patterns_ = 0;
    foreigns_ = 0;
    densities_ = 0;
    obses_ = 0;
    analysis_modes_ = 0;
    analysis_corners_ = 0;
    analysis_views_ = 0;
    active_setup_views_ = 0;
    active_hold_views_ = 0;
    // end of vector objects initialization.

    macro_class_index_ = -1;
    eeq_index_ = -1;

    setObjectType(kObjectTypeTCell);
}

/// @brief Cell constructor with owner and id, once id is given, assume pool is
/// already there.
Cell::Cell(Object *owner, ObjectId id) : Cell::BaseType(owner, id) {
    name_index_ = 0;
    tech_lib_ = 0;
    fills_ = 0;
    scan_chains_ = 0;

    analysis_modes_ = 0;
    analysis_corners_ = 0;
    analysis_views_ = 0;
    active_setup_views_ = 0;
    active_hold_views_ = 0;

    setObjectType(kObjectTypeTCell);
}


/// @brief ~Cell default deconstructor
Cell::~Cell() {
    // removed the original cells_ removal.
}

/// @brief setPool set memory pool to a cell
void Cell::setPool(MemPagePool *p) { pool_ = p; }

/// @brief getPool get memory pool of a cell
MemPagePool *Cell::getPool() { 
  //TODO: when a cell is a leaf cell, it doesn't have own memory pool
  //fetch the pool from its owner cell
  // getCellType() != CellType::kHierCell
  if (pool_ == nullptr) {
      Cell *owner_cell = addr<Cell>(getOwnerId());
      if (owner_cell) {
          return owner_cell->getPool();
      }
  }
  return pool_; 
}

/// @brief setSymbolTable
///
/// @param stb
void Cell::setSymbolTable(SymbolTable *stb) { symtbl_ = stb; }

void Cell::setPolygonTable(PolygonTable *pt) { polytbl_ = pt; }

PolygonTable *Cell::getPolygonTable() {
    Cell *top_cell = getTopCell();
    if (!top_cell) {
        message->issueMsg(kError,
                          "Cannot find top cell when finding polygontable.\n");
        return nullptr;
    }
    if (this == top_cell) {
        return polytbl_;
    } else {
        return top_cell->getPolygonTable();
    }
}

/// @brief getSymbolTable
///
/// @return
SymbolTable *Cell::getSymbolTable() {
    Cell *top_cell = getTopCell();
    if (!top_cell) {
        message->issueMsg(kError,
                          "Cannot find top cell when finding symboltable.\n");
        return nullptr;
    }
    if (this == top_cell) {
        return symtbl_;
    } else {
        return top_cell->getSymbolTable();
    }
}

/// @brief getOrCreateSymbol
///
/// @param name
///
/// @return
int64_t Cell::getOrCreateSymbol(const char *name) {
    return getSymbolTable()->getOrCreateSymbol(name);
}

/// @brief getOrCreateSymbol
///
/// @param name
///
/// @return
SymbolIndex Cell::getOrCreateSymbol(const std::string &name) {
    return getSymbolTable()->getOrCreateSymbol(name.c_str());
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
    return getSymbolTable()->getSymbolByIndex(name_index_);
}

/// @brief setName
///
/// @param v
void Cell::setName(std::string &v) {
    int64_t idx = getOrCreateSymbol(v);
    if (idx != -1) {
        name_index_ = idx;
        addSymbolReference(name_index_, this->getId());
    }
}

/// @brief setTechLib
///
/// @param t
void Cell::setTechLib(Tech *t) { tech_lib_ = t->getId(); }

/// @brief getTechLib
///
/// @return
Tech *Cell::getTechLib() { return addr<Tech>(tech_lib_); }

Layer *Cell::getLayerByLayerId(Int32 id) {
    Cell *top_cell = getTopCell();
    Tech *tech_lib = top_cell->getTechLib();
    if (!tech_lib) {
        message->issueMsg(kError, "Cannot find Tech LEF .\n");
        return nullptr;
    }

    return tech_lib->getLayer(id);
}

/// @brief setFloorplan
///
/// @param fp
void Cell::setFloorplan(Floorplan *fp) { floor_plan_ = fp->getId(); }

/// @brief getFloorplan
///
/// @return
Floorplan *Cell::getFloorplan() {
    return addr<Floorplan>(floor_plan_);
}

Floorplan *Cell::createFloorplan() {
    Floorplan *floorplan = createObject<Floorplan>(kObjectTypeFloorplan);
    if (!floorplan) {
        message->issueMsg(kError, "create floorplan failed.\n");
        return nullptr;
    }
    floorplan->setOwner(this);
    floor_plan_ = floorplan->getId();
    return floorplan;
}

/// @brief added a cell into array cells_
/// @return none
void Cell::addCell(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;

    if (cells_ == 0) {
        vct = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(256);        
        cells_ = vct->getId();
    } else {
        vct = addr< ArrayObject<ObjectId> >(cells_);
    }

    if (vct) vct->pushBack(id);
}

/// @brief createCell create a sub-cell in a cell
/// @return the cell created
Cell *Cell::createCell(std::string &name, bool isHier) {
    Cell *cell = createObject<Cell>(kObjectTypeCell);
    if (!cell) {
        message->issueMsg(kError, "create cell %s failed.\n", name.c_str());
        return nullptr;
    }
    if (isHier) {
        cell->setCellType(CellType::kHierCell);
        MemPagePool *page_pool = MemPool::newPagePool(cell->getId());
        if (!page_pool) {
            message->issueMsg(kError, "cannot crate a new page pool when create hierarchical cell %s\n", name.c_str());
            return nullptr;
        }
        cell->setPool(page_pool);
    }
    else {
        //TODO: check enum CellType
        cell->setCellType(CellType::kCell);
        cell->setPool(MemPool::getPagePool(getTopCell()->getId()));
    }
    cell->setName(name);
    cell->setOwner(this);

    addCell(cell->getId());
    return cell;
}

void Cell::deleteCell(Cell *cell) {
// TODO
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
        vct = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(32);        
        terms_ = vct->getId();
    } else {
        vct = addr< ArrayObject<ObjectId> >(terms_);
    }

    if (vct) vct->pushBack(id);
}

/// @brief createTerm create a term in a cell
/// @return the term created
Term *Cell::createTerm(std::string &name) {
    Term *term = createObject<Term>(kObjectTypeTerm);
    if (!term) {
        message->issueMsg(kError, "create term %s failed.\n", name.c_str());
        return nullptr;
    }
    term->setName(name);
    term->setOwner(this);
    addTerm(term->getId());
    return term;
}


/// @brief added a bus into array buses_
/// @return none
void Cell::addBus(ObjectId id) {
    ArrayObject<ObjectId>  *vct = nullptr;

    if (buses_ == 0) {
        vct = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(32);        
        buses_ = vct->getId();
    } else {
        vct = addr< ArrayObject<ObjectId> >(buses_);
    }

    if (vct) vct->pushBack(id);
}

/// @brief createBus create a bus in a cell
/// @return the bus created
Bus *Cell::createBus(std::string &name) {
    Bus *bus = createObject<Bus>(kObjectTypeBus);
    if (!bus) {
        message->issueMsg(kError, "create bus %s failed.\n", name.c_str());
        return nullptr;
    }
    bus->setName(name);
    bus->setOwner(this);
    addBus(bus->getId());
    return bus;
}

/// @brief added a net into array nets_
/// @return none
void Cell::addNet(ObjectId id) {
    ArrayObject<ObjectId>  *vct = nullptr;

    if (nets_ == 0) {
        Cell *top_cell = getTopCell();
        if (!top_cell) {
            message->issueMsg(
                kError, "Cannot find top cell when create vectorobject256.\n");
            return;  // risk , need opt
        }
        vct = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(256);        
        nets_ = vct->getId();
    } else {
        vct = addr< ArrayObject<ObjectId> >(nets_);
    }

    if (vct) vct->pushBack(id);
}

Net *Cell::createNet(std::string &name) {
    Net *net = createObject<Net>(kObjectTypeNet);
    if (!net) {
        message->issueMsg(kError, "create net %s failed.\n", name.c_str());
        return nullptr;
    }
    net->setCell(getId());
    net->setName(name);
    net->setOwner(this);
    addNet(net->getId());
    return net;
}

/// @brief added a special_net into array special_nets_
/// @return none
void Cell::addSpecialNet(ObjectId id) {
    ArrayObject<ObjectId>  *vct = nullptr;

    if (special_nets_ == 0) {
        vct = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(256);        
        special_nets_ = vct->getId();
    } else {
        vct = addr< ArrayObject<ObjectId> >(special_nets_);
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
    net->setOwner(this);
    addSpecialNet(net->getId());
    return net;
}

/// @brief added a instance into array instances_
/// @return none
void Cell::addInstance(ObjectId id) {
    ArrayObject<ObjectId>  *vct = nullptr;

    if (instances_ == 0) {
        vct = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(256);        
        instances_ = vct->getId();
    } else {
        vct = addr< ArrayObject<ObjectId> >(instances_);
    }

    if (vct) vct->pushBack(id);
}

Inst *Cell::createInstance(std::string &name) {
    Inst *inst = createObject<Inst>(kObjectTypeInst);
    if (!inst) {
        message->issueMsg(kError, "create instance %s failed.\n", name.c_str());
        return nullptr;
    }
    inst->setName(name);
    inst->setOwner(this);
    addInstance(inst->getId());
    return inst;
}

/// @brief added a io_pin into array io_pins_
/// @return none
void Cell::addIOPin(ObjectId id) {
    ArrayObject<ObjectId>  *vct = nullptr;

    if (io_pins_ == 0) {
        vct = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(64);        
        io_pins_ = vct->getId();
    } else {
        vct = addr< ArrayObject<ObjectId> >(io_pins_);
    }

    if (vct) vct->pushBack(id);
}

Pin *Cell::createIOPin(std::string &name) {
    Pin *pin = createObject<Pin>(kObjectTypePin);
    pin->setName(name);
    pin->setOwner(this);
    addIOPin(pin->getId());
    return pin;
}

/// @brief added a virtual_pin into array virtual_pins_
/// @return none
void Cell::addVPin(ObjectId id) {
    ArrayObject<ObjectId>  *vct = nullptr;

    if (virtual_pins_ == 0) {
        vct = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(64);        
        virtual_pins_ = vct->getId();
    } else {
        vct = addr< ArrayObject<ObjectId> >(io_pins_);
    }

    if (vct) vct->pushBack(id);
}

Pin *Cell::createVPin(std::string &name) {
    Pin *pin = createObject<Pin>(kObjectTypePin);
    pin->setName(name);
    pin->setOwner(this);
    addVPin(pin->getId());
    return pin;
}

uint64_t Cell::getNumOfCells() const {
    if (cells_ == 0) return 0;
    ArrayObject<ObjectId>  *obj_vector =
        addr< ArrayObject<ObjectId> >(cells_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfTerms() const {
    ArrayObject<ObjectId>  *vct = nullptr;
    if (terms_ == 0) {
        return 0;
    } else {
        vct = addr< ArrayObject<ObjectId> >(terms_);
    }
    if (vct) {
        return vct->getSize();
    }
    return 0;
}

uint64_t Cell::getNumOfBuses() const {
    if (buses_ == 0) return 0;
    ArrayObject<ObjectId>  *obj_vector =
        Object::addr< ArrayObject<ObjectId> >(buses_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfInsts() const {
    if (instances_ == 0) return 0;
    ArrayObject<ObjectId>  *obj_vector =
        addr< ArrayObject<ObjectId> >(instances_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfNets() const {
    if (nets_ == 0) return 0;
    ArrayObject<ObjectId>  *obj_vector = addr< ArrayObject<ObjectId> >(nets_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfSpecialNets() const {
    if (special_nets_ == 0) return 0;
    ArrayObject<ObjectId>  *obj_vector =
        addr< ArrayObject<ObjectId> >(special_nets_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfIOPins() const {
    if (io_pins_ == 0) return 0;
    ArrayObject<ObjectId>  *obj_vector =
        addr< ArrayObject<ObjectId> >(io_pins_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfGroups() const {
    if (groups_ == 0) return 0;
    ArrayObject<ObjectId> *obj_vector = addr< ArrayObject<ObjectId> >(groups_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfFills() const {
    if (fills_ == 0) return 0;
    ArrayObject<ObjectId> *obj_vector = addr< ArrayObject<ObjectId> >(fills_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfScanChains() const {
    if (scan_chains_ == 0) return 0;
    ArrayObject<ObjectId>  *obj_vector =
        addr< ArrayObject<ObjectId> >(scan_chains_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

uint64_t Cell::getNumOfAnalysisViews() const {
    if (analysis_views_ == UNINIT_OBJECT_ID) return 0;
    ArrayObject<ObjectId>  *p = addr< ArrayObject<ObjectId> >(analysis_views_);
    if (p == nullptr) return 0;
    return p->getSize();
}

ObjectId Cell::getInstances() const { return instances_; }
ObjectId Cell::getCells() const { return cells_; }
ObjectId Cell::getTerms() const { return terms_; }
ObjectId Cell::getBuses() const { return buses_; }
ObjectId Cell::getNets() const { return nets_; }
ObjectId Cell::getSpecialNets() const { return special_nets_; }

Cell *Cell::getCell(std::string name) {
    if (cells_ == 0) return nullptr;
    SymbolIndex symbol_index = this->getOrCreateSymbol(name.c_str());
    if (symbol_index == -1) return nullptr;

    if (symbol_index) {
        std::vector<ObjectId> object_vector =
            this->getSymbolTable()->getReferences(symbol_index);
        for (auto iter = object_vector.begin(); iter != object_vector.end();
             iter++) {
            Cell *target = addr<Cell>(*iter);
            if (target && (target->getObjectType() == kObjectTypeCell))
                return target;
        }
    }
    return nullptr;
}

Term *Cell::getTerm(std::string name) {
    if (terms_ == 0) return nullptr;
    SymbolIndex symbol_index = this->getOrCreateSymbol(name.c_str());
    if (symbol_index == -1) return nullptr;

    if (symbol_index) {
        std::vector<ObjectId> &object_vector =
            this->getSymbolTable()->getReferences(symbol_index);
        for (auto iter = object_vector.begin(); iter != object_vector.end();
             iter++) {
            Term *target = addr<Term>(*iter);
            if (target && (target->getObjectType() == kObjectTypeTerm))
                return target;
        }
    }
    return nullptr;
}

Bus *Cell::getBus(std::string name) {
    if (buses_ == 0) return nullptr;
    SymbolIndex symbol_index = this->getOrCreateSymbol(name.c_str());
    if (symbol_index == -1) return nullptr;

    if (symbol_index) {
        std::vector<ObjectId> &object_vector =
            this->getSymbolTable()->getReferences(symbol_index);
        for (auto iter = object_vector.begin(); iter != object_vector.end();
             iter++) {
            Bus *target = Object::addr<Bus>(*iter);
            if (target && (target->getObjectType() == kObjectTypeBus))
                return target;
        }
    }
    return nullptr;
}

Inst *Cell::getInstance(std::string name) {
    if (instances_ == 0) return nullptr;
    SymbolIndex symbol_index = this->getOrCreateSymbol(name.c_str());
    if (symbol_index == -1) return nullptr;

    if (symbol_index) {
        std::vector<ObjectId> object_vector =
            this->getSymbolTable()->getReferences(symbol_index);
        for (auto iter = object_vector.begin(); iter != object_vector.end();
             iter++) {
            Inst *target = addr<Inst>(*iter);
            if (target && (target->getObjectType() == kObjectTypeInst))
                return target;
        }
    }

    return nullptr;
}

Pin *Cell::getIOPin(const std::string &name) {
    if (io_pins_ == 0) return nullptr;
    SymbolIndex symbol_index = this->getOrCreateSymbol(name.c_str());
    if (symbol_index == -1) return nullptr;

    if (symbol_index) {
        std::vector<ObjectId> object_vector =
            this->getSymbolTable()->getReferences(symbol_index);
        for (auto iter = object_vector.begin(); iter != object_vector.end();
             iter++) {
            Pin *target = addr<Pin>(*iter);
            if (target && (target->getObjectType() == kObjectTypePin))
                return target;
        }
    }
    return nullptr;
}

Pin *Cell::getVPin(const std::string &name) {
    if (virtual_pins_ == 0) return nullptr;
    SymbolIndex symbol_index = this->getOrCreateSymbol(name.c_str());
    if (symbol_index == -1) return nullptr;

    if (symbol_index) {
        std::vector<ObjectId> object_vector =
            getSymbolTable()->getReferences(symbol_index);
        for (auto iter = object_vector.begin(); iter != object_vector.end();
             iter++) {
            Pin *target = addr<Pin>(*iter);
            if (target && (target->getObjectType() == kObjectTypePin))
                return target;
        }
    }
    return nullptr;
}

Net *Cell::getNet(std::string name) {
    if (nets_ == 0) return nullptr;

    SymbolIndex symbol_index = this->getOrCreateSymbol(name.c_str());
    if (symbol_index == -1) return nullptr;

    if (symbol_index) {
        std::vector<ObjectId> object_vector =
            this->getSymbolTable()->getReferences(symbol_index);
        for (auto iter = object_vector.begin(); iter != object_vector.end();
             iter++) {
            Net *target = addr<Net>(*iter);
            if (target && (target->getObjectType() == kObjectTypeNet))
                return target;
        }
    }
    return nullptr;
}

SpecialNet *Cell::getSpecialNet(std::string name) {
    if (nets_ == 0) return nullptr;

    SymbolIndex symbol_index = this->getOrCreateSymbol(name.c_str());
    if (symbol_index == -1) return nullptr;

    if (symbol_index) {
        std::vector<ObjectId> object_vector =
            this->getSymbolTable()->getReferences(symbol_index);
        for (auto iter = object_vector.begin(); iter != object_vector.end();
             iter++) {
            SpecialNet *target = addr<SpecialNet>(*iter);
            if (target && (target->getObjectType() == kObjectTypeSpecialNet))
                return target;
        }
    }
    return nullptr;
}

Inst *Cell::getInstance(ObjectId obj_id) const {
    return addr<Inst>(obj_id);
}

Pin *Cell::getIOPinById(ObjectId obj_id) {
    if (obj_id == 0) {
        return nullptr;
    }
    return addr<Pin>(obj_id);
}

Pin *Cell::getIOPin(size_t idx) {
    if (io_pins_ == 0) return nullptr;
    ArrayObject<ObjectId>  *obj_vector =
        addr< ArrayObject<ObjectId> >(io_pins_);
    if (obj_vector == nullptr) return nullptr;
    ObjectId pin_id = (*obj_vector)[idx];
    return (addr<Pin>(pin_id));
}


Cell *Cell::getCell(int i) const {
    ArrayObject<ObjectId>  *vct = nullptr;
    if (cells_ == 0) {
        return nullptr;
    } else {
        vct = addr< ArrayObject<ObjectId> >(cells_);
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
    if (terms_ == 0) return nullptr;
    ArrayObject<ObjectId> *obj_vector = addr< ArrayObject<ObjectId> >(terms_);
    if (obj_vector == nullptr) return nullptr;
    ObjectId object_id = (*obj_vector)[idx];
    return (addr<Term>(object_id));
}

/// @brief added a group into array groups_
/// @return none
void Cell::addGroup(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;

    if (groups_ == 0) {
        vct = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(32);        
        groups_ = vct->getId();
    } else {
        vct = addr< ArrayObject<ObjectId> >(groups_);
    }

    if (vct) vct->pushBack(id);
}

Group *Cell::createGroup(std::string &name) {
    Group *group = createObject<Group>(kObjectTypeGroup);
    if (!group) {
        message->issueMsg(kError, "create group %s failed.\n", name.c_str());
        return nullptr;
    }
    group->setCell(getId());
    group->setName(name);
    group->setOwner(this);
    if (groups_ == 0) {
        ArrayObject<ObjectId> *object_vector = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        groups_ = object_vector->getId();
        object_vector->setPool(getPool());
        object_vector->reserve(32);      
    }
    ArrayObject<ObjectId> *object_vector =
        addr< ArrayObject<ObjectId> >(groups_);
    object_vector->pushBack(group->getId());
    return group;
}

ObjectId Cell::getGroups() const { return groups_; }

Group *Cell::getGroup(size_t idx) const {
    if (groups_ == 0) return nullptr;
    ArrayObject<ObjectId> *obj_vector = addr< ArrayObject<ObjectId> >(groups_);
    if (obj_vector == nullptr) return nullptr;
    ObjectId object_id = (*obj_vector)[idx];
    return (addr<Group>(object_id));
}

Group *Cell::getGroup(std::string &name) {
    if (groups_ == 0) return nullptr;
    SymbolIndex symbol_index = this->getOrCreateSymbol(name.c_str());
    if (symbol_index == -1) return nullptr;

    if (symbol_index) {
        std::vector<ObjectId> object_vector =
            this->getSymbolTable()->getReferences(symbol_index);
        for (auto iter = object_vector.begin(); iter != object_vector.end();
             iter++) {
            Group *target = addr<Group>(*iter);
            if (target && (target->getObjectType() == kObjectTypeGroup))
                return target;
        }
    }
    return nullptr;
}

/// @brief added a fill into array fills_
/// @return none
void Cell::addFill(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;

    if (fills_ == 0) {
        vct = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(64);        
        fills_ = vct->getId();
    } else {
        vct = addr< ArrayObject<ObjectId> >(fills_);
    }

    if (vct) vct->pushBack(id);
}

Fill *Cell::createFill() {
    Fill *fill = createObject<Fill>(kObjectTypeFill);
    fill->setOwner(this);
    addFill(fill->getId());
    return fill;
}

Fill *Cell::getFill(size_t idx) const {
    if (fills_ == 0) return nullptr;
    ArrayObject<ObjectId>  *obj_vector =
        addr< ArrayObject<ObjectId> >(fills_);
    if (obj_vector == nullptr) return nullptr;
    ObjectId fill_id = (*obj_vector)[idx];
    return (addr<Fill>(fill_id));
}


/// @brief added a scan_chain into array scan_chains_
/// @return none
void Cell::addScanChain(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;

    if (scan_chains_ == 0) {
        vct = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getPool());
        vct->reserve(64);        
        scan_chains_ = vct->getId();
    } else {
        vct = addr< ArrayObject<ObjectId> >(scan_chains_);
    }

    if (vct) vct->pushBack(id);
}

ScanChain *Cell::createScanChain(std::string &name) {
    ScanChain *scan_chain = createObject<ScanChain>(kObjectTypeScanChain);
    scan_chain->setOwner(this);
    scan_chain->setChainName(name.c_str());
    addScanChain(scan_chain->getId());
    return scan_chain;
}

ScanChain *Cell::getScanChain(size_t idx) const {
    if (scan_chains_ == 0) return nullptr;
    ArrayObject<ObjectId>  *obj_vector =
        addr< ArrayObject<ObjectId> >(scan_chains_);
    if (obj_vector == nullptr) return nullptr;
    ObjectId sc_id = (*obj_vector)[idx];
    return (addr<ScanChain>(sc_id));
}

AnalysisMode *Cell::createAnalysisMode(std::string &name) {
    AnalysisMode *analysis_mode =
        createObject<AnalysisMode>(kObjectTypeAnalysisMode);
    if (analysis_mode == nullptr) return nullptr;
    analysis_mode->set_name(name);
    if (analysis_modes_ == 0) {
        ArrayObject<ObjectId> *am_vector = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        analysis_modes_ = am_vector->getId();
        am_vector->setPool(getPool());
        am_vector->reserve(32);      
    }
    ArrayObject<ObjectId> *am_vector =
        addr< ArrayObject<ObjectId>>(analysis_modes_);
    am_vector->pushBack(analysis_mode->getId());
    return analysis_mode;
}

AnalysisCorner *Cell::createAnalysisCorner(std::string &name) {
    AnalysisCorner *analysis_corner =
        createObject<AnalysisCorner>(kObjectTypeAnalysisCorner);
    if (analysis_corner == nullptr) return nullptr;
    analysis_corner->set_name(name);
    if (analysis_corners_ == 0) {
        ArrayObject<ObjectId> *ac_vector = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        analysis_corners_ = ac_vector->getId();
        ac_vector->setPool(getPool());
        ac_vector->reserve(32);      
    }
    ArrayObject<ObjectId> *ac_vector =
        addr< ArrayObject<ObjectId> >(analysis_corners_);
    ac_vector->pushBack(analysis_corner->getId());
    return analysis_corner;
}

AnalysisView *Cell::createAnalysisView(std::string &name) {
    AnalysisView *analysis_view =
        createObject<AnalysisView>(kObjectTypeAnalysisView);
    if (analysis_view == nullptr) return nullptr;
    analysis_view->set_name(name);
    if (analysis_views_ == 0) {
        ArrayObject<ObjectId> *av_vector = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        analysis_views_ = av_vector->getId();
        av_vector->setPool(getPool());
        av_vector->reserve(32);      
    }
    ArrayObject<ObjectId> *av_vector =
        addr< ArrayObject<ObjectId> >(analysis_views_);
    av_vector->pushBack(analysis_view->getId());
    return analysis_view;
}

void Cell::resetTerms(const std::vector<Term *> &terms) {
    if (terms.empty() && terms_ == UNINIT_OBJECT_ID) return;
    ArrayObject<ObjectId> *p = nullptr;
    
    if (terms_ == UNINIT_OBJECT_ID) {
        p = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        if (p) {
            terms_ = p->getId();
            p->setPool(getPool());
            p->reserve(32);
        }
    } else {
        p = addr< ArrayObject<ObjectId> >(terms_);
    }
    if (p != nullptr) {
        IndexType cur_size = terms.size();
        IndexType orig_size = p->getSize();
        if (cur_size < orig_size) {
            p->adjustSize(cur_size);

            IndexType index = 0;
            for (auto iter=p->begin(); iter != p->end(); ++iter) {
                auto &term = *iter;
                term = terms[index]->getId();
                ++index;
            }
        } else {
            IndexType index = 0;
            for (auto iter=p->begin(); iter != p->end(); ++iter) {
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

AnalysisMode *Cell::getAnalysisMode(std::string name) {
    if (analysis_modes_ == 0 || name == "") return nullptr;
    ArrayObject<ObjectId> *object_vector =
        addr< ArrayObject<ObjectId> >(analysis_modes_);
    if (object_vector == nullptr) return nullptr;
    for (auto iter = object_vector->begin(); iter != object_vector->end();
         iter++) {
        AnalysisMode *target = addr<AnalysisMode>(*iter);
        if (target && target->get_name() == name) return target;
    }
    return nullptr;
}

AnalysisCorner *Cell::getAnalysisCorner(std::string name) {
    if (analysis_corners_ == 0 || name == "") return nullptr;
    ArrayObject<ObjectId> *object_vector =
        addr< ArrayObject<ObjectId> >(analysis_corners_);
    if (object_vector == nullptr) return nullptr;
    for (auto iter = object_vector->begin(); iter != object_vector->end();
         iter++) {
        AnalysisCorner *target = addr<AnalysisCorner>(*iter);
        if (target && target->get_name() == name) return target;
    }
    return nullptr;
}

AnalysisView *Cell::getAnalysisView(std::string name) {
    if (analysis_views_ == 0 || name == "") return nullptr;
    ArrayObject<ObjectId> *object_vector =
        addr< ArrayObject<ObjectId> >(analysis_views_);
    if (object_vector == nullptr) return nullptr;
    for (auto iter = object_vector->begin(); iter != object_vector->end();
         iter++) {
        AnalysisView *target = addr<AnalysisView>(*iter);
        if (target && target->get_name() == name) return target;
    }
    return nullptr;
}

AnalysisView *Cell::getAnalysisView(size_t idx) const {
    if (analysis_views_ == 0) return nullptr;
    ArrayObject<ObjectId> *object_vector =
        addr< ArrayObject<ObjectId> >(analysis_views_);
    if (object_vector == nullptr) return nullptr;
    ObjectId id = (*object_vector)[idx];
    return addr<AnalysisView>(id);
}

void Cell::addActiveSetupView(ObjectId id) {
    if (id == 0) return;
    if (active_setup_views_ == 0) {
        ArrayObject<ObjectId> *asv_vector = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        active_setup_views_ = asv_vector->getId();
        asv_vector->setPool(getPool());
        asv_vector->reserve(32);      
    }
    ArrayObject<ObjectId> *asv_vector =
        addr< ArrayObject<ObjectId> >(active_setup_views_);
    asv_vector->pushBack(id);
}

void Cell::addActiveHoldView(ObjectId id) {
    if (id == 0) return;
    if (active_hold_views_ == 0) {
        ArrayObject<ObjectId> *ahv_vector = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        active_hold_views_ = ahv_vector->getId();
        ahv_vector->setPool(getPool());
        ahv_vector->reserve(32);      
    }
    ArrayObject<ObjectId> *ahv_vector =
        addr< ArrayObject<ObjectId> >(active_hold_views_);
    ahv_vector->pushBack(id);
}

std::string const &Cell::getClass() {
    return getSymbolTable()->getSymbolByIndex(macro_class_index_);
}

void Cell::setClass(const char *v) {
    int64_t idx = getOrCreateSymbol(v);
    if (idx != -1) {
        macro_class_index_ = idx;
    }
}

std::string const &Cell::getEEQ() {
    return getSymbolTable()->getSymbolByIndex(eeq_index_);
}

void Cell::setEEQ(const char *v) {
    int64_t idx = getOrCreateSymbol(v);
    if (idx != -1) {
        eeq_index_ = idx;
    }
}

std::string Cell::getSiteName() {
    Site *target = addr<Site>(site_index_);
    return target->getName();
}

Site* Cell::getSite() {
    return addr<Site>(site_index_);
}

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
    Site *target = addr<Site>(site_index_);
    return target->getName();
}

void SitePattern::setSiteID(ObjectId v) {
    site_index_ = v;
    //top_cell->addSymbolReference(name_index_, this->getId());
}

Site* SitePattern::getSite() {
    return addr<Site>(site_index_);
}

void Cell::addOBS(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;

    if (obses_ == 0) {
        vct = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        if (vct == nullptr) return;
        obses_ = vct->getId();
        vct->setPool(getPool());
        vct->reserve(32);         
    } else {
        vct = addr< ArrayObject<ObjectId> >(obses_);
    }

    if (vct) vct->pushBack(id);
}

LayerGeometry *Cell::getOBS(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (obses_ == 0) {
        return nullptr;
    } else {
        vct = addr< ArrayObject<ObjectId> >(obses_);
    }
    if (vct) {
        LayerGeometry *obj_data =
            addr<LayerGeometry>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

void Cell::addForeign(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;

    if (foreigns_ == 0) {
        vct = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        if (vct == nullptr) return;
        foreigns_ = vct->getId();
        vct->setPool(getPool());
        vct->reserve(32);        
    } else {
        vct = addr< ArrayObject<ObjectId> >(foreigns_);
    }

    if (vct) vct->pushBack(id);
}

Foreign *Cell::getForeign(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (foreigns_ == 0) {
        return nullptr;
    } else {
        vct = addr< ArrayObject<ObjectId> >(foreigns_);
    }
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
        vct = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        if (vct == nullptr) return;
        site_patterns_ = vct->getId();
        vct->setPool(getPool());
        vct->reserve(32);        
    } else {
        vct = addr< ArrayObject<ObjectId> >(site_patterns_);
    }

    if (vct) vct->pushBack(id);
}

SitePattern *Cell::getSitePattern(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (site_patterns_ == 0) {
        return nullptr;
    } else {
        vct = addr< ArrayObject<ObjectId> >(site_patterns_);
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
        Cell *top_cell = getTopCell();
        if (!top_cell) {
            message->issueMsg(
                kError, "Cannot find top cell when create vectorobject32.\n");
            return;  // risk , need opt
        }
        vct = createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
        if (vct == nullptr) return;
        densities_ = vct->getId();
        vct->setPool(getPool());
        vct->reserve(32);        
    } else {
        vct = addr< ArrayObject<ObjectId> >(densities_);
    }

    if (vct) vct->pushBack(id);
}

Density *Cell::getDensity(int index) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (densities_ == 0) {
        return nullptr;
    } else {
        vct = addr< ArrayObject<ObjectId> >(densities_);
    }
    if (vct) {
        Density *obj_data = addr<Density>((*vct)[index]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

void Cell::print() {
    Tech *lib = getTopCell()->getTechLib();
    // message->info("***************open-edi macro print
    // begin******************\n");
    message->info("MACRO %s \n", getName().c_str());
    message->info("CLASS %s ;\n", getClass().c_str());
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

    ArrayObject<ObjectId>  *vct = nullptr;
    if (obses_ != 0) {
        vct = addr< ArrayObject<ObjectId> >(obses_);
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
        vct = addr< ArrayObject<ObjectId> >(densities_);
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
    // message->info("***************open-edi macro print
    // end******************\n");
}

void Cell::printLEF(std::ofstream &ofs) {
    Tech *lib = getTopCell()->getTechLib();
    // message->info("***************open-edi macro print
    // begin******************\n");
    ofs << "MACRO"
        << " " << getName().c_str() << " \n";
    if (getClass().size() > 0) {
        ofs << "   CLASS"
            << " " << getClass().c_str() << " ;\n";
    }
    if (getIsFixedMask()) {
        ofs << "   FIXEDMASK ;\n";
    }
    if (numForeigns() > 0) {
        for (int i = 0; i < numForeigns(); i++) {
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
    if (getNumSites() > 0) {
        for (int i = 0; i < getNumSites(); i++) {
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
        vct = addr< ArrayObject<ObjectId> >(obses_);
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
        vct = addr< ArrayObject<ObjectId> >(densities_);
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
