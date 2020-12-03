/* @file  layer.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_DB_TECH_LAYER_H_
#define EDI_DB_TECH_LAYER_H_

#include <string.h>
#include <string>

#include "db/core/object.h"
#include "db/tech/type_def.h"
#include "db/tech/routing_layer_rule.h"
#include "db/tech/cut_layer_rule.h"
#include "db/tech/implant_rule.h"
#include "db/util/array.h"

namespace open_edi {
namespace db {
using IdArray = ArrayObject<ObjectId>;

class Tech;

/** @brief layer type */
enum LayerType {
    kLayerUnknown        =  0,
    kLayerImplant        =  1,  /**< implant layer */
    kLayerMS             =  2,  /**< masterslice layer*/
    kLayerOverlap        =  3,  /**< define MACRO rectilinear shape */
    kLayerCut            =  4,  /**< cut layer */
    kLayerRouting        =  5,  /**< routing layer */

    // cut layer with LEF58_TYPE
    kLayerTSV            =  6,  /**< LEF58: throught-silicon via(TSV) cut layer */
    kLayerPassivation    =  7,  /**< LEF58: passivation cut layer */
    kLayerMIMCapCut      =  8,  /**< LEF58: cut layer that is not suitable for routing */
    kLayerHighrCut       =  9,  /**< LEF58: special cut layer that is used to connect to a high resistance cut layer with type HIGHR */

    // cut layer with LEF58_REGION
    kLayerRegionCut      = 10,  /**< LEF58: cut layer with region-based or area-based rules (REGION regionLayer BASEDLAYRE cutLayer) */

    // masterslice with LEF58_TYPE
    kLayerRegion         = 11,  /**< LEF58: masterslice layer with type REGION */
    kLayerNWell          = 12,  /**< LEF58: nwell layer (masterslice) */
    kLayerPWell          = 13,  /**< LEF58: pwell layer (masterslice) */
    kLayerAboveDieEdge   = 14,  /**< LEF58: special masterslice layer for a hard macro to define an OBS on the layer such that the
                                  die boundary of above or below die do not overlap */
    kLayerBelowDieEdge   = 15,  /**< LEF58: special masterslice layer for a hard macro to define an OBS on the layer such that the
                                  die boundary of above or below die do not overlap */
    kLayerDiffusion      = 16,  /**< LEF58: diffusion layer (masterslice) */
    kLayerTrimPoly       = 17,  /**< LEF58: trim poly layer for self-aligned double patterning(SADP) technology. only cell  */
    kLayerTrimMetal      = 18,  /**< LEF58: trim metal layer(MS) is only used along with metal layers manufactured with SADP technology */
    kLayerMEOL           = 19,  /**< LEF58: a middle-end-of-line(MEOL) MS layer and it is the front-end metal layer on a 3D IC design */

    // masterslice with LEF58_REGION
    kLayerRegionTrimMetal = 20, /**< LEF58: specifies a set of region- or area-based rules on a trim layer */

    // routing layer with LEF58_TYPE
    kLayerPolyRouting    = 21,  /**< LEF58: indicate that the polysilicon layer should be considered as a routing layer */
    kLayerMIMCapRouting  = 22,  /**< LEF58: a mimcap metal layer is not to be used as a routing layer */
    kLayerHighrRouting   = 23,  /**< LEF58: high resistance routing layer is not used as a regular routing layer */
    kLayerTSVMetalRouting = 24,  /**< LEF58: not used for signal routing */
    kLayerPadMetalRouting = 25,  /**< LEF58: on the top or at the backside in layer stack. not used for signal routing */
    kLayerStackedMIMCapRouting = 26,  /**< LEF58: not used for signal routing */

    // routing layer with LEF58_REGION
    kLayerRegionRouting  = 27,  /**< LEF58: specifies a set of region- or area-based rules on a routing layer */
    kLayerMax = 28,
};


/**
 * @brief
 * relationship between LEF string and open-edi enum
 * it is for LEF parser only so far
 */
class LayerTypeTable {
  public:
    LayerTypeTable(const char* lef_type, LayerType edi_type) : lef_type_(lef_type), edi_type_(edi_type) {
    }

    const char *getLEFType() const {
        return lef_type_;
    }
    const LayerType getEDIType() const {
        return edi_type_;
    }

    const char*  lef_type_;
    const LayerType edi_type_;
};

UInt32 GetLayerTypeTableSize();
const char* GetLEFTypeFromTables(UInt32 index);
const LayerType GetEDITypeFromTables(UInt32 index);

