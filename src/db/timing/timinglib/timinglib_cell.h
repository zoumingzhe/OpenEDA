/**
 * @file timinglib_cell.h
 * @date 2020-08-26
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
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_CELL_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_CELL_H_

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "db/core/attr_object.h"
#include "db/timing/timinglib/timinglib_cellattr.h"
#include "db/timing/timinglib/timinglib_commondef.h"

namespace open_edi {
namespace db {

class TTerm;
class TPgTerm;

class TCell : public AttrObject<TCellAttr> {
  public:
    using BaseType = AttrObject<TCellAttr>;

    /// @brief default constructor
    TCell();

    /// @brief destructor
    ~TCell();

    /// @brief constructor
    TCell(Object *owner, IndexType id);

    /// @brief copy constructor
    TCell(TCell const &rhs);

    /// @brief move constructor
    TCell(TCell &&rhs) noexcept;

    /// @brief copy assignment
    TCell &operator=(TCell const &rhs);

    /// @brief move constructor
    TCell &operator=(TCell &&rhs) noexcept;

    /// @brief number of terms
    IndexType numTerms() const;

    IndexType numPgTerms() const;

    /// @brief get or create a instance terminal
    TTerm *get_or_create_term(const std::string &name);
    TTerm *get_term(const std::string &name);
    std::vector<TTerm *> get_terms(void);
    void reset_terms(const std::vector<TTerm *> &terms);

    TPgTerm *get_or_create_pgTerm(const std::string &name);
    TPgTerm *get_pgTerm(const std::string &name);
    std::vector<TPgTerm *> get_pgTerms(void);

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    bool is_physical_only(void);
    bool is_combinational(void);
    bool has_multi_power_rails(void);

    void print(std::ostream &stream);

    std::string get_full_name(void);
    IndexType get_num_pins(void);

  protected:
    /// @brief copy object
    void copy(TCell const &rhs);
    /// @brief move object
    void move(TCell &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TCell const &rhs);

  private:
    /// @brief add one term
    /// @return the added term
    TTerm *__addTermImpl();
    TPgTerm *__addPgTermImpl();

    ObjectId tterms_;
    ObjectId tpg_terms_;
    std::unordered_map<SymbolIndex, ObjectId> term_map_;
    std::unordered_map<SymbolIndex, ObjectId> pg_term_map_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_CELL_H_
