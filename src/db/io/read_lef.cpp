/* @file  read_lef.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "db/io/read_lef.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <string>

#include "db/core/db.h"
#include "db/core/object.h"
#include "db/util/geometrys.h"
#include "db/util/property_definition.h"
#include "util/polygon_table.h"

namespace open_edi {
namespace db {

char defaultName[128];
char defaultOut[128];
FILE *fout;
int isSessionles = 0;
static Cell *currentCell;

void checkType(lefrCallbackType_e c) {
    if (c >= 0 && c <= lefrLibraryEndCbkType) {
        // OK
    } else {
        fprintf(fout, "ERROR: callback type is out of bounds!\n");
    }
}

Orient orientTransfer(int orient) {
    switch (orient) {
        case 0:
            return Orient::kN;
        case 1:
            return Orient::kW;
        case 2:
            return Orient::kS;
        case 3:
            return Orient::kE;
        case 4:
            return Orient::kFN;
        case 5:
            return Orient::kFW;
        case 6:
            return Orient::kFS;
        case 7:
            return Orient::kFE;
    }
    return Orient::kUnknown;
}

void lefVia(lefiVia *via) { return; }

void lefSpacing(lefiSpacing *spacing) { return; }

void lefViaRuleLayer(lefiViaRuleLayer *vLayer) { return; }

int prtGeometry(lefiGeometries *geometry, Port *port = nullptr) {
    int numItems = geometry->lefiGeometries::numItems();
    int i, j;
    lefiGeomPath *path;
    lefiGeomPathIter *pathIter;
    lefiGeomRect *rect;
    lefiGeomRectIter *rectIter;
    lefiGeomPolygon *polygon;
    lefiGeomPolygonIter *polygonIter;
    lefiGeomVia *via;
    lefiGeomViaIter *viaIter;

    Cell *current_top_cell = getTopCell();
    if (!current_top_cell) return 0;
    Tech *lib = current_top_cell->getTechLib();
    LayerGeometry *current_lg = nullptr;
    // GeometryGroup *group = new GeometryGroup();
    // Geometryvia *via = new GeometryVia();
    for (i = 0; i < numItems; i++) {
        switch (geometry->lefiGeometries::itemType(i)) {
            case lefiGeomClassE:  // does it belong to port?
                // fprintf(fout, "CLASS %s ",
                // geometry->lefiGeometries::getClass(i));
                break;
            case lefiGeomLayerE: {
                if (current_lg) {
                    if (port) {
                        port->addLayerGeometry(current_lg->getId());
                    } else {
                        if (currentCell)
                            currentCell->addOBS(current_lg->getId());
                    }
                }
                LayerGeometry *geo =
                    current_top_cell->createObject<LayerGeometry>(
                        kObjectTypeLayerGeometry);
                current_lg = geo;
                current_lg->setName(geometry->lefiGeometries::getLayer(i));
                break;
            }
            case lefiGeomLayerExceptPgNetE:
                if (!current_lg) {
                    break;
                }
                current_lg->sethasEXCEPTPGNET(1);
                break;
            case lefiGeomLayerMinSpacingE:
                if (!current_lg) {
                    break;
                }
                current_lg->setHasSpacing(true);
                current_lg->setMinSpacing(lib->micronsToDBU(
                    geometry->lefiGeometries::getLayerMinSpacing(i)));
                break;
            case lefiGeomLayerRuleWidthE:
                if (!current_lg) {
                    break;
                }
                current_lg->setHasDRW(true);
                current_lg->setdesignRuleWidth(lib->micronsToDBU(
                    geometry->lefiGeometries::getLayerRuleWidth(i)));
                break;
            case lefiGeomWidthE:
                if (!current_lg) {
                    break;
                }
                current_lg->setWidth(
                    lib->micronsToDBU(geometry->lefiGeometries::getWidth(i)));
                break;
            case lefiGeomPathE: {
                if (!current_lg) {
                    break;
                }
                current_lg->setType(GeometryType::kPath);
                path = geometry->lefiGeometries::getPath(i);
                Geometry *base = current_top_cell->createObject<Geometry>(
                    kObjectTypeGeometry);
                Polygon *geoPath = new Polygon();  // use polygon to path
                if (path->colorMask != 0) {
                    base->setNumMask(path->colorMask);
                }
                for (j = 0; j < path->numPoints; j++) {
                    Point *p = new Point(lib->micronsToDBU(path->x[j]),
                                         lib->micronsToDBU(path->y[j]));
                    geoPath->addPoint(*p);
                }
                base->setPathID(current_lg->CreatePolygon(geoPath));
                base->setType(GeometryType::kPath);
                current_lg->addGeometry(base->getId());
                break;
            }
            case lefiGeomPathIterE:
                if (!current_lg) {
                    break;
                }
                current_lg->setType(GeometryType::kPath);
                // current_lg->setHasPathIter(1);
                pathIter = geometry->lefiGeometries::getPathIter(i);
                // GeomPathIter *geoPathiter = new GeomPathIter();
                for (int i = 0; i < pathIter->xStart; i++) {
                    for (int j = 0; j < pathIter->yStart; j++) {
                        Geometry *base =
                            current_top_cell->createObject<Geometry>(
                                kObjectTypeGeometry);
                        Polygon *geoPath = new Polygon();
                        if (pathIter->colorMask != 0)
                            base->setNumMask(pathIter->colorMask);
                        for (j = 0; j < pathIter->numPoints; j++) {
                            Point *p = new Point(
                                lib->micronsToDBU(pathIter->x[j] +
                                                  i * pathIter->xStep),
                                lib->micronsToDBU(pathIter->y[j] +
                                                  j * pathIter->yStep));
                            geoPath->addPoint(*p);
                        }
                        base->setPathID(current_lg->CreatePolygon(geoPath));
                        base->setType(GeometryType::kPath);
                        current_lg->addGeometry(base->getId());
                    }
                }
                break;
            case lefiGeomRectE: {
                if (!current_lg) {
                    break;
                }
                current_lg->setType(GeometryType::kRect);
                rect = geometry->lefiGeometries::getRect(i);
                Geometry *base = current_top_cell->createObject<Geometry>(
                    kObjectTypeGeometry);
                if (rect->colorMask != 0) {
                    base->setNumMask(rect->colorMask);
                }
                Box *b = creatBox(
                    lib->micronsToDBU(rect->xl), lib->micronsToDBU(rect->yl),
                    lib->micronsToDBU(rect->xh), lib->micronsToDBU(rect->yh));
                base->setBox(b);
                base->setType(GeometryType::kRect);
                current_lg->addGeometry(base->getId());
                break;
            }
            case lefiGeomRectIterE: {
                if (!current_lg) {
                    break;
                }
                current_lg->setType(GeometryType::kRect);
                rectIter = geometry->lefiGeometries::getRectIter(i);
                for (int i = 0; i < rectIter->xStart; i++) {
                    for (int j = 0; j < rectIter->yStart; j++) {
                        Geometry *base =
                            current_top_cell->createObject<Geometry>(
                                kObjectTypeGeometry);
                        if (rectIter->colorMask != 0)
                            base->setNumMask(rectIter->colorMask);

                        Box *b =
                            creatBox(lib->micronsToDBU(rectIter->xl +
                                                       i * rectIter->xStep),
                                     lib->micronsToDBU(rectIter->yl +
                                                       j * rectIter->yStep),
                                     lib->micronsToDBU(rectIter->xh +
                                                       i * rectIter->xStep),
                                     lib->micronsToDBU(rectIter->yh +
                                                       j * rectIter->yStep));
                        base->setBox(b);
                        base->setType(GeometryType::kRect);
                        current_lg->addGeometry(base->getId());
                    }
                }
                break;
            }
            case lefiGeomPolygonE: {
                if (!current_lg) {
                    break;
                }
                current_lg->setType(GeometryType::kPolygon);
                polygon = geometry->lefiGeometries::getPolygon(i);
                Geometry *base = current_top_cell->createObject<Geometry>(
                    kObjectTypeGeometry);
                Polygon *geoPoly = new Polygon();
                if (polygon->colorMask != 0) {
                    base->setNumMask(polygon->colorMask);
                }
                for (j = 0; j < polygon->numPoints; j++) {
                    Point *p = new Point(lib->micronsToDBU(polygon->x[j]),
                                         lib->micronsToDBU(polygon->y[j]));
                    geoPoly->addPoint(*p);
                }
                base->setPolygonID(current_lg->CreatePolygon(geoPoly));
                base->setType(GeometryType::kPolygon);
                current_lg->addGeometry(base->getId());
                break;
            }
            case lefiGeomPolygonIterE: {
                if (!current_lg) {
                    break;
                }
                polygonIter = geometry->lefiGeometries::getPolygonIter(i);
                current_lg->setType(GeometryType::kPolygon);
                for (int i = 0; i < polygonIter->xStart; i++) {
                    for (int j = 0; j < polygonIter->yStart; j++) {
                        Geometry *base =
                            current_top_cell->createObject<Geometry>(
                                kObjectTypeGeometry);
                        Polygon *geoPoly = new Polygon();
                        if (polygonIter->colorMask != 0)
                            base->setNumMask(polygonIter->colorMask);
                        for (j = 0; j < polygonIter->numPoints; j++) {
                            Point *p = new Point(
                                lib->micronsToDBU(polygonIter->x[j] +
                                                  i * polygonIter->xStep),
                                lib->micronsToDBU(polygonIter->y[j] +
                                                  j * polygonIter->yStep));
                            geoPoly->addPoint(*p);
                        }
                        base->setPolygonID(current_lg->CreatePolygon(geoPoly));
                        base->setType(GeometryType::kPolygon);
                        current_lg->addGeometry(base->getId());
                    }
                }
                break;
            }
            case lefiGeomViaE: {
                if (current_lg) {
                    if (current_lg->getType() != GeometryType::kVia) {
                        if (port) {
                            port->addLayerGeometry(current_lg->getId());
                        } else {
                            if (currentCell)
                                currentCell->addOBS(current_lg->getId());
                        }
                        LayerGeometry *geo =
                            current_top_cell->createObject<LayerGeometry>(
                                kObjectTypeLayerGeometry);
                        current_lg = geo;
                    }
                } else {
                    LayerGeometry *geo =
                        current_top_cell->createObject<LayerGeometry>(
                            kObjectTypeLayerGeometry);
                    current_lg = geo;
                }
                via = geometry->lefiGeometries::getVia(i);
                current_lg->setType(GeometryType::kVia);
                GeometryVia *v = current_top_cell->createObject<GeometryVia>(
                    kObjectTypeGeometryVia);
                v->setName(via->name);
                Point p(lib->micronsToDBU(via->x), lib->micronsToDBU(via->y));
                v->setPoint(p);
                v->setTopMaskNum(via->topMaskNum);
                v->setCutMaskNum(via->cutMaskNum);
                v->setBottomMaskNum(via->bottomMaskNum);
                current_lg->addGeometryVia(v->getId());
                break;
            }
            case lefiGeomViaIterE:
                if (current_lg) {
                    if (current_lg->getType() != GeometryType::kVia) {
                        if (port) {
                            port->addLayerGeometry(current_lg->getId());
                        } else {
                            if (currentCell)
                                currentCell->addOBS(current_lg->getId());
                        }
                        LayerGeometry *geo =
                            current_top_cell->createObject<LayerGeometry>(
                                kObjectTypeLayerGeometry);
                        current_lg = geo;
                    }
                } else {
                    LayerGeometry *geo =
                        current_top_cell->createObject<LayerGeometry>(
                            kObjectTypeLayerGeometry);
                    current_lg = geo;
                }
                viaIter = geometry->lefiGeometries::getViaIter(i);
                current_lg->setType(GeometryType::kVia);
                for (int i = 0; i < viaIter->xStart; i++) {
                    for (int j = 0; j < viaIter->yStart; j++) {
                        GeometryVia *v =
                            current_top_cell->createObject<GeometryVia>(
                                kObjectTypeGeometryVia);
                        v->setName(viaIter->name);
                        Point p(
                            lib->micronsToDBU(viaIter->x + i * viaIter->xStep),
                            lib->micronsToDBU(viaIter->y + j * viaIter->yStep));
                        v->setPoint(p);
                        v->setTopMaskNum(viaIter->topMaskNum);
                        v->setCutMaskNum(viaIter->cutMaskNum);
                        v->setBottomMaskNum(viaIter->bottomMaskNum);
                        current_lg->addGeometryVia(v->getId());
                    }
                }
                break;
            default:
                fprintf(fout, "BOGUS geometries type.\n");
                break;
        }
        // current_lg->addGeometryGroup(group);
    }
    if (!current_lg) {
        // message->info("-------current_lg do not exist------\n");
        return -1;
    }
    if (port) {
        port->addLayerGeometry(current_lg->getId());
        // message->info("will add lg num %d---------------\n",
        // port->getLayerGeometryNum());
    } else {
        if (currentCell) currentCell->addOBS(current_lg->getId());
    }
    return 0;
}

int antennaCB(lefrCallbackType_e c, double value, lefiUserData) {
    checkType(c);
    return 0;
}

int arrayBeginCB(lefrCallbackType_e c, const char *name, lefiUserData) {
    int status;

    checkType(c);
    return 0;
}

int arrayCB(lefrCallbackType_e c, lefiArray *a, lefiUserData) {
    checkType(c);
    return 0;
}

int arrayEndCB(lefrCallbackType_e c, const char *name, lefiUserData) {
    checkType(c);
    return 0;
}

int busBitCharsCB(lefrCallbackType_e c, const char *busBit, lefiUserData) {
    checkType(c);
    getTopCell()->getTechLib()->setBusBitsName(busBit);
    return 0;
}

int caseSensCB(lefrCallbackType_e c, int caseSense, lefiUserData) {
    checkType(c);
    getTopCell()->getTechLib()->setCaseSensitive((caseSense == 0) ? false
                                                                  : true);
    return 0;
}

int fixedMaskCB(lefrCallbackType_e c, int fixedMask, lefiUserData) {
    checkType(c);
    getTopCell()->getTechLib()->setHasFixedMask(fixedMask == 1);
    return 0;
}

int clearanceCB(lefrCallbackType_e c, const char *name, lefiUserData) {
    checkType(c);
    if (name == 0) {
        getTopCell()->getTechLib()->setHasClearanceMeasure(false);
    } else if (strcmp(name, "MAXXY") == 0) {
        getTopCell()->getTechLib()->setHasClearanceMeasure(true);
        getTopCell()->getTechLib()->setClearanceMeasureUseMaxxy(true);
    } else if (strcmp(name, "EUCLIDEAN") == 0) {
        getTopCell()->getTechLib()->setHasClearanceMeasure(true);
        getTopCell()->getTechLib()->setClearanceMeasureUseMaxxy(false);
    } else {
        getTopCell()->getTechLib()->setHasClearanceMeasure(false);
    }
    return 0;
}

int dividerCB(lefrCallbackType_e c, const char *name, lefiUserData) {
    checkType(c);
    getTopCell()->getTechLib()->setDividerName(name);
    return 0;
}

int noWireExtCB(lefrCallbackType_e c, const char *name, lefiUserData) {
    checkType(c);
    return 0;
}

int noiseMarCB(lefrCallbackType_e c, lefiNoiseMargin *, lefiUserData) {
    checkType(c);
    return 0;
}

int edge1CB(lefrCallbackType_e c, double name, lefiUserData) {
    checkType(c);
    return 0;
}

int edge2CB(lefrCallbackType_e c, double name, lefiUserData) {
    checkType(c);
    return 0;
}

int edgeScaleCB(lefrCallbackType_e c, double name, lefiUserData) {
    checkType(c);
    return 0;
}

int noiseTableCB(lefrCallbackType_e c, lefiNoiseTable *, lefiUserData) {
    checkType(c);
    return 0;
}

int correctionCB(lefrCallbackType_e c, lefiCorrectionTable *, lefiUserData) {
    checkType(c);
    return 0;
}

int dielectricCB(lefrCallbackType_e c, double dielectric, lefiUserData) {
    checkType(c);
    return 0;
}

int irdropBeginCB(lefrCallbackType_e c, void *, lefiUserData) {
    checkType(c);
    return 0;
}

int irdropCB(lefrCallbackType_e c, lefiIRDrop *irdrop, lefiUserData) {
    checkType(c);
    return 0;
}

int irdropEndCB(lefrCallbackType_e c, void *, lefiUserData) {
    checkType(c);
    return 0;
}

int layerCB(lefrCallbackType_e c, lefiLayer *layer, lefiUserData) {
    int i, j, k;
    int numPoints, propNum;
    double *widths, *current;
    lefiLayerDensity *density;
    lefiAntennaPWL *pwl;
    lefiSpacingTable *spTable;
    lefiInfluence *influence;
    lefiParallel *parallel;
    lefiTwoWidths *twoWidths;
    char pType;
    int numMinCut, numMinenclosed;
    lefiAntennaModel *aModel;
    lefiOrthogonal *ortho;

    checkType(c);

    lefrSetCaseSensitivity(0);

    // LEF57 rules
    layer->parse65nmRules();

    // parse information from io/lef to edi db
    readLayer(layer);

    // LEF58 information will be parsed directly to edi db
    layer->parseLEF58Layer();

    // Set it to case sensitive from here on
    lefrSetCaseSensitivity(1);

    return 0;
}

int macroBeginCB(lefrCallbackType_e c, const char *macroName, lefiUserData) {
    checkType(c);
    return 0;
}

int macroFixedMaskCB(lefrCallbackType_e c, int, lefiUserData) {
    checkType(c);

    return 0;
}

int macroClassTypeCB(lefrCallbackType_e c, const char *macroClassType,
                     lefiUserData) {
    checkType(c);
    return 0;
}

int macroOriginCB(lefrCallbackType_e c, lefiNum, lefiUserData) {
    checkType(c);
    return 0;
}

int macroSizeCB(lefrCallbackType_e c, lefiNum, lefiUserData) {
    checkType(c);
    return 0;
}
// int test_count = 0;
int macroCB(lefrCallbackType_e c, lefiMacro *macro, lefiUserData) {
    // message->info("current macro %s \n", macro->lefiMacro::name());
    lefiSitePattern *pattern;
    int propNum, i, hasPrtSym = 0;

    checkType(c);

    Cell *current_top_cell = getTopCell();
    if (!current_top_cell) return 0;
    Cell *cell;
    if (currentCell) {
        cell = currentCell;
    } else {
        cell = current_top_cell->createObject<Cell>(kObjectTypeCell);
        currentCell = cell;
    }

    std::string name = macro->lefiMacro::name();
    cell->setName(name);
    Tech *lib = getTopCell()->getTechLib();

    if (macro->lefiMacro::hasClass())
        cell->setClass(macro->lefiMacro::macroClass());
    if (macro->lefiMacro::isFixedMask()) cell->setIsFixedMask(1);
    cell->setHasEEQ(macro->lefiMacro::hasEEQ());
    if (macro->lefiMacro::hasEEQ()) cell->setEEQ(macro->lefiMacro::EEQ());
    if (macro->lefiMacro::hasXSymmetry()) {
        cell->setHasXSymmetry(true);
    }
    if (macro->lefiMacro::hasYSymmetry()) {  // print X Y & R90 in one line
        cell->setHasYSymmetry(true);
    }
    if (macro->lefiMacro::has90Symmetry()) {
        cell->setHas90Symmetry(true);
    }

    cell->setHasSiteName(macro->lefiMacro::hasSiteName());
    if (macro->lefiMacro::hasSiteName()) {
        Site* site = lib->getSiteByName(macro->lefiMacro::siteName());
        if (site) {
            cell->setSiteID(site->getId());
        }
    }
        
    // fprintf(fout, "  SITE %s ;\n", macro->lefiMacro::siteName());
    if (macro->lefiMacro::hasSitePattern()) {
        //cell->setNumSites(macro->lefiMacro::numSitePattern());
        for (i = 0; i < macro->lefiMacro::numSitePattern(); i++) {
            SitePattern *siteP = current_top_cell->createObject<SitePattern>(
                kObjectTypeCellSitePattern);
            pattern = macro->lefiMacro::sitePattern(i);
            Site* site = lib->getSiteByName(pattern->lefiSitePattern::name());
            if (site) {
                siteP->setSiteID(site->getId());
            }
            // message->info("  -----sitePattern name %s ;-----\n",
            // siteP->getName());
            siteP->setxOrigin(lib->micronsToDBU(pattern->lefiSitePattern::x()));
            siteP->setyOrigin(lib->micronsToDBU(pattern->lefiSitePattern::y()));
            siteP->setOrient(
                orientTransfer(pattern->lefiSitePattern::orient()));
            if (pattern->lefiSitePattern::hasStepPattern()) {
                StepPattern *stepP = new StepPattern(
                    pattern->lefiSitePattern::xStart(),
                    pattern->lefiSitePattern::yStart(),
                    lib->micronsToDBU(pattern->lefiSitePattern::xStep()),
                    lib->micronsToDBU(pattern->lefiSitePattern::yStep()));
                siteP->setStepPattern(stepP);
            }
            cell->addSitePattern(siteP->getId());
        }
    }

    cell->setHasSize(macro->lefiMacro::hasSize());
    if (macro->lefiMacro::hasSize()) {
        cell->setSizeX(lib->micronsToDBU(macro->lefiMacro::sizeX()));
        cell->setSizeY(lib->micronsToDBU(macro->lefiMacro::sizeY()));
    }

    if (macro->lefiMacro::hasForeign()) {
        //cell->setNumForeigns(macro->lefiMacro::numForeigns());
        for (i = 0; i < macro->lefiMacro::numForeigns(); i++) {
            Foreign *f =
                current_top_cell->createObject<Foreign>(kObjectTypeForeign);
            f->sethasForeignPoint(macro->lefiMacro::hasForeignPoint(i));
            f->setforeignOrient(
                orientTransfer(macro->lefiMacro::foreignOrient(i)));
            f->setforeignX(lib->micronsToDBU(macro->lefiMacro::foreignX(i)));
            f->setforeignY(lib->micronsToDBU(macro->lefiMacro::foreignY(i)));
            f->setName(macro->lefiMacro::foreignName(i));

            cell->addForeign(f->getId());
        }
    }
    cell->setHasOrigin(macro->lefiMacro::hasOrigin());
    if (macro->lefiMacro::hasOrigin()) {
        cell->setOriginX(lib->micronsToDBU(macro->lefiMacro::originX()));
        cell->setOriginY(lib->micronsToDBU(macro->lefiMacro::originX()));
    }

    // cell->print();
    current_top_cell->addCell(cell->getId());
    // test_count++;
    // message->info("add %dth macro to topcell\n", test_count);
    // cell->print();
    currentCell = nullptr;
    return 0;
}

int macroEndCB(lefrCallbackType_e c, const char *macroName, lefiUserData) {
    checkType(c);
    return 0;
}

int manufacturingCB(lefrCallbackType_e c, double num, lefiUserData) {
    checkType(c);
    Tech *lib = getTopCell()->getTechLib();
    // Based on the recommended behavior,
    // when manufacture-grid is smaller than db-micron,
    // the db-micron should be changed.
    Units *units = lib->getUnits();
    if (units) {
        UInt32 dbu = units->getLengthFactor();
        double dbu_based_grid = (1.0 / num);
        if ((UInt32)dbu_based_grid > dbu) {
            units->setLengthFactor((UInt32)dbu_based_grid);
        }
    }
    lib->setManuGrids(lib->micronsToDBU(num));
    return 0;
}

int maxStackViaCB(lefrCallbackType_e c, lefiMaxStackVia *maxStack,
                  lefiUserData) {
    checkType(c);
    MaxViaStack *mvs = getTopCell()->createObject<MaxViaStack>(kObjectTypeMaxViaStack);
    mvs->setNumStackedVia(maxStack->lefiMaxStackVia::maxStackVia());
    if (maxStack->lefiMaxStackVia::hasMaxStackViaRange()) {
        int top_id = getTopCell()->getTechLib()->getLayerLEFIndexByName(
            maxStack->lefiMaxStackVia::maxStackViaTopLayer());
        mvs->setTopLayerId(top_id);
        int bot_id = getTopCell()->getTechLib()->getLayerLEFIndexByName(
            maxStack->lefiMaxStackVia::maxStackViaBottomLayer());
        mvs->setBotLayerId(bot_id);
        if (top_id != -1 && bot_id != -1) {
            mvs->setIsRange(true);
        }
    }
    getTopCell()->getTechLib()->setMaxViaStack(mvs->getId());
    return 0;
}

int minFeatureCB(lefrCallbackType_e c, lefiMinFeature *min, lefiUserData) {
    checkType(c);
    return 0;
}

int nonDefaultCB(lefrCallbackType_e c, lefiNonDefault *def, lefiUserData) {
    int i;
    lefiVia *via;
    lefiSpacing *spacing;

    checkType(c);
    readNonDefaultRule(def);

    return 0;
}

int obstructionCB(lefrCallbackType_e c, lefiObstruction *obs, lefiUserData) {
    lefiGeometries *geometry;

    if (!currentCell) {
        Cell *current_top_cell = getTopCell();
        if (!current_top_cell) return 0;
        currentCell = current_top_cell->createObject<Cell>(kObjectTypeCell);
    }
    geometry = obs->lefiObstruction::geometries();
    prtGeometry(geometry);

    checkType(c);
    return 0;
}

int pinCB(lefrCallbackType_e c, lefiPin *pin, lefiUserData) {
    int numPorts, i, j;
    lefiGeometries *geometry;
    lefiPinAntennaModel *aModel;

    Cell *current_top_cell = getTopCell();
    if (!current_top_cell) return 0;
    if (!currentCell) {
        currentCell = current_top_cell->createObject<Cell>(kObjectTypeCell);
    }

    Term *term = current_top_cell->createObject<Term>(kObjectTypeTerm);
    checkType(c);
    Tech *lib = getTopCell()->getTechLib();
    term->setName(pin->lefiPin::name());
    if (pin->lefiPin::hasDirection())
        term->setDirection(pin->lefiPin::direction());
    if (pin->lefiPin::hasUse()) term->setUse(pin->lefiPin::use());
    if (pin->lefiPin::hasShape()) term->setShape(pin->lefiPin::shape());
    if (pin->lefiPin::hasMustjoin())
        term->setMustjoin(pin->lefiPin::mustjoin());
    if (pin->lefiPin::hasTaperRule())
        term->setTaperRule(pin->lefiPin::taperRule());
    if (pin->lefiPin::hasNetExpr()) term->setNetExpr(pin->lefiPin::netExpr());
    if (pin->lefiPin::hasSupplySensitivity())
        term->setSupplySensitivity(pin->lefiPin::supplySensitivity());
    if (pin->lefiPin::hasGroundSensitivity())
        term->setGroundSensitivity(pin->lefiPin::groundSensitivity());

    if (pin->lefiPin::hasAntennaPartialMetalArea()) {
        for (i = 0; i < pin->lefiPin::numAntennaPartialMetalArea(); i++) {
            AntennaArea *ap = current_top_cell->createObject<AntennaArea>(
                kObjectTypePinAntennaArea);
            ap->setArea(lib->areaMicronsToDBU(
                pin->lefiPin::antennaPartialMetalArea(i)));
            if (pin->lefiPin::antennaPartialMetalAreaLayer(i))
                ap->setLayerName(pin->lefiPin::antennaPartialMetalAreaLayer(i));

            term->addAntennaPartialMetalArea(ap->getId());
        }
    }
    if (pin->lefiPin::hasAntennaPartialMetalSideArea()) {
        for (i = 0; i < pin->lefiPin::numAntennaPartialMetalSideArea(); i++) {
            AntennaArea *ap = current_top_cell->createObject<AntennaArea>(
                kObjectTypePinAntennaArea);
            ap->setArea(lib->areaMicronsToDBU(
                pin->lefiPin::antennaPartialMetalSideArea(i)));
            if (pin->lefiPin::antennaPartialMetalSideAreaLayer(i))
                ap->setLayerName(
                    pin->lefiPin::antennaPartialMetalSideAreaLayer(i));

            term->addAntennaPartialMetalSideArea(ap->getId());
        }
    }

    if (pin->lefiPin::hasAntennaPartialCutArea()) {
        for (i = 0; i < pin->lefiPin::numAntennaPartialCutArea(); i++) {
            AntennaArea *ap = current_top_cell->createObject<AntennaArea>(
                kObjectTypePinAntennaArea);
            ap->setArea(
                lib->areaMicronsToDBU(pin->lefiPin::antennaPartialCutArea(i)));
            if (pin->lefiPin::antennaPartialCutAreaLayer(i))
                ap->setLayerName(pin->lefiPin::antennaPartialCutAreaLayer(i));

            term->addAntennaPartialCutArea(ap->getId());
        }
    }

    if (pin->lefiPin::hasAntennaDiffArea()) {
        for (i = 0; i < pin->lefiPin::numAntennaDiffArea(); i++) {
            AntennaArea *ap = current_top_cell->createObject<AntennaArea>(
                kObjectTypePinAntennaArea);
            ap->setArea(
                lib->areaMicronsToDBU(pin->lefiPin::antennaDiffArea(i)));
            if (pin->lefiPin::antennaDiffAreaLayer(i))
                ap->setLayerName(pin->lefiPin::antennaDiffAreaLayer(i));

            term->addAntennaDiffArea(ap->getId());
        }
    }

    for (j = 0; j < pin->lefiPin::numAntennaModel(); j++) {
        aModel = pin->lefiPin::antennaModel(j);
        AntennaModelTerm *am = new AntennaModelTerm();
        std::string oxide_string = aModel->lefiPinAntennaModel::antennaOxide();
        oxide_string = oxide_string.substr(5);
        int oxide = stoi(oxide_string);
        if (oxide < 1 || oxide > 4)
            message->issueMsg(kError, "oxide %d out of range.\n", oxide);
        // am->setOxide(aModel->lefiPinAntennaModel::antennaOxide());

        if (aModel->lefiPinAntennaModel::hasAntennaGateArea()) {
            for (i = 0; i < aModel->lefiPinAntennaModel::numAntennaGateArea();
                 i++) {
                AntennaArea *ap = current_top_cell->createObject<AntennaArea>(
                    kObjectTypePinAntennaArea);
                ap->setArea(lib->areaMicronsToDBU(
                    aModel->lefiPinAntennaModel::antennaGateArea(i)));
                if (aModel->lefiPinAntennaModel::antennaGateAreaLayer(i))
                    ap->setLayerName(
                        aModel->lefiPinAntennaModel::antennaGateAreaLayer(i));

                am->addAntennaGateArea(ap->getId());
            }
        }

        if (aModel->lefiPinAntennaModel::hasAntennaMaxAreaCar()) {
            for (i = 0; i < aModel->lefiPinAntennaModel::numAntennaMaxAreaCar();
                 i++) {
                AntennaArea *ap = current_top_cell->createObject<AntennaArea>(
                    kObjectTypePinAntennaArea);
                ap->setArea(lib->areaMicronsToDBU(
                    aModel->lefiPinAntennaModel::antennaMaxAreaCar(i)));
                if (aModel->lefiPinAntennaModel::antennaMaxAreaCarLayer(i))
                    ap->setLayerName(
                        aModel->lefiPinAntennaModel::antennaMaxAreaCarLayer(i));

                am->addAntennaMaxAreaCar(ap->getId());
            }
        }

        if (aModel->lefiPinAntennaModel::hasAntennaMaxSideAreaCar()) {
            for (i = 0;
                 i < aModel->lefiPinAntennaModel::numAntennaMaxSideAreaCar();
                 i++) {
                AntennaArea *ap = current_top_cell->createObject<AntennaArea>(
                    kObjectTypePinAntennaArea);
                ap->setArea(lib->areaMicronsToDBU(
                    aModel->lefiPinAntennaModel::antennaMaxSideAreaCar(i)));
                if (aModel->lefiPinAntennaModel::antennaMaxSideAreaCarLayer(i))
                    ap->setLayerName(
                        aModel->lefiPinAntennaModel::antennaMaxSideAreaCarLayer(
                            i));

                am->addAntennaMaxSideAreaCar(ap->getId());
            }
        }

        if (aModel->lefiPinAntennaModel::hasAntennaMaxCutCar()) {
            for (i = 0; i < aModel->lefiPinAntennaModel::numAntennaMaxCutCar();
                 i++) {
                AntennaArea *ap = current_top_cell->createObject<AntennaArea>(
                    kObjectTypePinAntennaArea);
                ap->setArea(lib->areaMicronsToDBU(
                    aModel->lefiPinAntennaModel::antennaMaxCutCar(i)));
                if (aModel->lefiPinAntennaModel::antennaMaxCutCarLayer(i))
                    ap->setLayerName(
                        aModel->lefiPinAntennaModel::antennaMaxCutCarLayer(i));

                am->addAntennaMaxCutCar(ap->getId());
            }
        }
        term->addAntennaModelTerm(oxide, am);
    }

    numPorts = pin->lefiPin::numPorts();
    for (i = 0; i < numPorts; i++) {
        geometry = pin->lefiPin::port(i);
        int numItems = geometry->lefiGeometries::numItems();
        if (numItems <= 0) {
            continue;
        }
        Port *p = current_top_cell->createObject<Port>(kObjectTypePort);
        for (int j = 0; j < numItems; j++) {
            if (lefiGeomClassE == geometry->lefiGeometries::itemType(j)) {
                p->setClass(geometry->lefiGeometries::getClass(j));
                break;
            }
        }
        prtGeometry(geometry, p);
        term->addPort(p->getId());
    }
    if (currentCell)
        currentCell->addTerm(term->getId());
    else
        message->info(
            "----------------pin %s do not find macro ---------------------\n",
            term->getName().c_str());

    return 0;
}

int densityCB(lefrCallbackType_e c, lefiDensity *density, lefiUserData) {
    struct lefiGeomRect rect;
    Tech *lib = getTopCell()->getTechLib();

    checkType(c);
    Cell *current_top_cell = getTopCell();
    if (!currentCell) {
        currentCell = current_top_cell->createObject<Cell>(kObjectTypeCell);
    }
    if (!current_top_cell) return 0;
    for (int i = 0; i < density->lefiDensity::numLayer(); i++) {
        Density *den =
            current_top_cell->createObject<Density>(kObjectTypeDensity);
        den->setName(density->lefiDensity::layerName(i));
        for (int j = 0; j < density->lefiDensity::numRects(i); j++) {
            DensityLayer *layer = current_top_cell->createObject<DensityLayer>(
                kObjectTypeDensityLayer);
            rect = density->lefiDensity::getRect(i, j);
            Box *box = creatBox(
                lib->micronsToDBU(rect.xl), lib->micronsToDBU(rect.yl),
                lib->micronsToDBU(rect.xh), lib->micronsToDBU(rect.yh));
            layer->setRect(*box);
            layer->setDensity(density->lefiDensity::densityValue(i, j));
            den->addDensityLayer(layer->getId());
        }
        if (currentCell)
            currentCell->addDensity(den->getId());
        else
            message->info(
                "----------------density do not find macro----------------\n");
    }

    return 0;
}

int propDefBeginCB(lefrCallbackType_e c, void *, lefiUserData) {
    checkType(c);
    return 0;
}

int propDefCB(lefrCallbackType_e c, lefiProp *prop, lefiUserData) {
    checkType(c);
    readPropertyDefinition(prop);
    return 0;
}

int propDefEndCB(lefrCallbackType_e c, void *, lefiUserData) {
    checkType(c);
    return 0;
}

int siteCB(lefrCallbackType_e c, lefiSite *site, lefiUserData) {
    checkType(c);
    readSite(site);
    return 0;
}

int spacingBeginCB(lefrCallbackType_e c, void *, lefiUserData) {
    checkType(c);
    return 0;
}

int spacingCB(lefrCallbackType_e c, lefiSpacing *spacing, lefiUserData) {
    checkType(c);
    return 0;
}

int spacingEndCB(lefrCallbackType_e c, void *, lefiUserData) {
    checkType(c);
    return 0;
}

int timingCB(lefrCallbackType_e c, lefiTiming *timing, lefiUserData) {
    int i;
    checkType(c);
    return 0;
}

int unitsCB(lefrCallbackType_e c, lefiUnits *unit, lefiUserData) {
    checkType(c);
    readUnit(unit);
    return 0;
}

int useMinSpacingCB(lefrCallbackType_e c, lefiUseMinSpacing *spacing,
                    lefiUserData) {
    checkType(c);
    if (strcasecmp(spacing->lefiUseMinSpacing::name(), "OBS")) {
        getTopCell()->getTechLib()->setHasOBSUseMinSpacing(true);
        if (spacing->lefiUseMinSpacing::value()) {
            getTopCell()->getTechLib()->setOBSUseMinSpacing(true);
        } else {
            getTopCell()->getTechLib()->setOBSUseMinSpacing(false);
        }
    } else if (strcasecmp(spacing->lefiUseMinSpacing::name(), "PIN")) {
        getTopCell()->getTechLib()->setHasPinUseMinSpacing(true);
        if (spacing->lefiUseMinSpacing::value()) {
            getTopCell()->getTechLib()->setPinUseMinSpacing(true);
        } else {
            getTopCell()->getTechLib()->setPinUseMinSpacing(false);
        }
    }
    return 0;
}

int versionCB(lefrCallbackType_e c, double num, lefiUserData) {
    checkType(c);
    getTopCell()->getTechLib()->setVersion(num);
    return 0;
}

int versionStrCB(lefrCallbackType_e c, const char *versionName, lefiUserData) {
    checkType(c);
    double version = strtod(versionName, NULL);

    getTopCell()->getTechLib()->setVersion(version);
    return 0;
}

int viaCB(lefrCallbackType_e c, lefiVia *via, lefiUserData) {
    checkType(c);
    // parse lef/in via info to db
    readViaMaster(via);
    return 0;
}

int viaRuleCB(lefrCallbackType_e c, lefiViaRule *viaRule, lefiUserData) {
    int numLayers, numVias, i;
    lefiViaRuleLayer *vLayer;
    readViaRule(viaRule);
    checkType(c);
    return 0;
}

int extensionCB(lefrCallbackType_e c, const char *extsn, lefiUserData) {
    checkType(c);
    std::string extensions = "BEGINEXT ";
    extensions.append(extsn);
    getTopCell()->getTechLib()->setExtensionsName(extensions.c_str());
    return 0;
}

int doneCB(lefrCallbackType_e c, void *, lefiUserData) {
    checkType(c);
    return 0;
}

void errorCB(const char *msg) {
    printf("%s : %s\n", (const char *)lefrGetUserData(), msg);
}

void warningCB(const char *msg) {
    printf("%s : %s\n", (const char *)lefrGetUserData(), msg);
}

void *mallocCB(int size) { return malloc(size); }

void *reallocCB(void *name, int size) { return realloc(name, size); }

void freeCB(void *name) {
    free(name);
    return;
}

void lineNumberCB(int lineNo) { return; }

void printWarning(const char *str) { fprintf(stderr, "%s\n", str); }

int readLef(int argc, const char **argv) {
    char *inFile[100];
    char *outFile;
    FILE *f;
    int res;
    int num;
    int status;
    int retStr = 0;
    int numInFile = 0;
    int fileCt = 0;
    const char *version = "N/A";
    int setVer = 0;
    char *userData;
    int verbose = 0;

    userData = strdup("(lefrw-5100)");
    snprintf(defaultName, sizeof(defaultName), "lef.in");
    snprintf(defaultOut, sizeof(defaultOut), "list");
    inFile[0] = defaultName;
    outFile = defaultOut;
    fout = stdout;
    bool is_dump = false;

#if (defined WIN32 && _MSC_VER < 1800)
    // Enable two-digit exponent format
    _set_output_format(_TWO_DIGIT_EXPONENT);
#endif

    argc--;
    argv++;
    while (argc--) {
        if (strcmp(*argv, "-dump") == 0) {
            is_dump = true;
        } else if (strcmp(*argv, "-d") == 0) {
            argv++;
            argc--;
            sscanf(*argv, "%d", &num);
            lefiSetDebug(num, 1);
        } else if (strcmp(*argv, "-verStr") == 0) {
            /* New to set the version callback routine to return a string    */
            /* instead of double.                                            */
            retStr = 1;
        } else if (strcmp(*argv, "-ver") == 0) {
            argv++;
            argc--;
            setVer = 1;
            version = *(const_cast<char **>(argv));
        } else if (strcmp(*argv, "-sessionless") == 0) {
            isSessionles = 1;
        } else if (argv[0][0] != '-') {
            if (numInFile >= 100) {
                fprintf(stderr, "ERROR: too many input files, max = 3.\n");
                return 2;
            }
            inFile[numInFile++] = *(const_cast<char **>(argv));
        } else {
            fprintf(stderr, "ERROR: Illegal command line option: '%s'\n",
                    *argv);
            return 2;
        }

        argv++;
    }

    // sets the parser to be case sensitive...
    // default was supposed to be the case but false...
    // lefrSetCaseSensitivity(true);
    if (isSessionles) {
        lefrSetOpenLogFileAppend();
    }

    lefrInitSession(isSessionles ? 0 : 1);

    // lefrSetWarningLogFunction(printWarning);
    lefrSetAntennaInputCbk(antennaCB);
    lefrSetAntennaInoutCbk(antennaCB);
    lefrSetAntennaOutputCbk(antennaCB);
    lefrSetArrayBeginCbk(arrayBeginCB);
    lefrSetArrayCbk(arrayCB);
    lefrSetArrayEndCbk(arrayEndCB);
    lefrSetBusBitCharsCbk(busBitCharsCB);
    lefrSetCaseSensitiveCbk(caseSensCB);
    lefrSetFixedMaskCbk(fixedMaskCB);
    lefrSetClearanceMeasureCbk(clearanceCB);
    lefrSetDensityCbk(densityCB);
    lefrSetDividerCharCbk(dividerCB);
    lefrSetNoWireExtensionCbk(noWireExtCB);
    lefrSetNoiseMarginCbk(noiseMarCB);
    lefrSetEdgeRateThreshold1Cbk(edge1CB);
    lefrSetEdgeRateThreshold2Cbk(edge2CB);
    lefrSetEdgeRateScaleFactorCbk(edgeScaleCB);
    lefrSetExtensionCbk(extensionCB);
    lefrSetNoiseTableCbk(noiseTableCB);
    lefrSetCorrectionTableCbk(correctionCB);
    lefrSetDielectricCbk(dielectricCB);
    lefrSetIRDropBeginCbk(irdropBeginCB);
    lefrSetIRDropCbk(irdropCB);
    lefrSetIRDropEndCbk(irdropEndCB);
    lefrSetLayerCbk(layerCB);
    lefrSetLibraryEndCbk(doneCB);
    lefrSetMacroBeginCbk(macroBeginCB);
    lefrSetMacroCbk(macroCB);
    lefrSetMacroClassTypeCbk(macroClassTypeCB);
    lefrSetMacroOriginCbk(macroOriginCB);
    lefrSetMacroSizeCbk(macroSizeCB);
    lefrSetMacroFixedMaskCbk(macroFixedMaskCB);
    lefrSetMacroEndCbk(macroEndCB);
    lefrSetManufacturingCbk(manufacturingCB);
    lefrSetMaxStackViaCbk(maxStackViaCB);
    lefrSetMinFeatureCbk(minFeatureCB);
    lefrSetNonDefaultCbk(nonDefaultCB);
    lefrSetObstructionCbk(obstructionCB);
    lefrSetPinCbk(pinCB);
    lefrSetPropBeginCbk(propDefBeginCB);
    lefrSetPropCbk(propDefCB);
    lefrSetPropEndCbk(propDefEndCB);
    lefrSetSiteCbk(siteCB);
    lefrSetSpacingBeginCbk(spacingBeginCB);
    lefrSetSpacingCbk(spacingCB);
    lefrSetSpacingEndCbk(spacingEndCB);
    lefrSetTimingCbk(timingCB);
    lefrSetUnitsCbk(unitsCB);
    lefrSetUseMinSpacingCbk(useMinSpacingCB);
    lefrSetUserData(reinterpret_cast<void *>(3));
    if (!retStr)
        lefrSetVersionCbk(versionCB);
    else
        lefrSetVersionStrCbk(versionStrCB);
    lefrSetViaCbk(viaCB);
    lefrSetViaRuleCbk(viaRuleCB);
    lefrSetInputAntennaCbk(antennaCB);
    lefrSetOutputAntennaCbk(antennaCB);
    lefrSetInoutAntennaCbk(antennaCB);

    lefrSetMallocFunction(mallocCB);
    lefrSetReallocFunction(reallocCB);
    lefrSetFreeFunction(freeCB);

    lefrSetLineNumberFunction(lineNumberCB);
    lefrSetDeltaNumberLines(50);

    lefrSetRegisterUnusedCallbacks();

    if (setVer) (void)lefrSetVersionValue(version);

    lefrSetAntennaInoutWarnings(30);
    lefrSetAntennaInputWarnings(30);
    lefrSetAntennaOutputWarnings(30);
    lefrSetArrayWarnings(30);
    lefrSetCaseSensitiveWarnings(30);
    lefrSetCorrectionTableWarnings(30);
    lefrSetDielectricWarnings(30);
    lefrSetEdgeRateThreshold1Warnings(30);
    lefrSetEdgeRateThreshold2Warnings(30);
    lefrSetEdgeRateScaleFactorWarnings(30);
    lefrSetInoutAntennaWarnings(30);
    lefrSetInputAntennaWarnings(30);
    lefrSetIRDropWarnings(30);
    lefrSetLayerWarnings(30);
    lefrSetMacroWarnings(30);
    lefrSetMaxStackViaWarnings(30);
    lefrSetMinFeatureWarnings(30);
    lefrSetNoiseMarginWarnings(30);
    lefrSetNoiseTableWarnings(30);
    lefrSetNonDefaultWarnings(30);
    lefrSetNoWireExtensionWarnings(30);
    lefrSetOutputAntennaWarnings(30);
    lefrSetPinWarnings(30);
    lefrSetSiteWarnings(30);
    lefrSetSpacingWarnings(30);
    lefrSetTimingWarnings(30);
    lefrSetUnitsWarnings(30);
    lefrSetUseMinSpacingWarnings(30);
    lefrSetViaRuleWarnings(30);
    lefrSetViaWarnings(30);

    (void)
        lefrSetShiftCase();  // will shift name to uppercase if caseinsensitive
    // is set to off or not set
    if (!isSessionles) {
        lefrSetOpenLogFileAppend();
    }

    message->info("\nReading LEF\n");
    fflush(stdout);
    for (fileCt = 0; fileCt < numInFile; fileCt++) {
        lefrReset();

        if ((f = fopen(inFile[fileCt], "r")) == 0) {
            fprintf(stderr, "Couldn't open input file '%s'\n", inFile[fileCt]);
            return (2);
        }

        (void)lefrEnableReadEncrypted();

        res = lefrRead(f, inFile[fileCt], reinterpret_cast<void *>(userData));

        if (res) fprintf(stderr, "Reader returns bad status.\n");

        (void)lefrReleaseNResetMemory();
    }
    (void)lefrUnsetCallbacks();

    // Unset all the callbacks
    void lefrUnsetAntennaInputCbk();
    void lefrUnsetAntennaInoutCbk();
    void lefrUnsetAntennaOutputCbk();
    void lefrUnsetArrayBeginCbk();
    void lefrUnsetArrayCbk();
    void lefrUnsetArrayEndCbk();
    void lefrUnsetBusBitCharsCbk();
    void lefrUnsetCaseSensitiveCbk();
    void lefrUnsetFixedMaskCbk();
    void lefrUnsetClearanceMeasureCbk();
    void lefrUnsetCorrectionTableCbk();
    void lefrUnsetDensityCbk();
    void lefrUnsetDielectricCbk();
    void lefrUnsetDividerCharCbk();
    void lefrUnsetEdgeRateScaleFactorCbk();
    void lefrUnsetEdgeRateThreshold1Cbk();
    void lefrUnsetEdgeRateThreshold2Cbk();
    void lefrUnsetExtensionCbk();
    void lefrUnsetInoutAntennaCbk();
    void lefrUnsetInputAntennaCbk();
    void lefrUnsetIRDropBeginCbk();
    void lefrUnsetIRDropCbk();
    void lefrUnsetIRDropEndCbk();
    void lefrUnsetLayerCbk();
    void lefrUnsetLibraryEndCbk();
    void lefrUnsetMacroBeginCbk();
    void lefrUnsetMacroCbk();
    void lefrUnsetMacroClassTypeCbk();
    void lefrUnsetMacroEndCbk();
    void lefrUnsetMacroOriginCbk();
    void lefrUnsetMacroSizeCbk();
    void lefrUnsetManufacturingCbk();
    void lefrUnsetMaxStackViaCbk();
    void lefrUnsetMinFeatureCbk();
    void lefrUnsetNoiseMarginCbk();
    void lefrUnsetNoiseTableCbk();
    void lefrUnsetNonDefaultCbk();
    void lefrUnsetNoWireExtensionCbk();
    void lefrUnsetObstructionCbk();
    void lefrUnsetOutputAntennaCbk();
    void lefrUnsetPinCbk();
    void lefrUnsetPropBeginCbk();
    void lefrUnsetPropCbk();
    void lefrUnsetPropEndCbk();
    void lefrUnsetSiteCbk();
    void lefrUnsetSpacingBeginCbk();
    void lefrUnsetSpacingCbk();
    void lefrUnsetSpacingEndCbk();
    void lefrUnsetTimingCbk();
    void lefrUnsetUseMinSpacingCbk();
    void lefrUnsetUnitsCbk();
    void lefrUnsetVersionCbk();
    void lefrUnsetVersionStrCbk();
    void lefrUnsetViaCbk();
    void lefrUnsetViaRuleCbk();

    lefrClear();

    if (is_dump) {
        ExportTechLef dump;
        dump.exportAll();
    }
    message->info("\nRead LEF successfully.\n");

    return 0;
}

