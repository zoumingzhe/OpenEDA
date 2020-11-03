/* @file  object_attr.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_DB_OBJECT_ATTR_H_
#define EDI_DB_OBJECT_ATTR_H_

#include "db/core/object.h"
#include "util/util.h"

namespace open_edi {
namespace db {

/// @brief Base class for all attributes.
class ObjectAttr {
  public:
    using IndexType = Object::IndexType;
    using CoordinateType = Object::CoordinateType;

    /// @brief default constructor
    ObjectAttr() = default;

    /// @brief destructor
    virtual ~ObjectAttr() = default;

    /// @brief summarize memory usage of the object in bytes
    virtual IndexType memory() const { return 0; }
};

}  // namespace db
}  // namespace open_edi

#endif
