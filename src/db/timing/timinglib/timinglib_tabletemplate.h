/**
 * @file timinglib_tabletemplate.h
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
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TABLETEMPLATE_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TABLETEMPLATE_H_

#include <algorithm>
#include <string>
#include <utility>

#include "db/core/object.h"
#include "db/timing/timinglib/timinglib_commondef.h"
#include "db/util/array.h"

namespace open_edi {
namespace db {

class TableAxis : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    TableAxis();

    /// @brief destructor
    ~TableAxis();

    /// @brief constructor
    TableAxis(Object *owner, IndexType id);

    /// @brief copy constructor
    TableAxis(TableAxis const &rhs);

    /// @brief move constructor
    TableAxis(TableAxis &&rhs) noexcept;

    /// @brief copy assignment
    TableAxis &operator=(TableAxis const &rhs);

    /// @brief move assignment
    TableAxis &operator=(TableAxis &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    void setVariable(TableAxisVariable tv);
    void addValue(float f);

    /// get
    TableAxisVariable getVariable(void);
    ArrayObject<float> *getValues(void);
    IndexType getSize(void);

  protected:
    /// @brief copy object
    void copy(TableAxis const &rhs);
    /// @brief move object
    void move(TableAxis &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TableAxis const &rhs);

  private:
    TableAxisVariable variable_;
    ObjectId values_;
};

class TableTemplate : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    TableTemplate();

    /// @brief destructor
    ~TableTemplate();

    /// @brief constructor
    TableTemplate(Object *owner, IndexType id);

    /// @brief copy constructor
    TableTemplate(TableTemplate const &rhs);

    /// @brief move constructor
    TableTemplate(TableTemplate &&rhs) noexcept;

    /// @brief copy assignment
    TableTemplate &operator=(TableTemplate const &rhs);

    /// @brief move assignment
    TableTemplate &operator=(TableTemplate &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    void setName(const std::string &name);
    TableAxis *createAxis1();
    TableAxis *createAxis2();
    TableAxis *createAxis3();

    /// get
    std::string getName(void) const;
    SymbolIndex getNameIndex(void);
    TableAxis *getAxis1(void);
    TableAxis *getAxis2(void);
    TableAxis *getAxis3(void);

  protected:
    /// @brief copy object
    void copy(TableTemplate const &rhs);
    /// @brief move object
    void move(TableTemplate &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TableTemplate const &rhs);

  private:
    SymbolIndex name_;
    ObjectId axis1_;
    ObjectId axis2_;
    ObjectId axis3_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TABLETEMPLATE_H_
