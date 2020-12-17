/* @file  implant_rule.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/tech/implant_rule.h"

#include "db/util/vector_object_var.h"
#include "db/core/cell.h"

namespace open_edi {
namespace db {

/**
 * @brief
 * release the allocated memory
 */
ImplantCoreEdgeLength::~ImplantCoreEdgeLength() {
}

/**
 * @brief
 * get min length
 *
 * @return
 */
UInt32 ImplantCoreEdgeLength::getMinLength() const { return min_length_; }

/**
 * @brief
 * set min length
 *
 * @param len
 */
void ImplantCoreEdgeLength::setMinLength(UInt32 len) { min_length_ = len; }

/**
 * @brief
 * get the number of adjacent edges
 */
UInt32 ImplantCoreEdgeLength::getNumExceptAdjLength() const {
    return except_adj_length_num_;
}

/**
 * @brief
 * set the number of adjacent edges
 *
 * @param num
 */
void ImplantCoreEdgeLength::setNumExceptAdjLength(UInt32 num) {
    except_adj_length_num_ = num;
    // exact_edge_length_ =
    //     (UInt32*)calloc(except_adj_length_num_, sizeof(UInt32));
    // adj_length_ = (UInt32*)calloc(except_adj_length_num_, sizeof(UInt32));
    // is_exact_adj_length_ = (bool*)calloc(except_adj_length_num_, sizeof(bool));
}

/**
 * @brief
 * get exact edge length
 *
 * @param idx
 *
 * @return
 */
UInt32 ImplantCoreEdgeLength::getExactEdgeLength(UInt32 idx) const {
    if (idx >= except_adj_length_num_) {
        return 0;
    }
    
    if (exact_edge_length_ == 0) 
        return -1;
    ArrayObject<UInt32> *array_ptr = addr< ArrayObject<UInt32> >(exact_edge_length_);
    if (array_ptr == nullptr)
        return -1;

    return (*array_ptr)[idx];
}

/**
 * @brief
 * set exact edge length
 *
 * @param idx
 * @param len
 */
void ImplantCoreEdgeLength::addExactEdgeLength(UInt32 len) {
    ArrayObject<UInt32> *array_ptr = nullptr;
    if (exact_edge_length_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<UInt32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        exact_edge_length_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<UInt32> >(exact_edge_length_);
    }

    if (array_ptr) array_ptr->pushBack(len);

}

/**
 * @brief
 * get adjacent length
 *
 * @param idx
 *
 * @return
 */
UInt32 ImplantCoreEdgeLength::getAdjLength(UInt32 idx) const {
    if (idx >= except_adj_length_num_) {
        return 0;
    }
    
    if (adj_length_ == 0) 
        return -1;
    ArrayObject<UInt32> *array_ptr = addr< ArrayObject<UInt32> >(adj_length_);
    if (array_ptr == nullptr)
        return -1;

    return (*array_ptr)[idx];
}

/**
 * @brief
 * set adjacent length
 *
 * @param idx
 * @param len
 */
void ImplantCoreEdgeLength::addAdjLength( UInt32 len) {
    ArrayObject<UInt32> *array_ptr = nullptr;
    if (adj_length_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<UInt32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        adj_length_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<UInt32> >(adj_length_);
    }

    if (array_ptr) array_ptr->pushBack(len);
}

/**
 * @brief
 * get EXACTADJACENTLENGTH flag
 *
 * @param idx
 *
 * @return
 */
bool ImplantCoreEdgeLength::isExactAdjLength(UInt32 idx) const {
    if (idx >= except_adj_length_num_) {
        return false;
    }
    
    if (is_exact_adj_length_ == 0) 
        return false;
    ArrayObject<bool> *array_ptr = addr< ArrayObject<bool> >(is_exact_adj_length_);
    if (array_ptr == nullptr)
        return -1;

    return (*array_ptr)[idx];
}

/**
 * @brief
 * set EXACTADJACENTLENGTH flag
 *
 * @param idx
 * @param v
 */
void ImplantCoreEdgeLength::addIsExactAdjLength(bool v) {
    ArrayObject<bool> *array_ptr = nullptr;
    if (is_exact_adj_length_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<bool>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        is_exact_adj_length_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<bool> >(is_exact_adj_length_);
    }

    if (array_ptr) array_ptr->pushBack(v);
}

/**
 * @brief
 * release the allocated memory
 */
ImplantSpacing::~ImplantSpacing() {
    // TODO: free layers_
}

/**
 * @brief
 * get min spacing
 *
 * @return
 */