/**
 * @brief
 * set MINENCLOSEDAREA rule
 *
 * @param io_layer
 * @param rl
 */
static void setMetalLayerMinEnclArea(lefiLayer *io_layer,
                                     RoutingLayerRule *rl) {
    UInt32 min_encl_area_num = io_layer->numMinenclosedarea();
    if (min_encl_area_num) {
        Tech *lib = getTopCell()->getTechLib();
        MinEnclArea *head_mea, *tail_mea;
        head_mea = tail_mea = nullptr;

        for (UInt32 ii = 0; ii < min_encl_area_num; ++ii) {
            MinEnclArea *mea = new MinEnclArea;
            mea->setArea(static_cast<UInt32>(
                lib->areaMicronsToDBU(io_layer->minenclosedarea(ii)) + 0.5));
            if (io_layer->hasMinenclosedareaWidth(ii)) {
                mea->setWidth(
                    lib->micronsToDBU(io_layer->minenclosedareaWidth(ii)));
            }
            if (!head_mea) head_mea = mea;
            if (tail_mea) tail_mea->setNext(mea);
            tail_mea = mea;
        }
        rl->setMinEnclAreaList(head_mea);
    }
}

/**
 * @brief
 * set SPACING rule
 *
 * @param io_layer
 * @param rl
 */
