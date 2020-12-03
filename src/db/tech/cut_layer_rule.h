/* @file  cut_layer_rule.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_DB_TECH_CUT_LAYER_RULE_H_
#define EDI_DB_TECH_CUT_LAYER_RULE_H_

#include <string>

#include "db/tech/layer.h"
#include "util/util.h"
#include "db/util/array.h"

namespace open_edi {
namespace db {

class Layer;
class CutSpacing;
class Enclosure;
class ArraySpacing;

/**
 * @class CutLayerRule
 * @brief
 * implementation of cut layer rules
 */
class CutLayerRule : public Object{
  public:
    CutLayerRule();
    ~CutLayerRule();
    ArrayObject<ObjectId> *getCutSpacingArray() const;
    CutSpacing *getCutSpacing(int i) const;
    void addCutSpacing(ObjectId id);
    ArrayObject<ObjectId> *getEnclosureArray() const;
    Enclosure *getEnclosure(int i) const;
    void addEnclosure(ObjectId id);
    ArrayObject<ObjectId> *getPreferEnclosureArray() const;
    Enclosure *getPreferEnclosure(int i) const;
    void addPreferEnclosure(ObjectId id);
    ArrayObject<ObjectId> *getArraySpacingArray() const;
    ArraySpacing *getArraySpacing(int i) const;
    void addArraySpacing(ObjectId id);

  private:
    // firstly support above 40nm rules
    ObjectId cut_spacings_;
    ObjectId enclosures_;
    ObjectId prefer_enclosures_;
    ObjectId array_spacings_;
    // AC/DC CURRENT DENSITY
    // Antenna
#if 0
    // advanced node to be supported
    CutClass *cut_class_;
    CutClass **cut_class_by_id_;
    CutClassSpTable *cut_class_sp_table_[kCutClassSpacingTableNumType];
    CenterSpTable *center_sp_table_[kCenterSpacingTableNumType];
    AdjFourCuts *adj_four_cuts_;
    CutOnCenterLine *cut_on_center_line_;
    DirectionalSpacing *directional_sp_;
    EnclosureEdge *enclosure_edge_;
    EnclosureTable *enclosure_table_;
    EnclosureToJoint *enclosure_to_joint_;
    EnclosureWidth *enclosure_width_;
    EolEnclosure *eol_enclosure_;
    EolSpacing *eol_spacing_;
    CutForbiddenSpacing *forbidden_spacing_;
    KeepOutZone *keep_out_zone_;
    MaxSpacing *max_spacing_;
    OneDArray *one_d_array_;
    OrthogonalSpacing *orthogonal_spacing_;
    OrthogonalSpTable *orthogonal_sp_table_;
    PrlTwoSidesSpacing *prl_two_sides_spacing_;
    SameMetalAlignedCuts *same_metal_aligned_cuts_;
    ViaCluster *via_cluster_;
    ViaGroup *via_group_;
    ViaGroupSpacing *via_group_spacing_;
#endif
};

static const UInt32 cut_spacing_is_second_layer = 0x01;
static const UInt32 cut_spacing_is_adj_cuts = 0x02;
static const UInt32 cut_spacing_is_parallel_overlap = 0x04;

class SecondLayer;
class AdjacentCuts;
class CutSpacingPrlOvlp;
/**
 * @class CutSpacing
 * @brief
 * implementation class of CUTSPACING syntax
 * [ PROPERTY LEF58_SPACING
 *   "SPACING  cutSpacing
 *     [SAMEMASK
 *     |MAXXY
 *     |[CENTERTOCENTER]
 *       [SAMENET | SAMEMETAL | SAMEVIA]
 *       [LAYER  secondLayerName
 *          [STACK
 *          | ORTHOGONALSPACING  orthogonalSpacing ]
 *          | CUTCLASS  className
 *              [SHORTEDGEONLY [PRL  prl ]
 *              |CONCAVECORNER
 *                 [WIDTH  width  ENCLOSURE  enclosure
 *                      EDGELENGTH  edgeLength
 *                 | PARALLEL  parLength  WITHIN  parWithin
 *                      ENCLOSURE  enclosure
 *                 | EDGELENGTH  edgeLength
 *                      ENCLOSURE  edgeEnclosure   adjEnclosure ]
 *              |EXTENSION  extension
 *              |NONEOLCONVEXCORNER  eolWidth
 *                [MINLENGTH  minLength ]
 *              |ABOVEWIDTH  width  [ENCLOSURE  enclosure ]
 *              |MASKOVERLAP
 *              |WRONGDIRECTION ]]]
 *       | ADJACENTCUTS {2 | 3 | 4}
 *           [EXACTALIGNED  exactAlignedCut ]
 *           [TWOCUTS  twoCuts  [TWOCUTSSPACING  twoCutsSpacing ]
 *              [SAMECUT]
 *           | EACHCUTS  numCuts  [EXCEPTEVENNUMCUT]]
 *           WITHIN  cutWithin  |  cutWithin1   cutWithin2 }
 *           [EXCEPTSAMEPGNET]
 *           [EXCEPTALLWITHIN  exceptAllWithin ]
 *           [ENCLOSURE [ABOVE|BELOW]  enclosure
 *             [EXCEPTLINEENDGAP]]
 *           [CUTCLASS  className  [TO ALL]]
 *           [NOPRL | SIDEPARALLELOVERLAP]
 *           [SAMEMASK]
 *       | PARALLELOVERLAP [EXCEPTSAMENET | EXCEPTSAMEMETAL
 *           |EXCEPTSAMEMETALOVERLAP | EXCEPTSAMEVIA]
 *       | PARALLELWITHIN  within  [EXCEPTSAMENET]
 *           [CUTCLASS  className
 *             [LONGEDGEONLY
 *             | ENCLOSURE  enclosure  {ABOVE | BELOW}
 *                  PARALLEL  parLength  WITHIN  parWithin ]]
 *       | SAMEMETALSHAREDEDGE  parwithin  [ABOVE][CUTCLASS  className ]
 *           [EXCEPTTWOEDGES] [EXCEPTSAMEVIA  numCut ]
 *       | AREA  cutArea ]
 * ;..." ;]
 *
 */
