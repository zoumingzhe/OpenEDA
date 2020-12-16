/* @file  transform.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_UTIL_TRANSFROM_H_
#define EDI_UTIL_TRANSFROM_H_

#include "util/util.h"
#include "db/util/box.h"
namespace open_edi {
namespace db {
class Inst;
class Pin;

bool transformByInst(const Inst *inst, Box &box);
bool transformByInst(const Inst *inst, Point &pt);

bool transformByIOPin(const Pin *pin, Box &box);
bool transformByIOPin(const Pin *pin, Point &pt);

class Transform {
  public:
    Transform() : offset_(0, 0), orient_() {}
    ~Transform() {}
    Transform(const Inst *inst);
    Transform(const Pin *pin);

    void transformCoord(Point &pt);
    void transformBox(Box &box);

  private:
    void __setOffset(Point &placed_pt, Point &origin, int size_x, int size_y);
    void __setOffset(Point &placed_pt, Point &origin);

    Point offset_;
    Orient orient_;
};

}  // namespace db
}  // namespace open_edi

#endif  // EDI_UTIL_TRANSFROM_H_
