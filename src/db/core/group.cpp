/* @file  group.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/core/group.h"

#include "db/core/cell.h"
#include "db/core/db.h"
#include "db/util/vector_object_var.h"

namespace open_edi {
namespace db {
using IdArray = ArrayObject<ObjectId>;
using SymIdxArray = ArrayObject<SymbolIndex>;

Group::Group() { memset((void *)this, 0, sizeof(Group)); }

Group::~Group() {}

void Group::setName(std::string &name) {
    Cell *cell = getOwnerCell();
    if (!cell) {
        message->issueMsg(
            kError, "Cannot get top cell when adding Group name %s.\n", name);
        return;
    }

    SymbolIndex symbol_index = cell->getOrCreateSymbol(name);
    name_index_ = symbol_index;
}

std::string Group::getName() const {
    std::string group_name;
    Cell *cell = getOwnerCell();
    if (!cell) {
        message->issueMsg(kError,
                          "Cannot get top cell when adding Group name %s.\n");
        return group_name;
    }

    return cell->getSymbolTable()->getSymbolByIndex(name_index_);
}

void Group::setNameIndex(SymbolIndex name_index) { name_index_ = name_index; }
SymbolIndex Group::getNameIndex() const { return name_index_; }

void Group::addPattern(const char *pattern_name) {
    Cell *cell = getOwnerCell();
    if (!cell) {
        message->issueMsg(
            kError,
            "Cannot get top cell when adding component name pattern %s.\n",
            pattern_name);
        return;
    }
    SymIdxArray *sym_idx_array_ptr = nullptr;
    SymbolIndex symbol_index = cell->getOrCreateSymbol(pattern_name);
    if (pattern_names_ == 0) {
        sym_idx_array_ptr =
          cell->createObject<SymIdxArray>(kObjectTypeArray);
        ediAssert(sym_idx_array_ptr != nullptr);
        sym_idx_array_ptr->setPool(cell->getPool());
        sym_idx_array_ptr->reserve(8);
        pattern_names_ = sym_idx_array_ptr->getId();
    } else {
        sym_idx_array_ptr = addr<SymIdxArray>(pattern_names_);
        ediAssert(sym_idx_array_ptr != nullptr);
    }
    
    sym_idx_array_ptr->pushBack(symbol_index);
}

ObjectId Group::getPatternNames() const {
    return pattern_names_;
}

void Group::setRegion(ObjectId v) {
    region_ = v;
}

Constraint *Group::getRegion() const {
    if (region_ != 0) {
        return addr<Constraint>(region_);
    }
    return nullptr;
}

void Group::addInstance(ObjectId instance_id) {
    if (instances_ == 0) {
        instances_ = __createObjectIdArray(8);
    }
    ediAssert(instances_ != 0);
    IdArray *id_array_ptr = addr<IdArray>(instances_);
    ediAssert(id_array_ptr != nullptr);
    id_array_ptr->pushBack(instance_id);
}

ObjectId Group::getInstances() const { 
    return instances_;
}

void Group::setPropertySize(uint64_t v) {
    if (v == 0 && properties_id_) {
        __deleteObjectIdArray(properties_id_);
        return;
    }
    if (!properties_id_) {
        properties_id_ = __createObjectIdArray(16);
    }
}

uint64_t Group::getNumProperties() const {
    if (!properties_id_) return 0;

    return addr<IdArray>(properties_id_)->getSize();
}

void Group::addProperty(ObjectId obj_id) {
    if (obj_id == 0) return;
    if (properties_id_ == 0) {
        properties_id_ = __createObjectIdArray(16);
    } 
    ediAssert(properties_id_ != 0);

    IdArray *id_array_ptr = addr<IdArray>(properties_id_);
    ediAssert(id_array_ptr != nullptr);
    id_array_ptr->pushBack(obj_id);
}

ObjectId Group::getPropertiesId() const { return properties_id_; }

#if 0
void Group::print() {
    Cell *top_cell = getTopCell();
    message->info("- %s", getName().c_str());
    std::vector<ObjectId> instance_id_vector = getInstanceIds();
    for (int i = 0; i < instance_id_vector.size(); ++i) {
        Inst *instance = top_cell->getInstance(instance_id_vector[i]);
        if (!instance) {
            message->issueMsg(kError,
                              "Cannot find instance with object id %d.\n",
                              instance_id_vector[i]);
            continue;
        }
        message->info(" %s", instance->getName().c_str());
    }
    message->info("\n");
    if (region_) {
        Constraint *region = addr<Constraint>(region_);
        ediAssert(region != nullptr);
        message->info(" + REGION %s ;\n", region->getName().c_str());
    }    
}
#endif

void Group::print(FILE *fp) {
    fprintf(fp, "- %s", getName().c_str());
    if (instances_) {
        IdArray *inst_array = addr<IdArray>(instances_);
        for (int i = 0; i < inst_array->getSize(); ++i) {
            Inst *instance = addr<Inst>((*inst_array)[i]);
            if (!instance) {
                message->issueMsg(kError,
                                  "Cannot find instance with object id %d.\n",
                                  inst_array[i]);
                continue;
            }
            fprintf(fp, " %s", instance->getName().c_str());
        }
    }
    if (region_) {
        Constraint *region = addr<Constraint>(region_);
        ediAssert(region != nullptr);
        fprintf(fp, "\n  + REGION %s", region->getName().c_str());
    }

    writeDEFProperty<Group>((void *)this, fp);
    fprintf(fp, " ;\n");
}
}  // namespace db
}  // namespace open_edi
