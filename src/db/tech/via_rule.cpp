/* @file  via_rule.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/tech/via_rule.h"

#include "db/core/db.h"
#include "db/util/array.h"

namespace open_edi {
namespace db {
using IdArray = ArrayObject<ObjectId>;

// VIARULE
ViaRule::ViaRule() {
    is_generate_ = 0;
    is_default_ = 0;
    has_property_ = 0;
    name_index_ = 0;
    metal_layers_ = 0;
    cut_layer_ = 0;
    via_masters_ = 0;
    properties_ = 0;
}
ViaRule::~ViaRule() {}
/**
 * @brief Get the Name Index object
 *
 * @return SymbolIndex
 */
SymbolIndex ViaRule::getNameIndex() const { return name_index_; }

/**
 * @brief Set the Name object
 *
 * @param name
 */
bool ViaRule::setName(std::string const &name) {
    SymbolIndex index = getTechLib()->getOrCreateSymbol(name.c_str());
    if (index == kInvalidSymbolIndex) return false;

    name_index_ = index;
    getTechLib()->addSymbolReference(name_index_, this->getId());
    return true;
}
/**
 * @brief Get the Name object
 *
 * @return const char*
 */
std::string const &ViaRule::getName() {
    return getTechLib()->getSymbolByIndex(name_index_);
}

/**
 * @brief getIsGenerate
 * get if it is VIARULE GENERATE rule
 *
 * @return
 */
bool ViaRule::getIsGenerate() const { return is_generate_; }

/**
 * @brief setIsGenerate
 * set if it is VIARULE GENERATE rule
 *
 * @param flag
 */
void ViaRule::setIsGenerate(bool flag) { is_generate_ = flag; }

/**
 * @brief getIsGenerate
 * get if it is VIARULE GENERATE DEFAULT rule
 *
 * @return
 */
bool ViaRule::getIsDefault() const { return is_default_; }

/**
 * @brief setIsGenerate
 * set if it is VIARULE GENERATE DEFAULT rule
 *
 * @param flag
 */
void ViaRule::setIsDefault(bool flag) { is_default_ = flag; }

/**
 * @brief get Num of properties
 *
 * @return uint64_t
 */
uint64_t ViaRule::numProperties() const {
    if (!properties_) return 0;

    return addr<IdArray>(properties_)->getSize();
}

/**
 * @brief set properties size
 *
 * @param v
 */
void ViaRule::setPropertySize(uint64_t v) {
    if (v == 0) {
        if (properties_) {
            __deleteObjectIdArray(properties_);
        }
        return;
    }
    if (!properties_) {
        properties_ = __createObjectIdArray(16);
    }
}

/**
 * @brief add properties
 *
 * @param obj_id
 */
void ViaRule::addProperty(ObjectId obj_id) {
    IdArray *vobj = nullptr;
    if (obj_id == 0) return;

    if (properties_ == 0) {
        properties_ = __createObjectIdArray(16);
        vobj = addr<IdArray>(properties_);
    } else {
        vobj = addr<IdArray>(properties_);
    }
    ediAssert(vobj != nullptr);
    vobj->pushBack(obj_id);
}

ObjectId ViaRule::getPropertiesId() const { return properties_; }
/**
 * @brief getHasProperty
 * get if VIARULE has property definition
 *
 * @return
 */
bool ViaRule::getHasProperty() const { return has_property_; }

/**
 * @brief setHasProperty
 * set if VIARULE has property definition
 *
 * @return
 */
void ViaRule::setHasProperty(bool flag) { has_property_ = flag; }

/**
 * @brief getViaRuleCutLayer
 * get cut layer definition of VIARULE GENERATE rule
 *
 * @return
 */
ViaRuleCutLayer *ViaRule::getViaRuleCutLayer() {
    ViaRuleCutLayer *cut_layer =
        addr<ViaRuleCutLayer>(cut_layer_);
    return cut_layer;
}

