/**
 * @file timinglib_pvt.h
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
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_PVT_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_PVT_H_

#include <algorithm>
#include <utility>

#include "db/core/object.h"
#include "db/timing/timinglib/timinglib_commondef.h"

namespace open_edi {
namespace db {

class TPvt : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    TPvt();

    /// @brief destructor
    ~TPvt();

    /// @brief constructor
    TPvt(Object *owner, IndexType id);

    /// @brief copy constructor
    TPvt(TPvt const &rhs);

    /// @brief move constructor
    TPvt(TPvt &&rhs) noexcept;

    /// @brief copy assignment
    TPvt &operator=(TPvt const &rhs);

    /// @brief move assignment
    TPvt &operator=(TPvt &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    void set_process(float c);
    void set_temperature(float t);
    void set_voltage(float v);

    /// get
    float get_process(void);
    float get_temperature(void);
    float get_voltage(void);

    /// @brief output the information
    void print(std::ostream &stream);

  protected:
    /// @brief copy object
    void copy(TPvt const &rhs);
    /// @brief move object
    void move(TPvt &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TPvt const &rhs);

  private:
    float process_;
    float temperature_;
    float voltage_;
};
}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_PVT_H_