class CutSpacing : public Object{
  public:
    CutSpacing();
    ~CutSpacing();

    Int32 getSpacing() const;
    void setSpacing(Int32 spacing);
    bool getIsC2C() const;
    void setIsC2C(bool is_c2c);
    bool getIsSameNet() const;
    void setIsSameNet(bool is_same_net);
    bool getIsCutArea() const;
    Int32 getCutArea() const;
    void setCutArea(Int32 area);
    bool getIsSecondLayer() const;
    void setIsSecondLayer();
    SecondLayer *getSecondLayer() const;
    void setSecondLayer(ObjectId id);
    bool getIsAdjCuts() const;
    void setIsAdjCuts();
    AdjacentCuts *getAdjCuts() const;
    void setAdjCuts(ObjectId id);
    bool getIsParallelOverlap() const;
    void setIsParallelOverlap();
    CutSpacingPrlOvlp *getParallelOverlap() const;
    void setParallelOverlap(ObjectId id);

  private:
    Bits is_c2c_ : 1;
    Bits is_same_net_ : 1;
    Int32 spacing_;
    Int32 cut_area_;
    UInt32 cut_spacing_type_;  // bit map flags the cut spacing type
    ObjectId cut_sp_rules_;
};

/**
 * @class SecondLayer
 * @brief
 * implementation class of LAYER secondLayerName keyword in CUTSPAICNG rule
 */
class SecondLayer : public Object{
  public:
    SecondLayer();
    ~SecondLayer();

    Int32 getSecondLayerId() const;
    void setSecondLayerId(Int32 id);
    bool getIsStack() const;
    void setIsStack(bool is_stack);

  private:
    Bits is_stack_ : 1;
    Int32 second_layer_id_;
};

/**
 * @class AdjacentCuts
 * @brief
 * implementation class of ADJACENTCUTS keyword in CUTSPAICNG rule
 */
class AdjacentCuts : public Object{
  public:
    AdjacentCuts();
    ~AdjacentCuts();

    Int32 getCutNum() const;
    void setCutNum(Int32 cut_num);
    Int32 getCutWithin() const;
    void setCutWithin(Int32 cut_within);
    bool getIsExceptSamePGNet() const;
    void setIsExceptSamePGNet(bool flag);

  private:
    Bits is_except_same_pg_net_ : 1;
    Int32 cut_num_;
    Int32 cut_within_;
};

/**
 * @class CutSpacingPrlOvlp
 * @brief
 * implementation class of PARALLELOVERLAP keyword in CUTSPAICNG rule
 */
class CutSpacingPrlOvlp : public Object{
  public:
    CutSpacingPrlOvlp();
    ~CutSpacingPrlOvlp();

    bool getIsParallelOverlap() const;
    void setIsParallelOverlap(bool is_plr_ovlp);

  private:
    Bits is_parallel_overlap_ : 1;
};

