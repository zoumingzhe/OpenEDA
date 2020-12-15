/**
 * @file timinglib_timingtable.h
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
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TIMINGTABLE_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TIMINGTABLE_H_

#include <algorithm>
#include <utility>
#include <vector>

#include "db/core/object.h"
#include "db/timing/timinglib/timinglib_commondef.h"

namespace open_edi {
namespace db {

class TableAxis;

class TimingTable : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    TimingTable();

    /// @brief destructor
    virtual ~TimingTable();

    /// @brief constructor
    TimingTable(Object *owner, IndexType id);

    /// @brief copy constructor
    TimingTable(TimingTable const &rhs);

    /// @brief move constructor
    TimingTable(TimingTable &&rhs) noexcept;

    /// @brief copy assignment
    TimingTable &operator=(TimingTable const &rhs);

    /// @brief move assignment
    TimingTable &operator=(TimingTable &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// get
    virtual TableAxis *getAxis1(void);
    virtual TableAxis *getAxis2(void);
    virtual TableAxis *getAxis3(void);

  protected:
    /// @brief copy object
    void copy(TimingTable const &rhs);
    /// @brief move object
    void move(TimingTable &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TimingTable const &rhs);
};

class TimingTable0 : public TimingTable {
  public:
    using BaseType = TimingTable;

    /// @brief default constructor
    TimingTable0();

    /// @brief destructor
    ~TimingTable0();

    /// @brief constructor
    TimingTable0(Object *owner, IndexType id);

    /// @brief copy constructor
    TimingTable0(TimingTable0 const &rhs);

    /// @brief move constructor
    TimingTable0(TimingTable0 &&rhs) noexcept;

    /// @brief copy assignment
    TimingTable0 &operator=(TimingTable0 const &rhs);

    /// @brief move assignment
    TimingTable0 &operator=(TimingTable0 &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    void setValue(float f);

    /// get
    float getValue(void);

  protected:
    /// @brief copy object
    void copy(TimingTable0 const &rhs);
    /// @brief move object
    void move(TimingTable0 &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TimingTable0 const &rhs);

  private:
    float value_;
};

class TimingTable1 : public TimingTable {
  public:
    using BaseType = TimingTable;

    /// @brief default constructor
    TimingTable1();

    /// @brief destructor
    ~TimingTable1();

    /// @brief constructor
    TimingTable1(Object *owner, IndexType id);

    /// @brief copy constructor
    TimingTable1(TimingTable1 const &rhs);

    /// @brief move constructor
    TimingTable1(TimingTable1 &&rhs) noexcept;

    /// @brief copy assignment
    TimingTable1 &operator=(TimingTable1 const &rhs);

    /// @brief move assignment
    TimingTable1 &operator=(TimingTable1 &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    void addValue(float f);
    void setAxis1(ObjectId id);

    /// get
    std::vector<float> getValues(void);
    TableAxis *getAxis1();

  protected:
    /// @brief copy object
    void copy(TimingTable1 const &rhs);
    /// @brief move object
    void move(TimingTable1 &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TimingTable1 const &rhs);

  private:
    ObjectId values_;
    ObjectId axis1_;
};

class TimingTable2 : public TimingTable {
  public:
    using BaseType = TimingTable;

    /// @brief default constructor
    TimingTable2();

    /// @brief destructor
    ~TimingTable2();

    /// @brief constructor
    TimingTable2(Object *owner, IndexType id);

    /// @brief copy constructor
    TimingTable2(TimingTable2 const &rhs);

    /// @brief move constructor
    TimingTable2(TimingTable2 &&rhs) noexcept;

    /// @brief copy assignment
    TimingTable2 &operator=(TimingTable2 const &rhs);

    /// @brief move assignment
    TimingTable2 &operator=(TimingTable2 &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    virtual void addValue(float f);
    virtual void setAxis1(ObjectId id);
    virtual void setAxis2(ObjectId id);

    /// get
    float getValue(IndexType index1, IndexType index2);
    TableAxis *getAxis1();
    TableAxis *getAxis2();

  protected:
    /// @brief copy object
    void copy(TimingTable2 const &rhs);
    /// @brief move object
    void move(TimingTable2 &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TimingTable2 const &rhs);

    ObjectId values_;
    ObjectId axis1_;
    ObjectId axis2_;
};

class TimingTable3 : public TimingTable2 {
  public:
    using BaseType = TimingTable2;

    /// @brief default constructor
    TimingTable3();

    /// @brief destructor
    ~TimingTable3();

    /// @brief constructor
    TimingTable3(Object *owner, IndexType id);

    /// @brief copy constructor
    TimingTable3(TimingTable3 const &rhs);

    /// @brief move constructor
    TimingTable3(TimingTable3 &&rhs) noexcept;

    /// @brief copy assignment
    TimingTable3 &operator=(TimingTable3 const &rhs);

    /// @brief move assignment
    TimingTable3 &operator=(TimingTable3 &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    void setAxis3(ObjectId id);

    /// get
    float getValue(IndexType index1, IndexType index2, IndexType index3);
    TableAxis *getAxis3();

  protected:
    /// @brief copy object
    void copy(TimingTable3 const &rhs);
    /// @brief move object
    void move(TimingTable3 &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TimingTable3 const &rhs);

  private:
    ObjectId axis3_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TIMINGTABLE_H_
