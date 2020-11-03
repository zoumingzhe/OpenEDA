/* @file  implant_rule.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_DB_TECH_IMPLANT_RULE_H_
#define EDI_DB_TECH_IMPLANT_RULE_H_

#include <string>
#include <vector>

#include "db/tech/layer.h"

namespace open_edi {
namespace db {

class Layer;
class CutClass;
class MinEnclArea;
class SecondLayer;

/**
 * @class ImplantCoreEdgeLength
 * @brief
 * the implementation class of COREEDGELENGTH syntax
 *
 * [ PROPERTY LEF58_COREEDGELENGTH
 *      "COREEDGELENGTH  minLength
 *          [EXCEPTADJACENTLENGTH
 *              { exactEdgeLength   adjLength  [EXACTADJACENTLENGTH]}…]
 * ]; " ;
 */
class ImplantCoreEdgeLength {
  public:
    ImplantCoreEdgeLength() {
        memset(static_cast<void*>(this), 0, sizeof(ImplantCoreEdgeLength));
    }
    ~ImplantCoreEdgeLength();

  public:
    UInt32 getMinLength() const;
    UInt32 getNumExceptAdjLength() const;
    UInt32 getExactEdgeLength(UInt32 idx) const;
    UInt32 getAdjLength(UInt32 idx) const;
    bool isExactAdjLength(UInt32 idx) const;

    void setMinLength(UInt32 len);
    void setNumExceptAdjLength(UInt32 num);
    void setExactEdgeLength(UInt32 idx, UInt32 len);
    void setAdjLength(UInt32 idx, UInt32 len);
    void setIsExactAdjLength(UInt32 idx, bool v);

  private:
    UInt32 min_length_;
    UInt32 except_adj_length_num_;
    UInt32* exact_edge_length_;
    UInt32* adj_length_;
    bool* is_exact_adj_length_;
};

/**
 * @class ImplantSpacing
 * @brief
 * the implementation class of SPACING syntax
 *
 * [SPACING  minSpacing  [LAYER  layerName2 ] ;] ...
 *
 * [ PROPERTY LEF58_SPACING
 *    "SPACING  minSpacing  [LAYER  layerName2 ]
 *        [HORIZONTAL|VERTICAL PRL  prl  ][EXCEPTABUTTED]
 *        [EXCEPTCORNERTOUCH] [LENGTH  length ]
 *        [INTERSECTLAYERS  layerNameList ...]
 * ]; " ;
 */
class ImplantSpacing {
  public:
    ImplantSpacing() {
        memset(static_cast<void*>(this), 0, sizeof(ImplantSpacing));
    }
    ~ImplantSpacing();

  public:
    UInt32 getMinSpacing() const;
    UInt32 getPRL() const;
    UInt32 getLength() const;
    Int32 getLayer2Id() const;
    void setLayer2Id(Int32 layer_id);
    UInt32 getNumLayers() const;
    bool hasPRL() const;
    bool isHorizonal() const;
    bool isVertical() const;
    bool isExceptAbutted() const;
    bool isExceptCornerTouch() const;
    bool hasIntersectLayer() const;
    Layer** getLayers() const;
    ImplantSpacing* getNext() const;

    void setMinSpacing(UInt32 sp);
    void setPRL(UInt32 prl);
    void setLength(UInt32 len);
    void setDir(bool v);
    void setIsExceptAbutted(bool v);
    void setIsExceptCornerTouch(bool v);
    void setNumLayers(UInt32 num);
    void setLayers(Layer** layers);
    void setLayer2(SecondLayer* l);
    void setNext(ImplantSpacing* n);

  private:
    ImplantSpacing* next_;
    UInt32 min_spacing_;
    UInt32 prl_;
    SecondLayer* sec_layer_;
    Layer** layers_;
    UInt32 layers_num_;
    UInt32 length_;                    // LENGTH
    Bits dir_ : 1;                     // HORIZONTAL | VERTICAL
    Bits is_except_abutted_ : 1;       // EXCEPTABUTTED
    Bits is_except_corner_touch_ : 1;  // EXCEPTCORNERTOUCH
};

// TODO: implant group is defined in PROPERTYDEFINITIONS
// all ImpantGroup should be saved to a hashtable for the purpose of search by
// group name
class ImplantGroup {};

/**
 * @brief
 * the implementation class of WIDTH syntax
 *
 * [ PROPERTY LEF58_WIDTH
 *      "WIDTH  minWidth  [LAYER  layerName2 ][ZEROPRL] [EXCEPTCORNERTOUCH]
 *          [LENGTH  length ] [CHECKIMPLANTGROUP  groupName ]
 * ]; " ;
 */
class ImplantWidth {
  public:
    ImplantWidth() {
        memset(static_cast<void*>(this), 0, sizeof(ImplantWidth));
    }
    ~ImplantWidth();

  public:
    UInt32 getWidth() const;
    UInt32 getLength() const;
    Int32 getLayer2Id() const;
    void setLayer2Id(Int32 layer_id);
    bool isZeroPRL() const;
    bool isExceptCornerTouch() const;
    ImplantGroup* getGroup() const;
    ImplantWidth* getNext() const;

    void setWidth(UInt32 w);
    void setLength(UInt32 l);
    void setIsZeroPRL(bool v);
    void setIsExceptCornerTouch(bool v);
    void setGroup(ImplantGroup* g);
    void setNext(ImplantWidth* iw);

  private:
    UInt32 width_;
    UInt32 length_;
    SecondLayer* sec_layer_;
    ImplantGroup* group_;
    ImplantWidth* next_;

    Bits is_zero_prl_ : 1;
    Bits is_except_corner_touch_ : 1;
};

class ImplantLayerRule {
  public:
    ImplantLayerRule() {
        memset(static_cast<void*>(this), 0, sizeof(ImplantLayerRule));
    }

  public:
    ImplantCoreEdgeLength* getCoreEdgeLength() const;
    ImplantSpacing* getSpacingList() const;
    MinEnclArea* getMinEnclArea() const;
    ImplantWidth* getWidth() const;

    void setCoreEdgeLength(ImplantCoreEdgeLength* iel);
    void setSpacingList(ImplantSpacing* is);
    void setMinEnclArea(MinEnclArea* mea);
    void setWidth(ImplantWidth* iw);

  private:
    ImplantCoreEdgeLength* core_edge_length_;
    ImplantSpacing* spacing_list_;
    MinEnclArea* min_encl_area_;
    ImplantWidth* width_;
};

}  // namespace db
}  // namespace open_edi
#endif  // EDI_DB_TECH_IMPLANT_RULE_H_
