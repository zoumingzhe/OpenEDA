/* @file  tech.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/tech/tech.h"

#include "db/core/db.h"
#include "db/core/cell.h"
#include "db/util/array.h"
#include "db/util/property_definition.h"
#include "db/util/vector_object_var.h"

namespace open_edi {
namespace db {

/**
 * @brief Tech
 * default constructor
 *
 * @param null
 */
Tech::Tech() {
    bus_bits_index_ = -1;
    divider_name_index_ = -1;
    extensions_name_index_ = -1;
    layer_ids_ = 0;
    max_via_stack_ = 0;
}

Tech::~Tech() {}

/**
 * @brief getHasClearanceMeasure
 * get if there is CLEARANCEMEASURE definition
 *
 * @return
 */
bool Tech::getHasClearanceMeasure() const { return has_clearance_measure_; }

/**
 * @brief setHasClearanceMeasure
 * set there is CLEARANCEMEASURE definition
 *
 * @param flag
 */
void Tech::setHasClearanceMeasure(bool flag) { has_clearance_measure_ = flag; }

/**
 * @brief getClearanceMeasureUseMaxxy
 * get if CLEARANCEMEASURE is MAXXY(or else EUCLIDEAN)
 *
 * @return
 */
bool Tech::getClearanceMeasureUseMaxxy() const {
    return clearance_measure_use_maxxy_;
}

/**
 * @brief setClearanceMeasureUseMaxxy
 * set if CLEARANCEMEASURE is MAXXY
 *
 * @param flag
 */
void Tech::setClearanceMeasureUseMaxxy(bool flag) {
    clearance_measure_use_maxxy_ = flag;
}

/**
 * @brief getHasOBSUseMinSpacing
 * get if there is USEMINSPACING OBS definition
 *
 * @return
 */
bool Tech::getHasOBSUseMinSpacing() const { return has_obs_use_min_spacing_; }

/**
 * @brief setHasOBSUseMinSpacing
 * set there is USEMINSPACING OBS definition
 *
 * @param flag
 */
void Tech::setHasOBSUseMinSpacing(bool flag) {
    has_obs_use_min_spacing_ = flag;
}

/**
 * @brief getOBSUseMinSpacing
 * get if USEMINSPACING OBS is ON(or else OFF)
 *
 * @return
 */
bool Tech::getOBSUseMinSpacing() const { return obs_use_min_spacing_; }

/**
 * @brief setOBSUseMinSpacing
 * set if USEMINSPACING OBS is ON
 *
 * @param flag
 */
void Tech::setOBSUseMinSpacing(bool flag) { obs_use_min_spacing_ = flag; }

/**
 * @brief getHasPinUseMinSpacing
 * get if there is USEMINSPACING PIN definition
 *
 * @return
 */
bool Tech::getHasPinUseMinSpacing() const { return has_pin_use_min_spacing_; }

/**
 * @brief setHasPinUseMinSpacing
 * set there is USEMINSPACING PIN definition
 *
 * @param flag
 */
void Tech::setHasPinUseMinSpacing(bool flag) {
    has_pin_use_min_spacing_ = flag;
}

/**
 * @brief getPinUseMinSpacing
 * get if USEMINSPACING PIN is ON(or else OFF)
 *
 * @return
 */
bool Tech::getPinUseMinSpacing() const { return pin_use_min_spacing_; }

/**
 * @brief setPinUseMinSpacing
 * set if USEMINSPACING PIN is ON
 *
 * @param flag
 */
void Tech::setPinUseMinSpacing(bool flag) { pin_use_min_spacing_ = flag; }

/**
 * @brief getHasFixedMask
 * get if FIXEDMASK is defined
 *
 * @return
 */
bool Tech::getHasFixedMask() const { return has_fixed_mask_; }

/**
 * @brief setHasFixedMask
 * set if FIXEDMASK is defined
 *
 * @param flag
 */
void Tech::setHasFixedMask(bool flag) { has_fixed_mask_ = flag; }

/**
 * @brief getCaseSensitive
 * get if "NAMESCASESENSITIVE ON ;"
 *
 * @return
 */
bool Tech::getCaseSensitive() const { return case_sensitive_; }

/**
 * @brief setCaseSensitive
 * set if "NAMESCASESENSITIVE ON ;"
 *
 * @param flag
 */
