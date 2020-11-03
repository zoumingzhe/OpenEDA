/**
 * @file timinglib_term.h
 * @date 2020-08-26
 * @brief TTerm Class
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TERM_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TERM_H_

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>

#include "db/core/attr_object.h"
#include "db/timing/timinglib/timinglib_commondef.h"
#include "db/timing/timinglib/timinglib_termattr.h"
#include "db/util/array.h"

namespace open_edi {
namespace db {

class TFunction;
class TPgTerm;
class TimingArc;

class TTerm : public AttrObject<TTermAttr> {
  public:
    using BaseType = AttrObject<TTermAttr>;

    /// @brief default constructor
    TTerm();

    /// @brief destructor
    ~TTerm();

    /// @brief constructor
    TTerm(Object *owner, IndexType id);

    /// @brief copy constructor
    TTerm(TTerm const &rhs);

    /// @brief move constructor
    TTerm(TTerm &&rhs) noexcept;

    /// @brief copy assignment
    TTerm &operator=(TTerm const &rhs);

    /// @brief move constructor
    TTerm &operator=(TTerm &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    void set_related_power_pin(ObjectId id);
    void set_related_ground_pin(ObjectId id);
    TFunction *set_function(const std::string &str);
    TFunction *get_function(void);
    void add_member_pin(ObjectId id);
    void add_timingarc(ObjectId id);
    TimingArc *get_timingarc(ObjectId id);
    TPgTerm *get_related_power_pin(void) const;
    TPgTerm *get_related_ground_pin(void) const;

    /// @brief output the information
    void print(std::ostream &stream);

  protected:
    /// @brief copy object
    void copy(TTerm const &rhs);
    /// @brief move object
    void move(TTerm &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TTerm const &rhs);

  private:
    ObjectId related_power_pin_;
    ObjectId related_ground_pin_;
    ObjectId function_;
    ObjectId member_pins_;
    ObjectId timing_arcs_;
    std::unordered_map<SymbolIndex, ObjectId> member_pins_map_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TERM_H_