static void setMetalLayerSpacing(lefiLayer *io_layer, RoutingLayerRule *rl) {
    if (io_layer->hasSpacingNumber()) {
        Tech *lib = getTopCell()->getTechLib();
        RoutingSpacing *head_rs, *tail_rs;
        head_rs = tail_rs = nullptr;
        UInt32 spacing_num = io_layer->numSpacing();

        for (UInt32 ii = 0; ii < spacing_num; ++ii) {
            RoutingSpacing *rs = new RoutingSpacing;
            // SPACING minSpacing
            UInt32 spacing = lib->micronsToDBU(io_layer->spacing(ii));
            rs->setMinSpacing(spacing);
            if (io_layer->hasSpacingRange(ii)) {  // SPACING RANGE
                rs->setIsRange(true);
                rs->setRangeMinWidth(
                    lib->micronsToDBU(io_layer->spacingRangeMin(ii)));
                rs->setRangeMaxWidth(
                    lib->micronsToDBU(io_layer->spacingRangeMax(ii)));
                if (io_layer->hasSpacingRangeUseLengthThreshold(
                        ii)) {  // SPACING RANGE USELENGTHRESHOLD
                    rs->setIsRangeUseLenghthThres(true);
                } else if (io_layer->hasSpacingRangeInfluence(
                               ii)) {  // SPACING RANGE INFLUENCE
                    UInt32 inf_val =
                        lib->micronsToDBU(io_layer->spacingRangeInfluence(ii));
                    if (inf_val == 0) {
                        delete rs;
                        continue;
                    }
                    rs->setIsRangeInfluence(true);
                    rs->setRangeInfluenceValue(inf_val);
                    if (io_layer->hasSpacingRangeInfluenceRange(ii)) {
                        rs->setIsRangeInfluenceRange(true);
                        UInt32 inf_min = lib->micronsToDBU(
                            io_layer->spacingRangeInfluenceMin(ii));
                        UInt32 inf_max = lib->micronsToDBU(
                            io_layer->spacingRangeInfluenceMax(ii));
                        rs->setRangeInfluenceMinWidth(inf_min);
                        rs->setRangeInfluenceMaxWidth(inf_max);
                    }
                } else if (io_layer->hasSpacingRangeRange(
                               ii)) {  // SPACING RANGE RANGE
                    rs->setIsRangeRange(true);
                    rs->setRangeRangeMinWidth(
                        lib->micronsToDBU(io_layer->spacingRangeRangeMin(ii)));
                    rs->setRangeRangeMaxWidth(
                        lib->micronsToDBU(io_layer->spacingRangeRangeMax(ii)));
                }
            } else if (io_layer->spacingLengthThreshold(
                           ii)) {  // SPACING LENGTHTHRESHOLD
                rs->setIsLengthThreshold(true);
                rs->setLenThresMaxLength(
                    lib->micronsToDBU(io_layer->spacingLengthThreshold(ii)));
                if (io_layer->hasSpacingLengthThresholdRange(ii)) {
                    rs->setLenThresMinWidth(lib->micronsToDBU(
                        io_layer->spacingLengthThresholdRangeMin(ii)));
                    rs->setLenThresMaxWidth(lib->micronsToDBU(
                        io_layer->spacingLengthThresholdRangeMax(ii)));
                } else {
                    rs->setLenThresMinWidth(0);
                    rs->setLenThresMaxWidth(0);
                }
            } else if (io_layer->hasSpacingEndOfLine(
                           ii)) {  // SPACING ENDOFLINE  lef5.7
                rs->setIsEndOfLine(true);
                rs->setEOLWidth(
                    lib->micronsToDBU(io_layer->spacingEolWidth(ii)));
                rs->setEOLWithin(
                    lib->micronsToDBU(io_layer->spacingEolWithin(ii)));
                if (io_layer->hasSpacingParellelEdge(ii)) {
                    rs->setEOLPRLEdgeSpace(
                        lib->micronsToDBU(io_layer->spacingParSpace(ii)));
                    rs->setEOLPRLEdgeWithin(
                        lib->micronsToDBU(io_layer->spacingParWithin(ii)));
                    rs->setIsEOLTwoEdges(io_layer->hasSpacingTwoEdges(ii));
                }
            } else if (io_layer->hasSpacingSamenet(
                           ii)) {  // SPACING SAMENET PGONLY
                rs->setIsSameNet(true);
                rs->setIsSameNetPGOnly(io_layer->hasSpacingSamenetPGonly(ii));
            } else if (io_layer->hasSpacingNotchLength(
                           ii)) {  // SPACING NOTCHLENGTH  lef5.7
                rs->setIsNotchLength(true);
                rs->setNotchLengthMinNotchLength(
                    lib->micronsToDBU(io_layer->spacingNotchLength(ii)));
            } else if (io_layer->hasSpacingEndOfNotchWidth(
                           ii)) {  // SPACING ENDOFNOTCHWIDTH
                rs->setIsEndOfNotchWidth(true);
                rs->setEONWidth(
                    lib->micronsToDBU(io_layer->spacingEndOfNotchWidth(ii)));
                rs->setEONSpacing(
                    lib->micronsToDBU(io_layer->spacingEndOfNotchSpacing(ii)));
                rs->setEONLength(
                    lib->micronsToDBU(io_layer->spacingEndOfNotchLength(ii)));
            }

            if (!head_rs) head_rs = rs;
            if (tail_rs) tail_rs->setNext(rs);
            tail_rs = rs;
        }

        rl->setSpacingList(head_rs);
    }
}

