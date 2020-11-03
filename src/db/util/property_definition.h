/** 
 * @file  property_definition.h
 * @date  08/21/2020 04:34:28 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_DB_UTIL_PROPERTYDEFINITION_H_
#define EDI_DB_UTIL_PROPERTYDEFINITION_H_

#include "db/core/object.h"
#include "util/util.h"
#include "db/tech/type_def.h"

namespace open_edi {
namespace db {

struct IntRange {
  int min;
  int max;
};

struct RealRange {
  double min;
  double max;
};

/**
 * @class PropertyDefinition
 * @brief for PROPERTYDEFINITIONS
 * [PROPERTYDEFINITIONS 
 *      [objectType propName propType [RANGE min max] [value | "stringValue"] ;] ... 
 *    END PROPERTYDEFINITIONS] 
 * */
class PropertyDefinition : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    PropertyDefinition();

    /// @brief constructor
    PropertyDefinition(Object* owner, UInt32 id);

    /// @brief destructor
    ~PropertyDefinition();

    /// @brief copy constructor
    PropertyDefinition(PropertyDefinition const &rhs);

    /// @brief move constructor
    PropertyDefinition(PropertyDefinition &&rhs) noexcept;

    /// @brief copy assignment
    PropertyDefinition &operator=(PropertyDefinition const &rhs);

    /// @brief move constructor
    PropertyDefinition &operator=(PropertyDefinition &&rhs) noexcept;
    /// @brief clone current object for extensibility.
    /// Users need to manage the pointer to avoid memory leakage.
    /// @return the pointer to the new object
    PropertyDefinition *clone() const;

    /// @brief summarize memory usage of the object in bytes
    UInt32 memory() const;

    //ACCESS
    //Get
    PropType getPropType() const;
    const std::string & getPropName() const;
    PropDataType getDataType() const;
    bool getFromDEF() const;
    bool getHasRange() const;
    bool getHasValue() const;
    int getIntRangeMin() const;
    int getIntRangeMax() const;
    double getRealRangeMin() const;
    double getRealRangeMax() const;
    int getIntValue() const;
    double getRealValue() const;
    std::string getStringValue() const;
    //Set
    void setPropType(PropType v);
    void setPropName(const std::string & v);
    void setDataType(PropDataType v);
    void setFromDEF(bool v);
    void setHasRange(bool v);
    void setIntRangeMin(int v);
    void setIntRangeMax(int v);
    void setRealRangeMin(double v);
    void setRealRangeMax(double v);
    //TODO: check range...
    void setIntValue(int v);
    void setRealValue(double v);
    void setStringValue(const std::string & v);
    //Print
    void printLEF(std::ofstream & ofs) const;
    void printDEF(FILE *fp) const;

  protected:
    /// @brief copy object
    void copy(PropertyDefinition const &rhs);
    /// @brief move object
    void move(PropertyDefinition &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, PropertyDefinition const &rhs);
    /// @brief overload input stream
    //friend IStreamBase &operator>>(IStreamBase &is, PropertyDefinition &rhs) {}

  private:
    //DATA
    PropType prop_type_;
    SymbolIndex  name_index_;
    PropDataType data_type_;
    bool from_def_;
    bool has_range_;
    bool has_value_;
    union {
      IntRange int_range_;
      RealRange real_range_;
    } range_u_;
    union {
      SymbolIndex string_value_;
      int int_value_;
      double real_value_;
    } value_u_;
};


}  // namespace db 
}  // namespace open_edi

#endif


