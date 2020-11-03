/* @file  routing_layer_rule.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_DB_TECH_ROUTING_LAYER_RULE_H_
#define EDI_DB_TECH_ROUTING_LAYER_RULE_H_

#include <vector>
#include <string>
#include "db/tech/layer.h"

namespace open_edi {
namespace db {

class Layer;
class CutClass;

/**
 * @brief
 * free object list
 *
 * @tparam T
 * @param r
 */
template<typename T> void FreeRuleList(T*& r) {
    while (r) {
        T* next_r = r->getNext();
        delete r;
        r = next_r;
    }
}

/**
 * @class BoundaryEOLBlockage
 * @brief
 * the implemenation calss of BOUNDARYEOLBLOCKAGE syntax
 * [PROPERTY LEF58_BOUNDARYEOLBLOCKAGE
 *        “BOUNDARYEOLBLOCKAGE  size  OFFSET  offset
 *           [PARALLEL  parLength  WITHIN  parWithin  SPACING  spacing ]
 *           ;” ;]
 */
class BoundaryEOLBlockage {
  public:
    BoundaryEOLBlockage() {
        memset(static_cast<void*>(this), 0, sizeof(BoundaryEOLBlockage));
    }

    UInt32  getSize() const;
    UInt32  getOffset() const;
    UInt32  getParallelLength() const;
    UInt32  getParallelWithin() const;
    UInt32  getSpacing() const;
    BoundaryEOLBlockage* getNext() const;  // to use template function to release rule

    void    setSize(UInt32 size);
    void    setOffset(UInt32 offset);
    void    setParallelLength(UInt32 len);
    void    setParallelWithin(UInt32 dist);
    void    setSpacing(UInt32 spacing);

  private:
    UInt32  size_;
    UInt32  offset_;
    UInt32  par_length_;
    UInt32  par_within_;
    UInt32  spacing_;
};

/**
 * @class CornerEOLKeepout
 * @brief
 * the implementation calss of CORNEREOLKEEPOUT syntax
 * [PROPERTY LEF58_CORNEREOLKEEPOUT
 *        “CORNEREOLKEEPOUT WIDTH  eolWidth  EOLSPACING  eolSpacing
 *           { SPACING  spacing1   spacing2  WITHIN  within1 within2
 *           | EXTENSION  backwardExt sideExt forwardExt  }
 *           ;” ; ...]
 */
class CornerEOLKeepout {
  public:
    CornerEOLKeepout() {
        memset(static_cast<void*>(this), 0, sizeof(CornerEOLKeepout));
    }

  public:
    UInt32  getSpacing1() const;
    UInt32  getSpacing2() const;
    UInt32  getWithin1() const;
    UInt32  getWithin2() const;

    UInt32  getBackwardExt() const;
    UInt32  getSideExt() const;
    UInt32  getForwardExt() const;

    void    setSpacing(UInt32 sp1, UInt32 sp2, UInt32 within1, UInt32 within2);
    void    setExtension(UInt32 backward, UInt32 side, UInt32 forward);

    bool    isSpacing() const;

    CornerEOLKeepout* getNext() const;
    void    setNext(CornerEOLKeepout* next);

  private:
    CornerEOLKeepout* next_;
    UInt32  eol_width_;
    UInt32  eol_spacing_;
    bool    is_spacing_;

    union {
        struct {
            UInt32  spacing1_;
            UInt32  spacing2_;
            UInt32  within1_;
            UInt32  within2_;
        } spacing_;
        struct {
            UInt32  backward_ext_;
            UInt32  side_ext_;
            UInt32  forward_ext_;
        } extension_;
    } u_;
};

/**
 * @class CornerFillSpacing
 * @brief
 * implementation calss of CORNERFILLSPACING syntax
 *
 * [PROPERTY LEF58_CORNERFILLSPACING
 *      "CORNERFILLSPACING  spacing  EDGELENGTH  length1   length2
 *      ADJACENTEOL  eolWidth  ;” ; ... ]
 */
class CornerFillSpacing {
  public:
    CornerFillSpacing() {
        memset(static_cast<void*>(this), 0, sizeof(CornerFillSpacing));
    }

  public:
    UInt32  getSpacing() const;
    UInt32  getLength1() const;
    UInt32  getLength2() const;
    UInt32  getEOLWidth() const;
    CornerFillSpacing* getNext() const;

    void    setCornerFillSpacing(UInt32 sp, UInt32 len1, UInt32 len2, UInt32 eol_w);
    void    setNext(CornerFillSpacing* next);

  private:
    UInt32  spacing_;
    UInt32  length1_;
    UInt32  length2_;
    UInt32  eol_width_;

    CornerFillSpacing* next_;
};

/**
 * @class CornerSpacing
 * @brief
 * the implementation class of CORNERSPACING syntax
 *
 *[ PROPERTY LEF58_CORNERSPACING
 *       "CORNERSPACING
 *          {CONVEXCORNER [SAMEMASK]
 *            [CORNERONLY  within  | CORNERTOCORNER]
 *            [EXCEPTEOL  eolWidth
 *              [EXCEPTJOGLENGTH  length  [EDGELENGTH]
 *                [INCLUDELSHAPE]]]
 *          |CONCAVECORNER
 *            [MINLENGTH  minLength ] [EXCEPTNOTCH [ notchLength ]] }
 *          [EXCEPTSAMENET | EXCEPTSAMEMETAL]
 *          {WIDTH  width  SPACING  spacing
 *          | WIDTH  width  SPACING  horizontalSpacing   verticalSpacing }...
 *          ; " ;]
 */
class CornerSpacing {
  public:
    CornerSpacing() {
        memset(static_cast<void*>(this), 0, sizeof(CornerSpacing));
    }
    ~CornerSpacing();

  public:
    bool  isConvexCorner() const;
    bool  isConcaveCorner() const;
    bool  isSameMask() const;
    bool  isCornerToCorner() const;
    bool  isEdgeLength() const;
    bool  isIncludeLShape() const;
    bool  isExceptNotch() const;
    bool  isExceptSameNet() const;
    bool  isExceptSameMetal() const;
    void  setIsConvexCorner(bool b);
    void  setIsConcaveCorner(bool b);
    void  setIsSameMask(bool b);
    void  setIsCornerToCorner(bool b);
    void  setIsEdgeLength(bool b);
    void  setIsIncludeLShape(bool b);
    void  setIsExceptNotch(bool b);
    void  setIsExceptSameNet(bool b);
    void  setIsExceptSameMetal(bool b);
    UInt32  getCornerOnlyWithin() const;
    UInt32  getExceptEOLWidth() const;
    UInt32  getExceptJogLength() const;
    UInt32  getMinLength() const;
    UInt32  getNotchLength() const;
    UInt32  getWidth(UInt32 index) const;
    UInt32  getSpacing(UInt32 index) const;
    UInt32  getHorizontalSpacing(UInt32 index) const;
    UInt32  getVerticalSpacing(UInt32 index) const;
    UInt32  getNumWidths() const;
    CornerSpacing*  getNext() const;
    void  setCornerOnlyWithin(UInt32 within);
    void  setExceptEOLWidth(UInt32 eol_width);
    void  setExceptJogLength(UInt32 jog_len);
    void  setMinLength(UInt32 min_len);
    void  setNotchLength(UInt32 notch_len);
    void  setWidth(UInt32 w, UInt32 index);
    void  setSpacing(UInt32 spacing, UInt32 index);
    void  setHorizontalSpacing(UInt32 hsp, UInt32 index);
    void  setVertcialSpacing(UInt32 vsp, UInt32 index);
    void  setNumWidths(UInt32 num);
    void  setNext(CornerSpacing* next);

  private:
    Bits  convex_corner_ : 1;
    Bits  concave_corner_ : 1;
    Bits  same_mask_ : 1;
    Bits  corner_to_corner_ : 1;
    Bits  edge_length_ : 1;
    Bits  include_lshape_ : 1;
    Bits  except_notch_ : 1;
    Bits  except_same_net_ : 1;
    Bits  except_same_metal_ : 1;
    UInt32  corner_only_within_ ;
    union {
        UInt32  eol_width_;  // CONVEXCORNER EXCEPTEOL eolWidth
        UInt32  min_length_;  // CONCAVECORNER MINLENGHT minLength
    };
    union {
        // CONVEXCORNER EXCEPTEOL EXCEPTJOGLENGTH length
        UInt32  except_jog_length_;
        // CONCAVECORNER EXCEPTNOTCH notchLength
        UInt32  notch_length_;
    };
    UInt32  widths_num_;  // the num of widths, spacings
    UInt32* widths_;
    UInt32* horizontal_spacings_;
    UInt32* vertical_spacings_;
    CornerSpacing*  next_;
};