/**
 * @brief
 * set MINIMUMCUT rules
 *
 * @param io_layer
 * @param rl
 */
static void setMetalLayerMinCutRules(lefiLayer *io_layer,
                                     RoutingLayerRule *rl) {
    UInt32 min_cut_rules_num = io_layer->numMinimumcut();
    if (min_cut_rules_num) {
        Tech *lib = getTopCell()->getTechLib();
        MinCut *head_mc, *tail_mc;
        head_mc = tail_mc = nullptr;
        for (UInt32 ii = 0; ii < min_cut_rules_num; ++ii) {
            MinCut *mc = new MinCut;
            mc->setNumCuts(io_layer->minimumcut(ii));
            mc->setWidth(lib->micronsToDBU(io_layer->minimumcutWidth(ii)));
            if (io_layer->hasMinimumcutWithin(ii)) {
                mc->setCutWithin(
                    lib->micronsToDBU(io_layer->minimumcutWithin(ii)));
            }
            if (io_layer->hasMinimumcutConnection(ii)) {
                if (strcmp(io_layer->minimumcutConnection(ii), "FROMABOVE") ==
                    0) {
                    mc->setIsFromAbove(true);
                } else if (strcmp(io_layer->minimumcutConnection(ii),
                                  "FROMBELOW") == 0) {
                    mc->setIsFromBelow(true);
                }
            }
            if (io_layer->hasMinimumcutNumCuts(ii)) {
                mc->setLength(
                    lib->micronsToDBU(io_layer->minimumcutLength(ii)));
                mc->setLengthWithin(
                    lib->micronsToDBU(io_layer->minimumcutDistance(ii)));
            }
            if (!head_mc) head_mc = mc;
            if (tail_mc) tail_mc->setNext(mc);
            tail_mc = mc;
        }

        rl->setMinCutList(head_mc);
    }
}

/**
 * @brief
 * set lef5.5 PROTRUSIONWIDTH
 *
 * @param io_layer
 * @param rl
 */
