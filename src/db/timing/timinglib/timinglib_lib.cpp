/**
 * @file timinglib_lib.cpp
 * @date 2020-08-26
 * @brief TLib Class
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/timing/timinglib/timinglib_lib.h"

#include <map>
#include <string>

#include "db/core/db.h"
#include "db/timing/timinglib/timinglib_cell.h"
#include "db/timing/timinglib/timinglib_opcond.h"
#include "db/timing/timinglib/timinglib_scalefactors.h"
#include "db/timing/timinglib/timinglib_tabletemplate.h"
#include "db/timing/timinglib/timinglib_timingtable.h"
#include "db/timing/timinglib/timinglib_units.h"
#include "db/timing/timinglib/timinglib_wireload.h"

namespace open_edi {
namespace db {

TLib::TLib()
    : TLib::BaseType(),
      default_operating_conditions_(UNINIT_OBJECT_ID),
      scaling_factors_(UNINIT_OBJECT_ID),
      units_(UNINIT_OBJECT_ID),
      default_wire_load_(UNINIT_OBJECT_ID),
      default_wire_load_selection_(UNINIT_OBJECT_ID),
      operating_conditions_(UNINIT_OBJECT_ID),
      wire_loads_(UNINIT_OBJECT_ID),
      wire_load_tables_(UNINIT_OBJECT_ID),
      wire_load_selections_(UNINIT_OBJECT_ID),
      table_templates_(UNINIT_OBJECT_ID),
      timing_cells_(UNINIT_OBJECT_ID),
      operating_conditions_map_(),
      wire_loads_map_(),
      wire_load_tables_map_(),
      wire_load_selections_map_(),
      table_templates_map_(),
      timing_cells_map_() {
    setObjectType(ObjectType::kObjectTypeTLib);
}

TLib::~TLib() {
    default_operating_conditions_ = UNINIT_OBJECT_ID;
    scaling_factors_ = UNINIT_OBJECT_ID;
    units_ = UNINIT_OBJECT_ID;
    default_wire_load_ = UNINIT_OBJECT_ID;
    default_wire_load_selection_ = UNINIT_OBJECT_ID;
    operating_conditions_ = UNINIT_OBJECT_ID;
    wire_loads_ = UNINIT_OBJECT_ID;
    wire_load_tables_ = UNINIT_OBJECT_ID;
    wire_load_selections_ = UNINIT_OBJECT_ID;
    table_templates_ = UNINIT_OBJECT_ID;
    timing_cells_ = UNINIT_OBJECT_ID;
    operating_conditions_map_.clear();
    wire_loads_map_.clear();
    wire_load_tables_map_.clear();
    wire_load_selections_map_.clear();
    table_templates_map_.clear();
    timing_cells_map_.clear();
}

TLib::TLib(Object *owner, TLib::IndexType id)
    : TLib::BaseType(owner, id),
      default_operating_conditions_(UNINIT_OBJECT_ID),
      scaling_factors_(UNINIT_OBJECT_ID),
      units_(UNINIT_OBJECT_ID),
      default_wire_load_(UNINIT_OBJECT_ID),
      default_wire_load_selection_(UNINIT_OBJECT_ID),
      wire_loads_(UNINIT_OBJECT_ID),
      wire_load_tables_(UNINIT_OBJECT_ID),
      wire_load_selections_(UNINIT_OBJECT_ID),
      table_templates_(UNINIT_OBJECT_ID),
      timing_cells_(UNINIT_OBJECT_ID),
      operating_conditions_map_(),
      wire_loads_map_(),
      wire_load_tables_map_(),
      wire_load_selections_map_(),
      table_templates_map_(),
      timing_cells_map_() {
    setObjectType(ObjectType::kObjectTypeTLib);
}

TLib::TLib(TLib const &rhs) { copy(rhs); }

TLib::TLib(TLib &&rhs) noexcept { move(std::move(rhs)); }

TLib &TLib::operator=(TLib const &rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

TLib &TLib::operator=(TLib &&rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

TLib::IndexType TLib::numTCells() const {
    if (timing_cells_ == UNINIT_OBJECT_ID) return 0;
    auto p = addr<ArrayObject<ObjectId>>(timing_cells_);
    if (p != nullptr)
        return p->getSize();
    else
        return 0;
}

TLib::IndexType TLib::numOperatingConditions() const {
    return operating_conditions_map_.size();
}

TCell *TLib::add_timing_cell(const std::string &name) {
    auto cell = __addTCellImpl();
    TCell::AttrType attr;
    attr.set_name(name);
    cell->setAttr(&attr);

    Cell *topCell = getTopCell();
    if (topCell) {
        int64_t index = topCell->getOrCreateSymbol(name.c_str());
        if (index != -1) {
            timing_cells_map_[index] = cell->getId();
            topCell->addSymbolReference(index, cell->getId());
        }
    }

    return cell;
}

OperatingConditions *TLib::add_operating_conditions(const std::string &name) {
    auto oc = __addOperatingConditionsImpl();
    oc->set_name(name);

    Cell *topCell = getTopCell();
    if (topCell) {
        int64_t index = topCell->getOrCreateSymbol(name.c_str());
        if (index != -1) {
            operating_conditions_map_[index] = oc->getId();
            topCell->addSymbolReference(index, oc->getId());
        }
    }

    return oc;
}

WireLoad *TLib::add_wire_load(const std::string &name) {
    Cell *topCell = getTopCell();
    if (topCell) {
        ArrayObject<ObjectId> *object_array = nullptr;
        if (wire_loads_ == UNINIT_OBJECT_ID) {
            object_array =
                topCell->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
            if (object_array != nullptr) {
                wire_loads_ = object_array->getId();
                object_array->setPool(topCell->getPool());
                object_array->reserve(32);
            }
        } else {
            object_array =
                addr<ArrayObject<ObjectId>>(wire_loads_);
        }
        if (object_array != nullptr) {
            auto p = topCell->createObject<WireLoad>(kObjectTypeWireLoad);
            if (p) {
                p->setOwner(this);
                p->set_name(name);
                ObjectId id = p->getId();
                object_array->pushBack(id);
                wire_loads_map_[p->get_name_index()] = id;
                return p;
            }
        }
    }
    return nullptr;
}
WireLoadTable *TLib::add_wire_load_table(const std::string &name) {
    Cell *topCell = getTopCell();
    if (topCell) {
        ArrayObject<ObjectId> *object_array = nullptr;
        if (wire_load_tables_ == UNINIT_OBJECT_ID) {
            object_array =
                topCell->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
            if (object_array != nullptr) {
                wire_load_tables_ = object_array->getId();
                object_array->setPool(topCell->getPool());
                object_array->reserve(32);
            }
        } else {
            object_array =
                addr<ArrayObject<ObjectId>>(wire_load_tables_);
        }
        if (object_array != nullptr) {
            auto p =
                topCell->createObject<WireLoadTable>(kObjectTypeWireLoadTable);
            if (p) {
                p->setOwner(this);
                p->set_name(name);
                ObjectId id = p->getId();
                object_array->pushBack(id);
                wire_load_tables_map_[p->get_name_index()] = id;
                return p;
            }
        }
    }
    return nullptr;
}
WireLoadSelection *TLib::add_wire_load_selection(const std::string &name) {
    Cell *topCell = getTopCell();
    if (topCell) {
        ArrayObject<ObjectId> *object_array = nullptr;
        if (wire_load_selections_ == UNINIT_OBJECT_ID) {
            object_array =
                topCell->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
            if (object_array != nullptr) {
                wire_load_selections_ = object_array->getId();
                object_array->setPool(topCell->getPool());
                object_array->reserve(32);
            }
        } else {
            object_array = addr<ArrayObject<ObjectId>>(
                wire_load_selections_);
        }
        if (object_array != nullptr) {
            auto p = topCell->createObject<WireLoadSelection>(
                kObjectTypeWireLoadSelection);
            if (p) {
                p->setOwner(this);
                p->set_name(name);
                ObjectId id = p->getId();
                object_array->pushBack(id);
                wire_load_selections_map_[p->get_name_index()] = id;
                return p;
            }
        }
    }
    return nullptr;
}
TableTemplate *TLib::add_table_template(const std::string &name) {
    Cell *topCell = getTopCell();
    if (topCell) {
        ArrayObject<ObjectId> *object_array = nullptr;
        if (table_templates_ == UNINIT_OBJECT_ID) {
            object_array =
                topCell->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
            if (object_array != nullptr) {
                table_templates_ = object_array->getId();
                object_array->setPool(topCell->getPool());
                object_array->reserve(32);
            }
        } else {
            object_array =
                addr<ArrayObject<ObjectId>>(table_templates_);
        }
        if (object_array != nullptr) {
            auto p =
                topCell->createObject<TableTemplate>(kObjectTypeTableTemplate);
            if (p) {
                p->setOwner(this);
                p->set_name(name);
                ObjectId id = p->getId();
                object_array->pushBack(id);
                table_templates_map_[p->get_name_index()] = id;
                return p;
            }
        }
    }
    return nullptr;
}

void TLib::set_default_operating_conditions(ObjectId id) {
    default_operating_conditions_ = id;
}
ScaleFactors *TLib::get_or_create_scaling_factors(const std::string &name) {
    if (scaling_factors_ != UNINIT_OBJECT_ID) {
        return addr<ScaleFactors>(scaling_factors_);
    } else {
        Cell *topCell = getTopCell();
        auto p = topCell->createObject<ScaleFactors>(kObjectTypeScaleFactors);
        if (p) {
            p->setOwner(this);
            p->set_name(name);
            scaling_factors_ = p->getId();
            return p;
        }
    }
    return nullptr;
}
TUnits *TLib::get_or_create_units() {
    if (units_ != UNINIT_OBJECT_ID) {
        return addr<TUnits>(units_);
    } else {
        Cell *topCell = getTopCell();
        auto p = topCell->createObject<TUnits>(kObjectTypeTUnits);
        if (p) {
            p->setOwner(this);
            units_ = p->getId();
            return p;
        }
    }
    return nullptr;
}
void TLib::set_default_wire_load(ObjectId id) { default_wire_load_ = id; }
void TLib::set_default_wire_load_selection(ObjectId id) {
    default_wire_load_selection_ = id;
}

OperatingConditions *TLib::get_default_operating_conditions(void) const {
    if (default_operating_conditions_ != UNINIT_OBJECT_ID)
        return addr<OperatingConditions>(
            default_operating_conditions_);
    else
        return nullptr;
}
ScaleFactors *TLib::get_scaling_factors(void) {
    if (scaling_factors_ != UNINIT_OBJECT_ID)
        return addr<ScaleFactors>(scaling_factors_);
    else
        return nullptr;
}
TUnits *TLib::get_units(void) {
    if (units_ != UNINIT_OBJECT_ID)
        return addr<TUnits>(units_);
    else
        return nullptr;
}
WireLoad *TLib::get_default_wire_load(void) const {
    if (default_wire_load_)
        return addr<WireLoad>(default_wire_load_);
    else
        return nullptr;
}
WireLoadSelection *TLib::get_default_wire_load_selection(void) const {
    if (default_wire_load_selection_ != UNINIT_OBJECT_ID)
        return addr<WireLoadSelection>(
            default_wire_load_selection_);
    else
        return nullptr;
}
OperatingConditions *TLib::get_operating_conditions(
    const std::string &name) const {
    Cell *topCell = getTopCell();
    if (topCell) {
        int64_t idx = topCell->getOrCreateSymbol(name.c_str());
        if (idx != -1) {
            auto it = operating_conditions_map_.find(idx);
            if (it != operating_conditions_map_.end())
                return addr<OperatingConditions>(it->second);
        }
    }
    return nullptr;
}
WireLoad *TLib::get_wire_load(const std::string &name) const {
    Cell *topCell = getTopCell();
    if (topCell) {
        int64_t idx = topCell->getOrCreateSymbol(name.c_str());
        if (idx != -1) {
            auto it = wire_loads_map_.find(idx);
            if (it != wire_loads_map_.end())
                return addr<WireLoad>(it->second);
        }
    }
    return nullptr;
}
WireLoadTable *TLib::get_wire_load_table(const std::string &name) {
    Cell *topCell = getTopCell();
    if (topCell) {
        int64_t idx = topCell->getOrCreateSymbol(name.c_str());
        if (idx != -1) {
            auto it = wire_load_tables_map_.find(idx);
            if (it != wire_load_tables_map_.end())
                return addr<WireLoadTable>(it->second);
        }
    }
    return nullptr;
}
WireLoadSelection *TLib::get_wire_load_selection(
    const std::string &name) const {
    Cell *topCell = getTopCell();
    if (topCell) {
        int64_t idx = topCell->getOrCreateSymbol(name.c_str());
        if (idx != -1) {
            auto it = wire_load_selections_map_.find(idx);
            if (it != wire_load_selections_map_.end())
                return addr<WireLoadSelection>(it->second);
        }
    }
    return nullptr;
}
TableTemplate *TLib::get_table_template(const std::string &name) {
    Cell *topCell = getTopCell();
    if (topCell) {
        int64_t idx = topCell->getOrCreateSymbol(name.c_str());
        if (idx != -1) {
            auto it = table_templates_map_.find(idx);
            if (it != table_templates_map_.end())
                return addr<TableTemplate>(it->second);
        }
    }
    return nullptr;
}
TCell *TLib::get_timing_cell(const std::string &name) {
    Cell *topCell = getTopCell();
    if (topCell) {
        int64_t idx = topCell->getOrCreateSymbol(name.c_str());
        if (idx != -1) {
            auto it = timing_cells_map_.find(idx);
            if (it != timing_cells_map_.end())
                return addr<TCell>(it->second);
        }
    }
    return nullptr;
}
std::vector<TCell *> TLib::get_timing_cells(void) {
    std::vector<TCell *> cells;
    for (auto p : timing_cells_map_) {
        auto cell = addr<TCell>(p.second);
        if (cell != nullptr) cells.emplace_back(cell);
    }
    return cells;
}
TLib::IndexType TLib::memory() const {
    IndexType ret = this->BaseType::memory();

    ret += sizeof(default_operating_conditions_);
    ret += sizeof(scaling_factors_);
    ret += sizeof(units_);
    ret += sizeof(default_wire_load_);
    ret += sizeof(default_wire_load_selection_);
    ret += sizeof(operating_conditions_);
    ret += sizeof(wire_loads_);
    ret += sizeof(wire_load_tables_);
    ret += sizeof(wire_load_selections_);
    ret += sizeof(table_templates_);
    ret += sizeof(timing_cells_);

    // TODO(liwei)

    return ret;
}

void TLib::print(std::ostream &stream) {
    if (attr_ == nullptr) return;

    stream << "TLib name: " << attr_->get_name() << " id: " << getId()
           << std::endl;

    if (timing_cells_ != UNINIT_OBJECT_ID) {
        auto p = addr<ArrayObject<ObjectId>>(timing_cells_);
        if (p != nullptr) {
            for (int64_t i = 0; i < p->getSize(); ++i) {
                auto c = addr<TCell>((*p)[i]);
                if (c) c->print(stream);
            }
        }
    }
    for (auto oc : operating_conditions_map_) {
        auto c = addr<OperatingConditions>(oc.second);
        if (c) c->print(stream);
    }
}

void TLib::copy(TLib const &rhs) {
    this->BaseType::copy(rhs);

    default_operating_conditions_ = rhs.default_operating_conditions_;
    scaling_factors_ = rhs.scaling_factors_;
    units_ = rhs.units_;
    default_wire_load_ = rhs.default_wire_load_;
    default_wire_load_selection_ = rhs.default_wire_load_selection_;
    operating_conditions_ = rhs.operating_conditions_;
    wire_loads_ = rhs.wire_loads_;
    wire_load_tables_ = rhs.wire_load_tables_;
    wire_load_selections_ = rhs.wire_load_selections_;
    table_templates_ = rhs.table_templates_;
    timing_cells_ = rhs.timing_cells_;
    operating_conditions_map_ = rhs.operating_conditions_map_;
    wire_loads_map_ = rhs.wire_loads_map_;
    wire_load_tables_map_ = rhs.wire_load_tables_map_;
    wire_load_selections_map_ = rhs.wire_load_selections_map_;
    table_templates_map_ = rhs.table_templates_map_;
    timing_cells_map_ = rhs.timing_cells_map_;
}

void TLib::move(TLib &&rhs) {
    this->BaseType::move(std::move(rhs));

    default_operating_conditions_ =
        std::move(rhs.default_operating_conditions_);
    scaling_factors_ = std::move(rhs.scaling_factors_);
    units_ = std::move(rhs.units_);
    default_wire_load_ = std::move(rhs.default_wire_load_);
    default_wire_load_selection_ = std::move(rhs.default_wire_load_selection_);
    operating_conditions_ = std::move(rhs.operating_conditions_);
    wire_loads_ = std::move(rhs.wire_loads_);
    wire_load_tables_ = std::move(rhs.wire_load_tables_);
    wire_load_selections_ = std::move(rhs.wire_load_selections_);
    table_templates_ = std::move(rhs.table_templates_);
    timing_cells_ = std::move(rhs.timing_cells_);
    operating_conditions_map_ = std::move(rhs.operating_conditions_map_);
    wire_loads_map_ = std::move(rhs.wire_loads_map_);
    wire_load_tables_map_ = std::move(rhs.wire_load_tables_map_);
    wire_load_selections_map_ = std::move(rhs.wire_load_selections_map_);
    table_templates_map_ = std::move(rhs.table_templates_map_);
    timing_cells_map_ = std::move(rhs.timing_cells_map_);

    rhs.default_operating_conditions_ = UNINIT_OBJECT_ID;
    rhs.scaling_factors_ = UNINIT_OBJECT_ID;
    rhs.units_ = UNINIT_OBJECT_ID;
    rhs.default_wire_load_ = UNINIT_OBJECT_ID;
    rhs.default_wire_load_selection_ = UNINIT_OBJECT_ID;
    operating_conditions_ = UNINIT_OBJECT_ID;
    wire_loads_ = UNINIT_OBJECT_ID;
    wire_load_tables_ = UNINIT_OBJECT_ID;
    wire_load_selections_ = UNINIT_OBJECT_ID;
    table_templates_ = UNINIT_OBJECT_ID;
    timing_cells_ = UNINIT_OBJECT_ID;
    rhs.timing_cells_ = UNINIT_OBJECT_ID;
    rhs.operating_conditions_map_.clear();
    rhs.wire_loads_map_.clear();
    rhs.wire_load_tables_map_.clear();
    rhs.wire_load_selections_map_.clear();
    rhs.table_templates_map_.clear();
    rhs.timing_cells_map_.clear();
}

template <typename T>
void output_map(const std::unordered_map<SymbolIndex, ObjectId> &origmap,
                OStreamBase *os) {
    Cell *topCell = getTopCell();
    std::map<std::string, T *> sorted_map;
    for (auto &v : origmap) {
        std::string str = topCell->getSymbolByIndex(v.first);
        auto p = Object::addr<T>(v.second);
        if (str != "" && p != nullptr) sorted_map[str] = p;
    }
    *os << sorted_map.size();
    *os << DataBegin("[");
    auto delimiter = DataDelimiter("");
    for (auto const &v : sorted_map) {
        *os << delimiter << *(v.second);
        delimiter = DataDelimiter();
    }
    *os << DataEnd("]");
}
template <typename T>
void output_vec(const ObjectId &origvec, OStreamBase *os) {
    ArrayObject<ObjectId> *p = nullptr;
    if (origvec != UNINIT_OBJECT_ID)
        p = Object::addr<ArrayObject<ObjectId>>(origvec);
    if (p == nullptr)
        *os << 0;
    else
        *os << p->getSize();
    *os << DataBegin("[");
    if (p != nullptr) {
        auto delimiter = DataDelimiter("");
        for (int64_t i = 0; i < p->getSize(); ++i) {
            auto q = Object::addr<T>((*p)[i]);
            if (q) {
                *os << delimiter << *q;
                delimiter = DataDelimiter();
            }
        }
    }
    *os << DataEnd("]");
}
OStreamBase &operator<<(OStreamBase &os, TLib const &rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    TLib::BaseType const &base = rhs;
    os << base << DataDelimiter();

    os << DataFieldName("default_operating_conditions_");
    {
        auto p = rhs.get_default_operating_conditions();
        if (p != nullptr)
            os << p->get_name();
        else
            os << "";
        os << DataDelimiter();
    }
    os << DataFieldName("scaling_factors_") << rhs.scaling_factors_;
    {
        os << DataBegin("[");
        if (rhs.scaling_factors_ != UNINIT_OBJECT_ID) {
            auto p = Object::addr<ScaleFactors>(rhs.scaling_factors_);
            if (p) os << *p;
        }
        os << DataEnd("]") << DataDelimiter();
    }
    os << DataFieldName("units_") << rhs.units_;
    {
        os << DataBegin("[");
        if (rhs.units_ != UNINIT_OBJECT_ID) {
            auto p = Object::addr<TUnits>(rhs.units_);
            if (p) os << *p;
        }
        os << DataEnd("]") << DataDelimiter();
    }
    os << DataFieldName("default_wire_load_");
    {
        auto p = rhs.get_default_wire_load();
        if (p != nullptr)
            os << p->get_name();
        else
            os << "";
        os << DataDelimiter();
    }
    os << DataFieldName("default_wire_load_selection_");
    {
        auto p = rhs.get_default_wire_load_selection();
        if (p != nullptr)
            os << p->get_name();
        else
            os << "";
        os << DataDelimiter();
    }
    os << DataFieldName("operating_conditions_");
    {
        output_map<OperatingConditions>(rhs.operating_conditions_map_, &os);
        os << DataDelimiter();
    }
    os << DataFieldName("wire_loads_");
    {
        output_map<WireLoad>(rhs.wire_loads_map_, &os);
        os << DataDelimiter();
    }
    os << DataFieldName("wire_load_tables_");
    {
        output_map<WireLoadTable>(rhs.wire_load_tables_map_, &os);
        os << DataDelimiter();
    }
    os << DataFieldName("wire_load_selections_");
    {
        output_map<WireLoadSelection>(rhs.wire_load_selections_map_, &os);
        os << DataDelimiter();
    }
    os << DataFieldName("table_templates_");
    {
        output_map<TableTemplate>(rhs.table_templates_map_, &os);
        os << DataDelimiter();
    }
    os << DataFieldName("timing_cells_");
    { output_vec<TCell>(rhs.timing_cells_, &os); }

    os << DataEnd(")");
    return os;
}

TCell *TLib::__addTCellImpl() {
    Cell *topCell = getTopCell();
    if (topCell) {
        ArrayObject<ObjectId> *p = nullptr;
        if (timing_cells_ == UNINIT_OBJECT_ID) {
            p = topCell->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
            if (p != nullptr) {
                timing_cells_ = p->getId();
                p->setPool(topCell->getPool());
                p->reserve(32);
            }
        } else {
            p = addr<ArrayObject<ObjectId>>(timing_cells_);
        }
        if (p != nullptr) {
            auto c = topCell->createObject<TCell>(kObjectTypeTCell);
            if (c) {
                c->setOwner(this);
                ObjectId id = c->getId();
                p->pushBack(id);
                return c;
            }
        }
    }
    return nullptr;
}

OperatingConditions *TLib::__addOperatingConditionsImpl() {
    Cell *topCell = getTopCell();
    if (topCell) {
        ArrayObject<ObjectId> *object_array = nullptr;
        if (operating_conditions_ == UNINIT_OBJECT_ID) {
            object_array =
                topCell->createObject<ArrayObject<ObjectId>>(kObjectTypeArray);
            if (object_array != nullptr) {
                operating_conditions_ = object_array->getId();
                object_array->setPool(topCell->getPool());
                object_array->reserve(32);
            }
        } else {
            object_array = addr<ArrayObject<ObjectId>>(
                operating_conditions_);
        }
        if (object_array != nullptr) {
            auto c = topCell->createObject<OperatingConditions>(
                kObjectTypeOperatingConditions);
            if (c) {
                c->setOwner(this);
                ObjectId id = c->getId();
                object_array->pushBack(id);
                return c;
            }
        }
    }
    return nullptr;
}
}  // namespace db
}  // namespace open_edi
