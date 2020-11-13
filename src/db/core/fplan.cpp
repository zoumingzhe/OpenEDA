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

#include "db/core/fplan.h"

#include "db/core/cell.h"
#include "db/core/db.h"
#include "db/util/symbol_table.h"
#include "db/util/array.h"
#include "util/util_mem.h"

namespace open_edi {
namespace db {
using IdArray = ArrayObject<ObjectId>;

/// @brief Row
Row::Row() {
    memset((void *)this, 0, sizeof(Row));
    row_name_index_ = 0;
    site_name_index_ = 0;
    bbox_ = Box();
    legal_orient_ = Orient::kN;
    orient_ = Orient::kN;
    site_count_ = 0;
}

/// @brief Row
///
/// @param r
Row::Row(Row &r) { copy(r); }

/// @brief Row
///
/// @param r
Row::Row(Row &&r) { move(std::move(r)); }

/// @brief ~Row
Row::~Row() {}

/// @brief getName
///
/// @return
std::string &Row::getName() {
    return getFloorplan()->getOwnerCell()->getSymbolByIndex(row_name_index_);
}

/// @brief setName
///
/// @param name
void Row::setName(SymbolIndex &row_name_index) {
    row_name_index_ = row_name_index;
    getFloorplan()->getOwnerCell()->addSymbolReference(row_name_index_,
                                                  this->getId());
}

/// @brief setName
///
/// @param name
///
/// @return
bool Row::setName(std::string &name) {
    int64_t index = getFloorplan()->getOwnerCell()->getOrCreateSymbol(name);
    if (index == -1) return false;

    row_name_index_ = index;
    getFloorplan()->getOwnerCell()->addSymbolReference(row_name_index_,
                                                  this->getId());
    return true;
}

/// @brief setName
///
/// @param name
///
/// @return
bool Row::setName(const char *name) {
    int64_t index = getFloorplan()->getOwnerCell()->getOrCreateSymbol(name);
    if (index == -1) return false;

    row_name_index_ = index;
    getFloorplan()->getOwnerCell()->addSymbolReference(row_name_index_,
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
void Row::setBox(Box &b) { bbox_ = b; }

/// @brief getLegalOrient
///
/// @return
Orient Row::getLegalOrient() const { return legal_orient_; }

/// @brief setLegalOrient
///
/// @param o
void Row::setLegalOrient(Orient &o) { legal_orient_ = o; }

/// @brief getOrient
///
/// @return
Orient Row::getOrient() const { return orient_; }

/// @brief setOrient
///
/// @param o
void Row::setOrient(Orient &o) { orient_ = o; }

void Row::setSiteId(ObjectId &site_id) { site_id_ = site_id; }

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
void Row::setSiteCount(int32_t &sc) { site_count_ = sc; }

/// @brief copy
///
/// @param r
void Row::copy(Row const &r) {
    this->BaseType::copy(r);
    bbox_ = r.bbox_;
    legal_orient_ = r.legal_orient_;
    orient_ = r.orient_;
    site_name_index_ = r.site_id_;
    site_count_ = r.site_count_;
}

/// @brief move
///
/// @param rhs
void Row::move(Row &&r) {
    this->BaseType::move(std::move(r));
    row_name_index_ = std::move(r.row_name_index_);
    bbox_ = std::move(r.bbox_);
    legal_orient_ = std::move(r.legal_orient_);
    orient_ = std::move(r.orient_);
    site_name_index_ = std::move(r.site_name_index_);
    site_count_ = std::move(r.site_count_);
}

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

    site_name_index_ = getFloorplan()->getOwnerCell()->getOrCreateSymbol(site_name);
    getFloorplan()->getOwnerCell()->addSymbolReference(site_name_index_,
                                                  this->getId());
}

std::string &Row::getSiteName() {
    return getFloorplan()->getOwnerCell()->getSymbolByIndex(site_name_index_);
}

void Row::setPropertySize(uint64_t v) {
    if (v == 0) {
        if (properties_id_) {
            __deleteObjectIdArray(properties_id_);
        }
        return;
    }
    if (!properties_id_) {
        properties_id_ = __createObjectIdArray(16);
    }
}

uint64_t Row::getNumProperties() const {
    if (!properties_id_) return 0;

    return addr<IdArray>(properties_id_)->getSize();
}

void Row::addProperty(ObjectId obj_id) {
    IdArray *id_array_ptr = nullptr;
    if (obj_id == 0) return;

    if (properties_id_ == 0) {
        properties_id_ = __createObjectIdArray(16);
    }
    ediAssert(properties_id_ != 0);
    id_array_ptr = addr<IdArray>(properties_id_);
    ediAssert(id_array_ptr != nullptr);
    id_array_ptr->pushBack(obj_id);
}

ObjectId Row::getPropertiesId() const { return properties_id_; }

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

    writeDEFProperty<Row>((void *)this, fp);

    fprintf(fp, " ;\n");
}

/// @brief Track default constructor
Track::Track() { memset((void *)this, 0, sizeof(Track)); }

/// @brief Track copy constructor
///
/// @param t
Track::Track(Track &t) { copy(t); }

/// @brief Track move constructor
///
/// @param rhs
Track::Track(Track &&rhs) { move(std::move(rhs)); }

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

void Track::addLayer(Int32 &layer_index) {
    ArrayObject<Int32> *id_array_ptr = nullptr;
    if (layers_ == 0) {
        id_array_ptr = getOwnerCell()->createObject<ArrayObject<Int32>>(kObjectTypeArray);
        if (id_array_ptr == nullptr) return;
        id_array_ptr->setPool(getOwnerCell()->getPool());
        id_array_ptr->reserve(256);        
        layers_ = id_array_ptr->getId();
    } else {
        id_array_ptr = addr< ArrayObject<Int32> >(layers_);
    }

    if (id_array_ptr) id_array_ptr->pushBack(layer_index);
}

void Track::addLayer(const char *layer_name) {
    Cell *top_cell = getOwnerCell();
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

    ArrayObject<Int32> *id_array_ptr = nullptr;
    if (layers_ == 0) {
        id_array_ptr = getOwnerCell()->createObject<ArrayObject<Int32>>(kObjectTypeArray);
        if (id_array_ptr == nullptr) return;
        id_array_ptr->setPool(getOwnerCell()->getPool());
        id_array_ptr->reserve(256);        
        layers_ = id_array_ptr->getId();
    } else {
        id_array_ptr = addr< ArrayObject<Int32> >(layers_);
    }

    if (id_array_ptr) id_array_ptr->pushBack(layer_index);
}

void Track::addLayer(std::string &layer_name) {
    Cell *top_cell = getOwnerCell();
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

    ArrayObject<Int32> *id_array_ptr = nullptr;
    if (layers_ == 0) {
        id_array_ptr = getOwnerCell()->createObject<ArrayObject<Int32>>(kObjectTypeArray);
        if (id_array_ptr == nullptr) return;
        id_array_ptr->setPool(getOwnerCell()->getPool());
        id_array_ptr->reserve(256);        
        layers_ = id_array_ptr->getId();
    } else {
        id_array_ptr = addr< ArrayObject<Int32> >(layers_);
    }

    if (id_array_ptr) id_array_ptr->pushBack(layer_index);
}

ObjectId Track::getLayers() const { return layers_; }

void Track::print() {
    Cell *top_cell = getOwnerCell();
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

    ArrayObject<Int32> *id_array_ptr = addr< ArrayObject<Int32> >(layers_);
    for (int i = 0; i < id_array_ptr->getSize(); ++i) {
        Int32 layer_index = (*id_array_ptr)[i];
        Layer *layer = tech_lib->getLayer(layer_index);
        if (0 == i) {
            message->info(" LAYER");
        }
        message->info(" %s", layer->getName());
    }
    message->info(" ;\n");
}

void Track::print(FILE *fp) {
    Cell *top_cell = getOwnerCell();
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
        ArrayObject<Int32> *id_array_ptr = addr< ArrayObject<Int32> >(layers_);
        for (int i = 0; i < id_array_ptr->getSize(); ++i) {
            Int32 layer_index = (*id_array_ptr)[i];
            Layer *layer = tech_lib->getLayer(layer_index);
            if (0 == i) {
                fprintf(fp, " LAYER");
            }
            fprintf(fp, " %s", layer->getName());
        }
    }
    fprintf(fp, " ;\n");
}

void Track::copy(Track const &t) {
    this->BaseType::copy(t);
    direction_x_ = t.direction_x_;
    start_ = t.start_;
    num_tracks_ = t.num_tracks_;
    space_ = t.space_;
    has_mask_ = t.has_mask_;
    mask_ = t.mask_;
    has_same_mask_ = t.has_same_mask_;
    layers_ = t.layers_;
}

/// @brief move
///
/// @param rhs
#if 0
void Track::move(Track &&rhs)
{
    this->BaseType::move(std::move(rhs));
    mask_ = std::move(rhs.mask_);
    layer_ = std::move(rhs.layer_);
    spacing_ = std::move(rhs.spacing_);
    width_ = std::move(rhs.width_);
    offset_ = std::move(rhs.offset_);
}
#endif

Grid::Grid() { memset((void *)this, 0, sizeof(Grid)); }

/// @brief Grid
///
/// @param t
Grid::Grid(Grid::GridType t) {
    memset((void *)this, 0, sizeof(Grid));
    grid_type_ = t;
}

/// @brief Grid
///
/// @param rhs
Grid::Grid(Grid &&rhs) { move(std::move(rhs)); }

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
    Cell *top_cell = getOwnerCell();
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
    Cell *top_cell = getOwnerCell();
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

void Grid::copy(Grid const &t) {
    this->BaseType::copy(t);
    direction_x_ = t.direction_x_;
    start_ = t.start_;
    number_ = t.number_;
    space_ = t.space_;
}

/// @brief move
///
/// @param rhs
#if 0
void Grid::move(Grid &&rhs)
{
    this->BaseType::move(std::move(rhs));
    mask_ = std::move(rhs.mask_);
    layer_ = std::move(rhs.layer_);
    spacing_ = std::move(rhs.spacing_);
}
#endif

uint64_t Floorplan::getNumOfRows() const {
    if (rows_ == 0) return 0;
    IdArray *obj_vector = addr<IdArray>(rows_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

Row *Floorplan::createRow() {
    Row *row = getOwnerCell()->createObject<Row>(kObjectTypeRow);
    row->setOwner(getOwnerCell());
    row->setFloorplan(this->getId());
    if (rows_ == 0) {
        rows_ = __createObjectIdArray(64);
    }
    IdArray *row_vector = addr<IdArray>(rows_);
    row_vector->pushBack(row->getId());
    return row;
}

ObjectId Floorplan::getRows() const { return rows_; }

uint64_t Floorplan::getNumOfTracks() const {
    if (tracks_ == 0) return 0;
    IdArray *obj_vector = addr<IdArray>(tracks_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

Track *Floorplan::createTrack() {
    Track *track = getOwnerCell()->createObject<Track>(kObjectTypeTrack);
    track->setOwner(getOwnerCell());
    track->setFloorplan(this->getId());
    if (tracks_ == 0) {
        tracks_ = __createObjectIdArray(64);
    }
    IdArray *track_vector = addr<IdArray>(tracks_);
    track_vector->pushBack(track->getId());
    return track;
}

ObjectId Floorplan::getTracks() const { return tracks_; }

uint64_t Floorplan::getNumOfGrids() const {
    if (gcell_grids_ == 0) return 0;
    IdArray *obj_vector = addr<IdArray>(gcell_grids_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

Grid *Floorplan::createGcellGrid() {
    Grid *grid = getOwnerCell()->createObject<Grid>(kObjectTypeGrid);
    grid->setOwner(getOwnerCell());
    grid->setGridType(Grid::kGridGcell);
    grid->setFloorplan(this->getId());
    if (gcell_grids_ == 0) {
        gcell_grids_ = __createObjectIdArray(64);
    }
    IdArray *grid_vector = addr<IdArray>(gcell_grids_);
    grid_vector->pushBack(grid->getId());
    return grid;
}

ObjectId Floorplan::getGcellGrids() const { return gcell_grids_; }

void Floorplan::setDieArea(ObjectIndex die_area) { die_area_ = die_area; }

ObjectIndex Floorplan::getDieArea() { return die_area_; }
Polygon *Floorplan::getDieAreaPolygon() {
    if (die_area_ == -1) {
        return nullptr;
    }
    
    if (!getOwnerCell()) {
        message->issueMsg(kError,
                          "Cannot get top cell when getting die area.\n");
        return nullptr;
    }
    if (!getOwnerCell()->getPolygonTable()) {
        message->issueMsg(kError,
                          "Cannot get polygon tale when getting die area.\n");
        return nullptr;
    }

    return getOwnerCell()->getPolygonTable()->getPolygonByIndex(die_area_);
}

// Floorplan class
/// @brief Floorplan
Floorplan::Floorplan() {
    memset((void *)this, 0, sizeof(Floorplan));
    die_area_ = -1;
}

/// @brief Floorplan
///
/// @param fp
Floorplan::Floorplan(Floorplan &fp) { copy(fp); }
/// @brief Floorplan
///
/// @param fp
Floorplan::Floorplan(Floorplan &&fp) { move(std::move(fp)); }
/// @brief ~Floorplan
Floorplan::~Floorplan() {}

/// @brief getCoreBox
///
/// @return
Box Floorplan::getCoreBox() const { return core_box_; }
/// @brief setCoreBox
///
/// @param box
void Floorplan::setCoreBox(Box &box) { core_box_ = box; }

/// @brief getXOffset
///
/// @return
int32_t Floorplan::getXOffset() const { return x_offset_; }
/// @brief setXOffset
///
/// @param offset
void Floorplan::setXOffset(int32_t &offset) { x_offset_ = offset; }

/// @brief getYOffset
///
/// @return
int32_t Floorplan::getYOffset() const { return y_offset_; }
/// @brief setYOffset
///
/// @param offset
void Floorplan::setYOffset(int32_t &offset) { y_offset_ = offset; }

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
void Floorplan::setCoreSiteId(ObjectId &id) { core_site_id_ = id; }

/// @brief copy
///
/// @param fp
void Floorplan::copy(Floorplan const &fp) {
    this->BaseType::copy(fp);
    core_box_ = fp.core_box_;
    x_offset_ = fp.x_offset_;
    y_offset_ = fp.y_offset_;
    core_site_id_ = fp.core_site_id_;
    // TODO: copy tracks & rows
}

/// @brief move
///
/// @param fp
void Floorplan::move(Floorplan &&fp) {
    this->BaseType::move(std::move(fp));
    core_box_ = std::move(fp.core_box_);
    x_offset_ = std::move(fp.x_offset_);
    y_offset_ = std::move(fp.y_offset_);
    core_site_id_ = std::move(fp.core_site_id_);
    // TODO: move tracks & rows
}

Constraint *Floorplan::createPlaceBlockage() {
    IdArray *id_array_ptr = nullptr;
    Constraint *cons = nullptr;

    if (place_blockages_ == 0) {
        place_blockages_ = __createObjectIdArray(32);
    }
    ediAssert(place_blockages_ != 0);
    id_array_ptr = addr<IdArray>(place_blockages_);
    if (id_array_ptr == nullptr) return nullptr;

    cons = getOwnerCell()->createObject<Constraint>(kObjectTypePhysicalConstraint);
    if (cons == nullptr) {
        getOwnerCell()->deleteObject<Constraint>(cons);
        return nullptr;
    }
    cons->setFloorplan(this->getId());
    cons->setConstraintType(Constraint::kConstraintPBlkg);
    id_array_ptr->pushBack(cons->getId());

    return cons;
}

Constraint *Floorplan::createRegion(const char *name) {
    IdArray *id_array_ptr = nullptr;
    Constraint *region = nullptr;

    if (regions_ == 0) {
        regions_ = __createObjectIdArray(32);
    }
    ediAssert(regions_ != 0);
    id_array_ptr = addr<IdArray>(regions_);
    if (id_array_ptr == nullptr) return nullptr;

    region = getOwnerCell()->createObject<Constraint>(kObjectTypeRegion);
    if (region == nullptr) {
        getOwnerCell()->deleteObject<Constraint>(region);
        return nullptr;
    }
    region->setFloorplan(this->getId());
    region->setName(name);
    region->setConstraintType(Constraint::kConstraintRegion);
    id_array_ptr->pushBack(region->getId());

    return region;
}
uint64_t Floorplan::getNumOfRegions() const {
    if (regions_ == 0) return 0;
    IdArray *obj_vector = addr<IdArray>(regions_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}
ObjectId Floorplan::getRegions() const { return regions_; }
Constraint *Floorplan::getRegion(std::string &name) const {
    if (regions_ == 0) return nullptr;
    Cell *top_cell = getOwnerCell();
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
    IdArray *obj_vector = addr<IdArray>(place_blockages_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}
ObjectId Floorplan::getPlaceBlockages() const { return place_blockages_; }

Constraint *Floorplan::createRouteBlockage() {
    IdArray *id_array_ptr = nullptr;
    Constraint *cons = nullptr;

    if (route_blockages_ == 0) {
        route_blockages_ = __createObjectIdArray(32);
    }
    ediAssert(route_blockages_ != 0);
    id_array_ptr = addr<IdArray>(route_blockages_);
    if (id_array_ptr == nullptr) return nullptr;

    cons = getOwnerCell()->createObject<Constraint>(kObjectTypePhysicalConstraint);
    if (cons == nullptr) {
        getOwnerCell()->deleteObject<Constraint>(cons);
        return nullptr;
    }
    cons->setFloorplan(this->getId());
    cons->setConstraintType(Constraint::kConstraintRBlkg);
    id_array_ptr->pushBack(cons->getId());

    return cons;
}
uint64_t Floorplan::getNumOfRouteBlockages() const {
    if (route_blockages_ == 0) return 0;
    IdArray *obj_vector = addr<IdArray>(route_blockages_);
    if (obj_vector == nullptr) return 0;
    return obj_vector->getSize();
}

ObjectId Floorplan::getRouteBlockages() const { return route_blockages_; }
// Constraint class
//
using ConstraintType = Constraint::ConstraintType;
using ConstraintSubType = Constraint::ConstraintSubType;

Constraint::Constraint() { type_ = kConstraintNone; }

Constraint::Constraint(const char *name, ConstraintType t /*, Shape &s*/) {
    // name_ = name; TODO: use symtable to gen id
    name_ = getFloorplan()->getOwnerCell()->getOrCreateSymbol(name);
    type_ = t;
}

Constraint::Constraint(SymbolIndex &name, ConstraintType t /*, Shape &s*/) {
    name_ = name;
    type_ = t;
}

/* TODO
Constraint::Constraint(Shape &s)
{
}
*/
Constraint::Constraint(Constraint::ConstraintType &t) { type_ = t; }

Constraint::~Constraint() {
    type_ = kConstraintNone;
    setPropertySize(0);
}

SymbolIndex Constraint::getNameIndex() { return name_; }

std::string &Constraint::getName() {
    return getFloorplan()->getOwnerCell()->getSymbolByIndex(name_);
}

void Constraint::setName(SymbolIndex &name) {
    name_ = name;
    getFloorplan()->getOwnerCell()->addSymbolReference(name_, this->getId());
}

bool Constraint::setName(std::string &name) {
    int64_t index = getFloorplan()->getOwnerCell()->getOrCreateSymbol(name);
    if (index == -1) return false;

    name_ = index;
    getFloorplan()->getOwnerCell()->addSymbolReference(name_, this->getId());
    return true;
}

bool Constraint::setName(const char *name) {
    int64_t index = getFloorplan()->getOwnerCell()->getOrCreateSymbol(name);
    if (index == -1) return false;

    name_ = index;
    getFloorplan()->getOwnerCell()->addSymbolReference(name_, this->getId());
    return true;
}

Constraint::ConstraintType Constraint::getConstraintType() { return type_; }

void Constraint::setConstraintType(ConstraintType t) { type_ = t; }

ConstraintSubType Constraint::getConstraintSubType() { return sub_type_; }

void Constraint::setConstraintSubType(ConstraintSubType st) { sub_type_ = st; }

int32_t Constraint::getUtilization() { return utilization_; }

void Constraint::setUtilization(int32_t &u) { utilization_ = u; }

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
    Cell *top_cell = getOwnerCell();
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
    Cell *top_cell = getOwnerCell();
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
    Cell *top_cell = getOwnerCell();
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
        boxes_id_ = __createObjectIdArray(256);
    }
    IdArray *box_vector = addr<IdArray>(boxes_id_);
    box_vector->pushBack(box->getId());
    return box;
}
ObjectId Constraint::getBoxesId() const { return boxes_id_; }

/// @brief addPolygon
///
/// @param polygon:
///
/// @return
void Constraint::addPolygon(Polygon *polygon) {
    Cell *top_cell = getOwnerCell();
    PolygonTable *polygon_table = top_cell->getPolygonTable();
    if (polygons_id_ == 0) {
        polygons_id_ = __createObjectIdArray(8);
    }
    IdArray *polygon_vector = addr<IdArray>(polygons_id_);
    ObjectIndex polygon_index = polygon_table->addPolygon(polygon);
    polygon_vector->pushBack(polygon_index);
    return;
}

ObjectId Constraint::getPolygonsId() const { return polygons_id_; }

void Constraint::printBlockage() const {
    Cell *top_cell = getOwnerCell();
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
        IdArray *box_vector = addr<IdArray>(boxes_id_);
        uint32_t num_boxes = box_vector->getSize();
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
        IdArray *polygon_index_vector = addr<IdArray>(polygons_id_);
        uint32_t num_polygons = polygon_index_vector->getSize();
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
    Cell *top_cell = getOwnerCell();
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
        IdArray *box_vector = addr<IdArray>(boxes_id_);
        uint32_t num_boxes = box_vector->getSize();
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
        IdArray *polygon_index_vector = addr<IdArray>(polygons_id_);
        uint32_t num_polygons = polygon_index_vector->getSize();
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

void Constraint::setPropertySize(uint64_t v) {
    if (v == 0) {
        if (properties_id_) {
            __deleteObjectIdArray(properties_id_);
        }
        return;
    }
    if (!properties_id_) {
        properties_id_ = __createObjectIdArray(16);
    }
}

uint64_t Constraint::getNumProperties() const {
    if (!properties_id_) return 0;

    return addr<IdArray>(properties_id_)->getSize();
}

void Constraint::addProperty(ObjectId obj_id) {
    IdArray *id_array_ptr = nullptr;
    if (obj_id == 0) return;

    if (properties_id_ == 0) {
        properties_id_ = __createObjectIdArray(16);
    }
    ediAssert(properties_id_ != 0);
    id_array_ptr = addr<IdArray>(properties_id_);
    ediAssert(id_array_ptr != nullptr);
    id_array_ptr->pushBack(obj_id);
}

ObjectId Constraint::getPropertiesId() const { return properties_id_; }

void Constraint::printRegion(FILE *fp) {
    std::string name = getName();
    fprintf(fp, "- %s", name.c_str());
    if (boxes_id_ > 0) {
        IdArray *box_vector = addr<IdArray>(boxes_id_);
        uint32_t num_boxes = box_vector->getSize();
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

    writeDEFProperty<Constraint>((void *)this, fp);

    fprintf(fp, " ;\n");
}
}  // namespace db
}  // namespace open_edi