static void setMetalLayerProtrusionWidth(lefiLayer *io_layer,
                                         RoutingLayerRule *rl) {
    if (io_layer->hasProtrusion()) {
        Tech *lib = getTopCell()->getTechLib();
        ProtrusionRule *pr = new ProtrusionRule;
        pr->setIsLength(true);
        pr->setWidth1(lib->micronsToDBU(io_layer->protrusionWidth1()));
        pr->setWidth2(lib->micronsToDBU(io_layer->protrusionWidth2()));
        pr->setLength(lib->micronsToDBU(io_layer->protrusionLength()));
        rl->setProtrusionRuleList(pr);
    }
}

/**
 * @brief
 * set SPACINGTABLE PARALLELRUNLENGTH | TWOWIDTHS | INFLUENCE
 *
 * @param io_layer
 * @param rl
 */
static void setMetalLayerSpacingTable(lefiLayer *io_layer,
                                      RoutingLayerRule *rl) {
    UInt32 io_tbl_num = io_layer->numSpacingTable();
    if (io_tbl_num) {
        Tech *lib = getTopCell()->getTechLib();
        WidthSpTbl *head_wsp, *tail_wsp;
        head_wsp = tail_wsp = nullptr;

        for (UInt32 ii = 0; ii < io_tbl_num; ++ii) {
            lefiSpacingTable *io_tbl = io_layer->spacingTable(ii);
            if (io_tbl->isInfluence()) {  // SPACINGTABLE INFLUENCE
                InfluenceSpTbl *edi_inf = new InfluenceSpTbl;
                lefiInfluence *io_inf = io_tbl->influence();
                UInt32 num = io_inf->numInfluenceEntry();
                edi_inf->setRowNum(num);
                for (UInt32 jj = 0; jj < num; ++jj) {
                    UInt32 width = lib->micronsToDBU(io_inf->width(jj));
                    UInt32 within = lib->micronsToDBU(io_inf->distance(jj));
                    UInt32 spacing = lib->micronsToDBU(io_inf->spacing(jj));
                    edi_inf->setRowElems(jj, width, within, spacing);
                }
                rl->setInfluenceSpTbl(edi_inf);
            } else {
                WidthSpTbl *w_tbl = new WidthSpTbl;
                UInt32 min_sp = INT_MAX, max_sp = INT_MIN;

                if (io_tbl->isParallel()) {  // PARALLELRUNLENGTH
                    w_tbl->setIsPRLWidth(true);
                    lefiParallel *prl = io_tbl->parallel();
                    UInt32 len_num = prl->numLength();
                    UInt32 width_num = prl->numWidth();
                    w_tbl->setPRLDim(len_num);
                    w_tbl->setWidthDim(width_num);
                    for (UInt32 len_idx = 0; len_idx < len_num; ++len_idx) {
                        w_tbl->setPRL(len_idx,
                                      lib->micronsToDBU(prl->length(len_idx)));
                    }
                    for (UInt32 width_idx = 0; width_idx < width_num;
                         ++width_idx) {
                        w_tbl->setWidth(width_idx, lib->micronsToDBU(
                                                       prl->width(width_idx)));
                    }
                    for (UInt32 width_idx = 0; width_idx < width_num;
                         ++width_idx) {
                        for (UInt32 len_idx = 0; len_idx < len_num; ++len_idx) {
                            UInt32 spacing = lib->micronsToDBU(
                                prl->widthSpacing(width_idx, len_idx));
                            w_tbl->setSpacing(width_idx, len_idx, spacing);
                            if (spacing > max_sp) max_sp = spacing;
                            if (min_sp > spacing) min_sp = spacing;
                        }
                    }
                } else {  // TWOWIDTHS
                    w_tbl->setIsWidthSpTbl(true);
                    lefiTwoWidths *two_widths = io_tbl->twoWidths();
                    UInt32 width_num = two_widths->numWidth();
                    w_tbl->setPRLDim(width_num);
                    w_tbl->setWidthDim(width_num);
                    for (UInt32 width_idx = 0; width_idx < width_num;
                         ++width_idx) {
                        UInt32 width =
                            lib->micronsToDBU(two_widths->width(width_idx));
                        w_tbl->setWidth(width_idx, width);
                        if (two_widths->hasWidthPRL(width_idx)) {
                            w_tbl->setPRL(width_idx,
                                          lib->micronsToDBU(
                                              two_widths->widthPRL(width_idx)));
                        } else {
                            w_tbl->setPRL(width_idx, INT_MAX);
                        }
                        for (UInt32 col_idx = 0; col_idx < width_num;
                             ++col_idx) {
                            UInt32 spacing = lib->micronsToDBU(
                                two_widths->widthSpacing(width_idx, col_idx));
                            w_tbl->setSpacing(width_idx, col_idx, spacing);
                            if (spacing > max_sp) max_sp = spacing;
                            if (min_sp > spacing) min_sp = spacing;
                        }
                    }
                }
                if (!head_wsp) head_wsp = w_tbl;
                if (tail_wsp) tail_wsp->setNext(w_tbl);
                tail_wsp = w_tbl;
                if (min_sp != INT_MAX) w_tbl->setMinSpacing(min_sp);
                if (max_sp != INT_MIN) w_tbl->setMaxSpacing(max_sp);
            }
        }
        if (head_wsp) rl->setWidthSpTabl(head_wsp);
    }
}

/**
 * @brief
 * set MINSIZE rule
 *
 * @param io_layer
 * @param rl
 */
static void setMetalLayerMinSize(lefiLayer *io_layer, RoutingLayerRule *rl) {
    UInt32 min_size_num = io_layer->numMinSize();
    if (min_size_num) {
        Tech *lib = getTopCell()->getTechLib();
        MinSize *ms = new MinSize;
        ms->setMinSizeNum(min_size_num);
        for (UInt32 idx = 0; idx < min_size_num; ++idx) {
            ms->addWidthLength(idx,
                               lib->micronsToDBU(io_layer->minSizeWidth(idx)),
                               lib->micronsToDBU(io_layer->minSizeLength(idx)));
        }
        rl->setMinSize(ms);
    }
}

/**
 * @brief
 * set MINSTEP
 * actually, only one minstep is allowed in lef5.6. here, support all statements
 *
 * @param io_layer
 * @param rl
 */
static void setMetalLayerMinStep(lefiLayer *io_layer, RoutingLayerRule *rl) {
    UInt32 minstep_num = io_layer->numMinstep();
    if (minstep_num) {
        Tech *lib = getTopCell()->getTechLib();
        MinStep *head_ms, *tail_ms;
        head_ms = tail_ms = nullptr;

        for (UInt32 ii = 0; ii < minstep_num; ++ii) {
            MinStep *ms = new MinStep;
            ms->setMinStepLength(lib->micronsToDBU(io_layer->minstep(ii)));
            if (io_layer->hasMinstepMaxedges(ii)) {
                ms->setMaxEdges(io_layer->minstepMaxedges(ii));
                if (io_layer->hasMinstepMinAdjLength(ii)) {
                    ms->setMinAdjLength(
                        lib->micronsToDBU(io_layer->minstepMinAdjLength(ii)));
                } else if (io_layer->hasMinstepMinAdjLength(ii)) {
                    ms->setMinBetweenLength(
                        lib->micronsToDBU(io_layer->minstepMinBetLength(ii)));
                    if (io_layer->hasMinstepXSameCorners(ii)) {
                        ms->setIsExceptSameCorners(true);
                    }
                }
            } else {
                if (io_layer->minstepType(ii)) {
                    char *type = io_layer->minstepType(ii);
                    if (strcmp(type, "INSIDECORNER") == 0) {
                        ms->setIsInsideCorner(true);
                    } else if (strcmp(type, "OUTSIDECORNER") == 0) {
                        ms->setIsOutsideCorner(true);
                    } else if (strcmp(type, "STEP") == 0) {
                        ms->setIsStep(true);
                    }
                }
                if (io_layer->hasMinstepLengthsum(ii)) {
                    ms->setMaxLength(
                        lib->micronsToDBU(io_layer->minstepLengthsum(ii)));
                }
            }
            if (!head_ms) head_ms = ms;
            if (tail_ms) tail_ms->setNext(ms);
            tail_ms = ms;
        }
        rl->setMinStepList(head_ms);
    }
}

// APIs to parse lefi information to EDI DB(io to edi)
static int setMetalLayerRule(lefiLayer *io_layer, Layer *edi_layer) {
    RoutingLayerRule *rl = new RoutingLayerRule;
    edi_layer->setRoutingLayerRule(rl);
    Tech *lib = getTopCell()->getTechLib();

    // DIRECTION
    if (io_layer->hasDirection()) {
        UInt32 dir = 0;
        if (strcmp(io_layer->direction(), "VERTICAL") == 0) {
            dir = kVert;
        } else if (strcmp(io_layer->direction(), "HORIZONTAL") == 0) {
            dir = kHoriz;
        } else if (strcmp(io_layer->direction(), "DIAG45") == 0) {
            dir = kDiag45;
        } else if (strcmp(io_layer->direction(), "DIAG135") == 0) {
            dir = kDiag135;
        } else {
            dir = kUnknownDir;
        }
        rl->setDirection(dir);
    }
    // OFFSET
    bool no_offset = false;
    if (io_layer->hasXYOffset()) {
        rl->setOffsetXY(lib->micronsToDBU(io_layer->offsetX()),
                        lib->micronsToDBU(io_layer->offsetY()));
    } else if (io_layer->hasOffset()) {
        rl->setOffset(lib->micronsToDBU(io_layer->offset()));
    } else {
        no_offset = true;
    }
    // PITCH
    if (io_layer->hasXYPitch()) {
        rl->setPitchXY(lib->micronsToDBU(io_layer->pitchX()),
                       lib->micronsToDBU(io_layer->pitchY()));
    } else if (io_layer->hasPitch()) {
        rl->setPitch(lib->micronsToDBU(io_layer->pitch()));
    }
    if (no_offset) {
        rl->setOffsetXY(rl->getPitchX() / 2, rl->getPitchY() / 2);
    }
    // DIAGPITCH lef5.6
    if (io_layer->hasDiagPitch()) {
        rl->setDiagPitch(lib->micronsToDBU(io_layer->diagPitch()));
    } else if (io_layer->hasXYDiagPitch()) {
        rl->setDiagPitchXY(lib->micronsToDBU(io_layer->diagPitchX()),
                           lib->micronsToDBU(io_layer->diagPitchY()));
    }
    // DIAGWIDTH lef5.6
    if (io_layer->hasDiagWidth()) {
        rl->setDiagWidth(lib->micronsToDBU(io_layer->diagWidth()));
    }
    // DIAGSPACING lef5.6
    if (io_layer->hasDiagSpacing()) {
        rl->setDiagSpacing(lib->micronsToDBU(io_layer->diagSpacing()));
    }
    // DIAGMINEDGELENGTH lef5.6
    if (io_layer->hasDiagMinEdgeLength()) {
        rl->setDiagMinEdgeLength(
            lib->micronsToDBU(io_layer->diagMinEdgeLength()));
    }
    // WIREEXTENSION
    if (io_layer->hasWireExtension()) {
        rl->setWireExt(lib->micronsToDBU(io_layer->wireExtension()));
    }
    if (rl->getWireExt() < edi_layer->getWidth() / 2) {
        rl->setWireExt(edi_layer->getWidth() / 2);
    }
    // MINIMUMWIDTH lef5.5
    if (io_layer->hasMinwidth()) {
        rl->setMinWidth(lib->micronsToDBU(io_layer->minwidth()));
    } else {
        rl->setMinWidth(edi_layer->getWidth());
    }
    // MAXIMUMWIDTH lef5.5
    if (io_layer->hasMaxwidth()) {
        rl->setMaxWidth(lib->micronsToDBU(io_layer->maxwidth()));
    }
    // MINIMUMDENSITY
    if (io_layer->hasMinimumDensity()) {
        rl->setFillMinDensity(io_layer->minimumDensity());
    }
    // MAXIMUMDENSITY
    if (io_layer->hasMaximumDensity()) {
        rl->setFillMaxDensity(io_layer->maximumDensity());
    }
    // DENSITYCHECKWINDOW
    if (io_layer->hasDensityCheckWindow()) {
        rl->setFillCheckWinLength(
            lib->micronsToDBU(io_layer->densityCheckWindowLength()));
        rl->setFillCheckWinWidth(
            lib->micronsToDBU(io_layer->densityCheckWindowWidth()));
    }
    // DENSITYCHECKSTEP
    if (io_layer->hasDensityCheckStep()) {
        rl->setFillCheckWinStep(
            lib->micronsToDBU(io_layer->densityCheckStep()));
    }
    // FILLTOACTIVESPACING
    if (io_layer->hasFillActiveSpacing()) {
        rl->setFillActiveSpacing(
            lib->micronsToDBU(io_layer->fillActiveSpacing()));
    }
    // HEIGHT
    if (io_layer->hasHeight()) {
        rl->setHeight(lib->micronsToDBU(io_layer->height()));
    }
    // THICKNESS
    if (io_layer->hasThickness()) {
        rl->setThickness(lib->micronsToDBU(io_layer->thickness()));
    }
    // SHINKAGE
    if (io_layer->hasShrinkage()) {
        rl->setShrinkage(lib->micronsToDBU(io_layer->shrinkage()));
    }
    // RESISTANCE
    if (io_layer->hasResistance()) {
        rl->setResPerSq(io_layer->resistance());
    }
    // CAPACITANCE
    if (io_layer->hasCapacitance()) {
        rl->setCapPerSq(io_layer->capacitance());
    }
    // CAPMULTIPLIER
    if (io_layer->hasCapMultiplier()) {
        rl->setCapMul(io_layer->capMultiplier());
    } else {
        rl->setCapMul(1);
    }
    // EDGECAPACITANCE
    if (io_layer->hasEdgeCap()) {
        rl->setEdgeCap(io_layer->edgeCap());
    }
    // SPACING
    setMetalLayerSpacing(io_layer, rl);
    // MINENCLOSEDAREA lef5.5
    setMetalLayerMinEnclArea(io_layer, rl);
    // MINIMUMCUT lef5.5
    setMetalLayerMinCutRules(io_layer, rl);
    // PROTRUSIONWIDTH lef5.5
    setMetalLayerProtrusionWidth(io_layer, rl);
    // SPACINGTABLE lef5.5
    setMetalLayerSpacingTable(io_layer, rl);
    // MINSIZE lef5.6
    setMetalLayerMinSize(io_layer, rl);
    // MINSTEP lef5.6
    setMetalLayerMinStep(io_layer, rl);

    return 0;
}

