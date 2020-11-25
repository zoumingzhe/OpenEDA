/* @file  bus.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/core/db.h"
#include "db/core/bus.h"

namespace open_edi {
namespace db {

/**
 * @brief Construct a new Bus:: Bus object
 *
 */
Bus::Bus() {
    name_index_ = -1;
    range_left_ = 0;
    range_right_ = 0;
    is_tri_ = false;
}
Bus::~Bus() {
}

bool Bus::setName(std::string const& name) {
    SymbolIndex index = getTopCell()->getOrCreateSymbol(name.c_str());
    if (index == kInvalidSymbolIndex) return false;

    name_index_ = index;
    getTopCell()->addSymbolReference(name_index_, this->getId());
    return true;
}

void Bus::setRangeLeft(int32_t left) {
    range_left_ = left;
}

void Bus::setRangeRight(int32_t right) {
    range_right_ = right;
}

void Bus::setIsTri(bool is_tri) {
    is_tri_ = is_tri;
}

SymbolIndex Bus::getNameIndex() const {
    return name_index_;
}

std::string const& Bus::getName() {
    return getTopCell()->getSymbolByIndex(name_index_);
}

int32_t Bus::getRangeLeft() {
    return range_left_;
}

int32_t Bus::getRangeRight() {
    return range_right_;
}

bool Bus::getIsTri() {
    return is_tri_;
}
}  // namespace db
}  // namespace open_edi
