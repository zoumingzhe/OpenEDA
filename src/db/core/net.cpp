/* @file  net.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/core/net.h"

#include <algorithm>

#include "db/core/cell.h"
#include "db/core/db.h"
#include "db/core/pin.h"
#include "db/util/array.h"
#include "db/util/vector_object_var.h"

namespace open_edi {
namespace db {
using IdArray = ArrayObject<ObjectId>;

// TODO: replace by sparce property
ObjectId vpins = 0;
ObjectId properties = 0;
std::map<ObjectId, ObjectId> Vpin_map;
std::map<ObjectId, ObjectId> property_map;
std::map<ObjectId, ObjectId> patch_map;

/**
 * @brief Construct a new VPin::VPin object
 *
 */
VPin::VPin() {
    name_index_ = 0;
    has_layer_ = false;
    layer_ = "";
}

/**
 * @brief Get the Name Index object
 *
 * @return SymbolIndex
 */
SymbolIndex VPin::getNameIndex() const { return name_index_; }

/**
 * @brief Get the Name object
 *
 * @return std::string const&
 */
std::string const& VPin::getName() {
    return getOwnerCell()->getSymbolByIndex(name_index_);
}

/**
 * @brief Set the Name object
 *
 * @param name
 */
bool VPin::setName(std::string const& name) {
    SymbolIndex index = getOwnerCell()->getOrCreateSymbol(name.c_str());
    if (index == kInvalidSymbolIndex) return false;

    name_index_ = index;
    getOwnerCell()->addSymbolReference(name_index_, this->getId());
    return true;
}

/**
 * @brief Set the Layer object
 *
 * @param name
 */
void VPin::setLayer(char* name) { layer_ = name; }
/**
 * @brief Set the flag of Layer object
 *
 * @param has layer flag
 */
void VPin::setHasLayer(bool has_layer) { has_layer_ = has_layer; }
/**
 * @brief Get the flag of Layer
 *
 * @return bool
 */
bool VPin::getHasLayer() const { return has_layer_; }

/**
 * @brief Get the Layer object
 *
 * @return const char*
 */
const char* VPin::getLayer() const { return layer_.c_str(); }

/**
 * @brief Set the Status object
 *
 * @param status
 */
void VPin::setStatus(Bits status) { status_ = status; }

/**
 * @brief Get the Status object
 *
 * @return Bits
 */
Bits VPin::getStatus() { return status_; }

/**
 * @brief Set the Orientation object
 *
 * @param orient
 */
void VPin::setOrientation(Bits orient) { orient_ = orient; }

/**
 * @brief Get the Orientation object
 *
 * @return Bits
 */
Bits VPin::getOrientation() { return orient_; }

/**
 * @brief Set the Box object
 *
 * @param box
 */
void VPin::setBox(Box box) { bbox_ = box; }

/**
 * @brief Get the Box object
 *
 * @return Box
 */
Box VPin::getBox() const { return bbox_; }

/**
 * @brief Set the Loc object
 *
 * @param loc
 */
void VPin::setLoc(Point loc) { loc_ = loc; }

/**
 * @brief Get the Loc object
 *
 * @return Point
 */
Point VPin::getLoc() const { return loc_; }

/**
 * @brief Construct a new Net:: Net object
 *
 */
Net::Net() {
    name_index_ = 0;
    is_bus_net_ = false;
    is_of_bus_ = false;
    is_from_term_ = false;
    is_sub_net_ = 0;
    must_jion_ = 0;
    net_type_ = 0;
    status_ = 0;
    pattern_ = 0;
    rule_ = 0;
    xtalk_ = 0;
    frequency_ = 0;
    cap_ = 0;
    weight_ = 0;
    // origin_net_ = "";
    assign_net_ = 0;
    assign_type_ = kAssignTypeUnknown;
    vpins = 0;
    properties = 0;
}

/**
 * @brief Get the Fix Bump object
 *
 * @return Bits
 */
Bits Net::getFixBump() const { return fix_bump_; }

/**
 * @brief Get the Must Join object
 *
 * @return Bits
 */
Bits Net::getMustJoin() const { return must_jion_; }

/**
 * @brief Get the Pattern object
 *
 * @return Bits
 */
Bits Net::getPattern() const { return pattern_; }

/**
 * @brief Get the Is Sub Net object
 *
 * @return Bits
 */
