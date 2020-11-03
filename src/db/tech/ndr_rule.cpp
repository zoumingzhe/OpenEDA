/**
 * @file  ndr_rule.cpp
 * @date  08/17/2020 01:51:59 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/tech/ndr_rule.h"

#include "db/core/db.h"
#include "db/util/vector_object_var.h"

namespace open_edi {
namespace db {

// Class NonDefaultRuleLayer
NonDefaultRuleLayer::NonDefaultRuleLayer() {
    width_ = 0;
    diag_width_ = 0;
    spacing_ = 0;
    wire_ext_ = 0;
    resistance_per_square_ = 0;
    capacitance_per_square_ = 0;
    edge_capacitance_ = 0;
    has_width_ = 0;
    has_diag_width_ = 0;
    has_spacing_ = 0;
    has_wire_ext_ = 0;
    has_resistance_per_square_ = 0;
    has_capacitance_per_square_ = 0;
    has_edge_capacitance_ = 0;
    name_index_ = -1;
}

NonDefaultRuleLayer &NonDefaultRuleLayer::operator=(
    NonDefaultRuleLayer const &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

NonDefaultRuleLayer &NonDefaultRuleLayer::operator=(
    NonDefaultRuleLayer &&rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

/// @brief copy constructor
NonDefaultRuleLayer::NonDefaultRuleLayer(NonDefaultRuleLayer const &rhs) {
    copy(rhs);
}

/// @brief move constructor
NonDefaultRuleLayer::NonDefaultRuleLayer(NonDefaultRuleLayer &&rhs) noexcept {
    move(std::move(rhs));
}

/// @brief summarize memory usage of the object in bytes
UInt32 NonDefaultRuleLayer::memory() const {
    UInt32 ret = 0;
    ret + sizeof(width_);
    ret + sizeof(diag_width_);
    ret + sizeof(spacing_);
    ret += sizeof(wire_ext_);
    ret += sizeof(has_width_);
    ret += sizeof(has_diag_width_);
    ret += sizeof(has_spacing_);
    ret += sizeof(has_wire_ext_);
    ret += sizeof(has_resistance_per_square_);
    ret += sizeof(has_capacitance_per_square_);
    ret += sizeof(has_edge_capacitance_);
    ret += sizeof(resistance_per_square_);
    ret += sizeof(capacitance_per_square_);
    ret += sizeof(edge_capacitance_);
    ret += sizeof(name_index_);

    return ret;
}

// Get:
const char *NonDefaultRuleLayer::getName() const {
    return getTopCell()->getSymbolByIndex(name_index_).c_str();
}

SymbolIndex NonDefaultRuleLayer::getNameIndex() const { return name_index_; }

UInt32 NonDefaultRuleLayer::getWidth() const { return width_; }

UInt32 NonDefaultRuleLayer::getDiagWidth() const { return diag_width_; }

UInt32 NonDefaultRuleLayer::getSpacing() const { return spacing_; }

UInt32 NonDefaultRuleLayer::getWireExt() const { return wire_ext_; }

float NonDefaultRuleLayer::getResistancePerSquare() const {
    return resistance_per_square_;
}

float NonDefaultRuleLayer::getCapacitancePerSquare() const {
    return capacitance_per_square_;
}

float NonDefaultRuleLayer::getEdgeCapacitance() const {
    return edge_capacitance_;
}

bool NonDefaultRuleLayer::hasWidth() const { return has_width_; }

bool NonDefaultRuleLayer::hasDiagWidth() const { return has_diag_width_; }

bool NonDefaultRuleLayer::hasSpacing() const { return has_spacing_; }

bool NonDefaultRuleLayer::hasWireExt() const { return has_wire_ext_; }

bool NonDefaultRuleLayer::hasResistancePerSquare() const {
    return has_resistance_per_square_;
}

bool NonDefaultRuleLayer::hasCapacitancePerSquare() const {
    return has_capacitance_per_square_;
}

bool NonDefaultRuleLayer::hasEdgeCapacitance() const {
    return has_edge_capacitance_;
}

// Set:
void NonDefaultRuleLayer::setName(const char *v) {
    int64_t index = getTopCell()->getOrCreateSymbol(v);
    if (index == -1) return;

    name_index_ = index;
    getTopCell()->addSymbolReference(name_index_, this->getId());
}

void NonDefaultRuleLayer::setWidth(UInt32 v) {
    width_ = v;
    has_width_ = true;
}

void NonDefaultRuleLayer::setDiagWidth(UInt32 v) {
    diag_width_ = v;
    has_diag_width_ = true;
}

void NonDefaultRuleLayer::setSpacing(UInt32 v) {
    spacing_ = v;
    has_spacing_ = true;
}

void NonDefaultRuleLayer::setWireExt(UInt32 v) {
    wire_ext_ = v;
    has_wire_ext_ = true;
}

void NonDefaultRuleLayer::setResistancePerSquare(float v) {
    resistance_per_square_ = v;
    has_resistance_per_square_ = true;
}

void NonDefaultRuleLayer::setCapacitancePerSquare(float v) {
    capacitance_per_square_ = v;
    has_capacitance_per_square_ = true;
}

void NonDefaultRuleLayer::setEdgeCapacitance(float v) {
    edge_capacitance_ = v;
    has_edge_capacitance_ = true;
}

/// @brief copy object
void NonDefaultRuleLayer::copy(NonDefaultRuleLayer const &rhs) {
    width_ = rhs.width_;
    diag_width_ = rhs.diag_width_;
    spacing_ = rhs.spacing_;
    wire_ext_ = rhs.wire_ext_;
    resistance_per_square_ = rhs.resistance_per_square_;
    capacitance_per_square_ = rhs.capacitance_per_square_;
    edge_capacitance_ = rhs.edge_capacitance_;
    has_width_ = rhs.has_width_;
    has_diag_width_ = rhs.has_diag_width_;
    has_spacing_ = rhs.has_spacing_;
    has_wire_ext_ = rhs.has_wire_ext_;
    has_resistance_per_square_ = rhs.has_resistance_per_square_;
    has_capacitance_per_square_ = rhs.has_capacitance_per_square_;
    has_edge_capacitance_ = rhs.has_edge_capacitance_;
    name_index_ = rhs.name_index_;
}

/// @brief move object
void NonDefaultRuleLayer::move(NonDefaultRuleLayer &&rhs) {
    width_ = std::move(rhs.width_);
    diag_width_ = std::move(rhs.diag_width_);
    spacing_ = std::move(rhs.spacing_);
    wire_ext_ = std::move(rhs.wire_ext_);
    resistance_per_square_ = std::move(rhs.resistance_per_square_);
    capacitance_per_square_ = std::move(rhs.capacitance_per_square_);
    edge_capacitance_ = std::move(rhs.edge_capacitance_);
    has_width_ = std::move(rhs.has_width_);
    has_diag_width_ = std::move(rhs.has_diag_width_);
    has_spacing_ = std::move(rhs.has_spacing_);
    has_wire_ext_ = std::move(rhs.has_wire_ext_);
    has_resistance_per_square_ = std::move(rhs.has_resistance_per_square_);
    has_capacitance_per_square_ = std::move(rhs.has_capacitance_per_square_);
    has_edge_capacitance_ = std::move(rhs.has_edge_capacitance_);
    name_index_ = std::move(rhs.name_index_);
}

void NonDefaultRuleLayer::printLEF(std::ofstream &ofs) const {
    Tech *lib = getTopCell()->getTechLib();
    ofs << "   LAYER " << getName() << "\n";
    if (hasWidth())
        ofs << "      WIDTH " << (lib->dbuToMicrons(getWidth())) << " ;\n";
    if (hasSpacing())
        ofs << "      SPACING " << (lib->dbuToMicrons(getSpacing())) << " ;\n";
    if (hasDiagWidth())
        ofs << "      DIAGWIDTH " << (lib->dbuToMicrons(getDiagWidth()))
            << " ;\n";
    if (hasWireExt())
        ofs << "      WIREEXTENSION " << (lib->dbuToMicrons(getWireExt()))
            << " ;\n";
    if (hasResistancePerSquare())
        ofs << "      RESISTANCE RPERSQ " << getResistancePerSquare() << " ;\n";
    if (hasCapacitancePerSquare())
        ofs << "      CAPACITANCE CPERSQDIST " << getCapacitancePerSquare()
            << " ;\n";
    if (hasEdgeCapacitance())
        ofs << "      EDGECAPACITANCE " << getEdgeCapacitance() << " ;\n";
    ofs << "   END " << getName() << "\n";
}

void NonDefaultRuleLayer::printDEF(FILE *fp) const {  // DEF out style print
    Tech *lib = getTopCell()->getTechLib();
    fprintf(fp, "\n  + LAYER %s", getName());
    if (hasWidth())
        fprintf(fp, "\n      WIDTH %g", (lib->dbuToMicrons(getWidth())));
    if (hasDiagWidth())
        fprintf(fp, "\n      DIAGWIDTH %g",
                (lib->dbuToMicrons(getDiagWidth())));
    if (hasSpacing())
        fprintf(fp, "\n      SPACING %g", (lib->dbuToMicrons(getSpacing())));
    if (hasWireExt())
        fprintf(fp, "\n      WIREEXT %g", (lib->dbuToMicrons(getWireExt())));
}

OStreamBase &operator<<(OStreamBase &os, NonDefaultRuleLayer const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");
    os << DataFieldName("name_index_") << rhs.name_index_ << DataDelimiter();

    os << DataFieldName("width_") << rhs.width_ << DataDelimiter();
    os << DataFieldName("diag_width_") << rhs.diag_width_ << DataDelimiter();
    os << DataFieldName("spacing_") << rhs.spacing_ << DataDelimiter();
    os << DataFieldName("wire_ext_") << rhs.wire_ext_ << DataDelimiter();
    os << DataFieldName("resistance_per_square_") << rhs.resistance_per_square_
       << DataDelimiter();
    os << DataFieldName("capacitance_per_square_")
       << rhs.capacitance_per_square_ << DataDelimiter();
    os << DataFieldName("edge_capacitance_") << rhs.edge_capacitance_
       << DataDelimiter();
    os << DataFieldName("has_width_") << rhs.has_width_ << DataDelimiter();
    os << DataFieldName("has_diag_width_") << rhs.has_diag_width_
       << DataDelimiter();
    os << DataFieldName("has_spacing_") << rhs.has_spacing_ << DataDelimiter();
    os << DataFieldName("has_wire_ext_") << rhs.has_wire_ext_
       << DataDelimiter();
    os << DataFieldName("has_resistance_per_square_")
       << rhs.has_resistance_per_square_ << DataDelimiter();
    os << DataFieldName("has_capacitance_per_square_")
       << rhs.has_capacitance_per_square_ << DataDelimiter();
    os << DataFieldName("has_edge_capacitance_") << rhs.has_edge_capacitance_;
    os << DataEnd(")");

    return os;
}

// Class NonDefaultRuleMinCuts
// @brief copy constructor
NonDefaultRuleMinCuts::NonDefaultRuleMinCuts(NonDefaultRuleMinCuts const &rhs) {
    copy(rhs);
}

// @brief move constructor
NonDefaultRuleMinCuts::NonDefaultRuleMinCuts(
    NonDefaultRuleMinCuts &&rhs) noexcept {
    move(std::move(rhs));
}

// @brief copy assignment
NonDefaultRuleMinCuts &NonDefaultRuleMinCuts::operator=(
    NonDefaultRuleMinCuts const &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

// @brief move constructor
NonDefaultRuleMinCuts &NonDefaultRuleMinCuts::operator=(
    NonDefaultRuleMinCuts &&rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

/// @brief copy object
void NonDefaultRuleMinCuts::copy(NonDefaultRuleMinCuts const &rhs) {
    num_cuts_ = rhs.num_cuts_;
    name_index_ = rhs.name_index_;
}
/// @brief move object
void NonDefaultRuleMinCuts::move(NonDefaultRuleMinCuts &&rhs) {
    num_cuts_ = std::move(rhs.num_cuts_);
    name_index_ = std::move(name_index_);
}

// @brief summarize memory usage of the object in bytes
UInt32 NonDefaultRuleMinCuts::memory() const {
    UInt32 ret = 0;
    ret += sizeof(num_cuts_);
    ret += sizeof(name_index_);
    return ret;
}

// Get:
const char *NonDefaultRuleMinCuts::getName() const {
    return getTopCell()->getSymbolByIndex(name_index_).c_str();
}

SymbolIndex NonDefaultRuleMinCuts::getNameIndex() const { return name_index_; }

uint32_t NonDefaultRuleMinCuts::getNumCuts() const { return num_cuts_; }

// Set:
void NonDefaultRuleMinCuts::setName(const char *v) {
    int64_t index = getTopCell()->getOrCreateSymbol(v);
    if (index == -1) return;

    name_index_ = index;
    getTopCell()->addSymbolReference(name_index_, this->getId());
}

void NonDefaultRuleMinCuts::setNumCuts(uint32_t v) { num_cuts_ = v; }

void NonDefaultRuleMinCuts::printLEF(std::ofstream &ofs) const {
    ofs << "   MINCUTS " << getName() << " " << getNumCuts() << " ;\n";
}

void NonDefaultRuleMinCuts::printDEF(FILE *fp) const {
    fprintf(fp, "\n  + MINCUTS %s %d", getName(), getNumCuts());
}

OStreamBase &operator<<(OStreamBase &os, NonDefaultRuleMinCuts const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");
    os << DataFieldName("name_index_") << rhs.name_index_ << DataDelimiter();
    os << DataFieldName("num_cuts_") << rhs.num_cuts_;
    os << DataEnd(")");

    return os;
}

// Class NonDefaultRule:
void NonDefaultRule::__init() {
    name_index_ = -1;
    hard_spacing_ = false;
    from_def_ = false;
    layers_ = 0;
    min_cuts_ = 0;
    properties_ = 0;
    use_vias_ = 0;
    use_via_rules_ = 0;
}

NonDefaultRule::NonDefaultRule() { __init(); }

NonDefaultRule::NonDefaultRule(Object *owner, UInt32 id) : BaseType(owner, id) {
    __init();
}

NonDefaultRule::NonDefaultRule(NonDefaultRule const &rhs) { copy(rhs); }

NonDefaultRule::NonDefaultRule(NonDefaultRule &&rhs) noexcept {
    move(std::move(rhs));
}

NonDefaultRule::~NonDefaultRule() {
    setLayerSize(0);
    setMinCutsSize(0);
    setViaSize(0);
    setUseViaSize(0);
    setUseViaRuleSize(0);
    setPropertySize(0);
}

NonDefaultRule &NonDefaultRule::operator=(NonDefaultRule const &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

NonDefaultRule &NonDefaultRule::operator=(NonDefaultRule &&rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void NonDefaultRule::copy(NonDefaultRule const &rhs) {
    this->BaseType::copy(rhs);
    setName(rhs.getName());
    hard_spacing_ = rhs.hard_spacing_;
    from_def_ = rhs.from_def_;
    layers_ = rhs.layers_;
    min_cuts_ = rhs.min_cuts_;
    properties_ = rhs.properties_;
    use_vias_ = rhs.use_vias_;
    use_via_rules_ = rhs.use_via_rules_;
}

void NonDefaultRule::move(NonDefaultRule &&rhs) {
    this->BaseType::move(std::move(rhs));
    name_index_ = std::move(rhs.name_index_);
    hard_spacing_ = std::move(rhs.hard_spacing_);
    from_def_ = std::move(rhs.from_def_);
    layers_ = std::move(rhs.layers_);
    min_cuts_ = std::move(rhs.min_cuts_);
    properties_ = std::move(rhs.properties_);
    use_vias_ = std::move(rhs.use_vias_);
    use_via_rules_ = std::move(rhs.use_via_rules_);
}

UInt32 NonDefaultRule::memory() const {
    UInt32 ret = this->BaseType::memory();
    ret += sizeof(name_index_) + sizeof(hard_spacing_) + sizeof(from_def_);
    ret += sizeof(ObjectId) *
           5;  // layers+min_cuts+property+use_vias+use_via_rules.
    VectorObject16 *vobj = addr<VectorObject16>(use_vias_);
    if (vobj) {
        ret += vobj->memory();
    }
    vobj = addr<VectorObject16>(use_via_rules_);
    if (vobj) {
        ret += vobj->memory();
    }
    vobj = addr<VectorObject16>(properties_);
    if (vobj) {
        ret += vobj->memory();
    }
    vobj = addr<VectorObject16>(layers_);
    if (vobj) {
        ret += vobj->memory();
    }
    vobj = addr<VectorObject16>(min_cuts_);
    if (vobj) {
        ret += vobj->memory();
    }
    return ret;
}

const char *NonDefaultRule::getName() const {
    return getTopCell()->getSymbolByIndex(name_index_).c_str();
}

SymbolIndex NonDefaultRule::getNameIndex() const { return name_index_; }

// Get:
bool NonDefaultRule::getHardSpacing() const { return hard_spacing_; }

bool NonDefaultRule::getFromDEF() const { return from_def_; }

uint64_t NonDefaultRule::numLayers() const {
    if (!layers_) return 0;

    return addr<VectorObject16>(layers_)->totalSize();
}

uint64_t NonDefaultRule::numMinCuts() const {
    if (!min_cuts_) return 0;

    return addr<VectorObject16>(min_cuts_)->totalSize();
}

uint64_t NonDefaultRule::numVias() const {
    if (!vias_) return 0;

    return addr<VectorObject8>(vias_)->totalSize();
}

uint64_t NonDefaultRule::numUseVias() const {
    if (!use_vias_) return 0;

    return addr<VectorObject16>(use_vias_)->totalSize();
}

uint64_t NonDefaultRule::numUseViaRules() const {
    if (!use_via_rules_) return 0;

    return addr<VectorObject16>(use_via_rules_)->totalSize();
}

uint64_t NonDefaultRule::numProperties() const {
    if (!properties_) return 0;

    return addr<VectorObject16>(properties_)->totalSize();
}

ObjectId NonDefaultRule::getLayersId() const { return layers_; }

ObjectId NonDefaultRule::getMinCutsId() const { return min_cuts_; }

ObjectId NonDefaultRule::getViasId() const { return vias_; }

ObjectId NonDefaultRule::getUseViasId() const { return use_vias_; }

ObjectId NonDefaultRule::getUseViaRulesId() const { return use_via_rules_; }

ObjectId NonDefaultRule::getPropertiesId() const { return properties_; }

// Set:
void NonDefaultRule::setName(const char *v) {
    int64_t index = getTopCell()->getOrCreateSymbol(v);
    if (index == -1) return;

    name_index_ = index;
    getTopCell()->addSymbolReference(name_index_, this->getId());
}

void NonDefaultRule::setLayerSize(uint64_t v) {
    if (v == 0) {
        if (layers_) {
            VectorObject16::deleteDBVectorObjectVar(layers_);
        }
        return;
    }
    if (!layers_) {
        VectorObject16 *vobj =
            VectorObject16::createDBVectorObjectVar(true /*is_header*/);
        ediAssert(vobj != nullptr);
        // using push_back to insert...remove reserve().
        // vobj->reserve(v);
        layers_ = vobj->getId();
    }
}

