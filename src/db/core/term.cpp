/* @file  term.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "db/core/term.h"

#include "db/core/cell.h"
#include "db/core/db.h"
#include "db/util/symbol_table.h"
#include "db/util/vector_object_var.h"
#include "util/util.h"

namespace open_edi {
namespace db {

Term::Term() : Term::BaseType() {
    name_index_ = 0;
    ports_ = 0;
    has_range_ = false;
    antenna_partial_metal_areas_ = 0;
    antenna_partial_metal_side_areas_ = 0;
    antenna_partial_cut_areas_ = 0;
    antenna_diff_areas_ = 0;
    setDirection(SignalDirection::kUnknown);
    setPinType(SignalType::kUnknown);
}

Term::Term(Object *owner, Term::IndexType id) : Term::BaseType(owner, id) {
    setDirection(SignalDirection::kUnknown);
    setPinType(SignalType::kUnknown);
}

Term::Term(Term const &rhs) { copy(rhs); }

Term::Term(Term &&rhs) noexcept { move(std::move(rhs)); }

void Term::setName(std::string name) {
    Cell *top_cell = getOwnerCell();
    if (!top_cell) {
        message->issueMsg(kError,
                          "Cannot find top cell when set term name %s \n",
                          name.c_str());
        return;
    }
    name_index_ = top_cell->getOrCreateSymbol(name);
    top_cell->addSymbolReference(name_index_, this->getId());
}

std::string &Term::getName() const {
    Cell *top_cell = getOwnerCell();
    return top_cell->getSymbolTable()->getSymbolByIndex(name_index_);
}

Term &Term::operator=(Term const &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

Term &Term::operator=(Term &&rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void Term::copy(Term const &rhs) { this->BaseType::copy(rhs); }

void Term::move(Term &&rhs) { this->BaseType::move(std::move(rhs)); }

Term::IndexType Term::memory() const { return this->BaseType::memory(); }

OStreamBase &operator<<(OStreamBase &os, Term const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    Term::BaseType const &base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("name_") << rhs.getName();

    os << DataEnd(")");
    return os;
}

IStreamBase &operator>>(IStreamBase &is, Term &rhs) {
    is >> DataTypeName(className(rhs)) >> DataBegin("(");

    Term::BaseType &base = rhs;
    is >> base >> DataDelimiter();

    // read name_
    std::string name;
    is >> DataFieldName("name_") >> name;
    rhs.setName(name);

    is >> DataEnd(")");
    return is;
}

std::string const &Term::getTaperRule() const {
    Cell *top_cell = getOwnerCell();
    return top_cell->getSymbolTable()->getSymbolByIndex(taper_rule_index_);
}

void Term::setTaperRule(const char *v) {
    Cell *top_cell = getOwnerCell();
    if (!top_cell) {
        message->issueMsg(kError, "Cannot find top cell \n");
        return;
    }
    taper_rule_index_ = top_cell->getOrCreateSymbol(v);
    top_cell->addSymbolReference(taper_rule_index_, this->getId());
}

bool Term::hasDirection() const { 
    if (direction_ != SignalDirection::kUnknown) {
        return true;
    }
    return false;
}

void Term::setDirection(const char *v) {
    direction_ = toEnumByString<SignalDirection>(v);
}

void Term::setDirection(SignalDirection v) {
    direction_ = v;
}

std::string Term::getDirectionStr() const {
    return toString(direction_);
}

bool Term::hasUse() const { 
    if (type_ != SignalType::kUnknown) {
        return true;
    }
    return false;
}

void Term::setUse(const char *v) {
    setPinTypeStr(v);
}

std::string Term::getPinTypeStr() const {
    return toString(type_);
}

SignalType Term::getPinType() const {
    return type_;
}

void Term::setPinType(SignalType v) {
    type_ = v;
}

void Term::setPinTypeStr(const char *v) {
    type_ = toEnumByString<SignalType>(v);
}

bool Term::isPGType() const {
    if (type_ == SignalType::kPower || type_ == SignalType::kGround) {
        return true;
    }
    return false;
}

SymbolIndex Term::getNetExprIndex() const { return net_expr_index_; }

std::string const &Term::getNetExpr() const {
    Cell *top_cell = getOwnerCell();
    return top_cell->getSymbolTable()->getSymbolByIndex(net_expr_index_);
}

void Term::setNetExpr(const char *v) {
    Cell *top_cell = getOwnerCell();
    if (!top_cell) {
        message->issueMsg(kError, "Cannot find top cell \n");
        return;
    }
    net_expr_index_ = top_cell->getOrCreateSymbol(v);
    top_cell->addSymbolReference(net_expr_index_, this->getId());
}

SymbolIndex Term::getSupplySensitivityIndex() const {
    return supply_sensitivity_index_;
}

std::string const &Term::getSupplySensitivity() const {
    Cell *top_cell = getOwnerCell();
    return top_cell->getSymbolTable()->getSymbolByIndex(
        supply_sensitivity_index_);
}

void Term::setSupplySensitivity(const char *v) {
    Cell *top_cell = getOwnerCell();
    if (!top_cell) {
        message->issueMsg(kError, "Cannot find top cell \n");
        return;
    }
    supply_sensitivity_index_ = top_cell->getOrCreateSymbol(v);
    top_cell->addSymbolReference(supply_sensitivity_index_, this->getId());
}

SymbolIndex Term::getGroundSensitivityIndex() const {
    return ground_sensitivity_index_;
}

std::string const &Term::getGroundSensitivity() const {
    Cell *top_cell = getOwnerCell();
    return top_cell->getSymbolTable()->getSymbolByIndex(
        ground_sensitivity_index_);
}

void Term::setGroundSensitivity(const char *v) {
    Cell *top_cell = getOwnerCell();
    if (!top_cell) {
        message->issueMsg(kError, "Cannot find top cell \n");
        return;
    }
    ground_sensitivity_index_ = top_cell->getOrCreateSymbol(v);
    top_cell->addSymbolReference(ground_sensitivity_index_, this->getId());
}

std::string const &Term::getMustjoin() const {
    Cell *top_cell = getOwnerCell();
    return top_cell->getSymbolTable()->getSymbolByIndex(mustjoin_index_);
}

void Term::setMustjoin(const char *v) {
    Cell *top_cell = getOwnerCell();
    if (!top_cell) {
        message->issueMsg(kError, "Cannot find top cell \n");
        return;
    }
    mustjoin_index_ = top_cell->getOrCreateSymbol(v);
    top_cell->addSymbolReference(mustjoin_index_, this->getId());
}

std::string const &Term::getShape() const {
    Cell *top_cell = getOwnerCell();
    return top_cell->getSymbolTable()->getSymbolByIndex(shape_index_);
}

void Term::setShape(const char *v) {
    Cell *top_cell = getOwnerCell();
    if (!top_cell) {
        message->issueMsg(kError, "Cannot find top cell \n");
        return;
    }
    shape_index_ = top_cell->getOrCreateSymbol(v);
    top_cell->addSymbolReference(shape_index_, this->getId());
}

void Term::addAntennaModelTerm(int index, AntennaModelTerm *am) {
    memcpy(&antenna_models_[index], am, sizeof(AntennaModelTerm));
    antenna_models_[index].setValid(true);
}

void AntennaArea::setLayerName(const char *v) {
    Cell *top_cell = getOwnerCell();
    if (!top_cell) {
        message->issueMsg(kError,
                          "Cannot find top cell when set layer name %s \n", v);
        return;
    }
    layer_name_index_ = top_cell->getOrCreateSymbol(v);
    top_cell->addSymbolReference(layer_name_index_, this->getId());
}

std::string &AntennaArea::getLayerName() const {
    Cell *top_cell = getOwnerCell();
    return top_cell->getSymbolTable()->getSymbolByIndex(layer_name_index_);
}

void AntennaModelTerm::addAntennaGateArea(ObjectId aa) {
    VectorObject32 *vct = nullptr;
    if (antenna_gate_areas_ == 0) {
        Cell *top_cell = getOwnerCell();
        if (!top_cell) {
            message->issueMsg(
                kError, "Cannot find top cell when create vectorobject32.\n");
            return;
        }
        vct = top_cell->createVectorObject<VectorObject32>();
        if (vct == nullptr) return;
        antenna_gate_areas_ = vct->getId();
    } else {
        vct = Object::addr<VectorObject32>(antenna_gate_areas_);
    }
    if (vct) vct->push_back(aa);
}

AntennaArea *AntennaModelTerm::getAntennaGateArea(int index) const {
    VectorObject32 *vct = nullptr;
    if (antenna_gate_areas_ == 0)
        return nullptr;
    else
        vct = Object::addr<VectorObject32>(antenna_gate_areas_);
    if (vct) {
        AntennaArea *obj_data =
            Object::addr<AntennaArea>((*vct)[index]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

int AntennaModelTerm::getAntennaGateAreaNum() const {
    VectorObject32 *vct = nullptr;
    if (antenna_gate_areas_ == 0)
        return 0;
    else
        vct = Object::addr<VectorObject32>(antenna_gate_areas_);
    if (vct)
        return vct->totalSize();
    else
        return 0;
}

void AntennaModelTerm::addAntennaMaxAreaCar(ObjectId aa) {
    VectorObject32 *vct = nullptr;
    if (antenna_max_area_cars_ == 0) {
        Cell *top_cell = getOwnerCell();
        if (!top_cell) {
            message->issueMsg(
                kError, "Cannot find top cell when create vectorobject32.\n");
            return;
        }
        vct = top_cell->createVectorObject<VectorObject32>();
        if (vct == nullptr) return;
        antenna_max_area_cars_ = vct->getId();
    } else {
        vct = Object::addr<VectorObject32>(antenna_max_area_cars_);
    }
    if (vct) vct->push_back(aa);
}

AntennaArea *AntennaModelTerm::getAntennaMaxAreaCar(int index) const {
    VectorObject32 *vct = nullptr;
    if (antenna_max_area_cars_ == 0)
        return nullptr;
    else
        vct = Object::addr<VectorObject32>(antenna_max_area_cars_);
    if (vct) {
        AntennaArea *obj_data =
            Object::addr<AntennaArea>((*vct)[index]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

int AntennaModelTerm::getAntennaMaxAreaCarNum() const {
    VectorObject32 *vct = nullptr;
    if (antenna_max_area_cars_ == 0)
        return 0;
    else
        vct = Object::addr<VectorObject32>(antenna_max_area_cars_);
    if (vct)
        return vct->totalSize();
    else
        return 0;
}

void AntennaModelTerm::addAntennaMaxSideAreaCar(ObjectId aa) {
    VectorObject32 *vct = nullptr;
    if (antenna_max_side_area_cars_ == 0) {
        Cell *top_cell = getOwnerCell();
        if (!top_cell) {
            message->issueMsg(
                kError, "Cannot find top cell when create vectorobject32.\n");
            return;
        }
        vct = top_cell->createVectorObject<VectorObject32>();
        if (vct == nullptr) return;
        antenna_max_side_area_cars_ = vct->getId();
    } else {
        vct =
            Object::addr<VectorObject32>(antenna_max_side_area_cars_);
    }
    if (vct) vct->push_back(aa);
}

AntennaArea *AntennaModelTerm::getAntennaMaxSideAreaCar(int index) const {
    VectorObject32 *vct = nullptr;
    if (antenna_max_side_area_cars_ == 0)
        return nullptr;
    else
        vct =
            Object::addr<VectorObject32>(antenna_max_side_area_cars_);
    if (vct) {
        AntennaArea *obj_data =
            Object::addr<AntennaArea>((*vct)[index]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

int AntennaModelTerm::getAntennaMaxSideAreaCarNum() const {
    VectorObject32 *vct = nullptr;
    if (antenna_max_side_area_cars_ == 0)
        return 0;
    else
        vct =
           Object:: addr<VectorObject32>(antenna_max_side_area_cars_);
    if (vct)
        return vct->totalSize();
    else
        return 0;
}

void AntennaModelTerm::addAntennaMaxCutCar(ObjectId aa) {
    VectorObject32 *vct = nullptr;
    if (antenna_max_cut_cars_ == 0) {
        Cell *top_cell = getOwnerCell();
        if (!top_cell) {
            message->issueMsg(
                kError, "Cannot find top cell when create vectorobject32.\n");
            return;
        }
        vct = top_cell->createVectorObject<VectorObject32>();
        if (vct == nullptr) return;
        antenna_max_cut_cars_ = vct->getId();
    } else {
        vct = Object::addr<VectorObject32>(antenna_max_cut_cars_);
    }
    if (vct) vct->push_back(aa);
}

AntennaArea *AntennaModelTerm::getAntennaMaxCutCar(int index) const {
    VectorObject32 *vct = nullptr;
    if (antenna_max_cut_cars_ == 0)
        return nullptr;
    else
        vct = Object::addr<VectorObject32>(antenna_max_cut_cars_);
    if (vct) {
        AntennaArea *obj_data =
            Object::addr<AntennaArea>((*vct)[index]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

int AntennaModelTerm::getAntennaMaxCutCarNum() const {
    VectorObject32 *vct = nullptr;
    if (antenna_max_cut_cars_ == 0)
        return 0;
    else
        vct = Object::addr<VectorObject32>(antenna_max_cut_cars_);
    if (vct)
        return vct->totalSize();
    else
        return 0;
}

void Term::addAntennaPartialMetalArea(ObjectId aa) {
    VectorObject32 *vct = nullptr;
    if (antenna_partial_metal_areas_ == 0) {
        Cell *top_cell = getOwnerCell();
        if (!top_cell) {
            message->issueMsg(
                kError, "Cannot find top cell when create vectorobject32.\n");
            return;
        }
        vct = top_cell->createVectorObject<VectorObject32>();
        if (vct == nullptr) return;
        antenna_partial_metal_areas_ = vct->getId();
    } else {
        vct =
            addr<VectorObject32>(antenna_partial_metal_areas_);
    }
    if (vct) vct->push_back(aa);
}

AntennaArea *Term::getAntennaPartialMetalArea(int index) const {
    VectorObject32 *vct = nullptr;
    if (antenna_partial_metal_areas_ == 0)
        return nullptr;
    else
        vct =
            addr<VectorObject32>(antenna_partial_metal_areas_);
    if (vct) {
        AntennaArea *obj_data =
            addr<AntennaArea>((*vct)[index]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

int Term::getAntennaPartialMetalAreaNum() const {
    VectorObject32 *vct = nullptr;
    if (antenna_partial_metal_areas_ == 0)
        return 0;
    else
        vct =
            addr<VectorObject32>(antenna_partial_metal_areas_);
    if (vct)
        return vct->totalSize();
    else
        return 0;
}

void Term::addAntennaPartialMetalSideArea(ObjectId aa) {
    VectorObject32 *vct = nullptr;
    if (antenna_partial_metal_side_areas_ == 0) {
        Cell *top_cell = getOwnerCell();
        if (!top_cell) {
            message->issueMsg(
                kError, "Cannot find top cell when create vectorobject32.\n");
            return;
        }
        vct = top_cell->createVectorObject<VectorObject32>();
        if (vct == nullptr) return;
        antenna_partial_metal_side_areas_ = vct->getId();
    } else {
        vct = addr<VectorObject32>(
            antenna_partial_metal_side_areas_);
    }
    if (vct) vct->push_back(aa);
}

AntennaArea *Term::getAntennaPartialMetalSideArea(int index) const {
    VectorObject32 *vct = nullptr;
    if (antenna_partial_metal_side_areas_ == 0)
        return nullptr;
    else
        vct = addr<VectorObject32>(
            antenna_partial_metal_side_areas_);
    if (vct) {
        AntennaArea *obj_data =
            addr<AntennaArea>((*vct)[index]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

int Term::getAntennaPartialMetalSideAreaNum() const {
    VectorObject32 *vct = nullptr;
    if (antenna_partial_metal_side_areas_ == 0)
        return 0;
    else
        vct = addr<VectorObject32>(
            antenna_partial_metal_side_areas_);
    if (vct)
        return vct->totalSize();
    else
        return 0;
}

void Term::addAntennaPartialCutArea(ObjectId aa) {
    VectorObject32 *vct = nullptr;
    if (antenna_partial_cut_areas_ == 0) {
        Cell *top_cell = getOwnerCell();
        if (!top_cell) {
            message->issueMsg(
                kError, "Cannot find top cell when create vectorobject32.\n");
            return;
        }
        vct = top_cell->createVectorObject<VectorObject32>();
        if (vct == nullptr) return;
        antenna_partial_cut_areas_ = vct->getId();
    } else {
        vct = addr<VectorObject32>(antenna_partial_cut_areas_);
    }
    if (vct) vct->push_back(aa);
}

AntennaArea *Term::getAntennaPartialCutArea(int index) const {
    VectorObject32 *vct = nullptr;
    if (antenna_partial_cut_areas_ == 0)
        return nullptr;
    else
        vct = addr<VectorObject32>(antenna_partial_cut_areas_);
    if (vct) {
        AntennaArea *obj_data =
            addr<AntennaArea>((*vct)[index]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

int Term::getAntennaPartialCutAreaNum() const {
    VectorObject32 *vct = nullptr;
    if (antenna_partial_cut_areas_ == 0)
        return 0;
    else
        vct = addr<VectorObject32>(antenna_partial_cut_areas_);
    if (vct)
        return vct->totalSize();
    else
        return 0;
}

void Term::addAntennaDiffArea(ObjectId aa) {
    VectorObject32 *vct = nullptr;
    if (antenna_diff_areas_ == 0) {
        Cell *top_cell = getOwnerCell();
        if (!top_cell) {
            message->issueMsg(
                kError, "Cannot find top cell when create vectorobject32.\n");
            return;
        }
        vct = top_cell->createVectorObject<VectorObject32>();
        if (vct == nullptr) return;
        antenna_diff_areas_ = vct->getId();
    } else {
        vct = addr<VectorObject32>(antenna_diff_areas_);
    }
    if (vct) vct->push_back(aa);
}

AntennaArea *Term::getAntennaDiffArea(int index) const {
    VectorObject32 *vct = nullptr;
    if (antenna_diff_areas_ == 0)
        return nullptr;
    else
        vct = addr<VectorObject32>(antenna_diff_areas_);
    if (vct) {
        AntennaArea *obj_data =
            addr<AntennaArea>((*vct)[index]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

int Term::getAntennaDiffAreaNum() const {
    VectorObject32 *vct = nullptr;
    if (antenna_diff_areas_ == 0)
        return 0;
    else
        vct = addr<VectorObject32>(antenna_diff_areas_);
    if (vct)
        return vct->totalSize();
    else
        return 0;
}

void Term::addPort(ObjectId p) {
    VectorObject32 *vct = nullptr;
    if (ports_ == 0) {
        Cell *top_cell = getOwnerCell();
        if (!top_cell) {
            message->issueMsg(
                kError, "Cannot find top cell when create vectorobject32.\n");
            return;
        }
        vct = top_cell->createVectorObject<VectorObject32>();
        if (vct == nullptr) return;
        ports_ = vct->getId();
    } else {
        vct = addr<VectorObject32>(ports_);
    }
    if (vct) vct->push_back(p);
}

Port *Term::getPort(int index) const {
    VectorObject32 *vct = nullptr;
    if (ports_ == 0)
        return nullptr;
    else
        vct = addr<VectorObject32>(ports_);
    if (vct) {
        Port *obj_data = addr<Port>((*vct)[index]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

int Term::getPortNum() const {
    VectorObject32 *vct = nullptr;
    if (ports_ == 0)
        return 0;
    else
        vct = addr<VectorObject32>(ports_);
    if (vct)
        return vct->totalSize();
    else
        return 0;
}

AntennaModelTerm::AntennaModelTerm() {
    antenna_gate_areas_ = 0;
    antenna_max_area_cars_ = 0;
    antenna_max_side_area_cars_ = 0;
    antenna_max_cut_cars_ = 0;
}

void Term::print() const {
    Tech *lib = getOwnerCell()->getTechLib();
    message->info("   PIN %s ;\n", getName().c_str());
    if (taper_rule_index_ != 0)
        message->info("      TAPERRULE %s ;\n", getTaperRule().c_str());
    std::string str_value;
    if (hasDirection()) {
        str_value.assign(getDirectionStr());
        toUpper(str_value);
        message->info("      DIRECTION %s ;\n", str_value.c_str());
    }
    if (hasUse()) {
        str_value.assign(getPinTypeStr());
        toUpper(str_value);      
        message->info("      USE %s ;\n", str_value.c_str());
    }
    if (net_expr_index_ != 0)
        message->info("      NETEXPR %s ;\n", getNetExpr().c_str());
    if (supply_sensitivity_index_ != 0)
        message->info("      SUPPLYSENSITIVITY %s ;\n",
                      getSupplySensitivity().c_str());
    if (ground_sensitivity_index_ != 0)
        message->info("      GROUNDSENSITIVITY %s ;\n",
                      getGroundSensitivity().c_str());
    if (shape_index_ != 0)
        message->info("      SHAPE %s ;\n", getShape().c_str());
    if (mustjoin_index_ != 0)
        message->info("      MUSTJOIN %s ;\n", getMustjoin().c_str());

    if (getPortNum() > 0) {
        for (int i = 0; i < getPortNum(); i++) {
            Port *p = getPort(i);
            message->info("      PORT\n");
            if (p->getLayerGeometryNum() > 0) {
                for (int j = 0; j < p->getLayerGeometryNum(); j++) {
                    LayerGeometry *lg = p->getLayerGeometry(j);
                    lg->print();
                }
            }
        }
    }

    if (getAntennaPartialMetalAreaNum() > 0) {
        for (int i = 0; i < getAntennaPartialMetalAreaNum(); i++) {
            AntennaArea *a = getAntennaPartialMetalArea(i);
            message->info("      ANTENNAPARTIALMETALAREA %g ",
                          lib->areaDBUToMicrons(a->getArea()));
            if (a->getLayerNameID())
                message->info("LAYER %s ", a->getLayerName().c_str());
            message->info(";\n");
        }
    }
    if (getAntennaPartialMetalSideAreaNum() > 0) {
        for (int i = 0; i < getAntennaPartialMetalSideAreaNum(); i++) {
            AntennaArea *a = getAntennaPartialMetalSideArea(i);
            message->info("      ANTENNAPARTIALMETALSIDEAREA %g ",
                          lib->areaDBUToMicrons(a->getArea()));
            if (a->getLayerNameID())
                message->info("LAYER %s ", a->getLayerName().c_str());
            message->info(";\n");
        }
    }
    if (getAntennaPartialCutAreaNum() > 0) {
        for (int i = 0; i < getAntennaPartialCutAreaNum(); i++) {
            AntennaArea *a = getAntennaPartialCutArea(i);
            message->info("      ANTENNAPARTIALCUTAREA %g ",
                          lib->areaDBUToMicrons(a->getArea()));
            if (a->getLayerNameID())
                message->info("LAYER %s ", a->getLayerName().c_str());
            message->info(";\n");
        }
    }
    if (getAntennaDiffAreaNum() > 0) {
        for (int i = 0; i < getAntennaDiffAreaNum(); i++) {
            AntennaArea *a = getAntennaDiffArea(i);
            message->info("      ANTENNADIFFAREA %g ",
                          lib->areaDBUToMicrons(a->getArea()));
            if (a->getLayerNameID())
                message->info("LAYER %s ", a->getLayerName().c_str());
            message->info(";\n");
        }
    }

    for (int i = 0; i < 6; i++) {
        const AntennaModelTerm *am = GetAntennaModelTerm(i);
        if (am->getValid()) {
            message->info("      ANTENNAMODEL OXIDE%d ;\n", i);
            am->print();
        }
    }

    message->info("   END %s\n", getName().c_str());
}

void AntennaModelTerm::print() const {
    Tech *lib = getOwnerCell()->getTechLib();
    if (getAntennaGateAreaNum() > 0) {
        for (int i = 0; i < getAntennaGateAreaNum(); i++) {
            AntennaArea *a = getAntennaGateArea(i);
            message->info("      ANTENNAGATEAREA %g ",
                          lib->areaDBUToMicrons(a->getArea()));
            if (a->getLayerNameID())
                message->info("LAYER %s ", a->getLayerName().c_str());
            message->info(";\n");
        }
    }

    if (getAntennaMaxAreaCarNum() > 0) {
        for (int i = 0; i < getAntennaMaxAreaCarNum(); i++) {
            AntennaArea *a = getAntennaMaxAreaCar(i);
            message->info("      ANTENNAMAXAREACAR %g ",
                          lib->areaDBUToMicrons(a->getArea()));
            if (a->getLayerNameID())
                message->info("LAYER %s ", a->getLayerName().c_str());
            message->info(";\n");
        }
    }

    if (getAntennaMaxSideAreaCarNum() > 0) {
        for (int i = 0; i < getAntennaMaxSideAreaCarNum(); i++) {
            AntennaArea *a = getAntennaMaxSideAreaCar(i);
            message->info("      ANTENNAMAXSIDEAREACAR %g ",
                          lib->areaDBUToMicrons(a->getArea()));
            if (a->getLayerNameID())
                message->info("LAYER %s ", a->getLayerName().c_str());
            message->info(";\n");
        }
    }

    if (getAntennaMaxCutCarNum() > 0) {
        for (int i = 0; i < getAntennaMaxCutCarNum(); i++) {
            AntennaArea *a = getAntennaMaxCutCar(i);
            message->info("      ANTENNAMAXCUTCAR %g ",
                          lib->areaDBUToMicrons(a->getArea()));
            if (a->getLayerNameID())
                message->info("LAYER %s ", a->getLayerName().c_str());
            message->info(";\n");
        }
    }
}

Port::Port() {
    is_real_ = true;
    has_placement_ = false;
    location_.set(0, 0);
    class_index_ = -1;
    status_ = PlaceStatus::kUnknown;
    orient_ = Orient::kUnknown;
}

Port::~Port() {}

bool Port::getHasPlacement() const { return has_placement_; }

void Port::setHasPlacement(bool flag) { has_placement_ = flag; }

PlaceStatus Port::getStatus() const { return status_; }

void Port::setStatus(PlaceStatus s) { status_ = s; }

Point Port::getLocation() const { return location_; }

void Port::setLocation(Point &p) { location_ = p; }

Orient Port::getOrient() const { return orient_; }

void Port::setOrient(Orient o) { orient_ = o; }

void Port::setClass(const char *v) {
    Cell *top_cell = getOwnerCell();
    if (!top_cell) {
        message->issueMsg(kError,
                          "Cannot find top cell when set term name %s \n", v);
        return;
    }
    class_index_ = top_cell->getOrCreateSymbol(v);
    top_cell->addSymbolReference(class_index_, this->getId());
}

std::string &Port::getClass() const {
    Cell *top_cell = getOwnerCell();
    return top_cell->getSymbolTable()->getSymbolByIndex(class_index_);
}

void Port::addLayerGeometry(ObjectId v) {
    VectorObject32 *vct = nullptr;
    if (layer_geometries_ == 0) {
        Cell *top_cell = getOwnerCell();
        if (!top_cell) {
            message->issueMsg(
                kError, "Cannot find top cell when create vectorobject32.\n");
            return;
        }
        vct = top_cell->createVectorObject<VectorObject32>();
        if (vct == nullptr) return;
        layer_geometries_ = vct->getId();
    } else {
        vct = addr<VectorObject32>(layer_geometries_);
    }
    if (vct) vct->push_back(v);
}

LayerGeometry *Port::getLayerGeometry(int index) const {
    VectorObject32 *vct = nullptr;
    if (layer_geometries_ == 0)
        return nullptr;
    else
        vct = addr<VectorObject32>(layer_geometries_);
    if (vct) {
        LayerGeometry *obj_data =
            addr<LayerGeometry>((*vct)[index]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

int Port::getLayerGeometryNum() const {
    VectorObject32 *vct = nullptr;
    if (layer_geometries_ == 0)
        return 0;
    else
        vct = addr<VectorObject32>(layer_geometries_);
    if (vct)
        return vct->totalSize();
    else
        return 0;
}

void Term::printLEF(std::ofstream &ofs) const {
    Tech *lib = getOwnerCell()->getTechLib();
    ofs << "   PIN " << getName().c_str() << "\n";
    if (taper_rule_index_ != 0)
        ofs << "      TAPERRULE " << getTaperRule().c_str() << " ;\n";
    std::string str_value = "temp";
    if (hasDirection()) {
        str_value.assign(getDirectionStr());
        toUpper(str_value);
        ofs << "      DIRECTION " << str_value << " ;\n";
    }
    if (hasUse()) {
        str_value.assign(getPinTypeStr());
        toUpper(str_value); 
        ofs << "      USE " << str_value << " ;\n";
    }    
    if (net_expr_index_ != 0)
        ofs << "      NETEXPR \"" << getNetExpr().c_str() << "\" ;\n";
    if (supply_sensitivity_index_ != 0)
        ofs << "      SUPPLYSENSITIVITY " << getSupplySensitivity().c_str()
            << " ;\n";
    if (ground_sensitivity_index_ != 0)
        ofs << "      GROUNDSENSITIVITY " << getGroundSensitivity().c_str()
            << " ;\n";
    if (shape_index_ != 0)
        ofs << "      SHAPE " << getShape().c_str() << " ;\n";
    if (mustjoin_index_ != 0)
        ofs << "      MUSTJOIN " << getMustjoin().c_str() << " ;\n";

    if (getPortNum() > 0) {
        for (int i = 0; i < getPortNum(); i++) {
            Port *p = getPort(i);
            ofs << "      PORT\n";
            if (p->getClass().size() > 0) {
                ofs << "         CLASS"
                    << " " << p->getClass().c_str() << " ;\n";
            }
            if (p->getLayerGeometryNum() > 0) {
                for (int j = 0; j < p->getLayerGeometryNum(); j++) {
                    LayerGeometry *lg = p->getLayerGeometry(j);
                    lg->printLEF(ofs, true);
                }
            }
            ofs << "      END\n";
        }
    }

    if (getAntennaPartialMetalAreaNum() > 0) {
        for (int i = 0; i < getAntennaPartialMetalAreaNum(); i++) {
            AntennaArea *a = getAntennaPartialMetalArea(i);
            ofs << "      ANTENNAPARTIALMETALAREA "
                << lib->areaDBUToMicrons(a->getArea());
            if (a->getLayerNameID() >= 0)
                ofs << " LAYER " << a->getLayerName().c_str();
            ofs << " ;\n";
        }
    }
    if (getAntennaPartialMetalSideAreaNum() > 0) {
        for (int i = 0; i < getAntennaPartialMetalSideAreaNum(); i++) {
            AntennaArea *a = getAntennaPartialMetalSideArea(i);
            ofs << "      ANTENNAPARTIALMETALSIDEAREA "
                << lib->areaDBUToMicrons(a->getArea());
            if (a->getLayerNameID() >= 0)
                ofs << " LAYER " << a->getLayerName().c_str();
            ofs << " ;\n";
        }
    }
    if (getAntennaPartialCutAreaNum() > 0) {
        for (int i = 0; i < getAntennaPartialCutAreaNum(); i++) {
            AntennaArea *a = getAntennaPartialCutArea(i);
            ofs << "      ANTENNAPARTIALCUTAREA "
                << lib->areaDBUToMicrons(a->getArea());
            if (a->getLayerNameID() >= 0)
                ofs << " LAYER " << a->getLayerName().c_str();
            ofs << " ;\n";
        }
    }
    if (getAntennaDiffAreaNum() > 0) {
        for (int i = 0; i < getAntennaDiffAreaNum(); i++) {
            AntennaArea *a = getAntennaDiffArea(i);
            ofs << "      ANTENNADIFFAREA "
                << lib->areaDBUToMicrons(a->getArea());
            if (a->getLayerNameID() >= 0)
                ofs << " LAYER " << a->getLayerName().c_str();
            ofs << " ;\n";
        }
    }

    for (int i = 0; i < 6; i++) {
        const AntennaModelTerm *am = GetAntennaModelTerm(i);
        if (am->getValid()) {
            ofs << "      ANTENNAMODEL OXIDE" << i << " ;\n";
            am->printLEF(ofs);
        }
    }

    ofs << "   END " << getName().c_str() << "\n";
}

void AntennaModelTerm::printLEF(std::ofstream &ofs) const {
    Tech *lib = getOwnerCell()->getTechLib();
    if (getAntennaGateAreaNum() > 0) {
        for (int i = 0; i < getAntennaGateAreaNum(); i++) {
            AntennaArea *a = getAntennaGateArea(i);
            ofs << "      ANTENNAGATEAREA "
                << lib->areaDBUToMicrons(a->getArea());
            if (a->getLayerNameID() >= 0)
                ofs << " LAYER " << a->getLayerName().c_str();
            ofs << " ;\n";
        }
    }

    if (getAntennaMaxAreaCarNum() > 0) {
        for (int i = 0; i < getAntennaMaxAreaCarNum(); i++) {
            AntennaArea *a = getAntennaMaxAreaCar(i);
            ofs << "      ANTENNAMAXAREACAR "
                << lib->areaDBUToMicrons(a->getArea());
            if (a->getLayerNameID() >= 0)
                ofs << " LAYER " << a->getLayerName().c_str();
            ofs << " ;\n";
        }
    }

    if (getAntennaMaxSideAreaCarNum() > 0) {
        for (int i = 0; i < getAntennaMaxSideAreaCarNum(); i++) {
            AntennaArea *a = getAntennaMaxSideAreaCar(i);
            ofs << "      ANTENNAMAXSIDEAREACAR "
                << lib->areaDBUToMicrons(a->getArea());
            if (a->getLayerNameID() >= 0)
                ofs << " LAYER " << a->getLayerName().c_str();
            ofs << " ;\n";
        }
    }

    if (getAntennaMaxCutCarNum() > 0) {
        for (int i = 0; i < getAntennaMaxCutCarNum(); i++) {
            AntennaArea *a = getAntennaMaxCutCar(i);
            ofs << "      ANTENNAMAXCUTCAR "
                << lib->areaDBUToMicrons(a->getArea());
            if (a->getLayerNameID() >= 0)
                ofs << " LAYER " << a->getLayerName().c_str();
            ofs << " ;\n";
        }
    }
}

}  // namespace db
}  // namespace open_edi
