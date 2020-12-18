/* @file  via_rule.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_DB_TECH_VIA_RULE_H_
#define EDI_DB_TECH_VIA_RULE_H_

#include "db/tech/property.h"
#include "db/tech/type_def.h"
#include "db/tech/via_master.h"
#include "db/util/box.h"

namespace open_edi {
namespace db {

class ViaRuleMetalLayer;
class ViaRuleCutLayer;

/**
 * @class ViaRule
 * @brief
 * implementation of VIARULE and VIARULE GENERATE
  VIARULE  viaRuleName
    LAYER  layerName ;
        DIRECTION {HORIZONTAL | VERTICAL} ;
        [WIDTH  minWidth  TO  maxWidth ;]
    LAYER  layerName ;
        DIRECTION {HORIZONTAL | VERTICAL} ;
        [WIDTH  minWidth  TO  maxWidth ;]
    {VIA  viaName ;} ...
    [PROPERTY  propName propVal  ;] ...
  END  viaRuleName

  VIARULE  viaRuleName   GENERATE [DEFAULT]
    LAYER  routingLayerName ;
        ENCLOSURE  overhang1   overhang2  ;
        [WIDTH  minWidth  TO  maxWidth ;]
    LAYER  routingLayerName ;
        ENCLOSURE  overhang1   overhang2  ;
        [WIDTH  minWidth  TO  maxWidth ;]
    LAYER  cutLayerName ;
        RECT  pt   pt ;
        SPACING  xSpacing  BY  ySpacing ;
        [RESISTANCE  resistancePerCut ;]
  END  viaRuleName
*/

class ViaRule : public Object {
  public:
    ViaRule();
    ~ViaRule();

    SymbolIndex getNameIndex() const;
    std::string const &getName();
    bool setName(std::string const &name);

    bool getIsGenerate() const;
    void setIsGenerate(bool flag);
    bool getIsDefault() const;
    void setIsDefault(bool flag);
    ObjectId getPropertiesId() const;
    uint64_t numProperties() const;

    ViaRuleCutLayer *getViaRuleCutLayer();
    ViaRuleMetalLayer *creatViaRuleMetalLayer();
    ViaRuleCutLayer *creatViaRuleCutLayer();

    void addViaMaster(ViaMaster *via_master);
    void addViaRuleMetalLayer(ViaRuleMetalLayer *via_rule);
    void setViaRuleCutLayer(ViaRuleCutLayer *via_rule);
    void setPropertySize(uint64_t v);
    ObjectId addProperty(ObjectId obj_id);
    bool getHasProperty() const;
    void setHasProperty(bool flag);

    void print();
    void printLEF(std::ofstream &ofs);

  private:
    Bits is_generate_ : 1;  // for VIARULE GENERATE
    Bits is_default_ : 1;
    Bits has_property_ : 1;
    Bits unused_ : 29;
    SymbolIndex name_index_;

    ObjectId metal_layers_;
    ObjectId cut_layer_;
    ObjectId via_masters_;
    // std::vector<ViaMaster *> via_masters_;
    ObjectId properties_;
};

class ViaRuleMetalLayer : public Object {
  public:
    ViaRuleMetalLayer();
    ~ViaRuleMetalLayer() {}

    bool getHasWidth() const;
    void setHasWidth(bool flag);
    bool getHasDirection() const;
    void setHasDirection(bool flag);
    bool getHasEnclosure() const;
    void setHasEnclosure(bool flag);
    bool getIsHorizontal() const;
    void setIsHorizontal(bool flag);

    Int32 getMinWidth() const;
    void setMinWidth(Int32 min_width);
    Int32 getMaxWidth() const;
    void setMaxWidth(Int32 max_width);
    Int32 getOverhang1() const;
    void setOverhang1(Int32 overhang);
    Int32 getOverhang2() const;
    void setOverhang2(Int32 overhang);
    Int32 getLayerId() const;
    void setLayerId(Int32 id);

    void print(int is_generate);
    void printLEF(std::ofstream &ofs, int is_generate);

  private:
    Bits has_width_ : 1;
    Bits has_direction_ : 1;
    Bits has_enclosure_ : 1;
    Bits is_horz_ : 1;  // or else is_vert
    Bits unused_ : 28;

    Int32 min_width_;
    Int32 max_width_;
    Int32 overhang1_;
    Int32 overhang2_;
    Int32 layer_id_;
};

class ViaRuleCutLayer : public Object {
  public:
    ViaRuleCutLayer();
    ~ViaRuleCutLayer() {}

    bool getHasResistance() const;
    void setHasRestance(bool flag);

    Box getBox() const;
    void setBox(const Box box);
    Int32 getXSpacing() const;
    void setXSpacing(Int32 spacing);
    Int32 getYSpacing() const;
    void setYSpacing(Int32 spacing);
    Int32 getLayerId() const;
    void setLayerId(Int32 id);
    float getResistancePerCut() const;
    void setResistancePerCut(float res);

    void print();
    void printLEF(std::ofstream &ofs);

  private:
    Bits has_resistance_ : 1;
    Bits unused_ : 31;

    Box rect_;
    Int32 x_spacing_;
    Int32 y_spacing_;
    Int32 layer_id_;
    float resistance_per_cut_;
    ObjectId properties_;
};

}  // namespace db
}  // namespace open_edi

#endif
