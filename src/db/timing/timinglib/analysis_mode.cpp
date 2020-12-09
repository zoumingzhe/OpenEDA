/**
 * @file analysis_mode.cpp
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
#include "db/timing/timinglib/analysis_mode.h"

#include <vector>

#include "db/core/db.h"

namespace open_edi {
namespace db {

AnalysisMode::AnalysisMode()
    : AnalysisMode::BaseType(), name_(0), constraint_files_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeAnalysisMode);
}

AnalysisMode::~AnalysisMode() {}

AnalysisMode::AnalysisMode(Object* owner, AnalysisMode::IndexType id)
    : AnalysisMode::BaseType(owner, id),
      name_(0),
      constraint_files_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeAnalysisMode);
}

AnalysisMode::AnalysisMode(AnalysisMode const& rhs) { copy(rhs); }

AnalysisMode::AnalysisMode(AnalysisMode&& rhs) noexcept {
    move(std::move(rhs));
}

AnalysisMode& AnalysisMode::operator=(AnalysisMode const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

AnalysisMode& AnalysisMode::operator=(AnalysisMode&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void AnalysisMode::copy(AnalysisMode const& rhs) {
    this->BaseType::copy(rhs);
    name_ = rhs.name_;
    constraint_files_ = rhs.constraint_files_;
}

void AnalysisMode::move(AnalysisMode&& rhs) {
    this->BaseType::move(std::move(rhs));
    name_ = std::move(rhs.name_);
    constraint_files_ = std::move(rhs.constraint_files_);
    constraint_files_ = UNINIT_OBJECT_ID;
}

AnalysisMode::IndexType AnalysisMode::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(name_);
    ret += sizeof(constraint_files_);

    return ret;
}

/// set
void AnalysisMode::setName(const std::string& name) {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex idx = timing_lib->getOrCreateSymbol(name.c_str());
        if (idx != kInvalidSymbolIndex) {
            name_ = idx;
            timing_lib->addSymbolReference(name_, this->getId());
        }
    }
}
void AnalysisMode::addConstraintFile(const std::string& file) {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        ArrayObject<SymbolIndex>* p = nullptr;
        if (constraint_files_ == UNINIT_OBJECT_ID) {
            p = Object::createObject<ArrayObject<SymbolIndex>>(
                kObjectTypeArray, timing_lib->getId());
            if (p != nullptr) {
                constraint_files_ = p->getId();
                p->setPool(timing_lib->getPool());
                p->reserve(32);
            }
        } else {
            p = Object::addr<ArrayObject<SymbolIndex>>(constraint_files_);
        }
        if (p != nullptr) {
            SymbolIndex idx = timing_lib->getOrCreateSymbol(file.c_str());
            if (idx != kInvalidSymbolIndex) {
                p->pushBack(idx);
                timing_lib->addSymbolReference(idx, this->getId());
            }
        }
    }
}

void AnalysisMode::createSdc(void) {
    sdc_ = std::make_shared<Sdc>();
    assert(sdc_);
    if (sdc_ == nullptr) {
        //TODO messages
    }
}

void AnalysisMode::setSdc(SdcPtr sdc) {
    sdc_ = sdc;
}

/// get
SymbolIndex AnalysisMode::getNameIndex(void) { return name_; }
std::string AnalysisMode::getName(void) const {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(name_);
    }
    return "";
}
std::vector<std::string> AnalysisMode::getConstraintFiles(void) {
    std::vector<std::string> strVec;
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        if (constraint_files_ != UNINIT_OBJECT_ID) {
            auto p = Object::addr<ArrayObject<SymbolIndex>>(constraint_files_);
            if (p != nullptr) {
                for (int64_t i = 0; i < p->getSize(); ++i) {
                    std::string str = timing_lib->getSymbolByIndex((*p)[i]);
                    if (str != "") strVec.emplace_back(str);
                }
            }
        }
    }
    return strVec;
}
std::string AnalysisMode::getConstraintFile(SymbolIndex index) const {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(index);
    }
    return "";
}
int AnalysisMode::numContraintFiles(void) const {
    if (constraint_files_ != UNINIT_OBJECT_ID) {
        auto p = Object::addr<ArrayObject<SymbolIndex>>(constraint_files_);
        if (p != nullptr) return p->getSize();
    }
    return 0;
}
SdcPtr AnalysisMode::getSdc(void) const { return sdc_; }

OStreamBase& operator<<(OStreamBase& os, AnalysisMode const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    AnalysisMode::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("name_") << rhs.getName() << DataDelimiter();

    // write constraint_files_
    os << DataFieldName("constraint_files_");
    ArrayObject<SymbolIndex>* p = nullptr;
    if (rhs.constraint_files_ != UNINIT_OBJECT_ID)
        p = Object::addr<ArrayObject<SymbolIndex>>(rhs.constraint_files_);
    if (p != nullptr)
        os << p->getSize();
    else
        os << 0;
    os << DataBegin("[");
    if (p != nullptr) {
        auto delimiter = DataDelimiter("");
        for (int64_t i = 0; i < p->getSize(); ++i) {
            os << delimiter << rhs.getConstraintFile((*p)[i]);
            delimiter = DataDelimiter();
        }
    }
    os << DataEnd("]");

    os << DataEnd(")");
    return os;
}
}  // namespace db
}  // namespace open_edi