/**
 * @class DirSpanLengthSpTbl
 * @brief
 * the implementation class of SPACINGTABLE DIRECTIONALSPANLENGTH syntax
 *
 * [ PROPERTY LEF58_SPACINGTABLE
 *    "SPACINGTABLE
 *      DIRECTIONALSPANLENGTH [WRONGDIRECTION]
 *      [SAMEMASK]
 *      [EXCEPTJOGLENGTH  length  [EDGELENGTH] [INCLUDELSHAPE]]
 *      [EXCEPTEOL  eolWidth ]
 *      [EXACTSPANLENGTHSPACING  spanLength1  TO  spanLength2
 *        [PRL  prl ] { exactSpacing }...]...
 *      PRL { prl } ...
 *      {SPANLENGTH  spanLength
 *        [EXACTSPACING  exactSpacing
 *        | SPACINGTOMINSPAN  spacingToMinSpan ]
 *        [EXACTSELFSPACING  exactSelfSpacing ]
 *        [NOEXCEPTEOL|EOLSPACING  eolspacing ] { spacing }...}...
 *    ]; " ;
 */
class DirSpanLengthSpTbl {
  public:
    // EXACTSPANLENGTHSPACING
    class ExactSLSpacing {
      public:
        ExactSLSpacing() {
            memset(static_cast<void*>(this), 0, sizeof(ExactSLSpacing));
        }
        ~ExactSLSpacing();

      public:
        UInt32  getSpanLength1() const;
        UInt32  getSpanLength2() const;
        UInt32  getPRL() const;
        UInt32  getSpacing(UInt32 idx) const;
        UInt32  getNumSpacings() const;
        ExactSLSpacing* getNext() const;
        void    setSpanLength1(UInt32 len);
        void    setSpanLength2(UInt32 len);
        void    setPRL(UInt32 prl);
        void    setSpacing(UInt32 spacing, UInt32 index);
        void    setNumSpacings(UInt32 num);
        void    setNext(ExactSLSpacing* next);

      private:
        ExactSLSpacing* next_;
        UInt32  span_len1_;
        UInt32  span_len2_;
        UInt32  prl_;
        UInt32  spacings_num_;
        UInt32* exact_spacings_;
    };

    // SPANLENGTH
    class SpanLength {
      public:
        SpanLength() {
            memset(static_cast<void*>(this), 0, sizeof(ExactSLSpacing));
        }
        ~SpanLength();
      public:
        bool  isExactSpacing() const;
        bool  isSpacingToMinSpan() const;
        bool  isExactSelfSpacing() const;
        bool  isNoExceptEOL() const;
        bool  isEOLSpacing() const;
        UInt32  getSpanLength() const;
        UInt32  getExactSpacing() const;
        UInt32  getSpacingToMinSpan() const;
        UInt32  getExactSelfSpacing() const;
        UInt32  getEOLSpacing() const;
        UInt32  getSpacing(UInt32 col_idx) const;
        UInt32  getNumSpacings() const;
        void  setIsExactSpacing(bool v);
        void  setIsSpacingToMinSpan(bool v);
        void  setIsExactSelfSpacing(bool v);
        void  setIsNoExceptEOL(bool v);
        void  setIsEOLSpacing(bool v);
        void  setSpanLength(UInt32 len);
        void  setExactSpacing(UInt32 sp);
        void  setSpacingToMinSpan(UInt32 sp);
        void  setExactSelefSpacing(UInt32 sp);
        void  setEOLSpacing(UInt32 sp);
        void  setNumSpacings(UInt32 num);
        void  setSpacing(UInt32 spacing, UInt32 index);

      private:
        Bits    is_exact_spacing_ : 1;
        Bits    is_spacing_to_min_span_ : 1;
        Bits    is_exact_self_spacing_ : 1;
        Bits    is_no_except_eol_ : 1;
        Bits    is_eol_spacing_ : 1;

        UInt32  span_length_;
        union {
            UInt32  exact_spacing_;
            UInt32  spacing_to_min_span_;
        };
        UInt32  exact_self_spacing_;
        UInt32  eol_spacing_;
        UInt32  spacings_num_;
        UInt32* row_spacings_;
    };

  public:
    DirSpanLengthSpTbl() {
        memset(static_cast<void*>(this), 0, sizeof(DirSpanLengthSpTbl));
    }
    ~DirSpanLengthSpTbl();

  public:
    bool  isWrongDir() const;
    bool  isSameMask() const;
    bool  isEdgeLength() const;
    bool  isIncludeLShape() const;
    UInt32  getJogLength() const;
    UInt32  getEOLWidth() const;
    UInt32  getNumPRLs() const;
    UInt32  getPRL(UInt32 col_idx) const;
    UInt32  getNumSpanLengths() const;
    UInt32  getSpanLength(UInt32 row_idx) const;
    UInt32  getSpacing(UInt32 row_idx, UInt32 col_idx) const;
    const DirSpanLengthSpTbl::SpanLength* getSpanLengthRow(UInt32 row_idx) const;
    ExactSLSpacing* getExactSLSpacingList() const;
    DirSpanLengthSpTbl*  getNext() const;
    void  setIsWrongDir(bool v);
    void  setIsSameMask(bool v);
    void  setIsEdgeLength(bool v);
    void  setIsIncludeLShape(bool v);
    void  setJogLength(UInt32 len);
    void  setEOLWidth(UInt32 w);
    void  setNumPRLs(UInt32 num);
    void  setPRL(UInt32 prl, UInt32 index);
    void  setNumSpanLengths(UInt32 num);
    void  setExactSLSpacingList(ExactSLSpacing* l);
    void  setNext(DirSpanLengthSpTbl* n);

  private:
    Bits  wrong_dir_ : 1;
    Bits  same_mask_ : 1;
    Bits  edge_length_ : 1;
    Bits  include_lshape_ : 1;
    UInt32  jog_length_;
    UInt32  eol_width_;
    ExactSLSpacing* exact_sl_spacings_;
    UInt32 prls_num_;
    UInt32 span_length_rows_num_;
    UInt32* prls_;
    SpanLength* span_length_rows_;
    // might have 4 rules in the list
    // 1. without WRONGDIRECTION and SAMEMASK
    // 2. WRONGDIRECTION
    // 3. SAMEMASK
    // 4. WRONGDIRECTION and SAMEMASK
    DirSpanLengthSpTbl*  next_;
};

/**
 * @class EOLKeepout
 * @brief
 * the implementation class of EOLKEEPOUT syntax
 *
 * [ PROPERTY LEF58_EOLKEEPOUT
 *     "EOLKEEPOUT { eolWidth  |  minEolWidth maxEolWidth }
 *        EXTENSION  backwardExt   sideExt   forwardExt
 *        [EXCEPTWITHIN  lowSideExt   highSideExt ]
 *        [CLASS  className  [OTHERENDEOL]]
 *        [CORNERONLY
 *          [EXCEPTFROM { BACKEDGE [FORWARDGAP  forwardGap ]
 *            | FRONTEDGE }
 *          [MASK  maskNum  [TWOSIDES]
 *          | DIFFMASK]
 *          [EXCEPTSAMESIDEMETAL  backwardExt1 sideExt1 forwardExt1 ]
 *          [EXCEPTNOOPPSIDEMETAL  backwardExt2 sideExt2 forwardExt2 ]
 *          [EXCEPTLINEENDSPACING  spacing  | EXCEPTLINEENDGAP]
 *          [EOLWITHINCUT  withinCut ]
 *          [EXCEPTEOLSPACING  eolSpacing ]
 *          ]
 *        ]
 *        [EXCEPTSAMEMETAL] ; " ; ... ]
 */
class EOLKeepout {
    enum EOLKeepoutExcetpFrom {
        kEOLKeepoutFromBothEdge,
        kEOLKeepoutFromBackEdge,
        kEOLKeepoutFromFrontEdge
    };

  public:
    EOLKeepout() {
        memset(static_cast<void*>(this), 0, sizeof(EOLKeepout));
        if (class_names_.size() == 0) {
            class_names_.push_back("default_noclass");
        }
    }

