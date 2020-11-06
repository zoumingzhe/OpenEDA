/**
 * @file  fplan.cpp
 * @date  08/24/2020 09:54:42 AM CST
 * @brief "Implementation of Floorplan related classes"
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include <map>
#include <algorithm>
#include <utility>
#include <vector>
#include <string>

#include "db/core/fplan.h"

#include "db/core/cell.h"
#include "db/core/db.h"
#include "db/util/symbol_table.h"
#include "db/util/array.h"
#include "util/util_mem.h"

namespace open_edi {
namespace db {

/// @brief Row
Row::Row() {
    memset(reinterpret_cast<void *>(this), 0, sizeof(Row));
    row_name_index_ = 0;
    site_name_index_ = 0;
    bbox_ = Box();
    row_orient_ = Direction::kUnknown;
    site_count_ = 0;
}

/// @brief ~Row
Row::~Row() {
    deleteSparseObject(kObjectTypeProperty);
}

/// @brief getName
///
/// @return
std::string &Row::getName() {
    return getFloorplan()->getCell()->getSymbolByIndex(row_name_index_);
}

/// @brief setName
///
/// @param name
void Row::setName(const SymbolIndex &row_name_index) {
    row_name_index_ = row_name_index;
    getFloorplan()->getCell()->addSymbolReference(row_name_index_,
                                                  this->getId());
}

/// @brief setName
///
/// @param name
///
/// @return
bool Row::setName(const std::string &name) {
    int64_t index = getFloorplan()->getCell()->getOrCreateSymbol(name);
    if (index == -1) return false;

    row_name_index_ = index;
    getFloorplan()->getCell()->addSymbolReference(row_name_index_,
                                                  this->getId());
    return true;
}

/// @brief setName
///
/// @param name
///
/// @return
bool Row::setName(const char *name) {
    int64_t index = getFloorplan()->getCell()->getOrCreateSymbol(name);
    if (index == -1) return false;

    row_name_index_ = index;
    getFloorplan()->getCell()->addSymbolReference(row_name_index_,
                                                  this->getId());
    return true;
}

/// @brief &getBox
///
/// @return
Box Row::getBox() const { return bbox_; }

/// @brief setBox
///
/// @param b
void Row::setBox(const Box &b) { bbox_ = b; }

/// @brief getOrient
///
/// @return
Direction Row::getRowOrient() const { return row_orient_; }

/// @brief setOrient
///
/// @param o
void Row::setRowOrient(const Direction &o) { row_orient_ = o; }

void Row::setSiteId(const ObjectId &site_id) { site_id_ = site_id; }

Site *Row::getSite() const {
    return addr<Site>(site_id_);
}

ObjectId Row::getSiteId() const { return site_id_; }

/// @brief getSiteCount
///
/// @return
int32_t Row::getSiteCount() const { return site_count_; }

/// @brief setSiteCount
///
/// @param sc
void Row::setSiteCount(const int32_t &sc) { site_count_ = sc; }

/// @brief setFloorplan
///
/// @param fp
void Row::setFloorplan(ObjectId fp) { floorplan_ = fp; }

/// @brief getFloorplan
///
/// @return
Floorplan *Row::getFloorplan() {
    return addr<Floorplan>(floorplan_);
}

void Row::setSiteName(const char *site_name) {
    if (!site_name) {
        message->issueMsg(kError, "site name is null.\n");
        return;
    }

    site_name_index_ = getFloorplan()->getCell()->getOrCreateSymbol(site_name);
    getFloorplan()->getCell()->addSymbolReference(site_name_index_,
                                                  this->getId());
}

std::string &Row::getSiteName() {
    return getFloorplan()->getCell()->getSymbolByIndex(site_name_index_);
}

uint64_t Row::getNumProperties() const {
    kSparsePair = kSparseMap.equal_range(IdType(getId(), kObjectTypeProperty));
    return std::distance(kSparsePair.first, kSparsePair.second);
}

void Row::addProperty(ObjectId obj_id) {
    if (obj_id == 0) return;

    kSparseMap.insert(
            std::make_pair(IdType(this->getId(), kObjectTypeProperty), obj_id));

    has_property_ = true;
}

void Row::print() {
    message->info("ROW %s %s %d %d %s", getName().c_str(),
                  getSiteName().c_str(), getOrigX(), getOrigY(),
                  toString(getSiteOrient()).c_str());
    if (getHasDo()) {
        message->info(" DO %d BY %d", getNumX(), getNumY());
        if (getHasDoStep()) {
            message->info(" STEP %d %d ;\n", getStepX(), getStepY());
        } else {
            message->info(" ;\n");
        }
    }
}

void Row::print(FILE *fp) {
    if (!fp) {
        return;
    }
    fprintf(fp, "ROW %s %s %d %d %s", getName().c_str(), getSiteName().c_str(),
            getOrigX(), getOrigY(), toString(getSiteOrient()).c_str());
    if (getHasDo()) {
        fprintf(fp, " DO %d BY %d", getNumX(), getNumY());
        if (getHasDoStep()) {
            fprintf(fp, " STEP %d %d", getStepX(), getStepY());
        }
    }

    writeDEFProperty<Row>(reinterpret_cast<void *>(this), fp);

    fprintf(fp, " ;\n");
}

/// @brief Track default constructor
Track::Track() { memset(reinterpret_cast<void *>(this), 0, sizeof(Track)); }

/// @brief ~Track
Track::~Track() {}

void Track::setFloorplan(ObjectId fp) { floorplan_ = fp; }

Floorplan *Track::getFloorplan() {
    return addr<Floorplan>(floorplan_);
}

void Track::setDirectionX(bool direction_x) { direction_x_ = direction_x; }
bool Track::getDirectionX() { return direction_x_; }

void Track::setStart(int32_t start) { start_ = start; }
int32_t Track::getStart() { return start_; }

void Track::setNumTracks(uint32_t num_tracks) { num_tracks_ = num_tracks; }
uint32_t Track::getNumTracks() { return num_tracks_; }

void Track::setSpace(uint32_t space) { space_ = space; }
uint32_t Track::getSpace() { return space_; }

void Track::setHasMask(bool has_mask) { has_mask_ = has_mask; }
bool Track::getHasMask() { return has_mask_; }

void Track::setMask(uint8_t mask) { mask_ = mask; }
uint8_t Track::getMask() { return mask_; }

void Track::setHasSameMask(bool has_same_mask) {
    has_same_mask_ = has_same_mask;
}
bool Track::getHasSameMask() { return has_same_mask_; }

void Track::addLayer(const Int32 &layer_index) {
    ArrayObject<Int32> *vct = nullptr;
    if (layers_ == 0) {
        vct = getTopCell()->createObject<ArrayObject<Int32>>(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getTopCell()->getPool());
        vct->reserve(256);
        layers_ = vct->getId();
    } else {
        vct = addr< ArrayObject<Int32> >(layers_);
    }

    if (vct) vct->pushBack(layer_index);
}

void Track::addLayer(const char *layer_name) {
    Cell *top_cell = getTopCell();
    Tech *tech_lib = top_cell->getTechLib();
    if (!tech_lib) {
        message->issueMsg(kError, "Cannot find Tech LEF when reading Track \n");
        return;
    }

    Int32 layer_index = tech_lib->getLayerLEFIndexByName(layer_name);
    if (layer_index < 0) {
        message->issueMsg(kError, "Cannot find layer %s in LEF.\n", layer_name);
        return;
    }

    ArrayObject<Int32> *vct = nullptr;
    if (layers_ == 0) {
        vct = getTopCell()->createObject<ArrayObject<Int32>>(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getTopCell()->getPool());
        vct->reserve(256);
        layers_ = vct->getId();
    } else {
        vct = addr< ArrayObject<Int32> >(layers_);
    }

    if (vct) vct->pushBack(layer_index);
}

void Track::addLayer(const std::string &layer_name) {
    Cell *top_cell = getTopCell();
    Tech *tech_lib = top_cell->getTechLib();
    if (!tech_lib) {
        message->issueMsg(kError, "Cannot find Tech LEF when reading Track \n");
        return;
    }

    Int32 layer_index = tech_lib->getLayerLEFIndexByName(layer_name.c_str());
    if (layer_index < 0) {
        message->issueMsg(kError, "Cannot find layer %s in LEF.\n",
                          layer_name.c_str());
        return;
    }

    ArrayObject<Int32> *vct = nullptr;
    if (layers_ == 0) {
        vct = getTopCell()->createObject<ArrayObject<Int32>>(kObjectTypeArray);
        if (vct == nullptr) return;
        vct->setPool(getTopCell()->getPool());
        vct->reserve(256);
        layers_ = vct->getId();
    } else {
        vct = addr< ArrayObject<Int32> >(layers_);
    }

    if (vct) vct->pushBack(layer_index);
}

ObjectId Track::getLayers() const { return layers_; }

void Track::print() {
    Cell *top_cell = getTopCell();
    Tech *tech_lib = top_cell->getTechLib();
    if (!tech_lib) {
        message->issueMsg(kError, "Cannot find Tech LEF when print Track \n");
        return;
    }
    message->info("TRACKS %c", getDirectionX() ? 'X' : 'Y');
    message->info(" %d", getStart());
    message->info(" DO %d", getNumTracks());
    message->info(" STEP %d", getSpace());

    if (getHasMask()) {
        message->info(" MASK %d", getMask());
        if (getHasSameMask()) {
            message->info(" SAMEMASK");
        }
    }

    ArrayObject<Int32> *vct = addr< ArrayObject<Int32> >(layers_);
    for (int i = 0; i < vct->getSize(); ++i) {
        Int32 layer_index = (*vct)[i];
        Layer *layer = tech_lib->getLayer(layer_index);
        if (0 == i) {
            message->info(" LAYER");
        }
        message->info(" %s", layer->getName());
    }
    message->info(" ;\n");
}

void Track::print(FILE *fp) {
    Cell *top_cell = getTopCell();
    Tech *tech_lib = top_cell->getTechLib();
    if (!tech_lib) {
        message->issueMsg(kError, "Cannot find Tech LEF when print Track \n");
        return;
    }
    fprintf(fp, "TRACKS %c", getDirectionX() ? 'X' : 'Y');
    fprintf(fp, " %d", getStart());
    fprintf(fp, " DO %d", getNumTracks());
    fprintf(fp, " STEP %d", getSpace());

    if (getHasMask()) {
        fprintf(fp, " MASK %d", getMask());
        if (getHasSameMask()) {
            fprintf(fp, " SAMEMASK");
        }
    }

    if (layers_ > 0) {
        ArrayObject<Int32> *vct = addr< ArrayObject<Int32> >(layers_);
        for (int i = 0; i < vct->getSize(); ++i) {
            Int32 layer_index = (*vct)[i];
            Layer *layer = tech_lib->getLayer(layer_index);
            if (0 == i) {
                fprintf(fp, " LAYER");
            }
            fprintf(fp, " %s", layer->getName());
        }
    }
    fprintf(fp, " ;\n");
}

Grid::Grid() { memset(reinterpret_cast<void *>(this), 0, sizeof(Grid)); }

/// @brief Grid
///
/// @param t
Grid::Grid(Grid::GridType t) {
    memset(reinterpret_cast<void *>(this), 0, sizeof(Grid));
    grid_type_ = t;
}

Grid::~Grid() {}

/// @brief setGridType
///
/// @param t
void Grid::setGridType(Grid::GridType t) { grid_type_ = t; }

/// @brief getGridType
///
/// @return
Grid::GridType Grid::getGridType() { return grid_type_; }

void Grid::setFloorplan(ObjectId fp) { floorplan_ = fp; }

Floorplan *Grid::getFloorplan() {
    return addr<Floorplan>(floorplan_);
}

void Grid::setDirectionX(bool direction_x) { direction_x_ = direction_x; }
bool Grid::getDirectionX() { return direction_x_; }

void Grid::setStart(int32_t start) { start_ = start; }
int32_t Grid::getStart() { return start_; }

void Grid::setNumber(uint32_t number) { number_ = number; }
uint32_t Grid::getNumber() { return number_; }

void Grid::setSpace(uint32_t space) { space_ = space; }
uint32_t Grid::getSpace() { return space_; }

void Grid::print() {
    Cell *top_cell = getTopCell();
    Tech *tech_lib = top_cell->getTechLib();
    if (!tech_lib) {
        message->issueMsg(kError, "Cannot find Tech LEF when print Grid.\n");
        return;
    }
    message->info("GCELLGRID %c", getDirectionX() ? 'X' : 'Y');
    message->info(" %d", getStart());
    message->info(" DO %d", getNumber() + 1);  // +1 based on DEF syntax.
    message->info(" STEP %d", getSpace());

    message->info(" ;\n");
}

void Grid::print(FILE *fp) {
    Cell *top_cell = getTopCell();
    Tech *tech_lib = top_cell->getTechLib();
    if (!tech_lib) {
        message->issueMsg(kError, "Cannot find Tech LEF when print Grid.\n");
        return;
    }
    fprintf(fp, "GCELLGRID %c", getDirectionX() ? 'X' : 'Y');
    fprintf(fp, " %d", getStart());
    fprintf(fp, " DO %d", getNumber() + 1);  // +1 based on DEF syntax.
    fprintf(fp, " STEP %d", getSpace());

    fprintf(fp, " ;\n");
}

uint64_t Floorplan::getNumOfRows() const {
    if (rows_ == 0) return 0;
    VectorObject64 *obj_vector = addr<VectorObject64>(rows_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->totalSize();
}

Row *Floorplan::createRow() {
    Row *row = getCell()->createObject<Row>(kObjectTypeRow);
    row->setOwner(cell_);
    row->setFloorplan(this->getId());
    if (rows_ == 0) {
        rows_ = getCell()->createVectorObject<VectorObject64>()->getId();
    }
    VectorObject64 *row_vector = addr<VectorObject64>(rows_);
    row_vector->push_back(row->getId());
    return row;
}

ObjectId Floorplan::getRows() const { return rows_; }

uint64_t Floorplan::getNumOfTracks() const {
    if (tracks_ == 0) return 0;
    VectorObject64 *obj_vector = addr<VectorObject64>(tracks_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->totalSize();
}

Track *Floorplan::createTrack() {
    Track *track = getCell()->createObject<Track>(kObjectTypeTrack);
    track->setOwner(cell_);
    track->setFloorplan(this->getId());
    if (tracks_ == 0) {
        tracks_ = getCell()->createVectorObject<VectorObject64>()->getId();
    }
    VectorObject64 *track_vector = addr<VectorObject64>(tracks_);
    track_vector->push_back(track->getId());
    return track;
}

ObjectId Floorplan::getTracks() const { return tracks_; }

uint64_t Floorplan::getNumOfGrids() const {
    if (gcell_grids_ == 0) return 0;
    VectorObject64 *obj_vector = addr<VectorObject64>(gcell_grids_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->totalSize();
}

Grid *Floorplan::createGcellGrid() {
    Grid *grid = getCell()->createObject<Grid>(kObjectTypeGrid);
    grid->setOwner(cell_);
    grid->setGridType(Grid::kGridGcell);
    grid->setFloorplan(this->getId());
    if (gcell_grids_ == 0) {
        gcell_grids_ = getCell()->createVectorObject<VectorObject64>()->getId();
    }
    VectorObject64 *grid_vector = addr<VectorObject64>(gcell_grids_);
    grid_vector->push_back(grid->getId());
    return grid;
}

ObjectId Floorplan::getGcellGrids() const { return gcell_grids_; }

void Floorplan::setDieArea(ObjectIndex die_area) { die_area_ = die_area; }

ObjectIndex Floorplan::getDieArea() { return die_area_; }
Polygon *Floorplan::getDieAreaPolygon() {
    if (die_area_ == -1) {
        return nullptr;
    }

    if (!getCell()) {
        message->issueMsg(kError,
                          "Cannot get top cell when getting die area.\n");
        return nullptr;
    }
    if (!getCell()->getPolygonTable()) {
        message->issueMsg(kError,
                          "Cannot get polygon tale when getting die area.\n");
        return nullptr;
    }

    return getCell()->getPolygonTable()->getPolygonByIndex(die_area_);
}

// Floorplan class
/// @brief Floorplan
Floorplan::Floorplan() {
    memset(reinterpret_cast<void *>(this), 0, sizeof(Floorplan));
    die_area_ = -1;
}

/// @brief ~Floorplan
Floorplan::~Floorplan() {}

/// @brief getCoreBox
///
/// @return
Box Floorplan::getCoreBox() const { return core_box_; }
/// @brief setCoreBox
///
/// @param box
void Floorplan::setCoreBox(const Box &box) { core_box_ = box; }

/// @brief getXOffset
///
/// @return
int32_t Floorplan::getXOffset() const { return x_offset_; }
/// @brief setXOffset
///
/// @param offset
void Floorplan::setXOffset(const int32_t &offset) { x_offset_ = offset; }

/// @brief getYOffset
///
/// @return
int32_t Floorplan::getYOffset() const { return y_offset_; }
/// @brief setYOffset
///
/// @param offset
void Floorplan::setYOffset(const int32_t &offset) { y_offset_ = offset; }

/// @brief getCoreSite
///
/// @return
Site *Floorplan::getCoreSite() const {
    return addr<Site>(core_site_id_);
}
/// @brief getCoreSiteId
///
/// @return
ObjectId Floorplan::getCoreSiteId() const { return core_site_id_; }
/// @brief setCoreSite
///
/// @param id
void Floorplan::setCoreSiteId(const ObjectId &id) { core_site_id_ = id; }

void Floorplan::setCell(ObjectId cell) { cell_ = cell; }

Cell *Floorplan::getCell() { return addr<Cell>(cell_); }

Constraint *Floorplan::createPlaceBlockage() {
    VectorObject32 *vct = nullptr;
    Constraint *cons = nullptr;

    if (place_blockages_ == 0) {
        vct = getCell()->createVectorObject<VectorObject32>();
        if (vct == nullptr) return nullptr;
        place_blockages_ = vct->getId();
    } else {
        vct = addr<VectorObject32>(place_blockages_);
    }

    cons = getCell()->createObject<Constraint>(kObjectTypePhysicalConstraint);
    if (cons == nullptr) {
        getCell()->deleteObject<Constraint>(cons);
        return nullptr;
    }
    cons->setFloorplan(this->getId());
    cons->setConstraintType(Constraint::kConstraintPBlkg);
    vct->push_back(cons->getId());

    return cons;
}

Constraint *Floorplan::createRegion(const char *name) {
    VectorObject32 *vct = nullptr;
    Constraint *region = nullptr;

    if (regions_ == 0) {
        vct = getCell()->createVectorObject<VectorObject32>();
        if (vct == nullptr) return nullptr;
        regions_ = vct->getId();
    } else {
        vct = addr<VectorObject32>(regions_);
    }

    region = getCell()->createObject<Constraint>(kObjectTypeRegion);
    if (region == nullptr) {
        getCell()->deleteObject<Constraint>(region);
        return nullptr;
    }
    region->setFloorplan(this->getId());
    region->setName(name);
    region->setConstraintType(Constraint::kConstraintRegion);
    vct->push_back(region->getId());

    return region;
}
uint64_t Floorplan::getNumOfRegions() const {
    if (regions_ == 0) return 0;
    VectorObject32 *obj_vector = addr<VectorObject32>(regions_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->totalSize();
}
ObjectId Floorplan::getRegions() const { return regions_; }
Constraint *Floorplan::getRegion(const std::string &name) const {
    if (regions_ == 0) return nullptr;
    Cell *top_cell = getTopCell();
    SymbolIndex symbol_index = top_cell->getOrCreateSymbol(name.c_str());
    if (symbol_index == -1) return nullptr;

    if (symbol_index) {
        std::vector<ObjectId> object_vector =
            top_cell->getSymbolTable()->getReferences(symbol_index);
        for (auto iter = object_vector.begin(); iter != object_vector.end();
             iter++) {
            Constraint *region = addr<Constraint>(*iter);
            if (region && (region->getObjectType() == kObjectTypeRegion))
                return region;
        }
    }

    return nullptr;
}

uint64_t Floorplan::getNumOfPlaceBlockages() const {
    if (place_blockages_ == 0) return 0;
    VectorObject32 *obj_vector = addr<VectorObject32>(place_blockages_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->totalSize();
}
ObjectId Floorplan::getPlaceBlockages() const { return place_blockages_; }

Constraint *Floorplan::createRouteBlockage() {
    VectorObject32 *vct = nullptr;
    Constraint *cons = nullptr;

    if (route_blockages_ == 0) {
        vct = getCell()->createVectorObject<VectorObject32>();
        if (vct == nullptr) return nullptr;
        route_blockages_ = vct->getId();
    } else {
        vct = addr<VectorObject32>(route_blockages_);
    }

    cons = getCell()->createObject<Constraint>(kObjectTypePhysicalConstraint);
    if (cons == nullptr) {
        getCell()->deleteObject<Constraint>(cons);
        return nullptr;
    }
    cons->setFloorplan(this->getId());
    cons->setConstraintType(Constraint::kConstraintRBlkg);
    vct->push_back(cons->getId());

    return cons;
}
uint64_t Floorplan::getNumOfRouteBlockages() const {
    if (route_blockages_ == 0) return 0;
    VectorObject32 *obj_vector = addr<VectorObject32>(route_blockages_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->totalSize();
}

ObjectId Floorplan::getRouteBlockages() const { return route_blockages_; }
// Constraint class
//
using ConstraintType = Constraint::ConstraintType;
using ConstraintSubType = Constraint::ConstraintSubType;

Constraint::Constraint() { type_ = kConstraintNone; }

Constraint::~Constraint() {
    type_ = kConstraintNone;
    deleteSparseObject(kObjectTypeProperty);
}

SymbolIndex Constraint::getNameIndex() { return name_; }

std::string &Constraint::getName() {
    return getFloorplan()->getCell()->getSymbolByIndex(name_);
}

void Constraint::setName(const SymbolIndex &name) {
    name_ = name;
    getFloorplan()->getCell()->addSymbolReference(name_, this->getId());
}

bool Constraint::setName(const std::string &name) {
    int64_t index = getFloorplan()->getCell()->getOrCreateSymbol(name);
    if (index == -1) return false;

    name_ = index;
    getFloorplan()->getCell()->addSymbolReference(name_, this->getId());
    return true;
}

bool Constraint::setName(const char *name) {
    int64_t index = getFloorplan()->getCell()->getOrCreateSymbol(name);
    if (index == -1) return false;

    name_ = index;
    getFloorplan()->getCell()->addSymbolReference(name_, this->getId());
    return true;
}

Constraint::ConstraintType Constraint::getConstraintType() { return type_; }

void Constraint::setConstraintType(ConstraintType t) { type_ = t; }

ConstraintSubType Constraint::getConstraintSubType() { return sub_type_; }

void Constraint::setConstraintSubType(ConstraintSubType st) { sub_type_ = st; }

int32_t Constraint::getUtilization() { return utilization_; }

void Constraint::setUtilization(const int32_t &u) { utilization_ = u; }

/* TODO
Shape Constraint::getShape()
{
}
void Constraint::setShape(Shape &s)
{
}
*/