ViaRuleCutLayer *ViaRule::creatViaRuleCutLayer() {
    ViaRuleCutLayer *cut_layer = Object::createObject<ViaRuleCutLayer>(
        kObjectTypeViaMaster, getTechLib()->getId());
    cut_layer_ = cut_layer->getId();

    return cut_layer;
}

ViaRuleMetalLayer *ViaRule::creatViaRuleMetalLayer() {
    ViaRuleMetalLayer *metal_layer = Object::createObject<ViaRuleMetalLayer>(
        kObjectTypeViaMaster, getTechLib()->getId());

    return metal_layer;
}

/**
 * @brief setViaRuleCutLayer
 * set cut layer definition of VIARULE GENERATE rule
 *
 * @param via_rule
 */
void ViaRule::setViaRuleCutLayer(ViaRuleCutLayer *via_rule) {
    cut_layer_ = via_rule->getId();
}

/**
 * @brief add metal layer
 *
 * @param via_master
 */
void ViaRule::addViaRuleMetalLayer(ViaRuleMetalLayer *metal_layer) {
    IdArray *metal_layer_vector = nullptr;
    if (metal_layers_ == 0) {
        metal_layers_ = __createObjectIdArray(64);
    }
    if (metal_layers_)
        metal_layer_vector =
            addr<IdArray>(metal_layers_);
    if (metal_layer_vector) metal_layer_vector->pushBack(metal_layer->getId());
}

/**
 * @brief addViaMaster
 * add via defined in VIARULE rule
 *
 * @param via_master_name
 */
void ViaRule::addViaMaster(ViaMaster *via_master) {
    IdArray *via_master_vector = nullptr;
    if (via_masters_ == 0) {
        via_masters_ = __createObjectIdArray(64);
    }
    if (via_masters_)
        via_master_vector = addr<IdArray>(via_masters_);
    if (via_master_vector) via_master_vector->pushBack(via_master->getId());
}

/**
 * @brief print out
 *
 * @param is_generate_
 */
void ViaRule::print() {
    if (!is_generate_) {
        message->info("\nVIARULE %s ", getName().c_str());
    } else {
        message->info("\nVIARULE %s GENERATE", getName().c_str());
        if (is_default_) message->info(" DEFAULT");
    }
    if (metal_layers_) {
        IdArray *metal_layer_vector =
            addr<IdArray>(metal_layers_);
        for (IdArray::iterator iter = metal_layer_vector->begin();
             iter != metal_layer_vector->end(); ++iter) {
            ObjectId id = (*iter);
            ViaRuleMetalLayer *metal_layer =
                addr<ViaRuleMetalLayer>(id);
            if (metal_layer) metal_layer->print(is_generate_);
        }
    }

    if (cut_layer_) {
        ViaRuleCutLayer *cut_layer =
            addr<ViaRuleCutLayer>(cut_layer_);
        if (cut_layer) cut_layer->print();
    }

    if (via_masters_) {
        IdArray *via_master_vector =
            addr<IdArray>(via_masters_);
        for (IdArray::iterator iter = via_master_vector->begin();
             iter != via_master_vector->end(); ++iter) {
            ObjectId id = (*iter);
            ViaMaster *via_master = addr<ViaMaster>(id);
            if (via_master)
                message->info("\n VIA %s ;\n", via_master->getName().c_str());
        }
    }

    message->info("\nEND %s\n", getName().c_str());
}

/**
 * @brief print out
 *
 * @param is_generate_
 */
