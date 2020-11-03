/* @file  scan_chain.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/core/scan_chain.h"

#include "db/core/db.h"

namespace open_edi {
namespace db {

ScanChainListItem::ScanChainListItem() {
    has_in_pin_ = false;
    has_out_pin_ = false;
    has_bits_ = false;
    inst_id_ = 0;
    in_pin_id_ = 0;
    out_pin_id_ = 0;
    num_bits_ = 0;
    next_ = 0;
}

ScanChainListItem::~ScanChainListItem() {}

bool ScanChainListItem::getHasInPin() const { return has_in_pin_; }

void ScanChainListItem::setHasInPin(bool flag) { has_in_pin_ = flag; }

bool ScanChainListItem::getHasOutPin() const { return has_out_pin_; }

void ScanChainListItem::setHasOutPin(bool flag) { has_out_pin_ = flag; }

bool ScanChainListItem::getHasBits() const { return has_bits_; }

void ScanChainListItem::setHasBits(bool flag) { has_bits_ = flag; }

ObjectId ScanChainListItem::getInstId() const { return inst_id_; }

void ScanChainListItem::setInstId(ObjectId id) { inst_id_ = id; }

ObjectId ScanChainListItem::getInPinId() const { return in_pin_id_; }

void ScanChainListItem::setInPinId(ObjectId id) { in_pin_id_ = id; }

ObjectId ScanChainListItem::getOutPinId() const { return out_pin_id_; }

void ScanChainListItem::setOutPinId(ObjectId id) { out_pin_id_ = id; }

ObjectId ScanChainListItem::getNumBits() const { return num_bits_; }

void ScanChainListItem::setNumBits(ObjectId num_bits) { num_bits_ = num_bits; }

ScanChainListItem *ScanChainListItem::getNext() const { return next_; }

void ScanChainListItem::setNext(ScanChainListItem *next) { next_ = next; }

ScanChainList::ScanChainList() {
    items_ = 0;
    next_ = 0;
}

ScanChainList::~ScanChainList() {}

ScanChainListItem *ScanChainList::getItems() const { return items_; }

void ScanChainList::setItems(ScanChainListItem *items) { items_ = items; }

ScanChainList *ScanChainList::getNext() const { return next_; }

void ScanChainList::setNext(ScanChainList *next) { next_ = next; }

ScanChainPoint::ScanChainPoint() {
    is_io_pin_ = false;
    has_pin_ = false;
    inst_id_ = 0;
    pin_id_ = 0;
}

ScanChainPoint::~ScanChainPoint() {}

bool ScanChainPoint::getIsIOPin() const { return is_io_pin_; }

void ScanChainPoint::setIsIOPin(bool flag) { is_io_pin_ = flag; }

bool ScanChainPoint::getHasPin() const { return has_pin_; }

void ScanChainPoint::setHasPin(bool flag) { has_pin_ = flag; }

ObjectId ScanChainPoint::getInstId() const { return inst_id_; }

void ScanChainPoint::setInstId(ObjectId id) { inst_id_ = id; }

ObjectId ScanChainPoint::getPinId() const { return pin_id_; }

void ScanChainPoint::setPinId(ObjectId id) { pin_id_ = id; }

ScanChain::ScanChain() {
    has_partition_ = false;
    has_common_scan_pins_ = false;
    has_common_in_pin_ = false;
    has_common_out_pin_ = false;
    max_bits_ = 0;
    floating_ = 0;
    ordered_ = 0;
    start_ = 0;
    stop_ = 0;
}

ScanChain::~ScanChain() {}

bool ScanChain::getHasPartition() const { return has_partition_; }

void ScanChain::setHasPartition(bool flag) { has_partition_ = flag; }

bool ScanChain::getHasCommonScanPins() const { return has_common_scan_pins_; }

void ScanChain::setHasCommonScanPins(bool flag) {
    has_common_scan_pins_ = flag;
}

bool ScanChain::getHasCommonInPin() const { return has_common_in_pin_; }

void ScanChain::setHasCommonInPin(bool flag) { has_common_in_pin_ = flag; }

bool ScanChain::getHasCommonOutPin() const { return has_common_out_pin_; }

void ScanChain::setHasCommonOutPin(bool flag) { has_common_out_pin_ = flag; }

bool ScanChain::getHasMaxBits() const { return has_max_bits_; }

void ScanChain::setHasMaxBits(bool flag) { has_max_bits_ = flag; }

const char *ScanChain::getChainName() const {
    return getTopCell()
        ->getSymbolTable()
        ->getSymbolByIndex(name_index_)
        .c_str();
}

void ScanChain::setChainName(const char *chain_name) {
    name_index_ = getTopCell()->getOrCreateSymbol(chain_name);
}

const char *ScanChain::getPartitionName() const {
    return getTopCell()
        ->getSymbolTable()
        ->getSymbolByIndex(partition_index_)
        .c_str();
}

void ScanChain::setPartitionName(const char *partition_name) {
    partition_index_ = getTopCell()->getOrCreateSymbol(partition_name);
}

const char *ScanChain::getCommonInPin() const {
    return getTopCell()
        ->getSymbolTable()
        ->getSymbolByIndex(common_in_pin_index_)
        .c_str();
}

void ScanChain::setCommonInPin(const char *in_pin_name) {
    common_in_pin_index_ = getTopCell()->getOrCreateSymbol(in_pin_name);
}

const char *ScanChain::getCommonOutPin() const {
    return getTopCell()
        ->getSymbolTable()
        ->getSymbolByIndex(common_out_pin_index_)
        .c_str();
}

void ScanChain::setCommonOutPin(const char *out_pin_name) {
    common_out_pin_index_ = getTopCell()->getOrCreateSymbol(out_pin_name);
}

ObjectId ScanChain::getMaxBits() const { return max_bits_; }

void ScanChain::setMaxBits(ObjectId max_bits) { max_bits_ = max_bits; }

ScanChainList *ScanChain::getFloating() const { return floating_; }

void ScanChain::setFloating(ScanChainList *floating) { floating_ = floating; }

ScanChainList *ScanChain::getOrdered() const { return ordered_; }

void ScanChain::setOrdered(ScanChainList *ordered) { ordered_ = ordered; }

ScanChainPoint *ScanChain::getStart() const { return start_; }

void ScanChain::setStart(ScanChainPoint *start) { start_ = start; }

ScanChainPoint *ScanChain::getStop() const { return stop_; }

void ScanChain::setStop(ScanChainPoint *stop) { stop_ = stop; }

}  // namespace db
}  // namespace open_edi
