/* @file  point.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "util/point.h"

namespace open_edi {
namespace util {

/**
 * @brief Construct a new Point:: Point object
 *
 * @param x
 * @param y
 */
Point::Point(int x, int y) {
    x_ = x;
    y_ = y;
}

/**
 * @brief get point x
 *
 * @return int
 */
int Point::getX() const { return x_; }

/**
 * @brief set point x
 *
 * @param x
 */
void Point::setX(int x) { x_ = x; }

/**
 * @brief get point y
 *
 * @return int
 */
int Point::getY() const { return y_; }

/**
 * @brief set point y
 *
 */
void Point::setY(int y) { y_ = y; }

/**
 * @brief set point x,y
 *
 * @param x,y
 */
void Point::set(int x, int y) {
    x_ = x;
    y_ = y;
}

/**
 * @brief Point operator '='
 *
 * @param point
 * @return Point&
 */
Point &Point::operator=(const Point &point) {
    x_ = point.getX();
    y_ = point.getY();
    return *this;
}

/**
 * @brief Point operator '=='
 *
 * @param point
 * @return true
 * @return false
 */
bool Point::operator==(const Point &point) { return (x_ == point.getX() && y_ == point.getY()); }

}  // namespace util
}  // namespace open_edi
