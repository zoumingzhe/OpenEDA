/* @file  layer.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/core/cell.h"
#include "db/tech/layer.h"
#include "db/core/db.h"

namespace open_edi {
namespace db {
using IdArray = ArrayObject<ObjectId>;

/**
 * @brief
 * relationship table
 */
static const LayerTypeTable LayerTypeTables[] = {
    {"",                kLayerUnknown},
    {"IMPLANT",         kLayerImplant},
    {"MASTERSLICE",     kLayerMS},
    {"OVERLAP",         kLayerOverlap},
    {"CUT",             kLayerCut},
    {"ROUTING",         kLayerRouting}
};

static const UInt32 kLayerTypeTblSize = sizeof(LayerTypeTables) / sizeof(LayerTypeTable);

UInt32 GetLayerTypeTableSize() {
    return kLayerTypeTblSize;
}

/**
  * @brief
  * get layer string type from tables
  *
  * @param index
  *
  * @return
  */
const char* GetLEFTypeFromTables(UInt32 index) {
    if (index >= kLayerTypeTblSize)
        return LayerTypeTables[0].getLEFType();
    return LayerTypeTables[index].getLEFType();
}

/**
 * @brief
 * get edi layer type from tables
 *
 * @param index
 *
 * @return
 */
const LayerType GetEDITypeFromTables(UInt32 index) {
    if (index >= kLayerTypeTblSize)
        return LayerTypeTables[0].getEDIType();
    return LayerTypeTables[index].getEDIType();
}

/**
 * @brief
 * release allocated memory
 *
 * @param ptr
 * @param is_ptr_set
 */
void AntennaModel::release(std::vector<std::pair<float, float>>*& ptr, UInt32 is_ptr_set) {
    if (is_ptr_set && ptr) {
        delete ptr;
        ptr = nullptr;
    }
}

/**
 * @brief
 * reset the current model. If one more same OXIDE is defined in LEF, the next one should overwrite previous one.
 * it means, if isSet() is true before last one, the existing model must be reset
 */
void AntennaModel::reset() {
    release(diff_area_ratio_pwl_, is_diff_area_ratio_pwl_);
    release(cum_diff_area_ratio_pwl_, is_cum_diff_area_ratio_pwl_);
    release(gate_plus_diff_pwl_, is_gate_plus_diff_pwl_);
    release(area_diff_reduce_pwl_, true);
    release(diff_gate_pwl_, true);
    release(gate_pwl_, true);
    release(diff_side_area_ratio_pwl_, is_diff_side_area_ratio_pwl_);
    release(cum_diff_side_area_ratio_pwl_, is_cum_diff_side_area_ratio_pwl_);
    memset(static_cast<void*>(this), 0, sizeof(AntennaModel));
    area_factor_ = 1.0f;
}

/**
 * @brief
 * add float values pair to specified pair container
 *
 * @param pairs
 * @param first
 * @param second
 */
void AntennaModel::pushBackPair(std::vector<std::pair<float, float>>*& pairs, float first, float second) {
    if (!pairs)
        pairs = new std::vector<std::pair<float, float>>;
    if (pairs) {
        pairs->push_back({first, second});
    }
}

/**
 * @brief
 * indicate whether same OXIDE is defined or not before this model
 *
 * @return
 */
bool AntennaModel::isSet() const {
    return is_set_;
}

/**
 * @brief
 * set isSet flag
 *
 * @param v
 */
void AntennaModel::setIsSet(bool v) {
    is_set_ = v;
}

/**
 * @brief
 * ANTENNADIFFAREARATIO PWL flag
 *
 * @return
 */
bool AntennaModel::isDiffAreaRatioPWL() const {
    return is_diff_area_ratio_pwl_;
}

/**
 * @brief
 * set ANTENNADIFFAREARATIO PWL flag
 *
 * @param v
 */
void AntennaModel::setIsDiffAreaRatioPWL(bool v) {
    is_diff_area_ratio_pwl_ = v;
}

/**
 * @brief
 * ANTENNACUMDIFFAREARATIO PWL flag
 *
 * @return
 */
bool AntennaModel::isCumDiffAreaRatioPWL() const {
    return is_cum_diff_area_ratio_pwl_;
}

/**
 * @brief
 * set ANTENNACUMDIFFAREARATIO PWL flag
 *
 * @param v
 */
void AntennaModel::setIsCumDiffAreaRatioPWL(bool v) {
    is_cum_diff_area_ratio_pwl_ = v;
}

/**
 * @brief
 * ANTENNAAREAFACTOR DIFFUSEONLY flag
 *
 * @return
 */
bool AntennaModel::isDiffUseOnly() const {
    return is_diff_use_only_;
}

/**
 * @brief
 * set ANTENNAAREAFACTOR DIFFUSEONLY flag
 *
 * @param v
 */
void AntennaModel::setIsDiffUseOnly(bool v) {
    is_diff_use_only_ = v;
}

/**
 * @brief
 * ANTENNACUMROUTINGPLUSCUT flag
 *
 * @return
 */
bool AntennaModel::isCumRoutingPlusCut() const {
    return is_cum_routing_plus_cut_;
}

/**
 * @brief
 * set ANTENNACUMROUTINGPLUSCUT flag
 *
 * @param v
 */
void AntennaModel::setIsCumRoutingPlusCut(bool v) {
    is_cum_routing_plus_cut_ = v;
}

/**
 * @brief
 * ANTENNADIFFSIDEAREARATIO PWL flag
 *
 * @return
 */
bool AntennaModel::isDiffSideAreaRatioPWL() const {
    return is_diff_side_area_ratio_pwl_;
}

/**
 * @brief
 * set ANTENNADIFFSIDEAREARATIO PWL flag
 *
 * @param v
 */
void AntennaModel::setIsDiffSideAreaRatioPWL(bool v) {
    is_diff_side_area_ratio_pwl_ = v;
}

/**
 * @brief
 * ANTENNACUMDIFFSIDEAREARATIO PWL flag
 *
 * @return
 */
bool AntennaModel::isCumDiffSideAreaRatioPWL() const {
    return is_cum_diff_side_area_ratio_pwl_;
}

/**
 * @brief
 * set ANTENNACUMDIFFSIDEAREARATIO PWL flag
 *
 * @param v
 */
void AntennaModel::setIsCumDiffSideAreaRatioPWL(bool v) {
    is_cum_diff_side_area_ratio_pwl_ = v;
}

/**
 * @brief
 * ANTENNASIDEAREAFACTOR DIFFUSEONLY flag
 *
 * @return
 */
bool AntennaModel::isSideDiffUseOnly() const {
    return is_side_diff_use_only_;
}

/**
 * @brief
 * set ANTENNASIDEAREAFACTOR DIFFUSEONLY flag
 *
 * @param v
 */
void AntennaModel::setIsSideDiffUseOnly(bool v) {
    is_side_diff_use_only_ = v;
}

/**
 * @brief 
 * indicate whether area factor is specified or not
 *
 * @return 
 */
bool AntennaModel::isDefaultAreaFactor() const {
    return is_default_area_factor_;
}

/**
 * @brief 
 * area factor is specified by LEF or default value(1.0)
 *
 * @param v
 */
void AntennaModel::setIsDefaultAreaFactor(bool v) {
    is_default_area_factor_ = v;
}

/**
 * @brief
 * get ANTENNAAREARATIO
 *
 * @return
 */
float AntennaModel::getAreaRatio() const {
    return area_ratio_;
}

/**
 * @brief
 * set ANTENNAAREARATIO
 *
 * @param area_ratio
 */
void AntennaModel::setAreaRatio(float area_ratio) {
    area_ratio_ = area_ratio;
}

/**
 * @brief
 * get ANTENNACUMAREARATIO
 *
 * @return
 */
float AntennaModel::getCumAreaRatio() const {
    return cum_area_ratio_;
}

/**
 * @brief
 * set ANTENNACUMAREARATIO
 *
 * @param r
 */
void AntennaModel::setCumAreaRatio(float r) {
    cum_area_ratio_ = r;
}

/**
 * @brief
 * get ANTENNAAREAFACTOR
 *
 * @return
 */
