/* @file  point.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_UTIL_POINT_H_
#define EDI_UTIL_POINT_H_
#include "util/namespace.h"

namespace open_edi {
namespace util {

class Point {
  public:
    Point(){};
    Point(int x, int y);

    int getX() const;
    void setX(int x);

    int getY() const;
    void setY(int y);

    void set(int x, int y);

    Point &operator=(const Point &point);
    bool operator==(const Point &point);

  private:
    int x_;
    int y_;
};

}  // namespace util
}  // namespace open_edi

#endif  // EDI_UTIL_POINT_H_
