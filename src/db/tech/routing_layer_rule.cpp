/* @file  routing_layer_rule.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/tech/routing_layer_rule.h"
#include "db/core/cell.h"
#include "db/core/db.h"


namespace open_edi {
namespace db {

ObjectId EOLKeepout::class_names_ = 0;

/**
 * @brief
 * get blockage dimension
 *
 * @return
 */
UInt32 BoundaryEOLBlockage::getSize() const {
    return size_;
}

/**
 * @brief
 * set blockage dimension
 *
 * @param size
 */
void BoundaryEOLBlockage::setSize(UInt32 size) {
    size_ = size;
}

/**
 * @brief
 * get boundary offset
 *
 * @return
 */
UInt32 BoundaryEOLBlockage::getOffset() const {
    return offset_;
}

/**
 * @brief
 * set boundary offset
 *
 * @param offset
 */
void BoundaryEOLBlockage::setOffset(UInt32 offset) {
    offset_ = offset;
}

/**
 * @brief
 * get parallel length
 *
 * @return
 */
UInt32 BoundaryEOLBlockage::getParallelLength() const {
    return par_length_;
}

/**
 * @brief
 * set parallel length
 *
 * @param len
 */
void BoundaryEOLBlockage::setParallelLength(UInt32 len) {
    par_length_ = len;
}

/**
 * @brief
 * get parallel within
 *
 * @return
 */
UInt32 BoundaryEOLBlockage::getParallelWithin() const {
    return par_within_;
}

/**
 * @brief
 * set parallel within
 *
 * @param dist
 */
void BoundaryEOLBlockage::setParallelWithin(UInt32 dist) {
    par_within_ = dist;
}

/**
 * @brief
 * get spacing
 *
 * @return
 */
UInt32 BoundaryEOLBlockage::getSpacing() const {
    return spacing_;
}

/**
 * @brief
 * set spacing
 *
 * @param spacing
 */
void BoundaryEOLBlockage::setSpacing(UInt32 spacing) {
    spacing_ = spacing;
}

/**
 * @brief
 * BoundaryEOLBlockage object has no list.
 * add getNext to free the object by template common function
 *
 * @return
 */
BoundaryEOLBlockage* BoundaryEOLBlockage::getNext() const {
    return nullptr;
}

/**
 * @brief
 * get SPACING spacing1
 *
 * @return
 */
UInt32 CornerEOLKeepout::getSpacing1() const {
    return is_spacing_ ? u_.spacing_.spacing1_ : 0;
}

/**
 * @brief
 * get SPACING spacing2
 *
 * @return
 */
UInt32 CornerEOLKeepout::getSpacing2() const {
    return is_spacing_ ? u_.spacing_.spacing2_ : 0;
}

/**
 * @brief
 * get SPACING WITHIN within1
 *
 * @return
 */
UInt32 CornerEOLKeepout::getWithin1() const {
    return is_spacing_ ? u_.spacing_.within1_ : 0;
}

/**
 * @brief
 * get SPACING WINTHIN x within2
 *
 * @return
 */
UInt32 CornerEOLKeepout::getWithin2() const {
    return is_spacing_ ? u_.spacing_.within2_ : 0;
}

/**
 * @brief
 * get EXTENSION backwardExt
 *
 * @return
 */
UInt32 CornerEOLKeepout::getBackwardExt() const {
    return is_spacing_ ? 0 : u_.extension_.backward_ext_;
}

/**
 * @brief
 * get EXTENSION x sideExt
 *
 * @return
 */
UInt32 CornerEOLKeepout::getSideExt() const {
    return is_spacing_ ? 0 : u_.extension_.side_ext_;
}

/**
 * @brief
 * get EXTENSION x x forwardExt
 *
 * @return
 */
UInt32 CornerEOLKeepout::getForwardExt() const {
    return is_spacing_ ? 0 : u_.extension_.forward_ext_;
}

/**
 * @brief
 * set SPACING statement
 *
 * @param sp1
 * @param sp2
 * @param within1
 * @param within2
 */
void CornerEOLKeepout::setSpacing(UInt32 sp1, UInt32 sp2, UInt32 within1, UInt32 within2) {
    is_spacing_ = true;
    u_.spacing_.spacing1_ = sp1;
    u_.spacing_.spacing2_ = sp2;
    u_.spacing_.within1_ = within1;
    u_.spacing_.within2_ = within2;
}

/**
 * @brief
 * set EXTENSION statement
 *
 * @param backward
 * @param side
 * @param forward
 */
void CornerEOLKeepout::setExtension(UInt32 backward, UInt32 side, UInt32 forward) {
    is_spacing_ = false;
    u_.extension_.backward_ext_ = backward;
    u_.extension_.side_ext_ = side;
    u_.extension_.forward_ext_ = forward;
}

/**
 * @brief
 * indicate whether there is SPACING statment or not
 *
 * @return
 */
bool CornerEOLKeepout::isSpacing() const {
    return is_spacing_;
}

/**
 * @brief
 * get the spacinf of CORNERFILLSPACING spacing
 *
 * @return
 */
UInt32 CornerFillSpacing::getSpacing() const {
    return spacing_;
}

/**
 * @brief
 * get the length1 of CORNERFILLSPACING x EDGELENGTH length1
 *
 * @return
 */
UInt32 CornerFillSpacing::getLength1() const {
    return length1_;
}

/**
 * @brief
 * get the length2 of CORNERFILLSPACING x EDGELENGTH x length2
 *
 * @return
 */
UInt32 CornerFillSpacing::getLength2() const {
    return length2_;
}

/**
 * @brief
 * get eolWidth of ADJACENTEOL eolWidth
 *
 * @return
 */
UInt32 CornerFillSpacing::getEOLWidth() const {
    return eol_width_;
}

/**
 * @brief
 * set vaules of CORNERFILLSPACING
 *
 * @param sp
 * @param len1
 * @param len2
 * @param eol_w
 */
void CornerFillSpacing::setCornerFillSpacing(UInt32 sp, UInt32 len1, UInt32 len2, UInt32 eol_w) {
    spacing_ = sp;
    length1_ = len1;
    length2_ = len2;
    eol_width_ = eol_w;
}

CornerSpacing::~CornerSpacing() {
}

/**
 * @brief
 * indicate whether it is CONVEXCORNER or not
 *
 * @return
 */
bool CornerSpacing::isConvexCorner() const {
    return convex_corner_;
}

/**
 * @brief
 * set CONVEXCORNER flag
 *
 * @param b
 */
void CornerSpacing::setIsConvexCorner(bool b) {
    convex_corner_ = b ? 1 : 0;
}

/**
 * @brief
 * indicate whether it is CONCAVECORNER or not
 *
 * @return
 */
bool CornerSpacing::isConcaveCorner() const {
    return concave_corner_;
}

/**
 * @brief
 * set CONCAVECORNER flag
 *
 * @param b
 */
void CornerSpacing::setIsConcaveCorner(bool b) {
    concave_corner_ = b ? 1 : 0;
}

/**
 * @brief
 * indicate whether it is SAMEMASK CONVEXCORNER
 *
 * @return
 */
bool CornerSpacing::isSameMask() const {
    return convex_corner_ ? same_mask_ : false;
}

/**
 * @brief
 * set SAMEMASK flag
 *
 * @param b
 */
void CornerSpacing::setIsSameMask(bool b) {
    same_mask_ = b ? 1 : 0;
}

/**
 * @brief
 * indicate whether it is CORNERTOCORNER's CONVEXCORNER
 *
 * @return
 */
bool CornerSpacing::isCornerToCorner() const {
    return convex_corner_ ? corner_to_corner_ : false;
}

/**
 * @brief
 * set CORNERTOCORNER flag
 *
 * @param b
 */
void CornerSpacing::setIsCornerToCorner(bool b) {
    corner_to_corner_ = b ? 1 : 0;
}

/**
 * @brief
 * indicate whether it has EDGELENGTH or not
 *
 * @return
 */
bool CornerSpacing::isEdgeLength() const {
    return convex_corner_ ? edge_length_ : false;
}

/**
 * @brief
 * set EDGELENGTH flag
 *
 * @param b
 */
void CornerSpacing::setIsEdgeLength(bool b) {
    edge_length_ = b ? 1 : 0;
}

/**
 * @brief
 * indicate whether it has INCLUDELSHAPE or not
 *
 * @return
 */
bool CornerSpacing::isIncludeLShape() const {
    return convex_corner_ ? include_lshape_ : false;
}

/**
 * @brief
 * set INCLUDELSHAPE flag
 *
 * @param b
 */
void CornerSpacing::setIsIncludeLShape(bool b) {
    include_lshape_ = b ? 1 : 0;
}

/**
 * @brief
 * indicate whether it has EXCEPTNOTCH or not
 *
 * @return
 */
bool CornerSpacing::isExceptNotch() const {
    return concave_corner_ ? except_notch_ : false;
}

/**
 * @brief
 * set EXCEPTNOTCH flag
 *
 * @param b
 */
void CornerSpacing::setIsExceptNotch(bool b) {
    except_notch_ = b ? 1 : 0;
}

/**
 * @brief
 * indicate whether is has EXCEPTSAMENET or not
 *
 * @return
 */
bool CornerSpacing::isExceptSameNet() const {
    return except_same_net_;
}

/**
 * @brief
 * set EXCEPTSAMENET flag
 *
 * @param b
 */
void CornerSpacing::setIsExceptSameNet(bool b) {
    except_same_net_ = b ? 1 : 0;
}

/**
 * @brief
 * indicate whether it has EXCEPTSAMEMETAL or not
 *
 * @return
 */
bool CornerSpacing::isExceptSameMetal() const {
    return except_same_metal_;
}

/**
 * @brief
 * set EXCEPTSAMEMETAL flag
 *
 * @param b
 */
void CornerSpacing::setIsExceptSameMetal(bool b) {
    except_same_metal_ = b ? 1 : 0;
}

/**
 * @brief
 * get corner within value
 *
 * @return
 */
UInt32 CornerSpacing::getCornerOnlyWithin() const {
    return corner_to_corner_ ? 0 : (convex_corner_ ? corner_only_within_ : 0);
}

/**
 * @brief
 * set corner within value
 *
 * @param within
 */
void CornerSpacing::setCornerOnlyWithin(UInt32 within) {
    corner_only_within_ = within;
}

/**
 * @brief
 * get except eol width
 *
 * @return
 */
UInt32 CornerSpacing::getExceptEOLWidth() const {
    return  convex_corner_ ? eol_width_ : 0;
}

/**
 * @brief
 * set except eol width
 *
 * @param w
 */
void CornerSpacing::setExceptEOLWidth(UInt32 w) {
    eol_width_ = w;
}

/**
 * @brief
 * get except jog length
 *
 * @return
 */
UInt32 CornerSpacing::getExceptJogLength() const {
    return  convex_corner_ ? except_jog_length_ : 0;
}

/**
 * @brief
 * set except jog length
 *
 * @param jog_len
 */
void CornerSpacing::setExceptJogLength(UInt32 jog_len) {
    except_jog_length_ = jog_len;
}

/**
 * @brief
 * get min concave corner length
 *
 * @return
 */
UInt32 CornerSpacing::getMinLength() const {
    return  concave_corner_ ? min_length_ : 0;
}

/**
 * @brief
 * set min concave corner length
 *
 * @param len
 */
void CornerSpacing::setMinLength(UInt32 len) {
    min_length_ = len;
}

/**
 * @brief
 * get except notch length
 *
 * @return
 */
UInt32 CornerSpacing::getNotchLength() const {
    return  concave_corner_ ? notch_length_ : 0;
}

/**
 * @brief
 * set except notch length
 *
 * @param len
 */
void CornerSpacing::setNotchLength(UInt32 len) {
    notch_length_ = len;
}

/**
 * @brief
 * get the number of widths
 *
 * @return
 */
UInt32 CornerSpacing::getNumWidths() const {
    return widths_num_;
}

/**
 * @brief
 * set the number of widths
 *
 * @param num
 */
void CornerSpacing::setNumWidths(UInt32 num) {
    widths_num_ = num;

}

/**
 * @brief
 * get the width value based on the specified index
 *
 * @param index
 *
 * @return
 */
UInt32 CornerSpacing::getWidth(UInt32 index) const {
    //return index < widths_num_ ? widths_[index] : 0;

    if (index >= widths_num_) {
        return -1;
    }
    
    if (widths_ == 0) 
        return -1;
    ArrayObject<Int32> *array_ptr = addr< ArrayObject<Int32> >(widths_);
    if (array_ptr == nullptr)
        return -1;

    return (*array_ptr)[index];
    
}

ArrayObject<Int32> *CornerSpacing::getWidths() const{
    if (widths_ != 0) {
        ArrayObject<Int32> *width_array = addr<ArrayObject<Int32>>(widths_);
        return width_array;
    } else {
        return nullptr;
    }
}


/**
 * @brief
 * append one width value
 *
 * @param w
 */
void CornerSpacing::addWidth(UInt32 w) {
    ArrayObject<Int32> *array_ptr = nullptr;
    if (widths_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<Int32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        widths_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<Int32> >(widths_);
    }

    if (array_ptr) array_ptr->pushBack(w);
}

/**
 * @brief
 * get the spacing value based on the specified index
 *
 * @param index
 *
 * @return
 */
UInt32 CornerSpacing::getSpacing(UInt32 index) const {
    return getHorizontalSpacing(index);
}

/**
 * @brief
 * append one spacing value
 *
 * @param sp
 */
void CornerSpacing::addSpacing(UInt32 sp) {
    ArrayObject<Int32> *array_ptr = nullptr;
    if (horizontal_spacings_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<Int32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        horizontal_spacings_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<Int32> >(horizontal_spacings_);
    }

    if (array_ptr) array_ptr->pushBack(sp);
}

/**
 * @brief
 * get the hoirzontal spacing value based on the specified index
 *
 * @param index
 *
 * @return
 */
UInt32 CornerSpacing::getHorizontalSpacing(UInt32 index) const {
    if (index >= widths_num_) {
        return -1;
    }
    
    if (horizontal_spacings_ == 0) 
        return -1;
    ArrayObject<Int32> *array_ptr = addr< ArrayObject<Int32> >(horizontal_spacings_);
    if (array_ptr == nullptr)
        return -1;

    return (*array_ptr)[index];
}

