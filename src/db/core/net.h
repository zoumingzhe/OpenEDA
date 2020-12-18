/* @file  net.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_DB_PHYSICAL_NET_H_
#define EDI_DB_PHYSICAL_NET_H_

#include "db/core/cell.h"
#include "db/core/object.h"
#include "db/core/pin.h"
#include "db/core/via.h"
#include "db/core/wire.h"
#include "db/tech/layer.h"
#include "util/enums.h"
#include "util/point.h"

namespace open_edi {
namespace db {

class Wire;
class WirePatch;
class NonDefaultRule;

enum NetType {
    kNetTypeAnalog = 1,
    kNetTypeClock = 2,
    kNetTypeGround = 3,
    kNetTypePower = 4,
    kNetTypeReset = 5,
    kNetTypeScan = 6,
    kNetTypeSignal = 7,
    kNetTypeTieOff = 8,
    kNetTypeTri = 9, /**< veilog tri */
    kNetTypeUnknown
};

enum AssignType {
    kAssignTypeNet = 1,
    kAssignTypeInt = 2,
    kAssignTypeReal = 3,
    kAssignTypeUnknown
};

class VPin : public Object {
  public:
    VPin();
    ~VPin();

    SymbolIndex getNameIndex() const;
    std::string const& getName();
    bool getHasLayer() const;
    const char* getLayer() const;
    Bits getStatus();
    Bits getOrientation();
    Box getBox() const;
    Point getLoc() const;

    bool setName(std::string const& name);
    void setHasLayer(bool has_layer);
    void setLayer(char* name);
    void setStatus(Bits status);
    void setOrientation(Bits orient);
    void setBox(Box box);
    void setLoc(Point loc);

  private:
    SymbolIndex name_index_;
    bool has_layer_;
    std::string layer_;
    Bits status_ : 4;
    Bits orient_ : 4;
    Box bbox_;
    Point loc_;
};  // class VPin

class Net : public Object {
  public:
    Net();
    ~Net();

    SymbolIndex getNameIndex() const;
    std::string const& getName();
    bool setName(std::string const& name);

    bool isAnalog();
    bool isClock();
    bool isGround();
    bool isPower();
    bool isReset();
    bool isScan();
    bool isSignal();
    bool isTieOff();

    Bits getFixBump() const;
    Bits getMustJoin() const;
    NetType getType() const;
    Bits getPattern() const;
    Bits getIsSubNet() const;
    bool getIsBusNet() const;
    bool getIsOfBus() const;
    bool getIsFromTerm() const;
    Bits getSource() const;
    AssignType getAssignType() const;
    ObjectId getAssignNet() const;
    int32_t getAssignInt() const;
    double getAssignReal() const;
    const char* getNonDefaultRule() const;
    int getXtalk() const;
    int getFrequency() const;
    double getWeight() const;
    double getCapacitance() const;
    Cell* getCell();
    const char* getOriginNet() const;
    uint64_t getNumProperties() const;
    ObjectId getPropertiesId() const;

    void setFixBump(Bits fix_bump);
    void setMustJoin(Bits must_join);
    void setType(NetType net_type);
    void setPattern(Bits pattern);
    void setNonDefaultRule(ObjectId rule);
    void setNonDefaultRule(NonDefaultRule* rule);
    void setCell(ObjectId cell);

    int addPin(Pin* pin);
    ObjectId addVPin(VPin* v_pin);
    void addWire(Wire* wire);
    void addVia(Via* via);
    void addSubNet(Net* sub_net);
    void addAssignNet(ObjectId assign_net);
    void addAssignConstant(int32_t assign_constant);
    void addAssignConstant(double assign_constant);
    ObjectId addProperty(ObjectId prop_id);

    void setXtalk(int xtalk);
    void setSource(Bits source);
    void setFrequency(int frequency);
    void setCapacitance(double cap);
    void setWeight(double weight);
    void setOriginNet(const char* name);
    void setIsSubNet(Bits is_sub_net);
    void setIsBusNet(bool is_bus_net);
    void setIsOfBus(bool is_of_bus);
    void setIsFromTerm(bool is_from_term);
    void setPropertySize(uint64_t v);

    ArrayObject<ObjectId>* getPinArray() const;

    Net* createSubNet(std::string& name);
    VPin* createVpin(std::string& name);
    Via* createVia(int x, int y, ViaMaster* via_master);
    Wire* createWire(int x1, int y1, int x2, int y2, int width);
    WirePatch* creatPatch(int loc_x, int loc_y, int x1, int y1, int x2, int y2,
                          int layer);
    ObjectId addPatch(WirePatch* patch, ObjectId patches);

    void deleteVia(Via* Via);
    void deleteWire(Wire* wire);

    void print();
    void printDEF(FILE* fp);

  private:
    SymbolIndex name_index_; /**< net name id */

    Bits is_bus_net_ : 1;   /**< the net is a bus */
    Bits is_of_bus_ : 1;    /**< the net is belong to a bus*/
    Bits is_from_term_ : 1; /**< the net is from module's term*/
    Bits is_sub_net_ : 1;
    Bits fix_bump_ : 1;
    Bits must_jion_ : 1;
    Bits xtalk_ : 8;
    Bits net_type_ : 4; /**< net type */
    Bits status_ : 4;   /**< net status */
    Bits source_ : 4;   /**< net status */
    Bits pattern_ : 3;
    Bits weight_ : 4;
    AssignType assign_type_ : 3; /**< the type of right assign statement*/

    ObjectId rule_; /**< rule in net */
    ObjectId pins_; /**< pins */
    ObjectId vias_;
    ObjectId wires_; /**< wire list in net */

    union {
        ObjectId assign_net_; /**< assign statement of verilog */
        int32_t assign_int_;
        double assign_real_;
    };

    int frequency_;
    float cap_;
};

}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_PHYSICAL_NET_H_
