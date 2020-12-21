/* @file  wire.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/core/wire.h"

#include "db/core/db.h"
#include "db/tech/tech.h"
#include "db/util/array.h"
namespace open_edi {
namespace db {

std::map<ObjectId, int> wire_status_map;
std::map<ObjectId, int> wire_mask_map;
std::map<ObjectId, bool> wire_virtual_map;
/**
 * @brief Construct a new Wire Patch:: Wire Patch object
 *
 */
WirePatch::WirePatch() {
    delta_x_1_ = 0;
    delta_x_2_ = 0;
    delta_y_1_ = 0;
    delta_y_2_ = 0;
    loc_x_ = 0;
    loc_y_ = 0;
    layer_ = 0;
}

/**
 * @brief Construct a new Wire Patch:: Wire Patch object
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 */
WirePatch::WirePatch(int x1, int y1, int x2, int y2) {
    delta_x_1_ = x1;
    delta_x_2_ = x2;
    delta_y_1_ = y1;
    delta_y_2_ = y2;
}

/**
 * @brief  get X1
 *
 * @return int
 */
int WirePatch::getX1() const { return delta_x_1_; }

/**
 * @brief get X2
 *
 * @return int
 */
int WirePatch::getX2() const { return delta_x_2_; }

/**
 * @brief get Y1
 *
 * @return int
 */
int WirePatch::getY1() const { return delta_y_1_; }

/**
 * @brief get Y2
 *
 * @return int
 */
int WirePatch::getY2() const { return delta_y_2_; }

/**
 * @brief get loc x
 *
 * @return int
 */
int WirePatch::getLocX() const { return loc_x_; }

/**
 * @brief get loc y
 *
 * @return int
 */

int WirePatch::getLocY() const { return loc_y_; }

/**
 * @brief set layer number
 *
 * @param layer_num
 */
void WirePatch::setLayerNum(Bits layer_num) { layer_ = layer_num; }

/**
 * @brief get layer number
 *
 * @return Bits
 */
Bits WirePatch::getLayerNum() const { return layer_; }

/**
 * @brief set X1
 *
 * @param x1
 */
void WirePatch::setX1(int x1) { delta_x_1_ = x1; }

/**
 * @brief set X2
 *
 * @param x2
 */
void WirePatch::setX2(int x2) { delta_x_2_ = x2; }

/**
 * @brief set Y1
 *
 * @param y1
 */
void WirePatch::setY1(int y1) { delta_y_1_ = y1; }

/**
 * @brief set Y2
 *
 * @param y2
 */
void WirePatch::setY2(int y2) { delta_y_2_ = y2; }

/**
 * @brief set x location
 *
 * @param x
 */
void WirePatch::setLocX(int x) { loc_x_ = x; }

/**
 * @brief set y location
 *
 * @param y
 */
void WirePatch::setLocY(int y) { loc_y_ = y; }

/**
 * @brief Print Out
 *
 */
void WirePatch::print() {
    message->info("RECT ( %d %d %d %d ) ", delta_x_1_, delta_y_1_, delta_x_2_,
                  delta_y_2_);
}

/**
 * @brief Print Out
 *
 */
void WirePatch::printDEF(FILE* fp) {
    Tech* lib = getTopCell()->getTechLib();
    Layer* layer = nullptr;
    layer = lib->getLayer(getLayerNum());

    fprintf(fp, "    NEW %s (%d %d) RECT ( %d %d %d %d ) \n", layer->getName(),
            loc_x_, loc_y_, delta_x_1_, delta_y_1_, delta_x_2_, delta_y_2_);
}

Wire::Wire() {
    x_ = 0;
    y_ = 0;
    layer_ = 0;
    height_ = 0;
    length_ = 0;
    net_ = 0;
}

/**
 * @brief Set the Net object
 *
 * @param net
 */
void Wire::setNet(ObjectId net) { net_ = net; }

/**
 * @brief Set the Net object
 *
 * @param net
 */
void Wire::setNet(Net* net) { net_ = net->getId(); }

void Wire::setOrient(Bits orient) { orient_ = orient; }
bool Wire::isHorizontal() { return orient_ == 0 ? true : false; }
bool Wire::isVertical() { return orient_ == 1 ? true : false; }
void Wire::setLayerNum(Bits layer_num) { layer_ = layer_num; }
Bits Wire::getLayerNum() const { return layer_; }

void Wire::setX(u_int32_t x) { x_ = x; }
int Wire::getX() { return x_; }
void Wire::setY(u_int32_t y) { y_ = y; }
int Wire::getY() { return y_; }
void Wire::setHeight(u_int32_t height) { height_ = height; }
u_int32_t Wire::getHeight() { return height_; }
void Wire::setLength(u_int32_t length) { length_ = length; }

u_int32_t Wire::getLength() { return length_; }

Point Wire::getHead() {
    Point head;
    if (isVertical()) {  // vertical
        head.setX(x_ + length_ / 2);
        head.setY(y_);
    } else {  // horizontal
        head.setX(x_);
        head.setY(y_ + height_ / 2);
    }
    return head;
}
Point Wire::getTail() {
    Point tail;
    if (isVertical()) {  // vertical
        tail.setX(x_ + length_ / 2);
        tail.setY(y_ + height_);
    } else {  // horizontal
        tail.setX(x_ + length_);
        tail.setY(y_ + height_ / 2);
    }
    return tail;
}

void Wire::printDEF(FILE* fp) {
    Tech* lib = getTopCell()->getTechLib();
    Layer* layer = nullptr;
    layer = lib->getLayer(getLayerNum());
    int status = 0;
    auto search = wire_status_map.find(getId());
    if (search != wire_status_map.end()) {
        status = search->second;
    }
    if (status) {
        switch (status) {
            case 1:
                fprintf(fp, "  + COVER \n    ");
                break;
            case 2:
                fprintf(fp, "  + FIXED \n    ");
                break;
            case 3:
                fprintf(fp, "  + ROUTED \n    ");
                break;
            case 4:
                fprintf(fp, "  + NOSHIELD \n    ");
                break;

            default:
                fprintf(fp, "\n");
                break;
        }
    } else {
        fprintf(fp, "    NEW ");
    }

    if (layer) fprintf(fp, "%s ", layer->getName());
    fprintf(fp, "( %d %d ) ", getHead().getX(), getHead().getY());

    int mask = 0;
    search = wire_mask_map.find(getId());
    if (search != wire_mask_map.end()) {
        mask = search->second;
    }

    if (mask) fprintf(fp, "MASK %d ", mask);
    auto is_virtual = wire_virtual_map.find(getId());
    if (is_virtual != wire_virtual_map.end()) {
       // fprintf(fp, "VIRTUAL %d ");
    }

    fprintf(fp, "( %d %d ) \n", getTail().getX(), getTail().getY());
}

}  // namespace db
}  // namespace open_edi