void NonDefaultRule::setMinCutsSize(uint64_t v) {
    if (v == 0) {
        if (min_cuts_) {
            VectorObject16::deleteDBVectorObjectVar(min_cuts_);
        }
        return;
    }
    if (!min_cuts_) {
        VectorObject16 *vobj =
            VectorObject16::createDBVectorObjectVar(true /*is_header*/);
        ediAssert(vobj != nullptr);
        // using push_back to insert...remove reserve().
        // vobj->reserve(v);
        min_cuts_ = vobj->getId();
    }
}

void NonDefaultRule::setViaSize(uint64_t v) {
    if (v == 0) {
        if (vias_) {
            VectorObject16::deleteDBVectorObjectVar(vias_);
        }
        return;
    }
    if (!vias_) {
        VectorObject16 *vobj =
            VectorObject16::createDBVectorObjectVar(true /*is_header*/);
        ediAssert(vobj != nullptr);
        // using push_back to insert...remove reserve().
        // vobj->reserve(v);
        vias_ = vobj->getId();
    }
}

void NonDefaultRule::setUseViaSize(uint64_t v) {
    if (v == 0) {
        if (use_vias_) {
            VectorObject16::deleteDBVectorObjectVar(use_vias_);
        }
        return;
    }
    if (!use_vias_) {
        VectorObject16 *vobj =
            VectorObject16::createDBVectorObjectVar(true /*is_header*/);
        ediAssert(vobj != nullptr);
        // using push_back to insert...remove reserve().
        // vobj->reserve(v);
        use_vias_ = vobj->getId();
    }
}