/// @brief setFloorplan
///
/// @param fp
void Constraint::setFloorplan(ObjectId fp) { floorplan_ = fp; }

/// @brief getFloorplan
///
/// @return
Floorplan *Constraint::getFloorplan() {
    return addr<Floorplan>(floorplan_);
}

void Constraint::setHasLayer(bool has_layer) { has_layer_ = has_layer; }

bool Constraint::setLayer(const char *name) {
    Cell *top_cell = getTopCell();
    Tech *tech_lib = top_cell->getTechLib();
    if (!tech_lib) {
        message->issueMsg(
            kError, "Cannot find Tech LEF when reading layer %s.\n", name);
        return false;
    }

    Int32 layer_index = tech_lib->getLayerLEFIndexByName(name);
    if (layer_index < 0) {
        message->issueMsg(kError, "Cannot find layer %s in LEF.\n", name);
        return false;
    }

    layer_index_ = layer_index;
    return true;
}

void Constraint::setPlacement(bool has_placement) {
    has_placement_ = has_placement;
}

void Constraint::setHasComponent(bool has_component) {
    has_component_ = has_component;
}

bool Constraint::setComponent(const char *name) {
    if (!name) {
        message->issueMsg(kError, "blockage's component is nullptr.\n");
        return false;
    }
    component_name_ = name;
    Cell *top_cell = getTopCell();
    Inst *instance = top_cell->getInstance(name);
    if (!instance) {
        message->issueMsg(kError, "Cannot find component %s in blockage.\n",
                          name);
        return false;
    }
    component_id_ = instance->getId();

    return true;
}
/// @brief component get component pointer
///
/// @return
Inst *Constraint::component() const {
    return addr<Inst>(component_id_);
}

