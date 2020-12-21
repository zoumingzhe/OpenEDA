/* @file  read_def.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_DB_IO_READ_DEF_H_
#define SRC_DB_IO_READ_DEF_H_

#include "db/core/fill.h"
#include "db/core/fplan.h"
#include "db/core/group.h"
#include "db/core/inst.h"
#include "db/core/net.h"
#include "db/core/scan_chain.h"
#include "db/core/special_net.h"
#include "db/core/via.h"
#include "db/core/wire.h"
#include "db/tech/tech.h"
#include "db/tech/via_master.h"
#include "db/util/property_definition.h"
#include "parser/def/def/defiAlias.hpp"
#include "parser/def/def/defrReader.hpp"

namespace open_edi {
namespace db {

// command api
int readDef(int argc, const char** argv);
// Die Area
int readDieArea(defiBox* io_die_area);

// Net
int readNet(defiNet* io_net);
int readSpecialNet(defiNet* io_net);

// Special Wire
int readSpecialWire(defiWire* io_wire, SpecialNet* net);
int readRectSpecialWire(defiNet* net);

// Rows
int readRow(defiRow* io_row);

// Tracks
int readTrack(defiTrack* io_track);

// GcellGrid
int readGcellGrid(defiGcellGrid* io_gcell_grid);

// Blockages
int readBlockage(defiBlockage* io_blockage);

// Instances

// Component mask shift
int readCompMaskShift(defiComponentMaskShiftLayer* co);

// Components
int readComp(defiComponent* co);

// Design

// Fills
int readFill(defiFill* io_fill);

// Pins
int readPin(defiPin* def_pin);

// Regions
int readRegion(defiRegion* re);

// ScanChains
int readScanChain(defiScanchain* sc);
int readScanChainPoint(defiScanchain* sc, ScanChain* scan_chain, bool is_start);
int readScanChainList(defiScanchain* sc, ScanChain* scan_chain,
                      bool is_floating, int idx = 0);

// Vias
int readVia(defiVia* io_via);

// Units
int readUnit(double dbu);

// Groups
int readGroup(defiGroup* io_group);
// Property Definition
int readPropertyDefinition(defiProp* io_prop);

// Non-Default Rule
int readNonDefaultRule(defiNonDefault* io_ndr_rule);
// ...

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_IO_READ_DEF_H_