    EOLKeepout* getNext() const;
    void    setNext(EOLKeepout* n);
    // EOLKEEPOUT
    UInt32  getMinEOLWidth() const;
    UInt32  getMaxEOLWidth() const;
    UInt32  getEOLWidth() const;
    void    setWidthRange(UInt32 min_eol_width, UInt32 max_eol_width);
    void    setEOLWidth(UInt32 eol_width);
    // EXTENSION
    UInt32  getBackwardExt() const;
    UInt32  getSideExt() const;
    UInt32  getForwardExt() const;
    void    setExtension(UInt32 backward, UInt32 side, UInt32 forward);
    // EXCEPTWITHIN
    UInt32  getExceptWithinLowExt() const;
    UInt32  getExceptWithinHighExt() const;
    void    setExceptWithinLowHighExt(UInt32 low, UInt32 high);
    // CLASS name
    UInt32  getClassId() const;
    const char* getClassName();
    void    setClassName(const char* name);
    bool    isOtherEndEOL() const;
    void    setOtherEndEOL();
    // CORNERONLY
    bool    isCornerOnly() const;
    void    setCornerOnly();
    // EXCEPTFROM
    bool    isFromBothEdge() const;
    bool    isFromBackEdge() const;
    bool    isFromFrontEdge() const;
    void    setFromBackEdge();
    void    setFromFrontEdge();
    void    setFromBothEdge();
    UInt32  getForwardGap() const;
    void    setForwardGap(UInt32 gap);
    // MASK
    UInt32  getMaskNum() const;
    void    setMaskNum(UInt32 num);
    bool    isTwoSides() const;
    void    setIsTwoSides(bool is_two_sides);
    bool    isDiffMask() const;
    void    setIsDiffMask(bool is_diff_mask);
    // EXCEPTSAMESIDEMETAL
    UInt32  getExceptSameSideMetalBackwardExt() const;
    UInt32  getExceptSameSideMetalSideExt() const;
    UInt32  getExceptSameSideMetalForwardExt() const;
    void    setExceptSameSideMetalExt(UInt32 backward, UInt32 side, UInt32 forward);
    // EXCEPTNOOPPSIDEMETAL
    UInt32  getExceptNoOppSideMetalBackwardExt() const;
    UInt32  getExceptNoOppSideMetalSideExt() const;
    UInt32  getExceptNoOppSideMetalForwardExt() const;
    void    setExceptNoOppSideMetalExt(UInt32 backward, UInt32 side, UInt32 forward);
    // EXCEPTLINEENDSPACING
    UInt32  getExceptLineEndSpacing() const;
    void    setExceptLineEndSpacing(UInt32 spacing);
    bool    isExceptLineEndGap() const;
    void    setIsExceptLineEndGap(bool isgap);
    // EOLWITHINCUT
    UInt32  getEOLWithinCut() const;
    void    setEOLWithinCut(UInt32 spacing);
    // EXCEPTEOLSPACING
    UInt32  getExceptEOLSpacing() const;
    void    setExceptEOLSpacing(UInt32 spacing);
    // EXCEPTSAMEMETAL
    bool    isExceptSameMetal() const;
    void    setIsExcetpSameMetal(bool b);

  private:
    EOLKeepout* next_;
    // EOLKEEPOUT { eolWidth  |  minEolWidth maxEolWidth }
    UInt32  min_eol_width_;
    UInt32  max_eol_width_;
    // EXTENSION  backwardExt   sideExt   forwardExt
    UInt32  backward_ext_;
    UInt32  side_ext_;
    UInt32  forward_ext_;
    // EXCEPTWITHIN  lowSideExt   highSideExt
    UInt32  low_side_ext_;
    UInt32  high_side_ext_;
    // FORWARDGAP
    UInt32  forward_gap_;
    // EXCEPTSAMESIDEMETAL  backwardExt1 sideExt1 forwardExt1
    UInt32  backward_ext1_;
    UInt32  side_ext1_;
    UInt32  forward_ext1_;
    // EXCEPTNOOPPSIDEMETAL  backwardExt2 sideExt2 forwardExt2
    UInt32  backward_ext2_;
    UInt32  side_ext2_;
    UInt32  forward_ext2_;
    // EXCEPTLINEENDSPACING  spacing  | EXCEPTLINEENDGAP
    UInt32  except_line_end_sp_;
    // EOLWITHINCUT  withinCut
    UInt32  eol_within_cut_sp_;
    // EXCEPTEOLSPACING  eolSpacing
    UInt32  except_eol_sp_;
    UInt32  class_id_;  // class name-related
    Bits    eol_width_range_ : 1;  // minEolWidth maxEolWidth instead of eolWidth
    Bits    except_within_ : 1;  // EXCEPTWITHIN
    Bits    other_end_eol_ : 1;  // OTHERENDEOL
    Bits    corner_only_ : 1;   // CORNERONLY is specifed
    Bits    has_except_from_ : 1;  // EXCEPTFROM is defined or not
    Bits    except_from_ : 2;  // EXCEPTFROM { BACKEDGE | FRONTEDGE }
    Bits    two_sides_   : 1;  // TWOSIDES
    Bits    diff_mask_   : 1;  // DIFFMASK
    Bits    mask_num_    : 3;  // MASK maskNum
    Bits    except_same_side_metal : 1;  // EXCEPTNOOPPSIDEMETAL
    Bits    except_no_opp_side_metal_ : 1;  // EXCEPTNOOPPSIDEMETAL
    Bits    has_except_line_end_sp_  :1;  // EXCEPTLINEENDSPACING
    Bits    except_line_end_gap_ : 1;  // EXCEPTLINEENDGAP
    Bits    eol_within_cut_ : 1;  // EOLWITHINCUT
    Bits    has_eol_sp_ : 1;  // EXCEPTEOLSPACING
    Bits    except_same_metal_ : 1;  // EXCEPTSAMEMETAL
    static std::vector<std::string> class_names_;
};

/**
 * @class MinCut
 * @brief
 * the implementation class of MINIMUMCUT syntax
 *
 * [ MINIMUMCUT   numCuts  WIDTH  width  [WITHIN  cutDistance ]
 *    [FROMABOVE | FROMBELOW]
 *    [LENGTH  length  WITHIN  distance ] ;] ...
 *
 * [ PROPERTY LEF58_MINIMUMCUT
 *    "MINIMUMCUT { numCuts  | {CUTCLASS  className   numCuts } ... }
 *        WIDTH  width  [WITHIN  cutDistance ]
 *        [FROMABOVE | FROMBELOW]
 *        [LENGTH  length  WITHIN  distance
 *        | AREA  area  [WITHIN  distance ]
 *        | SAMEMETALOVERLAP
 *        | FULLYENCLOSED]
 *        ; " ;]
 */
class MinCut {
  public:
    MinCut() {
        memset(static_cast<void*>(this), 0, sizeof(MinCut));
    }
    ~MinCut();

  public:
    UInt32  getNumCuts() const;
    UInt32  getWidth() const;
    UInt32  getCutWithin() const;
    std::vector<CutClass*>*  getCutClassArr();
    std::vector<UInt32>*  getCutClassNumCutsArr();
    UInt32  getLength() const;
    UInt32  getLengthWithin() const;
    double  getArea() const;
    UInt32  getAreaWithin() const;
    bool    isFromAbove() const;
    bool    isFromBelow() const;
    bool    isSameMetalOverlap() const;
    bool    isFullyEnclosed() const;
    MinCut*  getNext() const;

    void    setNumCuts(UInt32 num);
    void    setWidth(UInt32 w);
    void    setCutWithin(UInt32 dist);
    void    appendCutClassCuts(CutClass* cs, UInt32 num);
    void    setLength(UInt32 len);
    void    setLengthWithin(UInt32 dist);
    void    setArea(double area);
    void    setAreaWithin(UInt32 dist);
    void    setIsFromAbove(bool v);
    void    setIsFromBelow(bool v);
    void    setIsSameMetalOverlap(bool v);
    void    setIsFullyEnclosed(bool v);
    void    setNext(MinCut* n);

  private:
    std::vector<CutClass*>  *cut_classes_;
    std::vector<UInt32>  *cc_num_cuts_;
    UInt32  num_cuts_;
    UInt32  width_;
    UInt32  cut_within_;
    UInt32  area_length_within_;
    union {
        double  area_;
        UInt32  length_;
    };
    MinCut*  next_;

    Bits  from_above_ : 1;
    Bits  from_below_ : 1;
    Bits  is_length_within_ : 1;
    Bits  is_area_within_ : 1;
    Bits  same_metal_overlap_ : 1;
    Bits  fully_enclosed_ : 1;
};

/**
 * @class MINENCLOSEDAREA
 * @brief
 * the implementation class of MINENCLOSEDAREA syntax
 * [ MINENCLOSEDAREA   area  [WIDTH  width ] ;] ...
 */
class MinEnclArea {
  public:
    MinEnclArea() {
        memset(static_cast<void*>(this), 0, sizeof(MinEnclArea));
    }

    UInt32  getArea() const;
    UInt32 getWidth() const;
    MinEnclArea* getNext() const;
    void  setArea(UInt32 area);
    void  setWidth(UInt32 w);
    void  setNext(MinEnclArea* next);

  private:
    UInt32  area_;
    UInt32 width_;
    MinEnclArea* next_;
};

/**
 * @class MinSize
 * @brief
 * the implementation class of MINSIZE syntax
 *
 * [ MINSIZE   minWidth   minLength  [ minWidth2   minLength2 ] ... ;]
 * [ PROPERTY LEF58_MINSIZE
 *    "MINSIZE [RECTONLY]  minWidth   minLength  [ minWidth   minLength ]...
 *    ;..." ;]
 */
class MinSize {
  public:
    MinSize() {
        memset(static_cast<void*>(this), 0, sizeof(MinSize));
    }
    ~MinSize();

  public:
    UInt32  getMinSizeNum() const;
    UInt32  getWidth(UInt32 idx) const;
    UInt32  getLength(UInt32 idx) const;
    bool    isRectOnly() const;
    void    setRectOnly(bool v);
    void    setMinSizeNum(UInt32 num);
    void    addWidthLength(UInt32 index, UInt32 width, UInt32 length);
    MinSize* getNext() const;  // to use template function to release rule