enum LayerDirection {
    kHoriz    =   0,  /**< Routing parallel to the x axis is preferred */
    kVert     =   1,  /**< Routing parallel to the y axis is preferred */
    kDiag45   =   2,  /**< Rouing along a 45-degree angle is preferred */
    kDiag135  =   3,  /**< Routing along a 135-degree angle is preferred */
    kUnknownDir
};

const UInt32 kMaxOxideNum = 33; // the number of ANTENNAMODEL oxide(OXIDE1, OXIDE2, ..., OXIDE32)

class ImplantLayerRule;
class RoutingLayerRule;
class CutLayerRule;
class Cell;

class TrimLayerRule : public Object {
  public:
    TrimLayerRule() {
        memset(static_cast<void*>(this), 0, sizeof(TrimLayerRule));
    }

  private:
};

class MEOLLayerRule : public Object {
  public:
    MEOLLayerRule() {
        memset(static_cast<void*>(this), 0, sizeof(MEOLLayerRule));
    }

    UInt32 getMaxLength() const {
        return max_length_;
    }
    void   setMaxLength(UInt32 v) {
        max_length_ = v;
    }

  private:
    UInt32  max_length_;  /**< maximum length of any shape on MEOL layer */

};

/**
 * @brief
 * the implementation class of antenna model
 *
 * Common antenna rules on both cut and routing layers (default ANTENNAMODEL is OXIDE1 if no ANTENNAMODEL is specified)
 *
 * [ ANTENNAMODEL  {OXIDE1 | OXIDE2 | OXIDE3 | OXIDE4 | OXIDE5 | ...  | OXIDE32} ;] ...
 *   [ ANTENNAAREARATIO   value ;]
 *   [ ANTENNADIFFAREARATIO  { value  | PWL ( ( d1   r1 ) ( d2   r2 ) ...)} ;]
 *   [ ANTENNACUMAREARATIO   value ;]
 *   [ ANTENNACUMDIFFAREARATIO  { value  | PWL ( ( d1   r1 ) ( d2   r2 ) ...)} ;]
 *   [ ANTENNAAREAFACTOR   value  [DIFFUSEONLY] ;]
 *   [ ANTENNACUMROUTINGPLUSCUT  ;]
 *
 *   [ ANTENNAGATEPLUSDIFF   plusDiffFactor  ;]
 *   [ PROPERTY LEF58_ANTENNAGATEPLUSDIFF
 *     "ANTENNAGATEPLUSDIFF {OXIDE1 | OXIDE2 | OXIDE3 | OXIDE4 | OXIDE5 | ...  | OXIDE32}
 *     { plusDiffFactor  | PWL  (( diffArea1   plusDiffProtect1 ) ( diffArea1   plusDiffProtect2 ) ...)} ;" ;]
 *
 *   [ ANTENNAAREAMINUSDIFF   minusDiffFactor  ;]
 *   [ ANTENNAAREADIFFREDUCEPWL  ( (  diffArea1   diffAreaFactor1  ) (  diffArea2   diffAreaFactor2  ) ...) ; ]
 *
 *   [ PROPERTY LEF58_ANTENNADIFFGATEPWL
 *     "ANTENNADIFFGATEPWL {OXIDE1 | OXIDE2 | OXIDE3 | OXIDE4 | OXIDE5 | ...  | OXIDE32}
 *     (( gateArea1   effectiveGateArea1 ) ( gateArea2   effectiveGateArea2 ) ... ) ;" ;]
 *   [ PROPERTY LEF58_ANTENNAGATEPWL
 *     "ANTENNAGATEPWL {OXIDE1 | OXIDE2 | OXIDE3 | OXIDE4 | OXIDE5 | ...  | OXIDE32}
 *     (( gateArea1   effectiveGateArea1 ) ( gateArea2   effectiveGateArea2 ) ... ) ;" ;]
 *
 * Routing layers only
 *
 *   [ ANTENNASIDEAREARATIO   value ;]
 *   [ ANTENNADIFFSIDEAREARATIO  { value  | PWL ( ( d1   r1 ) ( d2   r2 ) ...) } ;]
 *   [ ANTENNACUMSIDEAREARATIO   value ;]
 *   [ ANTENNACUMDIFFSIDEAREARATIO  { value  | PWL ( ( d1   r1 ) ( d2   r2 ) ...) } ;]
 *   [ ANTENNASIDEAREAFACTOR   value  [DIFFUSEONLY] ;]
 */
struct FloatPair {
    float data_first;
    float data_second;
};

