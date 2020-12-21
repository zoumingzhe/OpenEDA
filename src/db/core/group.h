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

    void setName(std::string& name);
    std::string getName() const;

    void setNameIndex(SymbolIndex name_index);
    SymbolIndex getNameIndex() const;

    void addPattern(const char* pattern_name);
    ObjectId getPatternNames() const;

    void setRegion(ObjectId v);
    Constraint* getRegion() const;

    void addInstance(ObjectId instance_id);
    ObjectId getInstances() const;

    void setPropertySize(uint64_t v);
    uint64_t getNumProperties() const;
    ObjectId addProperty(ObjectId prop_id);
    ObjectId getPropertiesId() const;

    void print();
    void print(FILE* fp);

  private:
    // definition in DEF : GROUP
    SymbolIndex name_index_;
    ObjectId region_;
    // TODO (ly) : wildcard matching
    ObjectId pattern_names_; 
    ObjectId instances_;
    ObjectId properties_id_;
};
}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_PHYSICAL_GROUP_H_