float AntennaModel::getAreaFactor() const {
    return area_factor_;
}

/**
 * @brief
 * set ANTENNAAREAFACTOR
 *
 * @param f
 */
void AntennaModel::setAreaFactor(float f) {
    area_factor_ = f;
}

/**
 * @brief
 * get ANTENNAAREAMINUSDIFF
 *
 * @return
 */
float AntennaModel::getAreaMinusDiff() const {
    return area_minus_diff_;
}

/**
 * @brief
 * set ANTENNAAREAMINUSDIFF
 *
 * @param area_minus
 */
void AntennaModel::setAreaMinusDiff(float area_minus) {
    area_minus_diff_ = area_minus;
}

/**
 * @brief
 * get ANTENNASIDEAREARATIO
 *
 * @return
 */
float AntennaModel::getSideAreaRatio() const {
    return side_area_ratio_;
}

/**
 * @brief
 * set ANTENNASIDEAREARATIO
 *
 * @param side_ratio
 */
void AntennaModel::setSideAreaRatio(float side_ratio) {
    side_area_ratio_ = side_ratio;
}

/**
 * @brief
 * get ANTENNACUMSIDEAREARATIO
 *
 * @return
 */
float AntennaModel::getCumSideAreaRatio() const {
    return cum_side_area_ratio_;
}

/**
 * @brief
 * set ANTENNACUMSIDEAREARATIO
 *
 * @param cum_side_ratio
 */
void AntennaModel::setCumSideAreaRatio(float cum_side_ratio) {
    cum_side_area_ratio_ = cum_side_ratio;
}

/**
 * @brief
 * get ANTENNASIDEAREAFACTOR
 *
 * @return
 */
float AntennaModel::getSideAreaFactor() const {
    return side_area_factor_;
}

/**
 * @brief
 * set ANTENNASIDEAREAFACTOR
 *
 * @param side_factor
 */
void AntennaModel::setSideAreaFactor(float side_factor) {
    side_area_factor_ = side_factor;
}

/**
 * @brief
 * get ANTENNADIFFAREARATIO
 *
 * @return
 */
float AntennaModel::getDiffAreaRatio() const {
    return is_diff_area_ratio_pwl_ ? 0.0 : diff_area_ratio_;
}

/**
 * @brief
 * set ANTENNADIFFAREARATIO
 *
 * @param diff_ratio
 */
void AntennaModel::setDiffAreaRatio(float diff_ratio) {
    diff_area_ratio_ = diff_ratio;
}

/**
 * @brief
 * get ANTENNADIFFAREARATIO PWL
 *
 * @param index
 *
 * @return
 */
std::pair<float, float>* AntennaModel::getDiffAreaRatioPWL(UInt32 index) const {
    if (is_diff_area_ratio_pwl_ && diff_area_ratio_pwl_ && index < diff_area_ratio_pwl_->size())
        return &diff_area_ratio_pwl_->at(index);
    return nullptr;
}

/**
 * @brief
 * set ANTENNADIFFAREARATIO PWL
 *
 * @param d
 * @param r
 */
void AntennaModel::addDiffAreaRatioPWL(float d, float r) {
    if (!is_diff_area_ratio_pwl_)
        is_diff_area_ratio_pwl_ = 1;
    pushBackPair(diff_area_ratio_pwl_, d, r);
}

/**
 * @brief
 * get the size of ANTENNADIFFAREARATIO PWL
 *
 * @return
 */
UInt32 AntennaModel::getDiffAreaRatioPWLSize() const {
    return diff_area_ratio_pwl_ ? diff_area_ratio_pwl_->size() : 0;
}

/**
 * @brief
 * get ANTENNACUMDIFFAREARATIO
 *
 * @return
 */
float AntennaModel::getCumDiffAreaRatio() const {
    return is_cum_diff_area_ratio_pwl_ ? 0 : cum_diff_area_ratio_;
}

/**
 * @brief
 * set ANTENNACUMDIFFAREARATIO
 *
 * @param r
 */
void AntennaModel::setCumDiffAreaRatio(float r) {
    cum_diff_area_ratio_ = r;
}

/**
 * @brief
 * get n-th ANTENNACUMDIFFAREARATIO PWL
 *
 * @param index
 *
 * @return
 */
std::pair<float, float>* AntennaModel::getCumDiffAreaRatioPWL(UInt32 index) const {
    if (is_cum_diff_area_ratio_pwl_ && cum_diff_area_ratio_pwl_ && index < cum_diff_area_ratio_pwl_->size())
        return &cum_diff_area_ratio_pwl_->at(index);
    return nullptr;
}

/**
 * @brief
 * get the size of ANTENNACUMDIFFAREARATIO PWL
 *
 * @return
 */
UInt32 AntennaModel::getCumDiffAreaRatioPWLSize() const {
    return is_cum_diff_area_ratio_pwl_ ? cum_diff_area_ratio_pwl_->size() : 0;
}

/**
 * @brief
 * add ANTENNACUMDIFFAREARATIO PWL
 *
 * @param d
 * @param r
 */
void AntennaModel::addCumDiffAreaRatioPWL(float d, float r) {
    if (!is_cum_diff_area_ratio_pwl_)
        is_cum_diff_area_ratio_pwl_ = 1;
    pushBackPair(cum_diff_area_ratio_pwl_, d, r);
}

/**
 * @brief
 * get the id of ANTENNAGATEPLUSDIFF
 *
 * @return
 */
UInt32 AntennaModel::getGatePlusDiffId() const {
    return gate_plus_diff_id_;
}

/**
 * @brief
 * set the id of ANTENNAGATEPLUSDIFF
 *
 * @param id
 */
void AntennaModel::setGatePlusDiffId(UInt32 id) {
    gate_plus_diff_id_ = id;
}

/**
 * @brief
 * get ANTENNAGATEPLUSDIFF
 *
 * @return
 */
float AntennaModel::getGatePlusDiff() const {
    return gate_plus_diff_;
}

/**
 * @brief
 * set ANTENNAGATEPLUSDIFF
 *
 * @param d
 */
void AntennaModel::setGatePlusDiff(float d) {
    gate_plus_diff_ = d;
}

/**
 * @brief
 * get n-th ANTENNAGATEPLUSDIFF PWL
 *
 * @param index
 *
 * @return
 */
std::pair<float, float>* AntennaModel::getGatePlusDiffPWL(UInt32 index) const {
    if (is_gate_plus_diff_pwl_ && gate_plus_diff_pwl_ && index < gate_plus_diff_pwl_->size())
        return &gate_plus_diff_pwl_->at(index);
    return nullptr;
}

/**
 * @brief
 * get the size of ANTENNAGATEPLUSDIFF PWL
 *
 * @return
 */
UInt32 AntennaModel::getGatePlusDiffPWLSize() const {
    return is_gate_plus_diff_pwl_ ? gate_plus_diff_pwl_->size() : 0;
}

/**
 * @brief
 * add ANTENNAGATEPLUSDIFF PWL
 *
 * @param d
 * @param p
 */
void AntennaModel::addGatePlusDiffPWL(float d, float p) {
    if (!is_gate_plus_diff_pwl_)
        is_gate_plus_diff_pwl_ = 1;
    pushBackPair(gate_plus_diff_pwl_, d, p);
}

/**
 * @brief
 * get the n-th ANTENNAAREADIFFREDUCEPWL PWL
 *
 * @param index
 *
 * @return
 */
std::pair<float, float>* AntennaModel::getAreaDiffReducePWL(UInt32 index) const {
    if (area_diff_reduce_pwl_ && index < area_diff_reduce_pwl_->size())
        return &area_diff_reduce_pwl_->at(index);
    return nullptr;
}

/**
 * @brief
 * add ANTENNAAREADIFFREDUCEPWL PWL
 *
 * @param f
 * @param d
 */
void AntennaModel::addAreaDiffReducePWL(float f, float d) {
    pushBackPair(area_diff_reduce_pwl_, f, d);
}

/**
 * @brief
 * get the size of ANTENNAAREADIFFREDUCEPWL PWL
 *
 * @return
 */