  private:
    bool    is_rect_only_;
    UInt32  minsize_num_;
    UInt32* widths_;
    UInt32* lengths_;
};

/**
 * @brief
 * the implementation class of MINSTEP syntax
 * MINSTEP   minStepLength
 *    [ [INSIDECORNER | OUTSIDECORNER | STEP] [LENGTHSUM  maxLength ]
 *    | [MAXEDGES  maxEdges ] ;]
 *
 * [ PROPERTY LEF58_MINSTEP
 *    "MINSTEP  minStepLength
 *        [[INSIDECORNER | OUTSIDECORNER | STEP] [LENGTHSUM  maxLength ]]
 *        |[MAXEDGES  maxEdges
 *           [ MINADJACENTLENGTH  minAdjLength
 *             [CONVEXCORNER [EXCEPTWITHIN  exceptWithin ]
 *             |CONCAVECORNER
 *             |THREECONCAVECORNERS [CENTERWIDTH  width ]
 *             | minAdjLength2 ]
 *           | MINBETWEENLENGTH  minBetweenLength [EXCEPTSAMECORNERS]
 *           | NOADJACENTEOL  eolWidth
 *             [EXCEPTADJACENTLENGTH  minAdjLength | MINADJACENTLENGTH  minAdjLength]
 *             [CONCAVECORNERS]
 *           | NOBETWEENEOL  eolWidth ]
 *         ] ;..." ;]
 *
 */
class MinStep {
  public:
    MinStep() {
        memset(static_cast<void*>(this), 0, sizeof(MinStep));
    }

  public:
    bool    isMaxEdges() const;
    bool    isInsideCorner() const;
    bool    isOutsideCorner() const;
    bool    isStep() const;
    bool    isMinAdjLength() const;
    bool    isMinAdjLengthConvexCorner() const;
    bool    isMinAdjLengthConcaveCorner() const;
    bool    isMinAdjLengthThreeConcaveCorners() const;
    bool    isMinAdjLength2() const;
    bool    isMinBetweenLength() const;
    bool    isExceptSameCorners() const;
    bool    isNoAdjEOL() const;
    bool    isNoAdjEOLExceptAdjLength() const;
    bool    isNoAdjEOLMinAdjLength() const;
    bool    isNoAdjEOLConcaveCorners() const;
    bool    isNoBetweenEOL() const;
    UInt32  getMinStepLength() const;
    UInt32  getMaxLength() const;
    UInt32  getMaxEdges() const;
    UInt32  getMinAdjLength() const;
    UInt32  getConvexExceptWithin() const;
    UInt32  getCenterWidth() const;
    UInt32  getMinAdjLength2() const;
    UInt32  getMinBetweenLength() const;
    UInt32  getNoAdjEOLWidth() const;
    UInt32  getNoAdjExceptAdjLength() const;
    UInt32  getNoAdjEOLMinAdjLength() const;
    UInt32  getNoBetweenEOLWidth() const;
    MinStep*  getNext() const;
    void    setIsInsideCorner(bool v);
    void    setIsOutsideCorner(bool v);
    void    setIsStep(bool v);
    void    setMinStepLength(UInt32 len);
    void    setMaxLength(UInt32 sum);
    void    setMaxEdges(UInt32 num);
    void    setMinAdjLength(UInt32 len);
    void    setIsMinAdjLengthConvexCorner(bool v);
    void    setConvexExcetpWithin(UInt32 len);
    void    setIsMinAdjLengthConcaveCorner(bool v);
    void    setIsMinAdjLengthThreeConcaveCorners(bool v);
    void    setCenterWidth(UInt32 w);
    void    setMinAdjLength2(UInt32 len);
    void    setMinBetweenLength(UInt32 len);
    void    setIsExceptSameCorners(bool v);
    void    setNoAdjEOLWidth(UInt32 w);
    void    setNoAdjExceptAdjLength(UInt32 len);
    void    setNoAdjEOLMinAdjLength(UInt32 len);
    void    setIsNoAdjEOLConcaveCorners(bool v);
    void    setNoBetweenEOLWidth(UInt32 w);
    void    setNext(MinStep* ms);

  private:
    UInt32  min_step_length_;  //  MINSTEP minStepLength
    UInt32  max_edges_;        //  MAXEDGES  maxEdges
    union {
        UInt32  max_length_;          // LENGTHSUM maxLength
        UInt32  min_adj_length_;      // MINADJACENTLENGTH minAdjLength
        UInt32  min_between_length_;  // MINBETWEENLENGTH minBetweenLength
        UInt32  eol_width_;           // NOADJACENTEOL eolWidth | NOBETWEENEOL eolWidth
    };
    union {
        UInt32  except_within_;                  // EXCEPTWITHIN exceptWithin
        UInt32  center_width_;                   // CENTERWIDTH width
        UInt32  min_adj_length2_;                // minAdjLength2
        UInt32  no_adj_eol_except_adj_length_;   // EXCEPTADJACENTLENGTH minAdjLength
        UInt32  no_adj_eol_min_adj_length_;      // MINADJACENTLENGTH minAdjLength
    };
    Bits  is_max_edges_              : 1;
    Bits  inside_corner_             : 1;
    Bits  outside_corner_            : 1;
    Bits  step_                      : 1;
    Bits  is_min_adj_length_         : 1;
    Bits  is_adj_convex_corner_      : 1;
    Bits  is_adj_concave_corner_     : 1;
    Bits  is_three_concave_corners_  : 1;
    Bits  is_min_adj_length2_        : 1;
    Bits  is_min_between_length_     : 1;
    Bits  is_except_same_corners_    : 1;
    Bits  is_excetp_adj_length_      : 1;
    Bits  is_no_adj_eol_             : 1;
    Bits  is_no_adj_eol_except_adj_length_ : 1;
    Bits  is_no_adj_eol_min_adj_length_ : 1;
    Bits  is_no_adj_concave_corners_ : 1;
    Bits  is_no_between_eol_         : 1;
    MinStep*  next_;
};

/**
 * @brief
 * for spacing table rules, implement them separately.
 *
 * [ SPACINGTABLE
 *    [PARALLELRUNLENGTH { length } ...
 *        {WIDTH  width  { spacing } ...} ... ;
 *    | INFLUENCE {WIDTH  width  WITHIN  distance  SPACING  spacing } ... ;
 *    | TWOWIDTHS {WIDTH  width  [PRL  runLength ] { spacing } ...} ... ; ]
 * ;" ;]
 *
 * [ PROPERTY LEF58_SPACINGTABLE
 *    "SPACINGTABLE
 *        [PARALLELRUNLENGTH [WRONGDIRECTION] [SAMEMASK]
 *          [EXCEPTEOL  eolWidth ] { length } ...
 *          {WIDTH  width
 *            [EXCEPTWITHIN  lowExcludeSpacing   highExcludeSpacing ] { spacing }...} ... ;
 *        |INFLUENCE {WIDTH  width  WITHIN  distance  SPACING  spacing } ... ;
 *        |TWOWIDTHS {WIDTH  width  [PRL  runLength ] { spacing } ...} ... ;
 *        |PARALLELSPANLENGTH PRL  runLength
 *          {SPANLENGTH  spanLength  { spacing } ...
 *        ]
 * ;" ;]
 */

/**
 * @class WidthSpTbl
 * @brief
 * the implementation class of SPACINGTABLE PARALLELRUNLENGTH WIDTH and TWOWIDTHS
 *
 * [ SPACINGTABLE
 *    [PARALLELRUNLENGTH { length } ...
 *        {WIDTH  width  { spacing } ...} ... ;
 *    | TWOWIDTHS {WIDTH  width  [PRL  runLength ] { spacing } ...} ... ; ]
 * ;" ;]
 *
 * [ PROPERTY LEF58_SPACINGTABLE
 *    "SPACINGTABLE TWOWIDTHS [WRONGDIRECTION] [SAMEMASK | MASK  maskNum ]
 *    [EXCEPTEOL  eolWidth ]
 *    [FILLCONCAVECORNER  fillTriangle ]
 *    {WIDTH  width  [PRL  runLength ]{ spacing } ...} ...  ]; " ;
 *
 * [ PROPERTY LEF58_SPACINGTABLE
 *    "SPACINGTABLE
 *        [PARALLELRUNLENGTH [WRONGDIRECTION] [SAMEMASK]
 *          [EXCEPTEOL  eolWidth ] { length } ...
 *          {WIDTH  width
 *            [EXCEPTWITHIN  lowExcludeSpacing   highExcludeSpacing ] { spacing }...} ... ;
 *        |TWOWIDTHS {WIDTH  width  [PRL  runLength ] { spacing } ...} ... ; ]
 * ;" ;]
 *
 */
class WidthSpTbl {
  public:
    WidthSpTbl() {
        memset(static_cast<void*>(this), 0, sizeof(WidthSpTbl));
    }
    ~WidthSpTbl();