void ViaRule::printLEF(std::ofstream &ofs) {
    if (!is_generate_) {
        ofs << "\nVIARULE " << getName().c_str() << " ";
    } else {
        ofs << "\nVIARULE " << getName().c_str() << " GENERATE";
        if (is_default_) ofs << " DEFAULT";
    }
    if (metal_layers_) {
        IdArray *metal_layer_vector =
            addr<IdArray>(metal_layers_);
        for (IdArray::iterator iter = metal_layer_vector->begin();
             iter != metal_layer_vector->end(); ++iter) {
            ObjectId id = (*iter);
            ViaRuleMetalLayer *metal_layer =
                addr<ViaRuleMetalLayer>(id);
            if (metal_layer) metal_layer->printLEF(ofs, is_generate_);
        }
    }

    if (cut_layer_) {
        ViaRuleCutLayer *cut_layer =
            addr<ViaRuleCutLayer>(cut_layer_);
        if (cut_layer) cut_layer->printLEF(ofs);
    }

    if (via_masters_) {
        IdArray *via_master_vector =
            addr<IdArray>(via_masters_);
        for (IdArray::iterator iter = via_master_vector->begin();
             iter != via_master_vector->end(); ++iter) {
            ObjectId id = (*iter);
            ViaMaster *via_master = addr<ViaMaster>(id);
            if (via_master)
                ofs << "\n VIA " << via_master->getName().c_str() << " ;";
        }
    }

    if (numProperties() > 0) {
        ofs << "\n     PROPERTY";
        IdArray *vobj =
            addr<IdArray>(properties_);
        for (int i = 0; i < numProperties(); i++) {
            ObjectId obj_id = (*vobj)[i];
            Property *obj_data = addr<Property>(obj_id);
            if (obj_data == nullptr) continue;
            ofs << " ";
            obj_data->printLEF(ofs);
        }
        ofs << " ;";
    }

    ofs << "\nEND " << getName().c_str() << "\n";
}

// ViaRuleMetalLayer

ViaRuleMetalLayer::ViaRuleMetalLayer() {
    has_width_ = 0;
    has_direction_ = 0;
    has_enclosure_ = 0;
    is_horz_ = 0;
    min_width_ = 0;
    max_width_ = 0;
    overhang1_ = 0;
    overhang2_ = 0;
    layer_id_ = 0;
}
/**
 * @brief getHasWidth
 * get if metal/routing layer has WIDTH definition of VIARULE(or GENERATE) rule
 *
 * @return
 */
bool ViaRuleMetalLayer::getHasWidth() const { return has_width_; }

/**
 * @brief setHasWidth
 * set if metal/routing layer has WIDTH definition of VIARULE(or GENERATE) rule
 *
 * @param flag
 */
void ViaRuleMetalLayer::setHasWidth(bool flag) { has_width_ = flag; }

/**
 * @brief getHasDirection
 * get metal/routing layer DIRECTION definition of VIARULE rule
 *
 * @return
 */
bool ViaRuleMetalLayer::getHasDirection() const { return has_direction_; }

/**
 * @brief setHasDirection
 * set metal/routing layer DIRECTION definition of VIARULE rule
 *
 * @param flag
 */
void ViaRuleMetalLayer::setHasDirection(bool flag) { has_direction_ = flag; }

/**
 * @brief getHasEnclosure
 * get metal/routing layer ENCLOSURE definition of VIARULE GENERATE rule
 *
 * @return
 */
bool ViaRuleMetalLayer::getHasEnclosure() const { return has_enclosure_; }

/**
 * @brief setHasEnclosure
 * set metal/routing layer ENCLOSURE definition of VIARULE GENERATE rule
 *
 * @return
 */
void ViaRuleMetalLayer::setHasEnclosure(bool flag) { has_enclosure_ = flag; }

/**
 * @brief getIsHorizontal
 * get if metal/routing layer's direction is horizontal in VIARULE
 *
 * @return
 */
bool ViaRuleMetalLayer::getIsHorizontal() const { return is_horz_; }

/**
 * @brief setIsHorizontal
 * set if metal/routing layer's direction is horizontal in VIARULE
 *
 * @param flag
 */
void ViaRuleMetalLayer::setIsHorizontal(bool flag) { is_horz_ = flag; }

/**
 * @brief getMinWidth
 * get metal/routing layer's minWidth
 *
 * @return
 */
Int32 ViaRuleMetalLayer::getMinWidth() const { return min_width_; }

/**
 * @brief setMinWidth
 * set metal/routing layer's minWidth
 *
 * @param min_width
 */
void ViaRuleMetalLayer::setMinWidth(Int32 min_width) { min_width_ = min_width; }