static int setImplantLayerRule(lefiLayer *io_layer, Layer *edi_layer) {
    ImplantLayerRule *rule = new ImplantLayerRule;
    edi_layer->setImplantLayerRule(rule);
    if (io_layer->hasSpacingNumber()) {
        ImplantSpacing *head_is, *tail_is;
        head_is = tail_is = nullptr;
        Tech *lib = getTopCell()->getTechLib();
        UInt32 spacing_num = io_layer->numSpacing();
        for (UInt32 ii = 0; ii < spacing_num; ++ii) {
            ImplantSpacing *is = new ImplantSpacing;
            UInt32 spacing = lib->micronsToDBU(io_layer->spacing(ii));
            is->setMinSpacing(spacing);
            if (io_layer->hasSpacingName(ii)) {
                SecondLayer *sec_layer = new SecondLayer();
                sec_layer->setSecondLayerId(
                    lib->getLayerLEFIndexByName(io_layer->spacingName(ii)));
                if (io_layer->hasSpacingLayerStack(ii)) {
                    sec_layer->setIsStack(true);
                }
                is->setLayer2(sec_layer);
            }
            if (!head_is) head_is = is;
            if (tail_is) tail_is->setNext(is);
            tail_is = is;
        }
        rule->setSpacingList(head_is);
    }
    return 0;
}

/**
 * @brief
 * set AREA
 *
 * @param io_layer
 * @param edi_layer
 */
static void setEdiMinArea(lefiLayer *io_layer, Layer *edi_layer) {
    if (io_layer->hasArea()) {
        Tech *lib = getTopCell()->getTechLib();
        MinArea *area = edi_layer->createMinArea();
        area->setArea(lib->areaMicronsToDBU(io_layer->area()));
    }
}

/**
 * @brief
 * set ANTENNAMODEL
 *
 * @param io_layer
 * @param edi_layer
 */
static void setEdiAntennaModel(lefiLayer *io_layer, Layer *edi_layer) {
    // start from the index 1, 0 is not used
    for (UInt32 ii = 1; ii < kMaxOxideNum; ++ii) {
        AntennaModel *edi_am = edi_layer->getInitAntennaModel(ii);
        if (edi_am) {
            edi_am->setAreaFactor(1.0);
            edi_am->setIsDefaultAreaFactor(true);
        }
    }

    UInt32 num_ant = io_layer->numAntennaModel();
    for (UInt32 ii = 0; ii < num_ant; ++ii) {
        lefiAntennaModel *io_am = io_layer->antennaModel(ii);
        const char *oxide_name = io_am->antennaOxide();
        int prefix_len = strlen("OXIDE");
        UInt32 oxide_index = 1;
        if (oxide_name && strlen(oxide_name) > prefix_len) {
            oxide_name = oxide_name + prefix_len;
            oxide_index = atoi(oxide_name);
        }
        if (oxide_index < 1 || oxide_index >= kMaxOxideNum) continue;
        AntennaModel *edi_am = edi_layer->getInitAntennaModel(oxide_index);
        edi_am->setIsSet(true);
        bool is_empty = true;

        // common properties for routing layer and cut layer
        // [ ANTENNAAREARATIO   value ;]
        if (io_am->hasAntennaAreaRatio()) {
            edi_am->setAreaRatio(io_am->antennaAreaRatio());
            is_empty = false;
        }
        // [ ANTENNADIFFAREARATIO  { value  | PWL ( ( d1   r1 ) ( d2   r2 )
        // ...)} ;]
        if (io_am->hasAntennaDiffAreaRatio()) {
            edi_am->setDiffAreaRatio(io_am->antennaDiffAreaRatio());
            is_empty = false;
        } else if (io_am->hasAntennaDiffAreaRatioPWL()) {
            lefiAntennaPWL *pwl = io_am->antennaDiffAreaRatioPWL();
            for (UInt32 pwl_idx = 0; pwl_idx < pwl->numPWL(); ++pwl_idx) {
                edi_am->addDiffAreaRatioPWL(pwl->PWLdiffusion(pwl_idx),
                                            pwl->PWLratio(pwl_idx));
            }
            is_empty = false;
        }
        // [ ANTENNACUMAREARATIO   value ;]
        if (io_am->hasAntennaCumAreaRatio()) {
            edi_am->setCumAreaRatio(io_am->antennaCumAreaRatio());
            is_empty = false;
        }
        // [ ANTENNACUMDIFFAREARATIO  { value  | PWL ( ( d1   r1 ) ( d2   r2 )
        // ...)} ;]
        if (io_am->hasAntennaCumDiffAreaRatio()) {
            edi_am->setCumDiffAreaRatio(io_am->antennaCumDiffAreaRatio());
            is_empty = false;
        } else if (io_am->hasAntennaCumDiffAreaRatioPWL()) {
            lefiAntennaPWL *pwl = io_am->antennaCumDiffAreaRatioPWL();
            for (UInt32 pwl_idx = 0; pwl_idx < pwl->numPWL(); ++pwl_idx) {
                edi_am->addCumDiffAreaRatioPWL(pwl->PWLdiffusion(pwl_idx),
                                               pwl->PWLratio(pwl_idx));
            }
            is_empty = false;
        }
        // [ ANTENNAAREAFACTOR   value  [DIFFUSEONLY] ;]
        if (io_am->hasAntennaAreaFactor()) {
            is_empty = false;
            edi_am->setAreaFactor(io_am->antennaAreaFactor());
            edi_am->setIsDefaultAreaFactor(false);
            if (io_am->hasAntennaAreaFactorDUO()) {
                edi_am->setIsDiffUseOnly(true);
            }
        }
        // [ ANTENNACUMROUTINGPLUSCUT  ;]
        if (io_am->hasAntennaCumRoutingPlusCut()) {
            is_empty = false;
            edi_am->setIsCumRoutingPlusCut(true);
        }
        // [ ANTENNAGATEPLUSDIFF   plusDiffFactor  ;]
        if (io_am->hasAntennaGatePlusDiff()) {
            is_empty = false;
            edi_am->setGatePlusDiff(io_am->antennaGatePlusDiff());
        }
        // [ ANTENNAAREAMINUSDIFF   minusDiffFactor  ;]
        if (io_am->hasAntennaAreaMinusDiff()) {
            is_empty = false;
            edi_am->setAreaMinusDiff(io_am->antennaAreaMinusDiff());
        }
        // [ ANTENNAAREADIFFREDUCEPWL  ( (  diffArea1   diffAreaFactor1  ) (
        // diffArea2 diffAreaFactor2  ) ...) ; ]
        if (io_am->hasAntennaAreaDiffReducePWL()) {
            is_empty = false;
            lefiAntennaPWL *pwl = io_am->antennaAreaDiffReducePWL();
            for (UInt32 pwl_idx = 0; pwl_idx < pwl->numPWL(); ++pwl_idx) {
                edi_am->addAreaDiffReducePWL(pwl->PWLdiffusion(pwl_idx),
                                             pwl->PWLratio(pwl_idx));
            }
        }

        // routing layer properties only
        if (edi_layer->isRouting()) {
            // [ ANTENNASIDEAREARATIO   value ;]
            if (io_am->hasAntennaSideAreaRatio()) {
                is_empty = false;
                edi_am->setSideAreaRatio(io_am->antennaSideAreaRatio());
            }
            // [ ANTENNADIFFSIDEAREARATIO  { value  | PWL ( ( d1   r1 ) ( d2 r2
            // ) ...) } ;]
            if (io_am->hasAntennaDiffSideAreaRatio()) {
                is_empty = false;
                edi_am->setDiffSideAreaRatio(io_am->antennaDiffSideAreaRatio());
            } else if (io_am->hasAntennaDiffSideAreaRatioPWL()) {
                is_empty = false;
                lefiAntennaPWL *pwl = io_am->antennaDiffSideAreaRatioPWL();
                for (UInt32 pwl_idx = 0; pwl_idx < pwl->numPWL(); ++pwl_idx) {
                    edi_am->addDiffSideAreaRatioPWL(pwl->PWLdiffusion(pwl_idx),
                                                    pwl->PWLratio(pwl_idx));
                }
            }
            // [ ANTENNACUMSIDEAREARATIO   value ;]
            if (io_am->hasAntennaCumSideAreaRatio()) {
                is_empty = false;
                edi_am->setCumSideAreaRatio(io_am->antennaCumSideAreaRatio());
            }
            // [ ANTENNACUMDIFFSIDEAREARATIO  { value  | PWL ( ( d1   r1 ) ( d2
            // r2 ) ...) } ;]
            if (io_am->hasAntennaCumDiffSideAreaRatio()) {
                is_empty = false;
                edi_am->setCumDiffAreaRatio(
                    io_am->antennaCumDiffSideAreaRatio());
            } else if (io_am->hasAntennaCumDiffSideAreaRatioPWL()) {
                is_empty = false;
                lefiAntennaPWL *pwl = io_am->antennaCumDiffSideAreaRatioPWL();
                for (UInt32 pwl_idx = 0; pwl_idx < pwl->numPWL(); ++pwl_idx) {
                    edi_am->addCumDiffSideAreaRatioPWL(
                        pwl->PWLdiffusion(pwl_idx), pwl->PWLratio(pwl_idx));
                }
            }
            // [ ANTENNASIDEAREAFACTOR   value  [DIFFUSEONLY] ;]
            if (io_am->hasAntennaSideAreaFactor()) {
                is_empty = false;
                edi_am->setSideAreaFactor(io_am->antennaSideAreaRatio());
                if (io_am->hasAntennaSideAreaFactorDUO()) {
                    edi_am->setIsSideDiffUseOnly(true);
                }
            }
        }
        if (is_empty) {
            edi_am->setIsSet(false);
        }
    }
}

static void setEdiCurrentDensity(lefiLayer *io_layer, Layer *edi_layer,
                                 bool is_accurrent) {
    UInt32 den_num = is_accurrent ? io_layer->numAccurrentDensity()
                                  : io_layer->numDccurrentDensity();
    if (den_num) {
        Tech *lib = getTopCell()->getTechLib();
        // get/set density container
        CurrentDenContainer *den_con =
            is_accurrent ? edi_layer->getACCurrentDenContainer()
                         : edi_layer->getDCCurrentDenContainer();
        if (!den_con) {
            den_con = new CurrentDenContainer;
            if (is_accurrent) {
                edi_layer->setACCurrentDenContainer(den_con);
            } else {
                edi_layer->setDCCurrentDenContainer(den_con);
            }
        }

        for (UInt32 ii = 0; ii < den_num; ++ii) {
            lefiLayerDensity *io_den = is_accurrent ? io_layer->accurrent(ii)
                                                    : io_layer->dccurrent(ii);
            CurrentDen *den = nullptr;
            // init den, if den is already valid, reset it.
            // it means the latter with the same type overwrites the former.
            char *type = io_den->type();
            if (strcmp(type, "PEAK") == 0) {
                den = den_con->getInitACPeak();
                if (den->isValid()) den->reset();
                den->setIsPeak(true);
            } else if (strcmp(type, "AVERAGE") == 0) {
                den = den_con->getInitACAverage();
                if (den->isValid()) den->reset();
                den->setIsAverage(true);
            } else if (strcmp(type, "RMS") == 0) {
                den = den_con->getInitACRMS();
                if (den->isValid()) den->reset();
                den->setIsRMS(true);
            }
            if (den) {
                den->setIsAcDen(is_accurrent);
                den->setIsDcDen(!is_accurrent);
            }

            if (io_den->hasOneEntry()) {
                den->setCurrentDen(io_den->oneEntry());
                continue;
            }
            // FREQUENCY
            UInt32 num_freq = 0;
            if (is_accurrent) {
                num_freq = io_den->numFrequency();
                den->setFrequenciesNum(num_freq);
                for (UInt32 freq_idx = 0; freq_idx < num_freq; ++freq_idx) {
                    den->setFrequency(io_den->frequency(freq_idx), freq_idx);
                }
            }
            // WIDTH / CUTAREA
            UInt32 width_or_area_num = 0;
            if (edi_layer->isRouting()) {
                width_or_area_num = io_den->numWidths();
                den->setWidthsNum(width_or_area_num);
                for (UInt32 width_idx = 0; width_idx < width_or_area_num;
                     ++width_idx) {
                    den->setWidth(lib->micronsToDBU(io_den->width(width_idx)),
                                  width_idx);
                }
            } else {
                width_or_area_num = io_den->numCutareas();
                den->setCutAreasNum(width_or_area_num);
                for (UInt32 area_idx = 0; area_idx < width_or_area_num;
                     ++area_idx) {
                    den->setCutArea(
                        lib->areaMicronsToDBU(io_den->cutArea(area_idx)),
                        area_idx);
                }
            }
            // TABLEENTRIES
            UInt32 tbl_entries_num = io_den->numTableEntries();
            if (tbl_entries_num) {  // sanity check
                if (num_freq && width_or_area_num &&
                    (width_or_area_num * num_freq) != tbl_entries_num) {
                    den->reset();
                    continue;
                } else if ((!num_freq &&
                            width_or_area_num != tbl_entries_num) ||
                           (!width_or_area_num &&
                            num_freq != tbl_entries_num)) {
                    den->reset();
                    continue;
                }
                if (num_freq &&
                    width_or_area_num) {  // FREQUENCY WIDTH | CUTAREA
                    for (UInt32 freq_idx = 0; freq_idx < num_freq; ++freq_idx) {
                        for (UInt32 width_area_idx = 0;
                             width_area_idx < width_or_area_num;
                             ++width_area_idx) {
                            int entry_idx =
                                freq_idx * width_or_area_num + width_area_idx;
                            den->setTableElem(io_den->tableEntry(entry_idx),
                                              freq_idx, width_area_idx);
                        }
                    }
                } else if ((!num_freq && width_or_area_num) ||
                           (num_freq &&
                            !width_or_area_num)) {  // FREQUENCY | WIDTH |
                                                    // CUTAREA
                    if (!width_or_area_num) width_or_area_num = num_freq;
                    for (UInt32 width_area_idx = 0;
                         width_area_idx < width_or_area_num; ++width_area_idx) {
                        den->setTableElem(io_den->tableEntry(width_area_idx), 0,
                                          width_area_idx);
                    }
                }
            } else {
                den->reset();
                continue;
            }
        }
    }
}

static void setEdiCurrentDensity(lefiLayer *io_layer, Layer *edi_layer) {
    setEdiCurrentDensity(io_layer, edi_layer, true);
    setEdiCurrentDensity(io_layer, edi_layer, false);
}

static int setTrimLayerRule(lefiLayer *io_layer, Layer *edi_layer) { return 0; }

