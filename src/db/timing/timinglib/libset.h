/**
 * @file libset.h
 * @date 2020-08-26
 * @brief LibSet Class
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_DB_TIMING_TIMINGLIB_LIBSET_H_
#define SRC_DB_TIMING_TIMINGLIB_LIBSET_H_

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "db/core/object.h"
#include "db/util/array.h"
#include "util/data_traits.h"

namespace open_edi {
namespace db {

class TLib;
class LibSet : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    LibSet();

    /// @brief destructor
    ~LibSet();

    /// @brief constructor
    LibSet(Object *owner, IndexType id);

    /// @brief copy constructor
    LibSet(LibSet const &rhs);

    /// @brief move constructor
    LibSet(LibSet &&rhs) noexcept;

    /// @brief copy assignment
    LibSet &operator=(LibSet const &rhs);

    /// @brief move assignment
    LibSet &operator=(LibSet &&rhs) noexcept;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// set
    void set_name(const std::string &name);

    /// @brief number of tlibs
    IndexType numTLibs() const;

    /// @brief add a instance tlib
    TLib *addTLib(const std::string &filename, const std::string &name);

    /// get
    SymbolIndex get_name_index(void);
    std::string get_name(void) const;
    std::vector<TLib *> get_timing_libs(void);

    /// @brief output the information
    void print(std::ostream &stream);

    void test(const std::string &filename);

  protected:
    /// @brief copy object
    void copy(LibSet const &rhs);
    /// @brief move object
    void move(LibSet &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, LibSet const &rhs);

  private:
    /// @brief add one tlib
    /// @return the added tlib
    TLib *__addTLibImpl(const std::string &filename);

    SymbolIndex name_;

    ObjectId timing_libs_;

    /// file path name, ObjectId
    std::unordered_map<SymbolIndex, ObjectId> timing_libs_map_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_LIBSET_H_
