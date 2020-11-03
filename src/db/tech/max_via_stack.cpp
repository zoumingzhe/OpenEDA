/* @file  max_via_stack.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/tech/max_via_stack.h"

namespace open_edi {
namespace db {

/**
 * @brief
 * constructor of MaxViaStack
 */
MaxViaStack::MaxViaStack() {
    num_stacked_via_ = 0;
    is_range_ = false;
    top_layer_id_ = -1;
    bot_layer_id_ = -1;
}

/**
 * @brief
 * destructor of MaxViaStack
 */
MaxViaStack::~MaxViaStack() {}

/**
 * @brief getNumStackedVia
 * get number of stacked vias
 *
 * @return
 */
UInt32 MaxViaStack::getNumStackedVia() const { return num_stacked_via_; }

/**
 * @brief setNumStackedVia
 * set number of stacked vias
 *
 * @param num
 */
void MaxViaStack::setNumStackedVia(UInt32 num) { num_stacked_via_ = num; }

/**
 * @brief getIsRange
 * get RANGE definition in MAXVIASTACK
 *
 * @return
 */
bool MaxViaStack::getIsRange() const { return is_range_; }

/**
 * @brief setIsRange
 * set RANGE definition in MAXVIASTACK
 *
 * @param is_range
 */
void MaxViaStack::setIsRange(bool is_range) { is_range_ = is_range; }

/**
 * @brief getTopLayerId
 * get top layer id
 *
 * @return
 */
Int32 MaxViaStack::getTopLayerId() const { return top_layer_id_; }

/**
 * @brief setTopLayerId
 * set top layer id
 *
 * @param id
 */
void MaxViaStack::setTopLayerId(Int32 id) { top_layer_id_ = id; }

/**
 * @brief getBotLayerId
 * get bottom layer id
 *
 * @return
 */
Int32 MaxViaStack::getBotLayerId() const { return bot_layer_id_; }

/**
 * @brief setBotLayerId
 * set bottom layer id
 *
 * @param id
 */
void MaxViaStack::setBotLayerId(Int32 id) { bot_layer_id_ = id; }

}  // namespace db
}  // namespace open_edi