/**
 * @brief getMaxWidth
 * get metal/routing layer's maxWidth
 *
 * @return
 */
Int32 ViaRuleMetalLayer::getMaxWidth() const { return max_width_; }

/**
 * @brief setMaxWidth
 * set metal/routing layer's maxWidth
 *
 * @param max_width
 */
void ViaRuleMetalLayer::setMaxWidth(Int32 max_width) { max_width_ = max_width; }

/**
 * @brief getOverhang1
 * get metal/routing layer's overhang1 in VIARULE GENERATE
 *
 * @return
 */
Int32 ViaRuleMetalLayer::getOverhang1() const { return overhang1_; }

/**
 * @brief setOverhang1
 * set metal/routing layer's overhang1 in VIARULE GENERATE
 *
 * @param overhang
 */
void ViaRuleMetalLayer::setOverhang1(Int32 overhang) { overhang1_ = overhang; }

/**
 * @brief getOverhang2
 * get metal/routing layer's overhang2 in VIARULE GENERATE
 *
 * @return
 */
Int32 ViaRuleMetalLayer::getOverhang2() const { return overhang2_; }

/**
 * @brief setOverhang2
 * set metal/routing layer's overhang2 in VIARULE GENERATE
 *
 * @param overhang
 */
void ViaRuleMetalLayer::setOverhang2(Int32 overhang) { overhang2_ = overhang; }

/**
 * @brief getLayerId
 * get metal/routing layer's id
 *
 * @return
 */
Int32 ViaRuleMetalLayer::getLayerId() const { return layer_id_; }

/**
 * @brief setLayerId
 * set metal/routing layer's id
 *
 * @param id
 */
void ViaRuleMetalLayer::setLayerId(Int32 id) { layer_id_ = id; }

void ViaRuleMetalLayer::print(int is_generate) {
    Tech *lib = getTechLib();
    Layer *layer = lib->getLayer(layer_id_);
    if (layer) message->info("\n     LAYER %s ;", layer->getName());
    if (!is_generate) {
        if (is_horz_) {
            message->info("\n        DIRECTION HORIZONTAl ;");
        } else {
            message->info("\n        DIRECTION VERTICAL ;");
        }
    } else {
        message->info("\n        ENCLOSURE %d %d ;", overhang1_, overhang2_);
    }

    if (has_width_) {
        message->info("\n        WIDTH %d TO %d ;", min_width_, max_width_);
    }
}

void ViaRuleMetalLayer::printLEF(std::ofstream &ofs, int is_generate) {
    Tech *lib = getTechLib();
    Layer *layer = lib->getLayer(layer_id_);
    if (layer)
        ofs << "\n     LAYER"
            << " " << layer->getName() << " ;";
    if (!is_generate) {
        if (is_horz_) {
            ofs << "\n        DIRECTION HORIZONTAl ;";
        } else {
            ofs << "\n        DIRECTION VERTICAL ;";
        }
    } else {
        ofs << "\n        ENCLOSURE"
            << " " << lib->dbuToMicrons(overhang1_) << " "
            << lib->dbuToMicrons(overhang2_) << " ;";
    }

    if (has_width_) {
        ofs << "\n        WIDTH"
            << " " << lib->dbuToMicrons(min_width_) << " TO "
            << lib->dbuToMicrons(max_width_) << " ;";
    }
}

// ViaRuleCutLayer

/**
 * @brief Construct a new Via Rule Cut Layer:: Via Rule Cut Layer object
 *
 */
ViaRuleCutLayer::ViaRuleCutLayer() {
    has_resistance_ = 0;
    x_spacing_ = 0;
    layer_id_ = 0;
    resistance_per_cut_ = 0;
}

/**
 * @brief getHasResistance
 * get if cut layer has RESISTANCE definition in VIARULE GENERATE
 *
 * @return
 */
bool ViaRuleCutLayer::getHasResistance() const { return has_resistance_; }

