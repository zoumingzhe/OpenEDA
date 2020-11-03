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

#include "db/core/object.h"
#include "db/util/box.h"
#include "util/util.h"
#include "db/core/net.h"

namespace open_edi {
namespace db {

class Wire;

class WirePatch : public Object {
  public:
    WirePatch();
    WirePatch(int x1, int y1, int x2, int y2);
    ~WirePatch();

    int getX1() const;
    int getX2() const;
    int getY1() const;
    int getY2() const;

    void setX1(int x1);
    void setX2(int x2);
    void setY1(int y1);
    void setY2(int y2);

    void print();
    void printDEF(FILE* fp);

  private:
    int delta_x_1_;
    int delta_x_2_;
    int delta_y_1_;
    int delta_y_2_;
};

class WireNode : public Object {
  public:
    WireNode();
    WireNode(Wire* wire);
    WireNode(int x, int y, int z);
    ~WireNode();

    int getX();
    int getY();
    int getZ();
    int getExtension() const;
    int getWidth() const;
    Bits getMask() const;
    Bits getStatus() const;
    Bits getIsVirtul() const;
    Bits getIsVia();
    std::string const& getViaName();

    void setX(int x);
    void setY(int y);
    void setZ(int z);
    void setMask(Bits mask);
    void setExtension(int extension);
    void setIsVirtul(Bits is_virtual);
    void setWidth(int width);
    int setViaName(std::string const& name);
    void setIsVia(Bits is_via);
    void setStatus(Bits status);

    bool isNewLayer();
    bool isStartNode();
    bool hasTaper();
    bool hasTaperRule();
    const char* getTaperRule();
    Bits getStyle();

    void setIsNewLayer(Bits is_new_layer);
    void setIsStartNode(Bits is_start_node);
    void setHasTaper(Bits has_taper);
    void setHasTaperRule(Bits has_taper_rule);
    void setStyle(Bits style);
    bool setTaperRule(std::string const& name);

    void addOutEdgeList(Wire* out);

    WirePatch* creatPatch(int x1, int y1, int x2, int y2);

    void print();
    void printDEF(FILE* fp);

  private:
    int extension_;
    Bits32 is_new_layer_ : 1;
    Bits32 is_start_node_ : 1;
    Bits32 has_tapper_ : 1;
    Bits32 has_tapper_rule_ : 1;
    Bits32 mask_ : 4;
    Bits32 status_ : 4;
    Bits32 is_virtual_ : 1;
    Bits32 is_via_ : 1;
    Bits32 z_ : 10;
    Bits32 style_ : 8;

    SymbolIndex via_index_;
    SymbolIndex taper_rule_index_;
    ObjectId patchs_;
    int x_, y_;
    std::vector<Wire*> out_edges_;
};

class Wire : public Object {
  public:
    Wire() { memset(static_cast<void*>(this), 0, sizeof(Wire)); }
    Wire(ObjectId tail, ObjectId head);
    ~Wire();

    Box getBBox() const;
    SymbolIndex getNet() const;
    WireNode* getHeadNode() const;
    WireNode* getTailNode() const;
    Bits getRoutingRule() const;
    Bits getPhysicalStatus() const;
    Bits getLayerNum() const;
    std::string getLayerName();
    std::string getRoutingRule();

    void setBBox(Box bbox);
    void setNet(ObjectId net);
    void setNet(Net* net);
    void setHead(WireNode* head);
    void setTail(WireNode* tail);
    void setPhysicalStatus(Bits status);
    void setRoutingRule(Bits rule);
    void setLayerNum(Bits layer_num);
    int setIgnoreNdrSpacing();
    int setFixed();
    int setRoutingRule();

    bool isFixed();
    bool isHorizontal();
    bool isVertical();
    bool is45degree();
    bool isShadow();
    bool isIgnoreNdrSpacing();

  private:
    // Box bbox_;

    ObjectId net_;  // memory id
    ObjectId head_;    // first point to terminal
    ObjectId tail_;    // ending with point to terminal

    Bits32 wire_type_ : 3;        // via,
    Bits32 physical_status_ : 4;  // is_fixed, is_shadow, is_route
    Bits32 routing_rule_ : 8;     // index from net.
    Bits32 layer_num_ : 8;
    Bits32 isTerminal_ : 1;
    Bits32 __ : 8;

    /* shilding wire                  TODO */
};

class WireGraph : public Object {
  public:
    WireGraph();
    ~WireGraph();

    WireNode* creatWireNode(int x, int y, int z);
    Wire* creatWireEdge(WireNode* tail, WireNode* head);
    Bits getStatus();
    void setStatus(int status);

    void addWireNode(WireNode* node);
    void addWireEdge(Wire* edge);

    void print();
    void printDEF(FILE* fp);

  private:
    Bits16 status_ : 4;
    Bits16 unuse_ : 12;

    ObjectId nodes_;
    ObjectId edges_;
};

}  // namespace db
}  // namespace open_edi
#endif  // EDI_DB_PHYSICAL_WIRE_H_