void NonDefaultRule::setUseViaRuleSize(uint64_t v) {
    if (v == 0) {
        if (use_via_rules_) {
            VectorObject16::deleteDBVectorObjectVar(use_via_rules_);
        }
        return;
    }
    if (!use_via_rules_) {
        VectorObject16 *vobj =
            VectorObject16::createDBVectorObjectVar(true /*is_header*/);
        ediAssert(vobj != nullptr);
        // using push_back to insert...remove reserve().
        // vobj->reserve(v);
        use_via_rules_ = vobj->getId();
    }
}

void NonDefaultRule::setPropertySize(uint64_t v) {
    if (v == 0) {
        if (properties_) {
            VectorObject16::deleteDBVectorObjectVar(properties_);
        }
        return;
    }
    if (!properties_) {
        VectorObject16 *vobj =
            VectorObject16::createDBVectorObjectVar(true /*is_header*/);
        ediAssert(vobj != nullptr);
        // using push_back to insert...remove reserve().
        // vobj->reserve(v);
        properties_ = vobj->getId();
    }
}

void NonDefaultRule::addLayer(ObjectId obj_id) {
    VectorObject16 *vobj = nullptr;
    if (obj_id == 0) return;
    NonDefaultRuleLayer *obj_data =
        addr<NonDefaultRuleLayer>(obj_id);
    if (obj_data == nullptr) {
        return;
    }
    obj_data->setOwner(this);

    if (layers_ == 0) {
        vobj = VectorObject16::createDBVectorObjectVar(true /*is_header*/);
        layers_ = vobj->getId();
    } else {
        vobj = addr<VectorObject16>(layers_);
    }
    ediAssert(vobj != nullptr);
    vobj->push_back(obj_id);
}

