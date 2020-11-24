/**
 * @file timinglib_libset.cpp
 * @date 2020-08-26
 * @brief LibSet Class
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "db/timing/timinglib/libset.h"

#include <map>

#include "db/core/db.h"
#include "db/timing/timinglib/timinglib_lib.h"
#include "util/stream.h"

namespace open_edi {
namespace db {

LibSet::LibSet()
    : LibSet::BaseType(),
      name_(0),
      timing_libs_(UNINIT_OBJECT_ID),
      timing_libs_map_() {
    setObjectType(ObjectType::kObjectTypeLibSet);
}

LibSet::~LibSet() { timing_libs_map_.clear(); }

LibSet::LibSet(Object* owner, LibSet::IndexType id)
    : LibSet::BaseType(owner, id),
      name_(0),
      timing_libs_(UNINIT_OBJECT_ID),
      timing_libs_map_() {
    setObjectType(ObjectType::kObjectTypeLibSet);
}

LibSet::LibSet(LibSet const& rhs) { copy(rhs); }

LibSet::LibSet(LibSet&& rhs) noexcept { move(std::move(rhs)); }

LibSet& LibSet::operator=(LibSet const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

LibSet& LibSet::operator=(LibSet&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void LibSet::copy(LibSet const& rhs) {
    this->BaseType::copy(rhs);

    name_ = rhs.name_;
    timing_libs_ = rhs.timing_libs_;
    timing_libs_map_ = rhs.timing_libs_map_;
}

void LibSet::move(LibSet&& rhs) {
    this->BaseType::move(std::move(rhs));
    name_ = std::move(rhs.name_);
    timing_libs_ = std::move(rhs.timing_libs_);
    timing_libs_map_ = std::move(rhs.timing_libs_map_);
    rhs.timing_libs_map_.clear();
}

LibSet::IndexType LibSet::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(name_);
    ret += sizeof(timing_libs_);

    return ret;
}

/// set
void LibSet::set_name(const std::string& name) {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex idx = timing_lib->getOrCreateSymbol(name.c_str());
        if (idx != kInvalidSymbolIndex) {
            name_ = idx;
            timing_lib->addSymbolReference(name_, this->getId());
        }
    }
}

LibSet::IndexType LibSet::numTLibs() const { return timing_libs_map_.size(); }

TLib* LibSet::addTLib(const std::string& filename, const std::string& name) {
    auto lib = __addTLibImpl(filename);
    if (lib) {
        TLib::AttrType attr;
        attr.set_name(name);
        lib->setAttr(&attr);
    }
    return lib;
}

/// get
SymbolIndex LibSet::get_name_index(void) { return name_; }
std::string LibSet::get_name(void) const {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(name_);
    }
    return "";
}

std::vector<TLib*> LibSet::get_timing_libs(void) {
    std::vector<TLib*> libs;
    for (auto iter = timing_libs_map_.begin(); iter != timing_libs_map_.end();
         iter++) {
        ObjectId id = iter->second;
        auto p = Object::addr<TLib>(iter->second);
        if (p) libs.emplace_back(p);
    }
    return libs;
}

void LibSet::print(std::ostream& stream) {
    for (auto v : timing_libs_map_) {
        TLib* lib = Object::addr<TLib>(v.second);
        if (lib) lib->print(stream);
    }
}
void LibSet::test(const std::string& filename) {
    OStream<std::ofstream> os(filename.c_str(), std::ios::out);
    os << *this;
    os.close();
}

OStreamBase& operator<<(OStreamBase& os, LibSet const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    LibSet::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("name_") << rhs.get_name() << DataDelimiter();

    // write timing_libs_
    os << DataFieldName("timing_libs_");
    {
        Timing* timing_lib = getTimingLib();
        std::map<std::string, TLib*> sorted_map;
        for (auto& v : rhs.timing_libs_map_) {
            std::string str = timing_lib->getSymbolByIndex(v.first);
            auto p = Object::addr<TLib>(v.second);
            if (str != "" && p != nullptr) sorted_map[str] = p;
        }
        os << sorted_map.size();
        os << DataBegin("[");
        auto delimiter = DataDelimiter("");
        for (auto const& v : sorted_map) {
            auto p = v.second;
            if (p) {
                os << delimiter << v.first;
                delimiter = DataDelimiter();
                os << delimiter << *p;
            }
        }
        os << DataEnd("]");
    }

    os << DataEnd(")");
    return os;
}

TLib* LibSet::__addTLibImpl(const std::string& filename) {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex idx = timing_lib->getOrCreateSymbol(filename.c_str());
        if (idx != kInvalidSymbolIndex) {
            timing_lib->addSymbolReference(idx, this->getId());
            auto lib = Object::createObject<TLib>(
                      kObjectTypeTLib, timing_lib->getId());
            if (lib) {
                lib->setOwner(this);
                ArrayObject<ObjectId>* p = nullptr;
                if (timing_libs_ == UNINIT_OBJECT_ID) {
                    p = Object::createObject<ArrayObject<ObjectId>>(
                        kObjectTypeArray, timing_lib->getId());
                    if (p != nullptr) {
                        timing_libs_ = p->getId();
                        p->setPool(timing_lib->getPool());
                        p->reserve(32);
                    }
                } else {
                    p = Object::addr<ArrayObject<ObjectId>>(
                        timing_libs_);
                }
                if (p != nullptr) {
                    ObjectId id = lib->getId();
                    p->pushBack(id);
                    timing_libs_map_[idx] = id;
                    return lib;
                }
            }
        }
    }
    return nullptr;
}

}  // namespace db
}  // namespace open_edi