  public:
    bool  isTwoWidths() const;
    bool  isPRLWidth() const;
    bool  isWrongDir() const;
    bool  isSameMask() const;
    UInt32  getMaskNum() const;
    UInt32  getFillConcaveCornerLength() const;
    UInt32  getEOLWidth() const;
    UInt32  getWidthDim() const;
    UInt32  getPRLDim() const;  // PARALLELRUNLENGTH runLength & TWOWIDTHS PRL runLength
    bool    hasWidthPRL(UInt32 idx) const;  // TWOWIDTHS PRL runLength
    UInt32  getWidth(UInt32 idx) const;
    void  getLoHiExclSpacing(UInt32 row_idx, UInt32* lo_spacing, UInt32* hi_spacing) const;
    UInt32  getPRL(UInt32 idx) const;
    UInt32  getSpacing(UInt32 row_idx, UInt32 col_idx) const;
    UInt32  getMinSpacing() const;
    UInt32  getMaxSpacing() const;
    WidthSpTbl* getNext() const;

    void  setIsWidthSpTbl(bool v);
    void  setIsPRLWidth(bool v);
    void  setIsWrongDir(bool v);
    void  setIsSameMask(bool v);
    void  setMaskNum(UInt32 num);
    void  setFillConcaveCornerLength(UInt32 fill_triangle);
    void  setEOLWidth(UInt32 w);
    void  setWidthDim(UInt32 w_dim);
    void  setPRLDim(UInt32 prl_dim);
    void  setPRL(UInt32 row_col_idx, UInt32 prl);
    void  setWidth(UInt32 row_idx, UInt32 w);
    void  setLoHiExclSpacing(UInt32 row_idx, UInt32 lo_excl_sp, UInt32 hi_excl_sp);
    void  setSpacing(UInt32 row_idx, UInt32 col_idx, UInt32 sp);
    void  setMinSpacing(UInt32 min_sp);
    void  setMaxSpacing(UInt32 max_sp);
    void  setNext(WidthSpTbl* n);

  private:
    Bits  is_two_widths_ : 1;
    Bits  is_PRL_width_  : 1;
    Bits  is_wrong_dir_  : 1;
    Bits  is_same_mask_  : 1;
    Bits  mask_num_   : 3;   // TWOWIDTHS MASK maskNum

    UInt32  min_spacing_;
    UInt32  max_spacing_;
    UInt32  eol_width_; // EXCEPTEOL eolWidth
    UInt32  fill_triangle_;  // FILLCONCAVECORNER fillTriangle
    UInt32  prl_num_;
    UInt32  width_num_;
    UInt32  spacing_num_;
    UInt32* prls_;
    UInt32* widths_;
    UInt32* low_excl_spacing_;
    UInt32* high_excl_spacing_;
    UInt32* spacing_elems_;

    WidthSpTbl* next_;
};

/**
 * @class InfluenceSpTbl
 * @brief
 * the implementation class of SPACINGTABLE INFLUENCE
 *
 * [ SPACINGTABLE
 *    INFLUENCE {WIDTH  width  WITHIN  distance  SPACING  spacing } ... ;
 * ;" ;]
 *
 * [ PROPERTY LEF58_SPACINGTABLE
 *    "SPACINGTABLE
 *        INFLUENCE {WIDTH  width  WITHIN  distance  SPACING  spacing } ... ;
 * ;" ;]
 *
 */
class InfluenceSpTbl {
  public:
    InfluenceSpTbl() {
        memset(static_cast<void*>(this), 0, sizeof(InfluenceSpTbl));
    }
    ~InfluenceSpTbl();

  public:
    UInt32 getRowNum() const;
    void   setRowNum(UInt32 num);
    bool   getRowElems(UInt32 row_idx, UInt32& w, UInt32& d, UInt32& sp) const;
    UInt32 getWidth(UInt32 row_idx) const;
    UInt32 getWithin(UInt32 row_idx) const;
    UInt32 getSpacing(UInt32 row_idx) const;
    void   setRowElems(UInt32 row_idx, UInt32 w, UInt32 d, UInt32 sp);
    InfluenceSpTbl* getNext() const;  // to use template function to release rule

  private:
    UInt32   row_num_;
    UInt32*  widths_;
    UInt32*  withins_;
    UInt32*  spacings_;
};

/**
 * @class PRLSpacingTbl
 * @brief
 * the implementation class of SPACINGTABLE
 *
 * [ PROPERTY LEF58_SPACINGTABLE
 *    "SPACINGTABLE
 *        PARALLELSPANLENGTH PRL  runLength
 *          {SPANLENGTH  spanLength  { spacing } ...
 * ;" ;]
 *
 */
class ParaSpanLenTbl {
  public:
    ParaSpanLenTbl() {
        memset(static_cast<void*>(this), 0, sizeof(ParaSpanLenTbl));
    }
    ~ParaSpanLenTbl();

  public:
    UInt32  getDim() const;
    UInt32  getPRL() const;
    UInt32  getSpanLength(UInt32 row_idx) const;
    UInt32  getSpacing(UInt32 row_idx, UInt32 col_idx) const;
    ParaSpanLenTbl* getNext() const;  // to use template function to release rule

    void    setDim(UInt32 dim);
    void    setPRL(UInt32 prl);
    void    setSpanLength(UInt32 row_idx, UInt32 span_length);
    void    setSpacing(UInt32 row_idx, UInt32 col_idx, UInt32 spacing);

  private:
    void    release(UInt32*& ptr);
  private:
    UInt32  dim_;
    UInt32  prl_;
    UInt32* span_lengths_;
    UInt32* spacings_;  // dim_ x dim_ two-dimensional table
};



/**
 * @class ProtrusionRule
 * @brief
 * the implementation class of PROTRUSIONWIDTH syntax
 *
 * [ PROTRUSIONWIDTH   width1  LENGTH  length  WIDTH  width2  ;]
 *
 * [ PROPERTY LEF58_PROTRUSIONWIDTH
 *    "PROTRUSIONWIDTH  width1
 *      {LENGTH  length  WIDTH  width2
 *      | {WIDTH  width2
 *          {MINSIZE { {minWidth minLength} | {minLength CUTCLASS className {FROMABOVE | FROMBELOW}} }
 *          | MINLENGTH  minLength [EXCEPTCUT  cutDistance [FROMABOVE | FROMBELOW]]}...
 *        } ... }
 *    ; " ... ;]
 */
class ProtrusionRule {
  public:
    ProtrusionRule() {
        memset(static_cast<void*>(this), 0, sizeof(ProtrusionRule));
        protrusion_width_ = new std::vector<ProtrusionWidth*>;
    }
    ~ProtrusionRule() {
        for (auto& pw : *protrusion_width_) {
            delete pw;
        }
        delete protrusion_width_;
    }

    class ProtrusionWidth {
      public:
        ProtrusionWidth() {
            memset(static_cast<void*>(this), 0, sizeof(ProtrusionWidth));
        }

      public:
        UInt32  getWidth() const;
        UInt32  getMinWidth() const;
        UInt32  getMinLength() const;
        UInt32  getCutDistance() const;
        CutClass* getCutClass() const;
        bool    isMinSize() const;
        bool    isMinSizeMinWidth() const;
        bool    isFromAbove() const;
        bool    isFromBelow() const;
        bool    isFromBoth() const;

        void    setWidth(UInt32 w);
        void    setMinSizeWidthLength(UInt32 w, UInt32 len);  // MINSIZE minWidth minLength
        void    setMinSizeMinLength(UInt32 len);  // MINSIZE minLength
        void    setMinLength(UInt32 len);  // MINSIZE minLength
        void    setCutDistance(UInt32 d);
        void    setCutClass(CutClass* cs);
        void    setIsFromAbove(bool v);
        void    setIsFromBelow(bool v);

      private:
        UInt32  width_;  // WIDTH width2
        union {
            UInt32  min_size_min_width_;  // MINSIZE minWidth and MINSIZE minLength CUTCLASS share this variable
            UInt32  min_length_min_length_;  // MINLENGTH minLength
        };
        union {
            UInt32  min_size_min_length_;  // MINSIZE x minLength
            UInt32  min_length_cut_dist_;  // EXCEPTCUT cutDistance
        };

        Bits  is_min_size_ : 1;  // MINSIZE
        Bits  is_min_size_min_length_ : 1;  // MINSIZE  minLength CUTCLASS
        Bits  is_min_length_ : 1;  // MINLENGTH
        Bits  from_above_ : 1;
        Bits  from_below_ : 1;
        Bits  has_except_cut_ : 1;
        CutClass*  cut_class_;
    };

  public:
    UInt32  getWidth1() const;
    UInt32  getLength() const;
    UInt32  getWidth2() const;
    bool    isLength() const;  // LENGTH length WIDTH width2
    bool    isWidth() const; // LEF58: WIDTH width2
    std::vector<ProtrusionRule::ProtrusionWidth*>* getProtrusionWidth();
    void    addProtrusionWidth(ProtrusionRule::ProtrusionWidth* pw);
    ProtrusionRule* getNext() const;

