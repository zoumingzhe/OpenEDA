/* @file  scan_chain.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_DB_PHYSICAL_SCAN_CHAIN_H_
#define EDI_DB_PHYSICAL_SCAN_CHAIN_H_

#include <string>
#include <vector>

#include "db/core/object.h"
#include "db/tech/type_def.h"

namespace open_edi {
namespace db {

/*
 * [SCANCHAINS  numScanChain ;
 *     [-  chainName
 *         [+ PARTITION  partitionName  [MAXBITS  maxbits ]]
 *         [+ COMMONSCANPINS [ ( IN  pin )] [( OUT  pin ) ] ]
 *         + START { fixedInComp  | PIN} [ outPin ]
 *         [+ FLOATING
 *             { floatingComp  [ ( IN  pin ) ] [ ( OUT  pin ) ] [ ( BITS numBits
 * ) ]} ...]
 *         [+ ORDERED
 *             { fixedComp  [ ( IN  pin ) ] [ ( OUT pin ) ] [ ( BITS  numBits  )
 * ]} ... ] ...
 *         + STOP { fixedOutComp  | PIN} [ inPin ] ]
 *     ;] ...
 * END SCANCHAINS]
 */

// each item has multiple inst/inPin/outPin/numBits if defined
class ScanChainListItem {
  public:
    ScanChainListItem();
    ~ScanChainListItem();

    bool getHasInPin() const;
    void setHasInPin(bool flag);
    bool getHasOutPin() const;
    void setHasOutPin(bool flag);
    bool getHasBits() const;
    void setHasBits(bool flag);
    ObjectId getInstId() const;
    void setInstId(ObjectId id);
    ObjectId getInPinId() const;
    void setInPinId(ObjectId id);
    ObjectId getOutPinId() const;
    void setOutPinId(ObjectId id);
    ObjectId getNumBits() const;
    void setNumBits(ObjectId num_bits);
    ScanChainListItem *getNext() const;
    void setNext(ScanChainListItem *next);

  private:
    Bits has_in_pin_ : 1;
    Bits has_out_pin_ : 1;
    Bits has_bits_ : 1;
    Bits unused_ : 29;
    ObjectId inst_id_;
    ObjectId in_pin_id_;
    ObjectId out_pin_id_;
    ObjectId num_bits_;
    ScanChainListItem *next_;
};

// for FLOATING and ORDERED list of ScanChain
// FLOATING has only one item in list
// ORDERED has multiple items in the list
class ScanChainList {
  public:
    ScanChainList();
    ~ScanChainList();

    ScanChainListItem *getItems() const;
    void setItems(ScanChainListItem *items);
    ScanChainList *getNext() const;
    void setNext(ScanChainList *next);

  private:
    ScanChainListItem *items_;
    ScanChainList *next_;
};

// for START and STOP point of ScanChain
class ScanChainPoint {
  public:
    ScanChainPoint();
    ~ScanChainPoint();

    bool getIsIOPin() const;
    void setIsIOPin(bool flag);
    bool getHasPin() const;
    void setHasPin(bool flag);
    ObjectId getInstId() const;
    void setInstId(ObjectId id);
    ObjectId getPinId() const;
    void setPinId(ObjectId id);

  private:
    // { fixedInComp/fixedOutComp  | PIN}, here PIN is io pin
    // if it is io pin, search pin from io pin list instead of from inst pin
    // list
    Bits is_io_pin_ : 1;
    Bits has_pin_ : 1;
    Bits unused_ : 31;

    ObjectId inst_id_;
    ObjectId pin_id_;
};

class ScanChain : public Object {
  public:
    ScanChain();
    ~ScanChain();

    bool getHasPartition() const;
    void setHasPartition(bool flag);
    bool getHasCommonScanPins() const;
    void setHasCommonScanPins(bool flag);
    bool getHasCommonInPin() const;
    void setHasCommonInPin(bool flag);
    bool getHasCommonOutPin() const;
    void setHasCommonOutPin(bool flag);
    bool getHasMaxBits() const;
    void setHasMaxBits(bool flag);
    const char *getChainName() const;
    void setChainName(const char *chain_name);
    const char *getPartitionName() const;
    void setPartitionName(const char *partition_name);
    const char *getCommonInPin() const;
    void setCommonInPin(const char *in_pin_name);
    const char *getCommonOutPin() const;
    void setCommonOutPin(const char *out_pin_name);
    ObjectId getMaxBits() const;
    void setMaxBits(ObjectId max_bits);
    ScanChainList *getFloating() const;
    void setFloating(ScanChainList *floating);
    ScanChainList *getOrdered() const;
    void setOrdered(ScanChainList *ordered);
    void setStart(ScanChainPoint *start);
    ScanChainPoint *getStart() const;
    void setStop(ScanChainPoint *stop);
    ScanChainPoint *getStop() const;

  private:
    Bits has_partition_ : 1;
    Bits has_common_scan_pins_ : 1;
    Bits has_common_in_pin_ : 1;
    Bits has_common_out_pin_ : 1;
    Bits has_max_bits_ : 1;
    Bits unused_ : 27;
    SymbolIndex name_index_;
    SymbolIndex partition_index_;
    SymbolIndex common_in_pin_index_;
    SymbolIndex common_out_pin_index_;
    ObjectId max_bits_;
    ScanChainList *floating_;  // next = 0 if there is floating definition
    ScanChainList *ordered_;   // multiple ordered if ORDERED defined
    ScanChainPoint *start_;    // must have
    ScanChainPoint *stop_;     // must have
};

}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_PHYSICAL_SCAN_CHAIN_H_