class AntennaModel : public Object {
  public:
    AntennaModel() {
        memset(static_cast<void*>(this), 0, sizeof(AntennaModel));
    }
    ~AntennaModel() {
        reset();
    }

  public:
    void reset();

  private:
  //  void    release(std::vector<std::pair<float, float>>*& ptr, UInt32 is_ptr_set);
  //  void    pushBackPair(std::vector<std::pair<float, float>>*& pairs, float first, float second);

  public:
    bool   isSet() const;
    bool   isDiffAreaRatioPWL() const;
    bool   isCumDiffAreaRatioPWL() const;
    bool   isDiffUseOnly() const;
    bool   isCumRoutingPlusCut() const;
    bool   isDiffSideAreaRatioPWL() const;
    bool   isCumDiffSideAreaRatioPWL() const;
    bool   isSideDiffUseOnly() const;
    bool   isDefaultAreaFactor() const;
    float  getAreaRatio() const;
    float  getCumAreaRatio() const;
    float  getAreaFactor() const;
    float  getAreaMinusDiff() const;
    float  getSideAreaRatio() const;
    float  getCumSideAreaRatio() const;
    float  getSideAreaFactor() const;
    float  getDiffAreaRatio() const;
    FloatPair* getDiffAreaRatioPWL(UInt32 index) const;
    UInt32 getDiffAreaRatioPWLSize() const;
    float  getCumDiffAreaRatio() const;
    FloatPair* getCumDiffAreaRatioPWL(UInt32 index) const;
    UInt32 getCumDiffAreaRatioPWLSize() const;
    UInt32 getGatePlusDiffId() const;
    float  getGatePlusDiff() const;
    FloatPair* getGatePlusDiffPWL(UInt32 index) const;
    UInt32 getGatePlusDiffPWLSize() const;
    FloatPair* getAreaDiffReducePWL(UInt32 index) const;
    UInt32 getAreaDiffReducePWLSize() const;
    UInt32 getDiffGatePWLId() const;
    FloatPair* getDiffGatePWL(UInt32 index) const;
    UInt32 getDiffGatePWLSize() const;
    UInt32 getGatePWLId() const;
    FloatPair* getGatePWL(UInt32 index) const;
    UInt32 getGatePWLSize() const;
    float  getDiffSideAreaRatio() const;
    FloatPair* getDiffSideAreaRatioPWL(UInt32 index) const;
    UInt32 getDiffSideAreaRatioPWLSize() const;
    float  getCumDiffSideAreaRatio() const;
    FloatPair* getCumDiffSideAreaRatioPWL(UInt32 index) const;
    UInt32 getCumDiffSideAreaRatioPWLSize() const;

    void   setIsSet(bool v);
    void   setIsDiffAreaRatioPWL(bool v);
    void   setIsCumDiffAreaRatioPWL(bool v);
    void   setIsDiffUseOnly(bool v);
    void   setIsCumRoutingPlusCut(bool v);
    void   setIsDiffSideAreaRatioPWL(bool v);
    void   setIsCumDiffSideAreaRatioPWL(bool v);
    void   setIsSideDiffUseOnly(bool v);
    void   setIsDefaultAreaFactor(bool v);
    void   setAreaRatio(float r);
    void   setCumAreaRatio(float r);
    void   setAreaFactor(float f);
    void   setAreaMinusDiff(float f);
    void   setSideAreaRatio(float r);
    void   setCumSideAreaRatio(float r);
    void   setSideAreaFactor(float f);
    void   setDiffAreaRatio(float r);
    void   addDiffAreaRatioPWL(float d, float r);
    void   setCumDiffAreaRatio(float r);
    void   addCumDiffAreaRatioPWL(float d, float r);
    void   setGatePlusDiffId(UInt32 id);
    void   setGatePlusDiff(float d);
    void   addGatePlusDiffPWL(float d, float p);
    void   addAreaDiffReducePWL(float f, float d);
    void   setDiffGatePWLId(UInt32 index);
    void   addDiffGatePWL(float gate_area, float effective_gate_area);
    void   setGatePWLId(UInt32 index);
    void   addGatePWL(float gate_area, float effective_gate_area);
    void   setDiffSideAreaRatio(float ratio);
    void   addDiffSideAreaRatioPWL(float d, float r);
    void   setCumDiffSideAreaRatio(float v);
    void   addCumDiffSideAreaRatioPWL(float d, float r);

