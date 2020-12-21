/* @file  via.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_DB_PHYSICAL_VIA_H_
#define EDI_DB_PHYSICAL_VIA_H_

#include "db/core/object.h"
#include "db/tech/via_master.h"
#include "db/util/box.h"
#include "util/enums.h"
#include "util/point.h"

namespace open_edi {
namespace db {

enum ViaType {
    kViaSignal = 1,
    kViaSpecial = 2,
    kViaPillar = 3,
    kViaFill = 4,
    kTypeUnknown
};

enum ViaOrient {
    kR0 = 0,
    kR90 = 1,
    kR180 = 2,
    kR270 = 3,
    kMX = 4,
    kMXR90 = 5,
    kMY = 6,
    kMYR90 = 7,
    kOrientationUnknown
};

enum ViaStatus {
    kRounted = 0,
    kFixed = 1,
    kUnconstrained = 2,
    kAppFixed = 3,
    kStatusUnknown
};

class Via : public Object {
  public:
    Via();
    Via(ViaMaster* via_master);
    ~Via();

    Point getLoc() const;
    void setLoc(Point loc);
    void setLoc(int x, int y);

    ViaMaster* getMaster() const;
    void setMaster(ViaMaster* master);

    Bits getOrient() const;
    void setOrient(Bits orient);

    bool isR0() const;
    bool isR90() const;
    bool isR180() const;
    bool isR270() const;
    bool isMX() const;
    bool isMXR90() const;
    bool isMY() const;
    bool isMYR90() const;

    Bits getRouteStatus() const;
    void setRouteStatus(Bits status);

    void printDEF(FILE* fp);

  private:
    Point loc_;
    ObjectId master_;
    Bits orientation_ : 4;
    Bits status_ : 4;
    Bits is_array_ : 1;
};
}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_PHYSICAL_VIA_H_