void Constraint::setSlots(bool has_slots) { has_slots_ = has_slots; }

void Constraint::setFills(bool has_fills) { has_fills_ = has_fills; }

void Constraint::setPushdown(bool has_pushdown) {
    has_pushdown_ = has_pushdown;
}

void Constraint::setExceptpgnet(bool has_exceptpgnet) {
    has_exceptpgnet_ = has_exceptpgnet;
}

void Constraint::setSoft(bool has_soft) { has_soft_ = has_soft; }

void Constraint::setPartial(bool has_partial) { has_partial_ = has_partial; }
void Constraint::setDensity(double max_density) { max_density_ = max_density; }

void Constraint::setSpacing(bool has_spacing) { has_spacing_ = has_spacing; }
void Constraint::setMinSpacing(Bits64 min_spacing) {
    min_spacing_ = min_spacing;
}

void Constraint::setDesignRuleWidth(bool has_design_rule_width) {
    has_design_rule_width_ = has_design_rule_width;
}
void Constraint::setEffectiveWidth(Bits64 effective_width) {
    effective_width_ = effective_width;
}

void Constraint::setMask(bool has_mask) { has_mask_ = has_mask; }
void Constraint::setMaskNum(Bits64 mask_num) { mask_num_ = mask_num; }

bool Constraint::hasLayer() const { return has_layer_; }