ArrayObject<Int32> *CornerSpacing::getHorizontalSpacings() const{
    if (horizontal_spacings_ != 0) {
        ArrayObject<Int32> *hori_array = addr<ArrayObject<Int32>>(horizontal_spacings_);
        return hori_array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * append one horizontal spacing
 *
 * @param hsp
 */
void CornerSpacing::addHorizontalSpacing(UInt32 hsp) {
    ArrayObject<Int32> *array_ptr = nullptr;
    if (horizontal_spacings_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<Int32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        horizontal_spacings_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<Int32> >(horizontal_spacings_);
    }

    if (array_ptr) array_ptr->pushBack(hsp);
}

/**
 * @brief
 * get the horizontal spacing based on the specified index
 *
 * @param index
 *
 * @return
 */
UInt32 CornerSpacing::getVerticalSpacing(UInt32 index) const {
    if (index >= widths_num_) {
        return -1;
    }
    
    if (vertical_spacings_ == 0) 
        return -1;
    ArrayObject<Int32> *array_ptr = addr< ArrayObject<Int32> >(vertical_spacings_);
    if (array_ptr == nullptr)
        return -1;

    return (*array_ptr)[index];
}

ArrayObject<Int32> *CornerSpacing::getVerticalSpacings() const{
    if (vertical_spacings_ != 0) {
        ArrayObject<Int32> *vetical_array = addr<ArrayObject<Int32>>(vertical_spacings_);
        return vetical_array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * append one vertical spacing value
 *
 * @param vsp
 */
void CornerSpacing::addVertcialSpacing(UInt32 vsp) {
    ArrayObject<Int32> *array_ptr = nullptr;
    if (vertical_spacings_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<Int32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        vertical_spacings_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<Int32> >(vertical_spacings_);
    }

    if (array_ptr) array_ptr->pushBack(vsp);
}

/**
 * @brief
 * release allocated memory
 */
DirSpanLengthSpTbl::ExactSLSpacing::~ExactSLSpacing() {
}

/**
 * @brief
 * get spanlength1 in EXACTSPANLENGTHSPACING spanLength1
 *
 * @return
 */
UInt32 DirSpanLengthSpTbl::ExactSLSpacing::getSpanLength1() const {
    return span_len1_;
}

/**
 * @brief
 * set spanlength1
 *
 * @param len
 */
void DirSpanLengthSpTbl::ExactSLSpacing::setSpanLength1(UInt32 len) {
    span_len1_ = len;
}

/**
 * @brief
 * get the spanlength2 in EXACTSPANLENGTHSPACING x TO spanLength2
 *
 * @return
 */
UInt32 DirSpanLengthSpTbl::ExactSLSpacing::getSpanLength2() const {
    return span_len2_;
}

/**
 * @brief
 * set spanlength2
 *
 * @param len
 */
void DirSpanLengthSpTbl::ExactSLSpacing::setSpanLength2(UInt32 len) {
    span_len2_ = len;
}

/**
 * @brief
 * get the prl in EXACTSPANLENGTHSPACING
 *
 * @return
 */
UInt32 DirSpanLengthSpTbl::ExactSLSpacing::getPRL() const {
    return prl_;
}

/**
 * @brief
 * set prl
 *
 * @param prl
 */
void DirSpanLengthSpTbl::ExactSLSpacing::setPRL(UInt32 prl) {
    prl_ = prl;
}

/**
 * @brief
 * get the number of exact spacings
 *
 * @return
 */
UInt32 DirSpanLengthSpTbl::ExactSLSpacing::getNumSpacings() const {
    return spacings_num_;
}

/**
 * @brief
 * set the number of exact spacings
 *
 * @param num
 */
void DirSpanLengthSpTbl::ExactSLSpacing::setNumSpacings(UInt32 num) {
    spacings_num_ = num;
}

/**
 * @brief
 * get the spacing in EXACTSPANLENGTHSPACING
 *
 * @param idx
 *
 * @return
 */

ArrayObject<Int32> *DirSpanLengthSpTbl::ExactSLSpacing::getSpacings() const { 
    if (exact_spacings_ != 0) {
        ArrayObject<Int32> *spacing_array = addr<ArrayObject<Int32>>(exact_spacings_);
        return spacing_array;
    } else {
        return nullptr;
    }
}

UInt32 DirSpanLengthSpTbl::ExactSLSpacing::getSpacing(UInt32 idx) const {
    if (idx >= spacings_num_) {
        return -1;
    }
    
    if (exact_spacings_ == 0) 
        return -1;
    ArrayObject<Int32> *array_ptr = addr< ArrayObject<Int32> >(exact_spacings_);
    if (array_ptr == nullptr)
        return -1;

    return (*array_ptr)[idx];
}

/**
 * @brief
 * append one spacing
 *
 * @param sp
 */
void DirSpanLengthSpTbl::ExactSLSpacing::addSpacing(UInt32 sp) {
    ArrayObject<Int32> *array_ptr = nullptr;
    if (exact_spacings_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<Int32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        exact_spacings_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<Int32> >(exact_spacings_);
    }

    if (array_ptr) array_ptr->pushBack(sp);
}

DirSpanLengthSpTbl::SpanLength::~SpanLength() {
}

/**
 * @brief
 * SPANLENGTH EXACTSPACING flag
 *
 * @return
 */
bool DirSpanLengthSpTbl::SpanLength::isExactSpacing() const {
    return is_exact_spacing_;
}

/**
 * @brief
 * set SPANLENGTH EXACTSPACING flag
 *
 * @param v
 */
void DirSpanLengthSpTbl::SpanLength::setIsExactSpacing(bool v) {
    is_exact_spacing_ = v ? 1 : 0;
}

/**
 * @brief
 * get SPANLENGTH SPACINGTOMINSPAN flag
 *
 * @return
 */
bool DirSpanLengthSpTbl::SpanLength::isSpacingToMinSpan() const {
    return is_spacing_to_min_span_;
}

/**
 * @brief
 * set SPANLENGTH SPACINGTOMINSPAN flag
 *
 * @param v
 */
void DirSpanLengthSpTbl::SpanLength::setIsSpacingToMinSpan(bool v) {
    is_spacing_to_min_span_ = v ? 1 : 0;
}

/**
 * @brief
 * get SPANLENGTH EXACTSELFSPACING flag
 *
 * @return
 */
bool DirSpanLengthSpTbl::SpanLength::isExactSelfSpacing() const {
    return is_exact_self_spacing_;
}

/**
 * @brief
 * set SPANLENGTH EXACTSELFSPACING flag
 *
 * @param v
 */
void DirSpanLengthSpTbl::SpanLength::setIsExactSelfSpacing(bool v) {
    is_exact_self_spacing_ = v ? 1 : 0;
}

/**
 * @brief
 * get SPANLENGTH NOEXCEPTEOL flag
 *
 * @return
 */
bool DirSpanLengthSpTbl::SpanLength::isNoExceptEOL() const {
    return is_no_except_eol_;
}

/**
 * @brief
 * set SPANLENGTH NOEXCEPTEOL flag
 *
 * @param v
 */
void DirSpanLengthSpTbl::SpanLength::setIsNoExceptEOL(bool v) {
    is_no_except_eol_ = v ? 1 : 0;
}

/**
 * @brief
 * get SPANLENGTH EOLSPACING flag
 *
 * @return
 */
bool DirSpanLengthSpTbl::SpanLength::isEOLSpacing() const {
    return is_eol_spacing_;
}

/**
 * @brief
 * set SPANLENGTH EOLSPACING flag
 *
 * @param v
 */
void DirSpanLengthSpTbl::SpanLength::setIsEOLSpacing(bool v) {
    is_eol_spacing_ = v ? 1 : 0;
}

/**
 * @brief
 * get SPANLENGTH length on this row
 *
 * @return
 */
UInt32 DirSpanLengthSpTbl::SpanLength::getSpanLength() const {
    return span_length_;
}

/**
 * @brief
 * set SPANLENGTH length on this row
 *
 * @param len
 */
void DirSpanLengthSpTbl::SpanLength::setSpanLength(UInt32 len) {
    span_length_ = len;
}

/**
 * @brief
 * get SPANLENGTH EXACTSPACING spacing on this row
 *
 * @return
 */
UInt32 DirSpanLengthSpTbl::SpanLength::getExactSpacing() const {
    return is_exact_spacing_ ? exact_spacing_ : 0;
}

/**
 * @brief
 * set SPANLENGTH EXACTSPACING spacing
 *
 * @param sp
 */
void DirSpanLengthSpTbl::SpanLength::setExactSpacing(UInt32 sp) {
    is_exact_spacing_ = 1;
    exact_spacing_ = sp;
}

/**
 * @brief
 * get SPANLENGTH SPACINGTOMINSPAN spacing on this row spanlength
 *
 * @return
 */
UInt32 DirSpanLengthSpTbl::SpanLength::getSpacingToMinSpan() const {
    return is_spacing_to_min_span_ ? spacing_to_min_span_ : 0;
}

/**
 * @brief
 * set SPANLENGTH SPACINGTOMINSPAN spacing
 *
 * @param sp
 */
void DirSpanLengthSpTbl::SpanLength::setSpacingToMinSpan(UInt32 sp) {
    is_spacing_to_min_span_ = 1;
    spacing_to_min_span_ = sp;
}

/**
 * @brief
 * get SPANLENGTH EXACTSELFSPACING spacing
 *
 * @return
 */
UInt32 DirSpanLengthSpTbl::SpanLength::getExactSelfSpacing() const {
    return exact_self_spacing_;
}

/**
 * @brief
 * set SPANLENGTH EXACTSELFSPACING spacing
 *
 * @param sp
 */
void DirSpanLengthSpTbl::SpanLength::setExactSelefSpacing(UInt32 sp) {
    exact_self_spacing_ = sp;
}

/**
 * @brief
 * get SPANLENGTH EOLSPACING spacing
 *
 * @return
 */
UInt32 DirSpanLengthSpTbl::SpanLength::getEOLSpacing() const {
    return eol_spacing_;
}

/**
 * @brief
 * set SPANLENGTH EOLSPACING spacing
 *
 * @param sp
 */
void DirSpanLengthSpTbl::SpanLength::setEOLSpacing(UInt32 sp) {
    eol_spacing_ = sp;
}

/**
 * @brief
 * get the number of spacings
 *
 * @return
 */
UInt32 DirSpanLengthSpTbl::SpanLength::getNumSpacings() const {
    return spacings_num_;
}

/**
 * @brief
 * set the number of spacings
 *
 * @param num
 */
void DirSpanLengthSpTbl::SpanLength::setNumSpacings(UInt32 num) {
    spacings_num_ = num;
}

/**
 * @brief
 * get the spacing on the specified column
 *
 * @param col_idx
 *
 * @return
 */
UInt32 DirSpanLengthSpTbl::SpanLength::getSpacing(UInt32 idx) const {
    if (idx >= spacings_num_) {
        return -1;
    }
    
    if (row_spacings_ == 0) 
        return -1;
    ArrayObject<Int32> *array_ptr = addr< ArrayObject<Int32> >(row_spacings_);
    if (array_ptr == nullptr)
        return -1;

    return (*array_ptr)[idx];
}

 ArrayObject<Int32> *DirSpanLengthSpTbl::SpanLength::getSpacings() const {
    if (row_spacings_ != 0) {
        ArrayObject<Int32> *spacing_array = addr<ArrayObject<Int32>>(row_spacings_);
        return spacing_array;
    } else {
        return nullptr;
    }
 }

/**
 * @brief
 * append the spacing element
 *
 * @param sp
 */
void DirSpanLengthSpTbl::SpanLength::addSpacing(UInt32 sp) {
   ArrayObject<Int32> *array_ptr = nullptr;
    if (row_spacings_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<Int32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        row_spacings_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<Int32> >(row_spacings_);
    }

    if (array_ptr) array_ptr->pushBack(sp);
}

DirSpanLengthSpTbl::~DirSpanLengthSpTbl() {
}

/**
 * @brief
 * get WRONGDIRECTION flag
 *
 * @return
 */
bool DirSpanLengthSpTbl::isWrongDir() const {
    return wrong_dir_;
}

/**
 * @brief
 * set WRONGDIRECTION flag
 *
 * @param v
 */
void DirSpanLengthSpTbl::setIsWrongDir(bool v) {
    wrong_dir_ = v ? 1 : 0;
}

/**
 * @brief
 * get SAMEMASK flag
 *
 * @return
 */
bool DirSpanLengthSpTbl::isSameMask() const {
    return same_mask_;
}

/**
 * @brief
 * set SAMEMASK flag
 *
 * @param v
 */
void DirSpanLengthSpTbl::setIsSameMask(bool v) {
    same_mask_ = v ? 1 : 0;
}

/**
 * @brief
 * get EDGELENGTH flag
 *
 * @return
 */
bool DirSpanLengthSpTbl::isEdgeLength() const {
    return edge_length_;
}

/**
 * @brief
 * set EDGELENGTH flag
 *
 * @param v
 */
void DirSpanLengthSpTbl::setIsEdgeLength(bool v) {
    edge_length_ = v ? 1 : 0;
}

/**
 * @brief
 * get INCLUDELSHAPE flag
 *
 * @return
 */
bool DirSpanLengthSpTbl::isIncludeLShape() const {
    return include_lshape_;
}

/**
 * @brief
 * set INCLUDELSHAPE flag
 *
 * @param v
 */
void DirSpanLengthSpTbl::setIsIncludeLShape(bool v) {
    include_lshape_ = v ? 1 : 0;
}

/**
 * @brief
 * get EXCEPTJOGLENGTH length
 *
 * @return
 */
UInt32 DirSpanLengthSpTbl::getJogLength() const {
    return jog_length_;
}

/**
 * @brief
 * set EXCEPTJOGLENGTH length
 *
 * @param jog_len
 */
void DirSpanLengthSpTbl::setJogLength(UInt32 jog_len) {
    jog_length_ = jog_len;
}

/**
 * @brief
 * get EXCEPTEOL eolWidth
 *
 * @return
 */
UInt32 DirSpanLengthSpTbl::getEOLWidth() const {
    return eol_width_;
}

/**
 * @brief
 * set EXCEPTEOL eolWidth
 *
 * @param w
 */
void DirSpanLengthSpTbl::setEOLWidth(UInt32 w) {
    eol_width_ = w;
}

/**
 * @brief
 * get the number of prls
 *
 * @return
 */
UInt32 DirSpanLengthSpTbl::getNumPRLs() const {
    return prls_num_;
}

/**
 * @brief
 * set the number of prls
 *
 * @param num
 */
void DirSpanLengthSpTbl::setNumPRLs(UInt32 num) {
    prls_num_ = num;
}

/**
 * @brief
 * get PRL
 *
 * @param idx
 *
 * @return
 */
UInt32 DirSpanLengthSpTbl::getPRL(UInt32 idx) const {   
    if (prls_ == 0) 
        return 0;
    ArrayObject<UInt32> *array_ptr = addr< ArrayObject<UInt32> >(prls_);
    if (idx >= prls_num_) {
        return (*array_ptr)[prls_num_ - 1];
    }
    if (array_ptr == nullptr)
        return 0;

    return (*array_ptr)[idx];
}

ArrayObject<UInt32>* DirSpanLengthSpTbl::getPRLs() const {
    if (prls_ != 0) {
        ArrayObject<UInt32> *array = addr<ArrayObject<UInt32>>(prls_);
        return array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * append prl element
 *
 * @param prl
 */
void DirSpanLengthSpTbl::addPRL(UInt32 prl) {
    ArrayObject<UInt32> *array_ptr = nullptr;
    if (prls_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<UInt32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        prls_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<UInt32> >(prls_);
    }

    if (array_ptr) array_ptr->pushBack(prls_);
}

/**
 * @brief
 * get the number of span length rows
 *
 * @return
 */
UInt32 DirSpanLengthSpTbl::getNumSpanLengths() const {
    return span_length_rows_num_;
}

/**
 * @brief
 * set the number of span length rows
 *
 * @param num
 */
void DirSpanLengthSpTbl::setNumSpanLengths(UInt32 num) {
    span_length_rows_num_ = num;
    //span_length_rows_ = new DirSpanLengthSpTbl::SpanLength[span_length_rows_num_];
    ArrayObject<ObjectId> *vct = nullptr;
    if (span_length_rows_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr)
            return;
        span_length_rows_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(span_length_rows_);
    }

    Cell *current_top_cell = getTopCell();
    while(num != 0) {
        num--;
        DirSpanLengthSpTbl::SpanLength *sl = current_top_cell->createObject<DirSpanLengthSpTbl::SpanLength>(kObjectTypeSpanLength);
        if (vct)
            vct->pushBack(sl->getId());
    }

}

/**
 * @brief
 * get span length on specified spanlength row
 *
 * @param row_idx
 *
 * @return
 */
UInt32 DirSpanLengthSpTbl::getSpanLength(UInt32 row_idx) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (span_length_rows_ == 0) {
        return 0;
    } else {
        vct = addr<ArrayObject<ObjectId>>(span_length_rows_);
    }
    if (vct) {
         DirSpanLengthSpTbl::SpanLength *obj_data = addr<DirSpanLengthSpTbl::SpanLength>((*vct)[row_idx]);
        if (obj_data) {
            return obj_data->getSpanLength();
        }
    }
    return 0;
}

/**
 * @brief
 * get span length spacing on specified row and column
 *
 * @param row_idx
 * @param col_idx
 *
 * @return
 */
UInt32 DirSpanLengthSpTbl::getSpacing(UInt32 row_idx, UInt32 col_idx) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (span_length_rows_ == 0) {
        return 0;
    } else {
        vct = addr<ArrayObject<ObjectId>>(span_length_rows_);
    }
    if (vct) {
         DirSpanLengthSpTbl::SpanLength *obj_data = addr<DirSpanLengthSpTbl::SpanLength>((*vct)[row_idx]);
        if (obj_data) {
            return obj_data->getSpacing(col_idx);
        }
    }
    return 0;
}

/**
 * @brief
 * get specified spenlength row
 *
 * @param row_idx
 *
 * @return
 */
 DirSpanLengthSpTbl::SpanLength* DirSpanLengthSpTbl::getSpanLengthRow(UInt32 row_idx) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (span_length_rows_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(span_length_rows_);
    }
    if (vct) {
         DirSpanLengthSpTbl::SpanLength *obj_data = addr<DirSpanLengthSpTbl::SpanLength>((*vct)[row_idx]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

/**
 * @brief
 * get EXACTSPANLENGTHSPACING list
 *
 * @return
 */
DirSpanLengthSpTbl::ExactSLSpacing* DirSpanLengthSpTbl::getExactSLSpacingList(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (exact_sl_spacings_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(exact_sl_spacings_);
    }
    if (vct) {
        DirSpanLengthSpTbl::ExactSLSpacing *obj_data = addr<DirSpanLengthSpTbl::ExactSLSpacing>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

ArrayObject<ObjectId>* DirSpanLengthSpTbl::getExactSLSpacingLists() const {
    if (exact_sl_spacings_ != 0) {
        ArrayObject<ObjectId> *exact_sl_spacings = addr<ArrayObject<ObjectId>>(exact_sl_spacings_);
        return exact_sl_spacings;
    } else {
        return nullptr;
    }
}


/**
 * @brief
 * set EXACTSPANLENGTHSPACING list
 *
 * @param sl
 */
void DirSpanLengthSpTbl::addExactSLSpacingList(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (exact_sl_spacings_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        exact_sl_spacings_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(exact_sl_spacings_);
    }
    if (vct) vct->pushBack(id);
}

/**
 * @brief
 * get class id
 * if there is no CLASS, id is always 0. CLASS id begins from 1
 *
 * @return
 */
EOLKeepout::EOLKeepout() {
    memset(static_cast<void*>(this), 0, sizeof(EOLKeepout));
    ArrayObject<SymbolIndex> *vct = nullptr;
    if (class_names_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<SymbolIndex>>(kObjectTypeArray);
        if (vct == nullptr)
            return;
        class_names_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
        if (vct) {
            vct->pushBack(getTopCell()->getOrCreateSymbol("default_noclass"));
            class_id_ = 0;
        }
    }
}

UInt32 EOLKeepout::getClassId() const {
    return class_id_;
}

/**
 * @brief
 * get class name
 *
 * @return
 */
const char* EOLKeepout::getClassName() {
    ArrayObject<SymbolIndex> *vct = nullptr;
    if (class_names_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<SymbolIndex>>(class_names_);
    }
    if (vct) {
        SymbolIndex sym_index = (*vct)[class_id_];
        if (sym_index != kInvalidSymbolIndex) {
            Cell *top_cell = getTopCell();
            return top_cell->getSymbolTable()->getSymbolByIndex(sym_index).c_str();
        }
    }
    return nullptr;
}

/**
 * @brief
 * set class name and set the related class id
 *
 * @param name
 */
void EOLKeepout::setClassName(const char* name) {
    ArrayObject<SymbolIndex> *vct = nullptr;
    if (class_names_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<SymbolIndex>>(kObjectTypeArray);
        if (vct == nullptr) return;
        class_names_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<SymbolIndex>>(class_names_);
    }
    if (vct) {
        if (name || name[0] != '\0') {
            for (int i = 0; i < vct->getSize(); ++i) {
                if (strcmp(getTopCell()->getSymbolTable()->getSymbolByIndex((*vct)[i]).c_str(), name) == 0) {
                    class_id_ = i;
                    return;
                }
            }

            vct->pushBack(getTopCell()->getOrCreateSymbol(name));
            class_id_ = vct->getSize() - 1;
        }
    }
}

/**
 * @brief
 * whether OTHERENDEOL is defined or not
 *
 * @return
 */
bool EOLKeepout::isOtherEndEOL() const {
    return other_end_eol_;
}

/**
 * @brief
 * set OTHERENDEOL
 */
void EOLKeepout::setOtherEndEOL() {
    other_end_eol_ = 1;
}

/**
 * @brief
 * whether CORNERONLY is defined or not
 *
 * @return
 */
bool EOLKeepout::isCornerOnly() const {
    return corner_only_;
}

/**
 * @brief
 * set CORNERONLY
 */
void EOLKeepout::setCornerOnly() {
    corner_only_ = 1;
}

/**
 * @brief
 * without EXCEPTFROM
 *
 * @return
 */
bool EOLKeepout::isFromBothEdge() const {
    return except_from_ == kEOLKeepoutFromBothEdge;
}

/**
 * @brief
 * from EXCEPTFROM BACKEDGE
 *
 * @return
 */
bool EOLKeepout::isFromBackEdge() const {
    return except_from_ == kEOLKeepoutFromBackEdge;
}

/**
 * @brief
 * from EXCEPTFROM FRONTEDGE
 *
 * @return
 */
bool EOLKeepout::isFromFrontEdge() const {
    return except_from_ == kEOLKeepoutFromFrontEdge;
}

/**
 * @brief
 */
void EOLKeepout::setFromBothEdge() {
    except_from_ = kEOLKeepoutFromBothEdge;
    has_except_from_ = 1;
}

/**
 * @brief
 * set FRONTEDGE
 */
void EOLKeepout::setFromFrontEdge() {
    except_from_ = kEOLKeepoutFromFrontEdge;
    has_except_from_ = 1;
}

/**
 * @brief
 * set BACKEDGE
 */
void EOLKeepout::setFromBackEdge() {
    except_from_ = kEOLKeepoutFromBackEdge;
    has_except_from_ = 1;
}

/**
 * @brief
 * get FORWARDGAP
 *
 * @return
 */
UInt32 EOLKeepout::getForwardGap() const {
    return forward_gap_;
}

/**
 * @brief
 * set FORWARDGAP
 *
 * @param gap
 */
void EOLKeepout::setForwardGap(UInt32 gap) {
    forward_gap_ = gap;
}

/**
 * @brief
 * get mask num
 *
 * @return
 */
UInt32 EOLKeepout::getMaskNum() const {
    return mask_num_;
}

/**
 * @brief
 * set mask num
 *
 * @param num
 */
void EOLKeepout::setMaskNum(UInt32 num) {
    mask_num_ = num;
}

/**
 * @brief
 * whether TWOSIDES is defined or not
 *
 * @return
 */
bool EOLKeepout::isTwoSides() const {
    return two_sides_;
}

/**
 * @brief
 * set TWOSIDES flag
 *
 * @param is_two_sides
 */
void EOLKeepout::setIsTwoSides(bool is_two_sides) {
    two_sides_ = is_two_sides ? 1 : 0;
}

/**
 * @brief
 * get DIFFMASK flag
 *
 * @return
 */
bool EOLKeepout::isDiffMask() const {
    return diff_mask_;
}

/**
 * @brief
 * set DIFFMASK flag
 *
 * @param is_diff_mask
 */
void EOLKeepout::setIsDiffMask(bool is_diff_mask) {
    diff_mask_ = is_diff_mask ? 1 : 0;
}

/**
 * @brief
 * get min width
 *
 * @return
 */
UInt32 EOLKeepout::getMinEOLWidth() const {
    return min_eol_width_;
}

/**
 * @brief
 * get max width
 *
 * @return
 */
UInt32 EOLKeepout::getMaxEOLWidth() const {
    return max_eol_width_;
}

/**
 * @brief
 * get the width
 *
 * @return
 */
UInt32 EOLKeepout::getEOLWidth() const {
    return min_eol_width_;
}

/**
 * @brief
 * set width range
 *
 * @param min_eol_width
 * @param max_eol_width
 */
void EOLKeepout::setWidthRange(UInt32 min_eol_width, UInt32 max_eol_width) {
    if (min_eol_width < max_eol_width)
        eol_width_range_ = 1;
    min_eol_width_ = min_eol_width;
    max_eol_width_ = max_eol_width;
}

/**
 * @brief
 * set width that triggers rule
 *
 * @param eol_width
 */
void EOLKeepout::setEOLWidth(UInt32 eol_width) {
    min_eol_width_ = eol_width;
}

/**
 * @brief
 * get backward extension in EXTENSION
 *
 * @return
 */
UInt32 EOLKeepout::getBackwardExt() const {
    return backward_ext_;
}

/**
 * @brief
 * get side extension in EXTENSION
 *
 * @return
 */
UInt32 EOLKeepout::getSideExt() const {
    return side_ext_;
}

/**
 * @brief
 * get forward extension in EXTENSION
 *
 * @return
 */
UInt32 EOLKeepout::getForwardExt() const {
    return forward_ext_;
}

/**
 * @brief
 * set backward, side and forward extension in EXTENSION
 *
 * @param backward
 * @param side
 * @param forward
 */
void EOLKeepout::setExtension(UInt32 backward, UInt32 side, UInt32 forward) {
    backward_ext_ = backward;
    side_ext_ = side;
    forward_ext_ = forward;
}

/**
 * @brief
 * get low side extension in EXCEPTWITHIN
 *
 * @return
 */
UInt32 EOLKeepout::getExceptWithinLowExt() const {
    return low_side_ext_;
}

/**
 * @brief
 * get high side extension in EXCEPTWITHIN
 *
 * @return
 */
UInt32 EOLKeepout::getExceptWithinHighExt() const {
    return high_side_ext_;
}

/**
 * @brief
 * set low and high side extension in EXCEPTWITHIN
 *
 * @param low
 * @param high
 */
void EOLKeepout::setExceptWithinLowHighExt(UInt32 low, UInt32 high) {
    low_side_ext_ = low;
    high_side_ext_ = high;
    except_within_ = 1;
}

/**
 * @brief
 * get backward extension in EXCEPTSAMESIDEMETAL
 *
 * @return
 */
UInt32 EOLKeepout::getExceptSameSideMetalBackwardExt() const {
    return backward_ext1_;
}

/**
 * @brief
 * get side extension in EXCEPTSAMESIDEMETAL
 *
 * @return
 */
UInt32 EOLKeepout::getExceptSameSideMetalSideExt() const {
    return side_ext1_;
}

/**
 * @brief
 * get forward extension in EXCEPTSAMESIDEMETAL
 *
 * @return
 */
UInt32 EOLKeepout::getExceptSameSideMetalForwardExt() const {
    return forward_ext1_;
}

/**
 * @brief
 * set backward, side and forward extension in EXCEPTSAMESIDEMETAL
 *
 * @param backward
 * @param side
 * @param forward
 */
void EOLKeepout::setExceptSameSideMetalExt(UInt32 backward, UInt32 side, UInt32 forward) {
    backward_ext1_ = backward;
    side_ext1_ = side;
    forward_ext1_ = forward;
    except_same_side_metal = 1;
}

/**
 * @brief
 * get backward extension in EXCEPTNOOPPSIDEMETAL
 *
 * @return
 */
UInt32 EOLKeepout::getExceptNoOppSideMetalBackwardExt() const {
    return backward_ext2_;
}

/**
 * @brief
 * get side extension in EXCEPTNOOPPSIDEMETAL
 *
 * @return
 */
UInt32 EOLKeepout::getExceptNoOppSideMetalSideExt() const {
    return side_ext2_;
}

/**
 * @brief
 * get forward extension in EXCEPTNOOPPSIDEMETAL
 *
 * @return
 */
UInt32 EOLKeepout::getExceptNoOppSideMetalForwardExt() const {
    return forward_ext2_;
}

/**
 * @brief
 * set backward, side, forward extension for EXCEPTNOOPPSIDEMETAL
 *
 * @param backward
 * @param side
 * @param forward
 */
void EOLKeepout::setExceptNoOppSideMetalExt(UInt32 backward, UInt32 side, UInt32 forward) {
    backward_ext2_ = backward;
    side_ext2_ = side;
    forward_ext2_ = forward;
    except_no_opp_side_metal_ = 1;
}

/**
 * @brief
 * get the spacing in EXCEPTLINEENDSPACING
 *
 * @return
 */
UInt32 EOLKeepout::getExceptLineEndSpacing() const {
    return except_line_end_gap_ ? 0 : except_line_end_sp_;
}

/**
 * @brief
 * set EXCEPTLINEENDSPACING spacing
 *
 * @param spacing
 */
void EOLKeepout::setExceptLineEndSpacing(UInt32 spacing) {
    except_line_end_sp_ = spacing;
    has_except_line_end_sp_ = 1;
}

/**
 * @brief
 * whether it is EXCEPTLINEENDGAP or not
 *
 * @return
 */
bool EOLKeepout::isExceptLineEndGap() const {
    return except_line_end_gap_;
}

/**
 * @brief
 * set EXCEPTLINEENDGAP
 *
 * @param isgap
 */
void EOLKeepout::setIsExceptLineEndGap(bool isgap) {
    except_line_end_gap_ = isgap ? 1 : 0;
}

/**
 * @brief
 * get spacing in EOLWITHINCUT
 *
 * @return
 */
UInt32 EOLKeepout::getEOLWithinCut() const {
    return eol_within_cut_sp_;
}

/**
 * @brief
 * set spacing in EOLWITHINCUT
 *
 * @param spacing
 */
void EOLKeepout::setEOLWithinCut(UInt32 spacing) {
    eol_within_cut_sp_ = spacing;
    eol_within_cut_ = 1;
}

/**
 * @brief
 * get eol spacing in EXCEPTEOLSPACING
 *
 * @return
 */
UInt32 EOLKeepout::getExceptEOLSpacing() const {
    return except_eol_sp_;
}

/**
 * @brief
 * set eol spacing in EXCEPTEOLSPACING
 *
 * @param spacing
 */
void EOLKeepout::setExceptEOLSpacing(UInt32 spacing) {
    except_eol_sp_ = spacing;
    has_eol_sp_ = 1;
}

/**
 * @brief
 * indicate whether it is except same metal or not
 *
 * @return
 */
bool EOLKeepout::isExceptSameMetal() const {
    return except_same_metal_;
}

/**
 * @brief
 * set EXCEPTSAMESIDEMETAL flag
 *
 * @param b
 */
void EOLKeepout::setIsExcetpSameMetal(bool b) {
    except_same_metal_ = b ? 1 : 0;
}

MinCut::~MinCut() {
}

/**
 * @brief
 * minimum number of cuts
 *
 * @return
 */
UInt32 MinCut::getNumCuts() const {
    return num_cuts_;
}

/**
 * @brief
 * set minimum number of cuts
 *
 * @param num
 */
void MinCut::setNumCuts(UInt32 num) {
    num_cuts_ = num;
}

/**
 * @brief
 * get the width triggering minimumcut rule
 *
 * @return
 */
UInt32 MinCut::getWidth() const {
    return width_;
}

/**
 * @brief
 * set minimumcut width
 *
 * @param w
 */
void MinCut::setWidth(UInt32 w) {
    width_ = w;
}

/**
 * @brief
 * get the enlarged cut distance
 *
 * @return
 */
UInt32 MinCut::getCutWithin() const {
    return cut_within_;
}

void MinCut::setCutWithin(UInt32 dist) {
    cut_within_ = dist;
}

/**
 * @brief
 * get all cut classes in current minimumcut rule
 *
 * @return
 */
ArrayObject<ObjectId>* MinCut::getCutClassArr() {
    if (cut_classes_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(cut_classes_);
        return array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * get all the number of cuts in current minimumcut rule
 *
 * @return
 */
 ArrayObject<UInt32>* MinCut::getCutClassNumCutsArr() {
    if (cc_num_cuts_ != 0) {
        ArrayObject<UInt32> *array = addr<ArrayObject<UInt32>>(cc_num_cuts_);
        return array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * add the pair of cutclass and numcuts
 *
 * @param cs
 * @param num
 */
void MinCut::appendCutClassCuts(ObjectId cs, UInt32 num) {
    if (cs && num) {
        ArrayObject<ObjectId> *vct = nullptr;
        if (cut_classes_ == 0) {
            vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
            if (vct == nullptr)
                return;
            cut_classes_ = vct->getId();
            vct->setPool(getOwnerCell()->getPool());
            vct->reserve(16);
        }
        else
        {
            vct = addr<ArrayObject<ObjectId>>(cut_classes_);
        }
        if (vct)
            vct->pushBack(cs);

        ArrayObject<UInt32> *array_ptr = nullptr;
        if (cc_num_cuts_ == 0) {
            array_ptr = getOwnerCell()->createObject<ArrayObject<UInt32>>(kObjectTypeArray);
            if (array_ptr == nullptr)
                return;
            array_ptr->setPool(getOwnerCell()->getPool());
            array_ptr->reserve(16);
            cc_num_cuts_ = array_ptr->getId();
        } else {
            array_ptr = addr<ArrayObject<UInt32>>(cc_num_cuts_);
        }

        if (array_ptr)
            array_ptr->pushBack(num);
    }
}

/**
 * @brief
 * get the length
 *
 * @return
 */
UInt32 MinCut::getLength() const {
    return is_length_within_ ? length_ : 0;
}

/**
 * @brief
 * set the length
 *
 * @param len
 */
void MinCut::setLength(UInt32 len) {
    is_length_within_ = 1;
    is_area_within_ = 0;
    length_ = len;
}

/**
 * @brief
 * get the value of length within
 *
 * @return
 */
UInt32 MinCut::getLengthWithin() const {
    return is_length_within_ ? area_length_within_ : 0;
}

/**
 * @brief
 * set the value of length within
 *
 * @param dist
 */
void MinCut::setLengthWithin(UInt32 dist) {
    area_length_within_ = dist;
}

/**
 * @brief
 * get area
 *
 * @return
 */
double MinCut::getArea() const {
    return is_area_within_ ? area_ : 0;
}

/**
 * @brief
 * set area
 *
 * @param area
 */
void MinCut::setArea(double area) {
    area_ = area;
    is_area_within_ = 1;
}

/**
 * @brief
 * get the value of area within
 *
 * @return
 */
UInt32 MinCut::getAreaWithin() const {
    return is_area_within_ ? area_length_within_ : 0;
}

/**
 * @brief
 * set the value of area within
 *
 * @param dist
 */
void MinCut::setAreaWithin(UInt32 dist) {
    area_length_within_ = dist;
}

/**
 * @brief
 * indicate whether is is fromabove or not
 *
 * @return
 */
bool MinCut::isFromAbove() const {
    return from_above_;
}

/**
 * @brief
 * set fromabove flag
 *
 * @param v
 */
void MinCut::setIsFromAbove(bool v) {
    from_above_ = v;
}

/**
 * @brief
 * indicate whether it is frombelow or not
 *
 * @return
 */
bool MinCut::isFromBelow() const {
    return from_below_;
}

/**
 * @brief
 * set frombelow flag
 *
 * @param v
 */
void MinCut::setIsFromBelow(bool v) {
    from_below_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether SAMEMETALOVERLAP is defined or not
 *
 * @return
 */
bool MinCut::isSameMetalOverlap() const {
    return same_metal_overlap_;
}

/**
 * @brief
 * set SAMEMETALOVERLAP flag
 *
 * @param v
 */
void MinCut::setIsSameMetalOverlap(bool v) {
    same_metal_overlap_ = v;
}

/**
 * @brief
 * indicate whether FULLYENCLOSED is defined or not
 *
 * @return
 */
bool MinCut::isFullyEnclosed() const {
    return fully_enclosed_;
}

/**
 * @brief
 * set FULLYENCLOSED flag
 *
 * @param v
 */
void MinCut::setIsFullyEnclosed(bool v) {
    fully_enclosed_ = v ? 1 : 0;
}

/**
 * @brief
 * get the minimum area size of the hole
 *
 * @return
 */
UInt32 MinEnclArea::getArea() const {
    return area_;
}

/**
 * @brief
 * set the minimum area size of the hole
 *
 * @param area
 */
void MinEnclArea::setArea(UInt32 area) {
    area_ = area;
}

/**
 * @brief
 * get wire width
 *
 * @return
 */
UInt32 MinEnclArea::getWidth() const {
    return width_;
}

/**
 * @brief
 * set wire width
 *
 * @param w
 */
void MinEnclArea::setWidth(UInt32 w) {
    width_ = w;
}

/**
 * @brief
 * deconstructor and release the allocated memory
 */
MinSize::~MinSize() {

}

/**
 * @brief
 * get the number of width and length pair
 *
 * @return
 */
UInt32 MinSize::getMinSizeNum() const {
    return minsize_num_;
}

/**
 * @brief
 * initialize minsize number
 *
 * @param num
 *
 * @return
 */
void MinSize::setMinSizeNum(UInt32 num) {
    minsize_num_ = num;
}

/**
 * @brief
 * get width
 *
 * @param idx
 *
 * @return
 */
UInt32 MinSize::getWidth(UInt32 idx) const {
    if (widths_ == 0) 
        return -1;
    ArrayObject<Int32> *array_ptr = addr< ArrayObject<Int32> >(widths_);
    if (array_ptr == nullptr)
        return -1;

    return (*array_ptr)[idx];
}

/**
 * @brief
 * get length
 *
 * @param idx
 *
 * @return
 */
UInt32 MinSize::getLength(UInt32 idx) const {
    if (idx >= minsize_num_) {
        return 0;
    }
    
    if (lengths_ == 0) 
        return -1;
    ArrayObject<Int32> *array_ptr = addr< ArrayObject<Int32> >(lengths_);
    if (array_ptr == nullptr)
        return -1;

    return (*array_ptr)[idx];
}

/**
 * @brief
 * indicate whether it has RECTONLY definition or not
 *
 * @return
 */
bool MinSize::isRectOnly() const {
    return is_rect_only_;
}

/**
 * @brief
 * set RECTONLY flag
 *
 * @param v
 *
 * @return
 */
void MinSize::setRectOnly(bool v) {
    is_rect_only_ = v;
}

/**
 * @brief
 * add the pair of width and length
 *
 * @param index
 * @param width
 * @param length
 *
 * @return
 */
void MinSize::addWidthLength(UInt32 width, UInt32 length) {
    ArrayObject<Int32> *array_ptr = nullptr;
    if (widths_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<Int32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        widths_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<Int32> >(widths_);
    }
    if (array_ptr) array_ptr->pushBack(width);

    if (lengths_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<Int32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        lengths_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<Int32> >(lengths_);
    }

    if (array_ptr) array_ptr->pushBack(length);

}


/**
 * @brief
 * indicate whether is is a MAXEDGES or not
 *
 * @return
 */
bool MinStep::isMaxEdges() const {
    return is_max_edges_;
}

/**
 * @brief
 * indicate whether is has INSIDECORNER flag or not
 *
 * @return
 */
bool MinStep::isInsideCorner() const {
    return inside_corner_;
}

/**
 * @brief
 * set INSIDECORNER flag
 *
 * @param v
 */
void MinStep::setIsInsideCorner(bool v) {
    inside_corner_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether rule has OUTSIDECORNER flag or not
 *
 * @return
 */
bool MinStep::isOutsideCorner() const {
    return outside_corner_;
}

/**
 * @brief
 * set OUTSIDECORNER flag
 *
 * @param v
 */
void MinStep::setIsOutsideCorner(bool v) {
    outside_corner_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether rule has STEP flag or not
 *
 * @return
 */
bool MinStep::isStep() const {
    return step_;
}

/**
 * @brief
 * set STEP flag
 *
 * @param v
 */
void MinStep::setIsStep(bool v) {
    step_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether rule is MINADJACENTLENGTH or not
 *
 * @return
 */
bool MinStep::isMinAdjLength() const {
    return is_min_adj_length_;
}

/**
 * @brief
 * indicate whether rule is MINADJACENTLENGTH CONVEXCORNER
 *
 * @return
 */
bool MinStep::isMinAdjLengthConvexCorner() const {
    return is_adj_convex_corner_;
}

/**
 * @brief
 * set MINADJACENTLENGTH CONVEXCORNER flag
 *
 * @param v
 */
void MinStep::setIsMinAdjLengthConvexCorner(bool v) {
    is_adj_convex_corner_ = v ? 1 : 0;
    if (v) {
        is_min_adj_length_ = 1;
    }
}

/**
 * @brief
 * indicate whether rule is MINADJACENTLENGTH CONCAVECORNER
 *
 * @return
 */
bool MinStep::isMinAdjLengthConcaveCorner() const {
    return is_adj_concave_corner_;
}

/**
 * @brief
 * set MINADJACENTLENGTH CONCAVECORNER flag
 *
 * @param v
 */
void MinStep::setIsMinAdjLengthConcaveCorner(bool v) {
    is_adj_concave_corner_ = v ? 1 : 0;
    if (v) {
        is_min_adj_length_ = 1;
    }
}

/**
 * @brief
 * indicate whether rule is MINADJACENTLENGTH THREECONCAVECORNERS
 *
 * @return
 */
bool MinStep::isMinAdjLengthThreeConcaveCorners() const {
    return is_three_concave_corners_;
}

/**
 * @brief
 * set MINADJACENTLENGTH THREECONCAVECORNERS
 *
 * @param v
 */
void MinStep::setIsMinAdjLengthThreeConcaveCorners(bool v) {
    is_three_concave_corners_ = v ? 1 : 0;
    if (v) {
        is_min_adj_length_ = 1;
    }
}

/**
 * @brief
 * indicate whether rule is MINADJACENTLENGTH minAdjLength2 or not
 *
 * @return
 */
bool MinStep::isMinAdjLength2() const {
    return is_min_adj_length2_;
}

/**
 * @brief
 * indicate whether rule is MINBETWEENLENGTH or not
 *
 * @return
 */
bool MinStep::isMinBetweenLength() const {
    return is_min_between_length_;
}

/**
 * @brief
 * indicate whether rule is MINBETWEENLENGTH EXCEPTSAMECORNERS or not
 *
 * @return
 */
bool MinStep::isExceptSameCorners() const {
    return is_except_same_corners_;
}

/**
 * @brief
 * set MINBETWEENLENGTH EXCEPTSAMECORNERS flag
 *
 * @param v
 */
void MinStep::setIsExceptSameCorners(bool v) {
    is_except_same_corners_ = v ? 1 : 0;
    if (v) {
        is_min_between_length_ = 1;
    }
}

/**
 * @brief
 * indicate whether rule is NOADJACENTEOL or not
 *
 * @return
 */
bool MinStep::isNoAdjEOL() const {
    return is_no_adj_eol_;
}

/**
 * @brief
 * indicat whether rule is NOADJACENTEOL EXCEPTADJACENTLENGTH or not
 *
 * @return
 */
bool MinStep::isNoAdjEOLExceptAdjLength() const {
    return is_no_adj_eol_except_adj_length_;
}

/**
 * @brief
 * indicate whether rule is NOADJACENTEOL MINADJACENTLENGTH or not
 *
 * @return
 */
bool MinStep::isNoAdjEOLMinAdjLength() const {
    return is_no_adj_eol_min_adj_length_;
}

/**
 * @brief
 * indicate whether rule has NOADJACENTEOL CONCAVECORNERS flag
 *
 * @return
 */
bool MinStep::isNoAdjEOLConcaveCorners() const {
    return is_no_adj_concave_corners_;
}

/**
 * @brief
 * set NOADJACENTEOL CONCAVECORNERS flag
 *
 * @param v
 */
void MinStep::setIsNoAdjEOLConcaveCorners(bool v) {
    is_no_adj_concave_corners_ = v ? 1 : 0;
    if (v) {
        is_no_adj_eol_ = 1;
    }
}

/**
 * @brief
 * indicate whether rule is NOBETWEENEOL or not
 *
 * @return
 */
bool MinStep::isNoBetweenEOL() const {
    return is_no_between_eol_;
}

/**
 * @brief
 * get min step length
 *
 * @return
 */
UInt32 MinStep::getMinStepLength() const {
    return min_step_length_;
}

/**
 * @brief
 * set min step length
 *
 * @param len
 */
void MinStep::setMinStepLength(UInt32 len) {
    min_step_length_ = len;
}

/**
 * @brief
 * get maximum length sum
 *
 * @return
 */
UInt32 MinStep::getMaxLength() const {
    return is_max_edges_ ? (INT_MAX >> 1) : max_length_;
}

/**
 * @brief
 * set maximum length sum
 *
 * @param len
 */
void MinStep::setMaxLength(UInt32 len) {
    max_length_ = len;
    is_max_edges_ = 0;
}

/**
 * @brief
 * get maximum edges
 *
 * @return
 */
UInt32 MinStep::getMaxEdges() const {
    return is_max_edges_ ? max_edges_ : 0;
}

/**
 * @brief
 * set maximum edges
 *
 * @param num
 */
void MinStep::setMaxEdges(UInt32 num) {
    max_edges_ = num;
    is_max_edges_ = 1;
}

/**
 * @brief
 * get MINADJACENTLENGTH minimum adjacent length
 *
 * @return
 */
UInt32 MinStep::getMinAdjLength() const {
    return is_min_adj_length_ ? min_adj_length_ : 0;
}

/**
 * @brief
 * set MINADJACENTLENGTH minimum adjacent length
 *
 * @param len
 */
void MinStep::setMinAdjLength(UInt32 len) {
    min_adj_length_ = len;
    is_min_adj_length_ = 1;
}

/**
 * @brief
 * get MINADJACENTLENGTH CONVEXCORNER EXCEPTWITHIN value
 *
 * @return
 */
UInt32 MinStep::getConvexExceptWithin() const {
    return is_adj_convex_corner_ ? except_within_ : 0;
}

/**
 * @brief
 * set MINADJACENTLENGTH CONVEXCORNER EXCEPTWITHIN value
 *
 * @param len
 */
void MinStep::setConvexExcetpWithin(UInt32 len) {
    except_within_ = len;
    is_adj_convex_corner_ = 1;
    is_min_adj_length_ = 1;
}

/**
 * @brief
 * get MINADJACENTLENGTH THREECONCAVECORNERS CENTERWIDTH value
 *
 * @return
 */
UInt32 MinStep::getCenterWidth() const {
    return is_three_concave_corners_ ? center_width_ : 0;
}

/**
 * @brief
 * set MINADJACENTLENGTH THREECONCAVECORNERS CENTERWIDTH value
 *
 * @param w
 */
void MinStep::setCenterWidth(UInt32 w) {
    center_width_ = w;
    is_min_adj_length_ = 1;
    is_three_concave_corners_ = 1;
}

/**
 * @brief
 * get MINADJACENTLENGTH minAdjLength2 value
 *
 * @return
 */
UInt32 MinStep::getMinAdjLength2() const {
    return is_min_adj_length2_ ? min_adj_length2_ : 0;
}

/**
 * @brief
 * set MINADJACENTLENGTH minAdjLength2 value
 *
 * @param len
 */
void MinStep::setMinAdjLength2(UInt32 len) {
    min_adj_length2_ = len;
    is_min_adj_length2_ = 1;
    is_min_adj_length_ = 1;
}

/**
 * @brief
 * get MINBETWEENLENGTH minBetweenLength value
 *
 * @return
 */
UInt32 MinStep::getMinBetweenLength() const {
    return is_min_between_length_ ? min_between_length_ : 0;
}

/**
 * @brief
 * set MINBETWEENLENGTH minBetweenLength value
 *
 * @param len
 *
 * @return
 */
void MinStep::setMinBetweenLength(UInt32 len) {
    min_between_length_ = len;
    is_min_between_length_ = 1;
}

/**
 * @brief
 * get NOADJACENTEOL eolWidth value
 *
 * @return
 */
UInt32 MinStep::getNoAdjEOLWidth() const {
    return is_no_adj_eol_ ? eol_width_ : 0;
}

/**
 * @brief
 * set NOADJACENTEOL eolWidth value
 *
 * @param w
 */
void MinStep::setNoAdjEOLWidth(UInt32 w) {
    eol_width_ = w;
    is_no_adj_eol_ = 1;
}

/**
 * @brief
 * get NOADJACENTEOL EXCEPTADJACENTLENGTH minAdjLength value
 *
 * @return
 */
UInt32 MinStep::getNoAdjExceptAdjLength() const {
    return is_no_adj_eol_min_adj_length_ ? no_adj_eol_except_adj_length_ : 0;
}

/**
 * @brief
 * set NOADJACENTEOL EXCEPTADJACENTLENGTH minAdjLength value
 *
 * @param len
 */
void MinStep::setNoAdjExceptAdjLength(UInt32 len) {
    no_adj_eol_except_adj_length_ = len;
    is_no_adj_eol_ = 1;
    is_no_adj_eol_except_adj_length_ = 1;
}

/**
 * @brief
 * get NOADJACENTEOL MINADJACENTLENGTH minAdjLength value
 *
 * @return
 */
UInt32 MinStep::getNoAdjEOLMinAdjLength() const {
    return is_no_adj_eol_min_adj_length_ ? no_adj_eol_min_adj_length_ : 0;
}

/**
 * @brief
 * set NOADJACENTEOL MINADJACENTLENGTH minAdjLength value
 *
 * @param len
 */
void MinStep::setNoAdjEOLMinAdjLength(UInt32 len) {
    no_adj_eol_min_adj_length_ = len;
    is_no_adj_eol_ = 1;
    is_no_adj_eol_min_adj_length_ = 1;
}

/**
 * @brief
 * get NOBETWEENEOL eolWidth value
 *
 * @return
 */
UInt32 MinStep::getNoBetweenEOLWidth() const {
    return is_no_between_eol_ ? eol_width_ : 0;
}

/**
 * @brief
 * set NOBETWEENEOL eolWidth value
 *
 * @param w
 */
void MinStep::setNoBetweenEOLWidth(UInt32 w) {
    eol_width_ = w;
    is_no_between_eol_ = 1;
}

/**
 * @brief
 * get width2 in PROTRUSIONWIDTH width1 WIDTH width2
 *
 * @return
 */
UInt32 ProtrusionRule::ProtrusionWidth::getWidth() const {
    return width_;
}

/**
 * @brief
 * set width2
 *
 * @param w
 */
void ProtrusionRule::ProtrusionWidth::setWidth(UInt32 w) {
    width_ = w;
}

/**
 * @brief
 * set MINSIZE minWidth minLength
 *
 * @param w
 * @param len
 */
void ProtrusionRule::ProtrusionWidth::setMinSizeWidthLength(UInt32 w, UInt32 len) {
    is_min_size_ = 1;
    is_min_size_min_length_ = is_min_length_ = 0;
    min_size_min_width_ = w;
    min_size_min_length_ = len;
}

/**
 * @brief
 * set MINSIZE minLength
 *
 * @param len
 */
void ProtrusionRule::ProtrusionWidth::setMinSizeMinLength(UInt32 len) {
    is_min_size_ = 1;
    is_min_size_min_length_ = 1;
    is_min_length_ = 0;
    // share same variable with MINSIZE minWidth
    min_size_min_width_ = len;
}

/**
 * @brief
 * set MINLENGTH minLength
 *
 * @param len
 */
void ProtrusionRule::ProtrusionWidth::setMinLength(UInt32 len) {
    is_min_size_ = is_min_size_min_length_ = 0;
    is_min_length_ = 1;
    min_length_min_length_ = len;
}

/**
 * @brief
 * get MINSIZE minWidth
 *
 * @return
 */
UInt32 ProtrusionRule::ProtrusionWidth::getMinWidth() const {
    return is_min_size_ ? min_size_min_width_ : 0;
}

/**
 * @brief
 * get min length for MINSIZE x minLength, MINSIZE minLength or MINLENGTH minLength
 *
 * @return
 */
UInt32 ProtrusionRule::ProtrusionWidth::getMinLength() const {
    if (is_min_size_) {
        return is_min_size_min_length_ ? min_size_min_width_ : min_size_min_length_;
    } else {
        return min_length_min_length_;
    }
}

/**
 * @brief
 * get cut distance in EXCEPTCUT cutDistance
 *
 * @return
 */
UInt32 ProtrusionRule::ProtrusionWidth::getCutDistance() const {
    return is_min_length_ ? min_length_cut_dist_ : 0;
}

/**
 * @brief
 * set cut distance
 *
 * @param d
 */
void ProtrusionRule::ProtrusionWidth::setCutDistance(UInt32 d) {
    min_length_cut_dist_ = d;
}

/**
 * @brief
 * get cut class in MINSIZE minLength CUTCLASS className
 *
 * @return
 */
ObjectId ProtrusionRule::ProtrusionWidth::getCutClass() const {
    return cut_class_;
}

/**
 * @brief
 * set cut class
 *
 * @param cs
 */
void ProtrusionRule::ProtrusionWidth::setCutClass(ObjectId cs) {
    cut_class_ = cs;
}

/**
 * @brief
 * indicate whether MINSIZE is defined or not
 *
 * @return
 */
bool ProtrusionRule::ProtrusionWidth::isMinSize() const {
    return is_min_size_;
}

/**
 * @brief
 * indicate whether MINSIZE minWidth minLength is used or not
 *
 * @return
 */
bool ProtrusionRule::ProtrusionWidth::isMinSizeMinWidth() const {
    return !is_min_size_min_length_;
}

/**
 * @brief
 * indicate whether FROMABOVE is defined or not
 *
 * @return
 */
bool ProtrusionRule::ProtrusionWidth::isFromAbove() const {
    return from_above_;
}

/**
 * @brief
 * set FROMABOVE flag
 *
 * @param v
 */
void ProtrusionRule::ProtrusionWidth::setIsFromAbove(bool v) {
    from_above_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether FROMBELOW is defined or not
 *
 * @return
 */
bool ProtrusionRule::ProtrusionWidth::isFromBelow() const {
    return from_below_;
}

/**
 * @brief
 * set FROMBELOW flag
 *
 * @param v
 */
void ProtrusionRule::ProtrusionWidth::setIsFromBelow(bool v) {
    from_below_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether neither FROMABOVE or FROMBELOW is defined
 *
 * @return
 */
bool ProtrusionRule::ProtrusionWidth::isFromBoth() const {
    return !from_above_ && !from_below_;
}

/**
 * @brief
 * get width1 in PROTRUSIONWIDTH width1
 *
 * @return
 */
UInt32 ProtrusionRule::getWidth1() const {
    return width1_;
}

/**
 * @brief
 * set width1
 *
 * @param w
 */
void ProtrusionRule::setWidth1(UInt32 w) {
    width1_ = w;
}

/**
 * @brief
 * get length in PROTRUSIONWIDTH x LENGTH length
 *
 * @return
 */
UInt32 ProtrusionRule::getLength() const {
    return length_;
}

/**
 * @brief
 * set length
 *
 * @param len
 */
void ProtrusionRule::setLength(UInt32 len) {
    length_ = len;
    if (len) {
        is_length_ = 1;
    }
}

/**
 * @brief
 * get width2
 *
 * @return
 */
UInt32 ProtrusionRule::getWidth2() const {
    return is_length_ ? width2_ : 0;
}

/**
 * @brief
 * set width2
 *
 * @param w
 */
void ProtrusionRule::setWidth2(UInt32 w) {
    width2_ = w;
}

/**
 * @brief
 * indicate whether LENGTH length WIDTH width2 is defined or not
 *
 * @return
 */
bool ProtrusionRule::isLength() const {
    return is_length_;
}

/**
 * @brief
 * set LENGTH flag
 *
 * @param v
 */
void ProtrusionRule::setIsLength(bool v) {
    is_length_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether PROTRUSIONWIDTH x WIDTH table is defined or not
 *
 * @return
 */
bool ProtrusionRule::isWidth() const {
    if (is_length_ == true) {
        return false;
    }
    if (protrusion_width_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(protrusion_width_);
        if (array->getSize() > 0)
            return true;
    }

    return false;
    //return is_length_ ? false : protrusion_width_->size();
}

/**
 * @brief
 * get PROTRUSIONWIDTH x WIDTH table
 *
 * @return
 */
ArrayObject<ObjectId>* ProtrusionRule::getProtrusionWidths() {

    if (protrusion_width_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(protrusion_width_);
        return array;
    } else {
        return nullptr;
    }
}

ProtrusionRule::ProtrusionWidth *ProtrusionRule::getProtrusionWidth(int i) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (protrusion_width_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(protrusion_width_);
    }
    if (vct) {
        ProtrusionRule::ProtrusionWidth *obj_data = addr<ProtrusionRule::ProtrusionWidth>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

/**
 * @brief
 * append one protrusionwidth
 *
 * @param pw
 */
void ProtrusionRule::addProtrusionWidth(ObjectId pw) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (protrusion_width_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        protrusion_width_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(protrusion_width_);
    }
    if (vct) vct->pushBack(pw);
}

WidthSpTbl::~WidthSpTbl() {
}

/**
 * @brief
 * indicate whether it is TWOWIDTHS rule or not
 *
 * @return
 */
bool WidthSpTbl::isTwoWidths() const {
    return is_two_widths_;
}

/**
 * @brief
 * set TWOWIDTHS flag
 *
 * @param v
 */
void WidthSpTbl::setIsWidthSpTbl(bool v) {
    is_two_widths_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether it is PARALLELRUNLENGTH WIDTH or not
 *
 * @return
 */
bool WidthSpTbl::isPRLWidth() const {
    return is_PRL_width_;
}

/**
 * @brief
 * set PARALLELRUNLENGTH WIDTH flag
 *
 * @param v
 */
void WidthSpTbl::setIsPRLWidth(bool v) {
    is_PRL_width_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether rule has WRONGDIRECTION definition or not
 *
 * @return
 */
bool WidthSpTbl::isWrongDir() const {
    return is_wrong_dir_;
}

/**
 * @brief
 * set WRONGDIRECTION flag
 *
 * @param v
 */
void WidthSpTbl::setIsWrongDir(bool v) {
    is_wrong_dir_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether rule has SAMEMASK flag or not
 *
 * @return
 */
bool WidthSpTbl::isSameMask() const {
    return is_same_mask_;
}

/**
 * @brief
 * set SAMEMASK flag
 *
 * @param v
 */
void WidthSpTbl::setIsSameMask(bool v) {
    is_same_mask_ = v ? 1 : 0;
}

UInt32 WidthSpTbl::getMaskNum() const {
    return is_same_mask_ ? 0 : mask_num_;
}

void WidthSpTbl::setMaskNum(UInt32 num) {
    if (num) {
        is_same_mask_ = 0;
        mask_num_ = num;
    }
}

UInt32 WidthSpTbl::getFillConcaveCornerLength() const {
    return is_two_widths_ ? fill_triangle_ : 0;
}

void WidthSpTbl::setFillConcaveCornerLength(UInt32 len) {
    fill_triangle_ = len;
}

/**
 * @brief get except eol width
 *
 * @return
 */
UInt32 WidthSpTbl::getEOLWidth() const {
    return eol_width_;
}

/**
 * @brief
 * set except eol width
 *
 * @param w
 */
void WidthSpTbl::setEOLWidth(UInt32 w) {
    eol_width_ = w;
}

/**
 * @brief
 * get the number of width rows in table
 *
 * @return
 */
UInt32 WidthSpTbl::getWidthDim() const {
    return width_num_;
}

/**
 * @brief
 * assign the count of rows in width table
 *
 * @param w_dim
 */
void WidthSpTbl::setWidthDim(UInt32 w_dim) {
    width_num_ = w_dim;
}

/**
 * @brief
 * get the number of length columns in table
 *
 * @return
 */
UInt32 WidthSpTbl::getPRLDim() const {
    return prl_num_;
}

/**
 * @brief
 * set PARALLELRUNLENGTH length dimension, for TWOWIDTHS PRL dimension, use width dim to set PRL dim
 *
 * @param prl_dim
 */
void WidthSpTbl::setPRLDim(UInt32 prl_dim) {
    prl_num_ = prl_dim;
}

/**
 * @brief 
 * indicate whether there is PRL on n-th WIDTH line for TWOWIDTHS
 *
 * @param index
 *
 * @return 
 */
bool WidthSpTbl::hasWidthPRL(UInt32 index) const {
    if (prls_ == 0) 
        return false;
    ArrayObject<UInt32> *array_ptr = addr< ArrayObject<UInt32> >(prls_);
    if (array_ptr == nullptr)
        return false;
    UInt32 index_value = (*array_ptr)[index];
    return (prls_ && index < prl_num_ && index_value != INT_MAX);
}

/**
 * @brief
 * get the width of the n-th row/column
 *
 * @param idx  row index for PARALLELRUNLENGTH, and row and colum index for TWOWIDTHS
 *
 * @return
 */
UInt32 WidthSpTbl::getWidth(UInt32 idx) const {
    if (idx >= width_num_) {
        return 0;
    }
    
    if (widths_ == 0) 
        return 0;
    ArrayObject<UInt32> *array_ptr = addr< ArrayObject<UInt32> >(widths_);
    if (array_ptr == nullptr)
        return 0;

    return (*array_ptr)[idx];
}

/**
 * @brief
 * set the width of the n-th row
 *
 * @param row_idx
 * @param w
 */
void WidthSpTbl::addWidth( UInt32 w) {
    ArrayObject<UInt32> *array_ptr = nullptr;
    if (widths_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<UInt32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        widths_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<UInt32> >(widths_);
    }

    if (array_ptr) array_ptr->pushBack(w);
}

/**
 * @brief
 * get n-th-row low and high exclude spacing
 *
 * @param row_idx
 * @param lo_spacing
 * @param hi_spacing
 */
void WidthSpTbl::getLoHiExclSpacing(UInt32 row_idx, UInt32* lo_spacing, UInt32* hi_spacing) const {
    if (lo_spacing) {
        if (row_idx >= width_num_) {
            *lo_spacing = 0;
        } else if (low_excl_spacing_ == 0) {
            *lo_spacing = 0;
        } else {
            ArrayObject<UInt32> *array_ptr = addr<ArrayObject<UInt32>>(low_excl_spacing_);
            if (array_ptr == nullptr) {
                *lo_spacing = 0;
            } else {
                *lo_spacing = row_idx < width_num_ ? (*array_ptr)[row_idx] : 0;
            }
        }

    }
    
    if (hi_spacing) {
        if (row_idx >= width_num_) {
            *hi_spacing = 0;
        } else if (high_excl_spacing_ == 0) {
            *hi_spacing = 0;
        } else {
            ArrayObject<UInt32> *array_ptr = addr<ArrayObject<UInt32>>(high_excl_spacing_);
            if (array_ptr == nullptr) {
                *hi_spacing = 0;
            } else {
                *hi_spacing = row_idx < width_num_ ? (*array_ptr)[row_idx] : 0;
            }
        }
        //*hi_spacing = row_idx < width_num_ ? high_excl_spacing_[row_idx] : 0;
    }
}

/**
 * @brief
 * set n-th-row low and high exclude spacing
 *
 * @param row_idx
 * @param lo_excl_sp
 * @param hi_excl_sp
 */
void WidthSpTbl::addLoHiExclSpacing(UInt32 lo_excl_sp, UInt32 hi_excl_sp) {
    ArrayObject<UInt32> *array_ptr = nullptr;
    if (low_excl_spacing_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<UInt32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        low_excl_spacing_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<UInt32> >(low_excl_spacing_);
    }
    if (array_ptr) array_ptr->pushBack(lo_excl_sp);

    if (high_excl_spacing_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<UInt32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        high_excl_spacing_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<UInt32> >(high_excl_spacing_);
    }
    if (array_ptr) array_ptr->pushBack(hi_excl_sp);
}

/**
 * @brief
 * get n-th-column prl for PARALLELRUNLENGTH  or get n-th-row prl for TWOWIDTHS
 *
 * @param idx   column index for PARALLELRUNLENGTH, and row index for TWOWIDTHS
 *
 * @return
 */
UInt32 WidthSpTbl::getPRL(UInt32 idx) const {
    if (prls_ == 0) 
        return 0;
    ArrayObject<UInt32> *array_ptr = addr< ArrayObject<UInt32> >(prls_);
    if (array_ptr == nullptr)
        return 0;

    return (idx < prl_num_ && (*array_ptr)[idx] != INT_MAX) ? (*array_ptr)[idx] : 0;
}

/**
 * @brief
 * set n-th-column prl
 *
 * @param row_col_idx
 * @param prl
 */
void WidthSpTbl::addPRL(UInt32 prl) {
    ArrayObject<UInt32> *array_ptr = nullptr;
    if (prls_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<UInt32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        prls_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<UInt32> >(prls_);
    }
    if (array_ptr) array_ptr->pushBack(prl);
}

/**
 * @brief
 * get spacing from the table
 *
 * @param row_idx
 * @param col_idx
 *
 * @return
 */
UInt32 WidthSpTbl::getSpacing(UInt32 row_idx, UInt32 col_idx) const {
    UInt32 spacing_index = row_idx * prl_num_ + col_idx;
    
    if (spacing_elems_ == 0) 
        return 0;
    ArrayObject<UInt32> *array_ptr = addr< ArrayObject<UInt32> >(spacing_elems_);
    if (array_ptr == nullptr)
        return 0;

    return spacing_index < spacing_num_ ? (*array_ptr)[spacing_index] : 0;

}

/**
 * @brief
 * set spacing
 *
 * @param row_idx
 * @param col_idx
 * @param sp
 */
void WidthSpTbl::setSpacing(UInt32 row_idx, UInt32 col_idx, UInt32 sp) {//handle different as 
    if (!(row_idx < width_num_ && col_idx < prl_num_)) {
        return;
    }

    ArrayObject<UInt32> *array_ptr = nullptr;
    if (spacing_elems_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<UInt32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        spacing_elems_ = array_ptr->getId();
        if (array_ptr) {
            //to allocate data first
            spacing_num_ = width_num_ * prl_num_;
            for (int i = 0; i < spacing_num_; i++)
                array_ptr->pushBack(0);
        }
    } else {
        array_ptr = addr< ArrayObject<UInt32> >(spacing_elems_);
    }
    (*array_ptr)[row_idx * prl_num_ + col_idx] = sp;

    // if (row_idx < width_num_ && col_idx < prl_num_) {
    //     if (!spacing_elems_) {
    //         spacing_num_ = width_num_ * prl_num_;
    //         spacing_elems_ = (UInt32*)calloc(spacing_num_, sizeof(UInt32));
    //     }
    //     spacing_elems_[row_idx * prl_num_ + col_idx] = sp;
    // }

}

/**
 * @brief
 * get minimum spacing in this spacing table
 *
 * @return
 */
UInt32 WidthSpTbl::getMinSpacing() const {
    return min_spacing_;
}

/**
 * @brief
 * set minimum spacing in this spacing table
 *
 * @param min_sp
 */
void WidthSpTbl::setMinSpacing(UInt32 min_sp) {
    min_spacing_ = min_sp;
}

/**
 * @brief
 * get maximum spacing in this spacing table
 *
 * @return
 */
UInt32 WidthSpTbl::getMaxSpacing() const {
    return max_spacing_;
}

/**
 * @brief
 * set maximum spacing in this spacing table
 *
 * @param max_sp
 */
void WidthSpTbl::setMaxSpacing(UInt32 max_sp) {
    max_spacing_ = max_sp;
}

/**
 * @brief
 * release allocated memory
 */
InfluenceSpTbl::~InfluenceSpTbl() {
}

/**
 * @brief
 * get width num
 *
 * @return
 */
UInt32 InfluenceSpTbl::getRowNum() const {
    return row_num_;
}

/**
 * @brief
 * set the number of width rows
 *
 * @param num
 */
void InfluenceSpTbl::setRowNum(UInt32 num) {
    row_num_ = num;
}

/**
 * @brief
 * get elements on n-th row
 *
 * @param row_idx
 * @param w
 * @param d
 * @param sp
 *
 * @return
 */
bool InfluenceSpTbl::getRowElems(UInt32 row_idx, UInt32& w, UInt32& d, UInt32& sp) const {
    if (row_idx < row_num_ && widths_) {
        if (widths_ == 0)
            return false;
        ArrayObject<UInt32> *array_ptr = addr<ArrayObject<UInt32>>(widths_);
        if (array_ptr == nullptr)
            return false;
        w = (*array_ptr)[row_idx];

        if (withins_ == 0)
            return false;
        array_ptr = addr<ArrayObject<UInt32>>(withins_);
        if (array_ptr == nullptr)
            return false;
        d = (*array_ptr)[row_idx];

        if (spacings_ == 0)
            return false;
        array_ptr = addr<ArrayObject<UInt32>>(spacings_);
        if (array_ptr == nullptr)
            return false;
        sp = (*array_ptr)[row_idx];

        return true;
    }

    return false;
}

/**
 * @brief
 * set elements on n-th row
 *
 * @param row_idx
 * @param w
 * @param d
 * @param sp
 */
void InfluenceSpTbl::addRowElems(UInt32 w, UInt32 d, UInt32 sp) {

    ArrayObject<UInt32> *array_ptr = nullptr;
    if (widths_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<UInt32>>(kObjectTypeArray);
        if (array_ptr == nullptr)
            return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);
        widths_ = array_ptr->getId();
    } else {
        array_ptr = addr<ArrayObject<UInt32>>(widths_);
    }
    if (array_ptr)
        array_ptr->pushBack(w);

    if (withins_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<UInt32>>(kObjectTypeArray);
        if (array_ptr == nullptr)
            return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);
        withins_ = array_ptr->getId();
    } else {
        array_ptr = addr<ArrayObject<UInt32>>(withins_);
    }
    if (array_ptr)
        array_ptr->pushBack(d);   

    if (spacings_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<UInt32>>(kObjectTypeArray);
        if (array_ptr == nullptr)
            return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);
        spacings_ = array_ptr->getId();
    } else {
        array_ptr = addr<ArrayObject<UInt32>>(spacings_);
    }
    if (array_ptr)
        array_ptr->pushBack(sp);  
}

/**
 * @brief
 * get width on the n-th row
 *
 * @param row_idx
 *
 * @return
 */
UInt32 InfluenceSpTbl::getWidth(UInt32 row_idx) const {
    if (widths_ == 0)
        return 0;
    ArrayObject<UInt32> *array_ptr = addr<ArrayObject<UInt32>>(widths_);
    if (array_ptr == nullptr)
        return 0;
    return (*array_ptr)[row_idx];
}

/**
 * @brief
 * get within distance on the n-th row
 *
 * @param row_idx
 *
 * @return
 */
UInt32 InfluenceSpTbl::getWithin(UInt32 row_idx) const {
    if (withins_ == 0)
        return 0;
    ArrayObject<UInt32> *array_ptr = addr<ArrayObject<UInt32>>(withins_);
    if (array_ptr == nullptr)
        return 0;
    return (*array_ptr)[row_idx];
}

/**
 * @brief
 * get the spacing on the n-th row
 *
 * @param row_idx
 *
 * @return
 */
UInt32 InfluenceSpTbl::getSpacing(UInt32 row_idx) const {
    if (spacings_ == 0)
        return 0;
    ArrayObject<UInt32> *array_ptr = addr<ArrayObject<UInt32>>(spacings_);
    if (array_ptr == nullptr)
        return 0;
    return (*array_ptr)[row_idx];
}

/**
 * @brief
 * release allocated memory
 */
ParaSpanLenTbl::~ParaSpanLenTbl() {
}

/**
 * @brief
 * release allocated array
 *
 * @param ptr
 */
void ParaSpanLenTbl::release(UInt32*& ptr) {
    if (ptr) {
        delete ptr;
        ptr = nullptr;
    }
}

/**
 * @brief
 * get table dimension
 *
 * @return
 */
UInt32 ParaSpanLenTbl::getDim() const {
    return dim_;
}

/**
 * @brief
 * set table dimension
 *
 * @param dim
 */
void ParaSpanLenTbl::setDim(UInt32 dim) {
    dim_ = dim;
}

/**
 * @brief
 * get parallel run length
 *
 * @return
 */
UInt32 ParaSpanLenTbl::getPRL() const {
    return prl_;
}

/**
 * @brief
 * set parallel run length
 *
 * @param prl
 */
void ParaSpanLenTbl::setPRL(UInt32 prl) {
    prl_ = prl;
}

/**
 * @brief
 * get the n-th-row span length
 *
 * @param row_idx
 *
 * @return
 */
UInt32 ParaSpanLenTbl::getSpanLength(UInt32 row_idx) const {
    if (row_idx >= dim_) {
        return 0;
    }
    
    if (span_lengths_ == 0) 
        return 0;
    ArrayObject<Int32> *array_ptr = addr< ArrayObject<Int32> >(span_lengths_);
    if (array_ptr == nullptr)
        return 0;

    return (*array_ptr)[row_idx];
}

/**
 * @brief
 * set the n-th-row span length
 *
 * @param row_idx
 * @param span_len
 */
void ParaSpanLenTbl::addSpanLength(UInt32 span_len) {
    ArrayObject<UInt32> *array_ptr = nullptr;
    if (span_lengths_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<UInt32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        span_lengths_ = array_ptr->getId();
    } else {
        array_ptr = addr< ArrayObject<UInt32> >(span_lengths_);
    }

    if (array_ptr) array_ptr->pushBack(span_len);
}

/**
 * @brief
 * get the spacing of the m-th row and the n-th column
 *
 * @param row_idx
 * @param col_idx
 *
 * @return
 */
UInt32 ParaSpanLenTbl::getSpacing(UInt32 row_idx, UInt32 col_idx) const {
    if (spacings_ == 0) 
        return 0;
    ArrayObject<UInt32> *array_ptr = addr< ArrayObject<UInt32> >(spacings_);
    if (array_ptr == nullptr)
        return 0;

    return (row_idx < dim_ && col_idx < dim_) ? (*array_ptr)[row_idx * dim_ + col_idx] : 0;
}

/**
 * @brief
 * set the spacing of the m-th row and the n-th column
 *
 * @param row_idx
 * @param col_idx
 * @param spacing
 */
void ParaSpanLenTbl::setSpacing(UInt32 row_idx, UInt32 col_idx, UInt32 spacing) {
    ArrayObject<UInt32> *array_ptr = nullptr;
    if (spacings_ == 0) {
        array_ptr = getOwnerCell()->createObject<ArrayObject<UInt32>>(kObjectTypeArray);
        if (array_ptr == nullptr) return;
        array_ptr->setPool(getOwnerCell()->getPool());
        array_ptr->reserve(16);        
        spacings_ = array_ptr->getId();
        if (array_ptr) {
            //to allocate data first
            int spacing_num_ = dim_*dim_;
            for (int i = 0; i < spacing_num_; i++)
                array_ptr->pushBack(0);
        }
    } else {
        array_ptr = addr< ArrayObject<UInt32> >(spacings_);
    }
    (*array_ptr)[row_idx * dim_ + col_idx] = spacing;
}

/**
 * @brief
 * check if current SPACING is the specified type
 *
 * @param type
 *
 * @return
 */
bool RoutingSpacing::isType_(SpacingType type) const {
    return type_ & (1 << type);
}

/**
 * @brief
 * set SPACING type
 *
 * @param type
 * @param v
 */
void RoutingSpacing::setIsType_(SpacingType type, bool v) {
    if (v)
        type_ |= (1 << type);
    else
        type_ &= ~(1 << type);
}

/**
 * @brief
 * get min spacing
 *
 * @return
 */
UInt32 RoutingSpacing::getMinSpacing() const {
    return min_spacing_;
}

/**
 * @brief
 * set min spacing
 *
 * @param sp
 */
void RoutingSpacing::setMinSpacing(UInt32 sp) {
    min_spacing_ = sp;
}

/**
 * @brief
 * indicate whether current SPACING is RANGE rule or not
 *
 * @return
 */
bool RoutingSpacing::isRange() const {
    return isType_(kRange);
}

/**
 * @brief
 * set range spacing flag
 *
 * @param v
 */
void RoutingSpacing::setIsRange(bool v) {
    setIsType_(kRange, v);
}

/**
 * @brief
 * indicate whether USELENGTHTHRESHOLD is defined or not
 *
 * @return
 */
bool RoutingSpacing::isRangeUseLengthThres() const {
    return isRange() ? sp_rule_.range_.is_use_len_thres_ : false;
}

/**
 * @brief
 * set USELENGTHTHRESHOLD flag
 *
 * @param v
 */
void RoutingSpacing::setIsRangeUseLenghthThres(bool v) {
    sp_rule_.range_.is_use_len_thres_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether it is RANGE INFLUENCE rule or not
 *
 * @return
 */
bool RoutingSpacing::isRangeInfluence() const {
    return isRange() ? sp_rule_.range_.is_influence_ : false;
}

/**
 * @brief
 * set RANGE INFLUENCE flag
 *
 * @param v
 */
void RoutingSpacing::setIsRangeInfluence(bool v) {
    sp_rule_.range_.is_influence_ = v ? 1 : 0;
}

/**
 * @brief 
 * indicate whether there is RNAGE after INFLUENCE or not
 *
 * @return 
 */
bool RoutingSpacing::isRangeInfluenceRange() const {
    return isRangeInfluence() ? sp_rule_.range_.is_influence_range_ : false;
}

/**
 * @brief 
 * set INFLUENCE RANGE flag
 *
 * @param v
 */
void RoutingSpacing::setIsRangeInfluenceRange(bool v) {
    sp_rule_.range_.is_influence_range_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether it is RANGE RANGE rule or not
 *
 * @return
 */
bool RoutingSpacing::isRangeRange() const {
    return isRange() ? sp_rule_.range_.is_range_ : false;
}

/**
 * @brief
 * set RANGE RANGE flag
 *
 * @param v
 */
void RoutingSpacing::setIsRangeRange(bool v) {
    sp_rule_.range_.is_range_ = v ? 1 : 0;
}

/**
 * @brief
 * get the min width in RANGE minWidth
 *
 * @return
 */
UInt32 RoutingSpacing::getRangeMinWidth() const {
    return isRange() ? sp_rule_.range_.min_width_ : 0;
}

/**
 * @brief
 * set RANGE min width
 *
 * @param w
 */
void RoutingSpacing::setRangeMinWidth(UInt32 w) {
    sp_rule_.range_.min_width_ = w;
}

/**
 * @brief
 * get RNAGE max width
 *
 * @return
 */
UInt32 RoutingSpacing::getRangeMaxWidth() const {
    return isRange() ? sp_rule_.range_.max_width_ : 0;
}

/**
 * @brief
 * set RANGE max width
 *
 * @param w
 */
void RoutingSpacing::setRangeMaxWidth(UInt32 w) {
    sp_rule_.range_.max_width_ = w;
}

/**
 * @brief
 * set RANGE minWidth maxWidth
 *
 * @param min_w
 * @param max_w
 */
void RoutingSpacing::setRangeMinMaxWidth(UInt32 min_w, UInt32 max_w) {
    sp_rule_.range_.min_width_ = min_w;
    sp_rule_.range_.max_width_ = max_w;
}

/**
 * @brief
 * get RANGE INFLUENCE value
 *
 * @return
 */
UInt32 RoutingSpacing::getRangeInfluenceValue() const {
    return isRange() ? sp_rule_.range_.influence_value_ : 0;
}

/**
 * @brief
 * set RANGE INFLUENCE value
 *
 * @param v
 */
void RoutingSpacing::setRangeInfluenceValue(UInt32 v) {
    sp_rule_.range_.influence_value_ = v;
}

/**
 * @brief
 * get influence min stub width
 *
 * @return
 */
UInt32 RoutingSpacing::getRangeStubMinWidth() const {
    return isRange() ? sp_rule_.range_.stub_min_width_ : 0;
}

/**
 * @brief
 * set influence min stub width
 *
 * @param w
 */
void RoutingSpacing::setRangeInfluenceMinWidth(UInt32 w) {
    sp_rule_.range_.stub_min_width_ = w;
}

/**
 * @brief
 * get influence max stub width
 *
 * @return
 */
UInt32 RoutingSpacing::getRangeStubMaxWidth() const {
    return isRange() ? sp_rule_.range_.stub_max_width_ : 0;
}

/**
 * @brief
 * set influence max stub width
 *
 * @param w
 */
void RoutingSpacing::setRangeInfluenceMaxWidth(UInt32 w) {
    sp_rule_.range_.stub_max_width_ = w;
}

/**
 * @brief
 * get RANGE RANGE min width
 *
 * @return
 */
UInt32 RoutingSpacing::getRangeRangeMinWidth() const {
    return isRange() ? sp_rule_.range_.range_min_width_ : 0;
}

/**
 * @brief
 * set RANGE RANGE min width
 *
 * @param w
 */
void RoutingSpacing::setRangeRangeMinWidth(UInt32 w) {
    sp_rule_.range_.range_min_width_ = w;
}

/**
 * @brief
 * get RANGE RANGE max width
 *
 * @return
 */
UInt32 RoutingSpacing::getRangeRangeMaxWidth() const {
    return isRange() ? sp_rule_.range_.range_max_width_ : 0;
}

/**
 * @brief
 * set RANGE RANGE max width
 *
 * @param w
 */
void RoutingSpacing::setRangeRangeMaxWidth(UInt32 w) {
    sp_rule_.range_.range_max_width_ = w;
}

/**
 * @brief
 * set RANGE RANGE minWidth maxWidth
 *
 * @param min_w
 * @param max_w
 */
void RoutingSpacing::setRangeRangeMinMaxWidth(UInt32 min_w, UInt32 max_w) {
    sp_rule_.range_.range_min_width_ = min_w;
    sp_rule_.range_.range_max_width_ = max_w;
}

/**
 * @brief
 * indicate whether it is LENGTHTHRESHOLD rule or not
 *
 * @return
 */
bool RoutingSpacing::isLengthThreshold() const {
    return isType_(kLengthThres);
}

bool RoutingSpacing::isLengthThresholdRange() const {
    if (isLengthThreshold()) {
        if (sp_rule_.len_thres_.min_width_ != sp_rule_.len_thres_.max_width_ ||
           sp_rule_.len_thres_.max_width_ != 0)
      return true;
    }
    return false;
}

/**
 * @brief
 * set LENGTHTHRESHOLD flag
 *
 * @param v
 */
void RoutingSpacing::setIsLengthThreshold(bool v) {
    setIsType_(kLengthThres, v);
}

/**
 * @brief
 * get LENGTHTHRESHOLD max length
 *
 * @return
 */
UInt32 RoutingSpacing::getLenThresMaxLength() const {
    return isLengthThreshold() ? sp_rule_.len_thres_.max_length_ : 0;
}

/**
 * @brief
 * set LENGTHTHRESHOLD max length
 *
 * @param len
 */
void RoutingSpacing::setLenThresMaxLength(UInt32 len) {
    sp_rule_.len_thres_.max_length_ = len;
}

/**
 * @brief
 * get LENGTHTHRESHOLD min width
 *
 * @return
 */
UInt32 RoutingSpacing::getLenThresMinWidth() const {
    return isLengthThreshold() ? sp_rule_.len_thres_.min_width_ : 0;
}

/**
 * @brief
 * set LENGTHTHRESHOLD min width
 *
 * @param w
 */
void RoutingSpacing::setLenThresMinWidth(UInt32 w) {
    sp_rule_.len_thres_.min_width_ = w;
}

/**
 * @brief
 * get LENGTHTHRESHOLD max width
 *
 * @return
 */
UInt32 RoutingSpacing::getLenThresMaxWidth() const {
    return isLengthThreshold() ? sp_rule_.len_thres_.max_width_ : 0;
}

/**
 * @brief
 * set LENGTHTHRESHOLD max width
 *
 * @param w
 */
void RoutingSpacing::setLenThresMaxWidth(UInt32 w) {
    sp_rule_.len_thres_.max_width_ = w;
}

/**
 * @brief
 * set LENGTHTHRESHOLD min and max width
 *
 * @param min_w
 * @param max_w
 */
void RoutingSpacing::setLenThresMinMaxWidth(UInt32 min_w, UInt32 max_w) {
    sp_rule_.len_thres_.min_width_ = min_w;
    sp_rule_.len_thres_.max_width_ = max_w;
}

/**
 * @brief
 * indicate whether it is ENDOFLINE rule or not
 *
 * @return
 */
bool RoutingSpacing::isEndOfLine() const {
    return isType_(kEndOfLine);
}

/**
 * @brief
 * set ENDOFLINE flag
 *
 * @param v
 */
void RoutingSpacing::setIsEndOfLine(bool v) {
    setIsType_(kEndOfLine, v);
}

/**
 * @brief
 * get eol width
 *
 * @return
 */
UInt32 RoutingSpacing::getEOLWidth() const {
    return isEndOfLine() ? sp_rule_.eol_.eol_width_ : 0;
}

/**
 * @brief
 * set eol width
 *
 * @param w
 */
void RoutingSpacing::setEOLWidth(UInt32 w) {
    sp_rule_.eol_.eol_width_ = w;
}

/**
 * @brief
 * get eol within
 *
 * @return
 */
UInt32 RoutingSpacing::getEOLWithin() const {
    return isEndOfLine() ? sp_rule_.eol_.eol_within_ : 0;
}

/**
 * @brief
 * set eol within
 *
 * @param w
 */
void RoutingSpacing::setEOLWithin(UInt32 w) {
    sp_rule_.eol_.eol_within_ = w;
}

/**
 * @brief
 * set eol parallel edge space
 *
 * @return
 */
UInt32 RoutingSpacing::getEOLPRLEdgeSpace() const {
    return isEndOfLine() ? sp_rule_.eol_.prl_edge_space_ : 0;
}

/**
 * @brief
 * get eol parallel edge space
 *
 * @param sp
 */
void RoutingSpacing::setEOLPRLEdgeSpace(UInt32 sp) {
    sp_rule_.eol_.prl_edge_space_ = sp;
}

/**
 * @brief
 * get eol parallel edge within
 *
 * @return
 */
UInt32 RoutingSpacing::getEOLPRLEdgeWithin() const {
    return isEndOfLine() ? sp_rule_.eol_.prl_edge_within_ : 0;
}

/**
 * @brief
 * set eol parallel edge within
 *
 * @param sp
 */
void RoutingSpacing::setEOLPRLEdgeWithin(UInt32 sp) {
    sp_rule_.eol_.prl_edge_within_ = sp;
}

/**
 * @brief
 * indicate whether TWOEDGES is specified or not
 *
 * @return
 */
bool RoutingSpacing::isEOLTwoEdges() const {
    return isEndOfLine() ? sp_rule_.eol_.two_edges_ : false;
}

/**
 * @brief
 * set TWOEDGES flag
 *
 * @param v
 */
void RoutingSpacing::setIsEOLTwoEdges(bool v) {
    sp_rule_.eol_.two_edges_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether it is SAMENET rule or not
 *
 * @return
 */
bool RoutingSpacing::isSameNet() const {
    return isType_(kSameNet);
}

/**
 * @brief
 * set SAMENET flag
 *
 * @param v
 */
void RoutingSpacing::setIsSameNet(bool v) {
    setIsType_(kSameNet, v);
}

/**
 * @brief
 * indicate whether SAMENET has PGONLY attribute or not
 *
 * @return
 */
bool RoutingSpacing::isSameNetPGOnly() const {
    return isSameNet() ? is_same_net_pg_only_ : false;
}

/**
 * @brief
 * set SAMENET PGONLY flag
 *
 * @param v
 */
void RoutingSpacing::setIsSameNetPGOnly(bool v) {
    is_same_net_pg_only_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether it is NOTCHLENGTH rule or not
 *
 * @return
 */
bool RoutingSpacing::isNotchLength() const {
    return isType_(kNotchLength);
}

/**
 * @brief
 * set NOTCHLENGTH flag
 *
 * @param v
 */
void RoutingSpacing::setIsNotchLength(bool v) {
    setIsType_(kNotchLength, v);
}

/**
 * @brief
 * get min notch length
 *
 * @return
 */
UInt32 RoutingSpacing::getNotchLengthMinNotchLength() const {
    return isNotchLength() ? sp_rule_.notch_length_.min_notch_len_ : 0;
}

/**
 * @brief
 * set min notch length
 *
 * @param len
 */
void RoutingSpacing::setNotchLengthMinNotchLength(UInt32 len) {
    sp_rule_.notch_length_.min_notch_len_ = len;
}

/**
 * @brief
 * get low excluded spacing
 *
 * @return
 */
UInt32 RoutingSpacing::getNotchLengthLowExclSpacing() const {
    return isNotchLength() ? sp_rule_.notch_length_.low_excl_spacing_ : 0;
}

/**
 * @brief
 * set low excluded spacing
 *
 * @param len
 */
void RoutingSpacing::setNotchLengthLowExclSpacing(UInt32 len) {
    sp_rule_.notch_length_.low_excl_spacing_ = len;
}

/**
 * @brief
 * get high excluded spacing
 *
 * @return
 */
UInt32 RoutingSpacing::getNotchLengthHighExclSpacing() const {
    return isNotchLength() ? sp_rule_.notch_length_.high_excl_spacing_ : 0;
}

/**
 * @brief
 * set high excluded spacing
 *
 * @param hi
 */
void RoutingSpacing::setNotchLengthHighExclSpacing(UInt32 hi) {
    sp_rule_.notch_length_.high_excl_spacing_ = hi;
}

/**
 * @brief
 * set low and high excluded spacings
 *
 * @param lo
 * @param hi
 */
void RoutingSpacing::setNotchLengthLowHighExclSpacing(UInt32 lo, UInt32 hi) {
    sp_rule_.notch_length_.low_excl_spacing_ = lo;
    sp_rule_.notch_length_.high_excl_spacing_ = hi;
    if (lo || hi) {
        is_lef58_notch_length_ = 1;
    }
}

/**
 * @brief
 * get notch within distance
 *
 * @return
 */
UInt32 RoutingSpacing::getNotchLengthWithin() const {
    return (isNotchLength() && isNotchLengthWithin()) ? sp_rule_.notch_length_.within_ : 0;
}

/**
 * @brief
 * set notch within distance
 *
 * @param dist
 */
void RoutingSpacing::setNotchLengthWithin(UInt32 dist) {
    sp_rule_.notch_length_.within_ = dist;
}

/**
 * @brief
 * get the side of notch span length
 *
 * @return
 */
UInt32 RoutingSpacing::getNotchLengthSpanLength() const {
    return (isNotchLength() && isNotchLengthWidth()) ? sp_rule_.notch_length_.span_length_ : 0;
}

/**
 * @brief
 * set the side of notch span length
 *
 * @param len
 */
void RoutingSpacing::setNotchLengthSpanLength(UInt32 len) {
    sp_rule_.notch_length_.span_length_ = len;
}

/**
 * @brief
 * get the side of notch width
 *
 * @return
 */
UInt32 RoutingSpacing::getNotchLengthSideNotchWidth() const {
    return (isNotchLength() && (isNotchLengthWidth() || isNotchLengthConcaveEnds())) ?
           sp_rule_.notch_length_.side_of_notch_width_ : 0;
}

/**
 * @brief
 * set the side of notch width
 *
 * @param w
 */
void RoutingSpacing::setNotchLengthSideNotchWidth(UInt32 w) {
    sp_rule_.notch_length_.side_of_notch_width_ = w;
}

/**
 * @brief
 * get notch width
 *
 * @return
 */
UInt32 RoutingSpacing::getNotchLengthNotchWidth() const {
    return (isNotchLength() && isNotchLengthNotchWidth()) ?
           sp_rule_.notch_length_.notch_width_ : 0;
}

/**
 * @brief
 * set notch width
 *
 * @param w
 */
void RoutingSpacing::setNotchLengthNotchWidth(UInt32 w) {
    sp_rule_.notch_length_.notch_width_ = 0;
}

/**
 * @brief
 * indicate whether it is NOTCHLENGTH WITHIN or not
 *
 * @return
 */
bool RoutingSpacing::isNotchLengthWithin() const {
    return isNotchLength() ? sp_rule_.notch_length_.is_within_ : false;
}

/**
 * @brief
 * set NOTCHLENGTH WITHIN flag
 *
 * @param v
 */
void RoutingSpacing::setIsNotchLengthWithin(bool v) {
    sp_rule_.notch_length_.is_within_ = v ? 1 : 0;
    if (v) {
        is_lef58_notch_length_ = 1;
    }
}

bool RoutingSpacing::isLef58NotchLength() const {
    return is_lef58_notch_length_;
}

/**
 * @brief
 * indicate whether it is NOTCHLENGTH WIDTH or not
 *
 * @return
 */
bool RoutingSpacing::isNotchLengthWidth() const {
    return isNotchLength() ? sp_rule_.notch_length_.is_width_ : false;
}

/**
 * @brief
 * set NOTCHLENGTH WIDTH flag
 *
 * @param v
 */
void RoutingSpacing::setIsNotchLengthWidth(bool v) {
    sp_rule_.notch_length_.is_width_ = v ? 1 : 0;
    if (v) {
        is_lef58_notch_length_ = 1;
    }
}

/**
 * @brief
 * indicate whether it is NOTCHLENGTH CONCAVEENDS or not
 *
 * @return
 */
bool RoutingSpacing::isNotchLengthConcaveEnds() const {
    return isNotchLength() ? sp_rule_.notch_length_.is_concave_ends_ : false;
}

/**
 * @brief
 * set NOTCHLENGTH CONCAVEENDS flag
 *
 * @param v
 */
void RoutingSpacing::setIsNotchLengthConcaveEnds(bool v) {
    sp_rule_.notch_length_.is_concave_ends_ = v ? 1 : 0;
    if (v) {
        is_lef58_notch_length_ = 1;
    }
}

/**
 * @brief
 * indicate whether it is NOTCHWIDTH or not
 *
 * @return
 */
bool RoutingSpacing::isNotchLengthNotchWidth() const {
    return isNotchLength() ? sp_rule_.notch_length_.is_notch_width_ : false;
}

/**
 * @brief
 * set NOTCHWIDTH flag
 *
 * @param v
 */
void RoutingSpacing::setIsNotchLengthNotchWidth(bool v) {
    sp_rule_.notch_length_.is_notch_width_ = v ? 1 : 0;
    if (v) {
        is_lef58_notch_length_ = 1;
    }
}

/**
 * @brief
 * indicate whether its ENDOFNOTCHWIDTH rule or not
 *
 * @return
 */
bool RoutingSpacing::isEndOfNotchWidth() const {
    return isType_(kEndOfNotchWidth);
}

/**
 * @brief
 * set ENDOFNOTCHWIDTH flag
 *
 * @param v
 */
void RoutingSpacing::setIsEndOfNotchWidth(bool v) {
    setIsType_(kEndOfNotchWidth, v);
}

/**
 * @brief
 * get the width of end of notch
 *
 * @return
 */
UInt32 RoutingSpacing::getEONWidth() const {
    return isEndOfNotchWidth() ? sp_rule_.eon_width_.eon_width_ : 0;
}

/**
 * @brief
 * set width of end of notch
 *
 * @param w
 */
void RoutingSpacing::setEONWidth(UInt32 w) {
    sp_rule_.eon_width_.eon_width_ = w;
}

/**
 * @brief
 * get notch spacing
 *
 * @return
 */
UInt32 RoutingSpacing::getEONSpacing() const {
    return isEndOfNotchWidth() ? sp_rule_.eon_width_.notch_spacing_ : 0;
}

/**
 * @brief
 * set notch spacing
 *
 * @param sp
 */
void RoutingSpacing::setEONSpacing(UInt32 sp) {
    sp_rule_.eon_width_.notch_spacing_ = sp;
}

/**
 * @brief
 * get notch length
 *
 * @return
 */
UInt32 RoutingSpacing::getEONLength() const {
    return isEndOfNotchWidth() ? sp_rule_.eon_width_.min_notch_length_ : 0;
}

/**
 * @brief
 * set notch length
 *
 * @param len
 */
void RoutingSpacing::setEONLength(UInt32 len) {
    sp_rule_.eon_width_.min_notch_length_ = len;
}

/**
 * @brief
 * indicate whether it is EOLPERPENDICULAR rule or not
 *
 * @return
 */
bool RoutingSpacing::isEOLPerp() const {
    return isType_(kEOLPerp);
}

/**
 * @brief
 * set EOLPERPENDICULAR flag
 *
 * @param v
 */
void RoutingSpacing::setIsEOLPerp(bool v) {
    setIsType_(kEOLPerp, v);
}

/**
 * @brief
 * get eol width
 *
 * @return
 */
UInt32 RoutingSpacing::getEOLPerpEolWidth() const {
    return isEOLPerp() ? sp_rule_.eol_perp_.eol_width_ : 0;
}

/**
 * @brief
 * set eol width
 *
 * @param w
 */
void RoutingSpacing::setEOLPerpEolWidth(UInt32 w) {
    sp_rule_.eol_perp_.eol_width_ = w;
}

/**
 * @brief
 * get perpendicaular width
 *
 * @return
 */
UInt32 RoutingSpacing::getEOLPerpPerpWidth() const {
    return isEOLPerp() ? sp_rule_.eol_perp_.per_width_ : 0;
}

/**
 * @brief
 * set perpendicaular width
 *
 * @param w
 */
void RoutingSpacing::setEOLPerpPerpWidth(UInt32 w) {
    sp_rule_.eol_perp_.per_width_ = w;
}

/**
 * @brief
 * indicate whether it is SPACING AREA rule or not
 *
 * @return
 */
bool RoutingSpacing::isArea() const {
    return isType_(kMaxArea);
}

/**
 * @brief
 * set SPACING AREA flag
 *
 * @param v
 */
void RoutingSpacing::setIsArea(bool v) {
    setIsType_(kMaxArea, v);
}

/**
 * @brief
 * get area
 *
 * @return
 */
double RoutingSpacing::getArea() const {
    return isArea() ? sp_rule_.area_ : 0;
}

/**
 * @brief
 * set area
 *
 * @param area
 */
void RoutingSpacing::setArea(double area) {
    sp_rule_.area_ = area;
}

/**
 * @brief
 * indicate whether it is SPACING LAYER rule or not
 *
 * @return
 */
bool RoutingSpacing::isTrimLayerSpacing() const {
    return isType_(kTrimLayer);
}

/**
 * @brief
 * set SPACING LAYER flag
 *
 * @param v
 */
void RoutingSpacing::setIsTrimLayerSpacing(bool v) {
    setIsType_(kTrimLayer, v);
}

/**
 * @brief
 * indicate whether it is SPACING SAMEMASK rule or not
 *
 * @return
 */
bool RoutingSpacing::isSameMask() const {
    return isType_(kSameMask);
}

/**
 * @brief
 * set SPACING SAMEMASK flag
 *
 * @param v
 */
void RoutingSpacing::setIsSameMask(bool v) {
    setIsType_(kSameMask, v);
}

/**
 * @brief
 * indicate whether it is SPACING WRONGDIRECTION rule or not
 *
 * @return
 */
bool RoutingSpacing::isWrongDir() const {
    return isType_(kWrongDir);
}

/**
 * @brief
 * set SPACING WRONGDIRECTION flag
 *
 * @param v
 */
void RoutingSpacing::setIsWrongDir(bool v) {
    setIsType_(kWrongDir, v);
}

/**
 * @brief
 * get non-eol width
 *
 * @return
 */
UInt32 RoutingSpacing::getWrongDirEolWidth() const {
    return isWrongDir() ? sp_rule_.wrong_dir_.eol_width_ : 0;
}

/**
 * @brief
 * set non-eol width
 *
 * @param w
 */
void RoutingSpacing::setWrongDirEolWidth(UInt32 w) {
    sp_rule_.wrong_dir_.eol_width_ = w;
}

/**
 * @brief
 * get parallel run length
 *
 * @return
 */
UInt32 RoutingSpacing::getWrongDirPRL() const {
    return isWrongDir() ? sp_rule_.wrong_dir_.prl_ : 0;
}

/**
 * @brief
 * set parallel run length
 *
 * @param prl
 */
void RoutingSpacing::setWrongDirPRL(UInt32 prl) {
    sp_rule_.wrong_dir_.prl_ = prl;
}

/**
 * @brief
 * get length
 *
 * @return
 */
UInt32 RoutingSpacing::getWrongDirLength() const {
    return isWrongDir() ? sp_rule_.wrong_dir_.length_ : 0;
}

/**
 * @brief
 * set length
 *
 * @param len
 */
void RoutingSpacing::setWrongDirLength(UInt32 len) {
    sp_rule_.wrong_dir_.length_ = len;
}

/**
 * @brief
 * indicate whether it is NOTCHSPAN rule or not
 *
 * @return
 */
bool RoutingSpacing::isNotchSpan() const {
    return isType_(kNotchSpan);
}

/**
 * @brief
 * set NOTCHSPAN flag
 *
 * @param v
 */
void RoutingSpacing::setIsNotchSpan(bool v) {
    setIsType_(kNotchSpan, v);
}

/**
 * @brief
 * get notch span
 *
 * @return
 */
UInt32 RoutingSpacing::getNotchSpanSpan() const {
    return isNotchSpan() ? sp_rule_.notch_span_.span_ : 0;
}

/**
 * @brief
 * set notch span
 *
 * @param span
 */
void RoutingSpacing::setNotchSpanSpan(UInt32 span) {
    sp_rule_.notch_span_.span_ = span;
}

/**
 * @brief
 * get notch spacing
 *
 * @return
 */
UInt32 RoutingSpacing::getNotchSpanSpacing() const {
    return isNotchSpan() ? sp_rule_.notch_span_.spacing_ : 0;
}

/**
 * @brief
 * set notch spacing
 *
 * @param sp
 */
void RoutingSpacing::setNotchSpanSpacing(UInt32 sp) {
    sp_rule_.notch_span_.spacing_ = sp;
}

/**
 * @brief
 * get notch length
 *
 * @return
 */
UInt32 RoutingSpacing::getNotchSpanLength() const {
    return isNotchSpan() ? sp_rule_.notch_span_.length_ : 0;
}

/**
 * @brief
 * set notch length
 *
 * @param len
 */
void RoutingSpacing::setNotchSpanLength(UInt32 len) {
    sp_rule_.notch_span_.length_ = len;
}

/**
 * @brief
 * indicate whether it is CONVEXCORNERS rule or not
 *
 * @return
 */
bool RoutingSpacing::isConvexCorners() const {
    return isType_(kConvexCorners);
}

/**
 * @brief
 * set CONVEXCORNERS flag
 *
 * @param v
 */
void RoutingSpacing::setIsConvexCorners(bool v) {
    setIsType_(kConvexCorners, v);
}

/**
 * @brief
 * get next SPACING rule
 *
 * @return
 */

RoutingLayerRule::~RoutingLayerRule() {
}

/**
 * @brief isVert
 * indicate whether routing layer is vertical or not
 *
 * @return
 */
bool RoutingLayerRule::isVert() const {
    return pref_direction_ == kVert;
}

/**
 * @brief isHoriz
 * indicate whether routing layer is horizontal or not
 *
 * @return
 */
bool RoutingLayerRule::isHoriz() const {
    return pref_direction_ == kHoriz;
}

/**
 * @brief isDiag45
 * indicate whether routing layer is 45-degree or not
 *
 * @return
 */
bool RoutingLayerRule::isDiag45() const {
    return pref_direction_ == kDiag45;
}

/**
 * @brief isDiag135
 * indicate whether routing layer is 135-degree or not
 *
 * @return
 */
bool RoutingLayerRule::isDiag135() const {
    return pref_direction_ == kDiag135;
}

/**
 * @brief setDirection
 * set layer direction
 *
 * @param v
 */
void RoutingLayerRule::setDirection(UInt32 v) {
    if (v < kUnknownDir)
        pref_direction_ = v;
    else
        pref_direction_ = kUnknownDir;
}

/**
 * @brief
 * indicate whether layer is RECTONLY or not
 *
 * @return
 */
bool RoutingLayerRule::isRectOnly() const {
    return rect_only_;
}

/**
 * @brief
 * set RECTONLY flag
 *
 * @param b
 */
void RoutingLayerRule::setIsRectOnly(bool b) {
    rect_only_ = b ? 1 : 0;
}

/**
 * @brief 
 * whether to ignore rectonly check connecting to non-corepin
 *
 * @return 
 */
bool RoutingLayerRule::isExceptNonCorePins() const {
    return is_except_non_core_pins_;
}

/**
 * @brief 
 * set whether to ignore rectonly check connecting to non-corepin
 *
 * @param v
 */
void RoutingLayerRule::setIsExceptNonCorePins(bool v) {
    is_except_non_core_pins_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether pitch definition is PITCH pitch | x y or not
 *
 * @return
 */
bool RoutingLayerRule::isXYPitch() const {
    return xy_pitch_;
}

/**
 * @brief
 * indicate whether offset definition is OFFSET offset | xoffset yoffset or not
 *
 * @return
 */
bool RoutingLayerRule::isXYOffset() const {
    return xy_offset_;
}

/**
 * @brief
 * indicate whether diagpitch is DIAGPITCH pitch | xpitch ypitch or not
 *
 * @return
 */
bool RoutingLayerRule::isXYDiagPitch() const {
    return xy_diag_pitch_;
}

/**
 * @brief
 * get minimum width
 *
 * @return
 */
UInt32 RoutingLayerRule::getMinWidth() const {
    return min_width_;
}

/**
 * @brief
 * set minimum width
 *
 * @param w
 */
void RoutingLayerRule::setMinWidth(UInt32 w) {
    min_width_ = w;
}

/**
 * @brief
 * get maximum width
 *
 * @return
 */
UInt32 RoutingLayerRule::getMaxWidth() const {
    return max_width_;
}

/**
 * @brief
 * set maximum width
 *
 * @param w
 */
void RoutingLayerRule::setMaxWidth(UInt32 w) {
    max_width_ = w;
}

/**
 * @brief
 * get wrong direction width
 *
 * @return
 */
UInt32 RoutingLayerRule::getWrongDirWidth() const {
    return wrong_dir_width_;
}

/**
 * @brief
 * set wrong direction width
 *
 * @param w
 */
void RoutingLayerRule::setWrongDirWidth(UInt32 w) {
    wrong_dir_width_ = w;
}

/**
 * @brief
 * get layer pitch
 *
 * @return
 */
UInt32 RoutingLayerRule::getPitch() const {
    return pitch_x_;
}

/**
 * @brief
 * set layer pitch
 *
 * @param p
 *
 * @return
 */
void RoutingLayerRule::setPitch(UInt32 p) {
    setPitchXY(p, p);
    xy_pitch_ = 0;
}

/**
 * @brief
 * get pitch x
 *
 * @return
 */
UInt32 RoutingLayerRule::getPitchX() const {
    return pitch_x_;
}

/**
 * @brief
 * get pitch y
 *
 * @return
 */
UInt32 RoutingLayerRule::getPitchY() const {
    return pitch_y_;
}

/**
 * @brief
 * set pitch x y
 *
 * @param px
 * @param py
 */
void RoutingLayerRule::setPitchXY(UInt32 px, UInt32 py) {
    pitch_x_ = px;
    pitch_y_ = py;
    xy_pitch_ = 1;
}

/**
 * @brief 
 * get firstlast pitch
 *
 * @return 
 */
UInt32 RoutingLayerRule::getFirstLastPitch() const {
    return first_last_pitch_;
}

/**
 * @brief 
 * set firstlast pitch
 *
 * @param p
 */
void RoutingLayerRule::setFirstLastPitch(UInt32 p) {
    first_last_pitch_ = p;
}

/**
 * @brief
 * get layer offset
 *
 * @return
 */
UInt32 RoutingLayerRule::getOffset() const {
    return offset_x_;
}

/**
 * @brief
 * set layer offset
 *
 * @param offset
 */
void RoutingLayerRule::setOffset(UInt32 offset) {
    setOffsetXY(offset, offset);
    xy_offset_ = 0;
}

/**
 * @brief
 * get offset x
 *
 * @return
 */
UInt32 RoutingLayerRule::getOffsetX() const {
    return offset_x_;
}

/**
 * @brief
 * get offset y
 *
 * @return
 */
UInt32 RoutingLayerRule::getOffsetY() const {
    return offset_y_;
}

/**
 * @brief
 * set offset x y
 *
 * @param ox
 * @param oy
 */
void RoutingLayerRule::setOffsetXY(UInt32 ox, UInt32 oy) {
    offset_x_ = ox;
    offset_y_ = oy;
    xy_offset_ = 1;
}

/**
 * @brief
 * get diag pitch
 *
 * @return
 */
UInt32 RoutingLayerRule::getDiagPitch() const {
    return diag_pitch45_;
}

/**
 * @brief
 * set diag45 pitch
 *
 * @param pitch
 */
void RoutingLayerRule::setDiagPitch(UInt32 pitch) {
    setDiagPitchXY(pitch, pitch);
    xy_diag_pitch_ = 0;
}

/**
 * @brief
 * get diag45 pitch
 *
 * @return
 */
UInt32 RoutingLayerRule::getDiagPitch45() const {
    return diag_pitch45_;
}

/**
 * @brief
 * get diag135 pitch
 *
 * @return
 */
UInt32 RoutingLayerRule::getDiagPitch135() const {
    return diag_pitch135_;
}

/**
 * @brief
 * set diag45 & diag135 pitch
 *
 * @param dp45
 * @param dp135
 */
void RoutingLayerRule::setDiagPitchXY(UInt32 dp45, UInt32 dp135) {
    diag_pitch45_ = dp45;
    diag_pitch135_ = dp135;
    xy_diag_pitch_ = 1;
}

/**
 * @brief
 * get diag width
 *
 * @return
 */
UInt32 RoutingLayerRule::getDiagWidth() const {
    return diag_width_;
}

/**
 * @brief
 * set diag width
 *
 * @param w
 */
void RoutingLayerRule::setDiagWidth(UInt32 w) {
    diag_width_ = w;
}

/**
 * @brief
 * get diag spacing
 *
 * @return
 */
UInt32 RoutingLayerRule::getDiagSpacing() const {
    return diag_spacing_;
}

/**
 * @brief
 * set diag spacing
 *
 * @param sp
 */
void RoutingLayerRule::setDiagSpacing(UInt32 sp) {
    diag_spacing_ = sp;
}

/**
 * @brief
 * get diag minimum edge length
 *
 * @return
 */
UInt32 RoutingLayerRule::getDiagMinEdgeLength() const {
    return diag_min_edge_length_;
}

/**
 * @brief
 * set diag minimum edge length
 *
 * @param len
 */
void RoutingLayerRule::setDiagMinEdgeLength(UInt32 len) {
    diag_min_edge_length_ = len;
}

/**
 * @brief 
 * get lef58 manufacturing grid
 *
 * @return 
 */
UInt32 RoutingLayerRule::getMGrid() const {
    return m_grid_;
}

/**
 * @brief 
 * set lef58 manufacturing grid
 *
 * @param m
 */
void RoutingLayerRule::setMGrid(UInt32 m) {
    m_grid_ = m;
}

/**
 * @brief
 * get wire extension
 *
 * @return
 */
UInt32 RoutingLayerRule::getWireExt() const {
    return wire_extension_;
}

/**
 * @brief
 * set wire extension
 *
 * @param ext
 */
void RoutingLayerRule::setWireExt(UInt32 ext) {
    wire_extension_ = ext;
}

/**
 * @brief
 * get window length of density check
 *
 * @return
 */
UInt32 RoutingLayerRule::getFillCheckWinLength() const {
    return fill_check_length_;
}

/**
 * @brief
 * set window length of density check
 *
 * @param len
 */
void RoutingLayerRule::setFillCheckWinLength(UInt32 len) {
    fill_check_length_ = len;
}

/**
 * @brief
 * get window width of density check
 *
 * @return
 */
UInt32 RoutingLayerRule::getFillCheckWinWidth() const {
    return fill_check_width_;
}

/**
 * @brief
 * set window width of density check
 *
 * @param w
 */
void RoutingLayerRule::setFillCheckWinWidth(UInt32 w) {
    fill_check_width_ = w;
}

/**
 * @brief
 * get window step of density check
 *
 * @return
 */
UInt32 RoutingLayerRule::getFillCheckWinStep() const {
    return fill_check_step_;
}

/**
 * @brief
 * set window step of density check
 *
 * @param step
 */
void RoutingLayerRule::setFillCheckWinStep(UInt32 step) {
    fill_check_step_ = step;
}

/**
 * @brief
 * get spacing between metal fill and other shape
 *
 * @return
 */
UInt32 RoutingLayerRule::getFillActiveSpacing() const {
    return fill_to_active_spacing_;
}

/**
 * @brief
 * set the spacing between metal fill and other shape
 *
 * @param spacing
 */
void RoutingLayerRule::setFillActiveSpacing(UInt32 spacing) {
    fill_to_active_spacing_ = spacing;
}

/**
 * @brief
 * get the spacing between metal fills
 *
 * @return
 */
UInt32 RoutingLayerRule::getFillToFillSpacing() const {
    return fill_to_fill_spacing_;
}

/**
 * @brief
 * set the spacing between metal fills
 *
 * @param spacing
 */
void RoutingLayerRule::setFillToFillSpacing(UInt32 spacing) {
    fill_to_fill_spacing_ = spacing;
}

/**
 * @brief
 * get height
 *
 * @return
 */
UInt32 RoutingLayerRule::getHeight() const {
    return height_;
}

/**
 * @brief
 * set height
 *
 * @param h
 */
void RoutingLayerRule::setHeight(UInt32 h) {
    height_ = h;
}

/**
 * @brief
 * get thickness
 *
 * @return
 */
UInt32 RoutingLayerRule::getThickness() const {
    return thickness_;
}

/**
 * @brief
 * set thickness
 *
 * @param t
 */
void RoutingLayerRule::setThickness(UInt32 t) {
    thickness_ = t;
}

/**
 * @brief
 * set shrinkage
 *
 * @return
 */
UInt32 RoutingLayerRule::getShrinkage() const {
    return shrinkage_;
}

/**
 * @brief
 * set shrinkage
 *
 * @param s
 */
void RoutingLayerRule::setShrinkage(UInt32 s) {
    shrinkage_ = s;
}

/**
 * @brief
 * get resistance per square
 *
 * @return
 */
float RoutingLayerRule::getResPerSq() const {
    return res_per_square_;
}

/**
 * @brief
 * set resistance per square
 *
 * @param r
 */
void RoutingLayerRule::setResPerSq(float r) {
    res_per_square_ = r;
}

/**
 * @brief
 * get capacitance per square
 *
 * @return
 */
float RoutingLayerRule::getCapPerSq() const {
    return cap_per_square_;
}

void RoutingLayerRule::setCapPerSq(float cap) {
    cap_per_square_ = cap;
}

/**
 * @brief
 * get capacitance multiplier
 *
 * @return
 */
float RoutingLayerRule::getCapMul() const {
    return cap_multiplier_;
}

/**
 * @brief
 * set capacitance multiplier
 *
 * @param cm
 */
void RoutingLayerRule::setCapMul(float cm) {
    cap_multiplier_ = cm;
}

/**
 * @brief
 * get edge capacitance
 *
 * @return
 */
float RoutingLayerRule::getEdgeCap() const {
    return edge_capacitance_;
}

/**
 * @brief
 * set edge capacitance
 *
 * @param cap
 */
void RoutingLayerRule::setEdgeCap(float cap) {
    edge_capacitance_ = cap;
}

/**
 * @brief
 * get minimum metal fill density based on the window
 *
 * @return
 */
float RoutingLayerRule::getFillMinDensity() const {
    return fill_min_density_;
}

/**
 * @brief
 * set minimum metal fill desnity based on the window
 *
 * @param den
 */
void RoutingLayerRule::setFillMinDensity(float den) {
    fill_min_density_ = den;
}

/**
 * @brief
 * get maximum metal fill desnity based on the window
 *
 * @return
 */
float RoutingLayerRule::getFillMaxDensity() const {
    return fill_max_density_;
}

/**
 * @brief
 * set maximum metal fill desnity based on the window
 *
 * @param den
 */
void RoutingLayerRule::setFillMaxDensity(float den) {
    fill_max_density_ = den;
}

/**
 * @brief
 * get trim layer
 *
 * @return Layer
 */
Layer* RoutingLayerRule::getTrimLayer() const {
    return addr<Layer>(trim_layer_);
}

/**
 * @brief
 * set trim layer
 *
 * @param layer
 */
void RoutingLayerRule::setTrimLayer(ObjectId layer) {
    trim_layer_ = layer;
}

/**
 * @brief
 * get EOLKEEPOUT rule list
 *
 * @return EOLKeepout
 */
EOLKeepout* RoutingLayerRule::getEOLKeepout(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (eol_keepouts_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(eol_keepouts_);
    }
    if (vct) {
        EOLKeepout *obj_data = addr<EOLKeepout>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

ArrayObject<ObjectId> *RoutingLayerRule::getEOLKeepouts() const {
    //return eol_keepout_list_;
    if (eol_keepouts_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(eol_keepouts_);
        return array;
    } else {
        return nullptr;
    }
}
/**
 * @brief
 * set EOLKEEPOUT rule
 *
 * @param eolk
 */
void RoutingLayerRule::addEOLKeepout(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (eol_keepouts_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        eol_keepouts_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(eol_keepouts_);
    }
    if (vct) vct->pushBack(id);
}

/**
 * @brief
 * get BOUNDARYEOLBLOCKAGE rule
 *
 * @return BoundaryEOLBlockage
 */
BoundaryEOLBlockage* RoutingLayerRule::getBoundaryEOLBlocakge(int i) const {
    //return boundary_eol_blks_;
    ArrayObject<ObjectId> *vct = nullptr;
    if (boundary_eol_blks_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(boundary_eol_blks_);
    }
    if (vct) {
        BoundaryEOLBlockage *obj_data = addr<BoundaryEOLBlockage>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

ArrayObject<ObjectId> *RoutingLayerRule::getBoundaryEOLBlocakges() const {
    if (boundary_eol_blks_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(boundary_eol_blks_);
        return array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * set BOUNDARYEOLBLOCKAGE rule
 *
 * @param eol_blkg
 */
void RoutingLayerRule::addBoundaryEOLBlockage(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (boundary_eol_blks_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        boundary_eol_blks_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(boundary_eol_blks_);
    }
    if (vct) vct->pushBack(id);
}

/**
 * @brief
 * get CORNEREOLKEEPOUT rule list
 *
 * @return CornerEOLKeepout
 */
ArrayObject<ObjectId> *RoutingLayerRule::getCornerEOLKeepouts() const {
     if (corner_eol_keepouts_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(corner_eol_keepouts_);
        return array;
    } else {
        return nullptr;
    }
}

CornerEOLKeepout* RoutingLayerRule::getCornerEOLKeepout(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (corner_eol_keepouts_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(corner_eol_keepouts_);
    }
    if (vct) {
        CornerEOLKeepout *obj_data = addr<CornerEOLKeepout>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

/**
 * @brief
 * set CORNEREOLKEEPOUT rule
 *
 * @param ceolk
 */
void RoutingLayerRule::addCornerEOLKeepout(ObjectId id) {
    //corner_eol_keepout_list_ = ceolk;
    ArrayObject<ObjectId> *vct = nullptr;
    if (corner_eol_keepouts_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        corner_eol_keepouts_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(corner_eol_keepouts_);
    }
    if (vct) vct->pushBack(id);
}

/**
 * @brief
 * get CORNERFILLSPACING rule list
 *
 * @return CornerFillSpacing
 */
CornerFillSpacing* RoutingLayerRule::getCornerFillSpacing(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (corner_fill_spacings_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(corner_fill_spacings_);
    }
    if (vct) {
        CornerFillSpacing *obj_data = addr<CornerFillSpacing>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

ArrayObject<ObjectId> *RoutingLayerRule::getCornerFillSpacings() const {
     if (corner_fill_spacings_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(corner_fill_spacings_);
        return array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * set CORNERFILLSPACING rule
 *
 * @param cfs
 */
void RoutingLayerRule::addCornerFillSpacing(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (corner_fill_spacings_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        corner_fill_spacings_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(corner_fill_spacings_);
    }
    if (vct) vct->pushBack(id);

}

/**
 * @brief
 * get CORNERSPACING rule list
 *
 * @return CornerSpacing
 */
CornerSpacing* RoutingLayerRule::getCornerSpacing(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (corner_spacings_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(corner_spacings_);
    }
    if (vct) {
        CornerSpacing *obj_data = addr<CornerSpacing>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

ArrayObject<ObjectId> *RoutingLayerRule::getCornerSpacings() const {
    if (corner_spacings_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(corner_spacings_);
        return array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * set CORNERSPACING rule
 *
 * @param cs
 */
void RoutingLayerRule::addCornerSpacing(ObjectId id) {
    //corner_spacing_list_ = cs;
    ArrayObject<ObjectId> *vct = nullptr;
    if (corner_spacings_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        corner_spacings_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(corner_spacings_);
    }
    if (vct) vct->pushBack(id);
}

/**
 * @brief
 * get DIRECTIONALSPANLENGTH spacing table
 *
 * @return DirSpanLengthSpTbl
 */
DirSpanLengthSpTbl* RoutingLayerRule::getDirSpanLengthSpTbl(int i) const {
    //return dsl_list_;
    ArrayObject<ObjectId> *vct = nullptr;
    if (dsls_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(dsls_);
    }
    if (vct) {
        DirSpanLengthSpTbl *obj_data = addr<DirSpanLengthSpTbl>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

ArrayObject<ObjectId> *RoutingLayerRule::getDirSpanLengthSpTbls() const {
    if (dsls_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(dsls_);
        return array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * set DIRECTIONALSPANLENGTH spacing table
 *
 * @param dsl
 */
void RoutingLayerRule::addDirSpanLengthSpTbl(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (dsls_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        dsls_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(dsls_);
    }
    if (vct) vct->pushBack(id);
}

/**
 * @brief
 * get minimumcut rule list
 *
 * @return MinCut
 */
MinCut* RoutingLayerRule::getMinCut(int i) const {
    //return min_cut_list_;
    ArrayObject<ObjectId> *vct = nullptr;
    if (min_cuts_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(min_cuts_);
    }
    if (vct) {
        MinCut *obj_data = addr<MinCut>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

ArrayObject<ObjectId> *RoutingLayerRule::getMinCuts() const {
    if (min_cuts_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(min_cuts_);
        return array;
    } else {
        return nullptr;
    }
}
/**
 * @brief set minimumcut rule list
 *
 * @param l
 */
void RoutingLayerRule::addMinCut(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (min_cuts_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        min_cuts_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(min_cuts_);
    }
    if (vct) vct->pushBack(id);
}

/**
 * @brief
 * get MINENCLOSEDAREA rule list
 *
 * @return
 */
MinEnclArea* RoutingLayerRule::getMinEnclArea(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (min_enclosed_areas_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(min_enclosed_areas_);
    }
    if (vct) {
        MinEnclArea *obj_data = addr<MinEnclArea>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;

}

ArrayObject<ObjectId> *RoutingLayerRule::getMinEnclAreas() const {
    if (min_enclosed_areas_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(min_enclosed_areas_);
        return array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * set MINENCLOSEDAREA rule list
 *
 * @param l
 */
void RoutingLayerRule::addMinEnclArea(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (min_enclosed_areas_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        min_enclosed_areas_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(min_enclosed_areas_);
    }
    if (vct) vct->pushBack(id);
}

/**
 * @brief
 * get minsize rule
 *
 * @return MinSize
 */
MinSize* RoutingLayerRule::getMinSize(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (min_sizes_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(min_sizes_);
    }
    if (vct) {
        MinSize *obj_data = addr<MinSize>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

ArrayObject<ObjectId> *RoutingLayerRule::getMinSizes() const {
    //return min_size_;
    if (min_sizes_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(min_sizes_);
        return array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * set minsize rule
 *
 * @param l
 */
void RoutingLayerRule::addMinSize(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (min_sizes_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        min_sizes_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(min_sizes_);
    }
    if (vct) vct->pushBack(id);
}

/**
 * @brief
 * get minstep list
 *
 * @return MinStep
 */
MinStep* RoutingLayerRule::getMinStep(int i) const {
    //return min_step_list_;
    ArrayObject<ObjectId> *vct = nullptr;
    if (min_steps_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(min_steps_);
    }
    if (vct) {
        MinStep *obj_data = addr<MinStep>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

ArrayObject<ObjectId> *RoutingLayerRule::getMinSteps() const {
   if (min_steps_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(min_steps_);
        return array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * set minstep list
 *
 * @param l
 */
void RoutingLayerRule::addMinStep(ObjectId id) {
    //min_step_list_ = l;
    ArrayObject<ObjectId> *vct = nullptr;
    if (min_steps_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        min_steps_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(min_steps_);
    }
    if (vct) vct->pushBack(id);
}

/**
 * @brief
 * get protrusionwidth rule
 *
 * @return
 */
ProtrusionRule* RoutingLayerRule::getProtrusionRule(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (protrusions_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(protrusions_);
    }
    if (vct) {
        ProtrusionRule *obj_data = addr<ProtrusionRule>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

ArrayObject<ObjectId> *RoutingLayerRule::getProtrusionRules() const {
    if (protrusions_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(protrusions_);
        return array;
    } else {
        return nullptr;
    }

}

/**
 * @brief
 * set protrusionwidth rule
 *
 * @param l
 */
void RoutingLayerRule::addProtrusionRule(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (protrusions_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        protrusions_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(protrusions_);
    }
    if (vct) vct->pushBack(id);

}

/**
 * @brief
 * get PARALLELRUNLENGTH WIDTH and TWOWIDTHS rules list
 *
 * @return
 */
WidthSpTbl* RoutingLayerRule::getWidthSpTbl(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (width_sp_tbls_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(width_sp_tbls_);
    }
    if (vct) {
        WidthSpTbl *obj_data = addr<WidthSpTbl>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

ArrayObject<ObjectId> *RoutingLayerRule::getWidthSpTbls() const {
    if (width_sp_tbls_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(width_sp_tbls_);
        return array;
    } else {
        return nullptr;
    }
}
/**
 * @brief
 * set PARALLELRUNLENGTH WIDTH and TWOWIDTHS rules list
 *
 * @param l
 */
void RoutingLayerRule::addWidthSpTabl(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (width_sp_tbls_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        width_sp_tbls_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(width_sp_tbls_);
    }
    if (vct) vct->pushBack(id);

}

/**
 * @brief
 * get spacingtable influence rule
 *
 * @return
 */
InfluenceSpTbl* RoutingLayerRule::getInfluenceSpTbl(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (influence_sp_tbls_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(influence_sp_tbls_);
    }
    if (vct) {
        InfluenceSpTbl *obj_data = addr<InfluenceSpTbl>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

ArrayObject<ObjectId> *RoutingLayerRule::getInfluenceSpTbls() const {
    if (influence_sp_tbls_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(influence_sp_tbls_);
        return array;
    } else {
        return nullptr;
    }
}


/**
 * @brief
 * set spacingtable influence rule
 *
 * @param t
 */
void RoutingLayerRule::addInfluenceSpTbl(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (influence_sp_tbls_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        influence_sp_tbls_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(influence_sp_tbls_);
    }
    if (vct) vct->pushBack(id);
}

/**
 * @brief
 * get spacingtable parallel span length rule
 *
 * @return
 */
ParaSpanLenTbl* RoutingLayerRule::getParaSpanLenTbl(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (para_span_len_sp_tbls_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(para_span_len_sp_tbls_);
    }
    if (vct) {
        ParaSpanLenTbl *obj_data = addr<ParaSpanLenTbl>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

ArrayObject<ObjectId> *RoutingLayerRule::getParaSpanLenTbls() const {
    if (para_span_len_sp_tbls_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(para_span_len_sp_tbls_);
        return array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * set spacingtable parallel span length rule
 *
 * @param t
 */
void RoutingLayerRule::addParaSpanLenTbl(ObjectId id) {
    ArrayObject<ObjectId> *vct = nullptr;
    if (para_span_len_sp_tbls_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        para_span_len_sp_tbls_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(para_span_len_sp_tbls_);
    }
    if (vct) vct->pushBack(id);
}

/**
 * @brief
 * get SPACING list
 *
 * @return
 */
RoutingSpacing* RoutingLayerRule::getSpacing(int i) const {
    ArrayObject<ObjectId> *vct = nullptr;
    if (spacings_ == 0) {
        return nullptr;
    } else {
        vct = addr<ArrayObject<ObjectId>>(spacings_);
    }
    if (vct) {
        RoutingSpacing *obj_data = addr<RoutingSpacing>((*vct)[i]);
        if (obj_data) {
            return obj_data;
        }
    }
    return nullptr;
}

ArrayObject<ObjectId> *RoutingLayerRule::getSpacings() const {
    if (spacings_ != 0) {
        ArrayObject<ObjectId> *array = addr<ArrayObject<ObjectId>>(spacings_);
        return array;
    } else {
        return nullptr;
    }
}

/**
 * @brief
 * set spacing list
 *
 * @param l
 */
void RoutingLayerRule::addSpacing(ObjectId id) {
   ArrayObject<ObjectId> *vct = nullptr;
    if (spacings_ == 0) {
        vct = getOwnerCell()->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
        if (vct == nullptr) return;
        spacings_ = vct->getId();
        vct->setPool(getOwnerCell()->getPool());
        vct->reserve(16);
    } else {
        vct = addr<ArrayObject<ObjectId>>(spacings_);
    }
    if (vct) vct->pushBack(id);    
}

} // namespace db
} // namespace open_edi
