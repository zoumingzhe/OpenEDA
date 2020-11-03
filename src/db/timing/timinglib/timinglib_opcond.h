/**
 * @file timinglib_opcond.h
 * @date 2020-08-26
 * @brief OperatingConditions Class
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_OPCOND_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_OPCOND_H_

#include <algorithm>
#include <string>
#include <utility>

#include "db/core/object.h"
#include "db/timing/timinglib/timinglib_commondef.h"
#include "db/timing/timinglib/timinglib_pvt.h"

namespace open_edi {
namespace db {

class OperatingConditions : public TPvt {
  public:
    using BaseType = TPvt;

    /// @brief default constructor
    OperatingConditions();

    /// @brief destructor
    ~OperatingConditions();

    /// @brief constructor
    OperatingConditions(Object *owner, IndexType id);

    /// @brief copy constructor
    OperatingConditions(OperatingConditions const &rhs);

    /// @brief move constructor
    OperatingConditions(OperatingConditions &&rhs) noexcept;

    /// @brief copy assignment
    OperatingConditions &operator=(OperatingConditions const &rhs);

    /// @brief move assignment
    OperatingConditions &operator=(OperatingConditions &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    void set_name(const std::string &name);
    void set_wire_load_tree(WireLoadTree ty);

    /// get
    std::string get_name(void) const;
    SymbolIndex get_name_index(void);
    WireLoadTree get_wire_load_tree(void);

    /// @brief output the information
    void print(std::ostream &stream);

  protected:
    /// @brief copy object
    void copy(OperatingConditions const &rhs);
    /// @brief move object
    void move(OperatingConditions &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os,
                                   OperatingConditions const &rhs);
  private:
    SymbolIndex name_;
    WireLoadTree wire_load_tree_;
};
}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_OPCOND_H_