void NonDefaultRule::addMinCuts(ObjectId obj_id) {
    VectorObject16 *vobj = nullptr;
    if (obj_id == 0) return;
    NonDefaultRuleMinCuts *obj_data =
        addr<NonDefaultRuleMinCuts>(obj_id);
    if (obj_data == nullptr) {
        return;
    }
    obj_data->setOwner(this);

    if (min_cuts_ == 0) {
        vobj = VectorObject16::createDBVectorObjectVar(true /*is_header*/);
        min_cuts_ = vobj->getId();
    } else {
        vobj = addr<VectorObject16>(min_cuts_);
    }
    ediAssert(vobj != nullptr);
    vobj->push_back(obj_id);
}

void NonDefaultRule::addVia(ObjectId obj_id) {
    VectorObject8 *vobj = nullptr;
    if (obj_id == 0) return;

    if (vias_ == 0) {
        vobj = VectorObject8::createDBVectorObjectVar(true /*is_header*/);
        vias_ = vobj->getId();
    } else {
        vobj = addr<VectorObject8>(vias_);
    }
    ediAssert(vobj != nullptr);
    vobj->push_back(obj_id);
}

void NonDefaultRule::addUseVia(ObjectId obj_id) {
    VectorObject16 *vobj = nullptr;
    if (obj_id == 0) return;

    if (use_vias_ == 0) {
        vobj = VectorObject16::createDBVectorObjectVar(true /*is_header*/);
        use_vias_ = vobj->getId();
    } else {
        vobj = addr<VectorObject16>(use_vias_);
    }
    ediAssert(vobj != nullptr);
    vobj->push_back(obj_id);
}