void Tech::setCaseSensitive(bool flag) { case_sensitive_ = flag; }

/**
 * @brief getBusBitsId
 * get BUSBITSCHARS
 *
 * @return
 */
SymbolIndex Tech::getBusBitsId() const { return bus_bits_index_; }

/**
 * @brief getDividerId
 * get DIVIDERCHAR
 *
 * @return
 */
SymbolIndex Tech::getDividerId() const { return divider_name_index_; }

/**
 * @brief getExtensionsId
 * get EXTENSIONS
 *
 * @return
 */
SymbolIndex Tech::getExtensionsId() const { return extensions_name_index_; }

/**
 * @brief getBusBitsName
 * get BUSBITSCHARS
 *
 * @return
 */
const char *Tech::getBusBitsName() const {
    return getTopCell()->getSymbolByIndex(bus_bits_index_).c_str();
}

/**
 * @brief setBusBitsName
 * set BUSBITSCHARS
 *
 * @param s
 */
void Tech::setBusBitsName(const char *s) {
    SymbolIndex index = getTopCell()->getOrCreateSymbol(s);
    if (index == kInvalidSymbolIndex) return;

    bus_bits_index_ = index;
    getTopCell()->addSymbolReference(index, this->getId());
}

/**
 * @brief getDividerName
 * get DIVIDERCHAR
 *
 * @return
 */
const char *Tech::getDividerName() const {
    return getTopCell()->getSymbolByIndex(divider_name_index_).c_str();
}

/**
 * @brief setDividerName
 * set DIVIDERCHAR
 *
 * @param s
 */
void Tech::setDividerName(const char *s) {
    SymbolIndex index = getTopCell()->getOrCreateSymbol(s);
    if (index == kInvalidSymbolIndex) return;

    divider_name_index_ = index;
    getTopCell()->addSymbolReference(index, this->getId());
}

/**
 * @brief getExtensionsName
 * get EXTENSIONS
 *
 * @return
 */
const char *Tech::getExtensionsName() const {
    return getTopCell()->getSymbolByIndex(extensions_name_index_).c_str();
}

/**
 * @brief setExtensionsName
 * set EXTENSIONS
 *
 * @param s
 */
void Tech::setExtensionsName(const char *s) {
    SymbolIndex index = getTopCell()->getOrCreateSymbol(s);
    if (index == kInvalidSymbolIndex) return;

    extensions_name_index_ = index;
    getTopCell()->addSymbolReference(index, this->getId());
}

/**
 * @brief getVersion
 *
 * @return
 */
double Tech::getVersion() const { return version_; }

/**
 * @brief setVersion
 *
 * @param
 */
void Tech::setVersion(double v) { version_ = v; }

/**
 * @brief getManuGrids
 * get MANUFACTURINGGRID
 *
 * @return
 */
UInt32 Tech::getManuGrids() const { return manu_grids_; }

/**
 * @brief setManuGrids
 * set MANUFACTURINGGRID
 *
 * @param manu_grids
 */
void Tech::setManuGrids(UInt32 manu_grids) { manu_grids_ = manu_grids; }

/**
 * @brief getUnits
 * get UNITS
 *
 * @return db_units
 */
Units *Tech::getUnits() const {
    if (db_units_ == 0) {
        return nullptr;
    }
    Units *obj_data = addr<Units>(db_units_);

    return obj_data;
}

/**
 * @brief setUnits
 * set UNITS
 *
 * @param db_units
 */
void Tech::setUnits(Units *const vobj) {
    if (vobj != nullptr) {
        db_units_ = vobj->getId();
    }
}

/**
 * @brief addLayer
 * insert new layer into list
 * layer->getIndexInLef() should be the same as the index in
 * layer_ids_ of this layer
 *
 * @param layer
 */
bool Tech::addLayer(Layer *layer) {
    ArrayObject<ObjectId> *array_obj = nullptr;
    Cell *cell = addr<Cell>(getOwnerId());
    if (cell && layer_ids_ == 0) {
        array_obj = cell->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (nullptr == array_obj) return false;
        array_obj->setPool(cell->getPool());
        array_obj->reserve(32);
        layer_ids_ = array_obj->getId();
    } else if (layer_ids_ != 0) {
        array_obj = addr<ArrayObject<ObjectId>>(layer_ids_);
    }
    if (nullptr != array_obj) {
        array_obj->pushBack(layer->getId());
        return true;
    }
    return false;
}

