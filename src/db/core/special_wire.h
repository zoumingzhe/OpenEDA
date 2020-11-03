/* @file  special_wire.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_DB_PHYSICAL_SPECIAL_WIRE_H_
#define EDI_DB_PHYSICAL_SPECIAL_WIRE_H_
#include "db/core/object.h"
#include "db/core/special_wire.h"
#include "db/core/via.h"
#include "db/util/box.h"
#include "util/point.h"
#include "util/util.h"

namespace open_edi {
namespace db {

class SpecialWireNode;
class Net;

class SpecialWire : public Object {
  public:
    SpecialWire() { ; }
    ~SpecialWire();

    Bits getStatus() const;
    Bits getMask() const;
    Bits isVia() const;
    const char* getShapeName() const;
    Box getBox() const;
    Via* getVia() const;

    void setStatus(Bits status);
    void setMask(Bits mask);
    void setIsVia(Bits is_via);
    void setShapeName(char* shape);
    void setBox(Box box);
    void setVia(Via* via);

    void addViaPoint(Point point);

    void print();
    void printDEF(FILE* fp);

  private:
    Bits status_ : 4;
    Bits mask_ : 8;
    Bits is_via_ : 1;
    std::string shape_type_;
    Box bbox_;
    Via* via_;
    std::vector<Point> via_piont_list_;
};  // class SpecialWire

class SpecialWireEdge : public Object {
  public:
    SpecialWireEdge();
    SpecialWireEdge(SpecialWireNode* head, SpecialWireNode* tail);
    ~SpecialWireEdge();
    void setHead(SpecialWireNode* head);
    void setTail(SpecialWireNode* tail);

  private:
    ObjectId head_;
    ObjectId tail_;

    friend class SpecialWireNode;
};
class SpecialWireNode : public Object {
  public:
    SpecialWireNode() { memset((void*)this, 0, sizeof(SpecialWireNode)); };
    SpecialWireNode(SpecialWireEdge* special_ege);
    SpecialWireNode(int x, int y, int z);
    ~SpecialWireNode();

    void setX(int x);
    void setY(int y);
    void setZ(int z);

    Bits getMask() const;
    int getExtension() const;
    int getX();
    int getY();
    int getZ();
    Via* getVia() const;
    Bits getIsVia();

    void setMask(Bits mask);
    void setExtension(int extension);
    void setIsVia(Bits is_via);
    void setVia(Via* via);

    void addOutEdgeList(SpecialWireEdge* out);

    void print();
    void printDEF(FILE* fp);

  private:
    int extension_;
    Bits mask_ : 4;
    Bits is_via_ : 1;
    Bits z_ : 10;

    Via* via_;
    int x_, y_;
    std::vector<SpecialWireEdge*> out_edge_list_;
};

class SpecialWireGraph : public Object {
  public:
    SpecialWireGraph();
    ~SpecialWireGraph();

    bool isNewLayer();
    Bits getShape() const;
    int getWidth() const;
    Bits getStyle() const;
    Bits getStatus() const;

    void setIsNewLayer(Bits is_new_layer);
    void setShape(Bits shape);
    void setWidth(int width);
    void setStyle(Bits style);
    void setStatus(Bits status);

    int addWireNode(SpecialWireNode* node);
    int addWireEdge(SpecialWireEdge* edge);

    SpecialWireNode* createSpecialWireNode();
    SpecialWireNode* createSpecialWireNode(int x, int y, int z);
    SpecialWireEdge* createSpecialWireEdge(SpecialWireNode* tail, SpecialWireNode* head);

    void print();
    void printDEF(FILE* file);

  private:
    Bits is_new_layer_ : 1;
    Bits shape_ : 8;
    Bits style_ : 8;
    Bits status_ : 4;
    int width_;

    ObjectId nodes_;
    ObjectId edges_;
};
class SpecialWireSection : public Object {
  public:
    SpecialWireSection();
    ~SpecialWireSection();

    Bits getStatus();
    void setStatus(int status);

    void setShieldNetName(const char* net_name);
    Net* getShieldNet();
    std::string getShieldNetName();

    SpecialWireGraph* creatGraph();
    int addGraph(SpecialWireGraph* graph);
    void print();
    void printDEF(FILE* fp);

  private:
    Bits8 status_ : 4;
    Bits8 reserved_ : 4;
    std::string shield_net_name_;
    ObjectId shield_net_id_;
    ObjectId graphs_;
};

}  // namespace db
}  // namespace open_edi
#endif  // EDI_DB_PHYSICAL_SPECIAL_WIRE_H_