Bits Net::getIsSubNet() const { return is_sub_net_; }

/**
 * @brief is analog net
 *
 * @return true
 * @return false
 */
bool Net::isAnalog() { return getType() == kNetTypeAnalog; }

/**
 * @brief is clock net
 *
 * @return true
 * @return false
 */
bool Net::isClock() { return getType() == kNetTypeClock; }

/**
 * @brief is ground net
 *
 * @return true
 * @return false
 */
bool Net::isGround() { return getType() == kNetTypeGround; }

/**
 * @brief is power net
 *
 * @return true
 * @return false
 */
bool Net::isPower() { return getType() == kNetTypePower; }

/**
 * @brief is reset net
 *
 * @return true
 * @return false
 */
bool Net::isReset() { return getType() == kNetTypeReset; }

/**
 * @brief is scan net
 *
 * @return true
 * @return false
 */
bool Net::isScan() { return getType() == kNetTypeScan; }

/**
 * @brief is signal net
 *
 * @return true
 * @return false
 */
bool Net::isSignal() { return getType() == kNetTypeSignal; }

/**
 * @brief is tie off net
 *
 * @return true
 * @return false
 */
bool Net::isTieOff() { return getType() == kNetTypeTieOff; }

/**
 * @brief Get the assign type
 *
 * @return AssignType
 */
AssignType Net::getAssignType() const { return assign_type_; }

/**
 * @brief Get the assign Net object
 *
 * @return ObjectId
 */
ObjectId Net::getAssignNet() const { return assign_net_; }

/**
 * @brief Get the assign int constant
 *
 * @return int32_t
 */
int32_t Net::getAssignInt() const { return assign_int_; }

/**
 * @brief Get the assign real constant
 *
 * @return double
 */
double Net::getAssignReal() const { return assign_real_; }
/**
 * @brief Get the Source object
 *
 * @return Bits
 */
Bits Net::getSource() const { return source_; }

/**
 * @brief Get the Xtalk object
 *
 * @return int
 */
int Net::getXtalk() const { return xtalk_; }

/**
 * @brief Get the Frequency object
 *
 * @return int
 */
int Net::getFrequency() const { return frequency_; }

/**
 * @brief Get the Weight object
 *
 * @return double
 */
double Net::getWeight() const { return weight_; }

/**
 * @brief Get the Capacitance object
 *
 * @return double
 */
double Net::getCapacitance() const { return cap_; }

/**
 * @brief Get the Origin Net object
 *
 * @return const char*
 */
// const char* Net::getOriginNet() const { return origin_net_.c_str(); }

/**
 * @brief Set the Fix Bump object
 *
 * @param fix_bump
 */
void Net::setFixBump(Bits fix_bump) { fix_bump_ = fix_bump; }

/**
 * @brief Set the Must Join object
 *
 * @param must_join
 */
void Net::setMustJoin(Bits must_join) { must_jion_ = must_join; }

/**
 * @brief Set the Pattern object
 *
 * @param pattern
 */
void Net::setPattern(Bits pattern) { pattern_ = pattern; }

/**
 * @brief Set the Xtalk object
 *
 * @param xtalk
 */
void Net::setXtalk(int xtalk) { xtalk_ = xtalk; }

/**
 * @brief Set the Source object
 *
 * @param source
 */
void Net::setSource(Bits source) { source_ = source; }

/**
 * @brief Set the Frequency object
 *
 * @param frequency
 */
void Net::setFrequency(int frequency) { frequency_ = frequency; }

/**
 * @brief Set the Capacitance object
 *
 * @param cap
 */
void Net::setCapacitance(double cap) { cap_ = cap; }

/**
 * @brief Set the Weight object
 *
 * @param weight
 */
void Net::setWeight(double weight) { weight_ = weight; }

/**
 * @brief Set the Origin Net object
 *
 * @param name
 */
// void Net::setOriginNet(const char* name) { origin_net_ = name; }
/**
 * @brief Set the Is bus Net
 *
 * @param is_bus_net
 */
void Net::setIsBusNet(bool is_bus_net) { is_bus_net_ = is_bus_net; }

/**
 * @brief Get the Is bus flag
 *
 * @param
 */
bool Net::getIsBusNet() const { return is_bus_net_; }

/**
 * @brief Set the Is Net belong to a bus flag
 *
 * @param is_of_bus
 */
