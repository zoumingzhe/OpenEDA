/* @file  transform.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/util/transform.h"
#include "db/core/inst.h"
#include "db/core/cell.h"

namespace open_edi {
namespace db {

bool transformByInst(Inst *inst, Point &pt) {
    Transform transform(inst);
    transform.transformCoord(pt);
    return true;
}

bool transformByInst(Inst *inst, Box &box) {
    Transform transform(inst);
    Point ll(box.getLLX(), box.getLLY());
    Point ur(box.getURX(), box.getURY());

    transform.transformCoord(ll);
    transform.transformCoord(ur);
    if (ll.getX() < ur.getX()) {
        box.setLLX(ll.getX());
        box.setURX(ur.getX());
    } else {
        box.setLLX(ur.getX());
        box.setURX(ll.getX());
    }
    if (ll.getY() < ur.getY()) {
        box.setLLY(ll.getY());
        box.setURY(ur.getY());
    } else {
        box.setLLY(ur.getY());
        box.setURY(ll.getY());
    }
    return true;
}

Transform::Transform(Inst* inst) {
    ediAssert(inst != nullptr);
    Point pt = inst->getLocation();
    Orient ort = inst->getOrient();
    Cell* cell = inst->getMaster();
    ediAssert(cell != nullptr);

    int size_x = cell->getSizeX();
    int size_y = cell->getSizeY();
    int origin_x = cell->getOriginX();
    int origin_y = cell->getOriginY();
    
    switch (ort) {
    case Orient::kN:
        offset_.setX(pt.getX() - origin_x);
        offset_.setY(pt.getY() - origin_y);
        break;
    case Orient::kW:
        offset_.setX(pt.getX() + origin_y + size_y);
        offset_.setY(pt.getY() - origin_x);
        break;
    case Orient::kS:
        offset_.setX(pt.getX() + origin_x + size_x);
        offset_.setY(pt.getY() + origin_y + size_y);
        break;
    case Orient::kE:
        offset_.setX(pt.getX() - origin_y);
        offset_.setY(pt.getY() + origin_x + size_x);
        break;
    case Orient::kFN:
        offset_.setX(pt.getX() + origin_x + size_x);
        offset_.setY(pt.getY() - origin_y);
        break;
    case Orient::kFE:
        offset_.setX(pt.getX() + origin_y + size_y);
        offset_.setY(pt.getY() + origin_x + size_x);
        break;
    case Orient::kFS:
        offset_.setX(pt.getX() - origin_x);
        offset_.setY(pt.getY() + origin_y + size_y);
        break;
    case Orient::kFW:
        offset_.setX(pt.getX() - origin_y);
        offset_.setY(pt.getY() - origin_x);
        break;
    }
    orient_ = ort;
}

void Transform::transformCoord(Point &pt) {
    switch (orient_) {
    case Orient::kN:
        pt.setX(offset_.getX() + pt.getX());
        pt.setY(offset_.getY() + pt.getY());
        break;
    case Orient::kW:
        pt.setX(offset_.getX() - pt.getY());
        pt.setY(offset_.getY() + pt.getX());
        break;
    case Orient::kS:
        pt.setX(offset_.getX() - pt.getX());
        pt.setY(offset_.getY() - pt.getY());
        break;
    case Orient::kE:
        pt.setX(offset_.getX() + pt.getY());
        pt.setY(offset_.getY() - pt.getX());
        break;
    case Orient::kFN:
        pt.setX(offset_.getX() - pt.getX());
        pt.setY(offset_.getY() + pt.getY());
        break;
    case Orient::kFE:
        pt.setX(offset_.getX() - pt.getY());
        pt.setY(offset_.getY() - pt.getX());
        break;
    case Orient::kFS:
        pt.setX(offset_.getX() + pt.getX());
        pt.setY(offset_.getY() - pt.getY());
        break;
    case Orient::kFW:
        pt.setX(offset_.getX() + pt.getY());
        pt.setY(offset_.getY() + pt.getX());
        break;
    }
}

}  // namespace db 
}  // namespace open_edi
