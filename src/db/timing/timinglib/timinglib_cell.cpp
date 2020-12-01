
/**
 * @file timinglib_cell.cpp
 * @date 2020-08-26
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

#include "db/timing/timinglib/timinglib_cell.h"

#include "db/core/db.h"
#include "db/timing/timinglib/timinglib_lib.h"
#include "db/timing/timinglib/timinglib_pgterm.h"
#include "db/timing/timinglib/timinglib_term.h"

namespace open_edi {
namespace db {

TCell::TCell()
    : TCell::BaseType(),
      always_on_(false),
      dont_touch_(false),
      dont_use_(false),
      is_pad_(false),
      is_macro_cell_(false),
      is_clock_gating_cell_(false),
      is_clock_isolation_cell_(false),
      is_isolation_cell_(false),
      is_level_shifter_(false),
      is_enable_level_shifter_(false),
      is_decap_cell_(false),
      is_filler_cell_(false),
      is_tap_cell_(false),
      is_diode_cell_(false),
      is_power_switch_(false),
      is_retention_cell_(false),
      is_sequential_(false),
      is_integrated_clock_gating_cell_(false),
      is_three_state_(false),
      antenna_diode_type_(AntennaDiodeType::kUnknown),
      switch_cell_type_(SwitchCellType::kUnknown),
      threshold_votage_group_(ThresholdVotageGroup::kUnknown),
      clock_gating_integrated_cell_(ClockGateIntegratedType::kUnknown),
      name_(0),
      cell_footprint_(0),
      retention_cell_(0),
      area_(0.0),
      cell_leakage_power_(0.0),
      tterms_(UNINIT_OBJECT_ID),
      tpg_terms_(UNINIT_OBJECT_ID),
      term_map_(),
      pg_term_map_() {
    setObjectType(ObjectType::kObjectTypeTCell);
}

TCell::~TCell() {
    term_map_.clear();
    pg_term_map_.clear();
}

TCell::TCell(Object *owner, TCell::IndexType id)
    : TCell::BaseType(owner, id),
      always_on_(false),
      dont_touch_(false),
      dont_use_(false),
      is_pad_(false),
      is_macro_cell_(false),
      is_clock_gating_cell_(false),
      is_clock_isolation_cell_(false),
      is_isolation_cell_(false),
      is_level_shifter_(false),
      is_enable_level_shifter_(false),
      is_decap_cell_(false),
      is_filler_cell_(false),
      is_tap_cell_(false),
      is_diode_cell_(false),
      is_power_switch_(false),
      is_retention_cell_(false),
      is_sequential_(false),
      is_integrated_clock_gating_cell_(false),
      is_three_state_(false),
      antenna_diode_type_(AntennaDiodeType::kUnknown),
      switch_cell_type_(SwitchCellType::kUnknown),
      threshold_votage_group_(ThresholdVotageGroup::kUnknown),
      clock_gating_integrated_cell_(ClockGateIntegratedType::kUnknown),
      name_(0),
      cell_footprint_(0),
      retention_cell_(0),
      area_(0.0),
      cell_leakage_power_(0.0),
      tterms_(UNINIT_OBJECT_ID),
      tpg_terms_(UNINIT_OBJECT_ID),
      term_map_(),
      pg_term_map_() {
    setObjectType(ObjectType::kObjectTypeTCell);
}

TCell::TCell(TCell const &rhs) { copy(rhs); }

TCell::TCell(TCell &&rhs) noexcept { move(std::move(rhs)); }

TCell &TCell::operator=(TCell const &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TCell &TCell::operator=(TCell &&rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

TCell::IndexType TCell::numTerms() const {
    if (tterms_ == UNINIT_OBJECT_ID) return 0;

    ArrayObject<ObjectId> *object_vector =
        Object::addr<ArrayObject<ObjectId>>(tterms_);
    if (object_vector != nullptr)
        return object_vector->getSize();
    else
        return 0;
}

TCell::IndexType TCell::numPgTerms() const {
    if (tpg_terms_ == UNINIT_OBJECT_ID) return 0;

    ArrayObject<ObjectId> *object_vector =
        Object::addr<ArrayObject<ObjectId>>(tpg_terms_);
    if (object_vector != nullptr)
        return object_vector->getSize();
    else
        return 0;
}

TTerm *TCell::getOrCreateTerm(const std::string &name) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex idx = timing_lib->getOrCreateSymbol(name.c_str());
        if (idx != kInvalidSymbolIndex) {
            auto t = term_map_.find(idx);
            if (t != term_map_.end()) {
                return Object::addr<TTerm>(t->second);
            } else {
                auto term = __addTermImpl();
                if (term) {
                    setName(name);
                    term_map_[getNameIndex()] = term->getId();
                }
                return term;
            }
        }
    }
    return nullptr;
}
TTerm *TCell::getTerm(const std::string &name) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex idx = timing_lib->getOrCreateSymbol(name.c_str());
        if (idx != kInvalidSymbolIndex) {
            auto t = term_map_.find(idx);
            if (t != term_map_.end()) {
                return Object::addr<TTerm>(t->second);
            }
        }
    }
    return nullptr;
}
std::vector<TTerm *> TCell::getTerms(void) {
    std::vector<TTerm *> terms;
    for (auto p : term_map_) {
        auto term = Object::addr<TTerm>(p.second);
        if (term != nullptr) terms.emplace_back(term);
    }
    return terms;
}
void TCell::resetTerms(const std::vector<TTerm *> &terms) {
    if (terms.empty() && tterms_ == UNINIT_OBJECT_ID) return;

    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        ArrayObject<ObjectId> *p = nullptr;
        if (tterms_ == UNINIT_OBJECT_ID) {
            p = Object::createObject<ArrayObject<ObjectId>>(
                kObjectTypeArray, timing_lib->getId());
            if (p) {
                tterms_ = p->getId();
                p->setPool(timing_lib->getPool());
                p->reserve(32);
            }
        } else {
            p = Object::addr<ArrayObject<ObjectId>>(tterms_);
        }
        if (p != nullptr) {
            IndexType cur_size = terms.size();
            IndexType orig_size = p->getSize();
            term_map_.clear();
            if (cur_size < orig_size) {
                p->adjustSize(cur_size);

                for (int64_t i = 0; i < cur_size; ++i) {
                    auto &term = (*p)[i];
                    if (terms[i]) {
                        term = terms[i]->getId();
                        term_map_[terms[i]->getNameIndex()] = terms[i]->getId();
                    }
                }

            } else {
                for (int64_t i = 0; i < orig_size; ++i) {
                    auto &term = (*p)[i];
                    if (terms[i]) {
                        term = terms[i]->getId();
                        term_map_[terms[i]->getNameIndex()] = terms[i]->getId();
                    }
                }
                for (IndexType i = orig_size; i < cur_size; ++i) {
                    auto &term = terms[i];
                    if (term) {
                        ObjectId id = term->getId();
                        p->pushBack(id);
                        term_map_[term->getNameIndex()] = id;
                    }
                }
            }
        }
    }
}
TPgTerm *TCell::getOrCreatePgTerm(const std::string &name) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex idx = timing_lib->getOrCreateSymbol(name.c_str());
        if (idx != kInvalidSymbolIndex) {
            auto t = pg_term_map_.find(idx);
            if (t != pg_term_map_.end()) {
                return Object::addr<TPgTerm>(t->second);
            } else {
                auto term = __addPgTermImpl();
                if (term) {
                    term->setName(name);
                    pg_term_map_[term->getNameIndex()] = term->getId();
                }
                return term;
            }
        }
    }
    return nullptr;
}
TPgTerm *TCell::getPgTerm(const std::string &name) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex idx = timing_lib->getOrCreateSymbol(name.c_str());
        if (idx != kInvalidSymbolIndex) {
            auto t = pg_term_map_.find(idx);
            if (t != pg_term_map_.end()) {
                return Object::addr<TPgTerm>(t->second);
            }
        }
    }
    return nullptr;
}
std::vector<TPgTerm *> TCell::getPgTerms(void) {
    std::vector<TPgTerm *> pg_terms;
    for (auto p : pg_term_map_) {
        auto term = Object::addr<TPgTerm>(p.second);
        if (term != nullptr) pg_terms.emplace_back(term);
    }
    return pg_terms;
}
TCell::IndexType TCell::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(always_on_);
    ret += sizeof(dont_touch_);
    ret += sizeof(dont_use_);
    ret += sizeof(is_pad_);
    ret += sizeof(is_macro_cell_);
    ret += sizeof(is_clock_gating_cell_);
    ret += sizeof(is_clock_isolation_cell_);
    ret += sizeof(is_isolation_cell_);
    ret += sizeof(is_level_shifter_);
    ret += sizeof(is_enable_level_shifter_);
    ret += sizeof(is_decap_cell_);
    ret += sizeof(is_filler_cell_);
    ret += sizeof(is_tap_cell_);
    ret += sizeof(is_diode_cell_);
    ret += sizeof(is_power_switch_);
    ret += sizeof(is_retention_cell_);
    ret += sizeof(is_sequential_);
    ret += sizeof(is_integrated_clock_gating_cell_);
    ret += sizeof(is_three_state_);
    ret += sizeof(is_tap_cell_);
    ret += sizeof(antenna_diode_type_);
    ret += sizeof(switch_cell_type_);
    ret += sizeof(threshold_votage_group_);
    ret += sizeof(clock_gating_integrated_cell_);
    ret += sizeof(name_);
    ret += sizeof(cell_footprint_);
    ret += sizeof(retention_cell_);
    ret += sizeof(area_);
    ret += sizeof(cell_leakage_power_);
    ret += sizeof(tterms_);
    ret += sizeof(tpg_terms_);

    return ret;
}

void TCell::setAlwaysOn(bool b) { always_on_ = b; }
void TCell::setDontTouch(bool b) { dont_touch_ = b; }
void TCell::setDontUse(bool b) { dont_use_ = b; }
void TCell::setPad(bool b) { is_pad_ = b; }
void TCell::setMacroCell(bool b) { is_macro_cell_ = b; }
void TCell::setClockGatingCell(bool b) { is_clock_gating_cell_ = b; }
void TCell::setClockIsolationCell(bool b) { is_clock_isolation_cell_ = b; }
void TCell::setIsolationCell(bool b) { is_isolation_cell_ = b; }
void TCell::setLevelShifter(bool b) { is_level_shifter_ = b; }
void TCell::setEnableLevelShifter(bool b) { is_enable_level_shifter_ = b; }
void TCell::setDecapCell(bool b) { is_decap_cell_ = b; }
void TCell::setFillerCell(bool b) { is_filler_cell_ = b; }
void TCell::setTapCell(bool b) { is_tap_cell_ = b; }
void TCell::setDiodeCell(bool b) { is_diode_cell_ = b; }
void TCell::setPowerSwitch(bool b) { is_power_switch_ = b; }
void TCell::setRetentionCell(bool b) { is_retention_cell_ = b; }
void TCell::setSequential(bool b) { is_sequential_ = b; }
void TCell::setIntegratedClockGatingCell(bool b) {
    is_integrated_clock_gating_cell_ = b;
}
void TCell::setThreeState(bool b) { is_three_state_ = b; }
void TCell::setAntennaDiodeType(AntennaDiodeType t) { antenna_diode_type_ = t; }
void TCell::setSwitchCellType(SwitchCellType t) { switch_cell_type_ = t; }
void TCell::setThresholdVotageGroup(ThresholdVotageGroup t) {
    threshold_votage_group_ = t;
}
void TCell::setClockGatingIntegratedCell(ClockGateIntegratedType t) {
    clock_gating_integrated_cell_ = t;
}
void TCell::setName(const std::string &name) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(name.c_str());
        if (index != kInvalidSymbolIndex) {
            name_ = index;
            timing_lib->addSymbolReference(name_, this->getId());
        }
    }
}
void TCell::setCellFootprint(const std::string &cf) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(cf.c_str());
        if (index != kInvalidSymbolIndex) {
            cell_footprint_ = index;
            timing_lib->addSymbolReference(cell_footprint_, this->getId());
        }
    }
}
void TCell::setRetentionCell(const std::string &rc) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(rc.c_str());
        if (index != kInvalidSymbolIndex) {
            retention_cell_ = index;
            // timing_lib->addSymbolReference(retention_cell_, this->getId());
        }
    }
}
void TCell::setArea(float f) { area_ = f; }
void TCell::setCellLeakagePower(float f) { cell_leakage_power_ = f; }

bool TCell::isPhysicalOnly(void) {
    if (numTerms() == 0 && numPgTerms() != 0) return true;
    return false;
}

bool TCell::isCombinational(void) {
    if (isSequential() == false && isPhysicalOnly() == false) return true;
    return false;
}
bool TCell::hasMultiPowerRails(void) {
    if (numPgTerms() > 1) return true;
    return false;
}
void TCell::print(std::ostream &stream) {
    stream << "TCell name: " << getName() << " id: " << getId() << std::endl;
    if (tterms_ != UNINIT_OBJECT_ID) {
        ArrayObject<ObjectId> *object_vector =
            Object::addr<ArrayObject<ObjectId>>(tterms_);
        if (object_vector != nullptr) {
            for (int64_t i = 0; i < object_vector->getSize(); ++i) {
                auto p = Object::addr<TTerm>((*object_vector)[i]);
                if (p) p->print(stream);
            }
        }
    }
    if (tpg_terms_ != UNINIT_OBJECT_ID) {
        ArrayObject<ObjectId> *object_vector =
            Object::addr<ArrayObject<ObjectId>>(tpg_terms_);
        if (object_vector != nullptr) {
            for (int64_t i = 0; i < object_vector->getSize(); ++i) {
                auto p = Object::addr<TPgTerm>((*object_vector)[i]);
                if (p) p->print(stream);
            }
        }
    }
}

bool TCell::isAlwaysOn(void) { return always_on_; }
bool TCell::isDontTouch(void) { return dont_touch_; }
bool TCell::isDontUse(void) { return dont_use_; }
bool TCell::isPad(void) { return is_pad_; }
bool TCell::isMacroCell(void) { return is_macro_cell_; }
bool TCell::isClockGatingCell(void) { return is_clock_gating_cell_; }
bool TCell::isClockIsolationCell(void) { return is_clock_isolation_cell_; }
bool TCell::isIsolationCell(void) { return is_isolation_cell_; }
bool TCell::isLevelShifter(void) { return is_level_shifter_; }
bool TCell::isEnableLevelShifter(void) { return is_enable_level_shifter_; }
bool TCell::isDecapCell(void) { return is_decap_cell_; }
bool TCell::isFillerCell(void) { return is_filler_cell_; }
bool TCell::isTapCell(void) { return is_tap_cell_; }
bool TCell::isDiodeCell(void) { return is_diode_cell_; }
bool TCell::isPowerSwitch(void) { return is_power_switch_; }
bool TCell::isRetentionCell(void) { return is_retention_cell_; }
bool TCell::isSequential(void) { return is_sequential_; }
bool TCell::isIntegratedClockGatingCell(void) {
    return is_integrated_clock_gating_cell_;
}
bool TCell::isThreeState(void) { return is_three_state_; }
AntennaDiodeType TCell::getAntennaDiodeType(void) {
    return antenna_diode_type_;
}
SwitchCellType TCell::getSwitchCellType(void) { return switch_cell_type_; }
ThresholdVotageGroup TCell::getThresholdVotageGroup(void) {
    return threshold_votage_group_;
}
ClockGateIntegratedType TCell::getClockGatingIntegratedCell(void) {
    return clock_gating_integrated_cell_;
}
std::string TCell::getName(void) const {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(name_);
    }
    return "";
}
SymbolIndex TCell::getNameIndex(void) { return name_; }
std::string TCell::getCellFootprint(void) const {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(cell_footprint_);
    }
    return "";
}
std::string TCell::getRetentionCell(void) const {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(retention_cell_);
    }
    return "";
}
float TCell::getArea(void) { return area_; }
float TCell::getCellLeakagePower(void) { return cell_leakage_power_; }

std::string TCell::getFullName(void) {
    std::string str = "";
    Object *owner_obj = nullptr;

    if (owner_ != UNINIT_OBJECT_ID) {
        owner_obj = Object::addr<Object>(owner_);
    }
    if (owner_obj != nullptr &&
        owner_obj->getObjectType() == ObjectType::kObjectTypeTLib) {
        TLib *lib = static_cast<TLib *>(owner_obj);
        if (lib != nullptr) str = lib->getName();
    }

    if (str != "")
        str = str + "_" + getName();
    else
        str = getName();

    return str;
}

TCell::IndexType TCell::getNumPins(void) { return numTerms() + numPgTerms(); }

void TCell::copy(TCell const &rhs) {
    this->BaseType::copy(rhs);

    always_on_ = rhs.always_on_;
    dont_touch_ = rhs.dont_touch_;
    dont_use_ = rhs.dont_use_;
    is_pad_ = rhs.is_pad_;
    is_macro_cell_ = rhs.is_macro_cell_;
    is_clock_gating_cell_ = rhs.is_clock_gating_cell_;
    is_clock_isolation_cell_ = rhs.is_clock_isolation_cell_;
    is_isolation_cell_ = rhs.is_isolation_cell_;
    is_level_shifter_ = rhs.is_level_shifter_;
    is_enable_level_shifter_ = rhs.is_enable_level_shifter_;
    is_decap_cell_ = rhs.is_decap_cell_;
    is_filler_cell_ = rhs.is_filler_cell_;
    is_tap_cell_ = rhs.is_tap_cell_;
    is_diode_cell_ = rhs.is_diode_cell_;
    is_power_switch_ = rhs.is_power_switch_;
    is_retention_cell_ = rhs.is_retention_cell_;
    is_sequential_ = rhs.is_sequential_;
    is_integrated_clock_gating_cell_ = rhs.is_integrated_clock_gating_cell_;
    is_three_state_ = rhs.is_three_state_;
    is_tap_cell_ = rhs.is_tap_cell_;
    antenna_diode_type_ = rhs.antenna_diode_type_;
    switch_cell_type_ = rhs.switch_cell_type_;
    threshold_votage_group_ = rhs.threshold_votage_group_;
    clock_gating_integrated_cell_ = rhs.clock_gating_integrated_cell_;
    name_ = rhs.name_;
    cell_footprint_ = rhs.cell_footprint_;
    retention_cell_ = rhs.retention_cell_;
    area_ = rhs.area_;
    cell_leakage_power_ = rhs.cell_leakage_power_;
    tterms_ = rhs.tterms_;
    tpg_terms_ = rhs.tpg_terms_;
    term_map_ = rhs.term_map_;
    pg_term_map_ = rhs.pg_term_map_;
}

void TCell::move(TCell &&rhs) {
    this->BaseType::move(std::move(rhs));

    always_on_ = std::move(rhs.always_on_);
    dont_touch_ = std::move(rhs.dont_touch_);
    dont_use_ = std::move(rhs.dont_use_);
    is_pad_ = std::move(rhs.is_pad_);
    is_macro_cell_ = std::move(rhs.is_macro_cell_);
    is_clock_gating_cell_ = std::move(rhs.is_clock_gating_cell_);
    is_clock_isolation_cell_ = std::move(rhs.is_clock_isolation_cell_);
    is_isolation_cell_ = std::move(rhs.is_isolation_cell_);
    is_level_shifter_ = std::move(rhs.is_level_shifter_);
    is_enable_level_shifter_ = std::move(rhs.is_enable_level_shifter_);
    is_decap_cell_ = std::move(rhs.is_decap_cell_);
    is_filler_cell_ = std::move(rhs.is_filler_cell_);
    is_tap_cell_ = std::move(rhs.is_tap_cell_);
    is_diode_cell_ = std::move(rhs.is_diode_cell_);
    is_power_switch_ = std::move(rhs.is_power_switch_);
    is_retention_cell_ = std::move(rhs.is_retention_cell_);
    is_sequential_ = std::move(rhs.is_sequential_);
    is_integrated_clock_gating_cell_ =
        std::move(rhs.is_integrated_clock_gating_cell_);
    is_three_state_ = std::move(rhs.is_three_state_);
    is_tap_cell_ = std::move(rhs.is_tap_cell_);
    antenna_diode_type_ = std::move(rhs.antenna_diode_type_);
    switch_cell_type_ = std::move(rhs.switch_cell_type_);
    threshold_votage_group_ = std::move(rhs.threshold_votage_group_);
    clock_gating_integrated_cell_ =
        std::move(rhs.clock_gating_integrated_cell_);
    name_ = std::move(rhs.name_);
    cell_footprint_ = std::move(rhs.cell_footprint_);
    retention_cell_ = std::move(rhs.retention_cell_);
    area_ = std::move(rhs.area_);
    cell_leakage_power_ = std::move(rhs.cell_leakage_power_);
    tterms_ = std::move(rhs.tterms_);
    tpg_terms_ = std::move(rhs.tpg_terms_);
    term_map_ = std::move(rhs.term_map_);
    pg_term_map_ = std::move(rhs.pg_term_map_);

    rhs.tterms_ = UNINIT_OBJECT_ID;
    rhs.tpg_terms_ = UNINIT_OBJECT_ID;
    rhs.term_map_.clear();
    rhs.pg_term_map_.clear();
}

OStreamBase &operator<<(OStreamBase &os, TCell const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    TCell::BaseType const &base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("always_on_") << rhs.always_on_ << DataDelimiter();
    os << DataFieldName("dont_touch_") << rhs.dont_touch_ << DataDelimiter();
    os << DataFieldName("dont_use_") << rhs.dont_use_ << DataDelimiter();
    os << DataFieldName("is_pad_") << rhs.is_pad_ << DataDelimiter();
    os << DataFieldName("is_macro_cell_") << rhs.is_macro_cell_
       << DataDelimiter();
    os << DataFieldName("is_clock_gating_cell_") << rhs.is_clock_gating_cell_
       << DataDelimiter();
    os << DataFieldName("is_clock_isolation_cell_")
       << rhs.is_clock_isolation_cell_ << DataDelimiter();
    os << DataFieldName("is_isolation_cell_") << rhs.is_isolation_cell_
       << DataDelimiter();
    os << DataFieldName("is_level_shifter_") << rhs.is_level_shifter_
       << DataDelimiter();
    os << DataFieldName("is_enable_level_shifter_")
       << rhs.is_enable_level_shifter_ << DataDelimiter();
    os << DataFieldName("is_decap_cell_") << rhs.is_decap_cell_
       << DataDelimiter();
    os << DataFieldName("is_filler_cell_") << rhs.is_filler_cell_
       << DataDelimiter();
    os << DataFieldName("is_tap_cell_") << rhs.is_tap_cell_ << DataDelimiter();
    os << DataFieldName("is_diode_cell_") << rhs.is_diode_cell_
       << DataDelimiter();
    os << DataFieldName("is_power_switch_") << rhs.is_power_switch_
       << DataDelimiter();
    os << DataFieldName("is_retention_cell_") << rhs.is_retention_cell_
       << DataDelimiter();
    os << DataFieldName("is_sequential_") << rhs.is_sequential_
       << DataDelimiter();
    os << DataFieldName("is_integrated_clock_gating_cell_")
       << rhs.is_integrated_clock_gating_cell_ << DataDelimiter();
    os << DataFieldName("is_three_state_") << rhs.is_three_state_
       << DataDelimiter();
    os << DataFieldName("is_tap_cell_") << rhs.is_tap_cell_ << DataDelimiter();
    os << DataFieldName("antenna_diode_type_") << rhs.antenna_diode_type_
       << DataDelimiter();
    os << DataFieldName("switch_cell_type_") << rhs.switch_cell_type_
       << DataDelimiter();
    os << DataFieldName("threshold_votage_group_")
       << rhs.threshold_votage_group_ << DataDelimiter();
    os << DataFieldName("clock_gating_integrated_cell_")
       << rhs.clock_gating_integrated_cell_ << DataDelimiter();
    os << DataFieldName("name_") << rhs.getName() << DataDelimiter();
    os << DataFieldName("cell_footprint_") << rhs.getCellFootprint()
       << DataDelimiter();
    os << DataFieldName("retention_cell_") << rhs.getRetentionCell()
       << DataDelimiter();
    os << DataFieldName("area_") << rhs.area_ << DataDelimiter();
    os << DataFieldName("cell_leakage_power_") << rhs.cell_leakage_power_;

    // write tterms
    os << DataFieldName("tterms_");
    {
        os << rhs.numTerms();
        os << DataBegin("[");
        if (rhs.tterms_ != UNINIT_OBJECT_ID) {
            ArrayObject<ObjectId> *object_vector =
                Object::addr<ArrayObject<ObjectId>>(rhs.tterms_);
            if (object_vector != nullptr) {
                auto delimiter = DataDelimiter("");
                for (int64_t i = 0; i < object_vector->getSize(); ++i) {
                    auto p = Object::addr<TTerm>((*object_vector)[i]);
                    if (p) {
                        os << delimiter << *p;
                        delimiter = DataDelimiter();
                    }
                }
            }
        }
        os << DataEnd("]") << DataDelimiter();
    }
    // write tpg_terms
    os << DataFieldName("tpg_terms_");
    {
        os << rhs.numPgTerms();
        os << DataBegin("[");
        if (rhs.tpg_terms_ != UNINIT_OBJECT_ID) {
            ArrayObject<ObjectId> *object_vector =
                Object::addr<ArrayObject<ObjectId>>(rhs.tpg_terms_);
            if (object_vector != nullptr) {
                auto delimiter = DataDelimiter("");
                for (int64_t i = 0; i < object_vector->getSize(); ++i) {
                    auto p = Object::addr<TPgTerm>((*object_vector)[i]);
                    if (p) {
                        os << delimiter << *p;
                        delimiter = DataDelimiter();
                    }
                }
            }
        }
        os << DataEnd("]");
    }

    os << DataEnd(")");
    return os;
}

TTerm *TCell::__addTermImpl() {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        ArrayObject<ObjectId> *p = nullptr;
        if (tterms_ == UNINIT_OBJECT_ID) {
            p = Object::createObject<ArrayObject<ObjectId>>(
                kObjectTypeArray, timing_lib->getId());
            if (p) {
                tterms_ = p->getId();
                p->setPool(timing_lib->getPool());
                p->reserve(32);
            }
        } else {
            p = Object::addr<ArrayObject<ObjectId>>(tterms_);
        }
        if (p != nullptr) {
            auto term = Object::createObject<TTerm>(kObjectTypeTTerm,
                                                    timing_lib->getId());
            if (term) {
                term->setOwner(this);
                ObjectId id = term->getId();
                p->pushBack(id);
            }
            return term;
        }
    }
    return nullptr;
}

TPgTerm *TCell::__addPgTermImpl() {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        ArrayObject<ObjectId> *p = nullptr;
        if (tpg_terms_ == UNINIT_OBJECT_ID) {
            p = Object::createObject<ArrayObject<ObjectId>>(
                kObjectTypeArray, timing_lib->getId());
            if (p) {
                tpg_terms_ = p->getId();
                p->setPool(timing_lib->getPool());
                p->reserve(32);
            }
        } else {
            p = Object::addr<ArrayObject<ObjectId>>(tpg_terms_);
        }
        if (p != nullptr) {
            auto pg_term = Object::createObject<TPgTerm>(kObjectTypeTPgTerm,
                                                         timing_lib->getId());
            if (pg_term) {
                pg_term->setOwner(this);
                ObjectId id = pg_term->getId();
                p->pushBack(id);
            }
            return pg_term;
        }
    }
    return nullptr;
}
}  // namespace db
}  // namespace open_edi
