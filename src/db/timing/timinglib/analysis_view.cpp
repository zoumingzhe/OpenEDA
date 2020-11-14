/**
 * @file analysis_view.cpp
 * @date 2020-09-08
 * @brief
 *
 * Copyright (C) 2020 NIIC EDA
 *
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 *
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/timing/timinglib/analysis_view.h"

#include "db/core/db.h"
#include "db/timing/timinglib/analysis_corner.h"
#include "db/timing/timinglib/analysis_mode.h"

namespace open_edi {
namespace db {

AnalysisView::AnalysisView()
    : AnalysisView::BaseType(),
      name_(0),
      analysis_corner_(UNINIT_OBJECT_ID),
      analysis_mode_(UNINIT_OBJECT_ID),
      is_active_(false),
      is_setup_(false),
      is_hold_(false),
      is_max_cap_(false),
      is_max_tran_(false),
      is_min_cap_(false),
      is_leakage_power_(false),
      is_dynamic_power_(false),
      is_cell_em_(false),
      is_signal_em_(false) {
    setObjectType(ObjectType::kObjectTypeAnalysisView);
}

AnalysisView::~AnalysisView() {
#if 0
    if (analysis_corner_ != UNINIT_OBJECT_ID) {
        AnalysisCorner* analysis_corner = dynamic_cast<AnalysisCorner*>(
            getObject<AnalysisCorner>(analysis_corner_));
        if (analysis_corner) deleteObject<AnalysisCorner>(analysis_corner);
    }

    if (analysis_mode_ != UNINIT_OBJECT_ID) {
        AnalysisMode* analysis_mode = dynamic_cast<AnalysisMode*>(
            getObject<AnalysisMode>(analysis_mode_));
        if (analysis_mode) deleteObject<AnalysisMode>(analysis_mode);
    }
#endif
}

AnalysisView::AnalysisView(Object* owner, AnalysisView::IndexType id)
    : AnalysisView::BaseType(owner, id),
      name_(0),
      analysis_corner_(UNINIT_OBJECT_ID),
      analysis_mode_(UNINIT_OBJECT_ID),
      is_active_(false),
      is_setup_(false),
      is_hold_(false),
      is_max_cap_(false),
      is_max_tran_(false),
      is_min_cap_(false),
      is_leakage_power_(false),
      is_dynamic_power_(false),
      is_cell_em_(false),
      is_signal_em_(false) {
    setObjectType(ObjectType::kObjectTypeAnalysisView);
}

AnalysisView::AnalysisView(AnalysisView const& rhs) { copy(rhs); }

AnalysisView::AnalysisView(AnalysisView&& rhs) noexcept {
    move(std::move(rhs));
}

AnalysisView& AnalysisView::operator=(AnalysisView const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

AnalysisView& AnalysisView::operator=(AnalysisView&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void AnalysisView::copy(AnalysisView const& rhs) {
    this->BaseType::copy(rhs);
    name_ = rhs.name_;
    analysis_corner_ = rhs.analysis_corner_;
    analysis_mode_ = rhs.analysis_mode_;
    is_active_ = rhs.is_active_;
    is_setup_ = rhs.is_setup_;
    is_hold_ = rhs.is_hold_;
    is_max_cap_ = rhs.is_max_cap_;
    is_max_tran_ = rhs.is_max_tran_;
    is_min_cap_ = rhs.is_min_cap_;
    is_leakage_power_ = rhs.is_leakage_power_;
    is_dynamic_power_ = rhs.is_dynamic_power_;
    is_cell_em_ = rhs.is_cell_em_;
    is_signal_em_ = rhs.is_signal_em_;
}

void AnalysisView::move(AnalysisView&& rhs) {
    this->BaseType::move(std::move(rhs));
    name_ = std::move(rhs.name_);
    analysis_corner_ = std::move(rhs.analysis_corner_);
    rhs.analysis_corner_ = 0;
    analysis_mode_ = std::move(rhs.analysis_mode_);
    rhs.analysis_mode_ = 0;
    is_active_ = std::move(rhs.is_active_);
    is_setup_ = std::move(rhs.is_setup_);
    is_hold_ = std::move(rhs.is_hold_);
    is_max_cap_ = std::move(rhs.is_max_cap_);
    is_max_tran_ = std::move(rhs.is_max_tran_);
    is_min_cap_ = std::move(rhs.is_min_cap_);
    is_leakage_power_ = std::move(rhs.is_leakage_power_);
    is_dynamic_power_ = std::move(rhs.is_dynamic_power_);
    is_cell_em_ = std::move(rhs.is_cell_em_);
    is_signal_em_ = std::move(rhs.is_signal_em_);
}

AnalysisView::IndexType AnalysisView::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(name_);
    ret += sizeof(analysis_corner_);
    ret += sizeof(analysis_mode_);
    ret += sizeof(is_active_);
    ret += sizeof(is_setup_);
    ret += sizeof(is_hold_);
    ret += sizeof(is_max_cap_);
    ret += sizeof(is_max_tran_);
    ret += sizeof(is_min_cap_);
    ret += sizeof(is_leakage_power_);
    ret += sizeof(is_dynamic_power_);
    ret += sizeof(is_cell_em_);
    ret += sizeof(is_signal_em_);

    return ret;
}

/// set
void AnalysisView::set_name(const std::string& name) {
    Cell* topCell = getTopCell();
    if (topCell) {
        SymbolIndex idx = topCell->getOrCreateSymbol(name.c_str());
        if (idx != kInvalidSymbolIndex) {
            name_ = idx;
            topCell->addSymbolReference(name_, this->getId());
        }
    }
}
void AnalysisView::set_analysis_corner(ObjectId id) { analysis_corner_ = id; }
void AnalysisView::set_analysis_mode(ObjectId id) { analysis_mode_ = id; }
void AnalysisView::set_active(bool b) { is_active_ = b; }
void AnalysisView::set_setup(bool b) { is_setup_ = b; }
void AnalysisView::set_hold(bool b) { is_hold_ = b; }
void AnalysisView::set_max_cap(bool b) { is_max_cap_ = b; }
void AnalysisView::set_max_tran(bool b) { is_max_tran_ = b; }
void AnalysisView::set_min_cap(bool b) { is_min_cap_ = b; }
void AnalysisView::set_leakage_power(bool b) { is_leakage_power_ = b; }
void AnalysisView::set_dynamic_power(bool b) { is_dynamic_power_ = b; }
void AnalysisView::set_cell_em(bool b) { is_cell_em_ = b; }
void AnalysisView::set_signal_em(bool b) { is_signal_em_ = b; }

/// get
SymbolIndex AnalysisView::get_name_index(void) { return name_; }
std::string AnalysisView::get_name(void) const {
    Cell* topCell = getTopCell();
    if (topCell) {
        return topCell->getSymbolByIndex(name_);
    }
    return "";
}
AnalysisCorner* AnalysisView::get_analysis_corner(void) {
    if (analysis_corner_ == UNINIT_OBJECT_ID) return nullptr;
    return Object::addr<AnalysisCorner>(analysis_corner_);
}

AnalysisMode* AnalysisView::get_analysis_mode(void) {
    if (analysis_mode_ == UNINIT_OBJECT_ID) return nullptr;
    return Object::addr<AnalysisMode>(analysis_mode_);
}
bool AnalysisView::is_active(void) { return is_active_; }
bool AnalysisView::is_setup(void) { return is_setup_; }
bool AnalysisView::is_hold(void) { return is_hold_; }
bool AnalysisView::is_max_cap(void) { return is_max_cap_; }
bool AnalysisView::is_max_tran(void) { return is_max_tran_; }
bool AnalysisView::is_min_cap(void) { return is_min_cap_; }
bool AnalysisView::is_leakage_power(void) { return is_leakage_power_; }
bool AnalysisView::is_dynamic_power(void) { return is_dynamic_power_; }
bool AnalysisView::is_cell_em(void) { return is_cell_em_; }
bool AnalysisView::is_signal_em(void) { return is_signal_em_; }

OStreamBase& operator<<(OStreamBase& os, AnalysisView const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    AnalysisView::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("name_") << rhs.get_name() << DataDelimiter();

    os << DataFieldName("analysis_corner_");
    os << rhs.analysis_corner_;
    os << DataBegin("[");
    {
        if (rhs.analysis_corner_ != UNINIT_OBJECT_ID) {
            AnalysisCorner* analysis_corner =
                Object::addr<AnalysisCorner>(rhs.analysis_corner_);
            if (analysis_corner) os << *analysis_corner;
        }
    }
    os << DataEnd("]") << DataDelimiter();

    os << DataFieldName("analysis_mode_");
    os << rhs.analysis_mode_;
    os << DataBegin("[");
    {
        if (rhs.analysis_mode_ != UNINIT_OBJECT_ID) {
            AnalysisMode* analysis_mode =
                Object::addr<AnalysisMode>(rhs.analysis_mode_);
            if (analysis_mode) os << *analysis_mode;
        }
    }
    os << DataEnd("]") << DataDelimiter();

    os << DataFieldName("is_active_") << rhs.is_active_ << DataDelimiter();
    os << DataFieldName("is_setup_") << rhs.is_setup_ << DataDelimiter();
    os << DataFieldName("is_hold_") << rhs.is_hold_ << DataDelimiter();
    os << DataFieldName("is_max_cap_") << rhs.is_max_cap_ << DataDelimiter();
    os << DataFieldName("is_max_tran_") << rhs.is_max_tran_ << DataDelimiter();
    os << DataFieldName("is_min_cap_") << rhs.is_min_cap_ << DataDelimiter();
    os << DataFieldName("is_leakage_power_") << rhs.is_leakage_power_
       << DataDelimiter();
    os << DataFieldName("is_dynamic_power_") << rhs.is_dynamic_power_
       << DataDelimiter();
    os << DataFieldName("is_cell_em_") << rhs.is_cell_em_ << DataDelimiter();
    os << DataFieldName("is_signal_em_") << rhs.is_signal_em_;

    os << DataEnd(")");
    return os;
}
}  // namespace db
}  // namespace open_edi
