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
#include "db/core/term.h"

namespace open_edi {
namespace db {

bool transformByInst(const Inst *inst, Point &pt) {
    ediAssert(inst != nullptr);
    Transform transform(inst);
    transform.transformCoord(pt);
    return true;
}

bool transformByInst(const Inst *inst, Box &box) {
    ediAssert(inst != nullptr);
    Transform transform(inst);
    transform.transformBox(box);
    return true;
}


bool transformByIOPin(const Pin *io_pin, Point &pt) {
    if (io_pin == nullptr) {
        return false;
    }
    Inst *inst = io_pin->getInst();
    Term *term = io_pin->getTerm();
    //For IO pin, inst should be nullptr.
    if (term == nullptr || inst != nullptr) {
        return false;
    }
    Transform transform(io_pin);
    transform.transformCoord(pt);
    return true;
}

bool transformByIOPin(const Pin *io_pin, Box &box) {
    if (io_pin == nullptr) {
        return false;
    }
    Inst *inst = io_pin->getInst();
    Term *term = io_pin->getTerm();
    //For IO pin, inst should be nullboxr.
    if (term == nullptr || inst != nullptr) {
        return false;
    }
    Transform transform(io_pin);
    transform.transformBox(box);
    return true;
}

Transform::Transform(const Inst* inst) {
    ediAssert(inst != nullptr);
    Point pt = inst->getLocation();
    Orient ort = inst->getOrient();
    Cell* cell = inst->getMaster();
    ediAssert(cell != nullptr);

    int size_x = cell->getSizeX();
    int size_y = cell->getSizeY();
    Point origin;
    origin.setX(cell->getOriginX());
    origin.setY(cell->getOriginY());

    orient_ = ort;
    __setOffset(pt, origin, size_x, size_y);
}

// For IO pin, we only take the first Box from term/port/layergeom/geom.
Transform::Transform(const Pin *io_pin) {
    ediAssert(io_pin != nullptr);
    Inst *inst = io_pin->getInst();
    Term *term = io_pin->getTerm();
    //For IO pin, inst should be nullptr.
    ediAssert(term != nullptr && inst == nullptr);
    Port *port = nullptr;
    for (int index = 0; index < term->getPortNum(); ++index) {
        port = term->getPort(index);
        if (port->getStatus() == PlaceStatus::kPlaced) {
            break;
        }
    }

    Point placed_pt = {0, 0};
    Orient ort = Orient::kN;
    Point origin = {0, 0};

    if (port != nullptr) {
        placed_pt = port->getLocation();
        ort = port->getOrient();
    }
    
    orient_ = ort;
    // Based on lef/def_5.8 manual,
    // the origin is using {0 0}:
    __setOffset(placed_pt, origin);
}

// For the offset on the IO pin:
void Transform::__setOffset(
    Point &placed_pt, Point &origin
) {
    offset_.setX(placed_pt.getX() - origin.getX());
    offset_.setY(placed_pt.getY() - origin.getY());
}

// For the offset inside Inst's bounding-box
void Transform::__setOffset(
    Point &placed_pt, Point &origin,
    int size_x, int size_y
) {
    switch (orient_) {
    case Orient::kN:
        offset_.setX(placed_pt.getX() - origin.getX());
        offset_.setY(placed_pt.getY() - origin.getY());
        break;
    case Orient::kW:
        offset_.setX(placed_pt.getX() + origin.getY() + size_y);
        offset_.setY(placed_pt.getY() - origin.getX());
        break;
    case Orient::kS:
        offset_.setX(placed_pt.getX() + origin.getX() + size_x);
        offset_.setY(placed_pt.getY() + origin.getY() + size_y);
        break;
    case Orient::kE:
        offset_.setX(placed_pt.getX() - origin.getY());
        offset_.setY(placed_pt.getY() + origin.getX() + size_x);
        break;
    case Orient::kFN:
        offset_.setX(placed_pt.getX() + origin.getX() + size_x);
        offset_.setY(placed_pt.getY() - origin.getY());
        break;
    case Orient::kFE:
        offset_.setX(placed_pt.getX() + origin.getY() + size_y);
        offset_.setY(placed_pt.getY() + origin.getX() + size_x);
        break;
    case Orient::kFS:
        offset_.setX(placed_pt.getX() - origin.getX());
        offset_.setY(placed_pt.getY() + origin.getY() + size_y);
        break;
    case Orient::kFW:
        offset_.setX(placed_pt.getX() - origin.getY());
        offset_.setY(placed_pt.getY() - origin.getX());
        break;
    }
}

void Transform::transformCoord(Point &pt) {
    Point result_pt;
    switch (orient_) {
    case Orient::kN:
        result_pt.setX(offset_.getX() + pt.getX());
        result_pt.setY(offset_.getY() + pt.getY());
        break;
    case Orient::kW:
        result_pt.setX(offset_.getX() - pt.getY());
        result_pt.setY(offset_.getY() + pt.getX());
        break;
    case Orient::kS:
        result_pt.setX(offset_.getX() - pt.getX());
        result_pt.setY(offset_.getY() - pt.getY());
        break;
    case Orient::kE:
        result_pt.setX(offset_.getX() + pt.getY());
        result_pt.setY(offset_.getY() - pt.getX());
        break;
    case Orient::kFN:
        result_pt.setX(offset_.getX() - pt.getX());
        result_pt.setY(offset_.getY() + pt.getY());
        break;
    case Orient::kFE:
        result_pt.setX(offset_.getX() - pt.getY());
        result_pt.setY(offset_.getY() - pt.getX());
        break;
    case Orient::kFS:
        result_pt.setX(offset_.getX() + pt.getX());
        result_pt.setY(offset_.getY() - pt.getY());
        break;
    case Orient::kFW:
        result_pt.setX(offset_.getX() + pt.getY());
        result_pt.setY(offset_.getY() + pt.getX());
        break;
    }
    pt = result_pt;
}

void Transform::transformBox(Box &box) {
    Point ll(box.getLLX(), box.getLLY());
    Point ur(box.getURX(), box.getURY());

    transformCoord(ll);
    transformCoord(ur);
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
}

}  // namespace db 
}  // namespace open_edi