/**
 * @brief getLayerLEFIndexByName
 * use layer name to get layer id
 *
 * @param name
 */
Int32 Tech::getLayerLEFIndexByName(const char *name) {
    Layer *layer_obj = nullptr;
    Cell *cell = addr<Cell>(getOwnerId());
    if (nullptr == cell) return -1;

    std::string layer_name(name);
    layer_obj = cell->getSymbolTable()->getObjectByTypeAndName<Layer>(
        kObjectTypeLayer, layer_name);

    if (layer_obj) return layer_obj->getIndexInLef();

    return -1;
}

/**
 * @brief getLayerByName
 * get layer pointer by name
 *
 * @param name
 */

Layer *Tech::getLayerByName(const char *name) {
    Int32 layer_index = getLayerLEFIndexByName(name);
    if (-1 == layer_index) {
        return nullptr;
    }
    return getLayer(layer_index);
}

/**
 * @brief getLayer
 * get layer pointer
 *
 * @param name
 */
Layer *Tech::getLayer(Int32 layer_id) {
    if (layer_ids_ == 0) return nullptr;
    ArrayObject<ObjectId> *array_obj = nullptr;

    array_obj = addr<ArrayObject<ObjectId>>(layer_ids_);
    if (nullptr == array_obj || !array_obj->getIsValid()) return nullptr;

    if (layer_id < 0 || layer_id >= array_obj->getSize()) {
        return nullptr;
    }

#if 0
    ArrayObject<ObjectId>::iterator iter;
    for (iter = array_obj->begin(); iter != array_obj->end(); iter++) {
        Layer *o = nullptr;
        o = addr<Layer>(*iter);
        if (o && o->getIndexInLef() == layer_id) {
            return o;
        }
    }
#endif

    return addr<Layer>((*array_obj)[layer_id]);
}

/**
 * @brief
 * get the total number of layers
 *
 * @return
 */
UInt32 Tech::getNumLayers() const {
    if (layer_ids_ == 0) return 0;
    ArrayObject<ObjectId> *array_obj = nullptr;

    array_obj = addr<ArrayObject<ObjectId>>(layer_ids_);
    if (nullptr == array_obj || !(array_obj->getIsValid())) return 0;

    return array_obj->getSize();
}

/**
 * @brief addPropertyDefinition
 * append property definition into vector object
 *
 * @param object id
 */
void Tech::addPropertyDefinition(ObjectId pobj_id) {
    PropertyDefinition *pobj = addr<PropertyDefinition>(pobj_id);
    Cell *cell = addr<Cell>(getOwnerId());
    ediAssert(pobj != nullptr);
    ediAssert(cell != nullptr);

    PropType type = pobj->getPropType();
    uint32_t index = toInteger<PropType>(type);
    ObjectId array_id = property_definitions_array_[index];
    ArrayObject<ObjectId> *array_ptr = nullptr;

    if (array_id == 0) {
        array_ptr = 
            cell->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        array_ptr->setPool(cell->getPool());
        array_ptr->reserve(16);
        ediAssert(array_ptr != nullptr);
        property_definitions_array_[index] = array_ptr->getId();
    } else {
        array_ptr = addr<ArrayObject<ObjectId>>(array_id);
        ediAssert(array_ptr != nullptr);
    }
    array_ptr->pushBack(pobj_id);
}

/**
 * @brief getPropertyDefinitionVectorId
 * fetch vector object of property definition
 *
 * @param prop type: LAYER, LIBRARY, etc.
 * @return object id of vector object
 */
ObjectId Tech::getPropertyDefinitionVectorId(PropType type) {
    uint32_t index = toInteger<PropType>(type);
    return property_definitions_array_[index];
}

/**
 * @brief getPropertyDefinitionId
 * fetch property definition with the specified name
 *
 * @param prop type: LAYER, LIBRARY, etc.
 * @param prop name
 * @return object id of property definition if found. otherwise return 0.
 */