void Net::setIsOfBus(bool is_of_bus) { is_of_bus_ = is_of_bus; }

/**
 * @brief Get the Is Net belong to a bus flag
 *
 * @param
 */
bool Net::getIsOfBus() const { return is_of_bus_; }

/**
 * @brief Set the Is Net from term flag
 *
 * @param is_from_term
 */
void Net::setIsFromTerm(bool is_from_term) { is_from_term_ = is_from_term; }

/**
 * @brief Get the Is Net from term flag
 *
 * @param
 */
bool Net::getIsFromTerm() const { return is_from_term_; }

/**
 * @brief Set the Is Sub Net object
 *
 * @param is_sub_net
 */
void Net::setIsSubNet(Bits is_sub_net) { is_sub_net_ = is_sub_net; }

/**
 * @brief Get the Name Index object
 *
 * @return SymbolIndex
 */
SymbolIndex Net::getNameIndex() const { return name_index_; }

/**
 * @brief Get the Name object
 *
 * @return std::string const&
 */
std::string const& Net::getName() {
    return getOwnerCell()->getSymbolByIndex(name_index_);
}

/**
 * @brief Set the Name object
 *
 * @param name
 */
bool Net::setName(std::string const& name) {
    SymbolIndex index = getOwnerCell()->getOrCreateSymbol(name.c_str());
    if (index == kInvalidSymbolIndex) return false;

    name_index_ = index;
    getOwnerCell()->addSymbolReference(name_index_, this->getId());
    return true;
}

/**
 * @brief Get the Type object
 *
 * @return Bits
 */
NetType Net::getType() const { return static_cast<NetType>(net_type_); }

/**
 * @brief Set the Type object
 *
 * @param type
 */
void Net::setType(NetType net_type) { net_type_ = net_type; }

/**
 * @brief Get the Rule object
 *
 * @return ObjectIndex
 */
const char* Net::getNonDefaultRule() const {
    NonDefaultRule* rule = addr<NonDefaultRule>(rule_);
    return rule->getName();
}

/**
 * @brief Set the Rule object
 *
 * @param rule
 */
void Net::setNonDefaultRule(ObjectId rule) { rule_ = rule; }

/**
 * @brief Set the Rule object
 *
 * @param rule
 */
void Net::setNonDefaultRule(NonDefaultRule* rule) { rule_ = rule->getId(); }

/**
 * @brief delete a specific wire in net
 *
 * @param wire
 */
void Net::deleteWire(Wire* wire) {}

/**
 * @brief  add via to the net's vialist
 *
 * @param via
 */
void Net::addVia(Via* via) {
    ArrayObject<ObjectId>* via_vector = nullptr;
    if (vias_ == 0) {
        vias_ = __createObjectIdArray(64);
    }
    if (vias_) via_vector = addr<ArrayObject<ObjectId>>(vias_);
    if (via_vector) via_vector->pushBack(via->getId());
}

/**
 * @brief delete a specific via in net
 *
 * @param Via
 */
void Net::deleteVia(Via* via) {}

/**
 * @brief create sub net
 *
 * @param name
 * @return Net*
 */
Net* Net::createSubNet(std::string& name) {
    Net* sub_net = getOwnerCell()->createObject<Net>(kObjectTypeNet);
    sub_net->setName(name);
    sub_net->setOwner(getOwnerCell());

    return sub_net;
}

/**
 * @brief create virtual pin
 *
 * @param name
 * @return VPin*
 */
VPin* Net::createVpin(std::string& name) {
    VPin* v_pin = getOwnerCell()->createObject<VPin>(kObjectTypePin);
    v_pin->setName(name);
    v_pin->setOwner(getOwnerCell());

    return v_pin;
}

Via* Net::createVia(int x, int y, ViaMaster* via_master) {
    Via* via = getOwnerCell()->createObject<Via>(kObjectTypeVia);
    if (via) {
        via->setLoc(x, y);
        via->setMaster(via_master);
    }

    return via;
}

/**
 * @brief create a wire in net
 *
 * @return Wire*
 */
