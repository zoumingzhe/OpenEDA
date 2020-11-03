/* @file  group.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_DB_PHYSICAL_GROUP_H_
#define EDI_DB_PHYSICAL_GROUP_H_

#include "db/core/object.h"
#include "db/util/symbol_table.h"

namespace open_edi {
namespace db {

class Cell;
class Constraint;
class Group : public Object {
  public:
    Group();
    ~Group();

    void setCell(ObjectId cell);
    Cell* getCell() const;

    void setName(std::string& name);
    std::string getName() const;

    void setNameIndex(SymbolIndex name_index);
    SymbolIndex getNameIndex() const;

    void addPattern(const char* pattern_name);
    std::vector<SymbolIndex>& getPatternIndexes();

    void setHasRegion(bool has_region);
    bool getHasRegion() const;

    void addRegion(const char* region_name);
    SymbolIndex getRegionIndex() const;
    Constraint* getRegion() const;

    void addInstanceId(ObjectId instance_id);
    std::vector<ObjectId>& getInstanceIds();

    void setPropertySize(uint64_t v);
    uint64_t getNumProperties() const;
    void addProperty(ObjectId prop_id);
    ObjectId getPropertiesId() const;

    void print();
    void print(FILE* fp);

  private:
    ObjectId cell_;
    // definition in DEF
    SymbolIndex name_index_;
    std::vector<SymbolIndex> pattern_index_vector_;
    bool has_region_;
    SymbolIndex region_index_;
    ObjectId properties_id_;

    // relation to other objects
    std::vector<ObjectId> instance_id_vector_;
};
}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_PHYSICAL_GROUP_H_