UInt32 ImplantSpacing::getMinSpacing() const { return min_spacing_; }

/**
 * @brief
 * set min spacing
 *
 * @param sp
 */
void ImplantSpacing::setMinSpacing(UInt32 sp) { min_spacing_ = sp; }

/**
 * @brief
 * get prl
 *
 * @return
 */
UInt32 ImplantSpacing::getPRL() const { return prl_; }

/**
 * @brief
 * set prl
 *
 * @param prl
 */
void ImplantSpacing::setPRL(UInt32 prl) { prl_ = prl; }

/**
 * @brief
 * get length
 *
 * @return
 */
UInt32 ImplantSpacing::getLength() const { return length_; }

/**
 * @brief
 * set length
 *
 * @param len
 */
void ImplantSpacing::setLength(UInt32 len) { length_ = len; }

/**
 * @brief
 * set secondary layer index
 *
 * @param layer_id
 */
void ImplantSpacing::setLayer2Id(Int32 layer_id) {
    SecondLayer* ptr = addr<SecondLayer>(sec_layer_);
    if (ptr) {
        ptr->setSecondLayerId(layer_id);
    }
}

/**
 * @brief
 * get secondary layer index
 *
 * @return
 */
Int32 ImplantSpacing::getLayer2Id() const {
    SecondLayer* ptr = addr<SecondLayer>(sec_layer_);
    return ptr ? ptr->getSecondLayerId() : -1;
}

void ImplantSpacing::setLayer2(ObjectId l) { sec_layer_ = l; }

/**
 * @brief
 * get the number of layers
 *
 * @return
 */
UInt32 ImplantSpacing::getNumLayers() const { return layers_num_; }

/**
 * @brief
 * set the number of layers
 *
 * @param num
 */
void ImplantSpacing::setNumLayers(UInt32 num) { layers_num_ = num; }

/**
 * @brief
 * indicate whether there is HORIZONTAL|VERTICAL PRL
 *
 * @return
 */
bool ImplantSpacing::hasPRL() const { return prl_; }

/**
 * @brief
 * get horizontal flag
 *
 * @return
 */
bool ImplantSpacing::isHorizonal() const { return dir_ == 0; }

/**
 * @brief
 * get vertical flag
 *
 * @return
 */
bool ImplantSpacing::isVertical() const { return dir_ == 1; }

/**
 * @brief
 * set HORIZONTAL or VERTICAL
 *
 * @param v
 */
void ImplantSpacing::setDir(bool v) { dir_ = v ? 1 : 0; }

/**
 * @brief
 * get EXCEPTABUTTED flag
 *
 * @return
 */
bool ImplantSpacing::isExceptAbutted() const { return is_except_abutted_; }

/**
 * @brief
 * set EXCEPTABUTTED flag
 *
 * @param v
 */
void ImplantSpacing::setIsExceptAbutted(bool v) { is_except_abutted_ = v; }

/**
 * @brief
 * get EXCEPTCORNERTOUCH flag
 *
 * @return
 */
bool ImplantSpacing::isExceptCornerTouch() const {
    return is_except_corner_touch_;
}

/**
 * @brief
 * set EXCEPTCORNERTOUCH flag
 *
 * @param v
 */
void ImplantSpacing::setIsExceptCornerTouch(bool v) {
    is_except_corner_touch_ = v;
}

/**
 * @brief
 * indicate whether there is INTERSECTLAYERS or not
 *
 * @return
 */
bool ImplantSpacing::hasIntersectLayer() const { return layers_; }

/**
 * @brief
 * get INTERSECTLAYERS layers
 *
 * @return
 */
Layer** ImplantSpacing::getLayers() const { return layers_; }

/**
 * @brief
 * set INTERSECTLAYERS layers
 *
 * @param layers
 */
void ImplantSpacing::setLayers(Layer** layers) { layers_ = layers; }

/**
 * @brief
 * release the allocated memory
 */
ImplantWidth::~ImplantWidth() {
}

/**
 * @brief
 * get width
 *
 * @return
 */
UInt32 ImplantWidth::getWidth() const { return width_; }

/**
 * @brief
 * set width
 *
 * @param w
 */
void ImplantWidth::setWidth(UInt32 w) { width_ = w; }

/**
 * @brief
 * get length
 *
 * @return
 */
UInt32 ImplantWidth::getLength() const { return length_; }

/**
 * @brief
 * set length
 *
 * @param len
 */
void ImplantWidth::setLength(UInt32 len) { length_ = len; }

/**
 * @brief
 * set secondary layer index
 *
 * @param layer_id
 */
