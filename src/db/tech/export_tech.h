/* @file  export_tech.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_DB_TECH_EXPORT_TECH_H_
#define EDI_DB_TECH_EXPORT_TECH_H_

#include <fstream>
#include <vector>

#include "db/core/cell.h"
#include "db/core/object.h"
#include "db/tech/max_via_stack.h"
#include "db/tech/ndr_rule.h"
#include "db/tech/site.h"
#include "db/tech/tech.h"
#include "db/tech/units.h"
#include "db/tech/via_rule.h"
#include "db/util/property_definition.h"
#include "util/util.h"

namespace open_edi {
namespace db {

class Tech;
class ExportTechLef {
  public:
    ExportTechLef(const char* tech_name = "debug_tech.lef");
    ~ExportTechLef();

  public:
    // all tech statements
    void exportAll();

    // LEF order
    void exportVersion();
    void exportNameCaseSensitive();
    void exportBusBit();
    void exportDividerChar();
    void exportUnits();
    void exportMGrid();
    void exportUseMinSpacing();
    void exportClearanceMeasure();
    void exportAlias();
    void exportPropertyDefinitions();
    void exportFixedMask();
    void exportMaxViaStack();
    void exportSite();
    void exportAllLayers();
    void exportViaMaster();
    void exportViaRule();
    void exportNDR();
    void exportCells();
    void exportExtensions();

    // LAYER
    void exportLayer(Layer* layer);
    void exportLayerProperty(const Layer* layer);
    void exportLayerAntennaModel(const Layer* layer);
    void exportLayerCurrentDensity(const Layer* layer);
    void exportLayerCurrentDensityACItem(const Layer* layer, CurrentDen* den);
    void exportLayerCurrentDensityDCItem(const Layer* layer, CurrentDen* den);
    // Implant layer
    void exportImplantLayer(Layer* layer);
    // Masterslice layer
    void exportMastersliceLayer(Layer* layer);
    // Overlap layer
    void exportOverlapLayer(Layer* layer);
    // Routing layer rules
    void exportRoutingLayer(Layer* layer);
    void exportRoutingSpacing(RoutingLayerRule* r);
    void exportMinArea(const Layer* layer);
    void exportMinSize(const Layer* layer);
    void exportMinEnclosedArea(const RoutingLayerRule* layer);
    void exportMinStep(const RoutingLayerRule* layer);
    void exportProtrusionWidth(const RoutingLayerRule* layer);
    // Cut layer rules
    void exportCutLayer(Layer* layer);
    void exportSpacingTable(const RoutingLayerRule* r);
    void exportMinCut(const RoutingLayerRule* r);
    void exportCutSpacing(const CutLayerRule* cut_rule);
    void exportEnclosure(const CutLayerRule* cut_rule);
    void exportPreferEnclosure(const CutLayerRule* cut_rule);
    void exportArraySpacing(const CutLayerRule* cut_rule);

    //
    void exportCell(Cell* cell);

  private:
    UInt32 getTechPrecision();
    UInt32 getCurPrecision();
    UInt32 setPrecision(UInt32 precision);
    UInt32 resetPrecision();

  private:
    Tech* tech_lib_;
    std::ofstream ofs_;
    UInt32 default_precision_;
    UInt32 tech_precision_;
    UInt32 cur_precision_;
};

}  // namespace db
}  // namespace open_edi

#endif /* ----- #ifndef EDI_DB_TECH_EXPORT_TECH_H_  ----- */