  private:
    float   area_ratio_;  // ANTENNAAREARATIO  cut & routing layers share it
    float   cum_area_ratio_;  // ANTENNACUMAREARATIO cut & routing layers share it
    float   area_factor_;  // ANTENNAAREAFACTOR  cut & routing layers share it
    float   area_minus_diff_;  // ANTENNAAREAMINUSDIFF cut & routing layers share it
    float   side_area_ratio_;  // ANTENNASIDEAREARATIO routing layer only
    float   cum_side_area_ratio_;  // ANTENNACUMSIDEAREARATIO  routing layer only
    float   side_area_factor_;  // ANTENNASIDEAREAFACTOR routing layer only

    // ANTENNADIFFAREARATIO  cut & routing layers share it
    union {
        float   diff_area_ratio_;  // ANTENNADIFFAREARATIO value
        ObjectId diff_area_ratio_pwl_;  // ANTENNADIFFAREARATIO PWL
    };

    // ANTENNACUMDIFFAREARATIO cut & routing layers share it
    union {
        float   cum_diff_area_ratio_; // ANTENNACUMDIFFAREARATIO value
        ObjectId cum_diff_area_ratio_pwl_;
    };

    // ANTENNAGATEPLUSDIFF cut & routing layers share it
    int   gate_plus_diff_id_;  // ANTENNAGATEPLUSDIFF  OXIDE1 | ... | OXIDE32
    union {
        float   gate_plus_diff_;  // ANTENNAGATEPLUSDIFF
        ObjectId gate_plus_diff_pwl_; // ANTENNAGATEPLUSDIFF PWL
    };

    // ANTENNAAREADIFFREDUCEPWL cut & routing layers share it
    ObjectId area_diff_reduce_pwl_; // ANTENNAAREADIFFREDUCEPWL

    // ANTENNADIFFGATEPWL cut & routing layers share it
    int   diff_gate_pwl_id_;  // ANTENNADIFFGATEPWL OXIDE1 | ... | OXIDE32
    ObjectId diff_gate_pwl_; // ANTENNADIFFGATEPWL

    // ANTENNAGATEPWL cut & routing layers share it
    int   gate_pwl_id_;  // ANTENNAGATEPWL OXIDE1 | ... | OXIDE32
    ObjectId gate_pwl_; // ANTENNAGATEPWL

    // ANTENNADIFFSIDEAREARATIO routing layer only
    union {
        float   diff_side_area_ratio_;  // ANTENNADIFFSIDEAREARATIO value
        ObjectId diff_side_area_ratio_pwl_; // ANTENNADIFFSIDEAREARATIO PWL
    };

    // ANTENNACUMDIFFSIDEAREARATIO routing layer only
    union {
        float   cum_diff_side_area_ratio_;  // ANTENNACUMDIFFSIDEAREARATIO value
        ObjectId cum_diff_side_area_ratio_pwl_; // ANTENNACUMDIFFSIDEAREARATIO PWL
    };

    Bits    is_set_ : 1;  // whether current ANTENNAMODEL is alreayd set or not
    Bits    is_default_area_factor_ : 1;  // ANTENNAAREAFACTOR is not set, use default 1.0
    Bits    is_diff_area_ratio_pwl_ : 1;  // ANTENNADIFFAREARATIO PWL  cut & routing layers share it
    Bits    is_cum_diff_area_ratio_pwl_ : 1;  // ANTENNACUMDIFFAREARATIO PWL  cut & routing layers share it
    Bits    is_gate_plus_diff_pwl_ : 1;  // ANTENNAGATEPLUSDIFF PWL cut & routing layers share it
    Bits    is_diff_use_only_ : 1;  // ANTENNAAREAFACTOR DIFFUSEONLY  cut & routing layers share it
    Bits    is_cum_routing_plus_cut_ : 1;  // ANTENNACUMROUTINGPLUSCUT  cut & routing layers share it
    Bits    is_diff_side_area_ratio_pwl_ : 1;  // ANTENNADIFFSIDEAREARATIO PWL
    Bits    is_cum_diff_side_area_ratio_pwl_ : 1;  // ANTENNACUMDIFFSIDEAREARATIO PWL
    Bits    is_side_diff_use_only_ : 1; // ANTENNASIDEAREAFACTOR x DIFFUSEONLY  routing layer only
};


