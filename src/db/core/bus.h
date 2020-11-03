/* @file  bus.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_DB_PHYSICAL_BUS_H_
#define EDI_DB_PHYSICAL_BUS_H_

#include "db/core/object.h"

namespace open_edi {
namespace db {

class Bus : public Object {
  public:
    Bus();
    ~Bus();

    bool setName(std::string const& name);
    void setRangeLeft(int32_t left);
    void setRangeRight(int32_t right);
    void setIsTri(bool is_tri);

    SymbolIndex getNameIndex() const;
    std::string const& getName();
    int32_t getRangeLeft();
    int32_t getRangeRight();
    bool getIsTri();

  private:
    SymbolIndex name_index_; /**< bus name id */
    int32_t range_left_;
    int32_t range_right_;
    bool     is_tri_   :1;
    int32_t  reserved_ :31;
};

}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_PHYSICAL_BUS_H_
