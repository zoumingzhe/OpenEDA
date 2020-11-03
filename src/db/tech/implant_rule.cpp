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

namespace open_edi {
namespace db {

/**
 * @brief
 * release the allocated memory
 */
ImplantCoreEdgeLength::~ImplantCoreEdgeLength() {
    if (exact_edge_length_) free(exact_edge_length_);
    if (adj_length_) free(adj_length_);
    if (is_exact_adj_length_) free(is_exact_adj_length_);
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
    exact_edge_length_ =
        (UInt32*)calloc(except_adj_length_num_, sizeof(UInt32));
    adj_length_ = (UInt32*)calloc(except_adj_length_num_, sizeof(UInt32));
    is_exact_adj_length_ = (bool*)calloc(except_adj_length_num_, sizeof(bool));
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
    return idx < except_adj_length_num_ ? exact_edge_length_[idx] : 0;
}

/**
 * @brief
 * set exact edge length
 *
 * @param idx
 * @param len
 */
void ImplantCoreEdgeLength::setExactEdgeLength(UInt32 idx, UInt32 len) {
    if (idx < except_adj_length_num_) {
        exact_edge_length_[idx] = len;
    }
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
    return idx < except_adj_length_num_ ? adj_length_[idx] : 0;
}

/**
 * @brief
 * set adjacent length
 *
 * @param idx
 * @param len
 */
void ImplantCoreEdgeLength::setAdjLength(UInt32 idx, UInt32 len) {
    if (idx < except_adj_length_num_) {
        adj_length_[idx] = len;
    }
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
    return idx < except_adj_length_num_ ? is_exact_adj_length_[idx] : false;
}

/**
 * @brief
 * set EXACTADJACENTLENGTH flag
 *
 * @param idx
 * @param v
 */
void ImplantCoreEdgeLength::setIsExactAdjLength(UInt32 idx, bool v) {
    if (idx < except_adj_length_num_) {
        is_exact_adj_length_[idx] = v;
    }
}

/**
 * @brief
 * release the allocated memory
 */
ImplantSpacing::~ImplantSpacing() {
    if (sec_layer_) delete sec_layer_;
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
    if (sec_layer_) {
        sec_layer_->setSecondLayerId(layer_id);
    }
}

/**
 * @brief
 * get secondary layer index
 *
 * @return
 */
Int32 ImplantSpacing::getLayer2Id() const {
    return sec_layer_ ? sec_layer_->getSecondLayerId() : -1;
}

void ImplantSpacing::setLayer2(SecondLayer* l) { sec_layer_ = l; }

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
 * get next spacing
 *
 * @return
 */
ImplantSpacing* ImplantSpacing::getNext() const { return next_; }

/**
 * @brief
 * set next spacing
 *
 * @param n
 */
void ImplantSpacing::setNext(ImplantSpacing* n) { next_ = n; }

/**
 * @brief
 * release the allocated memory
 */
ImplantWidth::~ImplantWidth() {
    if (sec_layer_) delete sec_layer_;
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
    if (sec_layer_) {
        sec_layer_->setSecondLayerId(layer_id);
    }
}

/**
 * @brief
 * get secondary layer index
 *
 * @return
 */
Int32 ImplantWidth::getLayer2Id() const {
    return sec_layer_ ? sec_layer_->getSecondLayerId() : -1;
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
ImplantGroup* ImplantWidth::getGroup() const { return group_; }

/**
 * @brief
 * set check implant group
 *
 * @param group
 */
void ImplantWidth::setGroup(ImplantGroup* group) { group_ = group; }

/**
 * @brief
 * get next width;
 *
 * @return
 */
ImplantWidth* ImplantWidth::getNext() const { return next_; }

/**
 * @brief
 * set next width
 *
 * @param iw
 */
void ImplantWidth::setNext(ImplantWidth* iw) { next_ = iw; }

/**
 * @brief
 * get core edge length rule
 *
 * @return
 */
ImplantCoreEdgeLength* ImplantLayerRule::getCoreEdgeLength() const {
    return core_edge_length_;
}

/**
 * @brief
 * set core edge length
 *
 * @param iel
 */
void ImplantLayerRule::setCoreEdgeLength(ImplantCoreEdgeLength* iel) {
    core_edge_length_ = iel;
}

/**
 * @brief
 * get spacing list
 *
 * @return
 */
ImplantSpacing* ImplantLayerRule::getSpacingList() const {
    return spacing_list_;
}

/**
 * @brief
 * set spacing list
 *
 * @param is
 */
void ImplantLayerRule::setSpacingList(ImplantSpacing* is) {
    spacing_list_ = is;
}

/**
 * @brief
 * get min enclosed area rule
 *
 * @return
 */
MinEnclArea* ImplantLayerRule::getMinEnclArea() const { return min_encl_area_; }

/**
 * @brief
 * set min enclosed area rule
 *
 * @param mea
 */
void ImplantLayerRule::setMinEnclArea(MinEnclArea* mea) {
    min_encl_area_ = mea;
}

/**
 * @brief
 * get width rule
 *
 * @return
 */
ImplantWidth* ImplantLayerRule::getWidth() const { return width_; }

/**
 * @brief
 * set width rule
 *
 * @param iw
 */
void ImplantLayerRule::setWidth(ImplantWidth* iw) { width_ = iw; }

}  // namespace db
}  // namespace open_edi