/**
 * @brief
 * the implementation class of AREA syntax (routing layer, implant layer and masterslice trim layer)
 * so, it is treated as a Layer attribute
 *
 * [ AREA   minArea ;]
 *
 * [ PROPERTY LEF58_AREA
 *    "AREA  minArea  [MASK  maskNum ]
 *        [EXCEPTMINWIDTH  minWidth
 *        | [EXCEPTEDGELENGTH { minEdgeLength   maxEdgeLength  |  edgeLength }]
 *          [EXCEPTMINSIZE { minWidth   minLength }...]
 *          [EXCEPTSTEP  length1   length2 ]
 *        | RECTWIDTH  rectWidth
 *        | EXCEPTRECTANGLE
 *        | LAYER  trimLayer  OVERLAP {1 | 2} ] ;..." ;]
 */
struct UintPair {
  UInt32 data_first;
  UInt32 data_second;
};

class MinArea : public Object {
  public:
    double    getArea() const;
    UInt32    getMaskNum() const;
    bool      isExceptMinWidth() const;
    UInt32    getMinWidth() const;
    bool      isExceptEdgeLength() const;
    bool      isExceptMinMaxEdgeLength() const;
    UInt32    getExceptMinEdgeLength() const;
    UInt32    getExceptMaxEdgeLength() const;
    UInt32    getExceptEdgeLength() const;
    bool      isExceptMinSize() const;
    UintPair* getExceptMinSizePair(UInt32 index);
    UInt32    getExceptMinSizeSize() const;
    bool      isExceptStep() const;
    UInt32    getExceptStepLength1() const;
    UInt32    getExceptStepLength2() const;
    bool      isRectWidth() const;
    UInt32    getRectWidth() const;
    bool      isExceptRectangle() const;
    bool      isLayer() const;
    Layer*    getLayer() const;
    UInt32    getOverlap() const;

    void      setArea(double area);
    void      setMaskNum(UInt32 num);
    void      setIsExceptMinWidth(bool v);
    void      setMinWidth(UInt32 w);
    void      setIsExceptEdgeLength(bool v);
    void      setIsExceptMinMaxEdgeLength(bool v);
    void      setExceptMinEdgeLength(UInt32 min_len);
    void      setExceptMaxEdgeLength(UInt32 max_len);
    void      setExceptMinMaxEdgeLength(UInt32 min_len, UInt32 max_len);
    void      setExceptEdgeLength(UInt32 len);
    void      setIsExceptMinSize(bool v);
    void      appendExceptMinSizePair(UInt32 min_width, UInt32 min_len);
    void      setIsExceptStep(bool v);
    void      setExceptStepLength1(UInt32 len);
    void      setExceptStepLength2(UInt32 len);
    void      setIsRectWidth(bool v);
    void      setRectWidth(UInt32 w);
    void      setIsExceptRectangle(bool v);
    void      setLayer(ObjectId l);
    void      setOverlap(UInt32 ol);

  private:
    double    min_area_;                    // AREA minArea
    Bits      mask_num_               : 3;  // MASK maskNum
    Bits      is_except_min_width_    : 1;  // EXCEPTMINWIDTH
    Bits      is_except_edge_length_  : 1;  // EXCEPTEDGELENGTH
    Bits      is_min_max_edge_length_ : 1;  // EXCEPTEDGELENGTH {minLength maxLength} or length
    Bits      is_except_min_size_     : 1;  // EXCEPTMINSIZE
    Bits      is_except_step_         : 1;  // EXCEPTSTEP
    Bits      is_rect_width_          : 1;  // RECTWIDTH
    Bits      is_except_rectangle_    : 1;  // EXCEPTRECTANGLE
    Bits      is_layer_               : 1;  // LAYER
    Bits      layer_overlap_          : 2;  // LAYER x OVERLAP
    Bits      unused_ : 9;

    ObjectId    layer_;                       // LAYER trimLayer

    union {
        UInt32  min_width_;                   // EXCEPTMINWIDTH
        UInt32  rect_width_;                  // RECTWIDTH
    };

    UInt32 except_edge_min_length_;      // EXCEPTEDGELENGTH minEdgeLength  or edgeLength
    UInt32 except_edge_max_length_;      // EXCEPTEDGELENGTH x maxEdgeLength
    ObjectId except_min_size_;  // EXCEPTMINSIZE  {min max} pairs
    UInt32 except_step_length1_;         // EXCEPTSTEP length1
    UInt32 except_step_length2_;         // EXCEPTSTEP x length2
};