    void    setWidth1(UInt32 w);
    void    setLength(UInt32 len);
    void    setWidth2(UInt32 w);
    void    setIsLength(bool b);
    void    setNext(ProtrusionRule* n);

  private:
    UInt32  width1_;
    UInt32  length_;
    UInt32  width2_;
    Bits    is_length_ : 1;
    std::vector<ProtrusionWidth*>  *protrusion_width_;
    ProtrusionRule* next_;
};

/**
 * @class RoutingSpacing
 * @brief
 * the implementation class of SPACING syntax
 *
 * [ SPACING   minSpacing
 *    [ RANGE  minWidth maxWidth
 *        [ USELENGTHTHRESHOLD
 *        | INFLUENCE  value  [RANGE  stubMinWidth  stubMaxWidth ]
 *        | RANGE  minWidth  maxWidth ]
 *    | LENGTHTHRESHOLD  maxLength [RANGE  minWidth  maxWidth]
 *    | ENDOFLINE  eolWidth  WITHIN  eolWithin  [PARALLELEDGE  parSpace  WITHIN  parWithin  [TWOEDGES]]
 *    | SAMENET [PGONLY]
 *    | NOTCHLENGTH  minNotchLength
 *    | ENDOFNOTCHWIDTH  endOfNotchWidth  NOTCHSPACING  minNotchSpacing  NOTCHLENGTH  minNotchLength ] ;] ...
 *
 * [ PROPERTY LEF58_SPACING
 *    "SPACING eolSpace EOLPERPENDICULAR eolWidth perWidth  ;" ;]
 *
 * [ PROPERTY LEF58_SPACING
 *    "SPACING  minSpacing  AREA  maxArea  ;" ;]
 *
 * [ PROPERTY LEF58_SPACING
 *    "SPACING  minSpacing  LAYER  trimLayer  [TRIMMASK  trimMaskNum ]
 *        [MASK  maskNum ] EXCEPTOVERLAP  overlapLength
 *        [TRIMENDSPACING  trimEndSpacing ] ;" ;]
 *
 * [ PROPERTY LEF58_SPACING
 *    "SPACING  minSpacing  SAMEMASK ;" ;]
 *
 * [ PROPERTY LEF58_SPACING
 *    "SPACING  minSpacing  [WRONGDIRECTION [NONEOL  eolWidth ]
 *        [PRL  prl ] [LENGTH  length ]] ;" ;]
 *
 * [ PROPERTY LEF58_SPACING
 *    "SPACING  minSpacing  NOTCHLENGTH  minNotchLength
 *        [EXCEPTWITHIN  lowExcludeSpacing highExcludeSpacing ]
 *        [WITHIN  within  SPANLENGTH  sideOfNotchSpanLength
 *        |{WIDTH | CONCAVEENDS}  sideOfNotchWidth
 *        |NOTCHWIDTH  notchwidth ] ;" ;]
 *
 * [ PROPERTY LEF58_SPACING
 *    "SPACING  minSpacing  NOTCHSPAN  span  NOTCHSPACING  notchSpacing
 *        EXCEPTNOTCHLENGTH  notchLength ; " ;
 *
 * [ PROPERTY LEF58_SPACING
 *    "SPACING  eolSpace
 *        ENDOFLINE  eolWidth
 *        [EXACTWIDTH] [WRONGDIRSPACING  wrongDirSpace ]
 *        {[OPPOSITEWIDTH  oppositeWidth ] WITHIN  eolWithin  [ wrongDirWithin ]
 *        [SAMEMASK]
 *        [EXCEPTEXACTWIDTH  exactWidth   otherWidth ]
 *        [FILLCONCAVECORNER  fillTriangle ]
 *        [WITHCUT [CUTCLASS  cutClass ] [ABOVE]  withCutSpace
 *           [ENCLOSUREEND  enclosureEndWidth
 *              [WITHIN  enclosureEndWithin ]]]
 *        [ENDPRLSPACING  endPrlSpace  PRL  endPrl ]
 *        [ENDTOEND  endToEndSpace [oneCutSpace twoCutSpace]
 *            [EXTENSION extension [wrongDirExtension]]
 *            [OTHERENDWIDTH otherEndWidth] ]
 *        [MAXLENGTH  maxLength  | MINLENGTH  minLength [TWOSIDES] ]
 *        [EQUALRECTWIDTH]
 *        [PARALLELEDGE [ SUBTRACTEOLWIDTH] parSpace
 *            WITHIN  parWithin  [PRL  prl ]
 *            [MINLENGTH  minLength ] [TWOEDGES]
 *            [SAMEMETAL] [NONEOLCORNERONLY]
 *            [PARALLELSAMEMASK]]
 *        [ENCLOSECUT [BELOW | ABOVE]  encloseDist  CUTSPACING  cutToMetalSpace  [ALLCUTS]]
 *        | TOCONCAVECORNER [MINLENGTH  minLength ] [MINADJACENTLENGTH { minAdjLength  |  minAdjLength1   minAdjLength2 }]
 *        | TONOTCHLENGTH  notchLength } ;..." ;]
 *
 * [ PROPERTY LEF58_SPACING
 *    "SPACING  spacing  CONVEXCORNERS EXTENSION  sideExt   orthogonalExt
 *        [ SAMESIDE  extension
 *        | SINGLE  edgeForwardExt   cornerForwardExt   cornerBackwardExt
 *            SPANLENGTH  spanLength  OPPOSITEWIDTH  oppWidth
 *            OPPOSITEEXTENSION  oppSideExt  oppForwardExt1   oppForwardExt2  ] ; " ;]
 */
class RoutingSpacing {
  public:
    RoutingSpacing() {
        memset(static_cast<void*>(this), 0, sizeof(RoutingSpacing));
    }

    enum SpacingType {
        kRange            = 1,
        kLengthThres      = 2,
        kEndOfLine        = 3,
        kSameNet          = 4,
        kNotchLength      = 5,
        kEndOfNotchWidth  = 6,
        kEOLPerp          = 7,
        kMaxArea          = 8,
        kTrimLayer        = 9,
        kSameMask         = 10,
        kWrongDir         = 11,
        kNotchSpan        = 12,
        kConvexCorners    = 13,
        kUnknownSpacingType
    };

    struct Range {
        UInt32  min_width_;
        UInt32  max_width_;
        union {
            UInt32 stub_min_width_;
            UInt32 range_min_width_;
        };
        union {
            UInt32 stub_max_width_;
            UInt32 range_max_width_;
        };
        UInt32  influence_value_;
        Bits    is_use_len_thres_ : 1;
        Bits    is_influence_     : 1;
        Bits    is_range_         : 1;
        Bits    is_influence_range_ : 1;
    };

    struct LengthThreshold {
        UInt32  max_length_;
        UInt32  min_width_;
        UInt32  max_width_;
    };

    struct EndOfLine {
        UInt32 eol_width_;
        UInt32 eol_within_;
        UInt32 prl_edge_space_;
        UInt32 prl_edge_within_;
        Bits   two_edges_ : 1;
    };

    struct NotchLength {
        UInt32 min_notch_len_;
        UInt32 low_excl_spacing_;
        UInt32 high_excl_spacing_;
        UInt32 within_;
        union {
            UInt32 span_length_;
            UInt32 side_of_notch_width_;
            UInt32 notch_width_;
        };
        Bits is_within_ : 1;
        Bits is_width_ : 1;
        Bits is_concave_ends_ : 1;
        Bits is_notch_width_ : 1;
    };

    struct EndOfNotchWidth {
        UInt32 eon_width_;
        UInt32 notch_spacing_;
        UInt32 min_notch_length_;
    };

    struct EOLPerp {
        UInt32 eol_width_;
        UInt32 per_width_;
    };

    struct TrimLayer {
    };

    struct WrongDir {
        UInt32 eol_width_;
        UInt32 prl_;
        UInt32 length_;
    };

    struct NotchSpan {
        UInt32 span_;
        UInt32 spacing_;
        UInt32 length_;
    };

    struct ConvexCorners {
    };

  public:
    UInt32 getMinSpacing() const;
    void   setMinSpacing(UInt32 sp);

