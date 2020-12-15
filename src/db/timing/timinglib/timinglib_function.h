/**
 * @file timinglib_function.h
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
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_FUNCTION_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_FUNCTION_H_

#include <algorithm>
#include <string>
#include <utility>

#include "db/core/object.h"
#include "db/timing/timinglib/timinglib_commondef.h"

namespace open_edi {
namespace db {

class TTerm;

class TFunction : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    TFunction();

    /// @brief destructor
    ~TFunction();

    /// @brief constructor
    TFunction(Object *owner, IndexType id);

    /// @brief copy constructor
    TFunction(TFunction const &rhs);

    /// @brief move constructor
    TFunction(TFunction &&rhs) noexcept;

    /// @brief copy assignment
    TFunction &operator=(TFunction const &rhs);

    /// @brief move assignment
    TFunction &operator=(TFunction &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    void setFuncStr(const std::string &str);
    void setTterm(ObjectId id);
    void setOp(FuncOpType t);
    void setLeft(ObjectId id);
    void setRight(ObjectId id);

    /// get
    TTerm *getTterm(void);
    FuncOpType getOp(void);
    TFunction *getLeft(void);
    TFunction *getRight(void);

    /// @brief output the information
    void print(std::ostream &stream);

  protected:
    /// @brief copy object
    void copy(TFunction const &rhs);
    /// @brief move object
    void move(TFunction &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TFunction const &rhs);

  private:
    SymbolIndex func_str_;
    FuncOpType op_;
    ObjectId tterm_;
    ObjectId left_;
    ObjectId right_;
};
}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_FUNCTION_H_
