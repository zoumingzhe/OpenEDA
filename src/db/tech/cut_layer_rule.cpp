/* @file  cut_layer_rule.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/tech/cut_layer_rule.h"

#include "db/util/vector_object_var.h"
#include "db/core/cell.h"

namespace open_edi {
namespace db {

/**
 * @brief
 * constructor of CutLayerRule
 */
CutLayerRule::CutLayerRule() {
    cut_spacings_ = 0;
    enclosures_ = 0;
    prefer_enclosures_ = 0;
    array_spacings_ = 0;
}

/**
 * @brief
 * destructor of CutLayerRule
 */
CutLayerRule::~CutLayerRule() {}

/**
 * @brief
 * get cut spacing rule
 *
 * @return
 */

ArrayObject<ObjectId> *CutLayerRule::getCutSpacingArray() const {
    if (cut_spacings_ != 0) {
        ArrayObject<ObjectId> *cut_space_array = addr<ArrayObject<ObjectId>>(cut_spacings_);
        return cut_space_array;
    } else {
        return nullptr;
    }
}

CutSpacing *CutLayerRule::getCutSpacing(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (cut_spacings_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(cut_spacings_);
    }
    if (vct) {
        CutSpacing *obj_data = addr<CutSpacing>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}
/**
 * @brief
 * set cut spacing rule
 *
 * @param cut_spacing
 */

void CutLayerRule::addCutSpacing(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (cut_spacings_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        cut_spacings_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(cut_spacings_);
    }
    if (vct) vct->pushBack(id);
}

/**
 * @brief
 * get enclosure rule
 *
 * @return
 */
ArrayObject<ObjectId> *CutLayerRule::getEnclosureArray() const{
    if (enclosures_ != 0) {
        ArrayObject<ObjectId> *enclosure_array = addr<ArrayObject<ObjectId>>(enclosures_);
        return enclosure_array;
    } else {
        return nullptr;
    }
}

Enclosure *CutLayerRule::getEnclosure(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (enclosures_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(enclosures_);
    }
    if (vct) {
        Enclosure *obj_data = addr<Enclosure>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}
/**
 * @brief
 * set enclosure rule
 *
 * @param enc
 */

void CutLayerRule::addEnclosure(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (enclosures_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        enclosures_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(enclosures_);
    }
    if (vct) vct->pushBack(id);
}

/**
 * @brief
 * get prefer enclosure rule
 *
 * @return
 */

ArrayObject<ObjectId> *CutLayerRule::getPreferEnclosureArray() const {
    if (prefer_enclosures_ != 0) {
        ArrayObject<ObjectId> *prefer_enclosure_array = addr<ArrayObject<ObjectId>>(prefer_enclosures_);
        return prefer_enclosure_array;
    } else {
        return nullptr;
    }
}

Enclosure *CutLayerRule::getPreferEnclosure(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (prefer_enclosures_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(prefer_enclosures_);
    }
    if (vct) {
        Enclosure *obj_data = addr<Enclosure>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

/**
 * @brief
 * set prefer enclosure rule
 *
 * @param enc
 */
void CutLayerRule::addPreferEnclosure(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (prefer_enclosures_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        prefer_enclosures_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(prefer_enclosures_);
    }
    if (vct) vct->pushBack(id);
}

/**
 * @brief
 * get array spacing rule
 *
 * @return
 */
ArrayObject<ObjectId> *CutLayerRule::getArraySpacingArray() const {
    if (array_spacings_ != 0) {
        ArrayObject<ObjectId> *array_spacings_array = addr<ArrayObject<ObjectId>>(array_spacings_);
        return array_spacings_array;
    } else {
        return nullptr;
    }
}

ArraySpacing *CutLayerRule::getArraySpacing(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (array_spacings_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(array_spacings_);
    }
    if (vct) {
        ArraySpacing *obj_data = addr<ArraySpacing>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}
/**
 * @brief
 * set array spacing rule
 *
 * @param array_spacing
 */
void CutLayerRule::addArraySpacing(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (array_spacings_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        array_spacings_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(array_spacings_);
    }
    if (vct) vct->pushBack(id);
}

/**
 * @brief
 * constructor of cut spacing rule
 */
CutSpacing::CutSpacing() {
    spacing_ = 0;
    is_c2c_ = false;
    is_same_net_ = false;
    cut_area_ = 0;
    cut_spacing_type_ = 0;
    cut_sp_rules_ = 0;
}

/**
 * @brief
 * destructor of cut spacing rule
 */
CutSpacing::~CutSpacing() {}

/**
 * @brief
 * get cutSpacing value
 *
 * @return
 */
Int32 CutSpacing::getSpacing() const { return spacing_; }

/**
 * @brief
 * set cutSpacing value
 *
 * @param spacing
 */
void CutSpacing::setSpacing(Int32 spacing) { spacing_ = spacing; }

/**
 * @brief
 * get center to center flag
 *
 * @return
 */
bool CutSpacing::getIsC2C() const { return is_c2c_; }

/**
 * @brief
 * set center to center flag
 *
 * @param is_c2c
 */
void CutSpacing::setIsC2C(bool is_c2c) { is_c2c_ = is_c2c; }

/**
 * @brief
 * get same net flag
 *
 * @return
 */
bool CutSpacing::getIsSameNet() const { return is_same_net_; }

/**
 * @brief
 * set same net flag
 *
 * @param is_same_net
 */
void CutSpacing::setIsSameNet(bool is_same_net) { is_same_net_ = is_same_net; }

/**
 * @brief
 * get is area flag
 *
 * @return
 */
bool CutSpacing::getIsCutArea() const { return (cut_area_ > 0); }

/**
 * @brief
 * get area value
 *
 * @return
 */
Int32 CutSpacing::getCutArea() const { return cut_area_; }

/**
 * @brief
 * set area value
 *
 * @param cut_area
 */
void CutSpacing::setCutArea(Int32 cut_area) { cut_area_ = cut_area; }

/**
 * @brief
 * get second layer flag
 *
 * @return
 */
bool CutSpacing::getIsSecondLayer() const {
    return (cut_spacing_type_ & cut_spacing_is_second_layer);
}

/**
 * @brief
 * set second layer flag
 *
 * @param
 */
void CutSpacing::setIsSecondLayer() {
    cut_spacing_type_ |= cut_spacing_is_second_layer;
}

/**
 * @brief
 * get second layer class
 *
 * @return
 */
SecondLayer *CutSpacing::getSecondLayer() const {
    SecondLayer *obj_data = addr<SecondLayer>(cut_sp_rules_);
    return obj_data;
}

/**
 * @brief
 * set second layer class
 *
 * @param sec_layer
 */
void CutSpacing::setSecondLayer(ObjectId id) {
    cut_sp_rules_ = id;
}

/**
 * @brief
 * get adjacent cuts flag
 *
 * @return
 */
bool CutSpacing::getIsAdjCuts() const {
    return (cut_spacing_type_ & cut_spacing_is_adj_cuts);
}

/**
 * @brief
 * set adjacent cuts flag
 *
 * @param
 */
void CutSpacing::setIsAdjCuts() {
    cut_spacing_type_ |= cut_spacing_is_adj_cuts;
}

/**
 * @brief
 * set adjacent cuts class
 *
 * @return
 */
AdjacentCuts *CutSpacing::getAdjCuts() const { 
    AdjacentCuts *obj_data = addr<AdjacentCuts>(cut_sp_rules_);
    return obj_data;
}

/**
 * @brief
 * set adjacent cuts class
 *
 * @param adj_cuts
 */
void CutSpacing::setAdjCuts(ObjectId id) {
    cut_sp_rules_ = id;
}

/**
 * @brief
 * get parallel overlap flag
 *
 * @return
 */
bool CutSpacing::getIsParallelOverlap() const {
    return (cut_spacing_type_ & cut_spacing_is_parallel_overlap);
}

/**
 * @brief
 * set parallel overlap flag
 *
 * @param
 */
void CutSpacing::setIsParallelOverlap() {
    cut_spacing_type_ |= cut_spacing_is_parallel_overlap;
}

/**
 * @brief
 * get parallel overlap class
 *
 * @return
 */
CutSpacingPrlOvlp *CutSpacing::getParallelOverlap() const {
    CutSpacingPrlOvlp *obj_data = addr<CutSpacingPrlOvlp>(cut_sp_rules_);
    return obj_data;
}

/**
 * @brief
 * set parallel overlap class
 *
 * @param prl_ovlp
 */
void CutSpacing::setParallelOverlap(ObjectId id) {
    cut_sp_rules_ = id;
}

/**
 * @brief
 * constructor of SecondLayer class
 */
SecondLayer::SecondLayer() {
    second_layer_id_ = -1;
    is_stack_ = false;
}

/**
 * @brief
 * destructor of SecondLayer class
 */
SecondLayer::~SecondLayer() {}

/**
 * @brief
 * get second layer id
 *
 * @return
 */
Int32 SecondLayer::getSecondLayerId() const { return second_layer_id_; }

/**
 * @brief
 * set second layer id
 *
 * @param id
 */
void SecondLayer::setSecondLayerId(Int32 id) { second_layer_id_ = id; }

/**
 * @brief
 * get stack flag
 *
 * @return
 */
bool SecondLayer::getIsStack() const { return is_stack_; }

/**
 * @brief
 * set stack flag
 *
 * @param is_stack
 */
void SecondLayer::setIsStack(bool is_stack) { is_stack_ = is_stack; }

/**
 * @brief
 * constructor of AdjacentCuts class
 */
AdjacentCuts::AdjacentCuts() {
    cut_num_ = 0;
    ;
    cut_within_ = 0;
    is_except_same_pg_net_ = false;
}

/**
 * @brief
 * destructor of AdjacentCuts class
 */
AdjacentCuts::~AdjacentCuts() {}

/**
 * @brief
 * get cut number
 *
 * @return
 */
Int32 AdjacentCuts::getCutNum() const { return cut_num_; }

/**
 * @brief
 * set cut number
 *
 * @param cut_num
 */
void AdjacentCuts::setCutNum(Int32 cut_num) { cut_num_ = cut_num; }

/**
 * @brief
 * get cut within
 *
 * @return
 */
Int32 AdjacentCuts::getCutWithin() const { return cut_within_; }

/**
 * @brief
 * set cut within
 *
 * @param cut_within
 */
void AdjacentCuts::setCutWithin(Int32 cut_within) { cut_within_ = cut_within; }

/**
 * @brief
 * get except same PG net flag
 *
 * @return
 */
bool AdjacentCuts::getIsExceptSamePGNet() const {
    return is_except_same_pg_net_;
}

/**
 * @brief
 * set except same PG net flag
 *
 * @param flag
 */
void AdjacentCuts::setIsExceptSamePGNet(bool flag) {
    is_except_same_pg_net_ = flag;
}

/**
 * @brief
 * constructor of CutSpacingPrlOvlp
 */
CutSpacingPrlOvlp::CutSpacingPrlOvlp() { is_parallel_overlap_ = false; }

/**
 * @brief
 * destructor of CutSpacingPrlOvlp
 */
CutSpacingPrlOvlp::~CutSpacingPrlOvlp() {}

/**
 * @brief
 * get parallel overlap flag
 *
 * @return
 */
bool CutSpacingPrlOvlp::getIsParallelOverlap() const {
    return is_parallel_overlap_;
}

/**
 * @brief
 * set parallel overlap flag
 *
 * @param is_prl_ovlp
 */
void CutSpacingPrlOvlp::setIsParallelOverlap(bool is_prl_ovlp) {
    is_parallel_overlap_ = is_prl_ovlp;
}

/**
 * @brief
 * constructor of Enclosure
 */
Enclosure::Enclosure() {
    is_above_ = false;
    is_below_ = false;
    enclosure_type_ = 0;
    enclosure_rules_= 0;
}

/**
 * @brief
 * destructor of Enclosure
 */
Enclosure::~Enclosure() {}

/**
 * @brief
 * get above flag
 *
 * @return
 */
bool Enclosure::getIsAbove() const { return is_above_; }

/**
 * @brief
 * set above flag
 *
 * @param is_above
 */
void Enclosure::setIsAbove(bool is_above) { is_above_ = is_above; }

/**
 * @brief
 * get below flag
 *
 * @return
 */
bool Enclosure::getIsBelow() const { return is_below_; }

/**
 * @brief
 * set below flag
 *
 * @param is_below
 */
void Enclosure::setIsBelow(bool is_below) { is_below_ = is_below; }

/**
 * @brief
 * get eol rule flag
 *
 * @return
 */
bool Enclosure::getIsEol() const {
    return (enclosure_type_ & enclosure_is_eol);
}

/**
 * @brief
 * set eol rule flag
 *
 * @param
 */
void Enclosure::setIsEol() { enclosure_type_ |= enclosure_is_eol; }

/**
 * @brief
 * get eol class
 *
 * @return
 */
EnclosureEol *Enclosure::getEol() const {
    EnclosureEol *obj_data = addr<EnclosureEol>(enclosure_rules_);
    return obj_data;
}

/**
 * @brief
 * set eol class
 *
 * @param enc_eol
 */
void Enclosure::setEol(ObjectId id) {
    enclosure_rules_ = id;
}

/**
 * @brief
 * get overhang rule flag
 *
 * @return
 */
bool Enclosure::getIsOverhang() const {
    return (enclosure_type_ & enclosure_is_overhang);
}

/**
 * @brief
 * set overhang rule flag
 *
 * @param
 */
void Enclosure::setIsOverhang() { enclosure_type_ |= enclosure_is_overhang; }

/**
 * @brief
 * get overhang class
 *
 * @return
 */
EnclosureOverhang *Enclosure::getOverhang() const {
    EnclosureOverhang *obj_data = addr<EnclosureOverhang>(enclosure_rules_);
    return obj_data;
}

/**
 * @brief
 * set overhang class
 *
 * @param enc_overhang
 */
void Enclosure::setOverhang(ObjectId id) {
    enclosure_rules_ = id;
}

/**
 * @brief
 * constructor of EnclosureOverhang
 */
EnclosureOverhang::EnclosureOverhang() {
    overhang1_ = -1;
    overhang2_ = -1;
    min_width_ = -1;
    except_extra_cut_within_ = -1;
    min_length_ = -1;
}

/**
 * @brief
 * destructor of EnclosureOverhang
 */
EnclosureOverhang::~EnclosureOverhang() {}

/**
 * @brief
 * get overhang1
 *
 * @return
 */
Int32 EnclosureOverhang::getOverhang1() const { return overhang1_; }

/**
 * @brief
 * set overhang1
 *
 * @param overhang
 */
void EnclosureOverhang::setOverhang1(Int32 overhang) { overhang1_ = overhang; }

/**
 * @brief
 * get overhang2
 *
 * @return
 */
Int32 EnclosureOverhang::getOverhang2() const { return overhang2_; }

/**
 * @brief
 * set overhang2
 *
 * @param overhang
 */
void EnclosureOverhang::setOverhang2(Int32 overhang) { overhang2_ = overhang; }

/**
 * @brief
 * get width flag
 *
 * @return
 */
bool EnclosureOverhang::isWidth() const { return (min_width_ > 0); }

/**
 * @brief
 * get width
 *
 * @return
 */
Int32 EnclosureOverhang::getMinWidth() const { return min_width_; }

/**
 * @brief
 * set width
 *
 * @param width
 */
void EnclosureOverhang::setMinWidth(Int32 width) { min_width_ = width; }

/**
 * @brief
 * get except extra cut flag
 *
 * @return
 */
bool EnclosureOverhang::isExceptExtraCut() const {
    return (except_extra_cut_within_ > 0);
}

/**
 * @brief
 * get except extra cut within value
 *
 * @return
 */
Int32 EnclosureOverhang::getExceptExtraCutWithin() const {
    return except_extra_cut_within_;
}

/**
 * @brief
 * set except extra cut within value
 *
 * @param within
 */
void EnclosureOverhang::setExceptExtraCutWithin(Int32 within) {
    except_extra_cut_within_ = within;
}

/**
 * @brief
 * get length flag
 *
 * @return
 */
bool EnclosureOverhang::isLength() const { return (min_length_ > 0); }

/**
 * @brief
 * get min length value
 *
 * @return
 */
Int32 EnclosureOverhang::getMinLength() const { return min_length_; }

/**
 * @brief
 * set min length value
 *
 * @param length
 */
void EnclosureOverhang::setMinLength(Int32 length) { min_length_ = length; }

/**
 * @brief
 * constructor of ArraySpacing
 */
ArraySpacing::ArraySpacing() {
    is_long_array_ = false;
    via_width_ = -1;
    cut_spacing_ = -1;
    num_array_cuts_ = 0;
    array_cuts_ = 0;
    array_spacing_ = 0;
}

/**
 * @brief
 * destructor of ArraySpacing
 */
ArraySpacing::~ArraySpacing() {
    if (array_cuts_) {
        array_cuts_ = 0;
    }
    if (array_spacing_) {
        array_spacing_ = 0;
    }
}

/**
 * @brief
 * get long array flag
 *
 * @return
 */
bool ArraySpacing::getIsLongArray() const { return is_long_array_; }

/**
 * @brief
 * set long array flag
 *
 * @param is_long_array
 */
void ArraySpacing::setIsLongArray(bool is_long_array) {
    is_long_array_ = is_long_array;
}

/**
 * @brief
 * get via width flag
 *
 * @return
 */
bool ArraySpacing::hasViaWidth() const { return (via_width_ > 0); }

/**
 * @brief
 * get via width value
 *
 * @return
 */
Int32 ArraySpacing::getViaWidth() const { return via_width_; }

/**
 * @brief
 * set via width value
 *
 * @param via_width
 */
void ArraySpacing::setViaWidth(Int32 via_width) { via_width_ = via_width; }

/**
 * @brief
 * get cut spacing value
 *
 * @return
 */
Int32 ArraySpacing::getCutSpacing() const { return cut_spacing_; }

/**
 * @brief
 * set cut spacing value
 *
 * @param cut_spacing
 */
void ArraySpacing::setCutSpacing(Int32 cut_spacing) {
    cut_spacing_ = cut_spacing;
}

/**
 * @brief
 * get num array cuts
 *
 * @return
 */
Int32 ArraySpacing::getNumArrayCuts() const { return num_array_cuts_; }

/**
 * @brief
 * set num array cuts
 *
 * @param num
 */
void ArraySpacing::setNumArrayCuts(Int32 num) {
    num_array_cuts_ = num;
    //array_cuts_ = new Int32[num];
    //array_spacing_ = new Int32[num];
}

/**
 * @brief
 * get array cuts poInt32er
 *
 * @return
 */
ArrayObject<Int32> *ArraySpacing::getArrayCuts() const { 
    if (array_cuts_ != 0) {
        ArrayObject<Int32> *array_cuts_array = addr<ArrayObject<Int32>>(array_cuts_);
        return array_cuts_array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * get array cuts value by index
 *
 * @return
 */
Int32 ArraySpacing::getArrayCuts(Int32 idx) const {
    if (idx >= num_array_cuts_) {
        return -1;
    }
    
    if (array_cuts_ == 0) 
        return -1;
    ArrayObject<Int32> *array_ptr = addr< ArrayObject<Int32> >(array_cuts_);
    if (array_ptr == nullptr)
        return -1;

    return (*array_ptr)[idx];
}

/**
 * @brief
 * set array cuts value by index
 *
 * @param idx, array_cuts
 */
void ArraySpacing::addArrayCuts(Int32 array_cuts) {
    ArrayObject<Int32> *array_ptr = nullptr;
    if (array_cuts_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<Int32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        array_cuts_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<Int32> >(array_cuts_);
    }

    if (array_ptr) array_ptr->pushBack(array_cuts);
}

/**
 * @brief
 * get array spacing poInt32er
 *
 * @return
 */
ArrayObject<Int32> *ArraySpacing::getArraySpacing() const { 
    if (array_spacing_ != 0) {
        ArrayObject<Int32> *array_spacing_array = addr<ArrayObject<Int32>>(array_spacing_);
        return array_spacing_array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * get array spacing value by index
 *
 * @return
 */
Int32 ArraySpacing::getArraySpacing(Int32 idx) const {
    if (idx >= num_array_cuts_) {
        return -1;
    }
    
    if (array_spacing_ == 0) 
        return -1;
    ArrayObject<Int32> *array_ptr = addr< ArrayObject<Int32> >(array_spacing_);
    if (array_ptr == nullptr)
        return -1;

    return (*array_ptr)[idx];
}

/**
 * @brief
 * set array spacing value by index
 *
 * @param idx, array_spacing
 */
void ArraySpacing::addArraySpacing(Int32 array_spacing) {
    ArrayObject<Int32> *array_ptr = nullptr;
    if (array_spacing_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<Int32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        array_spacing_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<Int32> >(array_spacing_);
    }

    if (array_ptr) array_ptr->pushBack(array_spacing);
}

/**
 * @brief
 * set array cuts/spacing values by number
 *
 * @param num_array_cuts, array_cuts, array_spacing
 */
void ArraySpacing::setArray(Int32 num_array_cuts, Int32 *array_cuts,
                            Int32 *array_spacing) {
    num_array_cuts_ = num_array_cuts;
    for (Int32 i = 0; i < num_array_cuts; i++) {
        addArrayCuts(array_cuts[i]);
        addArraySpacing(array_spacing[i]);
    }
}

}  // namespace db
}  // namespace open_edi
