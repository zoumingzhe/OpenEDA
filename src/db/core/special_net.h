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
#ifndef EDI_DB_PHYSICAL_SPECIAL_NET_H_
#define EDI_DB_PHYSICAL_SPECIAL_NET_H_

#include "db/core/cell.h"
#include "db/core/object.h"
#include "db/core/pin.h"
#include "db/core/special_wire.h"
#include "db/core/via.h"
#include "db/tech/layer.h"
#include "util/enums.h"

namespace open_edi {
namespace db {

class SpecialWire;

class SpecialNet : public Object {
  public:
    SpecialNet();
    ~SpecialNet();

    SymbolIndex getNameIndex() const;
    std::string const& getName();
    bool setName(std::string const& name);

    Bits getFixBump() const;
    Bits getType() const;
    int getVoltage() const;
    Bits getPattern() const;
    const char* getRule() const;
    Bits getSource() const;
    int getFrequency() const;
    Cell* getCell();
    double getCapacitance() const;
    double getWeight() const;
    const char* getOriginNet();
    const char* getNonDefaultRule() const;
    uint64_t getNumProperties() const;
    ObjectId getPropertiesId() const;

    void setFixBump(Bits fix_bump);
    void setPattern(Bits pattern);
    void setNonDefaultRule(ObjectId rule);
    void setType(Bits net_type);
    void setSource(Bits source);
    void setVoltage(int voltage);
    void setFrequency(int frequency);
    void setCell(ObjectId cell);
    void setCapacitance(double cap);
    void setWeight(double weight);
    void setOriginNet(SpecialNet* net);
    void setPropertySize(uint64_t v);

    SpecialWireSection* createWireSection();
    int addWireSection(SpecialWireSection* section);
    void addProperty(ObjectId prop_id);
    bool getHasProperty() const;

    int addPin(Pin* pin);
    int addVia(Via* via);
    void addWire(SpecialWire* wire);

    void deleteWire(SpecialWire* wire);
    void deleteVia(Via* Via);

    void print();
    void printDEF(FILE* file);

 private:
    Bits fix_bump_     :1;
    Bits net_type_     :4; /**< net type */
    Bits status_       :4;   /**< net status */
    Bits source_       :4;   /**< net status */
    Bits pattern_      :3;
    bool has_property_ :1;
    int voltage_       :32;
    Bits64 reserved_   :15;

    ObjectId rule_;                        /**< rule in net */
    ObjectId pins_;                        /**< pins */
    ObjectId vias_;                        /**< Via list in net */
    std::vector<SpecialWire*> wire_rects_; /**< wire list in net */
    SymbolIndex name_index_; /**< via name id */
    int frequency_;
    double cap_;
    double weight_;
    ObjectId wire_sections_;
    ObjectId origin_net_;
};

}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_PHYSICAL_NET_H_
