/**
 * @file timinglib_lib.h
 * @date 2020-08-26
 * @brief TLib class
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIB_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIB_H_

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "db/core/attr_object.h"
#include "db/timing/timinglib/timinglib_commondef.h"
#include "db/timing/timinglib/timinglib_libattr.h"
#include "db/util/array.h"

namespace open_edi {
namespace db {

class OperatingConditions;
class TCell;
class WireLoad;
class WireLoadTable;
class WireLoadSelection;
class TableTemplate;
class TUnits;
class ScaleFactors;

class TLib : public AttrObject<TLibAttr> {
  public:
    using BaseType = AttrObject<TLibAttr>;

    /// @brief default constructor
    TLib();

    /// @brief destructor
    ~TLib();

    /// @brief constructor
    TLib(Object *owner, IndexType id);

    /// @brief copy constructor
    TLib(TLib const &rhs);

    /// @brief move constructor
    TLib(TLib &&rhs) noexcept;

    /// @brief copy assignment
    TLib &operator=(TLib const &rhs);

    /// @brief move constructor
    TLib &operator=(TLib &&rhs) noexcept;

    /// @brief number of cells
    IndexType numTCells() const;

    /// @brief number of cells
    IndexType numOperatingConditions() const;

    /// @brief add a instance tcell
    TCell *add_timing_cell(const std::string &name);

    /// @brief add a instance operatingconditions
    OperatingConditions *add_operating_conditions(const std::string &name);
    WireLoad *add_wire_load(const std::string &name);
    WireLoadTable *add_wire_load_table(const std::string &name);
    WireLoadSelection *add_wire_load_selection(const std::string &name);
    TableTemplate *add_table_template(const std::string &name);

    void set_default_operating_conditions(ObjectId id);
    ScaleFactors *get_or_create_scaling_factors(const std::string &name);
    TUnits *get_or_create_units();
    void set_default_wire_load(ObjectId id);
    void set_default_wire_load_selection(ObjectId id);

    OperatingConditions *get_default_operating_conditions(void) const;
    ScaleFactors *get_scaling_factors(void);
    TUnits *get_units(void);
    WireLoad *get_default_wire_load(void) const;
    WireLoadSelection *get_default_wire_load_selection(void) const;
    OperatingConditions *get_operating_conditions(
        const std::string &name) const;
    WireLoad *get_wire_load(const std::string &name) const;
    WireLoadTable *get_wire_load_table(const std::string &name);
    WireLoadSelection *get_wire_load_selection(const std::string &name) const;
    TableTemplate *get_table_template(const std::string &name);
    TCell *get_timing_cell(const std::string &name);
    std::vector<TCell *> get_timing_cells(void);

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

    /// @brief output the information
    void print(std::ostream &stream);

  protected:
    /// @brief copy object
    void copy(TLib const &rhs);
    /// @brief move object
    void move(TLib &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, TLib const &rhs);

  private:
    /// @brief add one tcell
    /// @return the added tcell
    TCell *__addTCellImpl();

    /// @brief add one operatingconditions
    /// @return the added operatingconditions
    OperatingConditions *__addOperatingConditionsImpl();

    ObjectId default_operating_conditions_;
    ObjectId scaling_factors_;
    ObjectId units_;
    ObjectId default_wire_load_;
    ObjectId default_wire_load_selection_;
    ObjectId operating_conditions_;
    ObjectId wire_loads_;
    ObjectId wire_load_tables_;
    ObjectId wire_load_selections_;
    ObjectId table_templates_;
    ObjectId timing_cells_;
    std::unordered_map<SymbolIndex, ObjectId> operating_conditions_map_;
    std::unordered_map<SymbolIndex, ObjectId> wire_loads_map_;
    std::unordered_map<SymbolIndex, ObjectId> wire_load_tables_map_;
    std::unordered_map<SymbolIndex, ObjectId> wire_load_selections_map_;
    std::unordered_map<SymbolIndex, ObjectId> table_templates_map_;
    std::unordered_map<SymbolIndex, ObjectId> timing_cells_map_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIB_H_
