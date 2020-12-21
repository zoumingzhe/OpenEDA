/* @file  wire.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_DB_PHYSICAL_WIRE_H_
#define EDI_DB_PHYSICAL_WIRE_H_

#include <string>
#include <vector>

#include "db/core/net.h"
#include "db/core/object.h"
#include "db/util/box.h"
#include "util/util.h"

namespace open_edi {
namespace db {

class WirePatch : public Object {
  public:
    WirePatch();
    WirePatch(int x1, int y1, int x2, int y2);
    ~WirePatch();

    int getX1() const;
    int getX2() const;
    int getY1() const;
    int getY2() const;
    int getLocX() const;
    int getLocY() const;

    void setX1(int x1);
    void setX2(int x2);
    void setY1(int y1);
    void setY2(int y2);
    void setLocX(int x);
    void setLocY(int y);
    
    void setLayerNum(Bits layer_num);
    Bits getLayerNum() const;

    void print();
    void printDEF(FILE* fp);

  private:
    int layer_;
    int loc_x_, loc_y_;
    int delta_x_1_;
    int delta_x_2_;
    int delta_y_1_;
    int delta_y_2_;
};

class Wire : public Object {
  public:
    Wire();
    ~Wire();

    Box getBBox() const;
    Bits getRoutingRule() const;
    Bits getPhysicalStatus() const;
    Bits getLayerNum() const;
    std::string getLayerName();

    void setNet(ObjectId net);
    void setNet(Net* net);
    void setPhysicalStatus(Bits status);
    void setLayerNum(Bits layer_num);

    bool isHorizontal();
    bool isVertical();
    void setOrient(Bits orient);
    bool isShadow();
    bool isIgnoreNdrSpacing();

    Point getHead();
    Point getTail();

    void setX(u_int32_t x);
    int getX();
    void setY(u_int32_t y);
    int getY();
    void setHeight(u_int32_t height);
    u_int32_t getHeight();
    void setLength(u_int32_t length);
    u_int32_t getLength();

    void printDEF(FILE* fp);

  private:
    // Box bbox_;

    int x_;
    int y_;
    Bits layer_ : 8;
    // Bits physical_status_ : 4;  // is_fixed, is_shadow, is_route
    Bits height_ : 27;
    Bits length_ : 27;
    Bits orient_ : 1;
    ObjectId net_;
};

}  // namespace db
}  // namespace open_edi
#endif  // EDI_DB_PHYSICAL_WIRE_H_