ObjectId Tech::getPropertyDefinitionId(PropType type, const char *prop_name) {
    ObjectId arr_id = getPropertyDefinitionVectorId(type);
    ArrayObject<ObjectId> *arr = addr<ArrayObject<ObjectId>>(arr_id);
    ediAssert(arr != nullptr && prop_name != nullptr);

    UInt32 size = arr->getSize();

    for (int i = 0; i < size; ++i) {
        ObjectId prop_id = (*arr)[i];
        if (prop_id == 0) continue;
        PropertyDefinition *prop_data = addr<PropertyDefinition>(prop_id);
        if (prop_data == nullptr) continue;
        if (!prop_data->getPropName().compare(prop_name)) {
            return prop_id;
        }
    }
    return 0;  // invalid object id.
}

/**
 * @brief getMaxViaStack
 * get MAXVIASTACK rule
 *
 * @return
 */
MaxViaStack *Tech::getMaxViaStack() const {
    if (max_via_stack_) {
        return addr<MaxViaStack>(max_via_stack_);
    }
    return nullptr;
}

/**
 * @brief setMaxViaStack
 * set MAXVIASTACK rule
 *
 * @param mvs
 */
void Tech::setMaxViaStack(ObjectId mvs_id) { max_via_stack_ = mvs_id; }

/**
 * @brief getViaRule
 * get VIARULE or VIA GENERATE rule by name
 *
 * @return
 */
// TODO: change via_rule_ to VectorObject; temporarily use this list.
ViaRule *Tech::getViaRule(const char *name) const {
    std::string via_rule_name = name;
    ViaRule *via_rule = getViaRule(via_rule_name);
    return via_rule;
}

/**
 * @brief micronsToDBU
 * convert data from user unit to DB unit
 * add bias to avoid precision loss
 *
 * @param data
 */
Int32 Tech::micronsToDBU(double data) {
    Units *db_units_data = getUnits();
    if (db_units_data == nullptr) return -1;

    const double bias = db_units_data->getMicronPerDBU() / 2;
    double dbu_data;
    if (data >= 0.0) {
        dbu_data = (data + bias) * db_units_data->getLengthFactor();
    } else {
        dbu_data = (data - bias) * db_units_data->getLengthFactor();
    }
    return static_cast<Int32>(dbu_data);
}

/**
 * @brief areaMicronsToDBU
 * convert data from user unit to DB unit in square
 *
 * @param data
 */
double Tech::areaMicronsToDBU(double data) {
    Units *db_units_data = getUnits();
    if (db_units_data == nullptr) return -1;

    const double bias = 0;  // db_units_data->getMicronPerDBU() / 2;

    double dbu_data;
    if (data >= 0.0) {
        dbu_data = (data + bias) * db_units_data->getLengthFactor() *
                   db_units_data->getLengthFactor();
    } else {
        dbu_data = (data - bias) * db_units_data->getLengthFactor() *
                   db_units_data->getLengthFactor();
    }
    return dbu_data;
}

/**
 * @brief dbuToMicrons
 * convert data from DB unit to user unit
 *
 * @param data
 */
double Tech::dbuToMicrons(Int32 data) {
    Units *db_units_data = getUnits();
    if (db_units_data == nullptr) return -1;
    return data * db_units_data->getMicronPerDBU();
}

/**
 * @brief areaDBUToMicrons
 * convert data from DB unit to user unit in square root
 *
 * @param data
 */
double Tech::areaDBUToMicrons(Long data) {
    Units *db_units_data = getUnits();
    if (db_units_data == nullptr) return -1;
    return data * db_units_data->getMicronPerDBU() *
           db_units_data->getMicronPerDBU();
}

/**
 * @brief addNonDefaultRule
 * insert new ndr_rule into list
 *
 * @param ndr_rule_id
 */
void Tech::addNonDefaultRule(ObjectId ndr_rule_id) {
    ArrayObject<ObjectId> *arr_ptr = nullptr;
    Cell *cell = addr<Cell>(getOwnerId());

    if (!ndr_rules_) {
        arr_ptr = cell->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        arr_ptr->setPool(cell->getPool());
        arr_ptr->reserve(8);
    } else {
        arr_ptr = addr<ArrayObject<ObjectId>>(ndr_rules_);
    }
    ediAssert(arr_ptr != nullptr);
    arr_ptr->pushBack(ndr_rule_id);
    if (!ndr_rules_) ndr_rules_ = arr_ptr->getId();
}