static int setCutLayerRule(lefiLayer *io_layer, Layer *edi_layer) {
    CutLayerRule *cut_layer_rule = new CutLayerRule();
    edi_layer->setCutLayerRule(cut_layer_rule);
    Tech *lib = getTopCell()->getTechLib();
    // SPACING
    if (io_layer->hasSpacingNumber()) {
        CutSpacing *head_cut_sp = 0;
        for (int i = 0; i < io_layer->numSpacing(); i++) {
            CutSpacing *cut_spacing = new CutSpacing();
            cut_spacing->setSpacing(lib->micronsToDBU(io_layer->spacing(i)));
            if (io_layer->hasSpacingCenterToCenter(i)) {
                cut_spacing->setIsC2C(true);
            }
            if (io_layer->hasSpacingSamenet(i)) {
                cut_spacing->setIsSameNet(true);
            }
            if (io_layer->hasSpacingName(i)) {
                SecondLayer *sec_layer = new SecondLayer();
                sec_layer->setSecondLayerId(
                    lib->getLayerLEFIndexByName(io_layer->spacingName(i)));
                if (io_layer->hasSpacingLayerStack(i)) {
                    sec_layer->setIsStack(true);
                }
                cut_spacing->setIsSecondLayer();
                cut_spacing->setSecondLayer(sec_layer);
            } else if (io_layer->hasSpacingAdjacent(i)) {
                AdjacentCuts *adj_cuts = new AdjacentCuts();
                adj_cuts->setCutNum(io_layer->spacingAdjacentCuts(i));
                adj_cuts->setCutWithin(
                    lib->micronsToDBU(io_layer->spacingAdjacentWithin(i)));
                if (io_layer->hasSpacingAdjacentExcept(i)) {
                    adj_cuts->setIsExceptSamePGNet(true);
                }
                // to be enhanced, separate AdjCut into individual list
                cut_spacing->setIsAdjCuts();
                cut_spacing->setAdjCuts(adj_cuts);
            } else if (io_layer->hasSpacingArea(i)) {
                cut_spacing->setCutArea(
                    lib->areaMicronsToDBU(io_layer->spacingArea(i)));
            } else if (io_layer->hasSpacingParallelOverlap(i)) {
                CutSpacingPrlOvlp *prl_ovlp = new CutSpacingPrlOvlp();
                prl_ovlp->setIsParallelOverlap(true);
                cut_spacing->setIsParallelOverlap();
                cut_spacing->setParallelOverlap(prl_ovlp);
            }
            cut_spacing->setNext(head_cut_sp);
            head_cut_sp = cut_spacing;
        }
        cut_layer_rule->setCutSpacing(head_cut_sp);
    }
    // ENCLOSURE
    Enclosure *head_enc = 0;
    for (int i = 0; i < io_layer->numEnclosure(); i++) {
        Enclosure *enc = new Enclosure();
        if (io_layer->hasEnclosureRule(i)) {
            if (strcmp(io_layer->enclosureRule(i), "ABOVE") == 0) {
                enc->setIsAbove(true);
            } else if (strcmp(io_layer->enclosureRule(i), "BELOW") == 0) {
                enc->setIsBelow(true);
            }
        }
        enc->setIsOverhang();
        EnclosureOverhang *enc_overhang = new EnclosureOverhang();
        enc->setOverhang(enc_overhang);
        enc_overhang->setOverhang1(
            lib->micronsToDBU(io_layer->enclosureOverhang1(i)));
        enc_overhang->setOverhang2(
            lib->micronsToDBU(io_layer->enclosureOverhang2(i)));
        if (io_layer->hasEnclosureWidth(i)) {
            enc_overhang->setMinWidth(
                lib->micronsToDBU(io_layer->enclosureMinWidth(i)));
        }
        if (io_layer->hasEnclosureExceptExtraCut(i)) {
            enc_overhang->setExceptExtraCutWithin(
                lib->micronsToDBU(io_layer->enclosureExceptExtraCut(i)));
        }
        if (io_layer->hasEnclosureMinLength(i)) {
            enc_overhang->setMinLength(
                lib->micronsToDBU(io_layer->enclosureMinLength(i)));
        }
        enc->setNext(head_enc);
        head_enc = enc;
    }
    cut_layer_rule->setEnclosure(head_enc);
    // PREFERENCLOSURE
    Enclosure *prefer_head_enc = 0;
    for (int i = 0; i < io_layer->numPreferEnclosure(); i++) {
        Enclosure *enc = new Enclosure();
        enc->setIsOverhang();
        EnclosureOverhang *enc_overhang = new EnclosureOverhang();
        enc->setOverhang(enc_overhang);
        enc_overhang->setOverhang1(
            lib->micronsToDBU(io_layer->preferEnclosureOverhang1(i)));
        enc_overhang->setOverhang2(
            lib->micronsToDBU(io_layer->preferEnclosureOverhang2(i)));
        if (io_layer->hasPreferEnclosureWidth(i)) {
            enc_overhang->setMinWidth(
                lib->micronsToDBU(io_layer->preferEnclosureMinWidth(i)));
        }
        enc->setNext(prefer_head_enc);
        prefer_head_enc = enc;
    }
    cut_layer_rule->setPreferEnclosure(prefer_head_enc);
    // ARRAYSPACING
    if (io_layer->hasArraySpacing()) {
        ArraySpacing *array_spacing = new ArraySpacing();
        if (io_layer->hasLongArray()) {
            array_spacing->setIsLongArray(true);
        }
        if (io_layer->hasViaWidth()) {
            array_spacing->setViaWidth(lib->micronsToDBU(io_layer->viaWidth()));
        }
        array_spacing->setCutSpacing(lib->micronsToDBU(io_layer->cutSpacing()));
        array_spacing->setNumArrayCuts(io_layer->numArrayCuts());
        for (int i = 0; i < io_layer->numArrayCuts(); i++) {
            array_spacing->setArrayCuts(i, io_layer->arrayCuts(i));
            array_spacing->setArraySpacing(
                i, lib->micronsToDBU(io_layer->arraySpacing(i)));
        }
        cut_layer_rule->setArraySpacing(array_spacing);
    }
    return 0;
}

static void readLayerProperty(lefiLayer *io_layer, Layer *edi_layer) {
    Cell *top_cell = getTopCell();
    if (!top_cell) return;

    for (int ii = 0; ii < io_layer->numProps(); ++ii) {
        Property *property =
            top_cell->createObject<Property>(kObjectTypeProperty);
        property->setDefinitionId(PropType::kLayer, io_layer->propName(ii));
        if (io_layer->propIsNumber(ii)) {
            if (property->getDataType() == PropDataType::kInt) {
                property->setIntValue(
                    static_cast<int>(io_layer->propNumber(ii)));
            } else {
                property->setRealValue(io_layer->propNumber(ii));
            }
        } else if (io_layer->propIsString(ii)) {
            property->setStringValue(io_layer->propValue(ii));
        }
        edi_layer->addProp(property->getId());
    }
}

static unsigned int layer_order_in_lef = 0;
static unsigned int layer_z = 1;

int readLayer(lefiLayer *io_layer) {
    Cell *curr_cell = getTopCell();
    if (!curr_cell) return 0;

    Tech *lib = curr_cell->getTechLib();
    Layer *edi_layer = curr_cell->createObject<Layer>(kObjectTypeLayer);

    // set basic layer information
    SymbolIndex sym_id = curr_cell->getOrCreateSymbol(io_layer->name());
    edi_layer->setNameId(sym_id);
    curr_cell->addSymbolReference(sym_id, edi_layer->getId());

    // only handle normal layer
    // for LEF58_TYPE, it will be handled during LEF parser
    UInt32 type_table_size = GetLayerTypeTableSize();
    for (int i = 0; i < type_table_size; i++) {
        if (strcmp(io_layer->type(), GetLEFTypeFromTables(i)) == 0) {
            edi_layer->setType(GetEDITypeFromTables(i));
            break;
        }
    }
    edi_layer->setIndexInLef(layer_order_in_lef++);
    if (edi_layer->isRouting()) {
        edi_layer->setZ(layer_z++);
    }
    if (io_layer->hasMask()) {
        edi_layer->setNumMask(io_layer->mask());
    }
    if (io_layer->hasWidth()) {
        edi_layer->setWidth(lib->micronsToDBU(io_layer->width()));
    }
    if (io_layer->numProps()) {
        readLayerProperty(io_layer, edi_layer);
    }

    // set layer rule
    if (edi_layer->isImplant()) {
        setImplantLayerRule(io_layer, edi_layer);
    } else if (edi_layer->isRouting()) {
        setMetalLayerRule(io_layer, edi_layer);
    } else if (edi_layer->isCut()) {
        setCutLayerRule(io_layer, edi_layer);
    } else if (edi_layer->isTrimMetal()) {
        setTrimLayerRule(io_layer, edi_layer);
    }
    setEdiMinArea(io_layer, edi_layer);
    setEdiAntennaModel(io_layer, edi_layer);
    setEdiCurrentDensity(io_layer, edi_layer);
    // set tech layer
    lib->addLayer(edi_layer);
    return 0;
}

int readViaMaster(lefiVia *io_via, bool is_from_ndr) {
    std::string io_via_name = io_via->name();
    Cell *top_cell = getTopCell();
    Tech *lib = top_cell->getTechLib();
    ViaMaster *db_via_master = lib->createAndAddViaMaster(io_via_name);
    // setting common
    if (io_via->hasDefault()) db_via_master->setDefault(1);
    db_via_master->setIsFromDEF(0);

    // property
    db_via_master->setPropertySize(io_via->numProperties());
    for (int i = 0; i < io_via->numProperties(); i++) {
        Property *property =
            top_cell->createObject<Property>(kObjectTypeProperty);
        property->setDefinitionId(PropType::kVia, io_via->propName(i));
        if (io_via->propIsNumber(i)) {
            if (property->getDataType() == PropDataType::kInt) {
                property->setIntValue(static_cast<int>(io_via->propNumber(i)));
            } else {
                property->setRealValue(io_via->propNumber(i));
            }
        }
        if (io_via->propIsString(i)) {
            property->setStringValue(io_via->propValue(i));
        }
        db_via_master->addProperty(property->getId());
    }

    if (io_via->hasGenerated())
        db_via_master->sethasGenerated(io_via->hasGenerated());
    if (io_via->hasViaRule()) {
        // db_via_master->hasViaRule(1);
        db_via_master->setViaRule(const_cast<char *>(io_via->viaRuleName()));

        db_via_master->setCutSizeX(lib->micronsToDBU(io_via->xCutSize()));
        db_via_master->setCutSizeY(lib->micronsToDBU(io_via->yCutSize()));

        db_via_master->setLowerLayerIndex(
            const_cast<char *>(io_via->botMetalLayer()));
        db_via_master->setCutLayerIndex(const_cast<char *>(io_via->cutLayer()));
        db_via_master->setUperLayerIndex(
            const_cast<char *>(io_via->topMetalLayer()));

        db_via_master->setCutSpacingX(lib->micronsToDBU(io_via->xCutSpacing()));
        db_via_master->setCutSpacingY(lib->micronsToDBU(io_via->yCutSpacing()));

        db_via_master->setLowerEncX(lib->micronsToDBU(io_via->xBotEnc()));
        db_via_master->setLowerEncY(lib->micronsToDBU(io_via->yBotEnc()));
        db_via_master->setUpperEncX(lib->micronsToDBU(io_via->xTopEnc()));
        db_via_master->setUpperEncY(lib->micronsToDBU(io_via->yTopEnc()));

        db_via_master->setIsArray(io_via->hasRowCol());
        if (io_via->hasRowCol()) {
            db_via_master->setRow(io_via->numCutRows());
            db_via_master->setCol(io_via->numCutCols());
        }

        db_via_master->setHasOrigin(io_via->hasOrigin());
        if (io_via->hasOrigin()) {
            db_via_master->setOffsetX(lib->micronsToDBU(io_via->xOffset()));
            db_via_master->setOffsetY(lib->micronsToDBU(io_via->yOffset()));
        }

        db_via_master->setHasOffset(io_via->hasOffset());
        if (io_via->hasOffset()) {
            db_via_master->setLowerOffsetX(
                lib->micronsToDBU(io_via->xBotOffset()));
            db_via_master->setLowerOffsetY(
                lib->micronsToDBU(io_via->yBotOffset()));
            db_via_master->setUpperOffsetX(
                lib->micronsToDBU(io_via->xTopOffset()));
            db_via_master->setUpperOffsetY(
                lib->micronsToDBU(io_via->yTopOffset()));
        }

        db_via_master->setHasCutPatten(io_via->hasCutPattern());
        if (io_via->hasCutPattern()) {
            std::string cut_patten_name = io_via->cutPattern();
            db_via_master->setCutPatten(cut_patten_name);
        }
    } else {  // fixed via type
        db_via_master->setHasResistance(io_via->hasResistance());
        if (io_via->hasResistance()) {
            db_via_master->setResistance(io_via->resistance());
        }

        for (int i = 0; i < io_via->numLayers(); ++i) {
            std::string via_layer_name = io_via->layerName(i);
            ViaLayer *via_layer = db_via_master->creatViaLayer(via_layer_name);
            for (int j = 0; j < io_via->numRects(i); ++j) {
                Box *box = creatBox(lib->micronsToDBU(io_via->xl(i, j)),
                                    lib->micronsToDBU(io_via->yl(i, j)),
                                    lib->micronsToDBU(io_via->xh(i, j)),
                                    lib->micronsToDBU(io_via->yh(i, j)));
                via_layer->addMask(io_via->rectColorMask(i, j));
                via_layer->addRect(box);
            }
            db_via_master->addViaLayer(via_layer);
        }
    }
    if (is_from_ndr) {
        db_via_master->setIsFromNDR(true);
    }
    return 0;
}

