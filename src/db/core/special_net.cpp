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
#include "db/core/special_net.h"

#include "db/core/cell.h"
#include "db/core/db.h"
#include "db/core/pin.h"
#include "db/util/vector_object_var.h"

namespace open_edi {
namespace db {
/**
 * @brief Construct a new Special Net object
 *
 */
SpecialNet::SpecialNet() {
    name_index_ = 0;
    fix_bump_ = 0;
    net_type_ = 0;
    status_ = 0;
    pattern_ = 0;
    rule_ = 0;
    voltage_ = 0;
    frequency_ = 0;
    cap_ = 0;
    weight_ = 0;
    origin_net_ = 0;
}

Cell* SpecialNet::getCell() {
    ObjectId cell_id = getOwnerId();
    if (cell_id) {
        return addr<Cell>(cell_id);
    }
    return nullptr;
}

/**
 * @brief Get the Name Index object
 *
 * @return SymbolIndex
 */
SymbolIndex SpecialNet::getNameIndex() const { return name_index_; }

/**
 * @brief Get the Name object
 *
 * @return std::string const&
 */
std::string const& SpecialNet::getName() {
    return getTopCell()->getSymbolByIndex(name_index_);
}

/**
 * @brief Get the Fix Bump object
 *
 * @return Bits
 */
Bits SpecialNet::getFixBump() const { return fix_bump_; }

/**
 * @brief Get the Voltage object
 *
 * @return int
 */
int SpecialNet::getVoltage() const { return voltage_; }

/**
 * @brief Get the Pattern object
 *
 * @return Bits
 */
Bits SpecialNet::getPattern() const { return pattern_; }
/**
 * @brief Get the Source object
 *
 * @return Bits
 */
Bits SpecialNet::getSource() const { return source_; }

/**
 * @brief Get the Frequency object
 *
 * @return int
 */
int SpecialNet::getFrequency() const { return frequency_; }

/**
 * @brief Get the Capacitance object
 *
 * @return double
 */
double SpecialNet::getCapacitance() const { return cap_; }

/**
 * @brief Get the Weight object
 *
 * @return double
 */
double SpecialNet::getWeight() const { return weight_; }

/**
 * @brief is analog net
 *
 * @return true
 * @return false
 */
bool SpecialNet::isAnalog() { return getType() == kSpecialNetTypeAnalog; }

/**
 * @brief is clock net
 *
 * @return true
 * @return false
 */
bool SpecialNet::isClock() { return getType() == kSpecialNetTypeClock; }

/**
 * @brief is ground net
 *
 * @return true
 * @return false
 */
bool SpecialNet::isGround() { return getType() == kSpecialNetTypeGround; }

/**
 * @brief is power net
 *
 * @return true
 * @return false
 */
bool SpecialNet::isPower() { return getType() == kSpecialNetTypePower; }

/**
 * @brief is reset net
 *
 * @return true
 * @return false
 */
bool SpecialNet::isReset() { return getType() == kSpecialNetTypeReset; }

/**
 * @brief is scan net
 *
 * @return true
 * @return false
 */
bool SpecialNet::isScan() { return getType() == kSpecialNetTypeScan; }

/**
 * @brief is signal net
 *
 * @return true
 * @return false
 */
bool SpecialNet::isSignal() { return getType() == kSpecialNetTypeSignal; }

/**
 * @brief is tie off net
 *
 * @return true
 * @return false
 */
bool SpecialNet::isTieOff() { return getType() == kSpecialNetTypeTieOff; }

/**
 * @brief Set the Fix Bump object
 *
 * @param fix_bump
 */
void SpecialNet::setFixBump(Bits fix_bump) { fix_bump_ = fix_bump; }

/**
 * @brief Set the Pattern object
 *
 * @param pattern
 */
void SpecialNet::setPattern(Bits pattern) { pattern_ = pattern; }

/**
 * @brief Set the Source object
 *
 * @param source
 */
void SpecialNet::setSource(Bits source) { source_ = source; }

/**
 * @brief Set the Voltage object
 *
 * @param voltage
 */
void SpecialNet::setVoltage(int voltage) { voltage_ = voltage; }

/**
 * @brief Set the Frequency object
 *
 * @param frequency
 */
void SpecialNet::setFrequency(int frequency) { frequency_ = frequency; }

/**
 * @brief Set the Capacitance object
 *
 * @param cap
 */
void SpecialNet::setCapacitance(double cap) { cap_ = cap; }

/**
 * @brief Set the Weight object
 *
 * @param weight
 */
void SpecialNet::setWeight(double weight) { weight_ = weight; }

/**
 * @brief Set the Name object
 *
 * @param name
 */
bool SpecialNet::setName(std::string const& name) {
    int64_t index = getTopCell()->getOrCreateSymbol(name.c_str());
    if (index == kInvalidSymbolIndex) return false;

    name_index_ = index;
    getTopCell()->addSymbolReference(name_index_, this->getId());
    return true;
}

/**
 * @brief Get the Type object
 *
 * @return Bits
 */
SpecialNetType SpecialNet::getType() const {
    return static_cast<SpecialNetType>(net_type_);
}

/**
 * @brief Set the Type object
 *
 * @param type
 */
void SpecialNet::setType(SpecialNetType net_type) { net_type_ = net_type; }

/**
 * @brief Get the Rule object
 *
 * @return ObjectIndex
 */
const char* SpecialNet::getNonDefaultRule() const {
    NonDefaultRule* rule = addr<NonDefaultRule>(rule_);
    return rule->getName();
}

/**
 * @brief Set the Rule object
 *
 * @param rule
 */
void SpecialNet::setNonDefaultRule(ObjectId rule) { rule_ = rule; }

/**
 * @brief add wire to the net's wirelist
 *
 * @param wire
 */
void SpecialNet::addWire(Wire* wire) {
    ArrayObject<ObjectId>* wire_vector = nullptr;
    if (wires_ == 0) {
        wires_ = __createObjectIdArray(256);
    }
    if (wires_) wire_vector = addr<ArrayObject<ObjectId>>(wires_);
    if (wire_vector) wire_vector->pushBack(wire->getId());
}

void SpecialNet::setOriginNet(SpecialNet* net) {
    if (net) origin_net_ = net->getId();
}

const char* SpecialNet::getOriginNet() {
    Net* net = nullptr;
    if (origin_net_) {
        net = addr<Net>(origin_net_);
    }
    if (net) {
        return net->getName().c_str();
    } else {
        return nullptr;
    }
}

/**
 * @brief create a wire in net
 *
 * @return Wire*
 */
Wire* SpecialNet::createWire(int x1, int y1, int x2, int y2, int width) {
    Wire* wire = getOwnerCell()->createObject<Wire>(kObjectTypeWire);
    int x = 0, y = 0, length = 0, height = 0;

    // the wire is horizontal
    if (y1 == y2) {
        if (x1 < x2) std::swap(x1, x2);  // keep the x1 in left

        x = x1 - width / 2;
        y = y1 - width / 2;
        length = x2 - x1 + width;
        height = width;
    }
    // the wire is vertical
    if (x1 = x2) {
        if (y1 > y2) std::swap(y1, y2);  // keep the y1 in down
        x = x1 - width / 2;
        y = y1 - width / 2;
        length = width;
        height = y2 - y1 + width;
    }

    wire->setX(x);
    wire->setY(y);
    wire->setHeight(height);
    wire->setLength(length);

    return wire;
}

Wire* SpecialNet::createWire(int x1, int y1, int x2, int y2) {
    Wire* wire = getOwnerCell()->createObject<Wire>(kObjectTypeWire);
    wire->setX(x1);
    wire->setY(y1);
    wire->setHeight(y2 - y1);
    wire->setLength(x2 - x1);
    return wire;
}

/**
 * @brief add pin to not
 *
 * @param pin
 * @return int
 */
int SpecialNet::addPin(Pin* pin) {
    VectorObject64* pin_vector = nullptr;
    if (pins_ == 0) {
        pins_ = getTopCell()->createVectorObject<VectorObject64>()->getId();
    }
    if (pins_) pin_vector = addr<VectorObject64>(pins_);
    if (pin) pin_vector->push_back(pin->getId());

    return 0;
}

Via* SpecialNet::createVia(int x, int y, ViaMaster* via_master) {
    Via* via = getOwnerCell()->createObject<Via>(kObjectTypeVia);
    if (via) via->setLoc(x, y);
    return via;
}

/**
 * @brief  add via to the net's vialist
 *
 * @param via
 */
int SpecialNet::addVia(Via* via) {
    VectorObject64* via_vector = nullptr;
    if (vias_ == 0) {
        vias_ = getTopCell()->createVectorObject<VectorObject64>()->getId();
    }
    if (vias_) via_vector = addr<VectorObject64>(vias_);
    if (via) via_vector->push_back(via->getId());

    return 0;
}

/**
 * @brief print function for SpecialNet
 *
 */

/*
void SpecialNet::print() {
    message->info("\n - %s ", getName().c_str());

    // pin
    if (pins_) {
        VectorObject64* pin_vector = addr<VectorObject64>(pins_);
        for (VectorObject64::iterator iter = pin_vector->begin();
             iter != pin_vector->end(); ++iter) {
            Pin* pin = nullptr;
            ObjectId id = (*iter);
            if (id) pin = addr<Pin>(id);
            // if (pin) pin->print();
        }
    }
    // wire section
    if (wire_sections_) {
        VectorObject64* section_vector = addr<VectorObject64>(wire_sections_);
        for (VectorObject64::iterator iter = section_vector->begin();
             iter != section_vector->end(); ++iter) {
            SpecialWireSection* section = nullptr;
            ObjectId id = (*iter);
            if (id) section = addr<SpecialWireSection>(id);
            if (section) section->print();
        }
    }

    if (voltage_) message->info("\n + VOLTAGE %d", voltage_);
    if (rule_) message->info("\n + NONDEFAULTRULE %s", getNonDefaultRule());

    if (source_) {
        switch (source_) {
            case 1:
                message->info("\n + SOURCE DIST ");
                break;
            case 2:
                message->info("\n + SOURCE NETLIST ");
                break;
            case 3:
                message->info("\n + SOURCE TEST ");
                break;
            case 4:
                message->info("\n + SOURCE TIMING ");
                break;
            case 5:
                message->info("\n + SOURCE USER ");
                break;
            default:
                break;
        }
    }

    if (fix_bump_) message->info("\n + FIXEDBUMP");
    if (frequency_) message->info("\n + FREQUENCY %d ", frequency_);
    if (getOriginNet()) message->info("\n + ORIGINAL %s ", getOriginNet());
    if (net_type_) {
        switch (net_type_) {
            case 1:
                message->info("\n + USE ANALOG ");
                break;
            case 2:
                message->info("\n + USE CLOCK");
                break;
            case 3:
                message->info("\n + USE GROUND ");
                break;
            case 4:
                message->info("\n + USE POWER");
                break;
            case 5:
                message->info("\n + USE RESET");
                break;
            case 6:
                message->info("\n + USE SCAN");
                break;
            case 7:
                message->info("\n + USE SIGNAL ");
                break;
            case 8:
                message->info("\n + USE TIEOFF ");
                break;
            default:
                break;
        }
    }

    if (pattern_) {
        switch (pattern_) {
            case 1:
                message->info("\n + PATTERN BALANCED ");
                break;
            case 2:
                message->info("\n + PATTERN STEINER ");
                break;
            case 3:
                message->info("\n + PATTERN TRUNK");
                break;
            case 4:
                message->info("\n + PATTERN WIREDLOGIC ");
                break;
            default:
                break;
        }
    }

    if (cap_) message->info("\n + ESTCAP %f ", cap_);
    if (weight_) message->info("\n + WEIGHT %f", weight_);
    message->info("\n\n");
}

/**
 * @brief print function for SpecialNet
 *
 */
void SpecialNet::printDEF(FILE* fp) {
    fprintf(fp, " - %s \n", getName().c_str());
    // pin
    if (pins_) {
        VectorObject64* pin_vector = addr<VectorObject64>(pins_);
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

    if (net_type_) {
        if (isAnalog()) fprintf(fp, " + USE ANALOG ;\n");
        if (isClock()) fprintf(fp, " + USE CLOCK ;\n");
        if (isGround()) fprintf(fp, " + USE GROUND ;\n");
        if (isPower()) fprintf(fp, " + USE POWER ;\n");
        if (isReset()) fprintf(fp, " + USE RESET ;\n");
        if (isScan()) fprintf(fp, " + USE SCAN ;\n");
        if (isSignal()) fprintf(fp, " + USE SIGNAL ;\n");
        if (isTieOff()) fprintf(fp, " + USE TIEOFF ;\n");
    }
    
}
/*
    fprintf(fp, " - %s ", getName().c_str());
    // pin
    if (pins_) {
        VectorObject64* pin_vector = addr<VectorObject64>(pins_);
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
    if (voltage_) fprintf(fp, "\n + VOLTAGE %d", voltage_);
    if (rule_) fprintf(fp, "\n + NONDEFAULTRULE %s", getNonDefaultRule());

    if (source_) {
        switch (source_) {
            case 1:
                fprintf(fp, "\n + SOURCE DIST");
                break;
            case 2:
                fprintf(fp, "\n + SOURCE NETLIST");
                break;
            case 3:
                fprintf(fp, "\n + SOURCE TEST");
                break;
            case 4:
                fprintf(fp, "\n + SOURCE TIMING");
                break;
            case 5:
                fprintf(fp, "\n + SOURCE USER");
                break;
            default:
                break;
        }
    }

    if (fix_bump_) fprintf(fp, "\n + FIXEDBUMP");
    if (frequency_) fprintf(fp, "\n + FREQUENCY %d", frequency_);
    if (origin_net_) fprintf(fp, "\n + ORIGINAL %s", getOriginNet());
    if (net_type_) {
        if (isAnalog()) fprintf(fp, "\n + USE ANALOG");
        if (isClock()) fprintf(fp, "\n + USE CLOCK");
        if (isGround()) fprintf(fp, "\n + USE GROUND");
        if (isPower()) fprintf(fp, "\n + USE POWER");
        if (isReset()) fprintf(fp, "\n + USE RESET");
        if (isScan()) fprintf(fp, "\n + USE SCAN");
        if (isSignal()) fprintf(fp, "\n + USE SIGNAL");
        if (isTieOff()) fprintf(fp, "\n + USE TIEOFF");
    }

    if (pattern_) {
        switch (pattern_) {
            case 1:
                fprintf(fp, "\n + PATTERN BALANCED");
                break;
            case 2:
                fprintf(fp, "\n + PATTERN STEINER");
                break;
            case 3:
                fprintf(fp, "\n + PATTERN TRUNK");
                break;
            case 4:
                fprintf(fp, "\n + PATTERN WIREDLOGIC");
                break;
            default:
                break;
        }
    }

    if (cap_) fprintf(fp, "\n + ESTCAP %f", cap_);
    if (weight_) fprintf(fp, "\n + WEIGHT %f", weight_);

    // wire section
    if (wire_sections_) {
        VectorObject64* section_vector = addr<VectorObject64>(wire_sections_);
        for (VectorObject64::iterator iter = section_vector->begin();
             iter != section_vector->end(); ++iter) {
            SpecialWireSection* section = nullptr;
            ObjectId id = (*iter);
            if (id) section = addr<SpecialWireSection>(id);
            if (section) section->printDEF(fp);
        }
    }

    for (auto iter = wire_rects_.begin(); iter != wire_rects_.end(); iter++) {
        (*iter)->printDEF(fp);
    }

    fprintf(fp, " ;\n");
}*/

void SpecialNet::setPropertySize(uint64_t v) {
    if (v == 0) {
        if (properties_id_) {
            VectorObject16::deleteDBVectorObjectVar(properties_id_);
        }
        return;
    }
    if (!properties_id_) {
        VectorObject16* vobj =
            VectorObject16::createDBVectorObjectVar(true /*is_header*/);
        ediAssert(vobj != nullptr);
        // using push_back to insert...remove reserve().
        // vobj->reserve(v);
        properties_id_ = vobj->getId();
    }
}

uint64_t SpecialNet::getNumProperties() const {
    if (!properties_id_) return 0;

    return addr<VectorObject16>(properties_id_)->totalSize();
}

ObjectId SpecialNet::addProperty(ObjectId obj_id) {
    VectorObject16* vobj = nullptr;
    if (obj_id == 0) return 0;

    if (properties_id_ == 0) {
        vobj = VectorObject16::createDBVectorObjectVar(true /*is_header*/);
        properties_id_ = vobj->getId();
    } else {
        vobj = addr<VectorObject16>(properties_id_);
    }
    ediAssert(vobj != nullptr);
    vobj->push_back(obj_id);
    return properties_id_;
}

ObjectId SpecialNet::getPropertiesId() const { return properties_id_; }
}  // namespace db
}  // namespace open_edi
