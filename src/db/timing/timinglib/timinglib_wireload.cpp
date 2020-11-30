/**
 * @file timinglib_wireload.cpp
 * @date 2020-10-09
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
#include "db/timing/timinglib/timinglib_wireload.h"

#include "db/core/db.h"

namespace open_edi {
namespace db {

WireLoadTable::WireLoadTable()
    : WireLoadTable::BaseType(),
      name_(0),
      fanout_length_(),
      fanout_capacitance_(),
      fanout_resistance_(),
      fanout_area_() {
    setObjectType(ObjectType::kObjectTypeWireLoadTable);
}

WireLoadTable::~WireLoadTable() {}

WireLoadTable::WireLoadTable(Object* owner, WireLoadTable::IndexType id)
    : WireLoadTable::BaseType(owner, id),
      name_(0),
      fanout_length_(),
      fanout_capacitance_(),
      fanout_resistance_(),
      fanout_area_() {
    setObjectType(ObjectType::kObjectTypeWireLoadTable);
}

WireLoadTable::WireLoadTable(WireLoadTable const& rhs) { copy(rhs); }

WireLoadTable::WireLoadTable(WireLoadTable&& rhs) noexcept {
    move(std::move(rhs));
}

WireLoadTable& WireLoadTable::operator=(WireLoadTable const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

WireLoadTable& WireLoadTable::operator=(WireLoadTable&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void WireLoadTable::copy(WireLoadTable const& rhs) {
    this->BaseType::copy(rhs);

    name_ = rhs.name_;
    fanout_length_ = rhs.fanout_length_;
    fanout_capacitance_ = rhs.fanout_capacitance_;
    fanout_resistance_ = rhs.fanout_resistance_;
    fanout_area_ = rhs.fanout_area_;
}

void WireLoadTable::move(WireLoadTable&& rhs) {
    this->BaseType::move(std::move(rhs));

    name_ = std::move(rhs.name_);
    rhs.name_ = 0;
    fanout_length_ = std::move(rhs.fanout_length_);
    fanout_capacitance_ = std::move(rhs.fanout_capacitance_);
    fanout_resistance_ = std::move(rhs.fanout_resistance_);
    fanout_area_ = std::move(rhs.fanout_area_);
}

WireLoadTable::IndexType WireLoadTable::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(name_);
    ret += sizeof(fanout_length_);
    ret += sizeof(fanout_capacitance_);
    ret += sizeof(fanout_resistance_);
    ret += sizeof(fanout_area_);

    return ret;
}

/// set
void WireLoadTable::set_name(const std::string& name) {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(name.c_str());
        if (index != kInvalidSymbolIndex) {
            name_ = index;
            timing_lib->addSymbolReference(name_, this->getId());
        }
    }
}
void WireLoadTable::set_fanout_length(int n, float f) {
    fanout_length_.num = n;
    fanout_length_.value = f;
    fanout_length_.valid = true;
}
void WireLoadTable::set_fanout_capacitance(int n, float f) {
    fanout_capacitance_.num = n;
    fanout_capacitance_.value = f;
    fanout_capacitance_.valid = true;
}
void WireLoadTable::set_fanout_resistance(int n, float f) {
    fanout_resistance_.num = n;
    fanout_resistance_.value = f;
    fanout_resistance_.valid = true;
}
void WireLoadTable::set_fanout_area(int n, float f) {
    fanout_area_.num = n;
    fanout_area_.value = f;
    fanout_area_.valid = true;
}

/// get
std::string WireLoadTable::get_name(void) const {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(name_);
    }
    return "";
}
SymbolIndex WireLoadTable::get_name_index(void) { return name_; }
FanoutPair WireLoadTable::get_fanout_length(void) { return fanout_length_; }
FanoutPair WireLoadTable::get_fanout_capacitance(void) {
    return fanout_capacitance_;
}
FanoutPair WireLoadTable::get_fanout_resistance(void) {
    return fanout_resistance_;
}
FanoutPair WireLoadTable::get_fanout_area(void) { return fanout_area_; }

OStreamBase& operator<<(OStreamBase& os, WireLoadTable const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    WireLoadTable::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("name_") << rhs.get_name() << DataDelimiter();
    os << DataFieldName("fanout_length_");
    if (rhs.fanout_length_.valid == false)
        os << "";
    else
        os << rhs.fanout_length_.num << "-" << rhs.fanout_length_.value;
    os << DataDelimiter();

    os << DataFieldName("fanout_capacitance_");
    if (rhs.fanout_capacitance_.valid == false)
        os << "";
    else
        os << rhs.fanout_capacitance_.num << "-"
           << rhs.fanout_capacitance_.value;
    os << DataDelimiter();

    os << DataFieldName("fanout_resistance_");
    if (rhs.fanout_resistance_.valid == false)
        os << "";
    else
        os << rhs.fanout_resistance_.num << "-" << rhs.fanout_resistance_.value;
    os << DataDelimiter();

    os << DataFieldName("fanout_area_");
    if (rhs.fanout_area_.valid == false)
        os << "";
    else
        os << rhs.fanout_area_.num << "-" << rhs.fanout_area_.value;

    os << DataEnd(")");
    return os;
}

WireLoad::WireLoad()
    : WireLoad::BaseType(),
      name_(0),
      area_(0.0),
      resistance_(0.0),
      capacitance_(0.0),
      slope_(0.0),
      fanout_lengths_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeWireLoad);
}

WireLoad::~WireLoad() {}

WireLoad::WireLoad(Object* owner, WireLoad::IndexType id)
    : WireLoad::BaseType(owner, id),
      name_(0),
      area_(0.0),
      resistance_(0.0),
      capacitance_(0.0),
      slope_(0.0),
      fanout_lengths_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeWireLoad);
}

WireLoad::WireLoad(WireLoad const& rhs) { copy(rhs); }

WireLoad::WireLoad(WireLoad&& rhs) noexcept { move(std::move(rhs)); }

WireLoad& WireLoad::operator=(WireLoad const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

WireLoad& WireLoad::operator=(WireLoad&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void WireLoad::copy(WireLoad const& rhs) {
    this->BaseType::copy(rhs);

    name_ = rhs.name_;
    area_ = rhs.area_;
    resistance_ = rhs.resistance_;
    capacitance_ = rhs.capacitance_;
    slope_ = rhs.slope_;
    fanout_lengths_ = rhs.fanout_lengths_;
}

void WireLoad::move(WireLoad&& rhs) {
    this->BaseType::move(std::move(rhs));

    name_ = std::move(rhs.name_);
    area_ = std::move(rhs.area_);
    resistance_ = std::move(rhs.resistance_);
    capacitance_ = std::move(rhs.capacitance_);
    slope_ = std::move(rhs.slope_);
    fanout_lengths_ = std::move(rhs.fanout_lengths_);
}

WireLoad::IndexType WireLoad::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(name_);
    ret += sizeof(area_);
    ret += sizeof(resistance_);
    ret += sizeof(capacitance_);
    ret += sizeof(slope_);
    ret += sizeof(fanout_lengths_);

    return ret;
}

/// set
void WireLoad::set_name(const std::string& name) {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(name.c_str());
        if (index != kInvalidSymbolIndex) {
            name_ = index;
            timing_lib->addSymbolReference(name_, this->getId());
        }
    }
}
void WireLoad::set_area(float f) { area_ = f; }
void WireLoad::set_resistance(float f) { resistance_ = f; }
void WireLoad::set_capacitance(float f) { capacitance_ = f; }
void WireLoad::set_slope(float f) { slope_ = f; }
void WireLoad::add_fanout_length(int n, float f) {
    ArrayObject<FanoutPair>* p = nullptr;
    if (fanout_lengths_ == UNINIT_OBJECT_ID) {
        Timing* timing_lib = getTimingLib();
        if (timing_lib) {
            p = Object::createObject<ArrayObject<FanoutPair>>(
                kObjectTypeArray, timing_lib->getId());
            if (p != nullptr) {
                p->setPool(timing_lib->getPool());
                p->reserve(32);
                fanout_lengths_ = p->getId();
            }
        }
    } else {
        p = addr<ArrayObject<FanoutPair>>(fanout_lengths_);
    }
    if (p != nullptr) {
        FanoutPair fp;
        fp.num = n;
        fp.value = f;
        fp.valid = true;
        p->pushBack(fp);
    }
}

/// get
std::string WireLoad::get_name(void) const {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(name_);
    }
    return "";
}
SymbolIndex WireLoad::get_name_index(void) { return name_; }
float WireLoad::get_area(void) { return area_; }
float WireLoad::get_resistance(void) { return resistance_; }
float WireLoad::get_capacitance(void) { return capacitance_; }
float WireLoad::get_slope(void) { return slope_; }
ArrayObject<FanoutPair>* WireLoad::get_fanout_lengths(void) {
    ArrayObject<FanoutPair>* p = nullptr;
    if (fanout_lengths_ != UNINIT_OBJECT_ID)
        p = addr<ArrayObject<FanoutPair>>(fanout_lengths_);
    return p;
}

OStreamBase& operator<<(OStreamBase& os, WireLoad const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    WireLoad::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("name_") << rhs.get_name() << DataDelimiter();
    os << DataFieldName("area_") << rhs.area_ << DataDelimiter();
    os << DataFieldName("resistance_") << rhs.resistance_ << DataDelimiter();
    os << DataFieldName("capacitance_") << rhs.capacitance_ << DataDelimiter();
    os << DataFieldName("slope_") << rhs.slope_ << DataDelimiter();
    os << DataFieldName("fanout_lengths_");
    ArrayObject<FanoutPair>* p = nullptr;
    if (rhs.fanout_lengths_ != UNINIT_OBJECT_ID)
        p = Object::addr<ArrayObject<FanoutPair>>(rhs.fanout_lengths_);
    if (p != nullptr)
        os << p->getSize();
    else
        os << 0;
    os << DataBegin("[");
    if (p != nullptr) {
        auto delimiter = DataDelimiter("");
        for (int64_t i = 0; i < p->getSize(); ++i) {
            os << delimiter << (*p)[i].num << "-" << (*p)[i].value;
            delimiter = DataDelimiter();
        }
    }
    os << DataEnd("]");

    os << DataEnd(")");
    return os;
}

WireLoadForArea::WireLoadForArea()
    : WireLoadForArea::BaseType(),
      min_area_(0.0),
      max_area_(0.0),
      wireload_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeWireLoadForArea);
}

WireLoadForArea::~WireLoadForArea() {
    if (wireload_ != UNINIT_OBJECT_ID) {
        WireLoad* wl = addr<WireLoad>(wireload_);
        if (wl) {
            Timing* timing_lib = getTimingLib();
            if (timing_lib) timing_lib->deleteObject<WireLoad>(wl);
        }
    }
}
WireLoadForArea::WireLoadForArea(Object* owner, WireLoadForArea::IndexType id)
    : WireLoadForArea::BaseType(owner, id),
      min_area_(0.0),
      max_area_(0.0),
      wireload_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeWireLoadForArea);
}

WireLoadForArea::WireLoadForArea(WireLoadForArea const& rhs) { copy(rhs); }

WireLoadForArea::WireLoadForArea(WireLoadForArea&& rhs) noexcept {
    move(std::move(rhs));
}

WireLoadForArea& WireLoadForArea::operator=(WireLoadForArea const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

WireLoadForArea& WireLoadForArea::operator=(WireLoadForArea&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void WireLoadForArea::copy(WireLoadForArea const& rhs) {
    this->BaseType::copy(rhs);
    min_area_ = rhs.min_area_;
    max_area_ = rhs.max_area_;
    wireload_ = rhs.wireload_;
}

void WireLoadForArea::move(WireLoadForArea&& rhs) {
    this->BaseType::move(std::move(rhs));

    min_area_ = std::move(rhs.min_area_);
    max_area_ = std::move(rhs.max_area_);
    wireload_ = std::move(rhs.wireload_);
}

WireLoadForArea::IndexType WireLoadForArea::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(min_area_);
    ret += sizeof(max_area_);
    ret += sizeof(wireload_);

    return ret;
}

/// set
void WireLoadForArea::set_min_area(float f) { min_area_ = f; }
void WireLoadForArea::set_max_area(float f) { max_area_ = f; }
void WireLoadForArea::set_wireload(ObjectId id) { wireload_ = id; }

/// get
float WireLoadForArea::get_min_area(void) { return min_area_; }
float WireLoadForArea::get_max_area(void) { return max_area_; }
WireLoad* WireLoadForArea::get_wireload(void) {
    if (wireload_ != UNINIT_OBJECT_ID)
        return addr<WireLoad>(wireload_);
    else
        return nullptr;
}

OStreamBase& operator<<(OStreamBase& os, WireLoadForArea const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    WireLoadForArea::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("min_area_") << rhs.min_area_ << DataDelimiter();
    os << DataFieldName("max_area_") << rhs.max_area_ << DataDelimiter();
    os << DataFieldName("wireload_") << rhs.wireload_;
    os << DataBegin("[");
    if (rhs.wireload_ != UNINIT_OBJECT_ID) {
        WireLoad* wl = Object::addr<WireLoad>(rhs.wireload_);
        if (wl) os << *wl;
    }
    os << DataEnd("]");

    os << DataEnd(")");
    return os;
}

WireLoadSelection::WireLoadSelection()
    : WireLoadSelection::BaseType(), name_(0), wireloads_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeWireLoadSelection);
}

WireLoadSelection::~WireLoadSelection() {}

WireLoadSelection::WireLoadSelection(Object* owner,
                                     WireLoadSelection::IndexType id)
    : WireLoadSelection::BaseType(owner, id),
      name_(0),
      wireloads_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeWireLoadSelection);
}

WireLoadSelection::WireLoadSelection(WireLoadSelection const& rhs) {
    copy(rhs);
}

WireLoadSelection::WireLoadSelection(WireLoadSelection&& rhs) noexcept {
    move(std::move(rhs));
}

WireLoadSelection& WireLoadSelection::operator=(WireLoadSelection const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

WireLoadSelection& WireLoadSelection::operator=(
    WireLoadSelection&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void WireLoadSelection::copy(WireLoadSelection const& rhs) {
    this->BaseType::copy(rhs);
    name_ = rhs.name_;
    wireloads_ = rhs.wireloads_;
}

void WireLoadSelection::move(WireLoadSelection&& rhs) {
    this->BaseType::move(std::move(rhs));

    name_ = std::move(rhs.name_);
    wireloads_ = std::move(rhs.wireloads_);
    wireloads_ = UNINIT_OBJECT_ID;
}

WireLoadSelection::IndexType WireLoadSelection::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(name_);
    ret += sizeof(wireloads_);
    return ret;
}

/// set
void WireLoadSelection::set_name(const std::string& name) {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        SymbolIndex index = timing_lib->getOrCreateSymbol(name.c_str());
        if (index != kInvalidSymbolIndex) {
            name_ = index;
            timing_lib->addSymbolReference(name_, this->getId());
        }
    }
}
void WireLoadSelection::add_wire_load_for_area(ObjectId id) {
    if (id != UNINIT_OBJECT_ID) {
        ArrayObject<ObjectId>* object_array = nullptr;
        if (wireloads_ == UNINIT_OBJECT_ID) {
            Timing* timing_lib = getTimingLib();
            if (timing_lib != nullptr) {
                object_array = Object::createObject<ArrayObject<ObjectId>>(
                    kObjectTypeArray, timing_lib->getId());
                if (object_array != nullptr) {
                    wireloads_ = object_array->getId();
                    object_array->setPool(timing_lib->getPool());
                    object_array->reserve(32);
                }
            }
        } else {
            object_array =
                addr<ArrayObject<ObjectId>>(wireloads_);
        }
        if (object_array != nullptr) object_array->pushBack(id);
    }
}

/// get
std::string WireLoadSelection::get_name(void) const {
    Timing* timing_lib = getTimingLib();
    if (timing_lib) {
        return timing_lib->getSymbolByIndex(name_);
    }
    return "";
}
SymbolIndex WireLoadSelection::get_name_index(void) { return name_; }
WireLoadForArea* WireLoadSelection::get_wire_load_for_area(ObjectId id) {
    if (id == UNINIT_OBJECT_ID) return nullptr;
    return addr<WireLoadForArea>(id);
}

OStreamBase& operator<<(OStreamBase& os, WireLoadSelection const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    WireLoadSelection::BaseType const& base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("name_") << rhs.get_name() << DataDelimiter();
    os << DataFieldName("wireloads_");
    ArrayObject<ObjectId>* p = nullptr;
    if (rhs.wireloads_ != UNINIT_OBJECT_ID)
        p = Object::addr<ArrayObject<ObjectId>>(rhs.wireloads_);
    if (p != nullptr)
        os << p->getSize();
    else
        os << 0;
    os << DataBegin("[");
    if (p != nullptr) {
        auto delimiter = DataDelimiter("");
        for (int64_t i = 0; i < p->getSize(); ++i) {
            auto wa = Object::addr<WireLoadForArea>((*p)[i]);
            if (wa) {
                os << delimiter << *wa;
                delimiter = DataDelimiter();
            }
        }
    }
    os << DataEnd("]");

    os << DataEnd(")");
    return os;
}
}  // namespace db
}  // namespace open_edi
