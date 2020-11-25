/* @file  inst.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
/**
 * @file   inst.cpp
 * @date   Apr 2020
 */

#include "db/core/inst.h"

#include "db/core/cell.h"
#include "db/core/db.h"
#include "db/core/pin.h"
#include "db/util/array.h"
#include "db/util/vector_object_var.h"

namespace open_edi {
namespace db {

Inst::Inst() {
    has_eeq_master_ = false;
    has_source_ = false;
    has_mask_shift_ = false;
    has_halo_ = false;
    has_soft_ = false;
    has_route_halo_ = false;
    has_weight_ = false;
    has_region_ = false;
    has_property_ = false;
    name_index_ = 0;
    master_ = 0;
    pg_pins_ = 0;
    pins_ = 0;
    status_ = PlaceStatus::kUnknown;
    location_ = Point(0, 0);
    orient_ = Orient::kUnknown;
    eeq_master_ = 0;
    source_ = kNetlist;
    for (int i = 0; i < max_layer_num; i++) {
        mask_shift_[i] = 0;
    }
    halo_ = Box(0, 0, 0, 0);
    route_halo_dist_ = 0;
    min_layer_id_ = -1;
    max_layer_id_ = -1;
    weight_ = 0;
    region_ = 0;
}

Inst::Inst(Inst const &rhs) { copy(rhs); }

Inst &Inst::operator=(Inst const &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

bool Inst::getHasEeqMaster() const { return has_eeq_master_; }

void Inst::setHasEeqMaster(bool flag) { has_eeq_master_ = flag; }

bool Inst::getHasSource() const { return has_source_; }

void Inst::setHasSource(bool flag) { has_source_ = flag; }

bool Inst::getHasMaskShift() const { return has_mask_shift_; }

void Inst::setHasMaskShift(bool flag) { has_mask_shift_ = flag; }

bool Inst::getHasHalo() const { return has_halo_; }

void Inst::setHasHalo(bool flag) { has_halo_ = flag; }

bool Inst::getHasSoft() const { return has_soft_; }

void Inst::setHasSoft(bool flag) { has_soft_ = flag; }

bool Inst::getHasRouteHalo() const { return has_route_halo_; }

void Inst::setHasRouteHalo(bool flag) { has_route_halo_ = flag; }

bool Inst::getHasWeight() const { return has_weight_; }

void Inst::setHasWeight(bool flag) { has_weight_ = flag; }

bool Inst::getHasRegion() const { return has_region_; }

void Inst::setHasRegion(bool flag) { has_region_ = flag; }

bool Inst::getHasProperty() const { return has_property_; }

void Inst::setHasProperty(bool flag) { has_property_ = flag; }

Box Inst::getBox() {
    Cell *cell = getMaster();
    int llx = 0, lly = 0, urx = 0, ury = 0;
    int origin_x = 0, origin_y = 0, size_x = 0, size_y = 0;
    if (cell && cell->hasOrigin()) {
        origin_x = cell->getOriginX();
        origin_y = cell->getOriginY();
    }

    if (cell && cell->hasSize()) {
        size_x = cell->getSizeX();
        size_y = cell->getSizeY();
    }

    size_x += origin_x;
    size_y += origin_y;

    if (getOrient() == Orient::kN) {
        llx = getLocation().getX();
        lly = getLocation().getY();
        urx = llx + size_x;
        ury = lly + size_y;
    }

    if (getOrient() == Orient::kS) {
        llx = getLocation().getX() - size_x;
        lly = getLocation().getY() - size_y;
        urx = getLocation().getX();
        ury = getLocation().getY();
    }

    if (getOrient() == Orient::kW) {
        llx = getLocation().getX() - size_y;
        lly = getLocation().getY();
        urx = getLocation().getX();
        ury = getLocation().getY() + size_x;
    }

    if (getOrient() == Orient::kE) {
        llx = getLocation().getX();
        lly = getLocation().getY() - size_x;
        urx = getLocation().getX() + size_y;
        ury = getLocation().getY();
    }

    if (getOrient() == Orient::kFN) {
        llx = getLocation().getX() - size_x;
        lly = getLocation().getY();
        urx = getLocation().getX();
        ury = getLocation().getY() + size_y;
    }

    if (getOrient() == Orient::kFS) {
        llx = getLocation().getX();
        lly = getLocation().getY() - size_y;
        urx = getLocation().getX() + size_x;
        ury = getLocation().getY();
    }

    if (getOrient() == Orient::kFW) {
        llx = getLocation().getX();
        lly = getLocation().getY();
        urx = getLocation().getX() + size_y;
        ury = getLocation().getY() + size_x;
    }

    if (getOrient() == Orient::kFS) {
        llx = getLocation().getX() - size_y;
        lly = getLocation().getY() - size_x;
        urx = getLocation().getX();
        ury = getLocation().getY();
    }

    Box bbox(llx, lly, urx, ury);

    return bbox;
}

void Inst::setName(std::string name) {
    name_index_ = getOwnerCell()->getOrCreateSymbol(name);
    getOwnerCell()->addSymbolReference(name_index_, this->getId());
}

std::string Inst::getName() const {
    return getOwnerCell()->getSymbolTable()->getSymbolByIndex(name_index_);
}

Cell *Inst::getMaster() const { return addr<Cell>(master_); }

void Inst::setMaster(ObjectId master) { master_ = master; }
void Inst::setMaster(const std::string name) {
    Cell *master = getOwnerCell()->getCell(name);
    if (master) {
        master_ = master->getId();
    } else {
        message->issueMsg(kError, "cannot find cell %s for instance %s\n",
                          name.c_str(), getName().c_str());
    }
}

UInt32 Inst::numPGPins() const {
    if (pg_pins_ == 0) {
        return 0;
    }
    ArrayObject<ObjectId> *pins_vector = addr<ArrayObject<ObjectId>>(pg_pins_);
    if (pins_vector == nullptr) {
        return 0;
    }
    return pins_vector->getSize();
}

void Inst::setPGPins(ObjectId pg_pins) { pg_pins_ = pg_pins; }

ObjectId Inst::getPGPins() const { return pg_pins_; }

Pin *Inst::getPGPin(const std::string &name) const {
    if (pg_pins_ == 0) {
        return nullptr;
    }
    ArrayObject<ObjectId> *pins_vector = addr<ArrayObject<ObjectId>>(pg_pins_);
    if (pins_vector == nullptr) {
        return nullptr;
    }
    for (int i = 0; i < pins_vector->getSize(); i++) {
        ObjectId pin_id = (*pins_vector)[i];
        Pin *pin = addr<Pin>(pin_id);
        if (pin && pin->getName() == name) {
            return pin;
        }
    }
    return nullptr;
}

ObjectId Inst::__createPinArray() {
    ArrayObject<ObjectId> *vobj =
        getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
    ediAssert(vobj != nullptr);
    vobj->setPool(getOwnerCell()->getPool());
    vobj->reserve(8);
    return (vobj->getId());
}

ObjectId Inst::__createPropertyArray() {
    ArrayObject<ObjectId> *vobj =
        getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
    ediAssert(vobj != nullptr);
    vobj->setPool(getOwnerCell()->getPool());
    vobj->reserve(16);
    return (vobj->getId());
}

void Inst::addPGPin(Pin *pin) {
    if (pg_pins_ == 0) {
        pg_pins_ = __createPinArray();
    }
    ArrayObject<ObjectId> *pin_vector = addr<ArrayObject<ObjectId>>(pg_pins_);
    pin_vector->pushBack(pin->getId());
}

UInt32 Inst::numPins() const {
    if (pins_ == 0) {
        return 0;
    }
    ArrayObject<ObjectId> *pins_vector = addr<ArrayObject<ObjectId>>(pins_);
    if (pins_vector == nullptr) {
        return 0;
    }
    return pins_vector->getSize();
}

Pin *Inst::getPin(const std::string &name) const {
    if (pins_ == 0) {
        return nullptr;
    }
    ArrayObject<ObjectId> *pins_vector = addr<ArrayObject<ObjectId>>(pins_);
    if (pins_vector == nullptr) {
        return nullptr;
    }
    for (int i = 0; i < pins_vector->getSize(); i++) {
        ObjectId pin_id = (*pins_vector)[i];
        Pin *pin = addr<Pin>(pin_id);
        if (pin && pin->getName() == name) {
            return pin;
        }
    }
    return nullptr;
}

ObjectId Inst::getPins() const { return pins_; }

Pin *Inst::getPinById(ObjectId obj_id) const {
    if (obj_id == 0) {
        return nullptr;
    }
    return addr<Pin>(obj_id);
}

void Inst::addPin(Pin *pin) {
    if (pins_ == 0) {
        pins_ = __createPinArray();
    }
    ArrayObject<ObjectId> *pin_vector = addr<ArrayObject<ObjectId>>(pins_);
    pin_vector->pushBack(pin->getId());
}

void Inst::createPins() {
    Cell *master = getMaster();
    if (!master) {
        return;
    }
    for (int i = 0; i < master->getNumOfTerms(); i++) {
        Term *term = master->getTerm(i);
        createInstancePin(term->getName());  // not handle return value, pin net
                                             // is attached in net parser
    }
}

Pin *Inst::createInstancePin(std::string &pin_name) {
    Cell *master = getMaster();
    if (!master) {
        return nullptr;
    }
    Term *term = master->getTerm(pin_name);
    if (!term) {
        return nullptr;
    }
    Pin *pin = getOwnerCell()->createObject<Pin>(kObjectTypePin);
    pin->setName(pin_name);
    pin->setInst(this);
    pin->setTerm(term);
    ArrayObject<ObjectId> *pin_vector = nullptr;
    if (term->isPGType()) {
        if (pg_pins_ == 0) {
            pg_pins_ = __createPinArray();
        }
        pin_vector = addr<ArrayObject<ObjectId>>(pg_pins_);
    } else {
        if (pins_ == 0) {
            pins_ = __createPinArray();
        }
        pin_vector = addr<ArrayObject<ObjectId>>(pins_);
    }
    pin_vector->pushBack(pin->getId());
    return pin;
}
Pin *Inst::createInstancePinWithoutMaster(std::string &pin_name) {
    Pin *pin = getOwnerCell()->createObject<Pin>(kObjectTypePin);
    pin->setName(pin_name);
    pin->setInst(this);
    ArrayObject<ObjectId> *pin_vector = nullptr;
    if (pins_ == 0) {
        pins_ = __createPinArray();
    }
    pin_vector = Object::addr<ArrayObject<ObjectId>>(pins_);
    pin_vector->pushBack(pin->getId());
    return pin;
}

PlaceStatus Inst::getStatus() const { return status_; }

void Inst::setStatus(const PlaceStatus &s) { status_ = s; }

Point Inst::getLocation() const { return location_; }

void Inst::setLocation(const Point &l) { location_ = l; }

Orient Inst::getOrient() const { return orient_; }

void Inst::setOrient(const Orient &o) { orient_ = o; }

SourceType Inst::getSource() const { return source_; }

std::string Inst::getSourceString() const {
    std::string source_string;
    switch (source_) {
        case kNetlist:
            source_string = "NETLIST";
            break;
        case kDist:
            source_string = "DIST";
            break;
        case kUser:
            source_string = "USER";
            break;
        case kTiming:
            source_string = "TIMING";
            break;
        default:
            break;
    }
    return source_string;
}

void Inst::setSource(const SourceType &s) { source_ = s; }

Cell *Inst::getEeqMaster() const { return addr<Cell>(eeq_master_); }

void Inst::setEeqMaster(const std::string &name) {
    Cell *cell = getOwnerCell()->getCell(name);
    if (cell) {
        eeq_master_ = cell->getId();
    }
}

UInt32 Inst::getMaskShift(Int32 layer_id) const {
    return mask_shift_[layer_id];
}

void Inst::setMaskShift(UInt32 m, Int32 layer_id) { mask_shift_[layer_id] = m; }

Box Inst::getHalo() const { return halo_; }

void Inst::setHalo(const Box &halo) { halo_ = halo; }

UInt32 Inst::getRouteHaloDist() const { return route_halo_dist_; }

void Inst::setRouteHaloDist(const UInt32 &d) { route_halo_dist_ = d; }

Int32 Inst::getMinLayerId() const { return min_layer_id_; }

void Inst::setMinLayer(std::string name) {
    min_layer_id_ =
        getOwnerCell()->getTechLib()->getLayerLEFIndexByName(name.c_str());
}

Int32 Inst::getMaxLayerId() const { return max_layer_id_; }

void Inst::setMaxLayer(std::string name) {
    max_layer_id_ =
        getOwnerCell()->getTechLib()->getLayerLEFIndexByName(name.c_str());
}

Int32 Inst::getWeight() const { return weight_; }

void Inst::setWeight(const Int32 &w) { weight_ = w; }

Constraint *Inst::getRegion() const { return addr<Constraint>(region_); }

void Inst::setRegion(std::string &name) {
    Constraint *region = getOwnerCell()->getFloorplan()->getRegion(name);
    if (region) {
        region_ = region->getId();
    }
}

void Inst::copy(Inst const &rhs) {
    has_eeq_master_ = rhs.has_eeq_master_;
    has_source_ = rhs.has_source_;
    has_mask_shift_ = rhs.has_mask_shift_;
    has_halo_ = rhs.has_halo_;
    has_soft_ = rhs.has_soft_;
    has_route_halo_ = rhs.has_route_halo_;
    has_weight_ = rhs.has_weight_;
    has_region_ = rhs.has_region_;
    has_property_ = rhs.has_property_;
    name_index_ = rhs.name_index_;
    master_ = rhs.master_;
    if (rhs.pg_pins_ == 0) {
        pg_pins_ = 0;
    }
    ArrayObject<ObjectId> *rhs_pg_pins_vector =
        addr<ArrayObject<ObjectId>>(rhs.pg_pins_);
    if (rhs_pg_pins_vector == nullptr) {
        pg_pins_ = 0;
    }
    pg_pins_ = __createPinArray();
    ArrayObject<ObjectId> *new_pg_pin_vector =
        addr<ArrayObject<ObjectId>>(pg_pins_);
    for (int i = 0; i < rhs_pg_pins_vector->getSize(); i++) {
        ObjectId pin_id = (*rhs_pg_pins_vector)[i];
        Pin *pin = addr<Pin>(pin_id);
        new_pg_pin_vector->pushBack(pin->getId());
    }
    if (rhs.pins_ == 0) {
        pins_ = 0;
    }
    ArrayObject<ObjectId> *rhs_pins_vector =
        addr<ArrayObject<ObjectId>>(rhs.pins_);
    if (rhs_pins_vector == nullptr) {
        pins_ = 0;
    }
    pins_ = __createPinArray();
    ArrayObject<ObjectId> *new_pin_vector = addr<ArrayObject<ObjectId>>(pins_);
    for (int i = 0; i < rhs_pins_vector->getSize(); i++) {
        ObjectId pin_id = (*rhs_pins_vector)[i];
        Pin *pin = addr<Pin>(pin_id);
        new_pin_vector->pushBack(pin->getId());
    }
    location_ = rhs.location_;
    orient_ = rhs.orient_;
    eeq_master_ = rhs.eeq_master_;
    source_ = rhs.source_;
    for (int i = 0; i < max_layer_num; i++) {
        mask_shift_[i] = rhs.getMaskShift(i);
    }
    halo_ = rhs.halo_;
    route_halo_dist_ = rhs.route_halo_dist_;
    min_layer_id_ = rhs.min_layer_id_;
    max_layer_id_ = rhs.max_layer_id_;
    weight_ = rhs.weight_;
    region_ = rhs.region_;
}

void Inst::move(Inst &&rhs) {
    // TODO.
}

void Inst::setPropertySize(uint64_t v) {
    if (v == 0) {
        if (properties_id_) {
            ArrayObject<ObjectId> *data =
                addr<ArrayObject<ObjectId>>(properties_id_);
            getOwnerCell()->deleteObject<ArrayObject<ObjectId>>(data);
        }
        return;
    }
    if (!properties_id_) {
        properties_id_ = __createPropertyArray();
    }
}

uint64_t Inst::getNumProperties() const {
    if (!properties_id_) return 0;

    return addr<ArrayObject<ObjectId>>(properties_id_)->getSize();
}

void Inst::addProperty(ObjectId obj_id) {
    ArrayObject<ObjectId> *vobj = nullptr;
    if (obj_id == 0) return;

    if (properties_id_ == 0) {
        properties_id_ = __createPropertyArray();
    } else {
        vobj = addr<ArrayObject<ObjectId>>(properties_id_);
    }
    ediAssert(vobj != nullptr);
    vobj->pushBack(obj_id);
}

ObjectId Inst::getPropertiesId() const { return properties_id_; }

ArrayObject<ObjectId> *Inst::getPinArray() const {
    if (pins_ != 0) {
        ArrayObject<ObjectId> *pin_array = addr<ArrayObject<ObjectId>>(pins_);
        return pin_array;
    } else {
        return nullptr;
    }
}
ArrayObject<ObjectId> *Inst::getPGPinArray() const {
        if (pg_pins_ != 0) {
        ArrayObject<ObjectId>* pg_pin_array = addr<ArrayObject<ObjectId>>(pg_pins_);
        return pg_pin_array;
    } else {
        return nullptr;
    }
}

void Inst::clear() {
    // TODO.
}

void Inst::print(FILE *fp) {
    Cell *cell = getMaster();
    if (!cell) {
        message->issueMsg(kError, "the cell of instatnce %s is null.\n",
                          getName().c_str());
        return;
    }
    fprintf(fp, "- %s %s", getName().c_str(), cell->getName().c_str());
    if (getHasEeqMaster()) {
        Cell *eeq_cell = getEeqMaster();
        if (!eeq_cell) {
            message->issueMsg(kError,
                              "the eeq master cell of instatnce %s is null.\n",
                              getName().c_str());
            return;
        }
        fprintf(fp, "\n  + EEQMASTER %s", eeq_cell->getName().c_str());
    }
    if (getHasSource()) {
        fprintf(fp, "\n  + SOURCE %s", getSourceString().c_str());
    }
    if ((getStatus() == PlaceStatus::kFixed) ||
        (getStatus() == PlaceStatus::kCover) ||
        (getStatus() == PlaceStatus::kPlaced)) {
        std::string status_string = toString(getStatus());
        toUpper(status_string);
        fprintf(fp, "\n  + %s", status_string.c_str());

        Point point = getLocation();
        Orient orient = getOrient();
        fprintf(fp, " ( %d %d ) %s", point.getX(), point.getY(),
                toString(orient).c_str());
    }
    if (getStatus() == PlaceStatus::kUnplaced) {
        fprintf(fp, "\n  + UNPLACED");
    }

    if (getHasMaskShift()) {
        fprintf(fp, "\n  + MASKSHIFT ");
        for (int i = 0; i < getOwnerCell()->getNumMaskShiftLayers(); ++i) {
            fprintf(fp, "%x", getMaskShift(i));
        }
    }

    if (getHasHalo()) {
        fprintf(fp, "\n  + HALO");
        if (getHasSoft()) {
            fprintf(fp, " SOFT");
            Box box = getHalo();
            fprintf(fp, " %d %d %d %d", box.getLLX(), box.getLLY(),
                    box.getURX(), box.getURY());
        }
    }

    if (getHasRouteHalo()) {
        Layer *min_layer = cell->getLayerByLayerId(getMinLayerId());
        Layer *max_layer = cell->getLayerByLayerId(getMaxLayerId());
        fprintf(fp, "\n  + ROUTEHALO %d %s %s", getRouteHaloDist(),
                min_layer->getName(), max_layer->getName());
    }

    if (getHasWeight()) {
        fprintf(fp, "\n  + WEIGHT %d", getWeight());
    }

    if (getHasRegion()) {
        fprintf(fp, "\n  + REGION %s", getRegion()->getName().c_str());
    }

    if (getNumProperties() > 0) {
        ArrayObject<ObjectId> *vobj =
            addr<ArrayObject<ObjectId>>(properties_id_);
        for (int i = 0; i < getNumProperties(); i++) {
            ObjectId obj_id = (*vobj)[i];
            if (!obj_id) continue;
            Property *property = addr<Property>(obj_id);
            if (property == nullptr) continue;
            property->printDEF(fp);
        }
    }

    fprintf(fp, " ;\n");
}

OStreamBase &operator<<(OStreamBase &os, Inst const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    os << DataFieldName("cell_");
    if (rhs.owner_) {
        os << rhs.owner_;
    } else {
        os << UNINIT_OBJECT_ID;
    }
    os << DataDelimiter();

    os << rhs.numPins();
    os << DataBegin("[");
    auto delimiter = DataDelimiter("");
    os << DataEnd("]") << DataDelimiter();

    os << DataEnd(")");
    return os;
}

IStreamBase &operator>>(IStreamBase &is, Inst &rhs) {
    is >> DataTypeName(className(rhs)) >> DataBegin("(");

    auto context = is.getContext();

    // add current context to Inst
    is.addContext(&rhs);

    // read cell_
    auto &db = is.getDb();
    InstAttr::IndexType cell_id;
    is >> DataFieldName("cell_") >> cell_id >> DataDelimiter();
    if (cell_id == 0) {
        rhs.owner_ = 0;
    } else {
        /* remove-database
        rhs.cell_ = db.getCell(cell_id);
        */
    }

    Inst::IndexType size;
    is >> size;
    is >> DataBegin("[");
    auto delimiter = DataDelimiter("");
    is >> DataEnd("]") >> DataDelimiter();
    // pop current context
    is.popContext();

    is >> DataEnd(")");
    return is;
}

}  // namespace db
}  // namespace open_edi
