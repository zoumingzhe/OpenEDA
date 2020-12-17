/**
 * @file timinglib_wireload.h
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
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_WIRELOAD_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_WIRELOAD_H_

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "db/core/object.h"
#include "db/timing/timinglib/timinglib_commondef.h"
#include "db/util/array.h"

namespace open_edi {
namespace db {

typedef struct fanout_pair {
    bool valid = false;
    int num = 0;
    float value = 0.0f;
} FanoutPair;

class WireLoadTable : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    WireLoadTable();

    /// @brief destructor
    ~WireLoadTable();

    /// @brief constructor
    WireLoadTable(Object *owner, IndexType id);

    /// @brief copy constructor
    WireLoadTable(WireLoadTable const &rhs);

    /// @brief move constructor
    WireLoadTable(WireLoadTable &&rhs) noexcept;

    /// @brief copy assignment
    WireLoadTable &operator=(WireLoadTable const &rhs);

    /// @brief move assignment
    WireLoadTable &operator=(WireLoadTable &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    void setName(const std::string &name);
    void setFanoutLength(int n, float f);
    void setFanoutCapacitance(int n, float f);
    void setFanoutResistance(int n, float f);
    void setFanoutArea(int n, float f);

    /// get
    std::string getName(void) const;
    SymbolIndex getNameIndex(void);
    FanoutPair getFanoutLength(void);
    FanoutPair getFanoutCapacitance(void);
    FanoutPair getFanoutResistance(void);
    FanoutPair getFanoutArea(void);

  protected:
    /// @brief copy object
    void copy(WireLoadTable const &rhs);
    /// @brief move object
    void move(WireLoadTable &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, WireLoadTable const &rhs);

  private:
    SymbolIndex name_;
    FanoutPair fanout_length_;
    FanoutPair fanout_capacitance_;
    FanoutPair fanout_resistance_;
    FanoutPair fanout_area_;
};

class WireLoad : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    WireLoad();

    /// @brief destructor
    ~WireLoad();

    /// @brief constructor
    WireLoad(Object *owner, IndexType id);

    /// @brief copy constructor
    WireLoad(WireLoad const &rhs);

    /// @brief move constructor
    WireLoad(WireLoad &&rhs) noexcept;

    /// @brief copy assignment
    WireLoad &operator=(WireLoad const &rhs);

    /// @brief move assignment
    WireLoad &operator=(WireLoad &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    void setName(const std::string &name);
    void setArea(float f);
    void setResistance(float f);
    void setCapacitance(float f);
    void setSlope(float f);
    void addFanoutLength(int n, float f);

    /// get
    std::string getName(void) const;
    SymbolIndex getNameIndex(void);
    float getArea(void);
    float getResistance(void);
    float getCapacitance(void);
    float getSlope(void);
    ArrayObject<FanoutPair> *getFanoutLengths(void);

  protected:
    /// @brief copy object
    void copy(WireLoad const &rhs);
    /// @brief move object
    void move(WireLoad &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, WireLoad const &rhs);

  private:
    SymbolIndex name_;
    float area_;
    float resistance_;
    float capacitance_;
    float slope_;
    ObjectId fanout_lengths_;
};

class WireLoadForArea : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    WireLoadForArea();

    /// @brief destructor
    ~WireLoadForArea();

    /// @brief constructor
    WireLoadForArea(Object *owner, IndexType id);

    /// @brief copy constructor
    WireLoadForArea(WireLoadForArea const &rhs);

    /// @brief move constructor
    WireLoadForArea(WireLoadForArea &&rhs) noexcept;

    /// @brief copy assignment
    WireLoadForArea &operator=(WireLoadForArea const &rhs);

    /// @brief move assignment
    WireLoadForArea &operator=(WireLoadForArea &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    void setMinArea(float f);
    void setMaxArea(float f);
    void setWireload(ObjectId id);

    /// get
    float getMinArea(void);
    float getMaxArea(void);
    WireLoad *getWireload(void);

  protected:
    /// @brief copy object
    void copy(WireLoadForArea const &rhs);
    /// @brief move object
    void move(WireLoadForArea &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, WireLoadForArea const &rhs);

  private:
    float min_area_;
    float max_area_;
    ObjectId wireload_;
};

class WireLoadSelection : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    WireLoadSelection();

    /// @brief destructor
    ~WireLoadSelection();

    /// @brief constructor
    WireLoadSelection(Object *owner, IndexType id);

    /// @brief copy constructor
    WireLoadSelection(WireLoadSelection const &rhs);

    /// @brief move constructor
    WireLoadSelection(WireLoadSelection &&rhs) noexcept;

    /// @brief copy assignment
    WireLoadSelection &operator=(WireLoadSelection const &rhs);

    /// @brief move assignment
    WireLoadSelection &operator=(WireLoadSelection &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    void setName(const std::string &name);
    void addWireLoadForArea(ObjectId id);

    /// get
    std::string getName(void) const;
    SymbolIndex getNameIndex(void);
    WireLoadForArea *getWireLoadForArea(ObjectId id);

  protected:
    /// @brief copy object
    void copy(WireLoadSelection const &rhs);
    /// @brief move object
    void move(WireLoadSelection &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os,
                                   WireLoadSelection const &rhs);

  private:
    SymbolIndex name_;
    ObjectId wireloads_;
};
}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_WIRELOAD_H_