void NonDefaultRule::addUseViaRule(ObjectId obj_id) {
    VectorObject16 *vobj = nullptr;
    if (obj_id == 0) return;

    if (use_via_rules_ == 0) {
        vobj = VectorObject16::createDBVectorObjectVar(true /*is_header*/);
        use_via_rules_ = vobj->getId();
    } else {
        vobj = addr<VectorObject16>(use_via_rules_);
    }
    ediAssert(vobj != nullptr);
    vobj->push_back(obj_id);
}

void NonDefaultRule::addProperty(ObjectId obj_id) {
    VectorObject16 *vobj = nullptr;
    if (obj_id == 0) return;

    if (properties_ == 0) {
        vobj = VectorObject16::createDBVectorObjectVar(true /*is_header*/);
        properties_ = vobj->getId();
    } else {
        vobj = addr<VectorObject16>(properties_);
    }
    ediAssert(vobj != nullptr);
    vobj->push_back(obj_id);
}

void NonDefaultRule::setHardSpacing(bool v) { hard_spacing_ = v; }

void NonDefaultRule::setFromDEF(bool v) { from_def_ = v; }

void NonDefaultRule::printLEF(std::ofstream &ofs) const {
    ofs << "\nNONDEFAULTRULE " << getName() << "\n";
    if (getHardSpacing()) {
        ofs << " HARDSPACING ;\n";
    }

    int i = 0;

    if (numLayers() > 0) {
        VectorObject16 *vobj = addr<VectorObject16>(layers_);
        for (i = 0; i < numLayers(); i++) {
            ObjectId obj_id = (*vobj)[i];
            NonDefaultRuleLayer *obj_data =
                addr<NonDefaultRuleLayer>(obj_id);
            if (obj_data == nullptr) continue;
            obj_data->printLEF(ofs);
        }
    }

    if (numVias() > 0) {
        VectorObject8 *vobj = addr<VectorObject8>(vias_);
        for (i = 0; i < numVias(); i++) {
            ObjectId obj_id = (*vobj)[i];
            ViaMaster *obj_data = addr<ViaMaster>(obj_id);
            if (obj_data == nullptr) continue;
            if (!obj_data->isFromNDR()) continue;
            obj_data->printLEF(ofs, 3);
        }
    }

    if (numUseVias() > 0) {
        VectorObject16 *vobj = addr<VectorObject16>(use_vias_);
        for (i = 0; i < numUseVias(); i++) {
            ObjectId obj_id = (*vobj)[i];
            ViaMaster *obj_data = addr<ViaMaster>(obj_id);
            if (obj_data == nullptr) continue;
            ofs << "   USEVIA " << obj_data->getName() << " ;\n";
        }
    }

    if (numUseViaRules() > 0) {
        VectorObject16 *vobj =
            addr<VectorObject16>(use_via_rules_);
        for (i = 0; i < numUseViaRules(); i++) {
            ObjectId obj_id = (*vobj)[i];
            ViaRule *obj_data = addr<ViaRule>(obj_id);
            if (obj_data == nullptr) continue;
            ofs << "   USEVIARULE " << obj_data->getName() << " ;\n";
        }
    }

    if (numMinCuts() > 0) {
        VectorObject16 *vobj = addr<VectorObject16>(min_cuts_);
        for (i = 0; i < numMinCuts(); i++) {
            ObjectId obj_id = (*vobj)[i];
            NonDefaultRuleMinCuts *obj_data =
                addr<NonDefaultRuleMinCuts>(obj_id);
            if (obj_data == nullptr) continue;
            obj_data->printLEF(ofs);
        }
    }

    if (numProperties() > 0) {
        ofs << "   PROPERTY";
        VectorObject16 *vobj =
            addr<VectorObject16>(properties_);
        for (i = 0; i < numProperties(); i++) {
            ObjectId obj_id = (*vobj)[i];
            Property *obj_data = addr<Property>(obj_id);
            if (obj_data == nullptr) continue;
            ofs << " ";
            obj_data->printLEF(ofs);
        }
        ofs << " ;\n";
    }
    ofs << "END " << getName() << "\n";
}

