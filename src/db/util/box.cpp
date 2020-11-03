/* @file  Box.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/util/box.h"
EDI_BEGIN_NAMESPACE

Box::Box() {
    llx_ = 0;
    lly_ = 0;
    urx_ = 0;
    ury_ = 0;
}

Box::Box(int llx, int lly, int urx, int ury) {
    setLLX(llx);
    setLLY(lly);
    setURX(urx);
    setURY(ury);
}

void Box::setBox(int llx, int lly, int urx, int ury) {
    setLLX(llx);
    setLLY(lly);
    setURX(urx);
    setURY(ury);
}

void Box::setBox(const Box &box) {
    llx_ = box.getLLX();
    lly_ = box.getLLY();
    urx_ = box.getURX();
    ury_ = box.getURY();
}

void Box::setBox(const Box *box) {
    llx_ = box->getLLX();
    lly_ = box->getLLY();
    urx_ = box->getURX();
    ury_ = box->getURY();
}

Box &Box::operator=(const Box &box) {
    llx_ = box.getLLX();
    lly_ = box.getLLY();
    urx_ = box.getURX();
    ury_ = box.getURY();
    return *this;
}

bool Box::operator==(const Box &box) {
    return (llx_ == box.getLLX() && lly_ == box.getLLY() &&
            urx_ == box.getURX() && ury_ == box.getURY());
}

void Box::maxBox(const Box &box) {
    if (llx_ == 0 && lly_ == 0 && urx_ == 0 && ury_ == 0) {
        llx_ = box.getLLX();
        lly_ = box.getLLY();
        urx_ = box.getURX();
        ury_ = box.getURY();
    } else {
        if (box.getLLX() < llx_) {
            llx_ = box.getLLX();
        }
        if (box.getLLY() < lly_) {
            lly_ = box.getLLY();
        }
        if (box.getURX() > urx_) {
            urx_ = box.getURX();
        }
        if (box.getURY() > ury_) {
            ury_ = box.getURY();
        }
    }
}

bool Box::isIntersect(const Box &box) const {
    return (llx_ <= box.getURX() && lly_ <= box.getURY() &&
            urx_ >= box.getLLX() && ury_ >= box.getLLY());
}

int Box::getWidth() {
    int width = (getURX() - getLLX());
    if (width >= 0) {
        return width;
    } else {
        return -width;
    }
}

int Box::getHeight() {
    int height = (getURY() - getLLY());
    if (height >= 0) {
        return height;
    } else {
        return -height;
    }
}

EDI_END_NAMESPACE