/**
 * @brief getNonDefaultRuleVectorId
 * fetch vector object of non-default vaule
 *
 * @param null
 * @return object id of vector object
 */
ObjectId Tech::getNonDefaultRuleVectorId() const { return ndr_rules_; }

/**
 * @brief getViaMasterVectorId
 * fetch vector object of via master
 *
 * @param null
 * @return object id of vector object
 */

ObjectId Tech::getViaMasterVectorId() const { return via_masters_; }

/**
 * @brief getViaRuleVectorId
 * fetch vector object of via rule
 *
 * @param null
 * @return object id of vector object
 */

ObjectId Tech::getViaRuleVectorId() const { return via_rules_; }

/**
 * @brief get non-default rule array
 *
 * @return ArrayObject<ObjectId>*
 */
ArrayObject<ObjectId> *Tech::getNonDefaultRuleArray() const {
    if (ndr_rules_ != 0) {
        ArrayObject<ObjectId> *ndr_array =
            addr<ArrayObject<ObjectId>>(ndr_rules_);
        return ndr_array;
    } else {
        return nullptr;
    }
}

/**
 * @brief get via master array
 *
 * @return ArrayObject<ObjectId>*
 */
ArrayObject<ObjectId> *Tech::getViaMasterArray() const {
    if (via_masters_ != 0) {
        ArrayObject<ObjectId> *via_master_array =
            addr<ArrayObject<ObjectId>>(via_masters_);
        return via_master_array;
    } else {
        return nullptr;
    }
}

/**
 * @brief get via rule array
 *
 * @return ArrayObject<ObjectId>*
 */
ArrayObject<ObjectId> *Tech::getViaRuleArray() const {
    if (via_rules_ != 0) {
        ArrayObject<ObjectId> *via_rule_array =
            addr<ArrayObject<ObjectId>>(via_rules_);
        return via_rule_array;
    } else {
        return nullptr;
    }
}  // namespace db

/**
 * @brief get site array
 *
 * @return ArrayObject<ObjectId>*
 */
ArrayObject<ObjectId> *Tech::getSiteArray() const {
    ArrayObject<ObjectId> *site_array = addr<ArrayObject<ObjectId>>(sites_);
    return site_array;
}

/**
 * @brief getNonDefaultRuleIdByName
 * use ndr_rule name to get ndr_rule id
 *
 * @param name
 */
ObjectId Tech::getNonDefaultRuleIdByName(const char *name) const {
    ArrayObject<ObjectId> *ndr_rule_array = addr<ArrayObject<ObjectId>>(ndr_rules_);
    if (ndr_rule_array == nullptr) {
        return 0;
    }
    for (UInt32 i = 0; i < ndr_rule_array->getSize(); i++) {
        ObjectId obj_id = (*ndr_rule_array)[i];
        NonDefaultRule *ndr_rule = addr<NonDefaultRule>(obj_id);
        if (strcmp(ndr_rule->getName(), name) == 0) {
            return obj_id;
        }
    }
    return 0;  // invalid object id.
}

/**
 * @brief getNonDefaultRule
 * use ndr_rule name to get ndr_rule object
 *
 * @param name
 */
NonDefaultRule *Tech::getNonDefaultRule(const char *name) const {
    ArrayObject<ObjectId> *ndr_rule_array = addr<ArrayObject<ObjectId>>(ndr_rules_);
    if (ndr_rule_array == nullptr) {
        return 0;
    }
    for (UInt32 i = 0; i < ndr_rule_array->getSize(); i++) {
        ObjectId obj_id = (*ndr_rule_array)[i];
        NonDefaultRule *ndr_rule = addr<NonDefaultRule>(obj_id);
        if (ndr_rule && strcmp(ndr_rule->getName(), name) == 0) {
            return ndr_rule;
        }
    }
    return nullptr;
}

/**
 * @brief Create a Via Master object
 *
 * @param name
 * @return ViaMaster*
 */