static const UInt32 enclosure_is_eol = 0x01;
static const UInt32 enclosure_is_overhang = 0x02;
class EnclosureEol;
class EnclosureOverhang;
/**
 * @class Enclosure
 * @brief
 * implementation class of enclosure rules
 * [ PROPERTY LEF58_ENCLOSURE
 *   "ENCLOSURE [CUTCLASS  className ][ABOVE | BELOW]
 *     [MINCORNER]
 *     {EOL  eolWidth  [HORIZONTAL | VERTICAL]
 *         [MINLENGTH  minLength ]
 *         [EOLONLY] [SHORTEDGEONEOL]  eolOverhang   otherOverhang
 *         [SIDESPACING  spacing  EXTENSION  backwardExt   forwardExt
 *         |ENDSPACING  spacing  EXTENSION  extension
 *         ]
 *     |{ overhang1   overhang2
 *         |[OFFCENTERLINE] END  overhang1  SIDE  overhang2
 *         |HORIZONTAL  overhang1  VERTICAL  overhang2 }
 *         [JOGLENGTHONLY  length  [INCLUDELSHAPE] ]
 *         [HOLLOW {HORIZONTAL|VERTICAL}  length ]
 *         [ WIDTH  minWidth
 *              [INCLUDEABUTTED]
 *              [EXCEPTEXTRACUT  cutWithin
 *                  [PRL | NOSHAREDEDGE | EXACTPRL  prl ]]
 *         | LENGTH  minLength
 *         | EXTRACUT [EXTRAONLY [PRL  prl ]]
 *         | REDUNDANTCUT  cutWithin
 *         | PARALLEL  parLength  [ parLength2 ] WITHIN  parWithin  [ parWithin2
 * ] [BELOWENCLOSURE  belowEnclosure [ALLSIDES  enclosure1   enclosure2 ]
 *              |ABOVEENCLOSURE  aboveEnclosure ]
 *         | CONCAVECORNERS  numCorner
 *         | OTHERWITHINWIDTH  width  WITHIN  within
 *         [ OTHERSIDE  otherEnclosure ]]
 *       }
 *   ;..." ;]
 */
class Enclosure : public Object{
  public:
    Enclosure();
    ~Enclosure();

    bool getIsAbove() const;
    void setIsAbove(bool is_above);
    bool getIsBelow() const;
    void setIsBelow(bool is_below);
    bool getIsEol() const;
    void setIsEol();
    EnclosureEol *getEol() const;
    void setEol(ObjectId id);
    bool getIsOverhang() const;
    void setIsOverhang();
    EnclosureOverhang *getOverhang() const;
    void setOverhang(ObjectId id);

  private:
    Bits is_above_ : 1;
    Bits is_below_ : 1;
    UInt32 enclosure_type_;  // bit map flags the enclosure type
    ObjectId enclosure_rules_;
};

/**
 * @class EnclosureEol
 * @brief
 * implementation class of EOL keyword in ENCLOSURE rule
 */
// advaned node rules, supported later
class EnclosureEol : public Object{
  public:
    EnclosureEol();
    ~EnclosureEol();

  private:
};

/**
 * @class EnclosureOverhang
 * @brief
 * implementation class of OVERHANG keyword in ENCLOSURE rule
 */
class EnclosureOverhang : public Object{
  public:
    EnclosureOverhang();
    ~EnclosureOverhang();

    Int32 getOverhang1() const;
    void setOverhang1(Int32 overhang);
    Int32 getOverhang2() const;
    void setOverhang2(Int32 overhang);
    bool isWidth() const;
    Int32 getMinWidth() const;
    void setMinWidth(Int32 width);
    bool isExceptExtraCut() const;
    Int32 getExceptExtraCutWithin() const;
    void setExceptExtraCutWithin(Int32 within);
    bool isLength() const;
    Int32 getMinLength() const;
    void setMinLength(Int32 length);

  private:
    // MUST values, overhang1 overhang2
    Int32 overhang1_;
    Int32 overhang2_;
    // WIDTH minWidth [EXCEPTEXTRACUT cutWithin]
    Int32 min_width_;
    Int32 except_extra_cut_within_;
    // LENGTH minLength
    Int32 min_length_;
    // Other rules
};

/**
 * @class ArraySpacing
 * @brief
 * implementation class of ARRAYSPACING rules
 * [ PROPERTY LEF58_ARRAYSPACING
 *     "ARRAYSPACING [CUTCLASS  className ] [PARALLELOVERLAP]
 *         [LONGARRAY] [WIDTH  viaWidth ]
 *         [WITHIN within ARRAYWIDTH arrayWidth] CUTSPACING  cutSpacing
 *         {ARRAYCUTS  arrayCuts  SPACING  arraySpacing } ...
 *         ;..." ;]
 */
class ArraySpacing : public Object{
  public:
    ArraySpacing();
    ~ArraySpacing();

    bool getIsLongArray() const;
    void setIsLongArray(bool is_long_array);
    bool hasViaWidth() const;
    Int32 getViaWidth() const;
    void setViaWidth(Int32 via_width);
    Int32 getCutSpacing() const;
    void setCutSpacing(Int32 cut_spacing);
    Int32 getNumArrayCuts() const;
    void setNumArrayCuts(Int32 num);
    ArrayObject<Int32> *getArrayCuts() const;
    Int32 getArrayCuts(Int32 idx) const;
    void addArrayCuts(Int32 array_cuts);
    ArrayObject<Int32> *getArraySpacing() const;
    Int32 getArraySpacing(Int32 idx) const;
    void addArraySpacing(Int32 array_spacing);
    void setArray(Int32 num_array_cuts, Int32 *array_cuts,
                  Int32 *array_spacing);

  private:
    Bits is_long_array_ : 1;
    Int32 via_width_;
    Int32 cut_spacing_;
    Int32 num_array_cuts_;
    ObjectId array_cuts_;
    ObjectId array_spacing_;
};

}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_TECH_CUT_LAYER_RULE_H_
