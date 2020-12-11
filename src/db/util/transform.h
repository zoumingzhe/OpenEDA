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
bool transformByInst(Inst *inst, Box &box);
bool transformByInst(Inst *inst, Point &pt);

class Transform {
  public:
    Transform() : offset_(0, 0), orient_() {}
    ~Transform() {}
    Transform(Inst *inst);
    void transformCoord(Point &pt);
  private:
    Point offset_;
    Orient orient_;
};

}  // namespace db
}  // namespace open_edi

#endif  // EDI_UTIL_TRANSFROM_H_