/**
 * @brief
 * the implementation class of ACCURRENTDENSITY & DCCURRENTDENSITY syntax
 *
 * Routing layer:
 * [ ACCURRENTDENSITY  {PEAK | AVERAGE | RMS}
 *    { value
 *      | FREQUENCY  freq_1 freq_2 ...  ;
 *        [WIDTH  width_1 width_2 ...  ;]
 *          TABLEENTRIES
 *          v_freq_1_width_1 v_freq_1_width_2 ...
 *          v_freq_2_width_1 v_freq_2_width_2 ...
 *          ...
 *    } ;]
 * [ PROPERTY LEF58_ACCURRENTDENSITY
 *    "ACCURRENTDENSITY {PEAK | AVERAGE | RMS}
 *    [TEMPPWL   temp_1    multi_1   temp_2    multi_2   ...]
 *    [HOURSPWL  hours_1   multi_1   hours_2   multi_2   ...]
 *    ; " ;]
 *
 * [ DCCURRENTDENSITY  AVERAGE
 *    { value
 *    | WIDTH  width_1 width_2 ...  ;
 *        TABLEENTRIES  value_1 value_2 ...
 *    } ; ]
 *
 * Cut layer:
 * [ ACCURRENTDENSITY  {PEAK | AVERAGE | RMS}
 *    { value
 *      | FREQUENCY freq_1 freq_2 ... ;
 *        [CUTAREA cutArea_1  cutArea_2  ... ;]
 *          TABLEENTRIES
 *          v_freq_1_cutArea_1 v_freq_1_cutArea_2 ...
 *          v_freq_2_cutArea_1 v_freq_2_cutArea_2 ...
 *        ...
 *     } ;]
 * [ DCCURRENTDENSITY  AVERAGE
 *    { value
 *    | CUTAREA  cutArea_1   cutArea_2 ...  ;
 *        TABLEENTRIES  value_1 value_2 ...  } ;]
 */
class CurrentDen : public Object {
  public:
    CurrentDen() {
        memset(static_cast<void*>(this), 0, sizeof(CurrentDen));
    }
    ~CurrentDen();

  public:
    bool    isValid() const;
    bool    isPeak() const;
    bool    isAverage() const;
    bool    isRMS() const;
    bool    isAcDen() const;
    bool    isDcDen() const;
    bool    isPWL() const;
    bool    hasTableEntries() const;
    float   getCurrentDen() const;
    // ACCURRENTDENSITY  routing or cut layer
    ArrayObject<float>* getFrequencies() const;
    float   getFrequency(UInt32 index) const;
    UInt32  getFrequenciesNum() const;
    void    setFrequecies(float* frequencies, UInt32 freq_num);
    void    addFrequency(float frequency);
    void    setFrequenciesNum(UInt32 freq_num);
    // ACCURRENTDENSITY + DCCURRENTDENSITY  routing layer
    ArrayObject<UInt32> *getWidths() const;
    UInt32  getWidth(UInt32 index) const;
    UInt32  getWidthsNum() const;
    void    addWidth(UInt32 width);
    void    setWidthsNum(UInt32 widths_num);
    // ACCURRENTDENSITY + DCCURRENTDENSITY  cut layer
    ArrayObject<float> *getCutAreas() const;
    float  getCutArea(UInt32 index) const;
    UInt32  getCutAreasNum() const;
    void    setCurrentDen(float den);
    void    addCutArea(float cut_area);
    void    setCutAreasNum(UInt32 cut_area_num);
    // ACCURRENTDENSITY PWL
    UInt32  getTempPWLPairNum() const;
    float   getTempPWLTemp(UInt32 index) const;
    float   getTempPWLMultiplier(UInt32 index) const;
    UInt32  getHoursPWLPairNum() const;
    float   getHoursPWLHours(UInt32 index) const;
    float   getHoursPWLMultiplier(UInt32 index) const;
    void    setTempPWLPairNum(UInt32 num);
    void    setTempPWLTemp(float t, UInt32 index); 
    void    setTempPWLMultiplier(float m, UInt32 index);
    void    setTempPWL(float t, float m, UInt32 index);
    void    setHoursPWLPairNum(UInt32 num);
    void    setHoursPWLHours(float h, UInt32 index);
    void    setHoursPWLMultiplier(float m, UInt32 index);
    void    setHoursPWL(float h, float m, UInt32 index);

    float   getTableElem(UInt32 row_idx, UInt32 col_index) const;
    void    setValid(bool v);
    void    setIsPeak(bool v);
    void    setIsAverage(bool v);
    void    setIsRMS(bool v);
    void    setIsAcDen(bool v);
    void    setIsDcDen(bool v);
    void    setIsPWL(bool v);
    void    setTableElem(float value, UInt32 row_idx, UInt32 col_idx);
    void    reset();