bool Constraint::hasPlacement() const { return has_placement_; }

bool Constraint::hasComponent() const { return has_component_; }

bool Constraint::hasSlots() const { return has_slots_; }

bool Constraint::hasFills() const { return has_fills_; }

bool Constraint::hasPushdown() const { return has_pushdown_; }

// 5.7
bool Constraint::hasExceptpgnet() const { return has_exceptpgnet_; }

// 5.7
bool Constraint::hasSoft() const { return has_soft_; }

// 5.7
bool Constraint::hasPartial() const { return has_partial_; }

bool Constraint::hasSpacing() const { return has_spacing_; }

Bits64 Constraint::minSpacing() const { return min_spacing_; }

bool Constraint::hasDesignRuleWidth() const { return has_design_rule_width_; }

Bits64 Constraint::effectiveWidth() const { return effective_width_; }

bool Constraint::hasMask() const { return has_mask_; }

Bits64 Constraint::maskNum() const { return mask_num_; }

// 5.7
double Constraint::placementMaxDensity() const { return max_density_; }

Int32 Constraint::layerId() const { return layer_index_; }

std::string Constraint::layerName() const { return layer_name_; }

ObjectId Constraint::componentId() const { return component_id_; }

std::string Constraint::componentName() const { return component_name_; }

Box *Constraint::createBox(int64_t xl, int64_t yl, int64_t xh, int64_t yh) {
    Cell *top_cell = getTopCell();
    Box *box = top_cell->createObject<Box>(kObjectTypeBox);
    if (!box) {
        message->issueMsg(kError, "create box failed.\n");
        return nullptr;
    }
    box->setOwner(top_cell);
    box->setLLX(xl);
    box->setLLY(yl);
    box->setURX(xh);
    box->setURY(yh);
    if (boxes_id_ == 0) {
        boxes_id_ = top_cell->createVectorObject<VectorObject256>()->getId();
    }
    VectorObject256 *box_vector = addr<VectorObject256>(boxes_id_);
    box_vector->push_back(box->getId());
    return box;
}
ObjectId Constraint::getBoxesId() const { return boxes_id_; }

