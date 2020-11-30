/**
 * @file  Site.cpp
 * @date  08/12/2020 06:33:16 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/tech/site.h"

#include "db/core/db.h"
#include "db/util/array.h"

namespace open_edi {
namespace db {
using IdArray = ArrayObject<ObjectId>;

// Class SitePatternPair:
SitePatternPair::SitePatternPair(SitePatternPair const &rhs) {
    orientation_ = rhs.orientation_;
    name_index_ = rhs.name_index_;
    getTechLib()->addSymbolReference(name_index_, this->getId());
}

SitePatternPair::SitePatternPair(const char *name, Orient orientation)
    : orientation_(orientation) {
    setName(name);
}

// Get
const char *SitePatternPair::getName() const {
    return getTechLib()->getSymbolByIndex(name_index_).c_str();
}

SymbolIndex SitePatternPair::getNameIndex() const { return name_index_; }

Orient SitePatternPair::getOrientation() const { return orientation_; }

// Set
void SitePatternPair::setName(const char *v) {
    int64_t index = getTechLib()->getOrCreateSymbol(v);
    if (index == kInvalidSymbolIndex) return;

    name_index_ = index;
    getTechLib()->addSymbolReference(name_index_, this->getId());
}

void SitePatternPair::setOrientation(Orient v) { orientation_ = v; }

void SitePatternPair::printLEF(std::ofstream &ofs) const {
    ofs << getName() << " " << toString(orientation_) << " ";
}

OStreamBase &operator<<(OStreamBase &os, SitePatternPair const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");
    os << DataFieldName("name_index_") << rhs.getName() << DataDelimiter();
    os << DataFieldName("orientation_") << toString(rhs.orientation_)
       << DataEnd(")");
    return os;
}

// Class Site:
Site::Site()
    : name_index_(kInvalidSymbolIndex),
      class_(SiteClass::kUnknown),
      symmetry_(Symmetry::kUnknown),
      orientation_(Orient::kUnknown),
      is_default_(0),
      width_(0),
      height_(0),
      site_patterns_(0) {}

Site::Site(Site const &rhs) { copy(rhs); }

Site::Site(Site &&rhs) noexcept { move(std::move(rhs)); }

Site::~Site() { setPatternSize(0); }

Site &Site::operator=(Site const &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

Site &Site::operator=(Site &&rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

UInt32 Site::memory() const {
    UInt32 ret = this->BaseType::memory();
    ret += sizeof(name_index_);
    ret += sizeof(class_);
    ret += sizeof(symmetry_);
    ret += sizeof(orientation_);
    ret += sizeof(is_default_);
    ret += sizeof(width_);
    ret += sizeof(height_);
    ret += sizeof(site_patterns_);
    IdArray *vobj =
        addr<IdArray>(site_patterns_);
    if (vobj) {
        ret += vobj->memory();
    }
    return ret;
}

void Site::copy(Site const &rhs) {
    this->BaseType::copy(rhs);
    name_index_ = rhs.name_index_;
    class_ = rhs.class_;
    symmetry_ = rhs.symmetry_;
    orientation_ = rhs.orientation_;
    is_default_ = rhs.is_default_;
    width_ = rhs.width_;
    height_ = rhs.height_;
    site_patterns_ = rhs.site_patterns_;
}

void Site::move(Site &&rhs) {
    this->BaseType::move(std::move(rhs));
    name_index_ = std::move(rhs.name_index_);
    class_ = std::move(rhs.class_);
    symmetry_ = std::move(rhs.symmetry_);
    orientation_ = std::move(rhs.orientation_);
    is_default_ = std::move(rhs.is_default_);
    width_ = std::move(rhs.width_);
    height_ = std::move(rhs.height_);
    site_patterns_ = std::move(rhs.site_patterns_);
}

// Get:
const char *Site::getName() const {
    return getTechLib()->getSymbolByIndex(name_index_).c_str();
}

SymbolIndex Site::getNameIndex() const { return name_index_; }

SiteClass Site::getClass() const { return class_; }

Symmetry Site::getSymmetry() const { return symmetry_; }

Orient Site::getOrientation() const { return orientation_; }

bool Site::getIsDefault() const { return is_default_; }

UInt32 Site::getWidth() const { return width_; }

UInt32 Site::getHeight() const { return height_; }

uint64_t Site::numSitePatterns() const {
    if (!site_patterns_) return 0;

    return addr<IdArray>(site_patterns_)->getSize();
}

ObjectId Site::getSitePatternsId() const { return site_patterns_; }

// Set:
void Site::setName(const char *v) {
    int64_t index = getTechLib()->getOrCreateSymbol(v);
    if (index == kInvalidSymbolIndex) return;

    name_index_ = index;
    getTechLib()->addSymbolReference(name_index_, this->getId());
}

void Site::setClass(SiteClass v) { class_ = v; }

void Site::setSymmetry(Symmetry v) { symmetry_ = v; }

void Site::setOrientation(Orient v) { orientation_ = v; }

void Site::setIsDefault(bool v) { is_default_ = v; }

void Site::setWidth(UInt32 v) { width_ = v; }

void Site::setHeight(UInt32 v) { height_ = v; }

void Site::setClass(const char *v) { class_ = toEnumByString<SiteClass>(v); }

void Site::setSymmetry(const char *v) {
    symmetry_ = toEnumByString<Symmetry>(v);
}

void Site::setOrientation(const char *v) {
    orientation_ = toEnumByString<Orient>(v);
}

void Site::setPatternSize(UInt32 v) {
    if (v == 0) {
        if (site_patterns_) {
            __deleteObjectIdArray(site_patterns_);
        }
        return;
    }
    if (!site_patterns_) {
        site_patterns_ = __createObjectIdArray(16);
    }
}

void Site::addSitePattern(ObjectId obj_id) {
    IdArray *vobj = nullptr;
    if (obj_id == 0) return;

    if (site_patterns_ == 0) {
        site_patterns_ = __createObjectIdArray(16);
        vobj = addr<IdArray>(site_patterns_);
    } else {
        vobj = addr<IdArray>(site_patterns_);
    }
    ediAssert(vobj != nullptr);
    vobj->pushBack(obj_id);
}

void Site::printLEF(std::ofstream &ofs) const {
    ofs << "SITE " << getName() << "\n";
    ofs << "   CLASS " << toString(class_) << " ;\n";
    if (numSitePatterns() > 0) {
        ofs << "   ROWPATTERN ";
        IdArray *vobj =
            addr<IdArray>(site_patterns_);
        for (int i = 0; i < numSitePatterns(); ++i) {
            ObjectId obj_id = (*vobj)[i];
            SitePatternPair *obj_data =
                addr<SitePatternPair>(obj_id);
            if (obj_data == nullptr) continue;
            obj_data->printLEF(ofs);
        }
        ofs << ";\n";
    } else if (symmetry_ != Symmetry::kUnknown) {
        ofs << "   SYMMETRY " << toString(symmetry_) << " ;\n";
    }
    Tech *lib = getTechLib();
    ofs << "   SIZE " << lib->dbuToMicrons(width_) << " BY "
        << lib->dbuToMicrons(height_) << " ;\n";

    ofs << "END " << getName() << "\n\n";
}

OStreamBase &operator<<(OStreamBase &os, Site const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");
    Site::BaseType const &base = rhs;
    os << base << DataDelimiter();
    os << DataFieldName("name_index_") << rhs.name_index_ << DataDelimiter();
    os << DataFieldName("name_string_") << rhs.getName() << DataDelimiter();
    os << DataFieldName("class_") << toString(rhs.class_) << DataDelimiter();
    os << DataFieldName("symmetry_") << toString(rhs.symmetry_)
       << DataDelimiter();
    os << DataFieldName("orientation_") << toString(rhs.orientation_)
       << DataDelimiter();
    os << DataFieldName("is_default_") << rhs.is_default_ << DataDelimiter();
    os << DataFieldName("width_") << rhs.width_ << DataDelimiter();
    os << DataFieldName("height_") << rhs.height_ << DataDelimiter();
    if (rhs.getSitePatternsId()) {
        auto delimiter = DataDelimiter();
        os << DataFieldName("site_patterns_") << rhs.getSitePatternsId()
           << DataDelimiter();
        IdArray *vobj =
            Object::addr<IdArray>(rhs.getSitePatternsId());
        if (vobj != nullptr) {
            os << *vobj << DataDelimiter();
            os << DataFieldName("site_patterns_detail") << DataBegin("[");
            // details:
            delimiter = DataDelimiter("");
            for (IdArray::iterator iter = vobj->begin();
                 iter != vobj->end(); ++iter) {
                ObjectId element_id = (*iter);
                SitePatternPair *obj_data =
                    Object::addr<SitePatternPair>(element_id);
                os << delimiter << (*obj_data);
                delimiter = DataDelimiter();
            }
            os << DataEnd("]");
        }
    }

    os << DataEnd(")");
    return os;
}

}  // namespace db
}  // namespace open_edi
