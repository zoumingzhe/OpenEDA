/* @file  inst.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_DB_DESIGN_INST_ATTR_H_
#define EDI_DB_DESIGN_INST_ATTR_H_

#include "db/core/object_attr.h"
#include "util/point.h"

namespace open_edi {
namespace db {

/// @brief Attributes of instance.
class InstAttr : public ObjectAttr {
  public:
    using BaseType = ObjectAttr;

    /// @brief default constructor
    InstAttr();

    /// @brief copy constructor
    InstAttr(InstAttr const &rhs);

    /// @brief move constructor
    InstAttr(InstAttr &&rhs) noexcept;

    /// @brief copy assignment
    InstAttr &operator=(InstAttr const &rhs);

    /// @brief move constructor
    InstAttr &operator=(InstAttr &&rhs) noexcept;

    /// @brief getter for name
    std::string const &getName() const { return name_; }

    /// @brief setter for name
    void setName(std::string const &v) { name_ = v; }

    /// @brief getter for location of the instance
    Point const &getLoc() const { return loc_; }

    /// @brief setter for location of the instance
    void setLoc(Point const &v) { loc_ = v; }

    /// @brief clone current object for extensibility.
    /// Users need to manage the pointer to avoid memory leakage.
    /// @return the pointer to the new object
    InstAttr *clone() const;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

  protected:
    /// @brief copy object
    void copy(InstAttr const &rhs);
    /// @brief move object
    void move(InstAttr &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, InstAttr const &rhs);
    /// @brief overload input stream
    friend IStreamBase &operator>>(IStreamBase &is, InstAttr &rhs);

    std::string name_;  ///< instance name
    Point loc_;         ///< lower left location of this instance
};

}  // namespace db
}  // namespace open_edi

#endif
