/**
 * @file analysis_corner.cpp
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

#include "db/timing/timinglib/analysis_corner.h"

#include "db/core/db.h"
#include "db/timing/timinglib/libset.h"

namespace open_edi {
namespace db {

AnalysisCorner::AnalysisCorner()
    : AnalysisCorner::BaseType(),
      name_(0),
      rc_tech_file_(0),
      libset_(UNINIT_OBJECT_ID),
      design_parasitics_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeAnalysisCorner);
}

AnalysisCorner::~AnalysisCorner() {
#if 0
    if (libset_ != UNINIT_OBJECT_ID) {
        LibSet* libset = dynamic_cast<LibSet*>(getObject<LibSet>(libset_));
        if (libset) deleteObject<LibSet>(libset);
    }
#endif
}

AnalysisCorner::AnalysisCorner(Object* owner, AnalysisCorner::IndexType id)
    : AnalysisCorner::BaseType(owner, id),
      name_(0),
      rc_tech_file_(0),
      libset_(UNINIT_OBJECT_ID),
      design_parasitics_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeAnalysisCorner);
}

AnalysisCorner::AnalysisCorner(AnalysisCorner const& rhs) { copy(rhs); }

AnalysisCorner::AnalysisCorner(AnalysisCorner&& rhs) noexcept {
    move(std::move(rhs));
}

AnalysisCorner& AnalysisCorner::operator=(AnalysisCorner const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

AnalysisCorner& AnalysisCorner::operator=(AnalysisCorner&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void AnalysisCorner::copy(AnalysisCorner const& rhs) {
    this->BaseType::copy(rhs);
    name_ = rhs.name_;
    rc_tech_file_ = rhs.rc_tech_file_;
    libset_ = rhs.libset_;
    design_parasitics_ = rhs.design_parasitics_;
}

void AnalysisCorner::move(AnalysisCorner&& rhs) {
    this->BaseType::move(std::move(rhs));
    name_ = std::move(rhs.name_);
    rc_tech_file_ = std::move(rhs.rc_tech_file_);
    libset_ = std::move(rhs.libset_);
    design_parasitics_ = std::move(rhs.design_parasitics_);
}

AnalysisCorner::IndexType AnalysisCorner::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(name_);
    ret += sizeof(rc_tech_file_);
    ret += sizeof(libset_);
    ret += sizeof(design_parasitics_);

    return ret;
}

/// set
void AnalysisCorner::set_name(const std::string& name) {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex idx = timing_lib->getOrCreateSymbol(name.c_str());
        if (idx != kInvalidSymbolIndex) {
            name_ = idx;
            timing_lib->addSymbolReference(name_, this->getId());
        }
    }
}
void AnalysisCorner::set_rc_tech_file(const std::string& file) {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex idx = timing_lib->getOrCreateSymbol(file.c_str());
        if (idx != kInvalidSymbolIndex) {
            rc_tech_file_ = idx;
            timing_lib->addSymbolReference(rc_tech_file_, this->getId());
        }
    }
}
void AnalysisCorner::set_libset(ObjectId id) { libset_ = id; }
void AnalysisCorner::set_design_parasitics(ObjectId id) { design_parasitics_ = id; }

/// get
SymbolIndex AnalysisCorner::get_name_index(void) { return name_; }
SymbolIndex AnalysisCorner::get_rc_tech_file_index(void) {
    return rc_tech_file_;
}
std::string AnalysisCorner::get_name(void) const {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(name_);
    }
    return "";
}
std::string AnalysisCorner::get_rc_tech_file(void) const {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(rc_tech_file_);
    }
    return "";
}
LibSet* AnalysisCorner::get_libset(void) {
    if (libset_ != UNINIT_OBJECT_ID)
        return Object::addr<LibSet>(libset_);
    else
        return nullptr;
}
DesignParasitics* AnalysisCorner::get_design_parasitics(void) {
    if (design_parasitics_ != UNINIT_OBJECT_ID)
	return Object::addr<DesignParasitics>(design_parasitics_);
    else
	return nullptr;
}

OStreamBase& operator<<(OStreamBase& os, AnalysisCorner const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    AnalysisCorner::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("name_") << rhs.get_name() << DataDelimiter();
    os << DataFieldName("rc_tech_file_") << rhs.get_rc_tech_file()
       << DataDelimiter();

    os << DataFieldName("libset_");
    os << rhs.libset_;
    os << DataBegin("[");
    {
        LibSet* libset = nullptr;
        if (rhs.libset_ != UNINIT_OBJECT_ID)
            libset = Object::addr<LibSet>(rhs.libset_);
        if (libset) os << *libset;
    }
    os << DataEnd("]");

    os << DataFieldName("design_parasitics_") << rhs.design_parasitics_
       << DataDelimiter();

    os << DataEnd(")");
    return os;
}
}  // namespace db
}  // namespace open_edi