Wire* Net::createWire(int x1, int y1, int x2, int y2, int width) {
    if (x1 != x2 && y1 != y2) return nullptr;
    Wire* wire = getOwnerCell()->createObject<Wire>(kObjectTypeWire);
    int x = 0, y = 0, length = 0, height = 0;
    // the wire is horizontal
    if (y1 == y2) {
        if (x1 > x2) std::swap(x1, x2);  // keep the x1 in left

        x = x1;
        y = y1 - width / 2;
        length = x2 - x1;
        height = width;
    }
    // the wire is vertical
    if (x1 == x2) {
        if (y1 > y2) std::swap(y1, y2);  // keep the y1 in down
        x = x1 - width / 2;
        y = y1;
        length = width;
        height = y2 - y1;
    }

    wire->setX(x);
    wire->setY(y);
    wire->setHeight(height);
    wire->setLength(length);

    return wire;
}

/**
 * @brief add wire to net
 *
 * @param wire
 */
void Net::addWire(Wire* wire) {
    ArrayObject<ObjectId>* wire_vector = nullptr;
    if (wires_ == 0) {
        wires_ = __createObjectIdArray(256);
    }
    if (wires_) wire_vector = addr<ArrayObject<ObjectId>>(wires_);
    if (wire_vector) wire_vector->pushBack(wire->getId());
}

/**
 * @brief add pin to net
 *
 * @param pin
 * @return int
 */
int Net::addPin(Pin* pin) {
    ArrayObject<ObjectId>* pin_vector = nullptr;
    if (pins_ == 0) {
        pins_ = __createObjectIdArray(64);
    }
    if (pins_) pin_vector = addr<ArrayObject<ObjectId>>(pins_);
    if (pin) pin_vector->pushBack(pin->getId());

    return 0;
}

/**
 * @brief add virtual pin to net
 *
 * @param v_pin
 * @return int
 */
ObjectId Net::addVPin(VPin* v_pin) {
    ArrayObject<ObjectId>* v_pin_vector = nullptr;
    if (vpins == 0) {
        vpins = __createObjectIdArray(64);
    }
    if (vpins) v_pin_vector = addr<ArrayObject<ObjectId>>(vpins);
    if (v_pin) v_pin_vector->pushBack(v_pin->getId());

    return vpins;
}

/**
 * @brief Get the Pin Array object
 *
 * @return ArrayObject<ObjectId>*
 */
ArrayObject<ObjectId>* Net::getPinArray() const {
    if (pins_ != 0) {
        ArrayObject<ObjectId>* pin_array = addr<ArrayObject<ObjectId>>(pins_);
        return pin_array;
    } else {
        return nullptr;
    }
}

/**
 * @brief add sub net to net
 *
 * @param sub_net
 */
/*
void Net::addSubNet(Net* sub_net) {
    ArrayObject<ObjectId>* sub_net_vector = nullptr;
    if (sub_nets_ == 0) {
        sub_nets_ = __createObjectIdArray(64);
    }
    if (sub_nets_) sub_net_vector = addr<ArrayObject<ObjectId>>(sub_nets_);
    if (sub_net_vector) sub_net_vector->pushBack(sub_net->getId());
}

/**
 * @brief add assign net to net
 *
 * @param assign_net
 */
void Net::addAssignNet(ObjectId assign_net) {
    assign_net_ = assign_net;
    assign_type_ = kAssignTypeNet;
}

/**
 * @brief add assign int constant to net
 *
 * @param assign_constant
 */
void Net::addAssignConstant(int32_t assign_constant) {
    assign_int_ = assign_constant;
    assign_type_ = kAssignTypeInt;
}

/**
 * @brief add assign real constant to net
 *
 * @param assign_constant
 */
void Net::addAssignConstant(double assign_constant) {
    assign_real_ = assign_constant;
    assign_type_ = kAssignTypeReal;
}

WirePatch* Net::creatPatch(int loc_x, int loc_y, int x1, int y1, int x2, int y2,
                           int layer) {
    WirePatch* patch = getTopCell()->createObject<WirePatch>(kObjectTypeWire);
    patch->setX1(x1);
    patch->setX2(x2);
    patch->setY1(y1);
    patch->setY2(y2);
    patch->setLocX(loc_x);
    patch->setLocY(loc_y);
    patch->setLayerNum(layer);

    return patch;
}

ObjectId Net::addPatch(WirePatch* patch, ObjectId patches) {
    if (patches == 0) {
        ArrayObject<ObjectId>* patch_vector =
            getTopCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        patch_vector->setPool(getTopCell()->getPool());
        patch_vector->reserve(256);
        patches = patch_vector->getId();
    }
    ArrayObject<ObjectId>* patch_vector =
        Object::addr<ArrayObject<ObjectId>>(patches);
    patch_vector->pushBack(patch->getId());
    return patches;
}