/// @brief addPolygon
///
/// @param polygon:
///
/// @return
void Constraint::addPolygon(Polygon *polygon) {
    Cell *top_cell = getTopCell();
    PolygonTable *polygon_table = top_cell->getPolygonTable();
    if (polygons_id_ == 0) {
        polygons_id_ = top_cell->createVectorObject<VectorObject8>()->getId();
    }
    VectorObject8 *polygon_vector = addr<VectorObject8>(polygons_id_);
    ObjectIndex polygon_index = polygon_table->addPolygon(polygon);
    polygon_vector->push_back(polygon_index);
    return;
}

ObjectId Constraint::getPolygonsId() const { return polygons_id_; }

void Constraint::printBlockage() const {
    Cell *top_cell = getTopCell();
    if (!top_cell) {
        message->issueMsg(kError,
                          "Cannot find top cell when outputting blockage.\n");
        return;
    }

    if (hasLayer()) {
        Layer *layer = top_cell->getLayerByLayerId(layer_index_);
        if (!layer) {
            message->issueMsg(kError, "Cannot find layer with id %d.\n",
                              layer_index_);
            return;
        }

        message->info("    - LAYER %s", layer->getName());
        if (hasSlots()) message->info(" + SLOTS");
        if (hasFills()) message->info(" + FILLS");
        if (hasPushdown()) message->info(" + PUSHDOWN");
        if (hasExceptpgnet()) message->info(" + EXCEPTPGNET");
        if (hasComponent()) {
            // Inst *instance = top_cell->getInstance(component_id_);
            message->info(" + COMPONENT %s", componentName().c_str());
        }
        if (hasSpacing()) {
            message->info(" + SPACING %d", minSpacing());
        }
        if (hasDesignRuleWidth()) {
            message->info(" + DESIGNRULEWIDTH %d", effectiveWidth());
        }
        if (hasMask()) {
            message->info(" + MASK %d", maskNum());
        }
        message->info("\n");
    }
    if (hasPlacement()) {
        message->info("    - PLACEMENT");
        if (hasSoft()) message->info(" + SOFT");
        if (hasPartial())
            message->info(" + PARTIAL %.2f", placementMaxDensity());
        if (hasPushdown()) message->info(" + PUSHDOWN");
        if (hasComponent()) {
            message->info(" + COMPONENT %s", componentName().c_str());
        }
        message->info("\n");
    }

    if (boxes_id_ > 0) {
        VectorObject256 *box_vector = addr<VectorObject256>(boxes_id_);
        uint32_t num_boxes = box_vector->totalSize();
        for (int i = 0; i < num_boxes; ++i) {
            Box *box = addr<Box>((*box_vector)[i]);
            if (!box) {
                continue;
            }
            if (i < num_boxes - 1) {
                message->info("        RECT ( %d %d ) ( %d %d )\n",
                              box->getLLX(), box->getLLY(), box->getURX(),
                              box->getURY());
            } else {
                message->info("        RECT ( %d %d ) ( %d %d ) ;\n",
                              box->getLLX(), box->getLLY(), box->getURX(),
                              box->getURY());
            }
        }
    }

    if (polygons_id_ > 0) {
        PolygonTable *polygon_table = top_cell->getPolygonTable();
        VectorObject8 *polygon_index_vector = addr<VectorObject8>(polygons_id_);
        uint32_t num_polygons = polygon_index_vector->totalSize();
        for (int i = 0; i < num_polygons; i++) {
            ObjectIndex index = (*polygon_index_vector)[i];
            Polygon *polygon = polygon_table->getPolygonByIndex(index);
            message->info("        POLYGON");
            for (int j = 0; j < polygon->getNumPoints(); ++j) {
                Point point = polygon->getPoint(j);
                message->info(" ( %d %d )", point.getX(), point.getY());
            }
            message->info(" ;\n");
        }
    }
}