ViaMaster *Tech::createAndAddViaMaster(std::string &name) {
    ArrayObject<ObjectId> *array_ptr = nullptr;
    Cell *cell = addr<Cell>(getOwnerId());

    ViaMaster *via_master =
        cell->createObject<ViaMaster>(kObjectTypeViaMaster);
    if (via_master) {
        via_master->setName(name);
    } else {
        // error out
    }
    if (via_masters_ == 0) {
        array_ptr = 
            cell->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        array_ptr->setPool(cell->getPool());
        array_ptr->reserve(32);
        via_masters_ = array_ptr->getId();
    } else {
        array_ptr = addr<ArrayObject<ObjectId>>(via_masters_);
    }
    array_ptr->pushBack(via_master->getId());
    return via_master;
}

ViaRule *Tech::createViaRule(std::string &name) {
    ViaRule *via_rule = getTopCell()->createObject<ViaRule>(kObjectTypeViaRule);
    via_rule->setName(name);

    return via_rule;
}

void Tech::addViaRule(ViaRule *via_rule) {
    ArrayObject<ObjectId> *array_ptr = nullptr;
    Cell *cell = addr<Cell>(getOwnerId());

    if (via_rules_ == 0) {
        array_ptr =
            cell->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        array_ptr->setPool(cell->getPool());
        array_ptr->reserve(16);
    } else {
        array_ptr = addr<ArrayObject<ObjectId>>(via_rules_);
    }
    ediAssert(array_ptr != nullptr);
    array_ptr->pushBack(via_rule->getId());
    if (via_rules_ == 0) via_rules_ = array_ptr->getId();
}

ViaRule *Tech::getViaRule(const std::string &name) const {
    if (via_rules_ == 0) return nullptr;
    ArrayObject<ObjectId> *arr_ptr = addr<ArrayObject<ObjectId>>(via_rules_);
    if (arr_ptr == nullptr) return nullptr;
    for (int i = 0; i < arr_ptr->getSize(); i++) {
        ViaRule *obj_data = nullptr;
        ObjectId obj_id = (*arr_ptr)[i];
        if (obj_id) obj_data = addr<ViaRule>(obj_id);
        if (obj_data && obj_data->getName() == name) {
            return obj_data;
        }
    }
    return nullptr;
}

/**
 * @brief Get the Via Master Id By Name object
 *
 * @param via_master_name
 * @return ViaMaster
 */
ViaMaster *Tech::getViaMaster(const std::string &name) const {
    if (via_masters_ == 0) return nullptr;
    ArrayObject<ObjectId> *arr_ptr = addr<ArrayObject<ObjectId>>(via_masters_);
    if (arr_ptr == nullptr) return nullptr;
    for (int i = 0; i < arr_ptr->getSize(); i++) {
        ObjectId obj_id = (*arr_ptr)[i];
        ViaMaster *obj_data = addr<ViaMaster>(obj_id);
        if (obj_data && obj_data->getName() == name) {
            return obj_data;
        }
    }
    return nullptr;
}

/**
 * @brief  addSite
 * Add site to the array
 *
 * @param site
 */
void Tech::addSite(Site *site) {
    ArrayObject<ObjectId> *arr_ptr = nullptr;
    Cell *cell = addr<Cell>(getOwnerId());

    if (sites_ == 0) {
        arr_ptr = cell->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        arr_ptr->setPool(cell->getPool());
        arr_ptr->reserve(16);
    } else {
        arr_ptr = addr<ArrayObject<ObjectId>>(sites_);
    }
    arr_ptr->pushBack(site->getId());
    if (sites_ == 0) sites_ = arr_ptr->getId();
}

/**
 * @brief Get the site by its name
 *
 * @param site_name
 * @return Site
 */
Site *Tech::getSiteByName(const char *site_name) const {
    if (sites_ == 0) return nullptr;
    ArrayObject<ObjectId> *arr_ptr = addr<ArrayObject<ObjectId>>(sites_);
    if (arr_ptr == nullptr) return nullptr;
    for (int i = 0; i < arr_ptr->getSize(); i++) {
        Site *obj_data = nullptr;
        ObjectId obj_id = (*arr_ptr)[i];
        if (obj_id) obj_data = addr<Site>(obj_id);
        if (obj_data && !strcmp(obj_data->getName(), site_name)) {
            return obj_data;
        }
    }
    return nullptr;
}

/**
 * @brief getSiteVectorId
 * fetch vector object of site
 *
 * @param null
 * @return object id of vector object
 */
ObjectId Tech::getSiteVectorId() const { return sites_; }

}  // namespace db
}  // namespace open_edi