/**
 * @brief print function for Net
 *
 */

/*
void Net::print() {
    if (!is_sub_net_) {
        message->info("\n\n- %s ", getName().c_str());
    } else {
        message->info("\n  + SUBNET %s ", getName().c_str());
    }

    if (must_jion_) message->info("MUSTJOIN ");

    if (vpins) {
        ArrayObject<ObjectId>* v_pin_vector =
            addr<ArrayObject<ObjectId>>(vpins);
        for (ArrayObject<ObjectId>::iterator iter = v_pin_vector->begin();
             iter != v_pin_vector->end(); ++iter) {
            VPin* v_pin = nullptr;
            ObjectId id = (*iter);
            if (id) v_pin = addr<VPin>(id);
            if (v_pin) {
                message->info("\n  + VPIN %s ", v_pin->getName().c_str());
                if (v_pin->getHasLayer())
                    message->info("LAYER %s ", v_pin->getLayer());
                message->info("( %d %d ) ( %d %d ) ", v_pin->getBox().getLLX(),
                              v_pin->getBox().getLLY(),
                              v_pin->getBox().getURX(),
                              v_pin->getBox().getURY());
                if (v_pin->getStatus()) {
                    switch (v_pin->getStatus()) {
                        case 1:
                            message->info("PLACED ");
                            break;
                        case 2:
                            message->info("FIXED ");
                            break;
                        case 3:
                            message->info("COVER ");
                            break;

                        default:
                            break;
                    }
                    message->info("(%d ", v_pin->getLoc().getX());
                    message->info("%d) ", v_pin->getLoc().getY());

                    switch (v_pin->getOrientation()) {
                        case 1:
                            message->info("N ");
                            break;
                        case 2:
                            message->info("S ");
                            break;
                        case 3:
                            message->info("E ");
                            break;
                        case 4:
                            message->info("W ");
                            break;
                        case 5:
                            message->info("FN ");
                            break;
                        case 6:
                            message->info("FS ");
                            break;
                        case 7:
                            message->info("FE ");
                            break;
                        case 8:
                            message->info("FW ");
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }

    // sub net
    if (sub_nets_) {
        ArrayObject<ObjectId>* sub_net_vector =
            addr<ArrayObject<ObjectId>>(sub_nets_);
        for (ArrayObject<ObjectId>::iterator iter = sub_net_vector->begin();
             iter != sub_net_vector->end(); ++iter) {
            Net* sub_net = nullptr;
            ObjectId id = (*iter);
            if (id) sub_net = addr<Net>(id);
            if (sub_net) sub_net->print();
        }
    }

    // wire
    if (wires_) {
        ArrayObject<ObjectId>* wire_vector =
            addr<ArrayObject<ObjectId>>(wires_);
        for (ArrayObject<ObjectId>::iterator iter = wire_vector->begin();
             iter != wire_vector->end(); ++iter) {
            Wire* wire = nullptr;
            ObjectId id = (*iter);
            if (id) wire = addr<Wire>(id);
            if (wire) wire->print();
        }
    }

    if (xtalk_) message->info("\n  + XTALK %d", xtalk_);
    if (rule_) {
        if (is_sub_net_) {
            message->info("\n    NONDEFAULTRULE %s", getNonDefaultRule());
        } else {
            message->info("\n  + NONDEFAULTRULE %s", getNonDefaultRule());
        }
    }
    if (source_) {
        switch (source_) {
            case 1:
                message->info("\n  + SOURCE DIST ");
                break;
            case 2:
                message->info("\n  + SOURCE NETLIST ");
                break;
            case 3:
                message->info("\n  + SOURCE TEST ;");
                break;
            case 4:
                message->info("\n  + SOURCE TIMING ");
                break;
            case 5:
                message->info("\n  + SOURCE USER ");
                break;
            default:
                break;
        }
    }

    if (fix_bump_) message->info("\n  + FIXEDBUMP");
    if (frequency_) message->info("\n  + FREQUENCY %d ", frequency_);
    if (origin_net_.size())
        message->info("\n  + ORIGINAL %s ", origin_net_.c_str());
    if (net_type_) {
        switch (net_type_) {
            case 1:
                message->info("\n  + USE ANALOG ");
                break;
            case 2:
                message->info("\n  + USE CLOCK");
                break;
            case 3:
                message->info("\n  + USE GROUND ");
                break;
            case 4:
                message->info("\n  + USE POWER");
                break;
            case 5:
                message->info("\n  + USE RESET");
                break;
            case 6:
                message->info("\n  + USE SCAN");
                break;
            case 7:
                message->info("\n  + USE SIGNAL ");
                break;
            case 8:
                message->info("\n  + USE TIEOFF ");
                break;
            default:
                break;
        }
    }

    if (pattern_) {
        switch (pattern_) {
            case 1:
                message->info("\n  + PATTERN BALANCED");
                break;
            case 2:
                message->info("\n  + PATTERN STEINER");
                break;
            case 3:
                message->info("\n  + PATTERN TRUNK");
                break;
            case 4:
                message->info("\n  + PATTERN WIREDLOGIC ");
                break;
            default:
                break;
        }
    }

    if (cap_) message->info("\n  + ESTCAP %f ", cap_);
    if (weight_) message->info("\n  + WEIGHT %f", weight_);
}

/**
 * @brief print file function for Net
 *
 */

