/**
 * @file timinglib_term.cpp
 * @date 2020-08-26
 * @brief TTerm Class
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/timing/timinglib/timinglib_term.h"

#include "db/core/db.h"
#include "db/timing/timinglib/timinglib_function.h"
#include "db/timing/timinglib/timinglib_pgterm.h"
#include "db/timing/timinglib/timinglib_timingarc.h"

namespace open_edi {
namespace db {

TTerm::TTerm()
    : TTerm::BaseType(),
      related_power_pin_(UNINIT_OBJECT_ID),
      related_ground_pin_(UNINIT_OBJECT_ID),
      function_(UNINIT_OBJECT_ID),
      member_pins_(UNINIT_OBJECT_ID),
      timing_arcs_(UNINIT_OBJECT_ID),
      member_pins_map_() {
    setObjectType(ObjectType::kObjectTypeTTerm);
}

TTerm::~TTerm() {}

TTerm::TTerm(Object *owner, TTerm::IndexType id)
    : TTerm::BaseType(owner, id),
      related_power_pin_(UNINIT_OBJECT_ID),
      related_ground_pin_(UNINIT_OBJECT_ID),
      function_(UNINIT_OBJECT_ID),
      member_pins_(UNINIT_OBJECT_ID),
      timing_arcs_(UNINIT_OBJECT_ID),
      member_pins_map_() {
    setObjectType(ObjectType::kObjectTypeTTerm);
}

TTerm::TTerm(TTerm const &rhs) { copy(rhs); }

TTerm::TTerm(TTerm &&rhs) noexcept { move(std::move(rhs)); }

TTerm &TTerm::operator=(TTerm const &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TTerm &TTerm::operator=(TTerm &&rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

TTerm::IndexType TTerm::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(related_power_pin_);
    ret += sizeof(related_ground_pin_);
    ret += sizeof(function_);
    ret += sizeof(member_pins_);
    ret += sizeof(timing_arcs_);

    return ret;
}

void TTerm::set_related_power_pin(ObjectId id) { related_power_pin_ = id; }
void TTerm::set_related_ground_pin(ObjectId id) { related_ground_pin_ = id; }
TFunction *TTerm::set_function(const std::string &str) {
    Timing *timing_lib = getTimingLib();
    if (timing_lib) {
        auto p = Object::createObject<TFunction>(
                kObjectTypeTFunction, timing_lib->getId());
        if (p) {
            p->setOwner(this);
            function_ = p->getId();
            p->set_func_str(str);
        }
        return p;
    }
    return nullptr;
}
TFunction *TTerm::get_function(void) {
    if (function_ != UNINIT_OBJECT_ID) {
        return Object::addr<TFunction>(function_);
    }
    return nullptr;
}
void TTerm::add_member_pin(ObjectId id) {
    if (id == UNINIT_OBJECT_ID) return;
    auto pin = Object::addr<TTerm>(id);
    if (pin) {
        if (pin->getAttr()) {
            ArrayObject<ObjectId> *p = nullptr;
            if (member_pins_ == UNINIT_OBJECT_ID) {
                Timing *timing_lib = getTimingLib();
                if (timing_lib != nullptr) {
                    p = Object::createObject<ArrayObject<ObjectId>>(
                        kObjectTypeArray, timing_lib->getId());
                    if (p != nullptr) {
                        member_pins_ = p->getId();
                        p->setPool(timing_lib->getPool());
                        p->reserve(32);
                    }
                }
            } else {
                p = Object::addr<ArrayObject<ObjectId>>(member_pins_);
            }
            if (p != nullptr) {
                p->pushBack(id);
                member_pins_map_[pin->getAttr()->get_name_index()] = id;
            }
        }
    }
}
void TTerm::add_timingarc(ObjectId id) {
    ArrayObject<ObjectId> *p = nullptr;
    if (timing_arcs_ == UNINIT_OBJECT_ID) {
        Timing *timing_lib = getTimingLib();
        if (timing_lib != nullptr) {
            p = Object::createObject<ArrayObject<ObjectId>>(
                    kObjectTypeArray, timing_lib->getId());
            if (p != nullptr) {
                timing_arcs_ = p->getId();
                p->setPool(timing_lib->getPool());
                p->reserve(32);
            }
        }
    } else {
        p = Object::addr<ArrayObject<ObjectId>>(timing_arcs_);
    }
    p->pushBack(id);
}
TimingArc *TTerm::get_timingarc(ObjectId id) {
    if (id != UNINIT_OBJECT_ID) {
        return Object::addr<TimingArc>(id);
    }
    return nullptr;
}
TPgTerm *TTerm::get_related_power_pin(void) const {
    if (related_power_pin_ != UNINIT_OBJECT_ID) {
        return Object::addr<TPgTerm>(related_power_pin_);
    }
    return nullptr;
}
TPgTerm *TTerm::get_related_ground_pin(void) const {
    if (related_ground_pin_ != UNINIT_OBJECT_ID) {
        return Object::addr<TPgTerm>(related_ground_pin_);
    }
    return nullptr;
}
void TTerm::print(std::ostream &stream) {
    if (attr_ == nullptr) return;

    stream << "TTerm name: " << attr_->get_name() << std::endl;
}

void TTerm::copy(TTerm const &rhs) {
    this->BaseType::copy(rhs);

    related_power_pin_ = rhs.related_power_pin_;
    related_ground_pin_ = rhs.related_ground_pin_;
    function_ = rhs.function_;
    member_pins_ = rhs.member_pins_;
    timing_arcs_ = rhs.timing_arcs_;
    member_pins_map_ = rhs.member_pins_map_;
}

void TTerm::move(TTerm &&rhs) {
    this->BaseType::move(std::move(rhs));

    related_power_pin_ = std::move(rhs.related_power_pin_);
    related_ground_pin_ = std::move(rhs.related_ground_pin_);
    function_ = std::move(rhs.function_);
    member_pins_ = std::move(rhs.member_pins_);
    timing_arcs_ = std::move(rhs.timing_arcs_);
    member_pins_map_ = std::move(rhs.member_pins_map_);
}

OStreamBase &operator<<(OStreamBase &os, TTerm const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    TTerm::BaseType const &base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("related_power_pin_");
    {
        auto p = rhs.get_related_power_pin();
        if (p != nullptr)
            os << p->get_name();
        else
            os << "";
        os << DataDelimiter();
    }

    os << DataFieldName("related_ground_pin_");
    {
        auto p = rhs.get_related_ground_pin();
        if (p != nullptr)
            os << p->get_name();
        else
            os << "";
        os << DataDelimiter();
    }

    os << DataFieldName("function_") << rhs.function_;
    {
        os << DataBegin("[");
        if (rhs.function_ != UNINIT_OBJECT_ID) {
            auto p = Object::addr<TFunction>(rhs.function_);
            if (p) os << *p;
        }
        os << DataEnd("]") << DataDelimiter();
    }
    os << DataFieldName("member_pins_");
    {
        ArrayObject<ObjectId> *p = nullptr;
        if (rhs.member_pins_ != UNINIT_OBJECT_ID) {
            p = Object::addr<ArrayObject<ObjectId>>(rhs.member_pins_);
        }
        if (p != nullptr)
            os << p->getSize();
        else
            os << 0;
        os << DataBegin("[");
        if (p != nullptr) {
            auto delimiter = DataDelimiter("");
            for (int64_t i = 0; i < p->getSize(); ++i) {
                auto q = Object::addr<TTerm>((*p)[i]);
                if (q != nullptr && q->getAttr() != nullptr)
                    os << delimiter << q->getAttr()->get_name();
                else
                    os << delimiter << "";
                delimiter = DataDelimiter();
            }
        }
        os << DataEnd("]") << DataDelimiter();
    }
    os << DataFieldName("timing_arcs_");
    {
        ArrayObject<ObjectId> *p = nullptr;
        if (rhs.timing_arcs_ != UNINIT_OBJECT_ID) {
            p = Object::addr<ArrayObject<ObjectId>>(rhs.timing_arcs_);
        }
        if (p != nullptr)
            os << p->getSize();
        else
            os << 0;
        os << DataBegin("[");
        if (p != nullptr) {
            auto delimiter = DataDelimiter("");
            for (int64_t i = 0; i < p->getSize(); ++i) {
                auto q = Object::addr<TimingArc>((*p)[i]);
                if (q) {
                    os << delimiter << *q;
                    delimiter = DataDelimiter();
                }
            }
        }
        os << DataEnd("]");
    }

    os << DataEnd(")");
    return os;
}
}  // namespace db
}  // namespace open_edi
