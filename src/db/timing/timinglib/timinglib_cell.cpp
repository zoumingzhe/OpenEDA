
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

TTerm *TCell::get_or_create_term(const std::string &name) {
    Cell *topCell = getTopCell();
    if (topCell) {
        SymbolIndex idx = topCell->getOrCreateSymbol(name.c_str());
        if (idx != kInvalidSymbolIndex) {
            auto t = term_map_.find(idx);
            if (t != term_map_.end()) {
                return Object::addr<TTerm>(t->second);
            } else {
                auto term = __addTermImpl();
                if (term) {
                    TTerm::AttrType attr;
                    attr.set_name(name);
                    term->setAttr(&attr);
                    term_map_[attr.get_name_index()] = term->getId();
                }
                return term;
            }
        }
    }
    return nullptr;
}
TTerm *TCell::get_term(const std::string &name) {
    Cell *topCell = getTopCell();
    if (topCell) {
        SymbolIndex idx = topCell->getOrCreateSymbol(name.c_str());
        if (idx != kInvalidSymbolIndex) {
            auto t = term_map_.find(idx);
            if (t != term_map_.end()) {
                return Object::addr<TTerm>(t->second);
            }
        }
    }
    return nullptr;
}
std::vector<TTerm *> TCell::get_terms(void) {
    std::vector<TTerm *> terms;
    for (auto p : term_map_) {
        auto term = Object::addr<TTerm>(p.second);
        if (term != nullptr) terms.emplace_back(term);
    }
    return terms;
}
void TCell::reset_terms(const std::vector<TTerm *> &terms) {
    if (terms.empty() && tterms_ == UNINIT_OBJECT_ID) return;

    Cell *topCell = getTopCell();
    if (topCell) {
        ArrayObject<ObjectId> *p = nullptr;
        if (tterms_ == UNINIT_OBJECT_ID) {
            p = topCell->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
            if (p) {
                tterms_ = p->getId();
                p->setPool(topCell->getPool());
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
                    if (terms[i]->getAttr()) {
                        term = terms[i]->getId();
                        term_map_[terms[i]->getAttr()->get_name_index()] =
                            terms[i]->getId();
                    }
                }

            } else {
                for (int64_t i = 0; i < orig_size; ++i) {
                    auto &term = (*p)[i];
                    if (terms[i]->getAttr()) {
                        term = terms[i]->getId();
                        term_map_[terms[i]->getAttr()->get_name_index()] =
                            terms[i]->getId();
                    }
                }
                for (IndexType i = orig_size; i < cur_size; ++i) {
                    auto &term = terms[i];
                    if (term->getAttr()) {
                        ObjectId id = term->getId();
                        p->pushBack(id);
                        term_map_[term->getAttr()->get_name_index()] = id;
                    }
                }
            }
        }
    }
}
TPgTerm *TCell::get_or_create_pgTerm(const std::string &name) {
    Cell *topCell = getTopCell();
    if (topCell) {
        SymbolIndex idx = topCell->getOrCreateSymbol(name.c_str());
        if (idx != kInvalidSymbolIndex) {
            auto t = pg_term_map_.find(idx);
            if (t != pg_term_map_.end()) {
                return Object::addr<TPgTerm>(t->second);
            } else {
                auto term = __addPgTermImpl();
                if (term) {
                    term->set_name(name);
                    pg_term_map_[term->get_name_index()] = term->getId();
                }
                return term;
            }
        }
    }
    return nullptr;
}
TPgTerm *TCell::get_pgTerm(const std::string &name) {
    Cell *topCell = getTopCell();
    if (topCell) {
        SymbolIndex idx = topCell->getOrCreateSymbol(name.c_str());
        if (idx != kInvalidSymbolIndex) {
            auto t = pg_term_map_.find(idx);
            if (t != pg_term_map_.end()) {
                return Object::addr<TPgTerm>(t->second);
            }
        }
    }
    return nullptr;
}
std::vector<TPgTerm *> TCell::get_pgTerms(void) {
    std::vector<TPgTerm *> pg_terms;
    for (auto p : pg_term_map_) {
        auto term = Object::addr<TPgTerm>(p.second);
        if (term != nullptr) pg_terms.emplace_back(term);
    }
    return pg_terms;
}
TCell::IndexType TCell::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(tterms_);
    ret += sizeof(tpg_terms_);

    return ret;
}

bool TCell::is_physical_only(void) {
    if (numTerms() == 0 && numPgTerms() != 0) return true;
    return false;
}

bool TCell::is_combinational(void) {
    TCell::AttrType *attr = getAttr();
    if (attr) {
        if (attr->is_sequential() == false && is_physical_only() == false)
            return true;
    }
    return false;
}
bool TCell::has_multi_power_rails(void) {
    if (numPgTerms() > 1) return true;
    return false;
}
void TCell::print(std::ostream &stream) {
    if (attr_ == nullptr) return;
    stream << "TCell name: " << attr_->get_name() << " id: " << getId()
           << std::endl;
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

std::string TCell::get_full_name(void) {
    std::string str = "";
    Object *owner_obj = nullptr;

    if (owner_ != UNINIT_OBJECT_ID) {
        owner_obj = Object::addr<Object>(owner_);
    }
    if (owner_obj != nullptr &&
        owner_obj->getObjectType() == ObjectType::kObjectTypeTLib) {
        TLib::AttrType *attr = static_cast<TLib *>(owner_obj)->getAttr();
        if (attr != nullptr) str = attr->get_name();
    }
    if (attr_ != nullptr) {
        if (str != "")
            str = str + "_" + attr_->get_name();
        else
            str = attr_->get_name();
    }

    return str;
}

TCell::IndexType TCell::get_num_pins(void) { return numTerms() + numPgTerms(); }

void TCell::copy(TCell const &rhs) {
    this->BaseType::copy(rhs);

    tterms_ = rhs.tterms_;
    tpg_terms_ = rhs.tpg_terms_;
    term_map_ = rhs.term_map_;
    pg_term_map_ = rhs.pg_term_map_;
}

void TCell::move(TCell &&rhs) {
    this->BaseType::move(std::move(rhs));

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
    Cell *topCell = getTopCell();
    if (topCell) {
        ArrayObject<ObjectId> *p = nullptr;
        if (tterms_ == UNINIT_OBJECT_ID) {
            p = topCell->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
            if (p) {
                tterms_ = p->getId();
                p->setPool(topCell->getPool());
                p->reserve(32);
            }
        } else {
            p = Object::addr<ArrayObject<ObjectId>>(tterms_);
        }
        if (p != nullptr) {
            auto term = topCell->createObject<TTerm>(kObjectTypeTTerm);
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
    Cell *topCell = getTopCell();
    if (topCell) {
        ArrayObject<ObjectId> *p = nullptr;
        if (tpg_terms_ == UNINIT_OBJECT_ID) {
            p = topCell->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
            if (p) {
                tpg_terms_ = p->getId();
                p->setPool(topCell->getPool());
                p->reserve(32);
            }
        } else {
            p = Object::addr<ArrayObject<ObjectId>>(tpg_terms_);
        }
        if (p != nullptr) {
            auto pg_term = topCell->createObject<TPgTerm>(kObjectTypeTPgTerm);
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