// DEF style print:
void NonDefaultRule::printDEF(FILE *fp) const {
    fprintf(fp, "- %s", getName());
    if (getHardSpacing()) {
        fprintf(fp, "\n  + HARDSPACING");
    }

    int i = 0;

    if (numLayers() > 0) {
        VectorObject16 *vobj = addr<VectorObject16>(layers_);
        for (i = 0; i < numLayers(); i++) {
            ObjectId obj_id = (*vobj)[i];
            if (!obj_id) continue;
            NonDefaultRuleLayer *obj_data =
                addr<NonDefaultRuleLayer>(obj_id);
            if (obj_data == nullptr) continue;
            obj_data->printDEF(fp);  // DEF-style doesn't print endl.
        }
    }

    if (numUseVias() > 0) {
        VectorObject16 *vobj = addr<VectorObject16>(use_vias_);
        for (i = 0; i < numUseVias(); i++) {
            ObjectId obj_id = (*vobj)[i];
            if (!obj_id) continue;
            ViaMaster *obj_data = addr<ViaMaster>(obj_id);
            if (obj_data == nullptr) continue;
            fprintf(fp, "\n  + VIA %s", obj_data->getName().c_str());
        }
    }

    if (numUseViaRules() > 0) {
        VectorObject16 *vobj =
            addr<VectorObject16>(use_via_rules_);
        for (i = 0; i < numUseViaRules(); i++) {
            ObjectId obj_id = (*vobj)[i];
            if (!obj_id) continue;
            ViaRule *obj_data = addr<ViaRule>(obj_id);
            if (obj_data == nullptr) continue;
            fprintf(fp, "\n  + VIARULE %s", obj_data->getName().c_str());
        }
    }

    if (numMinCuts() > 0) {
        VectorObject16 *vobj = addr<VectorObject16>(min_cuts_);
        for (i = 0; i < numMinCuts(); i++) {
            ObjectId obj_id = (*vobj)[i];
            if (!obj_id) continue;
            NonDefaultRuleMinCuts *obj_data =
                addr<NonDefaultRuleMinCuts>(obj_id);
            if (obj_data == nullptr) continue;
            obj_data->printDEF(fp);
        }
    }

    if (numProperties() > 0) {
        VectorObject16 *vobj =
            addr<VectorObject16>(properties_);
        for (i = 0; i < numProperties(); i++) {
            ObjectId obj_id = (*vobj)[i];
            if (!obj_id) continue;
            Property *obj_data = addr<Property>(obj_id);
            if (obj_data == nullptr) continue;
            obj_data->printDEF(fp);
        }
    }
    fprintf(fp, " ;\n");
}