    /// SPACING minSpacing Range
    bool   isRange() const;
    bool   isRangeUseLengthThres() const;
    bool   isRangeInfluence() const;
    bool   isRangeInfluenceRange() const;
    bool   isRangeRange() const;
    UInt32 getRangeMinWidth() const;
    UInt32 getRangeMaxWidth() const;
    UInt32 getRangeInfluenceValue() const;
    UInt32 getRangeStubMinWidth() const;
    UInt32 getRangeStubMaxWidth() const;
    UInt32 getRangeRangeMinWidth() const;
    UInt32 getRangeRangeMaxWidth() const;
    void   setIsRange(bool v);
    void   setIsRangeUseLenghthThres(bool v);
    void   setIsRangeInfluence(bool v);
    void   setIsRangeInfluenceRange(bool v);
    void   setIsRangeRange(bool v);
    void   setRangeMinWidth(UInt32 min_width);
    void   setRangeMaxWidth(UInt32 max_width);
    void   setRangeMinMaxWidth(UInt32 min_w, UInt32 max_w);
    void   setRangeInfluenceValue(UInt32 v);
    void   setRangeInfluenceMinWidth(UInt32 min_width);
    void   setRangeInfluenceMaxWidth(UInt32 max_width);
    void   setRangeRangeMinWidth(UInt32 min_width);
    void   setRangeRangeMaxWidth(UInt32 max_width);
    void   setRangeRangeMinMaxWidth(UInt32 min_w, UInt32 max_w);

    /// SPACING minSpacing LENGTHTHRESHOLD
    bool    isLengthThreshold() const;
    bool    isLengthThresholdRange() const;
    void    setIsLengthThreshold(bool v);
    UInt32  getLenThresMaxLength() const;
    UInt32  getLenThresMinWidth() const;
    UInt32  getLenThresMaxWidth() const;
    void    setLenThresMaxLength(UInt32 max_len);
    void    setLenThresMinWidth(UInt32 min_width);
    void    setLenThresMaxWidth(UInt32 max_width);
    void    setLenThresMinMaxWidth(UInt32 min_w, UInt32 max_w);

    /// SPACING minSpacing ENDOFLINE
    // TODO: LEF58
    bool   isEndOfLine() const;
    void   setIsEndOfLine(bool v);
    UInt32 getEOLWidth() const;
    UInt32 getEOLWithin() const;
    UInt32 getEOLPRLEdgeSpace() const;
    UInt32 getEOLPRLEdgeWithin() const;
    bool   isEOLTwoEdges() const;
    void   setEOLWidth(UInt32 w);
    void   setEOLWithin(UInt32 dist);
    void   setEOLPRLEdgeSpace(UInt32 sp);
    void   setEOLPRLEdgeWithin(UInt32 prl_dist);
    void   setIsEOLTwoEdges(bool v);

    /// SPACING minSpacing SAMENET
    bool   isSameNet() const;
    bool   isSameNetPGOnly() const;
    void   setIsSameNet(bool v);
    void   setIsSameNetPGOnly(bool v);

    /// SPACING minSpacing NOTCHLENGTH
    bool   isNotchLength() const;
    void   setIsNotchLength(bool v);
    UInt32 getNotchLengthMinNotchLength() const;
    UInt32 getNotchLengthLowExclSpacing() const;
    UInt32 getNotchLengthHighExclSpacing() const;
    UInt32 getNotchLengthWithin() const;
    UInt32 getNotchLengthSpanLength() const;
    UInt32 getNotchLengthSideNotchWidth() const;
    UInt32 getNotchLengthNotchWidth() const;
    bool   isNotchLengthWithin() const;
    bool   isNotchLengthWidth() const;
    bool   isNotchLengthConcaveEnds() const;
    bool   isNotchLengthNotchWidth() const;
    void   setNotchLengthMinNotchLength(UInt32 len);
    void   setNotchLengthLowExclSpacing(UInt32 sp);
    void   setNotchLengthHighExclSpacing(UInt32 sp);
    void   setNotchLengthLowHighExclSpacing(UInt32 lo, UInt32 hi);
    void   setNotchLengthWithin(UInt32 within);
    void   setNotchLengthSpanLength(UInt32 span_len);
    void   setNotchLengthSideNotchWidth(UInt32 w);
    void   setNotchLengthNotchWidth(UInt32 w);
    void   setIsNotchLengthWithin(bool v);
    void   setIsNotchLengthWidth(bool v);
    void   setIsNotchLengthConcaveEnds(bool v);
    void   setIsNotchLengthNotchWidth(bool v);
    bool   isLef58NotchLength() const;

    /// SPACING minSpacing ENDOFNOTCHWIDTH
    bool   isEndOfNotchWidth() const;
    void   setIsEndOfNotchWidth(bool v);
    UInt32 getEONWidth() const;
    UInt32 getEONSpacing() const;
    UInt32 getEONLength() const;
    void   setEONWidth(UInt32 w);
    void   setEONSpacing(UInt32 sp);
    void   setEONLength(UInt32 len);

    /// SPACING eolSpace EOLPERPENDICULAR
    bool   isEOLPerp() const;
    void   setIsEOLPerp(bool v);
    UInt32 getEOLPerpEolWidth() const;
    UInt32 getEOLPerpPerpWidth() const;
    void   setEOLPerpEolWidth(UInt32 w);
    void   setEOLPerpPerpWidth(UInt32 w);

    /// SPACING minSpacing  AREA
    bool   isArea() const;
    void   setIsArea(bool v);
    double getArea() const;
    void   setArea(double area);

    /// SPACING minSpacing  LAYER trimLayer
    // TODO:
    bool   isTrimLayerSpacing() const;
    void   setIsTrimLayerSpacing(bool v);

    /// SPACING minSpacing SAMEMASK
    bool   isSameMask() const;
    void   setIsSameMask(bool v);

    /// SPACING minSpacing WRONGDIRSPACING
    bool   isWrongDir() const;
    void   setIsWrongDir(bool v);
    UInt32 getWrongDirEolWidth() const;
    UInt32 getWrongDirPRL() const;
    UInt32 getWrongDirLength() const;
    void   setWrongDirEolWidth(UInt32 w);
    void   setWrongDirPRL(UInt32 prl);
    void   setWrongDirLength(UInt32 len);

    /// SPACING minSpacing NOTCHSPAN
    bool   isNotchSpan() const;
    void   setIsNotchSpan(bool v);
    UInt32 getNotchSpanSpan() const;
    UInt32 getNotchSpanSpacing() const;
    UInt32 getNotchSpanLength() const;
    void   setNotchSpanSpan(UInt32 span);
    void   setNotchSpanSpacing(UInt32 sp);
    void   setNotchSpanLength(UInt32 len);

    /// SPACING minSpacing CONVEXCORNERS
    // TODO:
    bool   isConvexCorners() const;
    void   setIsConvexCorners(bool v);

    RoutingSpacing* getNext() const;
    void   setNext(RoutingSpacing* n);
  private:
    bool   isType_(SpacingType type) const;
    void   setIsType_(SpacingType type, bool v);

  private:
    union {
        Range range_;
        LengthThreshold len_thres_;
        EndOfLine  eol_;
        NotchLength notch_length_;
        EndOfNotchWidth eon_width_;
        EOLPerp  eol_perp_;
        double area_;
        TrimLayer trim_layer_;
        WrongDir wrong_dir_;
        NotchSpan notch_span_;
        ConvexCorners convex_corners_;
    } sp_rule_;

    UInt32  type_;
    UInt32  min_spacing_;
    RoutingSpacing* next_;

    Bits    is_same_net_pg_only_ : 1;
    Bits    is_lef58_notch_length_ : 1;
};

/**
 * @class RoutingLayerRule
 * @brief
 * implementation of routing layer rules
 */
class RoutingLayerRule {
  public:
    RoutingLayerRule() {
        memset(static_cast<void*>(this), 0, sizeof(RoutingLayerRule));
    }

    ~RoutingLayerRule();

  public:
    bool isVert() const;
    bool isHoriz() const;
    bool isDiag45() const;
    bool isDiag135() const;
    bool isRectOnly() const;
    bool isExceptNonCorePins() const;
    bool isXYPitch() const;
    bool isXYOffset() const;
    bool isXYDiagPitch() const;

    UInt32  getMinWidth() const;
    UInt32  getMaxWidth() const;
    UInt32  getWrongDirWidth() const;
    UInt32  getPitch() const;
    UInt32  getPitchX() const;
    UInt32  getPitchY() const;
    UInt32  getFirstLastPitch() const;
    UInt32  getOffset() const;
    UInt32  getOffsetX() const;
    UInt32  getOffsetY() const;
    UInt32  getDiagPitch() const;
    UInt32  getDiagPitch45() const;
    UInt32  getDiagPitch135() const;
    UInt32  getDiagWidth() const;
    UInt32  getDiagSpacing() const;
    UInt32  getDiagMinEdgeLength() const;
    UInt32  getMGrid() const;
    UInt32  getWireExt() const;
    UInt32  getFillCheckWinLength() const;
    UInt32  getFillCheckWinWidth() const;
    UInt32  getFillCheckWinStep() const;
    UInt32  getFillActiveSpacing() const;
    UInt32  getFillToFillSpacing() const;
    UInt32  getHeight() const;
    UInt32  getThickness() const;
    UInt32  getShrinkage() const;

    float   getResPerSq() const;
    float   getCapPerSq() const;
    float   getCapMul() const;
    float   getEdgeCap() const;
    float   getFillMinDensity() const;
    float   getFillMaxDensity() const;
    Layer*  getTrimLayer() const;