void Net::printDEF(FILE* fp) {
    if (!is_sub_net_) {
        fprintf(fp, "\n- %s ", getName().c_str());
    } else {
        fprintf(fp, "\n  + SUBNET %s ", getName().c_str());
    }
    if (pins_) {
        ArrayObject<ObjectId>* pin_vector = addr<ArrayObject<ObjectId>>(pins_);
        for (ArrayObject<ObjectId>::iterator iter = pin_vector->begin();
             iter != pin_vector->end(); ++iter) {
            Pin* pin = nullptr;
            ObjectId id = (*iter);
            if (id) pin = addr<Pin>(id);
            if (pin) {
                Inst* inst = pin->getInst();
                if (inst) {
                    fprintf(fp, "\n  ( %s ", inst->getName().c_str());
                } else {
                    fprintf(fp, "\n  ( PIN ");
                }

                fprintf(fp, "%s ) \n", pin->getName().c_str());
            }
        }
    }

    if (must_jion_) fprintf(fp, "MUSTJOIN ");
    if (xtalk_) fprintf(fp, "  + XTALK %d\n", xtalk_);
    if (rule_) {
        if (is_sub_net_) {
            fprintf(fp, "    NONDEFAULTRULE %s\n", getNonDefaultRule());
        } else {
            fprintf(fp, "  + NONDEFAULTRULE %s\n", getNonDefaultRule());
        }
    }
    if (fix_bump_) fprintf(fp, "  + FIXEDBUMP\n");
    if (frequency_) fprintf(fp, "  + FREQUENCY %d \n", frequency_);
    if (net_type_) {
        if (isAnalog()) fprintf(fp, "  + USE ANALOG \n");
        if (isClock()) fprintf(fp, "  + USE CLOCK \n");
        if (isGround()) fprintf(fp, "  + USE GROUND \n");
        if (isPower()) fprintf(fp, "  + USE POWER \n");
        if (isReset()) fprintf(fp, "  + USE RESET \n");
        if (isScan()) fprintf(fp, "  + USE SCAN \n");
        if (isSignal()) fprintf(fp, "  + USE SIGNAL\n");
        if (isTieOff()) fprintf(fp, "  + USE TIEOFF\n");
    }

    // wire
    if (wires_) {
        ArrayObject<ObjectId>* wire_vector =
            addr<ArrayObject<ObjectId>>(wires_);
        for (ArrayObject<ObjectId>::iterator iter = wire_vector->begin();
             iter != wire_vector->end(); ++iter) {
            Wire* wire = nullptr;
            ObjectId id = (*iter);
            if (id) wire = addr<Wire>(id);
            if (wire) wire->printDEF(fp);
        }
    }
    // via
    if (vias_) {
        ArrayObject<ObjectId>* via_vector = addr<ArrayObject<ObjectId>>(vias_);
        for (ArrayObject<ObjectId>::iterator iter = via_vector->begin();
             iter != via_vector->end(); ++iter) {
            Via* via = nullptr;
            ObjectId id = (*iter);
            if (id) via = addr<Via>(id);
            if (via) via->printDEF(fp);
        }
    }
    // patch
    ObjectId patches = 0;
    auto search = patch_map.find(getId());
    if (search != patch_map.end()) {
        patches = search->second;
    }
    if (patches) {
        ArrayObject<ObjectId>* patch_vector =
            addr<ArrayObject<ObjectId>>(patches);
        for (ArrayObject<ObjectId>::iterator iter = patch_vector->begin();
             iter != patch_vector->end(); ++iter) {
            WirePatch* patch = nullptr;
            ObjectId id = (*iter);
            if (id) patch = addr<WirePatch>(id);
            if (patch) patch->printDEF(fp);
        }
    }

    // Vpin
    ObjectId vpins = 0;
    search = Vpin_map.find(getId());
    if (search != Vpin_map.end()) {
        vpins = search->second;
    }

    if (vpins) {
        ArrayObject<ObjectId>* v_pin_vector =
            addr<ArrayObject<ObjectId>>(vpins);
        for (ArrayObject<ObjectId>::iterator iter = v_pin_vector->begin();
             iter != v_pin_vector->end(); ++iter) {
            VPin* v_pin = nullptr;
            ObjectId id = (*iter);
            if (id) v_pin = addr<VPin>(id);
            if (v_pin) {
                fprintf(fp, "  + VPIN %s ", v_pin->getName().c_str());
                if (strlen(v_pin->getLayer()) > 0)
                    fprintf(fp, "LAYER %s ", v_pin->getLayer());
                fprintf(fp, "( %d %d ) ( %d %d ) ", v_pin->getBox().getLLX(),
                        v_pin->getBox().getLLY(), v_pin->getBox().getURX(),
                        v_pin->getBox().getURY());
                if (v_pin->getStatus()) {
                    switch (v_pin->getStatus()) {
                        case 1:
                            fprintf(fp, "PLACED ");
                            break;
                        case 2:
                            fprintf(fp, "FIXED ");
                            break;
                        case 3:
                            fprintf(fp, "COVER ");
                            break;

                        default:
                            break;
                    }
                    fprintf(fp, "( %d ", v_pin->getLoc().getX());
                    fprintf(fp, "%d ) ", v_pin->getLoc().getY());

                    switch (v_pin->getOrientation()) {
                        case 1:
                            fprintf(fp, "N ");
                            break;
                        case 2:
                            fprintf(fp, "S ");
                            break;
                        case 3:
                            fprintf(fp, "E ");
                            break;
                        case 4:
                            fprintf(fp, "W ");
                            break;
                        case 5:
                            fprintf(fp, "FN ");
                            break;
                        case 6:
                            fprintf(fp, "FS ");
                            break;
                        case 7:
                            fprintf(fp, "FE ");
                            break;
                        case 8:
                            fprintf(fp, "FW ");
                            break;
                        default:
                            break;
                    }
                }
            }
            fprintf(fp, "\n");
        }
    }

    ObjectId properties = 0;
    search = property_map.find(getId());
    if (search != property_map.end()) {
        properties = search->second;
    }
    if (properties) {
        ArrayObject<ObjectId>* id_array_ptr =
            Object::addr<ArrayObject<ObjectId>>(properties);
        for (int i = 0; i < id_array_ptr->getSize(); i++) {
            ObjectId obj_id = (*id_array_ptr)[i];
            if (!obj_id) continue;
            Property* property = Object::addr<Property>(obj_id);
            if (property == nullptr) continue;
            property->printDEF(fp);
        }
    }
    fprintf(fp, "  ;\n");
}

