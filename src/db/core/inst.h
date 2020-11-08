/* @file  inst.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_DB_DESIGN_INST_H_
#define EDI_DB_DESIGN_INST_H_

#include "db/core/attr_object.h"
#include "db/core/inst_attr.h"
#include "db/tech/type_def.h"
#include "db/util/box.h"
#include "util/enums.h"
#include "util/point.h"

namespace open_edi {
namespace db {

class Pin;
class Cell;
class Constraint;
/*
 * COMPONENTS  numComps ;
 *     [–  compName modelName
 *         [+ EEQMASTER  macroName ]
 *         [+ SOURCE {NETLIST | DIST | USER | TIMING}]
 *         [+ {FIXED  pt orient  | COVER  pt orient  | PLACED  pt orient
 *             | UNPLACED} ]
 *         [+ MASKSHIFT  shiftLayerMasks ]
 *         [+ HALO [SOFT]  left   bottom   right   top ]
 *         [+ ROUTEHALO  haloDist   minLayer   maxLayer ]
 *         [+ WEIGHT  weight ]
 *         [+ REGION  regionName ]
 *         [+ PROPERTY { propName propVal } ...]...
 *     ;] ...
 * END COMPONENTS
 */

enum SourceType { kNetlist, kDist, kUser, kTiming };

class Inst : public Object {
  public:
    Inst();
    Inst(Inst const &rhs);
    Inst &operator=(Inst const &rhs);

    bool getHasEeqMaster() const;
    void setHasEeqMaster(bool flag);
    bool getHasSource() const;
    void setHasSource(bool flag);
    bool getHasMaskShift() const;
    void setHasMaskShift(bool flag);
    bool getHasHalo() const;
    void setHasHalo(bool flag);
    bool getHasSoft() const;
    void setHasSoft(bool flag);
    bool getHasRouteHalo() const;
    void setHasRouteHalo(bool flag);
    bool getHasWeight() const;
    void setHasWeight(bool flag);
    bool getHasRegion() const;
    void setHasRegion(bool flag);
    bool getHasProperty() const;
    void setHasProperty(bool flag);

    std::string getName() const;
    void setName(std::string name);
    Cell *getCell() const;
    void setCell(const std::string &name);
    void setCell(const Cell *cell);
    Cell *getParent() const;
    void setParent(const std::string name);
    void setParent(const Cell *cell);
    Cell *getMaster() const;
    void setMaster(ObjectId master);
    void setMaster(const std::string name);
    UInt32 numPGPins() const;
    void setPGPins(ObjectId pg_pins);
    ObjectId getPGPins() const;
    Pin *getPGPin(const std::string &name) const;
    void addPGPin(Pin *pin);
    UInt32 numPins() const;
    Pin *getPin(const std::string &name) const;
    ObjectId getPins() const;
    Pin *getPinById(ObjectId obj_id) const;
    void addPin(Pin *pin);
    void createPins();
    Pin *createInstancePin(std::string &pin_name);
    Pin *createInstancePinWithoutMaster(std::string &pin_name);
    void addCell(Cell *model);

    PlaceStatus getStatus() const;
    void setStatus(const PlaceStatus &s);
    Point getLocation() const;
    void setLocation(const Point &l);
    Orient getOrient() const;
    void setOrient(const Orient &o);

    SourceType getSource() const;
    std::string getSourceString() const;
    void setSource(const SourceType &s);
    Cell *getEeqMaster() const;
    void setEeqMaster(const std::string &name);
    UInt32 getMaskShift(Int32 layer_id) const;
    void setMaskShift(UInt32 m, Int32 layer_id);
    Box getHalo() const;
    void setHalo(const Box &halo);
    UInt32 getRouteHaloDist() const;
    void setRouteHaloDist(const UInt32 &d);
    Int32 getMinLayerId() const;
    void setMinLayer(std::string name);
    Int32 getMaxLayerId() const;
    void setMaxLayer(std::string name);
    Int32 getWeight() const;
    void setWeight(const Int32 &w);
    Constraint *getRegion() const;
    void setRegion(std::string &name);

    void setPropertySize(uint64_t v);
    uint64_t getNumProperties() const;
    void addProperty(ObjectId prop_id);
    ObjectId getPropertiesId() const;

    void clear();

    void print(FILE *fp);

  protected:
    void copy(Inst const &rhs);
    void move(Inst &&rhs);
    friend OStreamBase &operator<<(OStreamBase &os, Inst const &rhs);
    friend IStreamBase &operator>>(IStreamBase &is, Inst &rhs);

  private:
    ObjectId __createPinArray();
    ObjectId __createPropertyArray();

    Bits has_eeq_master_ : 1;
    Bits has_source_ : 1;
    Bits has_mask_shift_ : 1;
    Bits has_halo_ : 1;
    Bits has_soft_ : 1;
    Bits has_route_halo_ : 1;
    Bits has_weight_ : 1;
    Bits has_region_ : 1;
    Bits has_property_ : 1;
    Bits unused_ : 22;

    SymbolIndex name_index_;            // instance name
    ObjectId master_;                   // master cell
    ObjectId parent_;                   // parent hierarchical cell
    ObjectId pg_pins_;                  // power/ground pins
    ObjectId pins_;                     // signal pins
    PlaceStatus status_;                // instance status
    Point location_;                    // instance location
    Orient orient_;                     // instance orientation
    ObjectId eeq_master_;               // EEQMASTER macroName
    SourceType source_;                 // SOURCE
    UInt32 mask_shift_[max_layer_num];  // MASKSHIFT
    Box halo_;                          // HALO
    UInt32 route_halo_dist_;            // haloDist
    Int32 min_layer_id_;                // minLayer
    Int32 max_layer_id_;                // maxLayer
    Int32 weight_;                      // WEIGHT
    ObjectId region_;                   // REGION
    ObjectId properties_id_;
};  // class Inst

}  // namespace db
}  // namespace open_edi

#endif
