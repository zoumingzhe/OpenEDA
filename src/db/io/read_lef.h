/* @file  read_lef.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_DB_IO_READ_LEF_H_
#define SRC_DB_IO_READ_LEF_H_

#include "db/core/cell.h"
#include "db/tech/export_tech.h"
#include "db/tech/layer.h"
#include "db/tech/max_via_stack.h"
#include "db/tech/ndr_rule.h"
#include "db/tech/site.h"
#include "db/tech/tech.h"
#include "db/tech/via_master.h"
#include "db/tech/via_rule.h"
#include "parser/lef/lef/lefiDebug.hpp"
#include "parser/lef/lef/lefiEncryptInt.hpp"
#include "parser/lef/lef/lefiUtil.hpp"
#include "parser/lef/lef/lefrReader.hpp"
#include "parser/lef/lef/lefwWriter.hpp"

namespace open_edi {
namespace db {

// add tech features
int readLef(int argc, const char** argv);

// LAYER
int readLayer(lefiLayer* io_layer);
int readLayerRule(lefiLayer* io_layer, Layer* edi_layer);
// int setMetalLayerRule(lefiLayer* io_layer, Layer *edi_layer);
// int setCutLayerRule(lefiLayer* io_layer, Layer *edi_layer);
// int setTrimLayerRule(lefiLayer* io_layer, Layer *edi_layer);

// SITE
int readSite(lefiSite* io_site);

// MACRO

// NDR
int readNonDefaultRule(lefiNonDefault* io_ndr_rule);
// VIA/VIARULE
int readViaMaster(lefiVia* io_via, bool is_from_ndr = false);
int readViaRule(lefiViaRule* io_via);
// UNITS
int readUnit(lefiUnits* io_units);
// PROPERTY DEFINITION
int readPropertyDefinition(lefiProp* io_prop);

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_IO_READ_LEF_H_