    void    setIsRectOnly(bool r);
    void    setIsExceptNonCorePins(bool v);
    void    setMinWidth(UInt32 w);
    void    setMaxWidth(UInt32 w);
    void    setWrongDirWidth(UInt32 w);
    void    setPitch(UInt32 p);
    void    setPitchXY(UInt32 px, UInt32 py);
    void    setFirstLastPitch(UInt32 p);
    void    setOffset(UInt32 o);
    void    setOffsetXY(UInt32 ox, UInt32 oy);
    void    setDiagPitch(UInt32 dp);
    void    setDiagPitchXY(UInt32 dp45, UInt32 dp135);
    void    setDiagWidth(UInt32 w);
    void    setDiagSpacing(UInt32 w);
    void    setDiagMinEdgeLength(UInt32 l);
    void    setMGrid(UInt32 m);
    void    setWireExt(UInt32 ext);
    void    setFillCheckWinLength(UInt32 l);
    void    setFillCheckWinWidth(UInt32 w);
    void    setFillCheckWinStep(UInt32 s);
    void    setFillActiveSpacing(UInt32 sp);
    void    setFillToFillSpacing(UInt32 sp);
    void    setHeight(UInt32 h);
    void    setThickness(UInt32 t);
    void    setShrinkage(UInt32 s);
    void    setResPerSq(float r);
    void    setCapPerSq(float c);
    void    setCapMul(float c);
    void    setEdgeCap(float c);
    void    setFillMinDensity(float d);
    void    setFillMaxDensity(float d);
    void    setTrimLayer(Layer* trim_layer);

    void    setDirection(UInt32 v);

    BoundaryEOLBlockage* getBoundaryEOLBlocakge() const;
    void    setBoundaryEOLBlockage(BoundaryEOLBlockage* eol_blkg);

    CornerEOLKeepout*  getCornerEOLKeepoutList() const;
    void    setCornerEOLKeepoutList(CornerEOLKeepout* ceolk);

    CornerFillSpacing* getCornerFillSpacingList() const;
    void    setCornerFillSpacingList(CornerFillSpacing* cfs);

    CornerSpacing* getCornerSpacingList() const;
    void    setCornerSpacingList(CornerSpacing* cs);

    DirSpanLengthSpTbl* getDirSpanLengthSpTblList() const;
    void  setDirSpanLengthSpTblList(DirSpanLengthSpTbl* dsl);

    EOLKeepout* getEOLKeepoutList() const;
    void    setEOLKeepoutList(EOLKeepout* eolk);

    MinCut* getMinCutList() const;
    void    setMinCutList(MinCut* l);

    MinEnclArea*  getMinEnclAreaList() const;
    void     setMinEnclAreaList(MinEnclArea* l);

    MinSize* getMinSize() const;
    void     setMinSize(MinSize* l);

    MinStep* getMinStepList() const;
    void     setMinStepList(MinStep* l);

    ProtrusionRule* getProtrusionRuleList() const;
    void     setProtrusionRuleList(ProtrusionRule* l);

    WidthSpTbl* getWidthSpTbl() const;
    void     setWidthSpTabl(WidthSpTbl* l);

    InfluenceSpTbl* getInfluenceSpTbl() const;
    void     setInfluenceSpTbl(InfluenceSpTbl* t);

    ParaSpanLenTbl* getParaSpanLenTbl() const;
    void     setParaSpanLenTbl(ParaSpanLenTbl* t);

    RoutingSpacing* getSpacingList() const;
    void     setSpacingList(RoutingSpacing* l);

  private:
    Bits     pref_direction_ : 3;  /**< DIRECTION HORIZONTAL | VETICAL | DIAG45 | DIAG135 */
    Bits     rect_only_      : 1;  /**< LEF58_RECTONLY */
    Bits     is_except_non_core_pins_ : 1;  /**< EXCEPTNONCOREPINS, ignore rectonly check connecting to non-corepin*/
    Bits     xy_pitch_       : 1;  /**< LEF 5.6 define PITCH xDistance yDistance in tech lef */
    Bits     xy_offset_      : 1;  /**< LEF 5.6 define OFFSET xDistance yDistance in tech lef */
    Bits     xy_diag_pitch_  : 1;  /**< LEF 5.6 define DIAGPITCH diag45Distance diag135Distance */

    UInt32   min_width_;          /**< minimum wire width: MINWIDTH */
    UInt32   max_width_;          /**< maximum wire width: MAXWIDTH */
    UInt32   wrong_dir_width_;    /**< default width: WIDTH minwidth WRONGDIRECTION */
    UInt32   pitch_x_;            /**< LEF 5.6 PITCH distance or PITCH xDistance */
    UInt32   pitch_y_;            /**< LEF 5.6 PITCH xDistance yDistance */
    UInt32   first_last_pitch_;   /**< LEF 5.8 FIRSTLASTPITCH */
    UInt32   offset_x_;           /**< LEF 5.6 OFFSET distance or OFFSET xDistance */
    UInt32   offset_y_;           /**< LEF 5.6 OFFSET xDistance yDistance */
    UInt32   diag_pitch45_;       /**< LEF 5.6 DIAGPITCH distance or DIAGPITCH diag45Distance*/
    UInt32   diag_pitch135_;      /**< LEF 5.6 DIAGPITCH diag45Distance diag135Distance */
    UInt32   diag_width_;         /**< 45-degree angle shape width: DIAGWIDTH diagWidth */
    UInt32   diag_spacing_;       /**< DIAGSPACING diagSpacing */
    UInt32   diag_min_edge_length_; /**< DIAGMINEDGELEGHTH diagLength */
    UInt32   m_grid_;             /**< LEF5.8 MANUFACTURINGGRID */
    UInt32   wire_extension_;     /**< the extension at vias is more than half of the routing width: WIREEXTENSION */
    UInt32   fill_check_length_;  /**< length of the check window : DENSITYCHECKWINDOW windowLength windowWidth */
    UInt32   fill_check_width_;   /**< width of the check window : DENSITYCHECKWINDOW windowLength windowWidth */
    UInt32   fill_check_step_;    /**< step of the check window : DENSITYCHECKSTEP stapeValue */
    UInt32   fill_to_active_spacing_;  /**< the spacing between metal fills and other geometries: FILLACTIVESPACING spacing */
    UInt32   fill_to_fill_spacing_;  /**< LEF58_FILLTOFILLSPACING: spacing between metal fills */
    UInt32   height_;             /**< distance from the top of ground place to the bottom of the interconnect: HEIGHT distance */
    UInt32   thickness_;          /**< thickness of the interconnect to calculate the antenna side area: THICKNESS distance */
    UInt32   shrinkage_;          /**< due to the etching process: SHRINKAGE distance */

    float   res_per_square_;     /**< the resistance for a square of wire, in ohms per square: RESISTANCE RPERSQ value */
    float   cap_per_square_;     /**< the capacitance for each square unit: CAPACITANCE CPERSQDIST value */
    float   cap_multiplier_;     /**< the multiplier for interconnect capacitance(default is 1): CAPMULTIPLIER value */
    float   edge_capacitance_;   /**< the value of peripheral capacitance: EDGECAPACITANCE value */
    float   fill_min_density_;   /**< minimum metal density, as a percentage: MINIMUMDENSITY minDensity */
    float   fill_max_density_;   /**< maximum metal density, as a percentage: MAXIMUMDENSITY maxDensity */

    Layer*   trim_layer_;        /**< trim layer */

    BoundaryEOLBlockage*       boundary_eol_blk_;  /**< BOUNDARYEOLBLOCKAGE */
    CornerEOLKeepout*          corner_eol_keepout_list_;  /**< CORNEREOLKEEPOUT */
    CornerFillSpacing*         corner_fill_spacing_list_; /**< CORNERFILLSPACING */
    CornerSpacing*             corner_spacing_list_; /**< CORNERSPACING */
    DirSpanLengthSpTbl*        dsl_list_;  /**< DIRECTIONALSPANLENGTH */
    EOLKeepout*                eol_keepout_list_;  /**< EOLKeepout */
    MinCut*                    min_cut_list_;  /**< MINIMUMCUT */
    MinEnclArea*               min_enclosed_area_list_;  /**< MINENCLOSEDAREA */
    MinSize*                   min_size_;  /**< MINSIZE */
    MinStep*                   min_step_list_;  /**< MINSTEP */
    ProtrusionRule*            protrusion_list_;  /**< PROTRUSIONWIDTH */
    WidthSpTbl*                width_sp_tbl_list_;  /**< SPACINGTABLE PARALLELRUNLENGTH WIDTH | TWOWIDTHS */
    InfluenceSpTbl*            influence_sp_tbl_;  /**< SPACINGTABLE INFLUENCE */
    ParaSpanLenTbl*            para_span_len_sp_tbl_;  /**< SPACINGTABLE PARALLELSPANLENGTH */
    RoutingSpacing*            spacing_list_;  /**< SPACING */
};

}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_TECH_ROUTING_LAYER_RULE_H_