  private:
    float  current_den_;
    union {
        ObjectId  frequencies_;
        ObjectId  temp_pwl_;
    };
    ObjectId  table_entries_;
    union {
        ObjectId widths_;
        ObjectId cut_areas_;
        ObjectId hours_pwl_;
    };
    union {
        UInt32 freq_num_;
        UInt32 temp_pwl_pair_num_;
    };
    union {
        UInt32 widths_num_;
        UInt32 cut_areas_num_;
        UInt32 hours_pwl_pair_num_;
    };
    UInt32 table_entries_num_;
    Bits  is_peak_    : 1;
    Bits  is_average_ : 1;
    Bits  is_rms_     : 1;
    Bits  is_ac_den_  : 1;
    Bits  is_dc_den_  : 1;
    Bits  is_valid_   : 1;
    Bits  is_pwl_     : 1;  // ACCURRENTDENSITY TEMPPWL or HOURSPWL
};

using ACCurrentDen = CurrentDen;
using DCCurrentDen = CurrentDen;

/**
 * @brief
 * the container of peak, average, rms current density rules
 */
class CurrentDenContainer : public Object {
    enum CurrentDenType {
        kCurrentPeak     = 0,
        kCurrentAverage  = 1,
        kCurrentRMS      = 2,
        kCurrentPWL      = 3,
        kCurrentMax      = 4
    };
  public:
    CurrentDenContainer();
    ~CurrentDenContainer() {
        //delete[] current_dens_;
    }

  public:
    // ACCURRENTDENSITY
    ACCurrentDen* getACPeak() const;
    ACCurrentDen* getACAverage() const;
    ACCurrentDen* getACRMS() const;

    // ACCURRENTDENSITY PWL
    ACCurrentDen* getACPeakPWL() const;
    ACCurrentDen* getACAveragePWL() const;
    ACCurrentDen* getACRMSPWL() const;

    // DCCURRENTDENSITY
    DCCurrentDen* getDCAverage() const;

  public:
    // used to set ACCURRENTDENSITY & DCCURRENTDENSITY when lefIn
    ACCurrentDen* getInitACPeak() ;
    ACCurrentDen* getInitACAverage() ;
    ACCurrentDen* getInitACRMS() ;
    ACCurrentDen* getInitACPeakPWL() ;
    ACCurrentDen* getInitACAveragePWL() ;
    ACCurrentDen* getInitACRMSPWL() ;
    DCCurrentDen* getInitDCAverage() ;

  private:
    ObjectId current_dens_;
};

template <typename ObjectClassName> class ArrayObjectIter {
  public:
        ArrayObjectIter(ObjectId obj_id) {
            obj_vector_ = Object::addr<IdArray>(obj_id);
            if (obj_vector_) iter_ = obj_vector_->begin();
        }
        ObjectClassName*  getNext() {
            if (!obj_vector_ || iter_ == obj_vector_->end())
                return nullptr;
            ObjectId obj_id = *iter_;
            ++iter_;
            if (!obj_id) return nullptr;
            return Object::addr<ObjectClassName>(obj_id);
        };
        void reset() { 
          if (obj_vector_) { iter_ = obj_vector_->begin(); }
        }

  private:
        IdArray* obj_vector_;
        typename IdArray::iterator iter_;
};

/**
 * @class Layer
 * @brief tech LEF layer implementation
 */
class Layer : public Object {
  public:
    Layer();
    ~Layer();

    uint64_t   getNameId() const;
    void   setNameId(uint64_t id);
    bool   setName(const char *name);
    const char* getName();

    UInt32 getZ() const;
    void   setZ(unsigned int z);

    UInt32 getIndexInLef() const;

    void   setIndexInLef(UInt32 v);

    bool   isImplant() const;
    bool   isMS() const;
    bool   isOverlap() const;
    bool   isCut() const;
    bool   isRouting() const;
    bool   isTSV() const;
    bool   isPassivation() const;
    bool   isMIMCapCut() const;
    bool   isHighrCut() const;
    bool   isRegionCut() const;
    bool   isRegion() const;
    bool   isNWell() const;
    bool   isPWell() const;
    bool   isAboveDieEdge() const;
    bool   isBelowDieEdge() const;
    bool   isDiffusion() const;
    bool   isTrimPoly() const;
    bool   isTrimMetal() const;
    bool   isMEOL() const;
    bool   isRegionTrimMetal() const;
    bool   isPolyRouting() const;
    bool   isMIMCapRouting() const;
    bool   isHighrRouting() const;
    bool   isTSVMetalRouting() const;
    bool   isPadMetalRouting() const;
    bool   isStackedMIMCapRouting() const;
    bool   isRegionRouting() const;

