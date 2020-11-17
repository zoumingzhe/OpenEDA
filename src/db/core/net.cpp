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

#include "db/core/cell.h"
#include "db/core/db.h"
#include "db/core/pin.h"
#include "db/util/array.h"
#include "db/util/vector_object_var.h"

namespace open_edi {
namespace db {
using IdArray = ArrayObject<ObjectId>;

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
    origin_net_ = "";
    assign_net_ = 0;
    v_pins_ = 0;
    properties_id_ = 0;
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
bool Net::isAnalog() {return getType()==kNetTypeAnalog;}

/**
 * @brief is clock net
 *
 * @return true
 * @return false
 */
bool Net::isClock() {return getType()==kNetTypeClock;}

/**
 * @brief is ground net
 *
 * @return true
 * @return false
 */
bool Net::isGround(){return getType()== kNetTypeGround;}

/**
 * @brief is power net
 *
 * @return true
 * @return false
 */
bool Net::isPower(){return getType()==kNetTypePower;}

/**
 * @brief is reset net
 *
 * @return true
 * @return false
 */
bool Net::isReset(){return getType()==kNetTypeReset;}

/**
 * @brief is scan net
 *
 * @return true
 * @return false
 */
bool Net::isScan() {return getType()==kNetTypeScan;}

/**
 * @brief is signal net
 *
 * @return true
 * @return false
 */
bool Net::isSignal(){return getType()==kNetTypeSignal;}

/**
 * @brief is tie off net
 *
 * @return true
 * @return false
 */
bool Net::isTieOff(){return getType()==kNetTypeTieOff;}

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
const char* Net::getOriginNet() const { return origin_net_.c_str(); }

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
void Net::setOriginNet(const char* name) { origin_net_ = name; }
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
 * @brief set cell
 *
 * @param cell
 */

void Net::setCell(ObjectId cell) { cell_ = cell; }
/**
 * @brief get cell
 *
 * @return Cell*
 */
Cell* Net::getCell() { return addr<Cell>(cell_); }

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
 * @brief add wire to the net's wirelist
 *
 * @param wire
 */
void Net::addWire(Wire* wire) {
    VectorObject64* wire_vector = nullptr;
    if (wires_ == 0) {
        wires_ = getOwnerCell()->createVectorObject<VectorObject64>()->getId();
    }
    if (wires_)
        wire_vector = addr<VectorObject64>(wires_);
    if (wire_vector) wire_vector->push_back(wire->getId());
}

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
    VectorObject64* via_vector = nullptr;
    if (vias_ == 0) {
        vias_ = getOwnerCell()->createVectorObject<VectorObject64>()->getId();
    }
    if (vias_) via_vector = addr<VectorObject64>(vias_);
    if (via_vector) via_vector->push_back(via->getId());
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

/**
 * @brief create a wire graph in section
 *
 * @return WireGraph*
 */
WireGraph* Net::creatGraph() {
    WireGraph* graph = getOwnerCell()->createObject<WireGraph>(kObjectTypeWire);

    return graph;
}

/**
 * @brief add graph to net
 *
 * @param graph
 */
void Net::addGraph(WireGraph* graph) {
    VectorObject256* graph_vector = nullptr;
    if (graphs_ == 0) {
        graphs_ = getOwnerCell()->createVectorObject<VectorObject256>()->getId();
    }
    if (graphs_)
        graph_vector = addr<VectorObject256>(graphs_);
    if (graph_vector) graph_vector->push_back(graph->getId());
}

/**
 * @brief add pin to net
 *
 * @param pin
 * @return int
 */
int Net::addPin(Pin* pin) {
    VectorObject64* pin_vector = nullptr;
    if (pins_ == 0) {
        pins_ = getOwnerCell()->createVectorObject<VectorObject64>()->getId();
    }
    if (pins_) pin_vector = addr<VectorObject64>(pins_);
    if (pin) pin_vector->push_back(pin->getId());

    return 0;
}

/**
 * @brief add virtual pin to net
 *
 * @param v_pin
 * @return int
 */
int Net::addVPin(VPin* v_pin) {
    VectorObject64* v_pin_vector = nullptr;
    if (v_pins_ == 0) {
        v_pins_ = getOwnerCell()->createVectorObject<VectorObject64>()->getId();
    }
    if (v_pins_)
        v_pin_vector = addr<VectorObject64>(v_pins_);
    if (v_pin) v_pin_vector->push_back(v_pin->getId());

    return 0;
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
void Net::addSubNet(Net* sub_net) {
    VectorObject64* sub_net_vector = nullptr;
    if (sub_nets_ == 0) {
        sub_nets_ = getOwnerCell()->createVectorObject<VectorObject64>()->getId();
    }
    if (sub_nets_)
        sub_net_vector =
            addr<VectorObject64>(sub_nets_);
    if (sub_net_vector) sub_net_vector->push_back(sub_net->getId());
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

/**
 * @brief print function for Net
 *
 */
void Net::print() {
    if (!is_sub_net_) {
        message->info("\n\n- %s ", getName().c_str());
    } else {
        message->info("\n  + SUBNET %s ", getName().c_str());
    }

    if (must_jion_) message->info("MUSTJOIN ");

    if (v_pins_) {
        VectorObject64* v_pin_vector =
            addr<VectorObject64>(v_pins_);
        for (VectorObject64::iterator iter = v_pin_vector->begin();
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
        VectorObject64* sub_net_vector =
            addr<VectorObject64>(sub_nets_);
        for (VectorObject64::iterator iter = sub_net_vector->begin();
             iter != sub_net_vector->end(); ++iter) {
            Net* sub_net = nullptr;
            ObjectId id = (*iter);
            if (id) sub_net = addr<Net>(id);
            if (sub_net) sub_net->print();
        }
    }
    // wire
    // wire
    if (graphs_) {
        VectorObject64* graph_vector =
            addr<VectorObject64>(graphs_);
        for (VectorObject64::iterator iter = graph_vector->begin();
             iter != graph_vector->end(); ++iter) {
            WireGraph* graph = nullptr;
            ObjectId id = (*iter);
            if (id) graph = addr<WireGraph>(id);
            if (graph) graph->print();
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
        VectorObject64* pin_vector =
            addr<VectorObject64>(pins_);
        for (VectorObject64::iterator iter = pin_vector->begin();
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

                fprintf(fp, "%s ) ", pin->getName().c_str());
            }
        }
    }

    if (must_jion_) fprintf(fp, "MUSTJOIN ");

    if (xtalk_) fprintf(fp, "\n  + XTALK %d", xtalk_);
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

    if (fix_bump_) fprintf(fp, "\n  + FIXEDBUMP");
    if (frequency_) fprintf(fp, "\n  + FREQUENCY %d ", frequency_);
    if (origin_net_.size())
        fprintf(fp, "\n  + ORIGINAL %s ", origin_net_.c_str());
    if (net_type_) {
        if (isAnalog()) fprintf(fp, "\n  + USE ANALOG");
        if (isClock()) fprintf(fp, "\n  + USE CLOCK");
        if (isGround()) fprintf(fp, "\n  + USE GROUND");
        if (isPower()) fprintf(fp, "\n  + USE POWER");
        if (isReset()) fprintf(fp, "\n  + USE RESET");
        if (isScan()) fprintf(fp, "\n  + USE SCAN");
        if (isSignal()) fprintf(fp, "\n  + USE SIGNAL");
        if (isTieOff()) fprintf(fp, "\n  + USE TIEOFF");
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

    if (v_pins_) {
        VectorObject64* v_pin_vector =
            addr<VectorObject64>(v_pins_);
        for (VectorObject64::iterator iter = v_pin_vector->begin();
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
    // wire
    if (graphs_) {
        VectorObject64* graph_vector =
            addr<VectorObject64>(graphs_);
        for (VectorObject64::iterator iter = graph_vector->begin();
             iter != graph_vector->end(); ++iter) {
            WireGraph* graph = nullptr;
            ObjectId id = (*iter);
            if (id) graph = addr<WireGraph>(id);
            if (graph) graph->printDEF(fp);
        }
    }

    // sub net
    if (sub_nets_) {
        VectorObject64* sub_net_vector =
            addr<VectorObject64>(sub_nets_);
        for (VectorObject64::iterator iter = sub_net_vector->begin();
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

void Net::setPropertySize(uint64_t v) {
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

uint64_t Net::getNumProperties() const {
    if (!properties_id_) return 0;

    return addr<IdArray>(properties_id_)->getSize();  
}

void Net::addProperty(ObjectId obj_id) {
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

ObjectId Net::getPropertiesId() const { return properties_id_; }

}  // namespace db
}  // namespace open_edi
