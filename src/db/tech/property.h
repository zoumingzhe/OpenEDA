/**
 * @file  property.h
 * @date  08/21/2020 04:34:28 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_DB_TECH_PROPERTY_H_
#define EDI_DB_TECH_PROPERTY_H_

#include "db/core/object.h"
#include "db/tech/type_def.h"
#include "db/util/vector_object_var.h"
#include "util/util.h"

namespace open_edi {
namespace db {

/**
 * @class Property
 * @brief for PROPERTY
 * [PROPERTY propName propVal ;]
 * */
class Property : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    Property();

    /// @brief constructor
    Property(Object *owner, UInt32 id);

    /// @brief destructor
    ~Property();

    /// @brief copy constructor
    Property(Property const &rhs);

    /// @brief move constructor
    Property(Property &&rhs) noexcept;

    /// @brief copy assignment
    Property &operator=(Property const &rhs);

    /// @brief move constructor
    Property &operator=(Property &&rhs) noexcept;
    /// @brief clone current object for extensibility.
    /// Users need to manage the pointer to avoid memory leakage.
    /// @return the pointer to the new object
    Property *clone() const;

    /// @brief summarize memory usage of the object in bytes
    UInt32 memory() const;

    // ACCESS
    // Get
    ObjectId getDefinitionId() const;
    PropType getPropType() const;
    const char *getPropName() const;
    PropDataType getDataType() const;
    bool getHasRange() const;
    int getIntRangeMin() const;
    int getIntRangeMax() const;
    double getRealRangeMin() const;
    double getRealRangeMax() const;
    int getIntValue() const;
    double getRealValue() const;
    std::string getStringValue() const;
    // Set
    void setDefinitionId(PropType type, const std::string &name);
    void setDefinitionId(ObjectId v);
    void setIntValue(int v);
    void setRealValue(double v);
    void setStringValue(const std::string &v);
    // Print
    void printLEF(std::ofstream &ofs) const;
    void printDEF(FILE *fp) const;

  protected:
    /// @brief copy object
    void copy(Property const &rhs);
    /// @brief move object
    void move(Property &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, Property const &rhs);
    /// @brief overload input stream
    // friend IStreamBase &operator>>(IStreamBase &is, Property &rhs) {}

  private:
    // DATA
    ObjectId definition_id_;
    union {
        SymbolIndex string_value_;
        int int_value_;
        double real_value_;
    } value_u_;
};

template <class object_type>
void writeDEFProperty(void *obj, FILE *fp) {
    object_type *object = (object_type *)obj;

    if (object->getNumProperties() > 0) {
        VectorObject16 *vobj =
            Object::addr<VectorObject16>(object->getPropertiesId());
        for (int i = 0; i < object->getNumProperties(); i++) {
            ObjectId obj_id = (*vobj)[i];
            if (!obj_id) continue;
            Property *property = Object::addr<Property>(obj_id);
            if (property == nullptr) continue;
            property->printDEF(fp);
        }
    }
}

}  // namespace db
}  // namespace open_edi

#endif