void Constraint::printBlockage(FILE *fp) const {
    Cell *top_cell = getTopCell();
    if (!top_cell) {
        message->issueMsg(kError,
                          "Cannot find top cell when outputting blockage.\n");
        return;
    }

    if (hasLayer()) {
        Layer *layer = top_cell->getLayerByLayerId(layer_index_);
        if (!layer) {
            message->issueMsg(kError, "Cannot find layer with id %d.\n",
                              layer_index_);
            return;
        }

        fprintf(fp, "    - LAYER %s", layer->getName());
        if (hasSlots()) fprintf(fp, " + SLOTS");
        if (hasFills()) fprintf(fp, " + FILLS");
        if (hasPushdown()) fprintf(fp, " + PUSHDOWN");
        if (hasExceptpgnet()) fprintf(fp, " + EXCEPTPGNET");
        if (hasComponent()) {
            // Inst *instance = top_cell->getInstance(component_id_);
            fprintf(fp, " + COMPONENT %s", componentName().c_str());
        }
        if (hasSpacing()) {
            fprintf(fp, " + SPACING %d", minSpacing());
        }
        if (hasDesignRuleWidth()) {
            fprintf(fp, " + DESIGNRULEWIDTH %d", effectiveWidth());
        }
        if (hasMask()) {
            fprintf(fp, " + MASK %d", maskNum());
        }
        fprintf(fp, "\n");
    }
    if (hasPlacement()) {
        fprintf(fp, "    - PLACEMENT");
        if (hasSoft()) fprintf(fp, " + SOFT");
        if (hasPartial()) fprintf(fp, " + PARTIAL %.2f", placementMaxDensity());
        if (hasPushdown()) fprintf(fp, " + PUSHDOWN");
        if (hasComponent()) {
            fprintf(fp, " + COMPONENT %s", componentName().c_str());
        }
        fprintf(fp, "\n");
    }

    if (boxes_id_ > 0) {
        VectorObject256 *box_vector = addr<VectorObject256>(boxes_id_);
        uint32_t num_boxes = box_vector->totalSize();
        for (int i = 0; i < num_boxes; ++i) {
            Box *box = addr<Box>((*box_vector)[i]);
            if (!box) {
                continue;
            }
            if (i < num_boxes - 1) {
                fprintf(fp, "        RECT ( %d %d ) ( %d %d )\n", box->getLLX(),
                        box->getLLY(), box->getURX(), box->getURY());
            } else {
                fprintf(fp, "        RECT ( %d %d ) ( %d %d ) ;\n",
                        box->getLLX(), box->getLLY(), box->getURX(),
                        box->getURY());
            }
        }
    }

    if (polygons_id_ > 0) {
        PolygonTable *polygon_table = top_cell->getPolygonTable();
        VectorObject8 *polygon_index_vector = addr<VectorObject8>(polygons_id_);
        uint32_t num_polygons = polygon_index_vector->totalSize();
        for (int i = 0; i < num_polygons; i++) {
            ObjectIndex index = (*polygon_index_vector)[i];
            Polygon *polygon = polygon_table->getPolygonByIndex(index);
            fprintf(fp, "        POLYGON");
            for (int j = 0; j < polygon->getNumPoints(); ++j) {
                Point point = polygon->getPoint(j);
                fprintf(fp, " ( %d %d )", point.getX(), point.getY());
            }
            fprintf(fp, " ;\n");
        }
    }
}

