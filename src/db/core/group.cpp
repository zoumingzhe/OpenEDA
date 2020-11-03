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

Group::Group() { memset((void *)this, 0, sizeof(Group)); }

Group::~Group() {}

void Group::setCell(ObjectId cell) { cell_ = cell; }

Cell *Group::getCell() const { return addr<Cell>(cell_); }

void Group::setName(std::string &name) {
    Cell *cell = getCell();
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
    Cell *cell = getCell();
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
    Cell *cell = getCell();
    if (!cell) {
        message->issueMsg(
            kError,
            "Cannot get top cell when adding component name pattern %s.\n",
            pattern_name);
        return;
    }

    SymbolIndex symbol_index = cell->getOrCreateSymbol(pattern_name);
    pattern_index_vector_.push_back(symbol_index);
}
std::vector<SymbolIndex> &Group::getPatternIndexes() {
    return pattern_index_vector_;
}

void Group::setHasRegion(bool has_region) { has_region_ = has_region; }
bool Group::getHasRegion() const { return has_region_; }

void Group::addRegion(const char *region_name) {
    Cell *cell = getCell();
    if (!cell) {
        message->issueMsg(kError,
                          "Cannot get top cell when adding region name %s.\n",
                          region_name);
        return;
    }

    SymbolIndex symbol_index = cell->getOrCreateSymbol(region_name);
    region_index_ = symbol_index;
}
SymbolIndex Group::getRegionIndex() const { return region_index_; }
Constraint *Group::getRegion() const {
    return addr<Constraint>(region_index_);
}

void Group::addInstanceId(ObjectId instance_id) {
    instance_id_vector_.push_back(instance_id);
}
std::vector<ObjectId> &Group::getInstanceIds() { return instance_id_vector_; }
void Group::setPropertySize(uint64_t v) {
    if (v == 0 && properties_id_) {
        VectorObject16::deleteDBVectorObjectVar(properties_id_);
        return;
    }
    if (!properties_id_) {
        VectorObject16 *vobj =
            VectorObject16::createDBVectorObjectVar(true /*is_header*/);
        ediAssert(vobj != nullptr);
        // using push_back to insert...remove reserve().
        // vobj->reserve(v);
        properties_id_ = vobj->getId();
    }
}

uint64_t Group::getNumProperties() const {
    if (!properties_id_) return 0;

    return addr<VectorObject16>(properties_id_)->totalSize();
}

void Group::addProperty(ObjectId obj_id) {
    VectorObject16 *vobj = nullptr;
    if (obj_id == 0) return;

    if (properties_id_ == 0) {
        vobj = VectorObject16::createDBVectorObjectVar(true /*is_header*/);
        properties_id_ = vobj->getId();
    } else {
        vobj = addr<VectorObject16>(properties_id_);
    }
    ediAssert(vobj != nullptr);
    vobj->push_back(obj_id);
}

ObjectId Group::getPropertiesId() const { return properties_id_; }

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
    if (getHasRegion()) {
        std::string region_name =
            top_cell->getSymbolTable()->getSymbolByIndex(getRegionIndex());
        message->info(" + REGION %s ;\n", region_name.c_str());
    }
}

void Group::print(FILE *fp) {
    Cell *top_cell = getTopCell();
    fprintf(fp, "- %s", getName().c_str());
    std::vector<ObjectId> instance_id_vector = getInstanceIds();
    for (int i = 0; i < instance_id_vector.size(); ++i) {
        Inst *instance = top_cell->getInstance(instance_id_vector[i]);
        if (!instance) {
            message->issueMsg(kError,
                              "Cannot find instance with object id %d.\n",
                              instance_id_vector[i]);
            continue;
        }
        fprintf(fp, " %s", instance->getName().c_str());
    }
    if (getHasRegion()) {
        std::string region_name =
            top_cell->getSymbolTable()->getSymbolByIndex(getRegionIndex());
        fprintf(fp, "\n  + REGION %s", region_name.c_str());
    }

    writeDEFProperty<Group>((void *)this, fp);
    fprintf(fp, " ;\n");
}
}  // namespace db
}  // namespace open_edi