/**
 * @brief setHasResistance
 * set if cut layer has RESISTANCE definition in VIARULE GENERATE
 *
 * @param flag
 */
void ViaRuleCutLayer::setHasRestance(bool flag) { has_resistance_ = flag; }

/**
 * @brief getBox
 * get cut layer RECT in VIARULE GENERATE
 *
 * @return
 */
Box ViaRuleCutLayer::getBox() const { return rect_; }

/**
 * @brief setBox
 * set cut layer RECT in VIARULE GENERATE
 *
 * @param box
 */
void ViaRuleCutLayer::setBox(const Box box) { rect_ = box; }

/**
 * @brief getXSpacing
 * get cut layer SPACING xSpacing in VIARULE GENERATE
 *
 * @return
 */
Int32 ViaRuleCutLayer::getXSpacing() const { return x_spacing_; }

/**
 * @brief setXSpacing
 * set cut layer SPACING xSpacing in VIARULE GENERATE
 *
 * @param spacing
 */
void ViaRuleCutLayer::setXSpacing(Int32 spacing) { x_spacing_ = spacing; }

/**
 * @brief getYSpacing
 * get cut layer SPACING ySpacing in VIARULE GENERATE
 *
 * @return
 */
Int32 ViaRuleCutLayer::getYSpacing() const { return y_spacing_; }

/**
 * @brief setYSpacing
 * set cut layer SPACING ySpacing in VIARULE GENERATE
 *
 * @param spacing
 */
void ViaRuleCutLayer::setYSpacing(Int32 spacing) { y_spacing_ = spacing; }

/**
 * @brief getLayerId
 * get cut layer id in VIARULE GENERATE
 *
 * @return
 */
Int32 ViaRuleCutLayer::getLayerId() const { return layer_id_; }

/**
 * @brief setLayerId
 * set cut layer id in VIARULE GENERATE
 *
 * @param id
 */
void ViaRuleCutLayer::setLayerId(Int32 id) { layer_id_ = id; }

/**
 * @brief getResistancePerCut
 * get cut layer RESISTANCE resistancePerCut in VIARULE GENERATE
 *
 * @return
 */
float ViaRuleCutLayer::getResistancePerCut() const {
    return resistance_per_cut_;
}

/**
 * @brief setResistancePerCut
 * set cut layer RESISTANCE resistancePerCut in VIARULE GENERATE
 *
 * @return
 */
void ViaRuleCutLayer::setResistancePerCut(float res) {
    resistance_per_cut_ = res;
}

void ViaRuleCutLayer::print() {
    Tech *lib = getTechLib();
    Layer *layer = lib->getLayer(layer_id_);
    if (layer) message->info("\n     LAYER %s ;", layer->getName());
    message->info("\n        RECT %d %d %d %d ;", rect_.getLLX(),
                  rect_.getLLY(), rect_.getURX(), rect_.getURY());
    message->info("\n        SPACING %d BY %d ;", x_spacing_, y_spacing_);
    if (has_resistance_)
        message->info("\n       RESISTANCE %f", resistance_per_cut_);
}

void ViaRuleCutLayer::printLEF(std::ofstream &ofs) {
    Tech *lib = getTechLib();
    Layer *layer = lib->getLayer(layer_id_);
    if (layer) {
        ofs << "\n     LAYER"
            << " " << layer->getName() << " ;";
    }

    ofs << "\n        RECT"
        << " " << lib->dbuToMicrons(rect_.getLLX()) << " "
        << lib->dbuToMicrons(rect_.getLLY()) << " "
        << lib->dbuToMicrons(rect_.getURX()) << " "
        << lib->dbuToMicrons(rect_.getURY()) << " ;";
    ofs << "\n        SPACING"
        << " " << lib->dbuToMicrons(x_spacing_) << " BY "
        << lib->dbuToMicrons(y_spacing_) << " ;";
    if (has_resistance_)
        ofs << "\n       RESISTANCE"
            << " " << lib->dbuToMicrons(resistance_per_cut_) << " ;";
}

}  // namespace db
}  // namespace open_edi