uint64_t Constraint::getNumProperties() const {
    kSparsePair = kSparseMap.equal_range(IdType(getId(), kObjectTypeProperty));
    return std::distance(kSparsePair.first, kSparsePair.second);
}

void Constraint::addProperty(ObjectId obj_id) {
    if (obj_id == 0) return;

    kSparseMap.insert(
            std::make_pair(IdType(this->getId(), kObjectTypeProperty), obj_id));

    has_property_ = true;
}

bool Constraint::getHasProperty() const { return has_property_; }

void Constraint::printRegion(FILE *fp) {
    std::string name = getName();
    fprintf(fp, "- %s", name.c_str());
    if (boxes_id_ > 0) {
        VectorObject256 *box_vector = addr<VectorObject256>(boxes_id_);
        uint32_t num_boxes = box_vector->totalSize();
        for (int i = 0; i < num_boxes; ++i) {
            Box *box = addr<Box>((*box_vector)[i]);
            if (!box) {
                continue;
            }
            fprintf(fp, " ( %d %d ) ( %d %d )", box->getLLX(), box->getLLY(),
                    box->getURX(), box->getURY());
        }
    }
    fprintf(fp, "\n");
    if (getConstraintSubType() == ConstraintSubType::kRegionFence) {
        fprintf(fp, "  + TYPE FENCE");
    } else if (getConstraintSubType() == ConstraintSubType::kRegionGuide) {
        fprintf(fp, "  + TYPE GUIDE");
    }

    writeDEFProperty<Constraint>(reinterpret_cast<void *>(this), fp);

    fprintf(fp, " ;\n");
}
}  // namespace db
}  // namespace open_edi
