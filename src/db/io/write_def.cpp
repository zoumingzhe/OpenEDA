/* @file  write_def.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "db/io/write_def.h"

#include <gperftools/profiler.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <vector>

#include "db/core/cell.h"
#include "db/core/db.h"
#include "db/util/property_definition.h"
#include "db/util/array.h"
#include "db/util/vector_object_var.h"
#include "util/util.h"

namespace open_edi {
namespace db {

#define OK (0)
#define ERROR (1)

static Cell *top_cell;

static FILE *getDefFilePointer(const char *file_name);
static bool writeFileHead(FILE *fp);
static bool writeVersion(FILE *fp);
static bool writeDividerChar(FILE *fp);
static bool writeBusbitChars(FILE *fp);
static bool writeDesign(FILE *fp);
static bool writeTechnology(FILE *fp);
static bool writeUnits(FILE *fp);
static bool writeHistory(FILE *fp);
static bool writePropertyDefinitions(FILE *fp);
static bool writeDieArea(FILE *fp);
static bool writeRows(FILE *fp);
static bool writeTracks(FILE *fp);
static bool writeGcellGrid(FILE *fp);
static bool writeVias(FILE *fp);
static bool writeStyles(FILE *fp);
static bool writeNonDefaultRules(FILE *fp);
static bool writeRegions(FILE *fp);
static bool writeComponentMaskShift(FILE *fp);
static bool writeComponents(FILE *fp);
static bool writePins(FILE *fp);
static bool writePinProperties(FILE *fp);
static bool writeBlockages(FILE *fp);
static bool writeSlots(FILE *fp);
static bool writeFills(FILE *fp);
static bool writeSpecialNets(FILE *fp);
static bool writeNets(FILE *fp);
static bool writeScanChains(FILE *fp);
static bool writeGroups(FILE *fp);
static bool writeExtension(FILE *fp);
static bool writeEndDesign(FILE *fp);

int writeDef(int argc, const char **argv) {
    int num_out_file = 0;
    char *def_file_name = nullptr;
    bool debug_mode = false;

    argc--;
    argv++;

    while (argc--) {
        if (strcmp(*argv, "--help") == 0) {
            fprintf(stderr, "Usage: write_def <options> <file> \n");
            fprintf(stderr,
                    "\tOutput design to DEF file. Default filename is "
                    "topCellName.def\n");
            fprintf(stderr, "\tOverwrite the previous DEF file.\n");
            fprintf(stderr, "options:\n");
            fprintf(stderr, "\t--help -- write output to the file.\n");
            return ERROR;
        } else if (argv[0][0] != '-') {
            if (num_out_file >= 1) {
                fprintf(stderr, "ERROR: too many output files, max = 1.\n");
                return ERROR;
            }
            def_file_name = *(const_cast<char **>(argv));
            ++num_out_file;
        } else if (strcmp(*argv, "--debug") == 0) {
            debug_mode = true;
        }

        argv++;
    }
    if (debug_mode == true) ProfilerStart("test_write_def.prof");

    if (0 == num_out_file) {
        message->issueMsg(kError, "Cannot find output DEF file name.\n");
        return ERROR;
    }

    top_cell = getTopCell();
    if (!top_cell) {
        message->issueMsg(kError,
                          "Cannot get top cell when writing DEF file.\n");
        return ERROR;
    }
    FILE *fp = getDefFilePointer(def_file_name);
    if (!fp) {
        message->issueMsg(kError, "Open DEF file failed %s.\n", def_file_name);
        return ERROR;
    }
    message->info("\nWriting DEF\n");
    fflush(stdout);
    if (!writeFileHead(fp)) {
        message->issueMsg(kError, "Write DEF file Header failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeVersion(fp)) {
        message->issueMsg(kError, "Write DEF file Version failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeDividerChar(fp)) {
        message->issueMsg(kError, "Write DEF file Divider char failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeBusbitChars(fp)) {
        message->issueMsg(kError, "Write DEF file Busbit char failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeDesign(fp)) {
        message->issueMsg(kError, "Write DEF file Design failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeTechnology(fp)) {
        message->issueMsg(kError, "Write DEF file Technology failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeUnits(fp)) {
        message->issueMsg(kError, "Write DEF file Unit failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeHistory(fp)) {
        message->issueMsg(kError, "Write DEF file History failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writePropertyDefinitions(fp)) {
        message->issueMsg(kError,
                          "Write DEF file Property definitions failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeDieArea(fp)) {
        message->issueMsg(kError, "Write DEF file Die area failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeRows(fp)) {
        message->issueMsg(kError, "Write DEF file Rows failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeTracks(fp)) {
        message->issueMsg(kError, "Write DEF file Tracks failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeGcellGrid(fp)) {
        message->issueMsg(kError, "Write DEF file Gcell grids failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeVias(fp)) {
        message->issueMsg(kError, "Write DEF file Vias failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeStyles(fp)) {
        message->issueMsg(kError, "Write DEF file Styles failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeNonDefaultRules(fp)) {
        message->issueMsg(kError, "Write DEF file Nondefault rules failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeRegions(fp)) {
        message->issueMsg(kError, "Write DEF file Regions failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeComponentMaskShift(fp)) {
        message->issueMsg(kError,
                          "Write DEF file Component mask shift failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeComponents(fp)) {
        message->issueMsg(kError, "Write DEF file Components failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writePins(fp)) {
        message->issueMsg(kError, "Write DEF file Pins failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writePinProperties(fp)) {
        message->issueMsg(kError, "Write DEF file Pin properties failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeBlockages(fp)) {
        message->issueMsg(kError, "Write DEF file Blockages failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeSlots(fp)) {
        message->issueMsg(kError, "Write DEF file Slots failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeFills(fp)) {
        message->issueMsg(kError, "Write DEF file Fills failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeSpecialNets(fp)) {
        message->issueMsg(kError, "Write DEF file Special nets failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeNets(fp)) {
        message->issueMsg(kError, "Write DEF file Nets failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeScanChains(fp)) {
        message->issueMsg(kError, "Write DEF file Scan chains failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeGroups(fp)) {
        message->issueMsg(kError, "Write DEF file Groups failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeExtension(fp)) {
        message->issueMsg(kError, "Write DEF file Extensions failed.\n");
        fclose(fp);
        return ERROR;
    }
    if (!writeEndDesign(fp)) {
        message->issueMsg(kError, "Write DEF file End design failed.\n");
        fclose(fp);
        return ERROR;
    }

    fclose(fp);
    if (debug_mode == true) ProfilerStop();
    message->info("\nWrite DEF successfully.\n");
    return OK;
}

static FILE *getDefFilePointer(const char *file_name) {
    FILE *fp = fopen(file_name, "w");
    if (!fp) {
        message->issueMsg(kError, "Cannot open file %s\n", file_name);
        return nullptr;
    }
    return fp;
}

static bool writeFileHead(FILE *fp) {
    time_t timep;
    time(&timep);
    char time_str[256];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S",
             localtime(&timep));

    fprintf(fp,
            "##################################################################"
            "#############\n");
    fprintf(fp, "# DEF file generated by NIICEDA Tool\n");
    fprintf(fp, "# %s\n", time_str);
    fprintf(fp,
            "##################################################################"
            "#############\n");
    return true;
}
static bool writeVersion(FILE *fp) {
    fprintf(fp, "VERSION 5.8 ;\n");
    return true;
}
static bool writeDividerChar(FILE *fp) {
    fprintf(fp, "DIVIDERCHAR \"/\" ;\n");
    return true;
}
static bool writeBusbitChars(FILE *fp) {
    fprintf(fp, "BUSBITCHARS \"[]\" ;\n");
    return true;
}
static bool writeDesign(FILE *fp) {
    fprintf(fp, "DESIGN %s ;\n", top_cell->getName().c_str());

    return true;
}
static bool writeTechnology(FILE *fp) { return true; }
static bool writeUnits(FILE *fp) {
    Tech *tech_lib = top_cell->getTechLib();
    if (!tech_lib) {
        message->issueMsg(kError,
                          "Cannot find tech lib when writting DEF file.\n");
        return false;
    }
    Units *units = tech_lib->getUnits();
    if (!units) {
        message->issueMsg(kError,
                          "Cannot find units when writting DEF file.\n");
        return false;
    }
    fprintf(fp, "UNITS DISTANCE MICRONS %d ;\n", units->getLengthFactor());
    return true;
}
static bool writeHistory(FILE *fp) { return true; }
static bool writePropertyDefinitions(FILE *fp) {
    Tech *tech_lib = top_cell->getTechLib();
    if (!tech_lib) {
        message->issueMsg(kError,
                          "Cannot find tech lib when writting DEF file.\n");
        return false;
    }
    fprintf(fp, "\n\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");
    fprintf(fp, "# PROPERTYDEFINITIONS\n");
    fprintf(fp,
            "#    objectType propName propType [ RANGE min max ] [ value ] ; "
            "... \n");
    fprintf(fp, "# END PROPERTYDEFINITIONS\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");
    int count = 0;
    for (int i = 0; i < toInteger(PropType::kUnknown); ++i) {
        ObjectId vobj_id =
            tech_lib->getPropertyDefinitionVectorId(toEnum<PropType, int>(i));
        if (vobj_id == 0) continue;
        VectorObject32 *vobj = Object::addr< VectorObject32 >(vobj_id);
        if (!vobj) continue;
        for (VectorObject32::iterator iter = vobj->begin(); iter != vobj->end();
             ++iter) {
            ObjectId obj_id = (*iter);
            if (!obj_id) continue;
            PropertyDefinition *obj_data =
                Object::addr<PropertyDefinition>(obj_id);
            if (!obj_data) continue;
            if (!obj_data->getFromDEF()) continue;
            if (count == 0) {
                fprintf(fp, "PROPERTYDEFINITIONS\n");
            }
            obj_data->printDEF(fp);
            ++count;
        }
    }
    if (count > 0) {
        fprintf(fp, "END PROPERTYDEFINITIONS\n");
    }
    return true;
}

static bool writeDieArea(FILE *fp) {
    Polygon *polygon = nullptr;
    fprintf(fp, "\n\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");
    fprintf(fp, "# [DIEAREA pt pt [pt] ... ;]\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");

    Floorplan *floorplan = top_cell->getFloorplan();
    if (!floorplan) {
        message->issueMsg(kError,
                          "Cannot find floorplan when writting DEF file.\n");
        return false;
    }
    if (floorplan) polygon = floorplan->getDieAreaPolygon();
    if (!polygon) {
        message->issueMsg(
            kError, "Cannot find die area polygon when writting DEF file.\n");
        return false;
    }

    fprintf(fp, "DIEAREA");
    for (int i = 0; i < polygon->getNumPoints(); ++i) {
        fprintf(fp, " ( %d %d )", polygon->getPoint(i).getX(),
                polygon->getPoint(i).getY());
    }
    fprintf(fp, " ;\n");
    return true;
}
static bool writeRows(FILE *fp) {
    fprintf(fp, "\n\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");
    fprintf(fp, "# [ROW rowName siteName origX origY siteOrient\n");
    fprintf(fp, "#     [DO numX BY numY [STEP stepX stepY]]\n");
    fprintf(fp, "#     [+ PROPERTY {propName propVal} ...] ... ;] ...\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");

    Floorplan *floorplan = top_cell->getFloorplan();
    if (!floorplan) {
        message->issueMsg(kError,
                          "Cannot find floorplan when writting DEF file.\n");
        return false;
    }

    ObjectId rows = floorplan->getRows();
    if (0 == rows) {
        return true;
    }
    VectorObject64 *obj_vector = Object::addr< VectorObject64 >(rows);
    if (!obj_vector) {
        message->issueMsg(kError,
                          "Cannot find rows vector when writting DEF file.\n");
        return false;
    }
    for (int i = 0; i < obj_vector->totalSize(); ++i) {
        Row *row = Object::addr<Row>((*obj_vector)[i]);
        if (!row) {
            message->issueMsg(
                kError, "Cannot find row %d when writting DEF file.\n", i);
            continue;
        }
        row->print(fp);
    }
    return true;
}
static bool writeTracks(FILE *fp) {
    fprintf(fp, "\n\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");
    fprintf(fp, "# [TRACKS\n");
    fprintf(fp, "#     [{X | Y} start DO numtracks STEP space\n");
    fprintf(fp, "#       [MASK maskNum [SAMEMASK]]\n");
    fprintf(fp, "#       [LAYER layerName ...]\n");
    fprintf(fp, "#     ;] ...] \n");
    fprintf(fp,
            "##################################################################"
            "#############\n");

    Floorplan *floorplan = top_cell->getFloorplan();
    if (!floorplan) {
        message->issueMsg(kError,
                          "Cannot find floorplan when writting DEF file.\n");
        return false;
    }

    ObjectId tracks = floorplan->getTracks();
    if (0 == tracks) {
        return true;
    }
    VectorObject64 *obj_vector = Object::addr< VectorObject64 >(tracks);
    if (!obj_vector) {
        message->issueMsg(
            kError, "Cannot find tracks vector when writting DEF file.\n");
        return false;
    }

    for (int i = 0; i < obj_vector->totalSize(); ++i) {
        Track *track = Object::addr<Track>((*obj_vector)[i]);
        if (!track) {
            message->issueMsg(
                kError, "Cannot find track %d when writting DEF file.\n", i);
            continue;
        }
        track->print(fp);
    }
    return true;
}
static bool writeGcellGrid(FILE *fp) {
    fprintf(fp, "\n\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");
    fprintf(fp, "# [GCELLGRID\n");
    fprintf(fp, "#     {X start DO numColumns+1 STEP space} ...\n");
    fprintf(fp, "#     {Y start DO numRows+1 STEP space} ...\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");

    Floorplan *floorplan = top_cell->getFloorplan();
    if (!floorplan) {
        message->issueMsg(kError,
                          "Cannot find floorplan when writting DEF file.\n");
        return false;
    }

    ObjectId gcell_grids = floorplan->getGcellGrids();
    if (0 == gcell_grids) {
        return true;
    }
    VectorObject64 *obj_vector =
        Object::addr< VectorObject64 >(gcell_grids);
    if (!obj_vector) {
        message->issueMsg(
            kError, "Cannot find gcell grid vector when writting DEF file.\n");
        return false;
    }

    for (int i = 0; i < obj_vector->totalSize(); ++i) {
        Grid *gcell_grid = Object::addr<Grid>((*obj_vector)[i]);
        if (!gcell_grid) {
            message->issueMsg(
                kError, "Cannot find gcell grid %d when writting DEF file.\n",
                i);
            continue;
        }
        gcell_grid->print(fp);
    }
    return true;
}
static bool writeVias(FILE *fp) {
    Tech *lib = getTopCell()->getTechLib();
    ObjectId vias = lib->getViaMasterVectorId();
    if (vias == 0) return true;
    VectorObject64 *via_vector = Object::addr< VectorObject64 >(vias);
    int num_vias = 0;
    for (int i = 0; i < via_vector->totalSize(); ++i) {
        ViaMaster *via = Object::addr<ViaMaster>((*via_vector)[i]);
        if (!via) {
            continue;
        }
        if (!via->isFromDEF()) continue;
        ++num_vias;
    }
    fprintf(fp, "\nVIAS %d ;\n", num_vias);
    for (int i = 0; i < via_vector->totalSize(); ++i) {
        ViaMaster *via = Object::addr<ViaMaster>((*via_vector)[i]);
        if (!via) {
            message->issueMsg(
                kError, "Cannot find via %d when writting DEF file.\n", i);
            continue;
        }
        if (!via->isFromDEF()) continue;
        via->printDEF(fp);
    }
    fprintf(fp, "END VIAS\n");
    return true;
}
static bool writeStyles(FILE *fp) { return true; }
static bool writeNonDefaultRules(FILE *fp) {
    Tech *tech_lib = top_cell->getTechLib();
    if (!tech_lib) {
        message->issueMsg(kError,
                          "Cannot find tech lib when writting DEF file.\n");
        return false;
    }
    fprintf(fp, "\n\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");
    fprintf(fp, "# [NONDEFAULTRULES numNDRs ;\n");
    fprintf(fp, "#     {- STYLE styleNum pt pt ... ;} ...\n");
    fprintf(fp, "# END NONDEFAULTRULES]\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");

    ObjectId vobj_id = tech_lib->getNonDefaultRuleVectorId();
    if (vobj_id == 0) return true;
    VectorObject16 *vobj = Object::addr<VectorObject16>(vobj_id);
    if (!vobj) return true;
    int defrule_count = 0;
    int nondefault_count = 0;

    for (VectorObject16::iterator iter = vobj->begin(); iter != vobj->end();
         ++iter) {
        ObjectId obj_id = (*iter);
        if (!obj_id) continue;
        NonDefaultRule *obj_data =
            Object::addr<NonDefaultRule>(obj_id);
        if (!obj_data) continue;
        if (obj_data->getFromDEF()) {
            defrule_count++;
            std::string name = obj_data->getName();
            if (name.find("DEFAULT") != std::string::npos) {
                nondefault_count++;
            }
        }
    }

    if (defrule_count == 0) return true;

    fprintf(fp, "NONDEFAULTRULES %d ;\n", nondefault_count);

    for (VectorObject16::iterator iter = vobj->begin(); iter != vobj->end();
         ++iter) {
        ObjectId obj_id = (*iter);
        if (!obj_id) continue;
        NonDefaultRule *obj_data =
            Object::addr<NonDefaultRule>(obj_id);
        if (!obj_data) continue;
        if (!obj_data->getFromDEF()) continue;
        obj_data->printDEF(fp);
    }

    fprintf(fp, "END NONDEFAULTRULES\n");
    return true;
}
static bool writeRegions(FILE *fp) {
    Floorplan *floorplan = top_cell->getFloorplan();
    if (!floorplan) {
        message->issueMsg(kError,
                          "Cannot find floorplan when writting DEF file.\n");
        return false;
    }
    int region_num = floorplan->getNumOfRegions();
    if (0 == region_num) {
        return true;
    }
    fprintf(fp, "\n\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");
    fprintf(fp, "# [REGIONS numRegions ;\n");
    fprintf(fp, "# [- regionName {pt pt} ...\n");
    fprintf(fp, "#   [+ TYPE {FENCE | GUIDE}]\n");
    fprintf(fp, "#   [+ PROPERTY {propName propVal} ...] ...\n");
    fprintf(fp, "# ;] ...\n");
    fprintf(fp, "# END REGIONS]\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");
    fprintf(fp, "REGIONS %d ;\n", region_num);
    ObjectId regions_id = floorplan->getRegions();
    VectorObject32 *obj_vector =
        Object::addr< VectorObject32 >(regions_id);
    for (int i = 0; i < region_num; i++) {
        Constraint *region =
            Object::addr<Constraint>((*obj_vector)[i]);
        if (!region) {
            continue;
        }
        region->printRegion(fp);
    }

    fprintf(fp, "END REGIONS\n");
    return true;
}
static bool writeComponentMaskShift(FILE *fp) {
    if (top_cell->getNumMaskShiftLayers() > 0) {
        fprintf(fp, "\n\n");
        fprintf(fp,
                "##############################################################"
                "#################\n");
        fprintf(fp, "#[COMPONENTMASKSHIFT layer1 [layer2 ...] ;]\n");
        fprintf(fp,
                "##############################################################"
                "#################\n");

        fprintf(fp, "COMPONENTMASKSHIFT");
        for (int i = 0; i < top_cell->getNumMaskShiftLayers(); ++i) {
            ObjectId layer_id = top_cell->getMaskShiftLayer(i);
            Layer *layer = top_cell->getLayerByLayerId(layer_id);

            fprintf(fp, " %s", layer->getName());
        }
        fprintf(fp, " ;\n");
    }
    return true;
}
static bool writeComponents(FILE *fp) {
    fprintf(fp, "\n\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");
    fprintf(fp, "# COMPONENTS numComps ;\n");
    fprintf(fp, "#     [– compName modelName\n");
    fprintf(fp, "#         [+ EEQMASTER macroName]\n");
    fprintf(fp, "#         [+ SOURCE {NETLIST | DIST | USER | TIMING}]\n");
    fprintf(fp,
            "#         [+ {FIXED pt orient | COVER pt orient | PLACED pt "
            "orient | UNPLACED} ]\n");
    fprintf(fp, "#         [+ MASKSHIFT shiftLayerMasks]\n");
    fprintf(fp, "#         [+ HALO [SOFT] left bottom right top]\n");
    fprintf(fp, "#         [+ ROUTEHALO haloDist minLayer maxLayer]\n");
    fprintf(fp, "#         [+ WEIGHT weight]\n");
    fprintf(fp, "#         [+ REGION regionName]\n");
    fprintf(fp, "#         [+ PROPERTY {propName propVal} ...]...\n");
    fprintf(fp, "#     ;] ...\n");
    fprintf(fp, "# END COMPONENTS\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");

    uint64_t num_components = top_cell->getNumOfInsts();
    ObjectId components = top_cell->getInstances();
    ArrayObject<ObjectId> *component_vector =
        Object::addr< ArrayObject<ObjectId> >(components);
    fprintf(fp, "COMPONENTS %d ;\n", num_components);
    for (auto iter = component_vector->begin(); iter != component_vector->end();
         ++iter) {
        Inst *instance = Object::addr<Inst>(*iter);
        if (!instance) {
            message->issueMsg(
                kError, "Cannot find instance %d when writting DEF file.\n");
            continue;
        }
        instance->print(fp);
    }
    fprintf(fp, "END COMPONENTS\n");

    return true;
}
static bool writePins(FILE *fp) {
    uint64_t pin_num = top_cell->getNumOfIOPins();
    if (pin_num == 0) {
        return true;
    }
    fprintf(fp, "PINS %d ;\n", pin_num);
    for (int i = 0; i < pin_num; i++) {
        Pin *pin = top_cell->getIOPin(i);
        if (!pin) {
            continue;
        }
        fprintf(fp, "    - %s", pin->getName().c_str());
        Net *net = pin->getNet();
        fprintf(fp, " + NET %s\n", (net ? net->getName().c_str() : "ERROR"));
        if (pin->getHasSpecial()) {
            fprintf(fp, "      + SPECIAL\n");
        }
        Term *term = pin->getTerm();
        std::string str_value;
        if (term->hasDirection()) {
            str_value.assign(term->getDirectionStr());
            toUpper(str_value);
            fprintf(fp, "      + DIRECTION %s\n", str_value.c_str());
        }
        if (term->getNetExprIndex() != 0) {
            fprintf(fp, "      + NETEXPR \"%s\"\n", term->getNetExpr().c_str());
        }
        if (term->getSupplySensitivityIndex() != 0) {
            fprintf(fp, "      + SUPPLYSENSITIVITY %s\n",
                    term->getSupplySensitivity().c_str());
        }
        if (term->getGroundSensitivityIndex() != 0) {
            fprintf(fp, "      + GROUNDSENSITIVITY %s\n",
                    term->getGroundSensitivity().c_str());
        }
        if (term->hasUse()) {
            str_value.assign(term->getPinTypeStr());
            toUpper(str_value);
            fprintf(fp, "      + USE %s\n", str_value.c_str());
        }
        if (term->getAntennaPartialMetalAreaNum() > 0) {
            for (int i = 0; i < term->getAntennaPartialMetalAreaNum(); i++) {
                AntennaArea *a = term->getAntennaPartialMetalArea(i);
                fprintf(fp, "      + ANTENNAPINPARTIALMETALAREA %f",
                        a->getArea());
                if (a->getLayerNameID() >= 0) {
                    fprintf(fp, " LAYER %s\n", a->getLayerName().c_str());
                } else {
                    fprintf(fp, "\n");
                }
            }
        }
        if (term->getAntennaPartialMetalSideAreaNum() > 0) {
            for (int i = 0; i < term->getAntennaPartialMetalSideAreaNum();
                 i++) {
                AntennaArea *a = term->getAntennaPartialMetalSideArea(i);
                fprintf(fp, "      + ANTENNAPINPARTIALMETALSIDEAREA %f",
                        a->getArea());
                if (a->getLayerNameID() >= 0) {
                    fprintf(fp, " LAYER %s\n", a->getLayerName().c_str());
                } else {
                    fprintf(fp, "\n");
                }
            }
        }
        if (term->getAntennaPartialCutAreaNum() > 0) {
            for (int i = 0; i < term->getAntennaPartialCutAreaNum(); i++) {
                AntennaArea *a = term->getAntennaPartialCutArea(i);
                fprintf(fp, "      + ANTENNAPINPARTIALCUTAREA %f",
                        a->getArea());
                if (a->getLayerNameID() >= 0) {
                    fprintf(fp, " LAYER %s\n", a->getLayerName().c_str());
                } else {
                    fprintf(fp, "\n");
                }
            }
        }
        if (term->getAntennaDiffAreaNum() > 0) {
            for (int i = 0; i < term->getAntennaDiffAreaNum(); i++) {
                AntennaArea *a = term->getAntennaDiffArea(i);
                fprintf(fp, "      + ANTENNAPINDIFFAREA %f", a->getArea());
                if (a->getLayerNameID() >= 0) {
                    fprintf(fp, " LAYER %s\n", a->getLayerName().c_str());
                } else {
                    fprintf(fp, "\n");
                }
            }
        }
        for (int i = 0; i < 6; i++) {
            const AntennaModelTerm *am = term->GetAntennaModelTerm(i);
            if (am->getValid()) {
                fprintf(fp, "      + ANTENNAMODEL OXIDE%d\n", i);
                if (am->getAntennaGateAreaNum() > 0) {
                    for (int i = 0; i < am->getAntennaGateAreaNum(); i++) {
                        AntennaArea *a = am->getAntennaGateArea(i);
                        fprintf(fp, "          + ANTENNAPINGATEAREA %f",
                                a->getArea());
                        if (a->getLayerNameID() >= 0) {
                            fprintf(fp, " LAYER %s\n",
                                    a->getLayerName().c_str());
                        } else {
                            fprintf(fp, "\n");
                        }
                    }
                }
                if (am->getAntennaMaxAreaCarNum() > 0) {
                    for (int i = 0; i < am->getAntennaMaxAreaCarNum(); i++) {
                        AntennaArea *a = am->getAntennaMaxAreaCar(i);
                        fprintf(fp, "          + ANTENNAPINMAXAREACAR %f",
                                a->getArea());
                        if (a->getLayerNameID() >= 0) {
                            fprintf(fp, " LAYER %s\n",
                                    a->getLayerName().c_str());
                        } else {
                            fprintf(fp, "\n");
                        }
                    }
                }
                if (am->getAntennaMaxSideAreaCarNum() > 0) {
                    for (int i = 0; i < am->getAntennaMaxSideAreaCarNum();
                         i++) {
                        AntennaArea *a = am->getAntennaMaxSideAreaCar(i);
                        fprintf(fp, "          + ANTENNAPINMAXSIDEAREACAR %f",
                                a->getArea());
                        if (a->getLayerNameID() >= 0) {
                            fprintf(fp, " LAYER %s\n",
                                    a->getLayerName().c_str());
                        } else {
                            fprintf(fp, "\n");
                        }
                    }
                }
                if (am->getAntennaMaxCutCarNum() > 0) {
                    for (int i = 0; i < am->getAntennaMaxCutCarNum(); i++) {
                        AntennaArea *a = am->getAntennaMaxCutCar(i);
                        fprintf(fp, "          + ANTENNAPINMAXCUTCAR %f",
                                a->getArea());
                        if (a->getLayerNameID() >= 0) {
                            fprintf(fp, " LAYER %s\n",
                                    a->getLayerName().c_str());
                        } else {
                            fprintf(fp, "\n");
                        }
                    }
                }
            }
        }
        if (term->getPortNum() > 0) {
            for (int i = 0; i < term->getPortNum(); i++) {
                Port *p = term->getPort(i);
                if (p->getIsReal()) {
                    fprintf(fp, "      + PORT\n");
                }
                if (p->getLayerGeometryNum() > 0) {
                    for (int j = 0; j < p->getLayerGeometryNum(); j++) {
                        LayerGeometry *lg = p->getLayerGeometry(j);
                        if (lg->getVecNum() > 0) {
                            for (int k = 0; k < lg->getVecNum(); k++) {
                                if (lg->getType() == GeometryType::kVia) {
                                    GeometryVia *via = lg->getGeometryVia(k);
                                    if (p->getIsReal()) {
                                        // more indents if there is PORT
                                        fprintf(fp, "        + VIA %s",
                                                via->getName().c_str());
                                    } else {
                                        fprintf(fp, "      + VIA %s",
                                                via->getName().c_str());
                                    }
                                    if (via->getTopMaskNum() ||
                                        via->getCutMaskNum() ||
                                        via->getBottomMaskNum()) {
                                        if (via->getTopMaskNum()) {
                                            fprintf(fp, " MASK %d%d%d",
                                                    via->getTopMaskNum(),
                                                    via->getCutMaskNum(),
                                                    via->getBottomMaskNum());
                                        } else if (via->getCutMaskNum()) {
                                            fprintf(fp, " MASK %d%d",
                                                    via->getCutMaskNum(),
                                                    via->getBottomMaskNum());
                                        } else {
                                            fprintf(fp, " MASK %d",
                                                    via->getBottomMaskNum());
                                        }
                                    }
                                    fprintf(fp, " ( %d %d )\n",
                                            via->getPoint().getX(),
                                            via->getPoint().getY());
                                } else {
                                    Geometry *geo = lg->getGeometry(k);
                                    switch (geo->getType()) {
                                        case GeometryType::kRect:
                                            if (p->getIsReal()) {
                                                fprintf(fp,
                                                        "        + LAYER %s",
                                                        lg->getName().c_str());
                                            } else {
                                                fprintf(fp, "      + LAYER %s",
                                                        lg->getName().c_str());
                                            }
                                            if (geo->getNumMask() > 0) {
                                                fprintf(fp, " MASK %d",
                                                        geo->getNumMask());
                                            }
                                            if (lg->getMinSpacing() >= 0) {
                                                fprintf(fp, " SPACING %d",
                                                        lg->getMinSpacing());
                                            }
                                            if (lg->getdesignRuleWidth() >= 0) {
                                                fprintf(
                                                    fp, " DESIGNRULEWIDTH %d",
                                                    lg->getdesignRuleWidth());
                                            }
                                            fprintf(fp,
                                                    " ( %d %d ) ( %d %d )\n",
                                                    geo->getBox().getLLX(),
                                                    geo->getBox().getLLY(),
                                                    geo->getBox().getURX(),
                                                    geo->getBox().getURY());
                                            break;
                                        case GeometryType::kPolygon:
                                            if (p->getIsReal()) {
                                                fprintf(fp,
                                                        "        + POLYGON %s",
                                                        lg->getName().c_str());
                                            } else {
                                                fprintf(fp,
                                                        "      + POLYGON %s",
                                                        lg->getName().c_str());
                                            }
                                            if (geo->getNumMask() > 0) {
                                                fprintf(fp, " MASK %d",
                                                        geo->getNumMask());
                                            }
                                            if (lg->getMinSpacing() >= 0) {
                                                fprintf(fp, " SPACING %d",
                                                        lg->getMinSpacing());
                                            }
                                            if (lg->getdesignRuleWidth() >= 0) {
                                                fprintf(
                                                    fp, " DESIGNRULEWIDTH %d",
                                                    lg->getdesignRuleWidth());
                                            }
                                            Polygon *po =
                                                lg->getPolygonTable()
                                                    ->getPolygonByIndex(
                                                        geo->getPolygonID());
                                            for (int x = 0;
                                                 x < po->getNumPoints(); x++) {
                                                fprintf(fp, " ( %d %d )",
                                                        po->getPoint(x).getX(),
                                                        po->getPoint(x).getY());
                                            }
                                            fprintf(fp, "\n");
                                            break;
                                    }
                                }
                            }
                        }
                    }
                }
                if (p->getHasPlacement()) {
                    Point pt = p->getLocation();
                    if (p->getStatus() == PlaceStatus::kCover) {
                        if (p->getIsReal()) {
                            fprintf(fp, "        + COVER ( %d %d ) %s\n",
                                    pt.getX(), pt.getY(),
                                    toString(p->getOrient()).c_str());
                        } else {
                            fprintf(fp, "      + COVER ( %d %d ) %s\n",
                                    pt.getX(), pt.getY(),
                                    toString(p->getOrient()).c_str());
                        }
                    } else if (p->getStatus() == PlaceStatus::kFixed) {
                        if (p->getIsReal()) {
                            fprintf(fp, "        + FIXED ( %d %d ) %s\n",
                                    pt.getX(), pt.getY(),
                                    toString(p->getOrient()).c_str());
                        } else {
                            fprintf(fp, "      + FIXED ( %d %d ) %s\n",
                                    pt.getX(), pt.getY(),
                                    toString(p->getOrient()).c_str());
                        }
                    } else if (p->getStatus() == PlaceStatus::kPlaced) {
                        if (p->getIsReal()) {
                            fprintf(fp, "        + PLACED ( %d %d ) %s\n",
                                    pt.getX(), pt.getY(),
                                    toString(p->getOrient()).c_str());
                        } else {
                            fprintf(fp, "      + PLACED ( %d %d ) %s\n",
                                    pt.getX(), pt.getY(),
                                    toString(p->getOrient()).c_str());
                        }
                    }
                }
            }
        }
        fprintf(fp, "    ;\n");
    }
    fprintf(fp, "END PINS\n");
    return true;
}
static bool writePinProperties(FILE *fp) { return true; }
static bool writeBlockages(FILE *fp) {
    Floorplan *floorplan = top_cell->getFloorplan();
    if (!floorplan) {
        message->issueMsg(kError,
                          "Cannot find floorplan when writting DEF file.\n");
        return false;
    }
    int sum_blockages = floorplan->getNumOfPlaceBlockages() +
                        floorplan->getNumOfRouteBlockages();
    if (0 == sum_blockages) {
        return true;
    }

    ObjectId route_blockages = floorplan->getRouteBlockages();
    VectorObject32 *route_vector = nullptr;
    if (route_blockages > 0) {
        route_vector = Object::addr< VectorObject32 >(route_blockages);
        if (!route_vector) {
            message->issueMsg(
                kError,
                "Cannot find route blockage vector when writting DEF file.\n");
            return false;
        }
    }
    ObjectId place_blockages = floorplan->getPlaceBlockages();
    VectorObject32 *place_vector = nullptr;
    if (place_blockages > 0) {
        place_vector = Object::addr< VectorObject32 >(place_blockages);
        if (!place_vector) {
            message->issueMsg(
                kError,
                "Cannot find place blockage vector when writting DEF file.\n");
            return false;
        }
    }

    fprintf(fp, "\n\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");
    fprintf(fp, "# [BLOCKAGES numBlockages ;\n");
    fprintf(fp,
            "#     [- LAYER layerName  [ + SLOTS | + FILLS] [ + PUSHDOWN] [ + "
            "EXCEPTPGNET]\n");
    fprintf(fp, "#       [ + COMPONENT compName]\n");
    fprintf(
        fp,
        "#       [ + SPACING minSpacing | + DESIGNRULEWIDTH effectiveWidth]\n");
    fprintf(fp, "#       [ + MASK maskNum]\n");
    fprintf(fp, "#            {RECT pt pt | POLYGON pt pt pt ...} ...\n");
    fprintf(fp, "#     ;] ...\n");
    fprintf(fp,
            "#     [- PLACEMENT [ + SOFT | + PARTIAL maxDensity] [ + PUSHDOWN] "
            "[ + COMPONENT "
            "compName]\n");
    fprintf(fp, "#            {RECT pt pt} ...\n");
    fprintf(fp, "#     ;] ...\n");
    fprintf(fp, "# END BLOCKAGES]\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");

    fprintf(fp, "BLOCKAGES %d ;\n", sum_blockages);
    if (route_vector) {
        for (int i = 0; i < route_vector->totalSize(); ++i) {
            Constraint *route_blockage =
                Object::addr<Constraint>((*route_vector)[i]);
            if (!route_blockage) {
                message->issueMsg(
                    kError,
                    "Cannot find route blockage %d when writting DEF file.\n",
                    i);
                continue;
            }
            route_blockage->printBlockage(fp);
        }
    }
    if (place_vector) {
        for (int i = 0; i < place_vector->totalSize(); ++i) {
            Constraint *place_blockage =
                Object::addr<Constraint>((*place_vector)[i]);
            if (!place_blockage) {
                message->issueMsg(
                    kError,
                    "Cannot find place blockage %d when writting DEF file.\n",
                    i);
                continue;
            }
            place_blockage->printBlockage(fp);
        }
    }
    fprintf(fp, "END BLOCKAGES\n");
    return true;
}
static bool writeSlots(FILE *fp) { return true; }
static bool writeFills(FILE *fp) {
    int fill_num = top_cell->getNumOfFills();
    if (fill_num == 0) {
        return true;
    }
    fprintf(fp, "FILLS %d ;\n", fill_num);
    for (int i = 0; i < fill_num; i++) {
        Fill *fill = top_cell->getFill(i);
        if (!fill) {
            continue;
        }
        if (fill->getIsLayer()) {
            Layer *layer = top_cell->getTechLib()->getLayer(fill->getLayerId());
            if (!layer) {
                continue;
            }
            fprintf(fp, "    - LAYER %s ", layer->getName());
            if (fill->getLayerMask()) {
                fprintf(fp, "+ MASK %d ", fill->getLayerMask());
            }
            if (fill->getIsOpc()) {
                fprintf(fp, "+ OPC");
            }
            std::vector<std::vector<Point> > *points = fill->getPointsArray();
            for (unsigned int j = 0; j < points->size(); j++) {
                if (fill->getIsRect(j)) {
                    fprintf(fp, "\n        RECT ( %d %d ) ( %d %d )",
                            (*points)[j][0].getX(), (*points)[j][0].getY(),
                            (*points)[j][1].getX(), (*points)[j][1].getY());
                } else {
                    fprintf(fp, "\n        POLYGON");
                    for (int k = 0; k < (*points)[j].size(); k++) {
                        fprintf(fp, " ( %d %d )", (*points)[j][k].getX(),
                                (*points)[j][k].getY());
                    }
                }
            }
            fprintf(fp, " ;\n");
        } else if (fill->getIsVia()) {
            ViaMaster *via = fill->getVia();
            if (!via) {
                continue;
            }
            fprintf(fp, "    - VIA %s ", via->getName().c_str());
            if (fill->getViaTopMask() || fill->getViaCutMask() ||
                fill->getViaBotMask()) {
                fprintf(fp, "+ MASK ");
                if (fill->getViaTopMask()) {
                    fprintf(fp, "%d", fill->getViaTopMask());
                }
                if (fill->getViaCutMask()) {
                    fprintf(fp, "%d", fill->getViaCutMask());
                }
                if (fill->getViaBotMask()) {
                    fprintf(fp, "%d", fill->getViaBotMask());
                }
                fprintf(fp, " ");
            }
            if (fill->getIsOpc()) {
                fprintf(fp, "+ OPC");
            }
            std::vector<std::vector<Point> > *points = fill->getPointsArray();
            for (unsigned int j = 0; j < points->size(); j++) {
                for (int k = 0; k < (*points)[j].size(); k++) {
                    fprintf(fp, " ( %d %d )", (*points)[j][k].getX(),
                            (*points)[j][k].getY());
                }
            }
            fprintf(fp, " ;\n");
        }
    }
    fprintf(fp, "END FILLS\n");
    return true;
}
static bool writeSpecialNets(FILE *fp) {
    int special_nets_num = top_cell->getNumOfSpecialNets();
    if (special_nets_num == 0) return true;
    ObjectId special_nets = top_cell->getSpecialNets();
    ArrayObject<ObjectId> *special_net_vector =
        Object::addr< ArrayObject<ObjectId> >(special_nets);
    fprintf(fp, "\nSPECIALNETS %d ;\n", special_nets_num);
    for (int i = 0; i < special_net_vector->getSize(); ++i) {
        SpecialNet *special_net =
            Object::addr<SpecialNet>((*special_net_vector)[i]);
        if (!special_net) {
            message->issueMsg(
                kError, "Cannot find special net %d when writting DEF file.\n");
            continue;
        }
        special_net->printDEF(fp);
    }
    fprintf(fp, "END SPECIALNETS\n");
    return true;
}
static bool writeNets(FILE *fp) {
    int nets_num = top_cell->getNumOfNets();
    if (nets_num == 0) return true;
    ObjectId nets = top_cell->getNets();
    ArrayObject<ObjectId> *net_vector =
                                   Object::addr< ArrayObject<ObjectId> >(nets);
    fprintf(fp, "\nNETS %d ;\n", nets_num);
    for (auto iter = net_vector->begin(); iter != net_vector->end(); ++iter) {
        Net *net = Object::addr<Net>(*iter);
        if (!net) {
            message->issueMsg(kError,
                              "Cannot find net %d when writting DEF file.\n");
            continue;
        }
        net->printDEF(fp);
    }
    fprintf(fp, "END NETS\n");

    return true;
}
static void writeScanChainPoint(FILE *fp, ScanChainPoint *point,
                                bool is_start) {
    if (is_start) {
        fprintf(fp, "        + START");
    } else {
        fprintf(fp, "        + STOP");
    }
    if (point->getIsIOPin()) {
        fprintf(fp, " PIN");
        if (point->getHasPin()) {
            Pin *pin = top_cell->getIOPinById(point->getPinId());
            if (pin) {
                fprintf(fp, " %s", pin->getName().c_str());
            }
        }
    } else {
        Inst *inst = top_cell->getInstance(point->getInstId());
        if (inst) {
            fprintf(fp, " %s", inst->getName().c_str());
            Pin *pin = inst->getPinById(point->getPinId());
            if (pin) {
                fprintf(fp, " %s", pin->getName().c_str());
            }
        }
    }
    fprintf(fp, "\n");
}
static void writeScanChainList(FILE *fp, ScanChainList *list,
                               bool is_floating) {
    for (; list; list = list->getNext()) {
        if (is_floating) {
            fprintf(fp, "        + FLOATING\n");
        } else {
            fprintf(fp, "        + ORDERED\n");
        }
        ScanChainListItem *items = list->getItems();
        for (ScanChainListItem *item = items; item; item = item->getNext()) {
            Inst *inst = top_cell->getInstance(item->getInstId());
            if (inst) {
                fprintf(fp, "            %s", inst->getName().c_str());
                if (item->getHasInPin()) {
                    Pin *pin = inst->getPinById(item->getInPinId());
                    if (pin) {
                        fprintf(fp, " ( IN %s )", pin->getName().c_str());
                    }
                }
                if (item->getHasOutPin()) {
                    Pin *pin = inst->getPinById(item->getOutPinId());
                    if (pin) {
                        fprintf(fp, " ( OUT %s )", pin->getName().c_str());
                    }
                }
                if (item->getHasBits()) {
                    fprintf(fp, " ( BITS %d )", item->getNumBits());
                }
                fprintf(fp, "\n");
            }
        }
        fprintf(fp, "\n");
    }
}
static bool writeScanChains(FILE *fp) {
    int scan_chain_num = top_cell->getNumOfScanChains();
    if (scan_chain_num == 0) {
        return true;
    }
    fprintf(fp, "SCANCHAINS %d ;\n", scan_chain_num);
    for (int i = 0; i < scan_chain_num; i++) {
        ScanChain *scan_chain = top_cell->getScanChain(i);
        if (!scan_chain) {
            continue;
        }
        fprintf(fp, "    - %s\n", scan_chain->getChainName());
        if (scan_chain->getHasPartition()) {
            fprintf(fp, "        + PARTITION %s ",
                    scan_chain->getPartitionName());
            if (scan_chain->getHasMaxBits()) {
                fprintf(fp, "MAXBITS %d", scan_chain->getMaxBits());
            }
            fprintf(fp, "\n");
        }
        if (scan_chain->getHasCommonScanPins()) {
            fprintf(fp, "        + COMMONSCANPINS");
            if (scan_chain->getHasCommonInPin()) {
                fprintf(fp, " ( IN %s )", scan_chain->getCommonInPin());
            }
            if (scan_chain->getHasCommonOutPin()) {
                fprintf(fp, " ( OUT %s )", scan_chain->getCommonOutPin());
            }
            fprintf(fp, "\n");
        }
        ScanChainPoint *start = scan_chain->getStart();
        if (start) {
            writeScanChainPoint(fp, start, true);
        }
        ScanChainList *floating = scan_chain->getFloating();
        if (floating) {
            writeScanChainList(fp, floating, true);
        }
        ScanChainList *ordered = scan_chain->getOrdered();
        if (ordered) {
            writeScanChainList(fp, ordered, false);
        }
        ScanChainPoint *stop = scan_chain->getStop();
        if (stop) {
            writeScanChainPoint(fp, stop, false);
        }
        fprintf(fp, "    ;\n");
    }
    fprintf(fp, "END SCANCHAINS\n");
    return true;
}
static bool writeGroups(FILE *fp) {
    int groups_num = top_cell->getNumOfGroups();
    if (groups_num == 0) {
        return true;
    }
    ObjectId groups = top_cell->getGroups();
    ArrayObject<ObjectId> *group_vector =
        Object::addr< ArrayObject<ObjectId> >(groups);

    fprintf(fp, "\n\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");
    fprintf(fp, "# [GROUPS numGroups ;\n");
    fprintf(fp, "#      [– groupName [compNamePattern ... ]\n");
    fprintf(fp, "#         [+ REGION regionNam]\n");
    fprintf(fp, "#         [+ PROPERTY {propName propVal} ...] ...\n");
    fprintf(fp, "#      ;] ...\n");
    fprintf(fp, "# END GROUPS]\n");
    fprintf(fp,
            "##################################################################"
            "#############\n");

    fprintf(fp, "GROUPS %d ;\n", groups_num);
    for (int i = 0; i < group_vector->getSize(); ++i) {
        Group *group = Object::addr<Group>((*group_vector)[i]);
        if (!group) {
            message->issueMsg(
                kError, "Cannot find group %d when writting DEF file.\n", i);
            continue;
        }
        group->print(fp);
    }
    fprintf(fp, "END GROUPS\n");

    return true;
}
static bool writeExtension(FILE *fp) { return true; }
static bool writeEndDesign(FILE *fp) {
    fprintf(fp, "END DESIGN\n");
    return true;
}

}  // namespace db
}  // namespace open_edi