// ostream overload function
OStreamBase &operator<<(OStreamBase &os, NonDefaultRule const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");
    NonDefaultRule::BaseType const &base = rhs;
    os << base << DataDelimiter();
    os << DataFieldName("name_index_") << rhs.getName() << DataDelimiter();
    os << DataFieldName("hard_spacing_") << rhs.hard_spacing_
       << DataDelimiter();
    os << DataFieldName("from_def_") << rhs.from_def_ << DataDelimiter();

    auto delimiter = DataDelimiter();
    os << DataFieldName("layers_") << rhs.getLayersId() << DataDelimiter();
    if (rhs.getLayersId()) {
        VectorObject16 *vobj =
            Object::addr<VectorObject16>(rhs.getLayersId());
        if (vobj != nullptr) {
            os << *vobj << DataDelimiter();
            os << DataFieldName("layers_detail") << DataBegin("[");
            // details:
            delimiter = DataDelimiter("");
            for (VectorObject16::iterator iter = vobj->begin();
                 iter != vobj->end(); ++iter) {
                ObjectId element_id = (*iter);
                NonDefaultRuleLayer *obj_data =
                    Object::addr<NonDefaultRuleLayer>(element_id);
                os << delimiter << (*obj_data);
                delimiter = DataDelimiter();
            }
            os << DataEnd("]");
        }
    }
    os << DataFieldName("min_cuts_") << rhs.getMinCutsId() << DataDelimiter();
    if (rhs.getMinCutsId()) {
        VectorObject16 *vobj =
            Object::addr<VectorObject16>(rhs.getMinCutsId());
        if (vobj != nullptr) {
            os << *vobj << DataDelimiter();
            os << DataFieldName("min_cuts_detail") << DataBegin("[");
            // details:
            delimiter = DataDelimiter("");
            for (VectorObject16::iterator iter = vobj->begin();
                 iter != vobj->end(); ++iter) {
                ObjectId element_id = (*iter);
                NonDefaultRuleMinCuts *obj_data =
                    Object::addr<NonDefaultRuleMinCuts>(element_id);
                os << delimiter << (*obj_data);
                delimiter = DataDelimiter();
            }
            os << DataEnd("]");
        }
    }
    os << DataFieldName("properties_") << rhs.getPropertiesId()
       << DataDelimiter();
    if (rhs.getPropertiesId()) {
        VectorObject16 *vobj =
            Object::addr<VectorObject16>(rhs.getPropertiesId());
        if (vobj != nullptr) {
            os << *vobj << DataDelimiter();
        }
    }
    os << DataFieldName("use_vias_") << rhs.getUseViasId() << DataDelimiter();
    if (rhs.getUseViasId()) {
        VectorObject16 *vobj =
            Object::addr<VectorObject16>(rhs.getUseViasId());
        if (vobj != nullptr) {
            os << *vobj << DataDelimiter();
        }
    }
    os << DataFieldName("use_via_rules_") << rhs.getUseViaRulesId()
       << DataDelimiter();
    if (rhs.getUseViaRulesId()) {
        VectorObject16 *vobj =
            Object::addr<VectorObject16>(rhs.getUseViaRulesId());
        if (vobj != nullptr) {
            os << *vobj << DataDelimiter();
        }
    }
    os << DataEnd(")");
    return os;
}

}  // namespace db
}  // namespace open_edi
