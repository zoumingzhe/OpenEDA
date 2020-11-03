/* @file  fill.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/core/fill.h"
namespace open_edi {
namespace db {

Fill::Fill() {
    is_layer_ = false;
    is_via_ = false;
    is_opc_ = false;
    layer_id_ = -1;
    for (Int32 i = 0; i < 3; i++) {
        mask_[i] = 0;
    }
    via_ = 0;
}

Fill::~Fill() {}

bool Fill::getIsLayer() const { return is_layer_; }

void Fill::setIsLayer(bool is_layer) { is_layer_ = is_layer; }

bool Fill::getIsVia() const { return is_via_; }

void Fill::setIsVia(bool is_via) { is_via_ = is_via; }

bool Fill::getIsOpc() const { return is_opc_; }

void Fill::setIsOpc(bool is_opc) { is_opc_ = is_opc; }

bool Fill::getIsRect(Int32 id) const {
    return (getIsLayer() && points_array_[id].size() == 2);
}

Int32 Fill::getLayerId() const { return layer_id_; }

void Fill::setLayerId(Int32 id) { layer_id_ = id; }

Int32 Fill::getLayerMask() const { return mask_[0]; }

void Fill::setLayerMask(Int32 mask) { mask_[0] = mask; }

Int32 Fill::getViaBotMask() const { return mask_[0]; }

void Fill::setViaBotMask(Int32 mask) { mask_[0] = mask; }

Int32 Fill::getViaCutMask() const { return mask_[1]; }

void Fill::setViaCutMask(Int32 mask) { mask_[1] = mask; }

Int32 Fill::getViaTopMask() const { return mask_[2]; }

void Fill::setViaTopMask(Int32 mask) { mask_[2] = mask; }

std::vector<std::vector<Point> > *Fill::getPointsArray() {
    return &points_array_;
}

void Fill::addPoints(const std::vector<Point> &points) {
    points_array_.push_back(points);
}

ViaMaster *Fill::getVia() const { return via_; }

void Fill::setVia(ViaMaster *via) { via_ = via; }

}  // namespace db
}  // namespace open_edi
