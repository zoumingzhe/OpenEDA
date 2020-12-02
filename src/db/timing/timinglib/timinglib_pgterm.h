/**
 * @file timinglib_pgterm.h
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
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_PGTERM_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_PGTERM_H_

#include <algorithm>
#include <string>
#include <utility>

#include "db/core/object.h"
#include "db/timing/timinglib/timinglib_commondef.h"

namespace open_edi {
namespace db {

class TPgTerm : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    TPgTerm();

    /// @brief destructor
    ~TPgTerm();

    /// @brief constructor
    TPgTerm(Object *owner, IndexType id);

    /// @brief copy constructor
    TPgTerm(TPgTerm const &rhs);

    /// @brief move constructor
    TPgTerm(TPgTerm &&rhs) noexcept;

    /// @brief copy assignment
    TPgTerm &operator=(TPgTerm const &rhs);

    /// @brief move assignment
    TPgTerm &operator=(TPgTerm &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// @brief output the information
    void print(std::ostream &stream);

    /// set
    void setName(const std::string &name);
    void setPgType(PGType t);
    void setVoltageName(const std::string &name);

    /// get
    std::string getName(void) const;
    SymbolIndex getNameIndex(void);
    PGType getPgType(void);
    std::string getVoltageName(void) const;

  protected:
    /// @brief copy object
    void copy(TPgTerm const &rhs);
    /// @brief move object
    void move(TPgTerm &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TPgTerm const &rhs);

  private:
    SymbolIndex name_;
    PGType pg_type_;
    SymbolIndex voltage_name_;
};
}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_PGTERM_H_