void ImplantWidth::setLayer2Id(Int32 layer_id) {
    SecondLayer* ptr = addr<SecondLayer>(sec_layer_);
    if (ptr) {
        ptr->setSecondLayerId(layer_id);
    }
}

/**
 * @brief
 * get secondary layer index
 *
 * @return
 */
Int32 ImplantWidth::getLayer2Id() const {
    SecondLayer* ptr = addr<SecondLayer>(sec_layer_);
    return ptr ? ptr->getSecondLayerId() : -1;
}

/**
 * @brief
 * get ZEROPRL flag
 *
 * @return
 */
bool ImplantWidth::isZeroPRL() const { return is_zero_prl_; }

/**
 * @brief
 * set ZEROPRL flag
 *
 * @param v
 */
void ImplantWidth::setIsZeroPRL(bool v) { is_zero_prl_ = v ? 1 : 0; }

/**
 * @brief
 * get EXCEPTCORNERTOUCH flag
 *
 * @return
 */
bool ImplantWidth::isExceptCornerTouch() const {
    return is_except_corner_touch_;
}

/**
 * @brief
 * set EXCEPTCORNERTOUCH flag
 *
 * @param v
 */
void ImplantWidth::setIsExceptCornerTouch(bool v) {
    is_except_corner_touch_ = v ? 1 : 0;
}

/**
 * @brief
 * get check implant group
 *
 * @return
 */
ArrayObject<ObjectId>* ImplantWidth::getGroup() const { 
    if (group_)
        return addr<ArrayObject<ObjectId>>(group_);
    
    return nullptr;
}

/**
 * @brief
 * set check implant group
 *
 * @param group
 */
void ImplantWidth::setGroup(ObjectId group) { group_ = group; }


/**
 * @brief
 * get core edge length rule
 *
 * @return
 */
ImplantCoreEdgeLength* ImplantLayerRule::getCoreEdgeLength(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (core_edge_length_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(core_edge_length_);
    }
    if (vct) {
        ImplantCoreEdgeLength *obj_data = addr<ImplantCoreEdgeLength>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

ArrayObject<ObjectId> *ImplantLayerRule::getCoreEdgeLengths() const {
    if (core_edge_length_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(core_edge_length_);
        return array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * set core edge length
 *
 * @param iel
 */
void ImplantLayerRule::addCoreEdgeLength(ObjectId iel) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (core_edge_length_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        core_edge_length_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(core_edge_length_);
    }
    if (vct) vct->pushBack(iel);
}

/**
 * @brief
 * get spacing list
 *
 * @return
 */
ImplantSpacing* ImplantLayerRule::getSpacingList(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (spacing_list_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(spacing_list_);
    }
    if (vct) {
        ImplantSpacing *obj_data = addr<ImplantSpacing>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

ArrayObject<ObjectId> *ImplantLayerRule::getSpacingLists() const {
    if (spacing_list_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(spacing_list_);
        return array;
    } else {
        return nullptr;
    }
}
/**
 * @brief
 * set spacing list
 *
 * @param is
 */
void ImplantLayerRule::addSpacingList(ObjectId is) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (spacing_list_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        spacing_list_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(spacing_list_);
    }
    if (vct) vct->pushBack(is);
}

/**
 * @brief
 * get min enclosed area rule
 *
 * @return
 */
MinEnclArea* ImplantLayerRule::getMinEnclArea(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (min_encl_area_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(min_encl_area_);
    }
    if (vct) {
        MinEnclArea *obj_data = addr<MinEnclArea>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

ArrayObject<ObjectId> *ImplantLayerRule::getMinEnclAreas() const {
    if (min_encl_area_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(min_encl_area_);
        return array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * set min enclosed area rule
 *
 * @param mea
 */
void ImplantLayerRule::addMinEnclArea(ObjectId mea) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (min_encl_area_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        min_encl_area_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(min_encl_area_);
    }
    if (vct) vct->pushBack(mea);
}

/**
 * @brief
 * get width rule
 *
 * @return
 */
ImplantWidth* ImplantLayerRule::getWidth(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (width_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(width_);
    }
    if (vct) {
        ImplantWidth *obj_data = addr<ImplantWidth>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

ArrayObject<ObjectId> *ImplantLayerRule::getWidths() const {
    if (width_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(width_);
        return array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * set width rule
 *
 * @param iw
 */
void ImplantLayerRule::addWidth(ObjectId iw) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (width_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        width_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(width_);
    }
    if (vct) vct->pushBack(iw);
}

}  // namespace db
}  // namespace open_edi