    void   setType(LayerType v);
    LayerType getType() const;

    bool   isVert() const;
    bool   isHoriz() const;
    bool   isDiag45() const;
    bool   isDiag135() const;
    void   setDirection(UInt32 dir);

    UInt32 getNumMask() const;
    void   setNumMask(UInt32 v);

    bool   isBackside() const;
    void   setIsBackside(bool v);

    bool   isCustomized() const;
    void   setIsCustomized(bool v);

    bool   isFixedMask() const;
    void   setIsFixedMask(bool v);

    UInt32 getFixedMaskNum() const;
    void   setFixedMaskNum(UInt32 v);

    UInt32 getWidth() const;
    void   setWidth(UInt32 v);

    MinArea* createMinArea();
    ObjectId getMinAreaRuleId() const;

    Layer* getRegionLayer() const;
    void   setRegionLayer(ObjectId v);

    Layer* getRegionBaseLayer() const;
    void   setRegionBaseLayer(ObjectId l);

    const AntennaModel* getAntennaModel(UInt32 index) const;

    AntennaModel* getInitAntennaModel(UInt32 index);

    CurrentDenContainer* getACCurrentDenContainer() const;
    void   setACCurrentDenContainer(ObjectId den);

    CurrentDenContainer* getDCCurrentDenContainer() const;
    void   setDCCurrentDenContainer(ObjectId den);

    ImplantLayerRule* getImplantLayerRule() const;
    void   setImplantLayerRule(ObjectId r);

    RoutingLayerRule* getRoutingLayerRule() const;
    void   setRoutingLayerRule(ObjectId r);

    CutLayerRule* getCutLayerRule() const;
    void   setCutLayerRule(ObjectId r);

    TrimLayerRule* getTrimLayerRule() const;
    void   setTrimLayerRule(ObjectId r);

    MEOLLayerRule* getMEOLLayerRule() const;
    void   setMEOLLayerRule(ObjectId r);

    void addProp(ObjectId obj_id);
    ObjectId getPropId() const;
  private:
    bool isGeneralRoutingLayer_() const;
    bool isGeneralCutLayer_() const;
    Tech *getTech_();

  public:
    // iterator
    typedef ArrayObjectIter<MinArea>  minAreaIter;

  private:
    Tech     *tech_;
    uint64_t nameId_;

    Bits     z_              : 10;
    Bits     lef_order_      : 10; /**< layer order index in which it appears in tech */
    Bits     type_           : 6;  /**< TYPE or LEF58_TYPE or LEF58_REGION*/
    Bits     num_mask_       : 3;  /**< LEF58_MASK */
    Bits     backside_       : 1;  /**< LEF58_BACKSIDE */
    Bits     customized_     : 1;  /**< customized layer */
    Bits     fixed_mask_     : 1;  /**< LEF_58_FIXEDMASK */

    Bits     fixed_mask_num_ : 3;  /**< LEF_58_FIXEDMASK MASK# */
    Bits     unused_bits_    : 29;

    // UInt32   min_spacing_;
    UInt32   width_;              /**< cut size or default wire width: WIDTH */
    ObjectId min_area_id_;       /**< MINAREA for routing layer, implant layer and trim layer */
    ObjectId   region_layer_;      /**< cut/routing layer LEF58_REGION REGION regionLayerName */
    ObjectId   base_layer_;        /**< cut/routing/trim layer*/
    ObjectId antenna_;  /**< antennamodel, from index 1 and index 32 for OXIDE1, ..., OXIDE32, 0 is not used. default is OXIDE1 */
    ObjectId ac_dens_;  /**< ACCURRENTDENSITY */
    ObjectId dc_dens_;  /**< DCCURRENTDENSITY */
    ObjectId ac_pwl_;   /**< ACCURRENTDENSITY TEMPPWL or HOURSPWL */
    ObjectId             properties_;

    // union {
    //     ImplantLayerRule* implant_;
    //     RoutingLayerRule* routing_; /**< routing layer rules */
    //     CutLayerRule*     cut_;    /**< cut layer rules */
    //     TrimLayerRule*    trim_;   /**< trimmetal layer rules */
    //     MEOLLayerRule*    meol_;   /**< middle-end-of-line layer rules */
    // } layer_rule_;
    ObjectId layer_rule_;
};

}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_TECH_LAYER_H_