int readViaRule(lefiViaRule *io_via_rule) {
    Cell *top_cell = getTopCell();
    Tech *lib = top_cell->getTechLib();
    std::string rule_name = io_via_rule->name();
    ViaRule *db_via_rule = lib->createViaRule(rule_name);

    if (io_via_rule->lefiViaRule::hasGenerate()) {
        db_via_rule->setIsGenerate(true);
    }
    if (io_via_rule->lefiViaRule::hasDefault()) {
        db_via_rule->setIsDefault(true);
    }
    int num_layers = io_via_rule->lefiViaRule::numLayers();
    // add vias
    if (num_layers == 2 && !(io_via_rule->lefiViaRule::hasGenerate())) {
        int num_vias = io_via_rule->numVias();
        for (int i = 0; i < num_vias; ++i) {
            std::string via_master_name = io_via_rule->viaName(i);
            ViaMaster *via_master = lib->getViaMaster(via_master_name);
            if (via_master) db_via_rule->addViaMaster(via_master);
        }
    }
    // add property
    // property
    db_via_rule->setPropertySize(io_via_rule->numProps());
    for (int i = 0; i < io_via_rule->numProps(); i++) {
        Property *property =
            top_cell->createObject<Property>(kObjectTypeProperty);
        property->setDefinitionId(PropType::kViaRule, io_via_rule->propName(i));
        if (io_via_rule->propIsNumber(i)) {
            if (property->getDataType() == PropDataType::kInt) {
                property->setIntValue(
                                 static_cast<int>(io_via_rule->propNumber(i)));
            } else {
                property->setRealValue(io_via_rule->propNumber(i));
            }
        }
        if (io_via_rule->propIsString(i)) {
            property->setStringValue(io_via_rule->propValue(i));
        }
        db_via_rule->addProperty(property->getId());
    }
    // add ViaRuleLayer
    for (int i = 0; i < num_layers; i++) {
        lefiViaRuleLayer *vLayer = io_via_rule->lefiViaRule::layer(i);
        if (i < 2) {
            // metal/routing layer VIARULE(GENERATE)
            ViaRuleMetalLayer *via_rule_layer =
                db_via_rule->creatViaRuleMetalLayer();
            Int32 layer_id =
                lib->getLayerLEFIndexByName(vLayer->lefiViaRuleLayer::name());
            if (layer_id == -1) {
                getTopCell()->deleteObject<ViaRule>(db_via_rule);
                message->issueMsg(kError, "Cannot find layer %s in LEF.\n",
                                  vLayer->lefiViaRuleLayer::name());
                return 1;
            }
            via_rule_layer->setLayerId(layer_id);
            if (vLayer->lefiViaRuleLayer::hasDirection()) {
                via_rule_layer->setHasDirection(true);
                if (vLayer->lefiViaRuleLayer::isHorizontal()) {
                    via_rule_layer->setIsHorizontal(
                        true);  // or else it is vertical
                }
            }
            if (vLayer->lefiViaRuleLayer::hasEnclosure()) {
                via_rule_layer->setHasEnclosure(true);
                via_rule_layer->setOverhang1(lib->micronsToDBU(
                    vLayer->lefiViaRuleLayer::enclosureOverhang1()));
                via_rule_layer->setOverhang2(lib->micronsToDBU(
                    vLayer->lefiViaRuleLayer::enclosureOverhang2()));
            }
            if (vLayer->lefiViaRuleLayer::hasWidth()) {
                via_rule_layer->setHasWidth(true);
                via_rule_layer->setMinWidth(
                    lib->micronsToDBU(vLayer->lefiViaRuleLayer::widthMin()));
                via_rule_layer->setMaxWidth(
                    lib->micronsToDBU(vLayer->lefiViaRuleLayer::widthMax()));
            }
            db_via_rule->addViaRuleMetalLayer(via_rule_layer);
        } else if (i == 2) {
            // cut layer VIARULE GENERATE
            ViaRuleCutLayer *via_rule_layer =
                db_via_rule->creatViaRuleCutLayer();
            via_rule_layer->setLayerId(
                lib->getLayerLEFIndexByName(vLayer->lefiViaRuleLayer::name()));
            if (vLayer->lefiViaRuleLayer::hasResistance()) {
                via_rule_layer->setHasRestance(true);
                via_rule_layer->setResistancePerCut(
                    vLayer->lefiViaRuleLayer::resistance());
            }
            Box rect(lib->micronsToDBU(vLayer->lefiViaRuleLayer::xl()),
                     lib->micronsToDBU(vLayer->lefiViaRuleLayer::yl()),
                     lib->micronsToDBU(vLayer->lefiViaRuleLayer::xh()),
                     lib->micronsToDBU(vLayer->lefiViaRuleLayer::yh()));
            via_rule_layer->setBox(rect);
            via_rule_layer->setXSpacing(
                lib->micronsToDBU(vLayer->lefiViaRuleLayer::spacingStepX()));
            via_rule_layer->setYSpacing(
                lib->micronsToDBU(vLayer->lefiViaRuleLayer::spacingStepY()));
            db_via_rule->setViaRuleCutLayer(via_rule_layer);
        }
    }
    lib->addViaRule(db_via_rule);

    return 0;
}

int readSite(lefiSite *io_site) {
    Cell *current_top_cell = getTopCell();
    if (!current_top_cell) return 0;
    Tech *lib = current_top_cell->getTechLib();

    Site *db_site = current_top_cell->createObject<Site>(kObjectTypeSite);

    db_site->setName(const_cast<char *>(io_site->lefiSite::name()));
    db_site->setClass(io_site->lefiSite::siteClass());
    if (io_site->lefiSite::has90Symmetry()) {
        db_site->setSymmetry(Symmetry::kR90);
    } else if (io_site->lefiSite::hasXSymmetry()) {
        db_site->setSymmetry(Symmetry::kX);
    } else if (io_site->lefiSite::hasYSymmetry()) {
        db_site->setSymmetry(Symmetry::kY);
    } else {
        db_site->setSymmetry(Symmetry::kUnknown);
    }
    if (io_site->lefiSite::hasSize()) {
        db_site->setWidth(lib->micronsToDBU(io_site->lefiSite::sizeX()));
        db_site->setHeight(lib->micronsToDBU(io_site->lefiSite::sizeY()));
    }
    if (io_site->hasRowPattern()) {
        db_site->setPatternSize(io_site->lefiSite::numSites());
        for (int i = 0; i < io_site->lefiSite::numSites(); i++) {
            SitePatternPair *site_pattern =
                current_top_cell->createObject<SitePatternPair>(
                    kObjectTypeSitePatternPair);
            if (site_pattern == nullptr) continue;
            site_pattern->setName(io_site->lefiSite::siteName(i));
            site_pattern->setOrientation(
                toEnumByString<Orient>(io_site->lefiSite::siteOrientStr(i)));
            db_site->addSitePattern(site_pattern->getId());
        }
    }
    getTopCell()->getTechLib()->addSite(db_site);

    return 0;
}

int readUnit(lefiUnits *io_units) {
    Cell *current_top_cell = getTopCell();
    if (!current_top_cell) return 0;

    Units *db_units = current_top_cell->createObject<Units>(kObjectTypeUnits);

    if (io_units->hasDatabase()) {
        db_units->setLengthUnits(io_units->databaseName());
        db_units->setLengthFactor(io_units->databaseNumber());
    }
    if (io_units->hasCapacitance()) {
        db_units->setCapacitanceUnits("PICOFARADS");
        db_units->setCapacitanceFactor(io_units->capacitance());
    }
    if (io_units->hasResistance()) {
        db_units->setResistanceUnits("OHMS");
        db_units->setResistanceFactor(io_units->resistance());
    }
    if (io_units->hasTime()) {
        db_units->setTimeUnits("NANOSECONDS");
        db_units->setTimeFactor(io_units->time());
    }
    if (io_units->hasPower()) {
        db_units->setPowerUnits("MILLIWATTS");
        db_units->setPowerFactor(io_units->power());
    }
    if (io_units->hasCurrent()) {
        db_units->setCurrentUnits("MILLIAMPS");
        db_units->setCurrentFactor(io_units->current());
    }
    if (io_units->hasVoltage()) {
        db_units->setVoltageUnits("VOLTS");
        db_units->setVoltageFactor(io_units->voltage());
    }
    if (io_units->hasFrequency()) {
        db_units->setFrequencyUnits("MEGAHERTZ");
        db_units->setFrequencyFactor(io_units->frequency());
    }
    // set tech layer
    getTopCell()->getTechLib()->setUnits(db_units);
    return 0;
}

int readPropertyDefinition(lefiProp *io_prop) {
    Cell *current_top_cell = getTopCell();
    if (!current_top_cell) return 0;

    PropertyDefinition *prop_definition =
        current_top_cell->createObject<PropertyDefinition>(
            kObjectTypePropertyDefinition);
    PropType type = toEnumByString<PropType>(io_prop->lefiProp::propType());
    prop_definition->setPropType(type);

    prop_definition->setPropName(io_prop->lefiProp::propName());

    PropDataType data_type = PropDataType::kUnknown;

    switch (io_prop->lefiProp::dataType()) {
        case 'I':
            data_type = PropDataType::kInt;
            break;
        case 'R':
        case 'N':
            data_type = PropDataType::kReal;
            break;
        case 'S':
        case 'Q':
            data_type = PropDataType::kString;
            break;
    }
    prop_definition->setDataType(data_type);

    if (io_prop->lefiProp::hasNumber()) {
        double io_number = io_prop->lefiProp::number();
        if (data_type == PropDataType::kInt) {
            prop_definition->setIntValue(static_cast<int>(io_number));
        } else {
            prop_definition->setRealValue(io_number);
        }
    }
    if (io_prop->lefiProp::hasString() && data_type == PropDataType::kString) {
        prop_definition->setStringValue(io_prop->lefiProp::string());
    }

    if (io_prop->lefiProp::hasRange()) {
        double left = io_prop->lefiProp::left();
        double right = io_prop->lefiProp::right();
        if (data_type == PropDataType::kInt) {
            prop_definition->setIntRangeMin(static_cast<int>(left));
            prop_definition->setIntRangeMax(static_cast<int>(right));
        } else {
            prop_definition->setRealRangeMin(left);
            prop_definition->setRealRangeMax(right);
        }
    }
    ObjectId obj_id = prop_definition->getId();
    getTopCell()->getTechLib()->addPropertyDefinition(obj_id);
    return 0;
}

int readNonDefaultRule(lefiNonDefault *io_ndr_rule) {
    Cell *current_top_cell = getTopCell();
    if (!current_top_cell) return 0;

    NonDefaultRule *edi_ndr_rule =
        current_top_cell->createObject<NonDefaultRule>(kObjectTypeRule);
    ediAssert(edi_ndr_rule != nullptr);
    int i = 0;
    Tech *lib = getTopCell()->getTechLib();
    // name
    edi_ndr_rule->setName(io_ndr_rule->lefiNonDefault::name());
    // hard_spacing
    edi_ndr_rule->setHardSpacing(io_ndr_rule->lefiNonDefault::hasHardspacing());
    // layer
    edi_ndr_rule->setLayerSize(io_ndr_rule->lefiNonDefault::numLayers());
    for (i = 0; i < io_ndr_rule->lefiNonDefault::numLayers(); i++) {
        NonDefaultRuleLayer *layer =
            current_top_cell->createObject<NonDefaultRuleLayer>(
                kObjectTypeNonDefaultRuleLayer);

        layer->setName(io_ndr_rule->lefiNonDefault::layerName(i));
        if (io_ndr_rule->lefiNonDefault::hasLayerWidth(i))
            layer->setWidth(
                lib->micronsToDBU(io_ndr_rule->lefiNonDefault::layerWidth(i)));
        if (io_ndr_rule->lefiNonDefault::hasLayerSpacing(i))
            layer->setSpacing(lib->micronsToDBU(
                io_ndr_rule->lefiNonDefault::layerSpacing(i)));
        if (io_ndr_rule->lefiNonDefault::hasLayerDiagWidth(i))
            layer->setDiagWidth(lib->micronsToDBU(
                io_ndr_rule->lefiNonDefault::layerDiagWidth(i)));
        if (io_ndr_rule->lefiNonDefault::hasLayerWireExtension(i))
            layer->setWireExt(lib->micronsToDBU(
                io_ndr_rule->lefiNonDefault::layerWireExtension(i)));
        if (io_ndr_rule->lefiNonDefault::hasLayerResistance(i))
            layer->setResistancePerSquare(
                io_ndr_rule->lefiNonDefault::layerResistance(i));
        if (io_ndr_rule->lefiNonDefault::hasLayerCapacitance(i))
            layer->setCapacitancePerSquare(
                io_ndr_rule->lefiNonDefault::layerCapacitance(i));
        if (io_ndr_rule->lefiNonDefault::hasLayerEdgeCap(i))
            layer->setEdgeCapacitance(
                io_ndr_rule->lefiNonDefault::layerEdgeCap(i));
        edi_ndr_rule->addLayer(layer->getId());
    }
    // min_cuts
    edi_ndr_rule->setMinCutsSize(io_ndr_rule->lefiNonDefault::numMinCuts());
    for (i = 0; i < io_ndr_rule->lefiNonDefault::numMinCuts(); i++) {
        NonDefaultRuleMinCuts *min_cuts =
            current_top_cell->createObject<NonDefaultRuleMinCuts>(
                kObjectTypeNonDefaultRuleMinCuts);
        min_cuts->setName(io_ndr_rule->lefiNonDefault::cutLayerName(i));
        min_cuts->setNumCuts(io_ndr_rule->lefiNonDefault::numCuts(i));
        edi_ndr_rule->addMinCuts(min_cuts->getId());
    }
    // via
    edi_ndr_rule->setViaSize(io_ndr_rule->lefiNonDefault::numVias());
    for (i = 0; i < io_ndr_rule->lefiNonDefault::numVias(); i++) {
        lefiVia *via = io_ndr_rule->lefiNonDefault::viaRule(i);
        if (via == nullptr) continue;
        readViaMaster(via, true /*is_from_ndr*/);
        ViaMaster *use_via = lib->getViaMaster(via->lefiVia::name());
        edi_ndr_rule->addVia(use_via->getId());
    }
    // use_via
    edi_ndr_rule->setUseViaSize(io_ndr_rule->lefiNonDefault::numUseVia());
    for (i = 0; i < io_ndr_rule->lefiNonDefault::numUseVia(); i++) {
        ViaMaster *use_via =
            lib->getViaMaster(io_ndr_rule->lefiNonDefault::viaName(i));
        if (use_via == nullptr) {
            continue;
        }

        edi_ndr_rule->addUseVia(use_via->getId());
    }
    // use_via_rule
    edi_ndr_rule->setUseViaRuleSize(
        io_ndr_rule->lefiNonDefault::numUseViaRule());
    for (i = 0; i < io_ndr_rule->lefiNonDefault::numUseViaRule(); i++) {
        ViaRule *use_via_rule =
            lib->getViaRule(io_ndr_rule->lefiNonDefault::viaRuleName(i));
        if (use_via_rule == nullptr) {
            continue;
        }
        edi_ndr_rule->addUseViaRule(use_via_rule->getId());
    }
    // property
    edi_ndr_rule->setPropertySize(io_ndr_rule->lefiNonDefault::numProps());
    for (i = 0; i < io_ndr_rule->lefiNonDefault::numProps(); i++) {
        Property *property =
            current_top_cell->createObject<Property>(kObjectTypeProperty);
        property->setDefinitionId(PropType::kNonDefaultRule,
                                  io_ndr_rule->lefiNonDefault::propName(i));
        if (io_ndr_rule->lefiNonDefault::propIsNumber(i)) {
            if (property->getDataType() == PropDataType::kInt) {
                property->setIntValue(
                  static_cast<int>(io_ndr_rule->lefiNonDefault::propNumber(i)));
            } else {
                property->setRealValue(
                    io_ndr_rule->lefiNonDefault::propNumber(i));
            }
        }
        if (io_ndr_rule->lefiNonDefault::propIsString(i)) {
            property->setStringValue(io_ndr_rule->lefiNonDefault::propValue(i));
        }
        edi_ndr_rule->addProperty(property->getId());
    }

    ObjectId obj_id = edi_ndr_rule->getId();
    getTopCell()->getTechLib()->addNonDefaultRule(obj_id);
    return 0;
}

}  // namespace db
}  // namespace open_edi
