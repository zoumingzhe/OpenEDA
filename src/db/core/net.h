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
class WireGraph;

enum NetType {
    kNetTypeSignal = 1,
    kNetTypeClock = 2,
    kNetTypeShielding = 3,
    kNetTypeRdl = 4,
    kNetTypeUserRoute = 5,
    kNetTypeAnalog = 6,
    kNetTypeTieHigh = 7,
    kNetTypeTieLow = 8,
    kNetTypePower = 9,
    kNetTypeGround = 10,
    kNetTypeScan = 11,
    kNetTypeTri = 12,     /**< veilog tri */
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

    Bits getFixBump() const;
    Bits getMustJoin() const;
    Bits getType() const;
    Bits getPattern() const;
    Bits getIsSubNet() const;
    bool getIsBusNet() const;
    bool getIsOfBus() const;
    bool getIsFromTerm() const;
    Bits getSource() const;
    AssignType getAssignType() const;
    ObjectId getAssignNet() const;
    int32_t getAssignInt() const;
    double  getAssignReal() const;
    const char* getNonDefaultRule() const;
    int getXtalk() const;
    int getFrequency() const;
    double getWeight() const;
    double getCapacitance() const;
    Cell* getCell();
    const char* getOriginNet() const;
    uint64_t getNumProperties() const;
    ObjectId getPropertiesId() const;
    bool getHasProperty() const;

    void setFixBump(Bits fix_bump);
    void setMustJoin(Bits must_join);
    void setType(Bits net_type);
    void setPattern(Bits pattern);
    void setNonDefaultRule(ObjectId rule);
    void setCell(ObjectId cell);

    int addPin(Pin* pin);
    int addVPin(VPin* v_pin);
    void addWire(Wire* wire);
    void addVia(Via* via);
    void addSubNet(Net* sub_net);
    void addAssignNet(ObjectId assign_net);
    void addAssignConstant(int32_t assign_constant);
    void addAssignConstant(double assign_constant);
    void addProperty(ObjectId prop_id);

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

    Net* createSubNet(std::string& name);
    VPin* createVpin(std::string& name);
    Via* createVia(ViaMaster* via_master);

    WireGraph* creatGraph();
    void addGraph(WireGraph* graph);

    void deleteVia(Via* Via);
    void deleteWire(Wire* wire);

    void print();
    void printDEF(FILE* fp);

  private:
    bool is_bus_net_        :1; /**< the net is a bus */
    bool is_of_bus_         :1; /**< the net is belong to a bus*/
    bool is_from_term_      :1; /**< the net is from module's term*/
    bool is_sub_net_        :1;
    bool fix_bump_          :1;
    bool must_jion_         :1;
    bool has_property_      :1;
    AssignType assign_type_ :3; /**< the type of right assign statement*/
    Bits net_type_          :4; /**< net type */
    Bits status_            :4; /**< net status */
    Bits source_            :4; /**< net status */
    Bits pattern_           :3;
    Bits reserved_          :39;

    SymbolIndex name_index_; /**< net name id */
    ObjectId cell_;
    ObjectId rule_; /**< rule in net */
    ObjectId pins_; /**< pins */
    ObjectId vias_;
    ObjectId v_pins_; /**< pins */
    ObjectId wires_;  /**< wire list in net */
    ObjectId graphs_;
    ObjectId sub_nets_;
    union {
        ObjectId assign_net_; /**< assign statement of verilog */
        int32_t  assign_int_;
        double   assign_real_;
    };
    ObjectId properties_id_;
    int xtalk_;
    int frequency_;
    double cap_;
    Bits weight_;
    std::string origin_net_;
};

}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_PHYSICAL_NET_H_
