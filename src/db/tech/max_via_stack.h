/* @file  max_via_stack.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_DB_TECH_MAX_VIA_STACK_H_
#define EDI_DB_TECH_MAX_VIA_STACK_H_

#include <string>

#include "db/core/object.h"
#include "db/tech/layer.h"

namespace open_edi {
namespace db {

/**
 * @class MaxViaStack
 * @brief
 * implementation of MAXVIASTACK rules
 */
class MaxViaStack : public Object {
  public:
    MaxViaStack();
    ~MaxViaStack();
    UInt32 getNumStackedVia() const;
    void setNumStackedVia(UInt32 num);
    bool getIsRange() const;
    void setIsRange(bool is_range);
    Int32 getTopLayerId() const;
    void setTopLayerId(Int32 num);
    Int32 getBotLayerId() const;
    void setBotLayerId(Int32 num);

  private:
    // first 32 bits
    Bits is_range_ : 1;
    Bits unused_ : 31;

    UInt32 num_stacked_via_;
    Int32 top_layer_id_;
    Int32 bot_layer_id_;
};

}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_TECH_MAX_VIA_STACK_H_
