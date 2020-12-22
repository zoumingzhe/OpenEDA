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
#ifndef EDI_UTIL_BOX_H_
#define EDI_UTIL_BOX_H_
#include "db/core/object.h"
#include "util/util.h"

namespace open_edi {
namespace db {

class Box {
  public:
    Box();
    Box(int llx, int lly, int urx, int ury);
    ~Box(){};
    bool isInvalid() const { return (llx_ >= lly_ || urx_ >= ury_); }
    int getLLX() const { return llx_; }
    int getLLY() const { return lly_; }
    int getURX() const { return urx_; }
    int getURY() const { return ury_; }
    void setLLX(int llx) { llx_ = llx; }
    void setLLY(int lly) { lly_ = lly; }
    void setURX(int urx) { urx_ = urx; }
    void setURY(int ury) { ury_ = ury; }
    void setBox(int llx, int lly, int urx, int ury);
    void setBox(const Box &box);
    void setBox(const Box *box);
    Box &operator=(const Box &box);
    bool operator==(const Box &box);

    bool isIntersect(const Box &box) const;
    int getWidth();
    int getHeight();
    void maxBox(const Box &box);

  private:
    int llx_;
    int lly_;
    int urx_;
    int ury_;
};

}  // namespace db
}  // namespace open_edi

#endif  // EDI_UTIL_BOX_H_