/*
if (!is_sub_net_) {
    fprintf(fp, "\n- %s ", getName().c_str());
} else {
    fprintf(fp, "\n  + SUBNET %s ", getName().c_str());
}





if (rule_) {
    if (is_sub_net_) {
        fprintf(fp, "\n    NONDEFAULTRULE %s", getNonDefaultRule());
    } else {
        fprintf(fp, "\n  + NONDEFAULTRULE %s", getNonDefaultRule());
    }
}
if (source_) {
    switch (source_) {
        case 1:
            fprintf(fp, "\n  + SOURCE DIST");
            break;
        case 2:
            fprintf(fp, "\n  + SOURCE NETLIST");
            break;
        case 3:
            fprintf(fp, "\n  + SOURCE TEST");
            break;
        case 4:
            fprintf(fp, "\n  + SOURCE TIMING");
            break;
        case 5:
            fprintf(fp, "\n  + SOURCE USER");
            break;
        default:
            break;
    }
}


if (pattern_) {
    switch (pattern_) {
        case 1:
            fprintf(fp, "\n  + PATTERN BALANCED");
            break;
        case 2:
            fprintf(fp, "\n  + PATTERN STEINER");
            break;
        case 3:
            fprintf(fp, "\n  + PATTERN TRUNK");
            break;
        case 4:
            fprintf(fp, "\n  + PATTERN WIREDLOGIC");
            break;
        default:
            break;
    }
}

if (cap_) fprintf(fp, "\n  + ESTCAP %f ", cap_);
if (weight_) fprintf(fp, "\n  + WEIGHT %d", weight_);

if (vpins) {
    ArrayObject<ObjectId>* v_pin_vector =
        addr<ArrayObject<ObjectId>>(vpins);
    for (ArrayObject<ObjectId>::iterator iter = v_pin_vector->begin();
         iter != v_pin_vector->end(); ++iter) {
        VPin* v_pin = nullptr;
        ObjectId id = (*iter);
        if (id) v_pin = addr<VPin>(id);
        if (v_pin) {
            fprintf(fp, "\n  + VPIN %s ", v_pin->getName().c_str());
            if (strlen(v_pin->getLayer()) > 0)
                fprintf(fp, "LAYER %s ", v_pin->getLayer());
            fprintf(fp, "( %d %d ) ( %d %d ) ", v_pin->getBox().getLLX(),
                    v_pin->getBox().getLLY(), v_pin->getBox().getURX(),
                    v_pin->getBox().getURY());
            if (v_pin->getStatus()) {
                switch (v_pin->getStatus()) {
                    case 1:
                        fprintf(fp, "PLACED ");
                        break;
                    case 2:
                        fprintf(fp, "FIXED ");
                        break;
                    case 3:
                        fprintf(fp, "COVER ");
                        break;

                    default:
                        break;
                }
                fprintf(fp, "( %d ", v_pin->getLoc().getX());
                fprintf(fp, "%d ) ", v_pin->getLoc().getY());

                switch (v_pin->getOrientation()) {
                    case 1:
                        fprintf(fp, "N ");
                        break;
                    case 2:
                        fprintf(fp, "S ");
                        break;
                    case 3:
                        fprintf(fp, "E ");
                        break;
                    case 4:
                        fprintf(fp, "W ");
                        break;
                    case 5:
                        fprintf(fp, "FN ");
                        break;
                    case 6:
                        fprintf(fp, "FS ");
                        break;
                    case 7:
                        fprintf(fp, "FE ");
                        break;
                    case 8:
                        fprintf(fp, "FW ");
                        break;
                    default:
                        break;
                }
            }
        }
    }
}


// sub net
if (sub_nets_) {
    ArrayObject<ObjectId>* sub_net_vector =
        addr<ArrayObject<ObjectId>>(sub_nets_);
    for (ArrayObject<ObjectId>::iterator iter = sub_net_vector->begin();
         iter != sub_net_vector->end(); ++iter) {
        Net* sub_net = nullptr;
        ObjectId id = (*iter);
        if (id) sub_net = addr<Net>(id);
        if (sub_net) sub_net->printDEF(fp);
    }
}

writeDEFProperty<Net>((void*)this, fp);

if (!is_sub_net_) {
    fprintf(fp, "\n  ;\n");
}
}
*/
void Net::setPropertySize(uint64_t v) {
    if (v == 0) {
        if (properties) {
            __deleteObjectIdArray(properties);
        }
        return;
    }
    if (!properties) {
        properties = __createObjectIdArray(16);
    }
}

uint64_t Net::getNumProperties() const {
    if (!properties) return 0;

    return addr<IdArray>(properties)->getSize();
}

ObjectId Net::addProperty(ObjectId obj_id) {
    IdArray* id_array_ptr = nullptr;
    if (obj_id == 0) return 0;

    if (properties == 0) {
        properties = __createObjectIdArray(16);
    }
    ediAssert(properties != 0);
    id_array_ptr = addr<IdArray>(properties);
    ediAssert(id_array_ptr != nullptr);
    id_array_ptr->pushBack(obj_id);
    return properties;
}

ObjectId Net::getPropertiesId() const { return properties; }

}  // namespace db
}  // namespace open_edi