UInt32 AntennaModel::getAreaDiffReducePWLSize() const {
    return area_diff_reduce_pwl_ ? area_diff_reduce_pwl_->size() : 0;
}

/**
 * @brief
 * get the id of ANTENNADIFFGATEPWL
 *
 * @return
 */
UInt32 AntennaModel::getDiffGatePWLId() const {
    return diff_gate_pwl_id_;
}

/**
 * @brief
 * set the id of ANTENNADIFFGATEPWL
 *
 * @param index
 */
void AntennaModel::setDiffGatePWLId(UInt32 index) {
    diff_gate_pwl_id_ = index;
}

/**
 * @brief
 * get the n-th ANTENNADIFFGATEPWL
 *
 * @param index
 *
 * @return
 */
std::pair<float, float>* AntennaModel::getDiffGatePWL(UInt32 index) const {
    if (diff_gate_pwl_ && index < diff_gate_pwl_->size())
        return &diff_gate_pwl_->at(index);
    return nullptr;
}

/**
 * @brief
 * get the size of ANTENNADIFFGATEPWL
 *
 * @return
 */
UInt32 AntennaModel::getDiffGatePWLSize() const {
    return diff_gate_pwl_ ? diff_gate_pwl_->size() : 0;
}

/**
 * @brief
 * add ANTENNADIFFGATEPWL
 *
 * @param gate_area
 * @param effective_gate_area
 */
void AntennaModel::addDiffGatePWL(float gate_area, float effective_gate_area) {
    pushBackPair(diff_gate_pwl_, gate_area, effective_gate_area);
}

/**
 * @brief
 * get the id of ANTENNAGATEPWL
 *
 * @return
 */
UInt32 AntennaModel::getGatePWLId() const {
    return gate_pwl_id_;
}

/**
 * @brief
 * set the id of ANTENNAGATEPWL
 *
 * @param index
 */
void AntennaModel::setGatePWLId(UInt32 index) {
    gate_pwl_id_ = index;
}

/**
 * @brief
 * get the n-th ANTENNAGATEPWL
 *
 * @param index
 *
 * @return
 */
std::pair<float, float>* AntennaModel::getGatePWL(UInt32 index) const {
    if (gate_pwl_ && index < gate_pwl_->size())
        return &gate_pwl_->at(index);
    return nullptr;
}

/**
 * @brief
 * get the size of ANTENNAGATEPWL
 *
 * @return
 */
UInt32 AntennaModel::getGatePWLSize() const {
    return gate_pwl_ ? gate_pwl_->size() : 0;
}

/**
 * @brief
 * add to ANTENNAGATEPWL
 *
 * @param gate_area
 * @param effective_gate_area
 */
void AntennaModel::addGatePWL(float gate_area, float effective_gate_area) {
    pushBackPair(gate_pwl_, gate_area, effective_gate_area);
}

/**
 * @brief
 * get ANTENNADIFFSIDEAREARATIO
 *
 * @return
 */
float AntennaModel::getDiffSideAreaRatio() const {
    return diff_side_area_ratio_;
}

/**
 * @brief
 * set ANTENNADIFFSIDEAREARATIO
 *
 * @param ratio
 */
void AntennaModel::setDiffSideAreaRatio(float ratio) {
    diff_side_area_ratio_ = ratio;
}

/**
 * @brief
 * get the n-th pair of ANTENNADIFFSIDEAREARATIO PWL
 *
 * @param index
 *
 * @return
 */
std::pair<float, float>* AntennaModel::getDiffSideAreaRatioPWL(UInt32 index) const {
    if (is_diff_side_area_ratio_pwl_ && diff_side_area_ratio_pwl_ && index < diff_side_area_ratio_pwl_->size())
        return &diff_side_area_ratio_pwl_->at(index);
    return nullptr;
}

/**
 * @brief
 * get the size of ANTENNADIFFSIDEAREARATIO PWL
 *
 * @return
 */
UInt32 AntennaModel::getDiffSideAreaRatioPWLSize() const {
    return (is_diff_side_area_ratio_pwl_ && diff_side_area_ratio_pwl_) ? diff_side_area_ratio_pwl_->size() : 0;
}

/**
 * @brief
 * add the pair to ANTENNADIFFSIDEAREARATIO PWL
 *
 * @param d
 * @param r
 */
void AntennaModel::addDiffSideAreaRatioPWL(float d, float r) {
    if (!is_diff_side_area_ratio_pwl_)
        is_diff_side_area_ratio_pwl_ = 1;
    pushBackPair(diff_side_area_ratio_pwl_, d, r);
}

/**
 * @brief
 * get ANTENNACUMDIFFSIDEAREARATIO
 *
 * @return
 */
float AntennaModel::getCumDiffSideAreaRatio() const {
    return cum_diff_side_area_ratio_;
}

/**
 * @brief
 * set ANTENNACUMDIFFSIDEAREARATIO
 *
 * @param v
 */
void AntennaModel::setCumDiffSideAreaRatio(float v) {
    cum_diff_side_area_ratio_ = v;
}

/**
 * @brief
 * get the n-th pair of ANTENNACUMDIFFSIDEAREARATIO PWL
 *
 * @param index
 *
 * @return
 */
std::pair<float, float>* AntennaModel::getCumDiffSideAreaRatioPWL(UInt32 index) const {
    if (is_cum_diff_side_area_ratio_pwl_ && cum_diff_side_area_ratio_pwl_ && index < cum_diff_side_area_ratio_pwl_->size())
        return &cum_diff_side_area_ratio_pwl_->at(index);
    return nullptr;
}

/**
 * @brief
 * get the size of ANTENNACUMDIFFSIDEAREARATIO PWL
 *
 * @return
 */
UInt32 AntennaModel::getCumDiffSideAreaRatioPWLSize() const {
    return is_cum_diff_side_area_ratio_pwl_ ? cum_diff_side_area_ratio_pwl_->size() : 0;
}

/**
 * @brief
 * add the pair to ANTENNACUMDIFFSIDEAREARATIO PWL
 *
 * @param d
 * @param r
 */
void AntennaModel::addCumDiffSideAreaRatioPWL(float d, float r) {
    if (!is_cum_diff_side_area_ratio_pwl_)
        is_cum_diff_side_area_ratio_pwl_ = 1;
    pushBackPair(cum_diff_side_area_ratio_pwl_, d, r);
}

void CurrentDen::reset() {
    if (frequencies_)
        free(frequencies_);
    if (table_entries_)
        free(table_entries_);
    if (widths_)
        free(widths_);

    memset(static_cast<void*>(this), 0, sizeof(CurrentDen));
}

/**
 * @brief
 * deconstructor to release the allocate memory
 */
CurrentDen::~CurrentDen() {
    reset();
}

/**
 * @brief
 * indicate whether is is ACCURRENTDENSITY PEAK or not
 *
 * @return
 */
bool CurrentDen::isPeak() const {
    return is_peak_;
}

/**
 * @brief
 * set ACCURRENTDENSITY PEAK flag
 *
 * @param v
 */
void CurrentDen::setIsPeak(bool v) {
    is_peak_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether is is ACCURRENTDENSITY AVERAGE or not
 *
 * @return
 */
bool CurrentDen::isAverage() const {
    return is_average_;
}

/**
 * @brief
 * set ACCURRENTDENSITY AVERAGE flag
 *
 * @param v
 */
void CurrentDen::setIsAverage(bool v) {
    is_average_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether it is ACCURRENTDENSITY RMS or not
 *
 * @return
 */
bool CurrentDen::isRMS() const {
    return is_rms_;
}

/**
 * @brief
 * set ACCURRENTDENSITY RMS flag
 *
 * @param v
 */
void CurrentDen::setIsRMS(bool v) {
    is_rms_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether it is ACCURRENTDENSITY or not
 *
 * @return
 */
bool CurrentDen::isAcDen() const {
    return is_ac_den_;
}

/**
 * @brief
 * set ACCURRENTDENSITY flag
 *
 * @param v
 */
void CurrentDen::setIsAcDen(bool v) {
    is_ac_den_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether it is DCCURENTDENSITY or not
 *
 * @return
 */
bool CurrentDen::isDcDen() const {
    return is_dc_den_;
}

/**
 * @brief
 * set DCCURRENTDENSITY flag
 *
 * @param v
 */
void CurrentDen::setIsDcDen(bool v) {
    is_dc_den_ = v ? 1 : 0;
}

/**
 * @brief 
 * indicate whether it is TEMPPWL/HOURSPWL or not 
 *
 * @return 
 */
bool CurrentDen::isPWL() const {
    return is_pwl_;
}

/**
 * @brief 
 * set TEMPPWL or HOURSPWL flag
 *
 * @param v
 */
void CurrentDen::setIsPWL(bool v) {
    is_pwl_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether it is single density value or table entries
 *
 * @return
 */
bool CurrentDen::hasTableEntries() const {
    return table_entries_;
}

/**
 * @brief
 * get single current density
 *
 * @return
 */
float CurrentDen::getCurrentDen() const {
    return table_entries_ ? 0 : current_den_;
}

/**
 * @brief
 * set single current density
 *
 * @param den
 */
void CurrentDen::setCurrentDen(float den) {
    current_den_ = den;
    is_valid_ = 1;
}

/**
 * @brief
 * get frequencies array
 *
 * @return
 */
float* CurrentDen::getFrequencies() const {
    return frequencies_;
}

/**
 * @brief
 * set frequencies array
 *
 * @param frequencies
 * @param freq_num
 */
void CurrentDen::setFrequecies(float* frequencies, UInt32 freq_num) {
    setFrequenciesNum(freq_num);
    if (frequencies_) {
        for (int ii = 0; ii < freq_num; ++ii) {
            frequencies_[ii] = frequencies[ii];
        }
    }
}

/**
 * @brief
 * get the n-th frequency
 *
 * @param index
 *
 * @return
 */
float CurrentDen::getFrequency(UInt32 index) const {
    return index < freq_num_ ? frequencies_[index] : 0;
}

/**
 * @brief
 * set the n-th frequency value
 *
 * @param frequency
 * @param index
 */
void CurrentDen::setFrequency(float frequency, UInt32 index) {
    if (index < freq_num_) {
        frequencies_[index] = frequency;
    }
}

/**
 * @brief
 * get the number of frequencies
 *
 * @return
 */
UInt32 CurrentDen::getFrequenciesNum() const {
    return freq_num_;
}

/**
 * @brief
 * set the number of frequencies
 *
 * @param freq_num
 */
void CurrentDen::setFrequenciesNum(UInt32 freq_num) {
    if (frequencies_) {
        free(frequencies_);
        frequencies_ = nullptr;
        freq_num_ = 0;
    }
    if (freq_num) {
        freq_num_ = freq_num;
        frequencies_ = (float*)calloc(freq_num_, sizeof(float));
    }
}

/**
 * @brief 
 * set the number of temp&multiplier pair
 *
 * @return 
 */
UInt32 CurrentDen::getTempPWLPairNum() const {
    return is_pwl_ ? temp_pwl_pair_num_ : 0;
}

/**
 * @brief 
 * set the number of temp&multiplier pair
 *
 * @param num
 */
void CurrentDen::setTempPWLPairNum(UInt32 num) {
    if (temp_pwl_) {
        free(temp_pwl_);
        temp_pwl_ = nullptr;
        temp_pwl_pair_num_ = 0;
    }
    if (num) {
        temp_pwl_pair_num_ = num;
        // * 2 to reserve temp & temp multiplier
        temp_pwl_ = (float*)calloc(temp_pwl_pair_num_ * 2, sizeof(float));
        is_valid_ = 1;
    }
}

/**
 * @brief 
 * get temp from temp pwl
 *
 * @param index
 *
 * @return 
 */
float CurrentDen::getTempPWLTemp(UInt32 index) const {
    if (is_pwl_) {
        return index <= temp_pwl_pair_num_ ? temp_pwl_[index << 1] : 0;
    }
    return 0;
}

/**
 * @brief 
 * set temp pwl
 *
 * @param t
 * @param index
 */
void CurrentDen::setTempPWLTemp(float t, UInt32 index) {
    if (index >= temp_pwl_pair_num_)
        return;
    temp_pwl_[index << 1] = t;
}

/**
 * @brief 
 * get temp pwl multiplier
 *
 * @param index
 *
 * @return 
 */
float CurrentDen::getTempPWLMultiplier(UInt32 index) const {
    if (is_pwl_) {
        return (index <= temp_pwl_pair_num_) ? temp_pwl_[(index << 1) + 1] : 0;
    }
    return 0;
}

/**
 * @brief 
 * set temp pwl multiplier
 *
 * @param m
 * @param index
 */
void CurrentDen::setTempPWLMultiplier(float m, UInt32 index) {
    if (index >= temp_pwl_pair_num_)
        return;
    temp_pwl_[(index << 1) + 1] = m;
}

/**
 * @brief 
 * set temp pwl temp & multiplier
 *
 * @param t
 * @param m
 * @param index
 */
void CurrentDen::setTempPWL(float t, float m, UInt32 index) {
    if (index >= temp_pwl_pair_num_)
        return;
    temp_pwl_[index << 1] = t;
    temp_pwl_[(index << 1) + 1] = m;
}

/**
 * @brief 
 * get them number of hours&multiplier pair
 *
 * @return 
 */
UInt32 CurrentDen::getHoursPWLPairNum() const {
    return is_pwl_ ? hours_pwl_pair_num_ : 0;
}

/**
 * @brief 
 * set the number of hours&multiplier pair
 *
 * @param num
 */
void CurrentDen::setHoursPWLPairNum(UInt32 num) {
    if (hours_pwl_) {
        free(hours_pwl_);
        hours_pwl_ = nullptr;
        hours_pwl_pair_num_ = 0;
    }
    if (num) {
        hours_pwl_pair_num_ = num;
        // * 2 to reserve hours & temp multiplier
        hours_pwl_ = (float*)calloc(hours_pwl_pair_num_ * 2, sizeof(float));
        is_valid_ = 1;
    }
}

/**
 * @brief 
 * get hourspwl hours
 *
 * @param index
 *
 * @return 
 */
float CurrentDen::getHoursPWLHours(UInt32 index) const {
    if (is_pwl_) {
        return index <= hours_pwl_pair_num_ ? hours_pwl_[index << 1] : 0;
    }
    return 0;
}

/**
 * @brief 
 * set hourspwl hours
 *
 * @param h
 * @param index
 */
void CurrentDen::setHoursPWLHours(float h, UInt32 index) {
    if (index >= hours_pwl_pair_num_)
        return;
    hours_pwl_[index << 1] = h;
}

/**
 * @brief 
 * get hourspwl multiplier
 *
 * @param m
 * @param index
 */
void CurrentDen::setHoursPWLMultiplier(float m, UInt32 index) {
    if (index >= hours_pwl_pair_num_)
        return;
    hours_pwl_[(index << 1) + 1] = m;
}

/**
 * @brief 
 * set hourspwl hours&multiplier
 *
 * @param h
 * @param m
 * @param index
 */
void CurrentDen::setHoursPWL(float h, float m, UInt32 index) {
    if (index >= hours_pwl_pair_num_)
        return;
    hours_pwl_[index << 1] = h;
    hours_pwl_[(index << 1) + 1] = m;
}


/**
 * @brief
 * get widths array
 *
 * @return
 */
UInt32* CurrentDen::getWidths() const {
    return widths_;
}

/**
 * @brief
 * get the n-th width
 *
 * @param index
 *
 * @return
 */
UInt32 CurrentDen::getWidth(UInt32 index) const {
    return index < widths_num_ ? widths_[index] : 0;
}

/**
 * @brief
 * set the n-the width
 *
 * @param width
 * @param index
 */
void CurrentDen::setWidth(UInt32 width, UInt32 index) {
    if (index < widths_num_) {
        widths_[index] = width;
    }
}

/**
 * @brief
 * get the number of widths
 *
 * @return
 */
UInt32 CurrentDen::getWidthsNum() const {
    return widths_num_;
}

/**
 * @brief
 * set the number of widths
 *
 * @param widths_num
 */
void CurrentDen::setWidthsNum(UInt32 widths_num) {
    if (widths_) {
        free(widths_);
        widths_ = nullptr;
        widths_num_ = 0;
    }
    if (widths_num) {
        widths_num_ = widths_num;
        widths_ = (UInt32*)calloc(widths_num_, sizeof(UInt32));
    }
}

/**
 * @brief
 * get cutareas array
 *
 * @return
 */
float* CurrentDen::getCutAreas() const {
    return cut_areas_;
}

/**
 * @brief
 * get the n-th cut area
 *
 * @param index
 *
 * @return
 */
float CurrentDen::getCutArea(UInt32 index) const {
    return index < cut_areas_num_ ? cut_areas_[index] : 0;
}

/**
 * @brief
 * get the number of cut areas
 *
 * @return
 */
UInt32 CurrentDen::getCutAreasNum() const {
    return cut_areas_num_;
}

/**
 * @brief
 * set the number of cut areas
 *
 * @param cut_areas_num
 */
void CurrentDen::setCutAreasNum(UInt32 cut_areas_num) {
    if (cut_areas_) {
        free(cut_areas_);
        cut_areas_ = nullptr;
        cut_areas_num_ = 0;
    }
    if (cut_areas_num) {
        cut_areas_num_ = cut_areas_num;
        cut_areas_ = (float*)calloc(cut_areas_num_, sizeof(float));
    }
}

/**
 * @brief
 * set the n-th cut area
 *
 * @param cut_area
 * @param index
 */
void CurrentDen::setCutArea(float cut_area, UInt32 index) {
    if (index < cut_areas_num_) {
        cut_areas_[index] = cut_area;
    }
}

/**
 * @brief
 * get the density on the specified row and col in the table
 *
 * @param row_idx
 * @param col_idx
 *
 * @return
 */
float CurrentDen::getTableElem(UInt32 row_idx, UInt32 col_idx) const {
    int idx = row_idx * widths_num_ + col_idx;
    return idx < table_entries_num_ ? table_entries_[idx] : 0;
}

/**
 * @brief
 * set the density on the specified row and col in the table
 *
 * @param value
 * @param row_idx
 * @param col_idx
 */
void CurrentDen::setTableElem(float value, UInt32 row_idx, UInt32 col_idx) {
    if (!table_entries_) {
        table_entries_num_ = (freq_num_ ? freq_num_ : 1) * (widths_num_ ? widths_num_ : 1);
        table_entries_ = (float*)calloc(table_entries_num_, sizeof(float));
        is_valid_ = 1;
    }

    int idx = row_idx * widths_num_ + col_idx;
    if (idx < table_entries_num_) {
        table_entries_[idx] = value;
    }
}

/**
 * @brief
 * indicate whether this object is valid current density or not
 *
 * @return
 */
bool CurrentDen::isValid() const {
    return is_valid_;
}

/**
 * @brief
 * set object invalidation
 *
 * @param v
 */
void CurrentDen::setIsValid(bool v) {
    is_valid_ = v ? 1 : 0;
}

/**
 * @brief
 * get ACCURRENTDENSITY PEAK
 *
 * @return
 */
ACCurrentDen* CurrentDenContainer::getACPeak() const {
    ACCurrentDen* den =  current_dens_ + CurrentDenContainer::kCurrentPeak;
    return den->isValid() ? den : nullptr;
}

/**
 * @brief
 * get ACCURRENTDENSITY AVERAGE
 *
 * @return
 */
ACCurrentDen* CurrentDenContainer::getACAverage() const {
    ACCurrentDen* den =  current_dens_ + CurrentDenContainer::kCurrentAverage;
    return den->isValid() ? den : nullptr;
}

/**
 * @brief
 * get ACCURRENTDENSITY RMS
 *
 * @return
 */
ACCurrentDen* CurrentDenContainer::getACRMS() const {
    ACCurrentDen* den =  current_dens_ + CurrentDenContainer::kCurrentRMS;
    return den->isValid() ? den : nullptr;
}

/**
 * @brief 
 * get ACCURRENTDENSITY PEAK PWL
 *
 * @return 
 */
ACCurrentDen* CurrentDenContainer::getACPeakPWL() const {
    return getACPeak();
}

/**
 * @brief
 * get ACCURRENTDENSITY AVERAGE PWL
 *
 * @return 
 */
ACCurrentDen* CurrentDenContainer::getACAveragePWL() const {
    return getACAverage();
}

/**
 * @brief 
 * get ACCURRENTDENSITY RMS PWL
 *
 * @return 
 */
ACCurrentDen* CurrentDenContainer::getACRMSPWL() const {
    return getACRMS();
}

/**
 * @brief
 * get DCCURRENTDENSITY AVERAGE
 *
 * @return
 */
DCCurrentDen* CurrentDenContainer::getDCAverage() const {
    DCCurrentDen* den =  current_dens_ + CurrentDenContainer::kCurrentAverage;
    return den->isValid() ? den : nullptr;
}

/**
 * @brief
 * get ACCURRENTDENSITY PEAK to set it, should be used only when lefIn
 * if it is not set, it is always undefined and invalid
 *
 * @return
 */
ACCurrentDen* CurrentDenContainer::getInitACPeak() const {
    return current_dens_ + CurrentDenContainer::kCurrentPeak;
}

/**
 * @brief
 * get ACCURRENTDENSITY PEAK PWL to set it, should be used only when lefIn
 *
 * @return 
 */
ACCurrentDen* CurrentDenContainer::getInitACPeakPWL() const {
    return getInitACPeak();
}

/**
 * @brief
 * get ACCURRENTDENSITY AVERAGE to set it, should be used only when lefIn
 *
 * @return
 */
ACCurrentDen* CurrentDenContainer::getInitACAverage() const {
    return current_dens_ + CurrentDenContainer::kCurrentAverage;
}

/**
 * @brief
 * get ACCURRENTDENSITY AVERAGE PWL to set it, should be used only when lefIn
 *
 * @return 
 */
ACCurrentDen* CurrentDenContainer::getInitACAveragePWL() const {
    return getInitACAverage();
}

/**
 * @brief
 * get ACCURRENTDENSITY RMS to set it, should be used only when lefIn
 *
 * @return
 */
ACCurrentDen* CurrentDenContainer::getInitACRMS() const {
    return current_dens_ + CurrentDenContainer::kCurrentRMS;
}

/**
 * @brief 
 * get ACCURRENTDENSITY RMS PWL to set it, should be used only when lefIn 
 *
 * @return 
 */
ACCurrentDen* CurrentDenContainer::getInitACRMSPWL() const {
    return getInitACRMS();
}

/**
 * @brief
 * get DCCURRENTDENSITY AVERAGE to set it only when lefIn
 *
 * @return
 */
DCCurrentDen* CurrentDenContainer::getInitDCAverage() const {
    return current_dens_ + CurrentDenContainer::kCurrentAverage;
}

/**
 * @brief
 * get min area
 *
 * @return
 */
double MinArea::getArea() const {
    return min_area_;
}

/**
 * @brief
 * set min area
 *
 * @param area
 */
void MinArea::setArea(double area) {
    min_area_ = area;
}

/**
 * @brief
 * get mask num
 *
 * @return
 */
UInt32 MinArea::getMaskNum() const {
    return mask_num_;
}

/**
 * @brief
 * set mask num
 *
 * @param num
 */
void MinArea::setMaskNum(UInt32 num) {
    mask_num_ = num;
}

/**
 * @brief
 * indicate whether EXCEPTMINWIDTH is specified or not
 *
 * @return
 */
bool MinArea::isExceptMinWidth() const {
    return is_except_min_width_;
}

/**
 * @brief
 * set EXCEPTMINWIDTH flag
 *
 * @param v
 */
void MinArea::setIsExceptMinWidth(bool v) {
    is_except_min_width_ = v ? 1 : 0;
}

/**
 * @brief
 * get EXCEPTMINWIDTH minWidth
 *
 * @return
 */
UInt32 MinArea::getMinWidth() const {
    return min_width_;
}

/**
 * @brief
 * set EXCEPTMINWIDTH minWidth
 *
 * @param w
 */
void MinArea::setMinWidth(UInt32 w) {
    min_width_ = w;
}

/**
 * @brief
 * indicate whether EXCEPTEDGELENGTH is specified or not
 *
 * @return
 */
bool MinArea::isExceptEdgeLength() const {
    return is_except_edge_length_;
}

/**
 * @brief
 * set EXCEPTEDGELENGTH flag
 *
 * @param v
 */
void MinArea::setIsExceptEdgeLength(bool v) {
    is_except_edge_length_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether it is EXCEPTEDGELENGTH minEdgeLength maxEdgeLength or not
 *
 * @return
 */
bool MinArea::isExceptMinMaxEdgeLength() const {
    return is_min_max_edge_length_;
}

/**
 * @brief
 * set the flag of EXCEPTEDGELENGTH minEdgeLength maxEdgeLength
 *
 * @param v
 */
void MinArea::setIsExceptMinMaxEdgeLength(bool v) {
    is_min_max_edge_length_ = v ? 1 : 0;
}

/**
 * @brief
 * get EXCEPTEDGELENGTH minLength or EXCEPTEDGELENGTH edgeLength
 *
 * @return
 */
UInt32 MinArea::getExceptMinEdgeLength() const {
    return except_edge_min_length_;
}

/**
 * @brief
 * set EXCEPTEDGELENGTH minLength
 *
 * @param len
 */
void MinArea::setExceptMinEdgeLength(UInt32 len) {
    except_edge_min_length_ = len;
}

/**
 * @brief
 * get EXCEPTEDGELENGTH x maxEdgeLength
 *
 * @return
 */
UInt32 MinArea::getExceptMaxEdgeLength() const {
    return except_edge_max_length_;
}

/**
 * @brief
 * set EXCEPTEDGELENGTH x maxEdgeLength
 *
 * @param len
 */
void MinArea::setExceptMaxEdgeLength(UInt32 len) {
    except_edge_max_length_ = len;
}

/**
 * @brief
 * get EXCEPTEDGELENGTH edgeLength
 *
 * @return
 */
UInt32 MinArea::getExceptEdgeLength() const {
    return except_edge_min_length_;
}

/**
 * @brief
 * set EXCEPTEDGELENGTH edgeLength
 *
 * @param len
 */
void MinArea::setExceptEdgeLength(UInt32 len) {
    except_edge_min_length_ = except_edge_max_length_ = len;
    is_min_max_edge_length_ = 0;
}

/**
 * @brief
 * set EXCEPTEDGELENGTH minEdgeLength maxEdgeLength
 *
 * @param min_len
 * @param max_len
 */
void MinArea::setExceptMinMaxEdgeLength(UInt32 min_len, UInt32 max_len) {
    is_min_max_edge_length_ = 1;
    except_edge_min_length_ = min_len;
    except_edge_max_length_ = max_len;
}

/**
 * @brief
 * indicate whether EXCEPTMINSIZE is specified or not
 *
 * @return
 */
bool MinArea::isExceptMinSize() const {
    return is_except_min_size_;
}

/**
 * @brief
 * set EXCEPTMINSIZE flag
 *
 * @param v
 */
void MinArea::setIsExceptMinSize(bool v) {
    is_except_min_size_ = v ? 1 : 0;
}

/**
 * @brief
 * get the n-th minsize pair
 *
 * @param index
 *
 * @return
 */
std::pair<UInt32, UInt32>* MinArea::getExceptMinSizePair(UInt32 index) {
    return (is_except_min_size_ && (index < except_min_size_.size())) ? &except_min_size_[index] : nullptr;
}

/**
 * @brief
 * append one minsize pair
 *
 * @param min_width
 * @param min_len
 */
void MinArea::appendExceptMinSizePair(UInt32 min_width, UInt32 min_len) {
    is_except_min_size_ = 1;
    except_min_size_.push_back({min_width, min_len});
}

/**
 * @brief
 * get the size of EXCEPTMINSIZE pairs
 *
 * @return
 */
UInt32 MinArea::getExceptMinSizeSize() const {
    return except_min_size_.size();
}

/**
 * @brief 
 * get EXCEPTSTEP flag
 *
 * @return 
 */
bool MinArea::isExceptStep() const {
    return is_except_step_;
}

/**
 * @brief 
 * set EXCEPTSTEP flag
 *
 * @param v
 */
void MinArea::setIsExceptStep(bool v) {
    is_except_step_ = v;
}

/**
 * @brief 
 * get EXCEPTSTEP length1
 *
 * @return 
 */
UInt32 MinArea::getExceptStepLength1() const {
    return except_step_length1_;
}

/**
 * @brief 
 * set EXCEPTSTEP length1
 *
 * @param len
 */
void MinArea::setExceptStepLength1(UInt32 len) {
    except_step_length1_ = len;
}

/**
 * @brief 
 * get EXCEPTSTEP x length2
 *
 * @return 
 */
UInt32 MinArea::getExceptStepLength2() const {
    return except_step_length2_;
}

/**
 * @brief 
 * set EXCEPTSTEP x length2
 *
 * @param len
 */
void MinArea::setExceptStepLength2(UInt32 len) {
    except_step_length2_ = len;
}

/**
 * @brief
 * indicate whether RECTWIDTH is specified or not
 *
 * @return
 */
bool MinArea::isRectWidth() const {
    return is_rect_width_;
}

/**
 * @brief
 * set RECTWIDTH flag
 *
 * @param v
 */
void MinArea::setIsRectWidth(bool v) {
    is_rect_width_ = v ? 1 : 0;
}

/**
 * @brief
 * get RECTWIDTH rectWidth
 *
 * @return
 */
UInt32 MinArea::getRectWidth() const {
    return rect_width_;
}

/**
 * @brief
 * set RECTWIDTH rectWidth
 *
 * @param w
 */
void MinArea::setRectWidth(UInt32 w) {
    rect_width_ = w;
}

/**
 * @brief
 * indicate whether EXCEPTRECTANGLE is specified or not
 *
 * @return
 */
bool MinArea::isExceptRectangle() const {
    return is_except_rectangle_;
}

/**
 * @brief
 * set EXCEPTRECTANGLE flag
 *
 * @param v
 */
void MinArea::setIsExceptRectangle(bool v) {
    is_except_rectangle_ = v ? 1 : 0;
}

/**
 * @brief
 * indicate whether LAYER is specified or not
 *
 * @return
 */
bool MinArea::isLayer() const {
    return is_layer_;
}

/**
 * @brief
 * set LAYER trimLayer
 *
 * @param l
 */
void MinArea::setLayer(Layer* l) {
    is_layer_ = 1;
    layer_ = l;
}

/**
 * @brief
 * get LAYER x OVERLAP
 *
 * @return
 */
UInt32 MinArea::getOverlap() const {
    return layer_overlap_;
}

/**
 * @brief
 * set LAYER x OVERLAP
 *
 * @param ol
 */
void MinArea::setOverlap(UInt32 ol) {
    layer_overlap_ = (ol & 0x3); // layer_overlap_ occupies 2 Bits
}

/// @brief Layer 
Layer::Layer()
{
    
}

/**
 * @brief 
 * get trim layer
 *
 * @return Layer
 */
Layer* MinArea::getLayer() const {
    return layer_;
}

/// @brief ~Layer 
Layer::~Layer() {
    if (isImplant()) {
        delete layer_rule_.implant_;
    }
    if (isRouting()) {
        delete layer_rule_.routing_;
    }
    if (isCut()) {
        delete layer_rule_.cut_;
    }
    if (isTrimMetal()) {
        delete layer_rule_.trim_;
    }
    if (isMEOL()) {
        delete layer_rule_.meol_;
    }
    if (ac_dens_) {
        delete ac_dens_;
    }
    if (dc_dens_) {
        delete dc_dens_;
    }
}

/// @brief getTech_ 
///
/// @return 
Tech *Layer::getTech_()
{
    if (nullptr != tech_) return tech_;
    tech_ = addr<Tech>(getOwnerId());
    return tech_;
}

/// @brief getNameId 
///
/// @return 
uint64_t Layer::getNameId() const {
    return nameId_;
}

/// @brief getName 
///
/// @return 
const char* Layer::getName() {
    return getTech_()->getSymbolByIndex(nameId_).c_str();
}

/**
 * @brief
 *
 * @param id
 */
void Layer::setNameId(uint64_t id) {
    nameId_ = id;
}

/// @brief setName 
///
/// @param name
///
/// @return 
bool Layer::setName(const char *name) {
    SymbolIndex sym_id = getTech_()->getOrCreateSymbol(name);
    if (sym_id != kInvalidSymbolIndex) {
        setNameId(sym_id);
        return true;
    }
    return false;
}

/**
 * @brief getZ
 *
 * @return
 */
UInt32 Layer::getZ() const {
    return z_;
}

/**
 * @brief setZ
 *
 * @param z
 */
void Layer::setZ(UInt32 z) {
    z_ = z;
}

/**
 * @brief getIndexInLef
 * get the index of layer in the LEF
 *
 * @return
 */
UInt32 Layer::getIndexInLef() const {
    return lef_order_;
}

/**
 * @brief
 * get the index of layer in the LEF
 *
 * @param idx
 */
void Layer::setIndexInLef(UInt32 idx) {
    lef_order_ = idx;
}

/**
 * @brief
 * get layer type
 *
 * @return LayerType
 */
LayerType Layer::getType() const {
    return static_cast<LayerType>(type_);
}

/**
 * @brief
 * set layer type
 *
 * @param type
 */
void Layer::setType(LayerType type) {
    type_ = type;
}

/**
 * @brief
 * indicate whether layer is a implant layer or not
 *
 * @return
 */
bool Layer::isImplant() const {
    return getType() == kLayerImplant;
}

/**
 * @brief
 * indicate whether layer is a masterslice layer or not
 *
 * @return
 */
bool Layer::isMS() const {
    return getType() == kLayerMS;
}

/**
 * @brief
 * indicate whether layer is an overlap layer or not
 *
 * @return
 */
bool Layer::isOverlap() const {
    return getType() == kLayerOverlap;
}

/**
 * @brief
 * indicate whether layer is a cut layer or not
 *
 * @return
 */
bool Layer::isCut() const {
    return getType() == kLayerCut;
}

/**
 * @brief
 * indicate whether layer is a routing layer or not
 *
 * @return
 */
bool Layer::isRouting() const {
    return getType() == kLayerRouting;
}

/**
 * @brief
 * indicate whether layer is a TSV layer or not
 *
 * @return
 */
bool Layer::isTSV() const {
    return getType() == kLayerTSV;
}

/**
 * @brief
 * indicate whether layer is a passivation layer or not
 *
 * @return
 */
bool Layer::isPassivation() const {
    return getType() == kLayerPassivation;
}

/**
 * @brief
 * indicate whether layer is a MIMCap cut layer or not
 *
 * @return
 */
bool Layer::isMIMCapCut() const {
    return getType() == kLayerMIMCapCut;
}

/**
 * @brief
 * indicate whether layer is a high resistance cut layer or not
 *
 * @return
 */
bool Layer::isHighrCut() const {
    return getType() == kLayerHighrCut;
}

/**
 * @brief
 * indicate whether layer is a cut layer with region-based (or area-based) layer or not
 *
 * @return
 */
bool Layer::isRegionCut() const {
    return getType() == kLayerRegionCut;
}

/**
 * @brief
 * indicate whether layer is a region layer or not
 *
 * @return
 */
bool Layer::isRegion() const {
    return getType() == kLayerRegion;
}

/**
 * @brief
 * indicate whether it is a NWell layer or not
 *
 * @return
 */
bool Layer::isNWell() const {
    return getType() == kLayerNWell;
}

/**
 * @brief
 * indicate whether it is a PWell layer or not
 *
 * @return
 */
bool Layer::isPWell() const {
    return getType() == kLayerPWell;
}

/**
 * @brief
 * indicate whether it is ABOVEDIEEDGE layer or not
 *
 * @return
 */
bool Layer::isAboveDieEdge() const {
    return getType() == kLayerAboveDieEdge;
}

/**
 * @brief
 * indicate whether it is BELOWDIEEDIGE layer or not
 *
 * @return
 */
bool Layer::isBelowDieEdge() const {
    return getType() == kLayerBelowDieEdge;
}

/**
 * @brief
 * indicate whether it is a diffusion layer or not
 *
 * @return
 */
bool Layer::isDiffusion() const {
    return getType() == kLayerDiffusion;
}

/**
 * @brief
 * indicate whether it is a trim poly layer or not
 *
 * @return
 */
bool Layer::isTrimPoly() const {
    return getType() == kLayerTrimPoly;
}

/**
 * @brief
 * indicate whether it is a trim metal layer or not
 *
 * @return
 */
bool Layer::isTrimMetal() const {
    return getType() == kLayerTrimMetal;
}

/**
 * @brief
 * indicate whether it is a MEOL layer or not
 *
 * @return
 */
bool Layer::isMEOL() const {
    return getType() == kLayerMEOL;
}

/**
 * @brief
 * indicate whether it is trim metal region layer or not
 *
 * @return
 */
bool Layer::isRegionTrimMetal() const {
    return getType() == kLayerRegionTrimMetal;
}

/**
 * @brief
 * indicate whether it is a poly routing layer or not
 *
 * @return
 */
bool Layer::isPolyRouting() const {
    return getType() == kLayerPolyRouting;
}

/**
 * @brief
 * indicate whether it is mimcap metal layer or not
 *
 * @return
 */
bool Layer::isMIMCapRouting() const {
    return getType() == kLayerMIMCapRouting;
}

/**
 * @brief
 * indicate whether it is a high resistance routing layer or not
 *
 * @return
 */
bool Layer::isHighrRouting() const {
    return getType() == kLayerHighrRouting;
}

/**
 * @brief
 * indicate whether it is a TSVMETAL layer
 *
 * @return
 */
bool Layer::isTSVMetalRouting() const {
    return getType() == kLayerTSVMetalRouting;
}

/**
 * @brief
 * indicate whether it is a PADMETAL layer
 *
 * @return
 */
bool Layer::isPadMetalRouting() const {
    return getType() == kLayerPadMetalRouting;
}

/**
 * @brief
 * indicate whether it is a STACKEDMIMCAP layer
 *
 * @return
 */
bool Layer::isStackedMIMCapRouting() const {
    return getType() == kLayerStackedMIMCapRouting;
}

/**
 * @brief
 * indicate whether it is a routing region layer or not
 *
 * @return
 */
bool Layer::isRegionRouting() const {
    return getType() == kLayerRegionRouting;
}

/**
 * @brief
 * get number of mask
 *
 * @return
 */
UInt32 Layer::getNumMask() const {
    return num_mask_;
}

/**
 * @brief
 * set number of mask
 *
 * @param num
 */
void Layer::setNumMask(UInt32 num) {
    num_mask_ = num;
}

/**
 * @brief
 * indicate whether it is a backside layer or not
 *
 * @return
 */
bool Layer::isBackside() const {
    return backside_;
}

/**
 * @brief
 * set backside layer flag
 *
 * @param b
 */
void Layer::setIsBackside(bool b) {
    backside_ = b ? 1 : 0;
}

/**
 * @brief
 * indicate whether it is a customized layer or not
 *
 * @return
 */
bool Layer::isCustomized() const {
    return customized_;
}

/**
 * @brief
 * set customized layer flag
 *
 * @param b
 */
void Layer::setIsCustomized(bool b) {
    customized_ = b ? 1 : 0;
}

/**
 * @brief
 * indicate whether mask is fixed or not
 *
 * @return
 */
bool Layer::isFixedMask() const {
    return fixed_mask_;
}

/**
 * @brief
 * set fixed mask flag
 *
 * @param v
 */
void Layer::setIsFixedMask(bool v) {
    fixed_mask_ = v ? 1 : 0;
}

/**
 * @brief
 * get the number of fixed mask
 *
 * @return
 */
UInt32 Layer::getFixedMaskNum() const {
    return fixed_mask_num_;
}

/**
 * @brief
 * set the number of fixed mask
 *
 * @param num
 */
void Layer::setFixedMaskNum(UInt32 num) {
    fixed_mask_num_ = num;
}

/**
 * @brief
 * get default wire width on routing layer or min square cut size on cut layer
 *
 * @return
 */
UInt32 Layer::getWidth() const {
    return width_;
}

/**
 * @brief
 * set default wire width on routing layer or min square cut size on cut layer
 *
 * @param w
 */
void Layer::setWidth(UInt32 w) {
    width_ = w;
}

/**
 * @brief 
 * create and reserve minarea to array object, and return the created minarea
 *
 * @return MinArea
 */
MinArea* Layer::createMinArea() {
    MinArea* ma = Object::createObject<MinArea>(
          kObjectTypeLayerMinArea, getTechLib()->getId());
    if (!ma)
      return nullptr;
    IdArray *ma_vector = nullptr;
    if (min_area_id_ == 0) {
      min_area_id_ = __createObjectIdArray(8);
    }
    ma_vector = addr<IdArray>(min_area_id_);
    ediAssert(ma_vector != nullptr);
    ma_vector->pushBack(ma->getId());
    return ma;
}

/**
 * @brief 
 * get object id of minarea rules
 * foreach minarea by getNext of Layer::minAreaIter
 *
 * @return 
 */
ObjectId Layer::getMinAreaRuleId() const {
    return min_area_id_;
}

/**
 * @brief
 * get the region layer that is an independent routing layer, cut layer or trim layer
 *
 * @return
 */
Layer* Layer::getRegionLayer() const {
    return (isRegionRouting() || isRegionCut() || isRegionTrimMetal()) ? region_layer_ : nullptr;
}

/**
 * @brief
 * set region layer
 *
 * @param l
 */
void Layer::setRegionLayer(Layer* l) {
    region_layer_ = l;
}

/**
 * @brief
 * get base layer of region layer
 *
 * @return
 */
Layer* Layer::getRegionBaseLayer() const {
    return (isRegionRouting() || isRegionCut() || isRegionTrimMetal()) ? base_layer_ : nullptr;
}

/**
 * @brief
 * set region base layer
 *
 * @param l
 */
void Layer::setRegionBaseLayer(Layer* l) {
    base_layer_ = l;
}

/**
 * @brief
 * get antenna model list
 *
 * @return
 */
const AntennaModel* Layer::getAntennaModel(UInt32 index) const {
    if (index > 0 && index < kMaxOxideNum) {
        return antenna_[index].isSet() ? &antenna_[index] : nullptr;
    }
    return nullptr;
}

/**
 * @brief
 * used only for init antenna model from lef parser
 *
 * @param index
 *
 * @return
 */
AntennaModel* Layer::getInitAntennaModel(UInt32 index) {
    if (index > 0 && index < kMaxOxideNum) {
        return &antenna_[index];
    }
    return nullptr;
}

/**
 * @brief
 * get ACCURRENTDENSITY
 *
 * @return
 */
CurrentDenContainer* Layer::getACCurrentDenContainer() const {
    return ac_dens_;
}

/**
 * @brief
 * set ACCURRENTDENSITY
 *
 * @param den
 */
void Layer::setACCurrentDenContainer(CurrentDenContainer* den) {
    ac_dens_ = den;
}

/**
 * @brief
 * get DCCURRENTDENSITY
 *
 * @return
 */
CurrentDenContainer* Layer::getDCCurrentDenContainer() const {
    return dc_dens_;
}

/**
 * @brief
 * set DCCURRENTDENSITY
 *
 * @param den
 */
void Layer::setDCCurrentDenContainer(CurrentDenContainer* den) {
    dc_dens_ = den;
}

/**
 * @brief
 * indicate whether it is a general routing layer or not (not only signal routing)
 *
 * @return
 */
bool Layer::isGeneralRoutingLayer_() const {
    return isRouting() || isMIMCapRouting() || isPolyRouting() || isHighrRouting() || isTSVMetalRouting() || isPadMetalRouting() || isStackedMIMCapRouting() || isRegionRouting();
}

/**
 * @brief
 * indicate whether it is a general cut layer or not
 *
 * @return
 */
bool Layer::isGeneralCutLayer_() const {
    return isCut() || isMIMCapCut() || isHighrCut() || isRegionCut();
}

/**
 * @brief
 * get implant layer
 *
 * @return
 */
ImplantLayerRule* Layer::getImplantLayerRule() const {
    return isImplant() ? layer_rule_.implant_ : nullptr;
}

/**
 * @brief
 * set implant layer rule
 *
 * @param r
 */
void Layer::setImplantLayerRule(ImplantLayerRule* r) {
    layer_rule_.implant_ = r;
}

/**
 * @brief
 * get routing layer rules
 *
 * @return
 */
RoutingLayerRule* Layer::getRoutingLayerRule() const {
    return isGeneralRoutingLayer_() ? layer_rule_.routing_ : nullptr;
}

/**
 * @brief
 * set routing layer rule
 *
 * @param r
 */
void Layer::setRoutingLayerRule(RoutingLayerRule* r) {
    layer_rule_.routing_ = r;
}

/**
 * @brief
 * get cut layer rules
 *
 * @return
 */
CutLayerRule* Layer::getCutLayerRule() const {
    return isGeneralCutLayer_() ? layer_rule_.cut_ : nullptr;
}

/**
 * @brief
 * set cut layer rule
 *
 * @param r
 */
void Layer::setCutLayerRule(CutLayerRule* r) {
    layer_rule_.cut_ = r;
}


/**
 * @brief
 * get trim layer rule
 *
 * @return TrimLayerRule
 */
TrimLayerRule* Layer::getTrimLayerRule() const {
    return (isTrimMetal() || isRegionTrimMetal()) ? layer_rule_.trim_ : nullptr;
}

/**
 * @brief
 * set trim layer rule
 *
 * @param r
 */
void Layer::setTrimLayerRule(TrimLayerRule* r) {
    layer_rule_.trim_ = r;
}

/**
 * @brief
 * get MEOL layer rule
 *
 * @return
 */
MEOLLayerRule* Layer::getMEOLLayerRule() const {
    return isMEOL() ? layer_rule_.meol_ : nullptr;
}

/**
 * @brief
 * set MEOL layer rule
 *
 * @param r
 */
void Layer::setMEOLLayerRule(MEOLLayerRule* r) {
    layer_rule_.meol_ = r;
}

/**
 * @brief
 * indicate whether routing layer is vertical or not
 *
 * @return
 */
bool Layer::isVert() const {
    return getRoutingLayerRule() ? getRoutingLayerRule()->isVert() : false;
}

/**
 * @brief
 * indicate whether routing layer is horizontal or not
 *
 * @return
 */
bool Layer::isHoriz() const {
    return getRoutingLayerRule() ? getRoutingLayerRule()->isHoriz() : false;
}

/**
 * @brief
 * indicate whether routing layer is 45-degree or not
 *
 * @return
 */
bool Layer::isDiag45() const {
    return getRoutingLayerRule() ? getRoutingLayerRule()->isDiag45() : false;
}

/**
 * @brief
 * indicate whether routing layer is 135-degree or not
 *
 * @return
 */
bool Layer::isDiag135() const {
    return getRoutingLayerRule() ? getRoutingLayerRule()->isDiag135() : false;
}

/**
 * @brief
 * set routing layer direction
 *
 * @param v
 */
void Layer::setDirection(UInt32 v) {
    if (getRoutingLayerRule()) {
        getRoutingLayerRule()->setDirection(v);
    }
}

void Layer::addProp(ObjectId obj_id) {
    if (obj_id == 0) return;
    IdArray *vobj = nullptr;

    if (properties_ == 0) {
        properties_ = __createObjectIdArray(16);
    }
    vobj = addr<IdArray>(properties_);
    ediAssert(vobj != nullptr);
    vobj->pushBack(obj_id);
}

ObjectId Layer::getPropId() const {
    return properties_;
}

}  // namespace db
}  // namespace open_edi
