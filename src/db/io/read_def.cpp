/* @file  read_def.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <string>
#include <vector>

#ifndef WIN32
#include <unistd.h>
#endif /* not WIN32 */

#include "db/core/db.h"
#include "db/io/read_def.h"
#include "util/util.h"

namespace open_edi {
namespace db {

#define OK (0)
#define ERROR (1)

static char defaultName[64];
static char defaultOut[64];

// Global variables
static FILE* fout;
void* userData;
int numObjs;
int isSumSet;    // to keep track if within SUM
int isProp = 0;  // for PROPERTYDEFINITIONS
int begOperand;  // to keep track for constraint, to print - as the 1st char
static double curVer = 0;
static int setSNetWireCbk = 0;
static int isSessionless = 0;
static int ignoreRowNames = 0;
static int ignoreViaNames = 0;
static std::string current_net_name = "";

static std::vector<char*> kGroupCompNamePatterns;
static void saveGroupMember(char* name);
static void clearGroupMember();

template <class from_t, class to_t>
static void readProperties(PropType prop_type, void* from, void* to) {
    from_t* from_object = reinterpret_cast<from_t*>(from);
    to_t* to_object = reinterpret_cast<to_t*>(to);

    Cell* top_cell = getTopCell();

    to_object->setPropertySize(from_object->numProps());
    for (int i = 0; i < from_object->numProps(); i++) {
        Property* property =
            top_cell->createObject<Property>(kObjectTypeProperty);
        property->setDefinitionId(prop_type, from_object->propName(i));
        if (from_object->propIsNumber(i)) {
            if (property->getDataType() == PropDataType::kInt) {
                property->setIntValue(
                    static_cast<int>(from_object->propNumber(i)));
            } else {
                property->setRealValue(from_object->propNumber(i));
            }
        }
        if (from_object->propIsString(i)) {
            property->setStringValue(from_object->propValue(i));
        }
        to_object->addProperty(property->getId());
    }
}

void myLogFunction(const char* errMsg) {
    fprintf(fout, "ERROR: found error: %s\n", errMsg);
}

void myWarningLogFunction(const char* errMsg) {
    fprintf(fout, "WARNING: found error: %s\n", errMsg);
}

void dataError() {
    fprintf(fout, "ERROR: returned user data is not correct!\n");
}

void checkType(defrCallbackType_e c) {
    if (c >= 0 && c <= defrDesignEndCbkType) {
        // OK
    } else {
        fprintf(fout, "ERROR: callback type is out of bounds!\n");
    }
}

int done(defrCallbackType_e c, void*, defiUserData ud) {
    checkType(c);
    if (ud != userData) dataError();

    return 0;
}

int endfunc(defrCallbackType_e c, void*, defiUserData ud) {
    checkType(c);
    if (ud != userData) dataError();
    return 0;
}

char* orientStr(int orient) {
    switch (orient) {
        case 0:
            return (const_cast<char*>("N"));
        case 1:
            return (const_cast<char*>("W"));
        case 2:
            return (const_cast<char*>("S"));
        case 3:
            return (const_cast<char*>("E"));
        case 4:
            return (const_cast<char*>("FN"));
        case 5:
            return (const_cast<char*>("FW"));
        case 6:
            return (const_cast<char*>("FS"));
        case 7:
            return (const_cast<char*>("FE"));
    }
    return (const_cast<char*>("BOGUS"));
}

int compMSL(defrCallbackType_e c, defiComponentMaskShiftLayer* co,
            defiUserData ud) {
    int i;

    checkType(c);
    if (ud != userData) dataError();
    readCompMaskShift(co);

    return 0;
}

int compf(defrCallbackType_e c, defiComponent* co, defiUserData ud) {
    int i;

    checkType(c);
    if (ud != userData) dataError();
    readComp(co);

    --numObjs;
    return 0;
}

int netpath(defrCallbackType_e, defiNet*, defiUserData) { return 0; }

int netNamef(defrCallbackType_e c, const char* netName, defiUserData ud) {
    checkType(c);
    if (ud != userData) dataError();
    current_net_name = netName;
    return 0;
}

int subnetNamef(defrCallbackType_e c, const char* subnetName, defiUserData ud) {
    checkType(c);
    if (ud != userData) dataError();

    return 0;
}

int nondefRulef(defrCallbackType_e c, const char* ruleName, defiUserData ud) {
    checkType(c);
    if (ud != userData) dataError();

    return 0;
}

int netf(defrCallbackType_e c, defiNet* net, defiUserData ud) {
    // For net and special net.
    int i, j, k, w, x, y, z, count, newLayer;
    defiPath* p;
    defiSubnet* s;
    int path;
    defiVpin* vpin;
    // defiShield *noShield;
    defiWire* wire;

    checkType(c);
    if (ud != userData) dataError();

    readNet(net);
    --numObjs;

    return 0;
}

int snetpath(defrCallbackType_e c, defiNet* ppath, defiUserData ud) {
    int i, j, x, y, z, count, newLayer;
    char* layerName;
    double dist, left, right;
    defiPath* p;
    defiSubnet* s;
    int path;
    defiShield* shield;
    defiWire* wire;
    int numX, numY, stepX, stepY;

    if (c != defrSNetPartialPathCbkType) return 1;
    if (ud != userData) dataError();

    return 0;
}

int snetwire(defrCallbackType_e c, defiNet* ppath, defiUserData ud) {
    int i, j, x, y, z, count = 0, newLayer;
    defiPath* p;
    int path;
    defiWire* wire;
    defiShield* shield;
    int numX, numY, stepX, stepY;

    if (c != defrSNetWireCbkType) return 1;
    if (ud != userData) dataError();

    return 0;
}

int snetf(defrCallbackType_e c, defiNet* net, defiUserData ud) {
    // For net and special net.
    int i, j, x, y, z, count, newLayer;
    char* layerName;
    double dist, left, right;
    defiPath* p;
    defiSubnet* s;
    int path;
    defiShield* shield;
    defiWire* wire;
    int numX, numY, stepX, stepY;

    checkType(c);
    if (ud != userData) dataError();

    readSpecialNet(net);

    --numObjs;

    return 0;
}

int ndr(defrCallbackType_e c, defiNonDefault* nd, defiUserData ud) {
    // For nondefaultrule
    int i;

    checkType(c);
    if (ud != userData) dataError();

    readNonDefaultRule(nd);

    --numObjs;

    return 0;
}

int tname(defrCallbackType_e c, const char* string, defiUserData ud) {
    checkType(c);
    if (ud != userData) dataError();

    return 0;
}

int dname(defrCallbackType_e c, const char* string, defiUserData ud) {
    checkType(c);
    if (ud != userData) dataError();
    if (!string) {
        message->issueMsg(
            kError, "Design name is null pointer when reading DEF DESIGN.\n");
        return ERROR;
    }
    Cell* top_cell = getTopCell();
    if (!top_cell) {
        message->issueMsg(kError,
                          "Cannot find top cell when reading DEF DESIGN.\n");
        return ERROR;
    }

    std::string design_name = string;
    top_cell->setName(design_name);

    return OK;
}

char* address(const char* in) { return (const_cast<char*>(in)); }

int cs(defrCallbackType_e c, int num, defiUserData ud) {
    char* name;

    checkType(c);

    if (ud != userData) dataError();

    numObjs = num;
    return 0;
}

int constraintst(defrCallbackType_e c, int num, defiUserData ud) {
    // Handles both constraints and assertions
    checkType(c);
    if (ud != userData) dataError();

    numObjs = num;
    return 0;
}

void operand(defrCallbackType_e c, defiAssertion* a, int ind) {
    int i, first = 1;
    char* netName;
    char *fromInst, *fromPin, *toInst, *toPin;
}

int constraint(defrCallbackType_e c, defiAssertion* a, defiUserData ud) {
    // Handles both constraints and assertions

    checkType(c);
    if (ud != userData) dataError();

    --numObjs;
    return 0;
}

int propstart(defrCallbackType_e c, void*, defiUserData) {
    checkType(c);

    isProp = 1;

    return 0;
}

int prop(defrCallbackType_e c, defiProp* p, defiUserData ud) {
    checkType(c);
    if (ud != userData) dataError();

    readPropertyDefinition(p);

    return 0;
}

int propend(defrCallbackType_e c, void*, defiUserData) {
    checkType(c);

    return 0;
}

int hist(defrCallbackType_e c, const char* h, defiUserData ud) {
    checkType(c);
    defrSetCaseSensitivity(0);
    if (ud != userData) dataError();

    defrSetCaseSensitivity(1);
    return 0;
}

int an(defrCallbackType_e c, const char* h, defiUserData ud) {
    checkType(c);
    if (ud != userData) dataError();

    return 0;
}

int fn(defrCallbackType_e c, const char* h, defiUserData ud) {
    checkType(c);
    if (ud != userData) dataError();

    return 0;
}

int bbn(defrCallbackType_e c, const char* h, defiUserData ud) {
    checkType(c);
    if (ud != userData) dataError();

    return 0;
}

int vers(defrCallbackType_e c, double d, defiUserData ud) {
    checkType(c);
    if (ud != userData) dataError();

    return 0;
}

int versStr(defrCallbackType_e c, const char* versionName, defiUserData ud) {
    checkType(c);
    if (ud != userData) dataError();

    return 0;
}

int units(defrCallbackType_e c, double d, defiUserData ud) {
    checkType(c);
    if (ud != userData) dataError();
    readUnit(d);
    return 0;
}

int casesens(defrCallbackType_e c, int d, defiUserData ud) {
    checkType(c);
    if (ud != userData) dataError();

    return 0;
}

int cls(defrCallbackType_e c, void* cl, defiUserData ud) {
    defiSite* site;  // Site and Canplace and CannotOccupy
    defiBox* box;    // DieArea and
    defiPinCap* pc;
    defiPin* pin;
    int i, j, k;
    defiRow* row;
    defiTrack* track;
    defiGcellGrid* gcg;
    defiVia* via;
    defiRegion* re;
    defiGroup* group;
    defiComponentMaskShiftLayer* maskShiftLayer = NULL;
    defiScanchain* sc;
    defiIOTiming* iot;
    defiFPC* fpc;
    defiTimingDisable* td;
    defiPartition* part;
    defiPinProp* pprop;
    defiBlockage* block;
    defiSlot* slots;
    defiFill* fills;
    defiStyles* styles;
    int xl, yl, xh, yh;
    char *name, *a1, *b1;
    char **inst, **inPin, **outPin;
    int* bits;
    int size;
    int corner, typ;
    const char* itemT;
    char dir;
    defiPinAntennaModel* aModel;

    checkType(c);
    if (ud != userData) dataError();
    switch (c) {
        case defrSiteCbkType:
            site = reinterpret_cast<defiSite*>(cl);
            break;
        case defrCanplaceCbkType:
            site = reinterpret_cast<defiSite*>(cl);
            break;
        case defrCannotOccupyCbkType:
            site = reinterpret_cast<defiSite*>(cl);
            break;
        case defrDieAreaCbkType:
            box = reinterpret_cast<defiBox*>(cl);
            readDieArea(box);
            break;
        case defrPinCapCbkType:
            pc = reinterpret_cast<defiPinCap*>(cl);
            break;
        case defrPinCbkType:
            pin = reinterpret_cast<defiPin*>(cl);
            readPin(pin);
            --numObjs;
            break;
        case defrDefaultCapCbkType:
            i = (long long)cl;
            numObjs = (long)i;
            break;
        case defrRowCbkType:
            row = reinterpret_cast<defiRow*>(cl);
            readRow(row);
            break;
        case defrTrackCbkType:
            track = reinterpret_cast<defiTrack*>(cl);
            readTrack(track);
            break;
        case defrGcellGridCbkType:
            gcg = reinterpret_cast<defiGcellGrid*>(cl);
            readGcellGrid(gcg);
            break;
        case defrViaCbkType:
            via = reinterpret_cast<defiVia*>(cl);
            readVia(via);
            --numObjs;
            break;
        case defrRegionCbkType:
            re = reinterpret_cast<defiRegion*>(cl);
            readRegion(re);
            --numObjs;
            break;
        case defrGroupNameCbkType:
            break;
        case defrGroupMemberCbkType:
            saveGroupMember(reinterpret_cast<char*>(cl));
            break;
        case defrComponentMaskShiftLayerCbkType:
            break;
        case defrGroupCbkType:
            group = reinterpret_cast<defiGroup*>(cl);
            readGroup(group);
            clearGroupMember();
            --numObjs;
            break;
        case defrScanchainCbkType:
            sc = reinterpret_cast<defiScanchain*>(cl);
            readScanChain(sc);
            --numObjs;
            break;
        case defrIOTimingCbkType:
            iot = reinterpret_cast<defiIOTiming*>(cl);
            --numObjs;
            break;
        case defrFPCCbkType:
            fpc = reinterpret_cast<defiFPC*>(cl);
            --numObjs;
            break;
        case defrTimingDisableCbkType:
            td = reinterpret_cast<defiTimingDisable*>(cl);
            break;
        case defrPartitionCbkType:
            part = reinterpret_cast<defiPartition*>(cl);
            --numObjs;
            break;
        case defrPinPropCbkType:
            pprop = reinterpret_cast<defiPinProp*>(cl);
            --numObjs;
            break;
        case defrBlockageCbkType:
            block = reinterpret_cast<defiBlockage*>(cl);
            readBlockage(block);
            --numObjs;
            break;
        case defrSlotCbkType:
            slots = reinterpret_cast<defiSlot*>(cl);
            --numObjs;
            break;
        case defrFillCbkType:
            fills = reinterpret_cast<defiFill*>(cl);
            readFill(fills);
            --numObjs;
            break;
        case defrStylesCbkType:
            styles = reinterpret_cast<defiStyles*>(cl);
            --numObjs;
            break;
        default:
            return 1;
    }

    return 0;
}

int dn(defrCallbackType_e c, const char* h, defiUserData ud) {
    checkType(c);
    if (ud != userData) dataError();

    return 0;
}

int ext(defrCallbackType_e t, const char* c, defiUserData ud) {
    char* name;

    checkType(t);
    if (ud != userData) dataError();

    switch (t) {
        case defrNetExtCbkType:
            name = address("net");
            break;
        case defrComponentExtCbkType:
            name = address("component");
            break;
        case defrPinExtCbkType:
            name = address("pin");
            break;
        case defrViaExtCbkType:
            name = address("via");
            break;
        case defrNetConnectionExtCbkType:
            name = address("net connection");
            break;
        case defrGroupExtCbkType:
            name = address("group");
            break;
        case defrScanChainExtCbkType:
            name = address("scanchain");
            break;
        case defrIoTimingsExtCbkType:
            name = address("io timing");
            break;
        case defrPartitionsExtCbkType:
            name = address("partition");
            break;
        default:
            name = address("BOGUS");
            return 1;
    }
    fprintf(fout, "  %s extension %s\n", name, c);
    return 0;
}

int extension(defrCallbackType_e c, const char* extsn, defiUserData ud) {
    checkType(c);
    if (ud != userData) dataError();

    return 0;
}

void* mallocCB(size_t size) { return malloc(size); }

void* reallocCB(void* name, size_t size) { return realloc(name, size); }

static void freeCB(void* name) {
    free(name);
    return;
}

BEGIN_LEFDEF_PARSER_NAMESPACE
extern long long nlines;
END_LEFDEF_PARSER_NAMESPACE

void lineNumberCB(long long lineNo) {
    // The CCR 1131444 tests ability of the DEF parser to count
    // input line numbers out of 32-bit int range. On the first callback
    // call 10G lines will be added to line counter. It should be
    // reflected in output.
    lineNo += 10000000000LL;
    defrSetNLines(lineNo);

#ifdef _WIN32
// fprintf(fout, "Parsed %I64d number of lines!!\n", lineNo);
#else
// fprintf(fout, "Parsed %lld number of lines!!\n", lineNo);
#endif
}  // namespace EDI_DB

int unUsedCB(defrCallbackType_e, void*, defiUserData) {
    fprintf(fout, "This callback is not used.\n");
    return 0;
}

static void printWarning(const char* str) { fprintf(stderr, "%s\n", str); }

int readDef(int argc, const char** argv) {
    int num = 99;
    char* inFile[6];
    char* outFile;
    FILE* f;
    int res;
    int noCalls = 0;
    //  long start_mem;
    int retStr = 0;
    int numInFile = 0;
    int fileCt = 0;
    int noNetCb = 0;
    int line_num_print_interval = 50;

#if (defined WIN32 && _MSC_VER < 1800)
    // Enable two-digit exponent format
    _set_output_format(_TWO_DIGIT_EXPONENT);
#endif

    //  start_mem = (long)sbrk(0);

    snprintf(defaultName, sizeof(defaultName), "def.in");
    snprintf(defaultOut, sizeof(defaultOut), "list");
    inFile[0] = defaultName;
    outFile = defaultOut;
    fout = stdout;
    userData = reinterpret_cast<void*>(0x01020304);
    argc--;
    argv++;

    if (argc == 0) {
        fprintf(stderr, "Type 'defrw --help' for the help.\n");
        return 2;
    }

    while (argc--) {
        if (strcmp(*argv, "-d") == 0) {
            argv++;
            argc--;
            sscanf(*argv, "%d", &num);
            defiSetDebug(num, 1);
        } else if (strcmp(*argv, "-nc") == 0) {
            noCalls = 1;
        } else if (strcmp(*argv, "-o") == 0) {
            argv++;
            argc--;
            outFile = *(const_cast<char**>(argv));
            if ((fout = fopen(outFile, "w")) == 0) {
                fprintf(stderr, "ERROR: could not open output file\n");
                return 2;
            }
        } else if (strcmp(*argv, "-verStr") == 0) {
            /* New to set the version callback routine to return a string    */
            /* instead of double.                                            */
            retStr = 1;
        } else if (strcmp(*argv, "-i") == 0) {
            argv++;
            argc--;
            line_num_print_interval = atoi(*argv);
        } else if (argv[0][0] != '-') {
            if (numInFile >= 6) {
                fprintf(stderr, "ERROR: too many input files, max = 6.\n");
                return 2;
            }
            inFile[numInFile++] = *(const_cast<char**>(argv));
        } else if ((strcmp(*argv, "-h") == 0) ||
                   (strcmp(*argv, "--help") == 0)) {
            fprintf(stderr, "Usage: defrw (<option>|<file>)* \n");
            fprintf(stderr, "Files:\n");
            fprintf(stderr, "\tupto 6 DEF files many be supplied.\n");
            fprintf(stderr,
                    "\tif <defFileName> is set to 'STDIN' - takes data from "
                    "stdin.\n");
            fprintf(stderr, "Options:\n");
            fprintf(stderr,
                    "\t-i <num_lines> -- sets processing msg interval "
                    "(default: 50 lines).\n");
            fprintf(stderr,
                    "\t-nc            -- no functional callbacks will be "
                    "called.\n");
            fprintf(stderr, "\t-o <out_file>  -- write output to the file.\n");
            fprintf(stderr, "\t-ignoreRowNames   -- don't output row names.\n");
            fprintf(stderr, "\t-ignoreViaNames   -- don't output via names.\n");
            return 2;
        } else if (strcmp(*argv, "-setSNetWireCbk") == 0) {
            setSNetWireCbk = 1;
        } else {
            fprintf(stderr, "ERROR: Illegal command line option: '%s'\n",
                    *argv);
            return 2;
        }

        argv++;
    }

    // defrSetLogFunction(myLogFunction);
    // defrSetWarningLogFunction(myWarningLogFunction);

    if (isSessionless) {
        defrInitSession(0);
        defrSetLongLineNumberFunction(lineNumberCB);
        defrSetDeltaNumberLines(line_num_print_interval);
    }

    defrInitSession(isSessionless ? 0 : 1);

    if (noCalls == 0) {
        // defrSetWarningLogFunction(printWarning);

        defrSetUserData(reinterpret_cast<void*>(3));
        defrSetDesignCbk(dname);
        defrSetTechnologyCbk(tname);
        defrSetExtensionCbk(extension);
        defrSetDesignEndCbk(done);
        defrSetPropDefStartCbk(propstart);
        defrSetPropCbk(prop);
        defrSetPropDefEndCbk(propend);
        /* Test for CCR 766289*/
        if (!noNetCb) defrSetNetCbk(netf);
        defrSetNetNameCbk(netNamef);
        defrSetNetNonDefaultRuleCbk(nondefRulef);
        defrSetNetSubnetNameCbk(subnetNamef);
        defrSetNetPartialPathCbk(netpath);
        defrSetSNetCbk(snetf);
        defrSetSNetPartialPathCbk(snetpath);
        if (setSNetWireCbk) defrSetSNetWireCbk(snetwire);
        defrSetComponentMaskShiftLayerCbk(compMSL);
        defrSetComponentCbk(compf);
        defrSetAddPathToNet();
        defrSetHistoryCbk(hist);
        defrSetConstraintCbk(constraint);
        defrSetAssertionCbk(constraint);
        defrSetArrayNameCbk(an);
        defrSetFloorPlanNameCbk(fn);
        defrSetDividerCbk(dn);
        defrSetBusBitCbk(bbn);
        defrSetNonDefaultCbk(ndr);

        defrSetAssertionsStartCbk(constraintst);
        defrSetConstraintsStartCbk(constraintst);
        defrSetComponentStartCbk(cs);
        defrSetPinPropStartCbk(cs);
        defrSetNetStartCbk(cs);
        defrSetStartPinsCbk(cs);
        defrSetViaStartCbk(cs);
        defrSetRegionStartCbk(cs);
        defrSetSNetStartCbk(cs);
        defrSetGroupsStartCbk(cs);
        defrSetScanchainsStartCbk(cs);
        defrSetIOTimingsStartCbk(cs);
        defrSetFPCStartCbk(cs);
        defrSetTimingDisablesStartCbk(cs);
        defrSetPartitionsStartCbk(cs);
        defrSetBlockageStartCbk(cs);
        defrSetSlotStartCbk(cs);
        defrSetFillStartCbk(cs);
        defrSetNonDefaultStartCbk(cs);
        defrSetStylesStartCbk(cs);

        // All of the extensions point to the same function.
        defrSetNetExtCbk(ext);
        defrSetComponentExtCbk(ext);
        defrSetPinExtCbk(ext);
        defrSetViaExtCbk(ext);
        defrSetNetConnectionExtCbk(ext);
        defrSetGroupExtCbk(ext);
        defrSetScanChainExtCbk(ext);
        defrSetIoTimingsExtCbk(ext);
        defrSetPartitionsExtCbk(ext);

        defrSetUnitsCbk(units);
        if (!retStr)
            defrSetVersionCbk(vers);
        else
            defrSetVersionStrCbk(versStr);
        defrSetCaseSensitiveCbk(casesens);

        // The following calls are an example of using one function "cls"
        // to be the callback for many DIFFERENT types of constructs.
        // We have to cast the function type to meet the requirements
        // of each different set function.
        defrSetSiteCbk((defrSiteCbkFnType)cls);
        defrSetCanplaceCbk((defrSiteCbkFnType)cls);
        defrSetCannotOccupyCbk((defrSiteCbkFnType)cls);
        defrSetDieAreaCbk((defrBoxCbkFnType)cls);
        defrSetPinCapCbk((defrPinCapCbkFnType)cls);
        defrSetPinCbk((defrPinCbkFnType)cls);
        defrSetPinPropCbk((defrPinPropCbkFnType)cls);
        defrSetDefaultCapCbk((defrIntegerCbkFnType)cls);
        defrSetRowCbk((defrRowCbkFnType)cls);
        defrSetTrackCbk((defrTrackCbkFnType)cls);
        defrSetGcellGridCbk((defrGcellGridCbkFnType)cls);
        defrSetViaCbk((defrViaCbkFnType)cls);
        defrSetRegionCbk((defrRegionCbkFnType)cls);
        defrSetGroupNameCbk((defrStringCbkFnType)cls);
        defrSetGroupMemberCbk((defrStringCbkFnType)cls);
        defrSetGroupCbk((defrGroupCbkFnType)cls);
        defrSetScanchainCbk((defrScanchainCbkFnType)cls);
        defrSetIOTimingCbk((defrIOTimingCbkFnType)cls);
        defrSetFPCCbk((defrFPCCbkFnType)cls);
        defrSetTimingDisableCbk((defrTimingDisableCbkFnType)cls);
        defrSetPartitionCbk((defrPartitionCbkFnType)cls);
        defrSetBlockageCbk((defrBlockageCbkFnType)cls);
        defrSetSlotCbk((defrSlotCbkFnType)cls);
        defrSetFillCbk((defrFillCbkFnType)cls);
        defrSetStylesCbk((defrStylesCbkFnType)cls);

        defrSetAssertionsEndCbk(endfunc);
        defrSetComponentEndCbk(endfunc);
        defrSetConstraintsEndCbk(endfunc);
        defrSetNetEndCbk(endfunc);
        defrSetFPCEndCbk(endfunc);
        defrSetFPCEndCbk(endfunc);
        defrSetGroupsEndCbk(endfunc);
        defrSetIOTimingsEndCbk(endfunc);
        defrSetNetEndCbk(endfunc);
        defrSetPartitionsEndCbk(endfunc);
        defrSetRegionEndCbk(endfunc);
        defrSetSNetEndCbk(endfunc);
        defrSetScanchainsEndCbk(endfunc);
        defrSetPinEndCbk(endfunc);
        defrSetTimingDisablesEndCbk(endfunc);
        defrSetViaEndCbk(endfunc);
        defrSetPinPropEndCbk(endfunc);
        defrSetBlockageEndCbk(endfunc);
        defrSetSlotEndCbk(endfunc);
        defrSetFillEndCbk(endfunc);
        defrSetNonDefaultEndCbk(endfunc);
        defrSetStylesEndCbk(endfunc);

        defrSetMallocFunction(mallocCB);
        defrSetReallocFunction(reallocCB);
        defrSetFreeFunction(freeCB);

        // defrSetRegisterUnusedCallbacks();

        // Testing to set the number of warnings
        defrSetAssertionWarnings(3);
        defrSetBlockageWarnings(3);
        defrSetCaseSensitiveWarnings(3);
        defrSetComponentWarnings(3);
        defrSetConstraintWarnings(0);
        defrSetDefaultCapWarnings(3);
        defrSetGcellGridWarnings(3);
        defrSetIOTimingWarnings(3);
        defrSetNetWarnings(3);
        defrSetNonDefaultWarnings(3);
        defrSetPinExtWarnings(3);
        defrSetPinWarnings(3);
        defrSetRegionWarnings(3);
        defrSetRowWarnings(3);
        defrSetScanchainWarnings(3);
        defrSetSNetWarnings(3);
        defrSetStylesWarnings(3);
        defrSetTrackWarnings(3);
        defrSetUnitsWarnings(3);
        defrSetVersionWarnings(3);
        defrSetViaWarnings(3);
    }

    if (!isSessionless) {
        defrSetLongLineNumberFunction(lineNumberCB);
        defrSetDeltaNumberLines(line_num_print_interval);
    }

    (void)defrSetOpenLogFileAppend();

    message->info("\nReading DEF\n");
    fflush(stdout);
    for (fileCt = 0; fileCt < numInFile; fileCt++) {
        if (strcmp(inFile[fileCt], "STDIN") == 0) {
            f = stdin;
        } else if ((f = fopen(inFile[fileCt], "r")) == 0) {
            fprintf(stderr, "Couldn't open input file '%s'\n", inFile[fileCt]);
            return (2);
        }
        // Set case sensitive to 0 to start with,
        // in History & PropertyDefinition
        // reset it to 1.

        res = defrRead(f, inFile[fileCt], userData, 1);

        if (res) fprintf(stderr, "Reader returns bad status.\n");

        // Testing the aliases API.
        defrAddAlias("alias1", "aliasValue1", 1);

        defiAlias_itr aliasStore;
        const char* alias1Value = NULL;

        while (aliasStore.Next()) {
            if (strcmp(aliasStore.Key(), "alias1") == 0) {
                alias1Value = aliasStore.Data();
            }
        }

        if (!alias1Value || strcmp(alias1Value, "aliasValue1")) {
            fprintf(stderr, "ERROR: Aliases don't work\n");
        }

        (void)defrPrintUnusedCallbacks(fout);
        (void)defrReleaseNResetMemory();
    }
    (void)defrUnsetCallbacks();
    (void)defrSetUnusedCallbacks(unUsedCB);

    // Unset all the callbacks
    defrUnsetArrayNameCbk();
    defrUnsetAssertionCbk();
    defrUnsetAssertionsStartCbk();
    defrUnsetAssertionsEndCbk();
    defrUnsetBlockageCbk();
    defrUnsetBlockageStartCbk();
    defrUnsetBlockageEndCbk();
    defrUnsetBusBitCbk();
    defrUnsetCannotOccupyCbk();
    defrUnsetCanplaceCbk();
    defrUnsetCaseSensitiveCbk();
    defrUnsetComponentCbk();
    defrUnsetComponentExtCbk();
    defrUnsetComponentStartCbk();
    defrUnsetComponentEndCbk();
    defrUnsetConstraintCbk();
    defrUnsetConstraintsStartCbk();
    defrUnsetConstraintsEndCbk();
    defrUnsetDefaultCapCbk();
    defrUnsetDesignCbk();
    defrUnsetDesignEndCbk();
    defrUnsetDieAreaCbk();
    defrUnsetDividerCbk();
    defrUnsetExtensionCbk();
    defrUnsetFillCbk();
    defrUnsetFillStartCbk();
    defrUnsetFillEndCbk();
    defrUnsetFPCCbk();
    defrUnsetFPCStartCbk();
    defrUnsetFPCEndCbk();
    defrUnsetFloorPlanNameCbk();
    defrUnsetGcellGridCbk();
    defrUnsetGroupCbk();
    defrUnsetGroupExtCbk();
    defrUnsetGroupMemberCbk();
    defrUnsetComponentMaskShiftLayerCbk();
    defrUnsetGroupNameCbk();
    defrUnsetGroupsStartCbk();
    defrUnsetGroupsEndCbk();
    defrUnsetHistoryCbk();
    defrUnsetIOTimingCbk();
    defrUnsetIOTimingsStartCbk();
    defrUnsetIOTimingsEndCbk();
    defrUnsetIOTimingsExtCbk();
    defrUnsetNetCbk();
    defrUnsetNetNameCbk();
    defrUnsetNetNonDefaultRuleCbk();
    defrUnsetNetConnectionExtCbk();
    defrUnsetNetExtCbk();
    defrUnsetNetPartialPathCbk();
    defrUnsetNetSubnetNameCbk();
    defrUnsetNetStartCbk();
    defrUnsetNetEndCbk();
    defrUnsetNonDefaultCbk();
    defrUnsetNonDefaultStartCbk();
    defrUnsetNonDefaultEndCbk();
    defrUnsetPartitionCbk();
    defrUnsetPartitionsExtCbk();
    defrUnsetPartitionsStartCbk();
    defrUnsetPartitionsEndCbk();
    defrUnsetPathCbk();
    defrUnsetPinCapCbk();
    defrUnsetPinCbk();
    defrUnsetPinEndCbk();
    defrUnsetPinExtCbk();
    defrUnsetPinPropCbk();
    defrUnsetPinPropStartCbk();
    defrUnsetPinPropEndCbk();
    defrUnsetPropCbk();
    defrUnsetPropDefEndCbk();
    defrUnsetPropDefStartCbk();
    defrUnsetRegionCbk();
    defrUnsetRegionStartCbk();
    defrUnsetRegionEndCbk();
    defrUnsetRowCbk();
    defrUnsetScanChainExtCbk();
    defrUnsetScanchainCbk();
    defrUnsetScanchainsStartCbk();
    defrUnsetScanchainsEndCbk();
    defrUnsetSiteCbk();
    defrUnsetSlotCbk();
    defrUnsetSlotStartCbk();
    defrUnsetSlotEndCbk();
    defrUnsetSNetWireCbk();
    defrUnsetSNetCbk();
    defrUnsetSNetStartCbk();
    defrUnsetSNetEndCbk();
    defrUnsetSNetPartialPathCbk();
    defrUnsetStartPinsCbk();
    defrUnsetStylesCbk();
    defrUnsetStylesStartCbk();
    defrUnsetStylesEndCbk();
    defrUnsetTechnologyCbk();
    defrUnsetTimingDisableCbk();
    defrUnsetTimingDisablesStartCbk();
    defrUnsetTimingDisablesEndCbk();
    defrUnsetTrackCbk();
    defrUnsetUnitsCbk();
    defrUnsetVersionCbk();
    defrUnsetVersionStrCbk();
    defrUnsetViaCbk();
    defrUnsetViaExtCbk();
    defrUnsetViaStartCbk();
    defrUnsetViaEndCbk();

    // fclose(fout);

    // Release allocated singleton data.
    defrClear();

    message->info("\nRead DEF successfully.\n");
    return res;
}
int readDieArea(defiBox* io_die_area) {
    if (!io_die_area) {
        message->issueMsg(kError, "io die area is null pointer.\n");
        return ERROR;
    }
    Cell* top_cell = getTopCell();
    Floorplan* floorplan = top_cell->getFloorplan();
    if (!floorplan) {
        message->issueMsg(kError, "Cannot find floorplan when read DEF row.\n");
        return ERROR;
    }

    defiPoints points = io_die_area->getPoint();

    PolygonTable* polygon_table = top_cell->getPolygonTable();
    if (!polygon_table) {
        message->issueMsg(kError,
                          "polygon table is null pointer in top cell.\n");
        return ERROR;
    }
    Polygon* polygon = new Polygon;
    for (int i = 0; i < points.numPoints; ++i) {
        Point* point = new Point(points.x[i], points.y[i]);
        polygon->addPoint(point);
    }
    ObjectIndex polygon_index = polygon_table->addPolygon(polygon);
    floorplan->setDieArea(polygon_index);

    return OK;
}

int readVia(defiVia* io_via) {
    char *via_rule_name, *lower_layer_name, *cut_layer_name, *upper_layer_name;
    int cut_size_x, cut_size_y, cut_space_x, cut_space_y, lower_enclosure_x,
        lower_enclosure_y, upper_enclosure_x, upper_enclosure_y;
    int row, col, origin_offset_x, origin_offset_y, lower_offset_x,
        lower_offset_y, upper_offset_x, upper_offset_y;
    std::string io_via_name = io_via->name();
    Tech* lib = getTopCell()->getTechLib();

    (void)io_via->viaRule(&via_rule_name, &cut_size_x, &cut_size_y,
                          &lower_layer_name, &cut_layer_name, &upper_layer_name,
                          &cut_space_x, &cut_space_y, &lower_enclosure_x,
                          &lower_enclosure_y, &upper_enclosure_x,
                          &upper_enclosure_y);

    ViaMaster* db_via = lib->createAndAddViaMaster(io_via_name);

    // setting common

    db_via->setIsFromDEF(1);

    if (io_via->hasViaRule()) {
        // db_via->hasViaRule(1);
        db_via->setViaRule(via_rule_name);

        db_via->setCutSizeX(cut_size_x);
        db_via->setCutSizeY(cut_size_y);

        db_via->setLowerLayerIndex(lower_layer_name);
        db_via->setCutLayerIndex(cut_layer_name);
        db_via->setUperLayerIndex(upper_layer_name);

        db_via->setCutSpacingX(cut_space_x);
        db_via->setCutSpacingY(cut_space_y);

        db_via->setLowerEncX(lower_enclosure_x);
        db_via->setLowerEncY(lower_enclosure_y);
        db_via->setUpperEncX(upper_enclosure_x);
        db_via->setUpperEncY(upper_enclosure_y);

        db_via->setIsArray(io_via->hasRowCol());
        if (io_via->hasRowCol()) {
            (void)io_via->rowCol(&row, &col);
            db_via->setRow(row);
            db_via->setCol(col);
        }

        db_via->setHasOrigin(io_via->hasOrigin());
        if (io_via->hasOrigin()) {
            (void)io_via->origin(&origin_offset_x, &origin_offset_y);
            db_via->setOffsetX(origin_offset_x);
            db_via->setOffsetY(origin_offset_y);
        }

        db_via->setHasOffset(io_via->hasOffset());
        if (io_via->hasOffset()) {
            (void)io_via->offset(&lower_offset_x, &lower_offset_y,
                                 &upper_offset_x, &upper_offset_y);
            db_via->setLowerOffsetX(lower_offset_x);
            db_via->setLowerOffsetY(lower_offset_y);
            db_via->setUpperOffsetX(upper_offset_x);
            db_via->setUpperOffsetY(upper_offset_y);
        }

        db_via->setHasCutPatten(io_via->hasCutPattern());
        if (io_via->hasCutPattern()) {
            db_via->setCutPatten(const_cast<char*>(io_via->cutPattern()));
        }
    } else {  // fixed via type
        if (io_via->hasPattern()) {
            std::string pattern_name = io_via->pattern();
            db_via->setPattern(pattern_name);
        }
        for (int i = 0; i < io_via->numLayers(); ++i) {
            char* layer_name;
            int mask, xl, yl, xh, yh;
            io_via->layer(i, &layer_name, &xl, &yl, &xh, &yh);
            std::string via_layer_name = layer_name;
            ViaLayer* via_layer = db_via->creatViaLayer(via_layer_name);
            Box* box = creatBox(xl, yl, xh, yh);
            via_layer->addMask(io_via->rectMask(i));
            via_layer->addRect(box);
            db_via->addViaLayer(via_layer);
        }
    }
    // lib->addViaMaster(db_via);
    // db_via->print();
    return 0;
}

int getSpecialWireRouteStatus(const char* status) {
    if (strcmp(status, "COVER") == 0) return 1;
    if (strcmp(status, "FIXED") == 0) return 2;
    if (strcmp(status, "ROUTED") == 0) return 3;
    if (strcmp(status, "SHIELD") == 0) return 4;
    return 0;
}

int getRegularWireRouteStatus(const char* status) {
    if (strcmp(status, "COVER") == 0) return 1;
    if (strcmp(status, "FIXED") == 0) return 2;
    if (strcmp(status, "ROUTED") == 0) return 3;
    if (strcmp(status, "NOSHIELD") == 0) return 4;
    return 0;
}

int readSubNet(defiNet* io_net, Net* net) {
    defiSubnet* io_sub_net;
    Net* sub_net;
    Cell* top_cell = getTopCell();
    Tech* lib = top_cell->getTechLib();

    if (io_net->hasSubnets()) {
        for (int i = 0; i < io_net->numSubnets(); i++) {
            io_sub_net = io_net->subnet(i);
            std::string sub_net_name = io_sub_net->name();
            sub_net = net->createSubNet(sub_net_name);
            sub_net->setIsSubNet(1);
            sub_net->setCell(top_cell->getId());

            if (io_sub_net->hasNonDefaultRule()) {
                std::string rule_name = io_sub_net->nonDefaultRule();
                sub_net->setNonDefaultRule(
                    lib->getNonDefaultRuleIdByName(rule_name.c_str()));
            }
            // set pin
            for (i = 0; i < io_net->numConnections(); i++) {
                Pin* pin = nullptr;
                std::string inst_name = io_net->instance(i);

                if (inst_name.compare("PIN")) {
                    pin = top_cell->getIOPin(io_net->pin(i));
                } else {
                    Inst* inst = top_cell->getInstance(inst_name.c_str());
                    if (inst) pin = inst->getPin(io_net->pin(i));
                }
                if (!pin) pin = top_cell->getIOPin(io_net->pin(i));

                // if (pin) sub_net->addPin(pin);
            }
            // regular wiring
            if (io_sub_net->numWires()) {
                for (int k = 0; k < io_sub_net->numWires(); k++) {
                    defiWire* wire = io_sub_net->wire(k);
                    readWire(wire, sub_net);
                }
            }
            net->addSubNet(sub_net);
        }
    }
    return 0;
}

int readWire(defiWire* io_wire, Net* net) {
    int status = getRegularWireRouteStatus(io_wire->wireType());
    Tech* lib = getTopCell()->getTechLib();
    std::string layername;
    int new_layer = 0;
    int next_path_type = 0;
    int node_num = 0, mask_num = 0, via_mask_num = 0;
    int w = 0, x = 0, y = 0, z = 0, ext = 0;
    int layer_num = 0;
    int width = 0;
    int new_section_start_flag = 0;
    int via_upper_mask = 0, via_cut_mask = 0, via_lower_mask = 0;
    bool hash_mask = false;
    bool hash_via_mask = false;
    int hash_taper = 0;
    int style = 0;
    int is_start_node = 0;
    std::string taperRule = "";

    WirePatch* patch = nullptr;
    WireNode *wire_node_prev = nullptr, *wire_node_current = nullptr;
    WireGraph* wire_graph = nullptr;
    Wire* edge = nullptr;

    wire_graph = net->creatGraph();
    net->addGraph(wire_graph);
    wire_graph->setStatus(status);

    for (int j = 0; j < io_wire->numPaths(); j++) {
        defiPath* p = io_wire->path(j);
        p->initTraverse();
        while ((next_path_type = static_cast<int>(p->next())) !=
               DEFIPATH_DONE) {
            switch (next_path_type) {
                case DEFIPATH_LAYER:
                    layername = p->getLayer();
                    is_start_node = 1;
                    if (new_layer == 0 && node_num != 0) {
                        new_layer = 1;
                    }
                    layer_num = lib->getLayerLEFIndexByName(layername.c_str());
                    break;
                case DEFIPATH_TAPER:
                    hash_taper = 1;
                    break;
                case DEFIPATH_STYLE:
                    style = p->getStyle();
                    break;
                case DEFIPATH_TAPERRULE:
                    taperRule = p->getTaperRule();
                    break;
                case DEFIPATH_POINT:
                    p->getPoint(&x, &y);
                    if (node_num == 0) {
                        wire_node_current =
                            wire_graph->creatWireNode(x, y, layer_num);
                        wire_graph->addWireNode(wire_node_current);
                    } else {
                        wire_node_current =
                            wire_graph->creatWireNode(x, y, layer_num);
                        wire_graph->addWireNode(wire_node_current);
                        if (!new_layer) {
                            edge = wire_graph->creatWireEdge(wire_node_prev,
                                                             wire_node_current);
                            if (edge) {
                                wire_node_prev->addOutEdgeList(edge);
                                edge->setNet(net);
                            }
                        }
                    }

                    if (hash_mask) {  // set mask
                        wire_node_current->setMask(mask_num);
                        hash_mask = false;
                    }

                    if (is_start_node) {
                        wire_node_current->setIsStartNode(1);
                        wire_node_current->setIsNewLayer(new_layer);
                        wire_node_current->setStyle(style);
                        if (hash_taper) wire_node_current->setHasTaper(1);
                        if (taperRule.size() != 0) {
                            wire_node_current->setTaperRule(taperRule.c_str());
                            wire_node_current->setHasTaperRule(1);
                        }
                        is_start_node = 0;
                        new_layer = 0;
                        hash_taper = 0;
                        taperRule = "";
                        style = 0;
                    }

                    wire_node_prev = wire_node_current;
                    node_num++;
                    break;
                case DEFIPATH_FLUSHPOINT:
                    p->getFlushPoint(&x, &y, &ext);

                    if (node_num == 0) {
                        wire_node_current =
                            wire_graph->creatWireNode(x, y, layer_num);
                        wire_graph->addWireNode(wire_node_current);
                    } else {
                        wire_node_current =
                            wire_graph->creatWireNode(x, y, layer_num);
                        wire_graph->addWireNode(wire_node_current);
                        if (!new_layer) {
                            edge = wire_graph->creatWireEdge(wire_node_prev,
                                                             wire_node_current);
                            if (edge) {
                                edge->setNet(net);
                                wire_node_prev->addOutEdgeList(edge);
                            }
                        }
                    }

                    if (hash_mask) {  // set mask
                        wire_node_current->setMask(mask_num);
                        hash_mask = false;
                    }

                    if (is_start_node) {
                        wire_node_current->setStyle(style);
                        wire_node_current->setIsStartNode(1);
                        wire_node_current->setIsNewLayer(new_layer);
                        if (hash_taper) wire_node_current->setHasTaper(1);
                        if (taperRule.size() != 0) {
                            wire_node_current->setTaperRule(taperRule.c_str());
                            wire_node_current->setHasTaperRule(1);
                        }
                        is_start_node = 0;
                        new_layer = 0;
                        style = 0;
                        hash_taper = 0;
                        taperRule = "";
                    }

                    wire_node_current->setExtension(ext);

                    wire_node_prev = wire_node_current;
                    node_num++;
                    break;
                case DEFIPATH_MASK:
                    mask_num = p->getMask();
                    hash_mask = true;
                    break;
                case DEFIPATH_VIAMASK:
                    hash_via_mask = true;
                    via_upper_mask = p->getViaTopMask();
                    via_cut_mask = p->getViaCutMask();
                    via_lower_mask = p->getViaBottomMask();
                    break;
                case DEFIPATH_VIA:
                    wire_node_prev->setIsVia(1);
                    wire_node_prev->setViaName(const_cast<char*>(p->getVia()));
                    break;
                case DEFIPATH_VIAROTATION:
                    fprintf(fout, "% ", orientStr(p->getViaRotation()));
                    break;
                case DEFIPATH_RECT:
                    p->getViaRect(&w, &x, &y, &z);
                    patch = wire_node_current->creatPatch(w, x, y, z);
                    break;
                case DEFIPATH_VIRTUALPOINT:
                    p->getVirtualPoint(&x, &y);
                    wire_node_current =
                        wire_graph->creatWireNode(x, y, layer_num);
                    wire_node_current->setIsVirtul(1);
                    wire_graph->addWireNode(wire_node_current);
                    edge = wire_graph->creatWireEdge(wire_node_prev,
                                                     wire_node_current);
                    if (edge) {
                        edge->setNet(net);
                        wire_node_prev->addOutEdgeList(edge);
                    }
                    wire_node_prev = wire_node_current;
                    break;
                case DEFIPATH_WIDTH:
                    width = p->getWidth();
                    break;
            }
        }
    }
    return 0;
}

int getVPinRouteStatus(char status) {
    if (status == 'P') return 1;
    if (status == 'F') return 2;
    if (status == 'C') return 3;
    return 0;
}

int getOrientStatus(const char* status) {
    if (strcmp(status, "N") == 0) return 1;
    if (strcmp(status, "S") == 0) return 2;
    if (strcmp(status, "E") == 0) return 3;
    if (strcmp(status, "W") == 0) return 4;
    if (strcmp(status, "FN") == 0) return 5;
    if (strcmp(status, "FS") == 0) return 6;
    if (strcmp(status, "FE") == 0) return 7;
    if (strcmp(status, "FW") == 0) return 8;
    return 0;
}

int getShapeStatus(const char* status) {
    if (strcmp(status, "RING") == 0) return 1;
    if (strcmp(status, "PADRING") == 0) return 2;
    if (strcmp(status, "BLOCKRING") == 0) return 3;
    if (strcmp(status, "STRIPE") == 0) return 4;
    if (strcmp(status, "FOLLOWPIN") == 0) return 5;
    if (strcmp(status, "IOWIRE") == 0) return 6;
    if (strcmp(status, "COREWIRE") == 0) return 7;
    if (strcmp(status, "BLOCKWIRE") == 0) return 8;
    if (strcmp(status, "BLOCKAGEWIRE") == 0) return 9;
    if (strcmp(status, "FILLWIRE") == 0) return 10;
    if (strcmp(status, "FILLWIREOPC") == 0) return 11;
    if (strcmp(status, "DRCFILL") == 0) return 12;
    return 0;
}

int getSourceStatus(const char* status) {
    if (strcmp(status, "DIST") == 0) return 1;
    if (strcmp(status, "NETLIST") == 0) return 2;
    if (strcmp(status, "TEST") == 0) return 3;
    if (strcmp(status, "TIMING") == 0) return 4;
    if (strcmp(status, "USER") == 0) return 5;
    return 0;
}

NetType getNetType(const char* type) {
    if (strcmp(type, "ANALOG") == 0) return NetType::kNetTypeAnalog;
    if (strcmp(type, "CLOCK") == 0) return NetType::kNetTypeClock;
    if (strcmp(type, "GROUND") == 0) return NetType::kNetTypeGround;
    if (strcmp(type, "POWER") == 0) return NetType::kNetTypePower;
    if (strcmp(type, "RESET") == 0) return NetType::kNetTypeReset;
    if (strcmp(type, "SCAN") == 0) return NetType::kNetTypeScan;
    if (strcmp(type, "SIGNAL") == 0) return NetType::kNetTypeSignal;
    if (strcmp(type, "TIEOFF") == 0) return NetType::kNetTypeTieOff;
    return NetType::kNetTypeUnknown;
}

SpecialNetType getSpecialNetType(const char* type) {
    if (strcmp(type, "ANALOG") == 0) return SpecialNetType::kSpecialNetTypeAnalog;
    if (strcmp(type, "CLOCK") == 0) return SpecialNetType::kSpecialNetTypeClock;
    if (strcmp(type, "GROUND") == 0) return SpecialNetType::kSpecialNetTypeGround;
    if (strcmp(type, "POWER") == 0) return SpecialNetType::kSpecialNetTypePower;
    if (strcmp(type, "RESET") == 0) return SpecialNetType::kSpecialNetTypeReset;
    if (strcmp(type, "SCAN") == 0) return SpecialNetType::kSpecialNetTypeScan;
    if (strcmp(type, "SIGNAL") == 0) return SpecialNetType::kSpecialNetTypeSignal;
    if (strcmp(type, "TIEOFF") == 0) return SpecialNetType::kSpecialNetTypeTieOff;
    return SpecialNetType::kSpecialNetTypeUnknown;
}

int getNetPattern(const char* pattern) {
    if (strcmp(pattern, "BALANCED") == 0) return 1;
    if (strcmp(pattern, "STEINER") == 0) return 2;
    if (strcmp(pattern, "TRUNK") == 0) return 3;
    if (strcmp(pattern, "WIREDLOGIC") == 0) return 4;
    return 0;
}

int readRectSpecialWire(defiNet* net) {
    Tech* lib = getTopCell()->getTechLib();
    SpecialWire* wire = nullptr;
    Via* via = nullptr;
    if (net->numRectangles()) {
        for (int i = 0; i < net->numRectangles(); i++) {
            wire = new SpecialWire();
            int status = getSpecialWireRouteStatus(net->rectRouteStatus(i));
            wire->setStatus(status);

            if (strcmp(net->rectShapeType(i), "") != 0) {
                wire->setShapeName(const_cast<char*>(net->rectShapeType(i)));
            }

            wire->setMask(net->rectMask(i));
            Box box = Box(net->xl(i), net->yl(i), net->xh(i), net->yh(i));
            wire->setBox(box);
        }
    }

    if (net->numViaSpecs()) {
        for (int i = 0; i < net->numViaSpecs(); i++) {
            ViaMaster* via_master = lib->getViaMaster(net->viaName(i));
            if (via_master) {
                // via_master->getViaLayer(0)->addMask(net->topMaskNum(i));
                // via_master->getViaLayer(1)->addMask(net->cutMaskNum(i));
                // via_master->getViaLayer(2)->addMask(net->bottomMaskNum(i));
            }
            int status = getSpecialWireRouteStatus(net->rectRouteStatus(i));
            int orient = getOrientStatus(net->viaOrientStr(i));
            defiPoints points = net->getViaPts(i);
            for (int jj = 0; jj < points.numPoints; jj++) {
                via = new Via();
                if (via_master) via->setMaster(via_master);
                Point point = Point(points.x[jj], points.y[jj]);
                via->setLoc(point);
                via->setRouteStatus(status);
            }
        }
        // wire->print();
    }
    return 0;
}

int readBlockage(defiBlockage* io_blockage) {
    if (!io_blockage) {
        message->issueMsg(kError, "io blockage is null pointer.\n");
        return ERROR;
    }
    Cell* top_cell = getTopCell();
    Floorplan* floorplan = top_cell->getFloorplan();
    if (!floorplan) {
        message->issueMsg(kError,
                          "Cannot find floorplan when read DEF blockage.\n");
        return ERROR;
    }
    Tech* tech_lib = top_cell->getTechLib();
    if (!tech_lib) {
        message->issueMsg(kError, "Cannot find Tech LEF when DEF blockage.\n");
        return ERROR;
    }

    if (io_blockage->hasLayer()) {
        Int32 layer_id = tech_lib->getLayerLEFIndexByName(io_blockage->layerName());
        if (layer_id < 0) {
            message->issueMsg(kError, "Cannot find layer %s in LEF.\n",
                              io_blockage->layerName());
            return ERROR;
        }
    }

    if (io_blockage->hasComponent()) {
        Inst* instance =
            top_cell->getInstance(io_blockage->layerComponentName());
        if (!instance) {
            message->issueMsg(kError, "Cannot find component %s in blockage.\n",
                              io_blockage->layerComponentName());
            return ERROR;
        }
    }

    Constraint* blockage = nullptr;
    if (io_blockage->hasPlacement()) {
        blockage = floorplan->createPlaceBlockage();
    } else {
        blockage = floorplan->createRouteBlockage();
    }
    if (!blockage) {
        message->issueMsg(kError, "Create blockage failed.\n");
        return ERROR;
    }

    if (io_blockage->hasLayer()) {
        blockage->setLayer(io_blockage->layerName());
        blockage->setHasLayer(true);
    }
    blockage->setPlacement((io_blockage->hasPlacement() != 0) ? true : false);
    if (io_blockage->hasComponent()) {
        blockage->setComponent(io_blockage->layerComponentName());
        blockage->setHasComponent(true);
    }
    blockage->setSlots((io_blockage->hasSlots() != 0) ? true : false);
    blockage->setFills((io_blockage->hasFills() != 0) ? true : false);
    blockage->setPushdown((io_blockage->hasPushdown() != 0) ? true : false);
    blockage->setExceptpgnet((io_blockage->hasExceptpgnet() != 0) ? true
                                                                  : false);
    blockage->setSoft((io_blockage->hasSoft() != 0) ? true : false);
    if (io_blockage->hasPartial()) {
        blockage->setPartial(true);
        blockage->setDensity(io_blockage->placementMaxDensity());
    }
    if (io_blockage->hasSpacing()) {
        blockage->setSpacing(true);
        blockage->setMinSpacing(io_blockage->minSpacing());
    }
    if (io_blockage->hasDesignRuleWidth()) {
        blockage->setDesignRuleWidth(true);
        blockage->setEffectiveWidth(io_blockage->designRuleWidth());
    }
    if (io_blockage->hasMask()) {
        blockage->setMask(true);
        blockage->setMaskNum(io_blockage->mask());
    }

    for (int i = 0; i < io_blockage->numRectangles(); ++i) {
        blockage->createBox(io_blockage->xl(i), io_blockage->yl(i),
                            io_blockage->xh(i), io_blockage->yh(i));
    }
    for (int i = 0; i < io_blockage->numPolygons(); ++i) {
        Polygon* polygon = new Polygon;
        defiPoints points = io_blockage->getPolygon(i);
        for (int j = 0; j < points.numPoints; ++j) {
            Point* point = new Point(points.x[j], points.y[j]);
            polygon->addPoint(point);
        }
        blockage->addPolygon(polygon);
    }

    return OK;
}

int readSpecialWire(defiWire* io_wire, SpecialNet* net) {
    Tech* lib = getTopCell()->getTechLib();
    std::string layername;
    int numX = 0, numY = 0, stepX = 0, stepY = 0;
    int new_layer = 0;
    int next_path_type = 0;
    int node_num = 0, mask_num = 0, via_mask_num = 0;
    int w = 0, x = 0, y = 0, z = 0, ext = 0;
    int layer_num = 0;
    int new_section_start_flag = 0;
    int via_upper_mask = 0, via_cut_mask = 0, via_lower_mask = 0;
    bool hash_mask = false;
    bool hash_via_mask = false;
    int shape = 0;
    int width = 0;

    Via* via = nullptr;
    ViaMaster* via_master = nullptr;
    SpecialWireNode *wire_node_prev = nullptr, *wire_node_current = nullptr;
    SpecialWireGraph* wire_graph = nullptr;
    SpecialWireEdge* edge = nullptr;
    SpecialWireSection* wire_section;
    wire_section = net->createWireSection();
    wire_section->setStatus(getSpecialWireRouteStatus(io_wire->wireType()));
    if (4 == wire_section->getStatus()) {
        wire_section->setShieldNetName(io_wire->wireShieldNetName());
    }
    for (int j = 0; j < io_wire->numPaths(); j++) {
        defiPath* p = io_wire->path(j);
        p->initTraverse();
        while ((next_path_type = static_cast<int>(p->next())) !=
               DEFIPATH_DONE) {
            switch (next_path_type) {
                case DEFIPATH_LAYER:
                    layername = p->getLayer();
                    wire_graph = wire_section->creatGraph();
                    node_num = 0;
                    wire_section->addGraph(wire_graph);
                    if (new_layer == 0) {
                        wire_graph->setIsNewLayer(0);
                        new_layer = 1;
                    } else {
                        wire_graph->setIsNewLayer(1);
                    }
                    layer_num = lib->getLayerLEFIndexByName(layername.c_str());
                    break;
                case DEFIPATH_SHAPE:
                    shape = getShapeStatus(p->getShape());
                    wire_graph->setShape(shape);
                    break;
                case DEFIPATH_STYLE:
                    wire_graph->setStyle(p->getStyle());
                    break;
                case DEFIPATH_POINT:
                    p->getPoint(&x, &y);
                    wire_node_current =
                        wire_graph->createSpecialWireNode(x, y, layer_num);
                    wire_graph->addWireNode(wire_node_current);
                    if (hash_mask) {  // set mask
                        wire_node_current->setMask(mask_num);
                        hash_mask = false;
                    }

                    if (width != 0 && node_num == 0) {
                        wire_graph->setWidth(width);
                        width = 0;
                    }

                    if (node_num != 0) {
                        edge = new SpecialWireEdge(wire_node_prev,
                                                   wire_node_current);
                        wire_node_prev->addOutEdgeList(edge);
                    }
                    wire_node_prev = wire_node_current;
                    node_num++;
                    break;
                case DEFIPATH_FLUSHPOINT:
                    p->getFlushPoint(&x, &y, &ext);
                    wire_node_current =
                        wire_graph->createSpecialWireNode(x, y, layer_num);
                    wire_graph->addWireNode(wire_node_current);
                    wire_node_current->setExtension(ext);
                    if (hash_mask) {  // set mask
                        wire_node_current->setMask(mask_num);
                        hash_mask = false;
                    }

                    if (width != 0 && node_num == 0) {
                        wire_graph->setWidth(width);
                        width = 0;
                    }

                    if (node_num != 0) {
                        edge = new SpecialWireEdge(wire_node_prev,
                                                   wire_node_current);
                        wire_node_prev->addOutEdgeList(edge);
                    }
                    wire_node_prev = wire_node_current;
                    node_num++;
                    break;
                case DEFIPATH_MASK:
                    mask_num = p->getMask();
                    hash_mask = true;
                    break;
                case DEFIPATH_VIAMASK:
                    hash_via_mask = true;
                    via_upper_mask = p->getViaTopMask();
                    via_cut_mask = p->getViaCutMask();
                    via_lower_mask = p->getViaBottomMask();
                    break;
                case DEFIPATH_VIA:
                    via_master = lib->getViaMaster(p->getVia());
                    if (via_master) {
                        wire_node_current->setIsVia(1);
                        via = new Via(via_master);
                        wire_node_current->setVia(via);
                    } else {
                        message->info("ERROR: cannot find the via: %s\n",
                                      p->getVia());
                    }
                    break;
                case DEFIPATH_VIAROTATION:
                    if (via) via->setOrient(p->getViaRotation());
                    break;
                case DEFIPATH_WIDTH:
                    width = p->getWidth();
                    break;
                case DEFIPATH_VIADATA:
                    p->getViaData(&numX, &numY, &stepX, &stepY);
                    if (via) {
                        via->setIsArray(1);
                        via->setCol(numX);
                        via->setRow(numY);
                        via->setSpaceX(stepX);
                        via->setSpaceY(stepY);
                    }
                    break;
            }
        }
    }

    net->addWireSection(wire_section);
    return 0;
}

int readSpecialNet(defiNet* io_net) {
    int i, j, k, w, x, y, z, count, newLayer;
    defiPath* p;
    defiSubnet* s;
    int path;
    defiWire* wire;
    SpecialNet* net;
    Point* loc;
    Box* bbox;
    Cell* top_cell = getTopCell();
    Tech* lib = top_cell->getTechLib();
    net = top_cell->createSpecialNet(current_net_name);

    // set pin
    for (i = 0; i < io_net->numConnections(); i++) {
        Pin* pin = nullptr;
        std::string inst_name = io_net->instance(i);

        if (inst_name.compare("PIN")) {
            pin = top_cell->getIOPin(io_net->pin(i));
        } else {
            Inst* inst = top_cell->getInstance(inst_name.c_str());
            if (inst) pin = inst->getPin(io_net->pin(i));
        }
        if (pin) net->addPin(pin);
    }

    // specialWiring
    if (io_net->numWires()) {
        newLayer = 0;
        for (i = 0; i < io_net->numWires(); i++) {
            newLayer = 0;
            wire = io_net->wire(i);
            readSpecialWire(wire, net);
            count = 0;
        }
    }

    // RECT

    readRectSpecialWire(io_net);

    // NDR

    if (io_net->hasNonDefaultRule()) {
        std::string rule_name = io_net->nonDefaultRule();
        net->setNonDefaultRule(
            lib->getNonDefaultRuleIdByName(rule_name.c_str()));
    }

    if (io_net->hasUse()) {
        SpecialNetType type = getSpecialNetType(io_net->use());
        net->setType(type);
    }
    if (io_net->hasSource()) {
        int source = getSourceStatus(io_net->source());
        net->setSource(source);
    }

    if (io_net->hasOriginal()) {
        SpecialNet* origin_net = top_cell->getSpecialNet(io_net->original());
        if (net) net->setOriginNet(origin_net);
    }

    if (io_net->hasVoltage()) net->setVoltage(io_net->voltage());
    if (io_net->hasFixedbump()) net->setFixBump(1);
    if (io_net->hasFrequency()) {
        net->setFrequency(static_cast<int>(io_net->frequency()));
    }
    if (io_net->hasPattern()) net->setPattern(getNetPattern(io_net->pattern()));
    if (io_net->hasCap()) net->setCapacitance(io_net->cap());
    if (io_net->hasWeight()) net->setWeight(io_net->weight());

    readProperties<defiNet, SpecialNet>(PropType::kSpecialNet,
                                        reinterpret_cast<void*>(io_net),
                                        reinterpret_cast<void*>(net));

    return 0;
}

int readNet(defiNet* io_net) {
    int i, j, k, w, x, y, z, count, newLayer;
    defiPath* p;
    defiSubnet* s;
    int path;
    defiVpin* io_vpin;
    defiWire* wire;
    Net* net;
    VPin* v_pin;
    Point* loc;
    Box* bbox;
    Cell* top_cell = getTopCell();
    Tech* lib = top_cell->getTechLib();

    net = top_cell->createNet(current_net_name);
    if (net == 0) {
        return -1;
    }
    net->setCell(top_cell->getId());

    if (io_net->pinIsMustJoin(0)) net->setMustJoin(1);

    // set pin
    for (i = 0; i < io_net->numConnections(); i++) {
        Pin* pin = nullptr;
        std::string inst_name = io_net->instance(i);

        if (inst_name.compare("PIN") == 0) {
            pin = top_cell->getIOPin(io_net->pin(i));
        } else {
            Inst* inst = top_cell->getInstance(inst_name.c_str());
            if (inst) pin = inst->getPin(io_net->pin(i));
        }
        if (pin) {
            net->addPin(pin);
            if (pin->getNet() && pin->getNet() != net) {
                message->issueMsg(
                    kWarn, "Pin has different net definition, Pin %s, Net %s\n",
                    pin->getName().c_str(), net->getName().c_str());
            }
            pin->setNet(net);
        }
    }

    // regularWiring
    if (io_net->numWires()) {
        for (i = 0; i < io_net->numWires(); i++) {
            wire = io_net->wire(i);
            readWire(wire, net);
        }
    }

    // sub net
    readSubNet(io_net, net);

    // NDR
    if (io_net->hasNonDefaultRule()) {
        std::string rule_name = io_net->nonDefaultRule();
        net->setNonDefaultRule(
            lib->getNonDefaultRuleIdByName(rule_name.c_str()));
    }

    // VPin
    for (i = 0; i < io_net->numVpins(); i++) {
        io_vpin = io_net->vpin(i);
        std::string io_vpin_name = io_vpin->name();
        v_pin = net->createVpin(io_vpin_name);
        if (io_vpin->layer()) {
            v_pin->setHasLayer(true);
            v_pin->setLayer(const_cast<char*>(io_vpin->layer()));
        }
        bbox = creatBox(io_vpin->xl(), io_vpin->yl(), io_vpin->xh(),
                        io_vpin->yh());
        v_pin->setBox(*bbox);

        if (io_vpin->status() != ' ') {
            int status = getVPinRouteStatus(io_vpin->status());
            v_pin->setStatus(status);
            loc = new Point(io_vpin->xLoc(), io_vpin->yLoc());
            v_pin->setLoc(*loc);
            if (io_vpin->orient() != -1) {
                v_pin->setOrientation(io_vpin->orient() + 1);
            }
        }

        net->addVPin(v_pin);
    }

    if (io_net->hasUse()) {
        NetType type = getNetType(io_net->use());
        net->setType(type);
    }
    if (io_net->hasSource()) {
        int source = getSourceStatus(io_net->source());
        net->setSource(source);
    }

    if (io_net->hasXTalk()) net->setXtalk(io_net->XTalk());
    if (io_net->hasFixedbump()) net->setFixBump(1);
    if (io_net->hasFrequency()) {
        net->setFrequency(static_cast<int>(io_net->frequency()));
    }
    if (io_net->hasOriginal()) net->setOriginNet(io_net->original());
    if (io_net->hasPattern()) net->setPattern(getNetPattern(io_net->pattern()));
    if (io_net->hasCap()) net->setCapacitance(io_net->cap());
    if (io_net->hasWeight()) net->setWeight(io_net->weight());

    readProperties<defiNet, Net>(PropType::kNet,
                                 reinterpret_cast<void*>(io_net),
                                 reinterpret_cast<void*>(net));

    return 0;
}

int readFill(defiFill* io_fill) {
    Cell* top_cell = getTopCell();
    Tech* lib = top_cell->getTechLib();
    Fill* fill = top_cell->createFill();
    if (fill == 0) {
        return -1;
    }
    if (io_fill->hasLayer()) {
        fill->setIsLayer(true);
        fill->setLayerId(lib->getLayerLEFIndexByName(io_fill->layerName()));
        if (io_fill->layerMask()) {
            fill->setLayerMask(io_fill->layerMask());
        }
        if (io_fill->hasLayerOpc()) {
            fill->setIsOpc(true);
        }

        for (int i = 0; i < io_fill->numRectangles(); i++) {
            Point pt1(io_fill->xl(i), io_fill->yl(i));
            Point pt2(io_fill->xh(i), io_fill->yh(i));
            std::vector<Point> rect;  // rect has two points
            rect.push_back(pt1);
            rect.push_back(pt2);
            fill->addPoints(rect);
        }
        for (int i = 0; i < io_fill->numPolygons(); i++) {
            defiPoints pts = io_fill->getPolygon(i);
            std::vector<Point> polygon;  // poly has multiple points
            for (int j = 0; j < pts.numPoints; j++) {
                Point pt(pts.x[j], pts.y[j]);
                polygon.push_back(pt);
            }
            fill->addPoints(polygon);
        }
    }
    if (io_fill->hasVia()) {
        fill->setIsVia(true);
        fill->setVia(lib->getViaMaster(io_fill->viaName()));
        if (io_fill->viaTopMask() || io_fill->viaCutMask() ||
            io_fill->viaBottomMask()) {
            fill->setViaBotMask(io_fill->viaBottomMask());
            fill->setViaCutMask(io_fill->viaCutMask());
            fill->setViaTopMask(io_fill->viaTopMask());
        }
        if (io_fill->hasViaOpc()) {
            fill->setIsOpc(true);
        }

        std::vector<Point> via_points;  // via has only one-d points
        for (int i = 0; i < io_fill->numViaPts(); i++) {
            defiPoints pts = io_fill->getViaPts(i);
            for (int j = 0; j < pts.numPoints; j++) {
                Point pt(pts.x[j], pts.y[j]);
                via_points.push_back(pt);
            }
        }
        fill->addPoints(via_points);
    }
    return 0;
}

int readScanChainPoint(defiScanchain* sc, ScanChain* scan_chain,
                       bool is_start) {
    Cell* top_cell = getTopCell();
    ScanChainPoint* point = new ScanChainPoint();
    char *a1, *b1;
    if (is_start) {
        scan_chain->setStart(point);
        sc->start(&a1, &b1);
        if (b1 == 0) {
            point->setHasPin(false);
        } else {
            point->setHasPin(true);
        }
        if (b1 == 0 && scan_chain->getHasCommonOutPin()) {
            b1 = (const_cast<char*>(scan_chain->getCommonOutPin()));
        }
    } else {
        scan_chain->setStop(point);
        sc->stop(&a1, &b1);
        if (b1 == 0) {
            point->setHasPin(false);
        } else {
            point->setHasPin(true);
        }
        if (b1 == 0 && scan_chain->getHasCommonInPin()) {
            b1 = (const_cast<char*>(scan_chain->getCommonInPin()));
        }
    }
    if (strcmp(a1, "PIN") == 0) {
        point->setIsIOPin(true);
        if (b1) {
            Pin* io_pin = top_cell->getIOPin(b1);
            if (io_pin) {
                point->setPinId(io_pin->getId());
            }
        }
    } else if (a1) {
        Inst* inst = top_cell->getInstance(a1);
        if (inst) {
            point->setInstId(inst->getId());
            if (b1) {
                Pin* pin = inst->getPin(b1);
                if (!pin) {
                    pin = inst->getPGPin(b1);
                }
                if (pin) {
                    point->setPinId(pin->getId());
                }
            }
        }
    }
    return 0;
}

int readScanChainList(defiScanchain* sc, ScanChain* scan_chain,
                      bool is_floating, int idx) {
    Cell* top_cell = getTopCell();
    char** inst;
    char** inPin;
    char** outPin;
    int* bits = 0;
    int size = 0;
    if (is_floating) {
        sc->floating(&size, &inst, &inPin, &outPin, &bits);
    } else {
        sc->ordered(idx, &size, &inst, &inPin, &outPin, &bits);
    }
    if (size <= 0) {
        return -1;
    }
    ScanChainList* list = new ScanChainList();
    if (is_floating) {
        list->setNext(scan_chain->getFloating());
        scan_chain->setFloating(list);
    } else {
        list->setNext(scan_chain->getOrdered());
        scan_chain->setOrdered(list);
    }
    for (int i = 0; i < size; i++) {
        ScanChainListItem* item = new ScanChainListItem();
        item->setNext(list->getItems());
        list->setItems(item);
        Inst* inst1 = top_cell->getInstance(inst[i]);
        if (inst1) {
            item->setInstId(inst1->getId());
            if (inPin[i]) {
                item->setHasInPin(true);
                Pin* in_pin = inst1->getPin(inPin[i]);
                if (!in_pin) {
                    in_pin = inst1->getPGPin(inPin[i]);
                }
                if (in_pin) {
                    item->setInPinId(in_pin->getId());
                }
            } else if (scan_chain->getHasCommonInPin()) {
                Pin* in_pin = inst1->getPin(scan_chain->getCommonInPin());
                if (!in_pin) {
                    in_pin = inst1->getPGPin(scan_chain->getCommonInPin());
                }
                if (in_pin) {
                    item->setInPinId(in_pin->getId());
                }
            }
            if (outPin[i]) {
                item->setHasOutPin(true);
                Pin* out_pin = inst1->getPin(outPin[i]);
                if (!out_pin) {
                    out_pin = inst1->getPGPin(outPin[i]);
                }
                if (out_pin) {
                    item->setOutPinId(out_pin->getId());
                }
            } else if (scan_chain->getHasCommonOutPin()) {
                Pin* out_pin = inst1->getPin(scan_chain->getCommonOutPin());
                if (!out_pin) {
                    out_pin = inst1->getPGPin(scan_chain->getCommonOutPin());
                }
                if (out_pin) {
                    item->setOutPinId(out_pin->getId());
                }
            }
        }
        if (bits[i] != -1) {
            item->setHasBits(true);
            item->setNumBits(bits[i]);
        }
    }
    return 0;
}

int readScanChain(defiScanchain* sc) {
    Cell* top_cell = getTopCell();
    std::string name(sc->name());
    ScanChain* scan_chain = top_cell->createScanChain(name);
    if (scan_chain == 0) {
        return -1;
    }
    if (sc->hasCommonInPin() || sc->hasCommonOutPin()) {
        scan_chain->setHasCommonScanPins(true);
        if (sc->hasCommonInPin()) {
            scan_chain->setHasCommonInPin(true);
            scan_chain->setCommonInPin(sc->commonInPin());
        }
        if (sc->hasCommonOutPin()) {
            scan_chain->setHasCommonOutPin(true);
            scan_chain->setCommonOutPin(sc->commonOutPin());
        }
    }
    if (sc->hasStart()) {
        readScanChainPoint(sc, scan_chain, true);
    }
    if (sc->hasStop()) {
        readScanChainPoint(sc, scan_chain, false);
    }
    if (sc->hasFloating()) {
        readScanChainList(sc, scan_chain, true);
    }
    if (sc->hasOrdered()) {
        for (int i = 0; i < sc->numOrderedLists(); i++) {
            readScanChainList(sc, scan_chain, false, i);
        }
    }
    if (sc->hasPartition()) {
        scan_chain->setHasPartition(true);
#ifdef USE_SYMBOL_TABLE_
#else
        scan_chain->setPartitionName(sc->partitionName());
#endif
        if (sc->hasPartitionMaxBits()) {
            scan_chain->setHasMaxBits(true);
            scan_chain->setMaxBits(sc->partitionMaxBits());
        }
    }
    return 0;
}

int readRegion(defiRegion* io_region) {
    if (!io_region) {
        message->issueMsg(kError, "io region is null pointer.\n");
        return ERROR;
    }
    Cell* top_cell = getTopCell();
    Floorplan* floorplan = top_cell->getFloorplan();
    if (!floorplan) {
        message->issueMsg(kError,
                          "Cannot find floorplan when read DEF region.\n");
        return ERROR;
    }

    Constraint* region = floorplan->createRegion(io_region->name());
    if (!region) {
        message->issueMsg(kError, "Create region %s failed.\n",
                          io_region->name());
        return ERROR;
    }

    for (int i = 0; i < io_region->numRectangles(); i++) {
        region->createBox(io_region->xl(i), io_region->yl(i), io_region->xh(i),
                          io_region->yh(i));
    }

    if (io_region->hasType()) {
        if (0 == strcmp(io_region->type(), "FENCE")) {
            region->setConstraintSubType(
                Constraint::ConstraintSubType::kRegionFence);
        } else if (0 == strcmp(io_region->type(), "GUIDE")) {
            region->setConstraintSubType(
                Constraint::ConstraintSubType::kRegionGuide);
        }
    }
    readProperties<defiRegion, Constraint>(PropType::kRegion,
                                           reinterpret_cast<void*>(io_region),
                                           reinterpret_cast<void*>(region));

    return OK;
}

Orient convertDefIntToOrient(int io_orient) {
    Orient orient = Orient::kUnknown;
    switch (io_orient) {
        case 0:
            orient = Orient::kN;
            break;
        case 1:
            orient = Orient::kW;
            break;
        case 2:
            orient = Orient::kS;
            break;
        case 3:
            orient = Orient::kE;
            break;
        case 4:
            orient = Orient::kFN;
            break;
        case 5:
            orient = Orient::kFW;
            break;
        case 6:
            orient = Orient::kFS;
            break;
        case 7:
            orient = Orient::kFE;
            break;
        case 8:
            message->issueMsg(kError, "Invalid site orient %d\n", io_orient);
            orient = Orient::kUnknown;
            break;
    }

    return orient;
}

int readRow(defiRow* io_row) {
    if (!io_row) {
        message->issueMsg(kError, "io row is null pointer.\n");
        return ERROR;
    }
    Cell* top_cell = getTopCell();
    Floorplan* floorplan = top_cell->getFloorplan();
    if (!floorplan) {
        message->issueMsg(kError, "Cannot find floorplan when read DEF row.\n");
        return ERROR;
    }
    Row* row = floorplan->createRow();
    if (!row) {
        message->issueMsg(kError, "Create row failed.\n");
        return ERROR;
    }

    row->setName(io_row->name());
    row->setSiteName(io_row->macro());
    row->setOrigX(io_row->x());
    row->setOrigY(io_row->y());
    row->setSiteOrient(convertDefIntToOrient(io_row->orient()));

    if (io_row->hasDo()) {
        row->setHasDo(true);
        row->setNumX((Bits64)io_row->xNum());
        row->setNumY((Bits64)io_row->yNum());
        if (io_row->hasDoStep()) {
            row->setHasDoStep(true);
            row->setStepX((Bits64)io_row->xStep());
            row->setStepY((Bits64)io_row->yStep());
        }
    }

    int32_t site_count = row->getNumX() * row->getNumY();
    row->setSiteCount(site_count);
    Site* site =
        top_cell->getTechLib()->getSiteByName(row->getSiteName().c_str());
    if (!site) {
        message->issueMsg(kError, "cannot find site %s.\n",
                          row->getSiteName().c_str());
        return ERROR;
    }
    ObjectId site_id = site->getId();
    row->setSiteId(site_id);

    readProperties<defiRow, Row>(PropType::kRow,
                                 reinterpret_cast<void*>(io_row),
                                 reinterpret_cast<void*>(row));

    return OK;
}

int readTrack(defiTrack* io_track) {
    if (!io_track) {
        message->issueMsg(kError, "io track is null pointer.\n");
        return ERROR;
    }
    Cell* top_cell = getTopCell();
    Floorplan* floorplan = top_cell->getFloorplan();
    if (!floorplan) {
        message->issueMsg(kError,
                          "Cannot find floorplan when read DEF track.\n");
        return ERROR;
    }
    Track* track = floorplan->createTrack();
    if (!track) {
        message->issueMsg(kError, "Create track failed.\n");
        return ERROR;
    }

    track->setFloorplan(floorplan->getId());
    track->setDirectionX((*(io_track->macro()) == 'X') ? true : false);
    track->setStart(io_track->x());
    track->setNumTracks(io_track->xNum());
    track->setSpace(io_track->xStep());
    track->setHasMask(io_track->firstTrackMask() != 0);
    track->setMask(io_track->firstTrackMask());
    track->setHasSameMask(io_track->sameMask() != 0);

    for (int i = 0; i < io_track->numLayers(); ++i) {
        track->addLayer(io_track->layer(i));
    }

    return OK;
}

int readCompMaskShift(defiComponentMaskShiftLayer* co) {
    Cell* top_cell = getTopCell();
    Tech* tech_lib = getTopCell()->getTechLib();
    if (!tech_lib) {
        message->issueMsg(kError,
                          "Cannot find techlib when read DEF ndr_rule.\n");
        return ERROR;
    }
    for (int i = 0; i < co->numMaskShiftLayers(); ++i) {
        Int32 layer_id = tech_lib->getLayerLEFIndexByName(co->maskShiftLayer(i));
        if (layer_id < 0) {
            message->issueMsg(kError, "Cannot find layer %s in LEF.\n",
                              co->maskShiftLayer(i));
            return ERROR;
        }
    }

    for (int i = 0; i < co->numMaskShiftLayers(); ++i) {
        Int32 layer_id = tech_lib->getLayerLEFIndexByName(co->maskShiftLayer(i));
        top_cell->addMaskShiftLayer(layer_id);
    }

    return OK;
}

int readComp(defiComponent* co) {
    Cell* top_cell = getTopCell();
    std::string inst_name(co->id());
    Inst* inst = top_cell->createInstance(inst_name);
    if (!inst) {
        return -1;
    }
    inst->setMaster(co->name());
    inst->createPins();
    if (co->isFixed()) {
        inst->setStatus(PlaceStatus::kFixed);
    } else if (co->isCover()) {
        inst->setStatus(PlaceStatus::kCover);
    } else if (co->isPlaced()) {
        inst->setStatus(PlaceStatus::kPlaced);
    } else if (co->isUnplaced()) {
        inst->setStatus(PlaceStatus::kUnplaced);
    } else {
        inst->setStatus(PlaceStatus::kUnknown);
    }
    if (inst->getStatus() != PlaceStatus::kUnknown &&
        inst->getStatus() != PlaceStatus::kUnplaced) {
        Point p(co->placementX(), co->placementY());
        inst->setLocation(p);
        inst->setOrient(convertDefIntToOrient(co->placementOrient()));
    }
    if (co->hasSource()) {
        inst->setHasSource(true);
        if (strcmp(co->source(), "NETLIST") == 0) {
            inst->setSource(kNetlist);
        } else if (strcmp(co->source(), "DIST") == 0) {
            inst->setSource(kDist);
        } else if (strcmp(co->source(), "USER") == 0) {
            inst->setSource(kUser);
        } else if (strcmp(co->source(), "TIMING") == 0) {
            inst->setSource(kTiming);
        }
    }
    if (co->hasWeight()) {
        inst->setHasWeight(true);
        inst->setWeight(co->weight());
    }
    if (co->hasEEQ()) {
        inst->setHasEeqMaster(true);
        inst->setEeqMaster(co->EEQ());
    }
    if (co->hasRegionName()) {
        inst->setHasRegion(true);
        std::string region_name(co->regionName());
        inst->setRegion(region_name);
    }
    if (co->maskShiftSize()) {
        inst->setHasMaskShift(true);
        for (int i = 0; i < co->maskShiftSize(); ++i) {
            // revert the order because it is reverted in io DEF.
            inst->setMaskShift(co->maskShift(i), co->maskShiftSize() - i - 1);
        }
    }
    if (co->hasHalo()) {
        inst->setHasHalo(true);
        inst->setHasSoft(co->hasHaloSoft());
        int left, bottom, right, top;
        (void)co->haloEdges(&left, &bottom, &right, &top);
        Box box(left, bottom, right, top);
        inst->setHalo(box);
    }
    if (co->hasRouteHalo()) {
        inst->setHasRouteHalo(true);
        inst->setRouteHaloDist(co->haloDist());
        inst->setMinLayer(co->minLayer());
        inst->setMaxLayer(co->maxLayer());
    }
    readProperties<defiComponent, Inst>(PropType::kComponent,
                                        reinterpret_cast<void*>(co),
                                        reinterpret_cast<void*>(inst));

    /*
    if (co->numProps()) {
        inst->setHasProperty(true);
        for (int i = 0; i < co->numProps(); i++) {
            std::string property(co->propName(i));
            property.append(" ");
            property.append(co->propValue(i));

            inst->addProperty(property);
        }
    }
    */
    return 0;
}

int readGcellGrid(defiGcellGrid* io_gcell_grid) {
    if (!io_gcell_grid) {
        message->issueMsg(kError, "io gcell grid is null pointer.\n");
        return ERROR;
    }
    Cell* top_cell = getTopCell();
    Floorplan* floorplan = top_cell->getFloorplan();
    if (!floorplan) {
        message->issueMsg(kError,
                          "Cannot find floorplan when read DEF GcellGrid.\n");
        return ERROR;
    }
    Grid* gcell_grid = floorplan->createGcellGrid();
    if (!gcell_grid) {
        message->issueMsg(kError, "Create gcell grid failed.\n");
        return ERROR;
    }

    gcell_grid->setFloorplan(floorplan->getId());
    gcell_grid->setDirectionX((*(io_gcell_grid->macro()) == 'X') ? true
                                                                 : false);
    gcell_grid->setStart(io_gcell_grid->x());
    gcell_grid->setNumber(io_gcell_grid->xNum() -
                          1);  // -1 to get the real number
    gcell_grid->setSpace(io_gcell_grid->xStep());

    return OK;
}

void saveGroupMember(char* name) {
    char* compName = reinterpret_cast<char*>(malloc(strlen(name) + 1));
    if (!compName) {
        message->issueMsg(
            kError, "malloc failed when read DEF GROUP's compNamePattern.\n");
        return;
    }
    snprintf(compName, sizeof(compName), name);
    kGroupCompNamePatterns.push_back(compName);
}

void clearGroupMember() {
    for (int i = 0; i < kGroupCompNamePatterns.size(); ++i) {
        free(kGroupCompNamePatterns[i]);
    }
    kGroupCompNamePatterns.clear();
}

int readGroup(defiGroup* io_group) {
    if (!io_group) {
        message->issueMsg(kError, "io group is null pointer.\n");
        return ERROR;
    }
    Cell* top_cell = getTopCell();
    std::string group_name = io_group->name();
    Group* group = top_cell->createGroup(group_name);
    if (!group) {
        message->issueMsg(kError, "Create group failed.\n");
        return ERROR;
    }

    for (int i = 0; i < kGroupCompNamePatterns.size(); ++i) {
        group->addPattern(kGroupCompNamePatterns[i]);
        // TODO (ly): wildcard matching.
        Inst* instance = top_cell->getInstance(kGroupCompNamePatterns[i]);
        if (!instance) {
            message->issueMsg(kError, "Cannot find instance %s \n",
                              kGroupCompNamePatterns[i]);
            continue;
        }
        group->addInstance(instance->getId());
    }

    if (io_group->hasRegionName()) {
        std::string name = io_group->regionName();
        Constraint* region = top_cell->getFloorplan()->getRegion(name);
        if (region != nullptr) {
            group->setRegion(region->getId());
        }
    }

    readProperties<defiGroup, Group>(PropType::kGroup,
                                     reinterpret_cast<void*>(io_group),
                                     reinterpret_cast<void*>(group));

    return OK;
}

int readNonDefaultRule(defiNonDefault* io_ndr_rule) {
    if (!io_ndr_rule) {
        message->issueMsg(kError, "io ndr_rule is null pointer.\n");
        return ERROR;
    }
    Cell* top_cell = getTopCell();
    Tech* lib = getTopCell()->getTechLib();

    if (!lib) {
        message->issueMsg(kError,
                          "Cannot find techlib when read DEF ndr_rule.\n");
        return ERROR;
    }

    int i = 0;
    NonDefaultRule* ndr_rule =
        top_cell->createObject<NonDefaultRule>(kObjectTypeRule);
    // name
    ndr_rule->setName(io_ndr_rule->name());
    // hard_spacing
    ndr_rule->setHardSpacing(io_ndr_rule->hasHardspacing());
    // from_def
    ndr_rule->setFromDEF(true);
    // layer
    ndr_rule->setLayerSize(io_ndr_rule->numLayers());
    for (i = 0; i < io_ndr_rule->numLayers(); i++) {
        NonDefaultRuleLayer* layer =
            top_cell->createObject<NonDefaultRuleLayer>(
                kObjectTypeNonDefaultRuleLayer);
        layer->setName(io_ndr_rule->layerName(i));
        layer->setWidth(lib->micronsToDBU(io_ndr_rule->layerWidth(i)));
        if (io_ndr_rule->hasLayerSpacing(i))
            layer->setSpacing(lib->micronsToDBU(io_ndr_rule->layerSpacing(i)));
        if (io_ndr_rule->hasLayerDiagWidth(i))
            layer->setDiagWidth(
                lib->micronsToDBU(io_ndr_rule->layerDiagWidth(i)));
        if (io_ndr_rule->hasLayerWireExt(i))
            layer->setWireExt(lib->micronsToDBU(io_ndr_rule->layerWireExt(i)));
        ndr_rule->addLayer(layer->getId());
    }
    // min_cuts
    ndr_rule->setMinCutsSize(io_ndr_rule->numMinCuts());
    for (i = 0; i < io_ndr_rule->numMinCuts(); i++) {
        NonDefaultRuleMinCuts* min_cuts =
            top_cell->createObject<NonDefaultRuleMinCuts>(
                kObjectTypeNonDefaultRuleMinCuts);
        min_cuts->setName(io_ndr_rule->cutLayerName(i));
        min_cuts->setNumCuts(io_ndr_rule->numCuts(i));
        ndr_rule->addMinCuts(min_cuts->getId());
    }
    // use_via
    ndr_rule->setUseViaSize(io_ndr_rule->numVias());
    for (i = 0; i < io_ndr_rule->numVias(); i++) {
        ViaMaster* use_via = lib->getViaMaster(io_ndr_rule->viaName(i));
        if (use_via == nullptr) {
            continue;
        }

        ndr_rule->addUseVia(use_via->getId());
    }
    // use_via_rule
    ndr_rule->setUseViaRuleSize(io_ndr_rule->numViaRules());
    for (i = 0; i < io_ndr_rule->numViaRules(); i++) {
        ViaRule* use_via_rule = lib->getViaRule(io_ndr_rule->viaRuleName(i));
        if (use_via_rule == nullptr) {
            continue;
        }
        ndr_rule->addUseViaRule(use_via_rule->getId());
    }
    // property
    ndr_rule->setPropertySize(io_ndr_rule->numProps());
    for (i = 0; i < io_ndr_rule->numProps(); i++) {
        Property* property =
            top_cell->createObject<Property>(kObjectTypeProperty);
        property->setDefinitionId(PropType::kNonDefaultRule,
                                  io_ndr_rule->propName(i));
        if (io_ndr_rule->propIsNumber(i)) {
            if (property->getDataType() == PropDataType::kInt) {
                property->setIntValue(
                    static_cast<int>(io_ndr_rule->propNumber(i)));
            } else {
                property->setRealValue(io_ndr_rule->propNumber(i));
            }
        }
        if (io_ndr_rule->propIsString(i)) {
            property->setStringValue(io_ndr_rule->propValue(i));
        }
        ndr_rule->addProperty(property->getId());
    }

    ObjectId obj_id = ndr_rule->getId();
    getTopCell()->getTechLib()->addNonDefaultRule(obj_id);
    return OK;
}

int readPropertyDefinition(defiProp* io_prop) {
    Cell* current_top_cell = getTopCell();
    if (!current_top_cell) return 0;

    PropertyDefinition* prop_definition =
        current_top_cell->createObject<PropertyDefinition>(
            kObjectTypePropertyDefinition);
    PropType type = toEnumByString<PropType>(io_prop->propType());
    prop_definition->setPropType(type);
    prop_definition->setFromDEF(true);
    prop_definition->setPropName(io_prop->propName());

    PropDataType data_type = PropDataType::kUnknown;

    switch (io_prop->dataType()) {
        case 'I':
            data_type = PropDataType::kInt;
            break;
        case 'R':
        case 'N':  // TODO(luoying): not supported yet.
            data_type = PropDataType::kReal;
            break;
        case 'S':
        case 'Q':
            data_type = PropDataType::kString;
            break;
    }
    prop_definition->setDataType(data_type);

    if (io_prop->hasNumber()) {
        double io_number = io_prop->number();
        if (data_type == PropDataType::kInt) {
            prop_definition->setIntValue(static_cast<int>(io_number));
        } else {
            prop_definition->setRealValue(io_number);
        }
    }
    if (io_prop->hasString() && data_type == PropDataType::kString) {
        prop_definition->setStringValue(io_prop->string());
    }

    if (io_prop->hasRange()) {
        double left = io_prop->left();
        double right = io_prop->right();
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
    return OK;
}

int readUnit(double dbu) {
    Cell* current_top_cell = getTopCell();
    if (!current_top_cell) return ERROR;

    Units* db_units = current_top_cell->getTechLib()->getUnits();
    if (db_units == nullptr) {
        db_units = current_top_cell->createObject<Units>(kObjectTypeUnits);
        db_units->setFromDEF(true);
        db_units->setLengthUnits("MICRONS");
        db_units->setLengthFactor(static_cast<int>(dbu));
        db_units->setCapacitanceUnits("PICOFARADS");
        db_units->setCapacitanceFactor(1000000);
        db_units->setResistanceUnits("OHMS");
        db_units->setResistanceFactor(1000);
        db_units->setTimeUnits("NANOSECONDS");
        db_units->setTimeFactor(1000);
        db_units->setPowerUnits("MILLIWATTS");
        db_units->setPowerFactor(10000);
        db_units->setCurrentUnits("MILLIAMPS");
        db_units->setCurrentFactor(10000);
        db_units->setVoltageUnits("VOLTS");
        db_units->setVoltageFactor(1000);
        db_units->setFrequencyUnits("MEGAHERTS");
        db_units->setFrequencyFactor(1000000);  // TODO(luoying):
        getTopCell()->getTechLib()->setUnits(db_units);
        return OK;
    } else {
        int origin_units = db_units->getLengthFactor();
        if (origin_units != static_cast<int>(dbu)) {
            message->issueMsg(kError,
                              "There is a mismatch between DEF UNITS %d"
                              "and original defined UNITS "
                              "%d, so the DEF UNITS is discarded.\n",
                              static_cast<int>(dbu), origin_units);
            return ERROR;
        }
    }
    return OK;
}

int readPin(defiPin* def_pin) {
    Cell* top_cell = getTopCell();
    std::string pin_name(def_pin->pinName());
    Pin* pin = top_cell->createIOPin(pin_name);
    if (!pin) {
        return -1;
    }
    Term* term = top_cell->createObject<Term>(kObjectTypeTerm);
    if (!term) {
        return -1;
    }
    std::string net_name(def_pin->netName());
    term->setName(pin_name);
    // set IO pin attributes
    pin->setTerm(term);
    pin->setNet(top_cell->getNet(net_name));
    pin->setInst(0);  // IO pin has no instance
    if (def_pin->hasSpecial()) {
        pin->setIsSpecial(true);
    }
    // set term attributes for IO pin, reuse term definition
    if (def_pin->hasDirection()) {
        term->setDirection(def_pin->direction());
    }
    if (def_pin->hasUse()) {
        term->setUse(def_pin->use());
    }
    if (def_pin->hasNetExpr()) {
        term->setNetExpr(def_pin->netExpr());
    }
    if (def_pin->hasSupplySensitivity()) {
        term->setSupplySensitivity(def_pin->supplySensitivity());
    }
    if (def_pin->hasGroundSensitivity()) {
        term->setGroundSensitivity(def_pin->groundSensitivity());
    }
    /* reuse term Antenna definiton except PIN keyword(in export)
     * [+ ANTENNAPINPARTIALMETALAREA  value  [LAYER  layerName ]] ...
     * [+ ANTENNAPINPARTIALMETALSIDEAREA  value  [LAYER  layerName ]] ...
     * [+ ANTENNAPINPARTIALCUTAREA  value  [LAYER  layerName ]] ...
     * [+ ANTENNAPINDIFFAREA  value  [LAYER  layerName ]] ...
     * [+ ANTENNAMODEL {OXIDE1 | OXIDE2 | OXIDE3 | OXIDE4}] ...
     * [+ ANTENNAPINGATEAREA  value  [LAYER  layerName ]] ...
     * [+ ANTENNAPINMAXAREACAR  value  LAYER  layerName ] ...
     * [+ ANTENNAPINMAXSIDEAREACAR  value  LAYER  layerName ] ...
     * [+ ANTENNAPINMAXCUTCAR  value  LAYER  layerName ] ...
     */
    if (def_pin->hasAPinPartialMetalArea()) {
        for (int i = 0; i < def_pin->numAPinPartialMetalArea(); i++) {
            AntennaArea* ap =
                top_cell->createObject<AntennaArea>(kObjectTypePinAntennaArea);
            ap->setArea(def_pin->APinPartialMetalArea(i));
            if (*(def_pin->APinPartialMetalAreaLayer(i))) {
                ap->setLayerName(def_pin->APinPartialMetalAreaLayer(i));
            }
            term->addAntennaPartialMetalArea(ap->getId());
        }
    }
    if (def_pin->hasAPinPartialMetalSideArea()) {
        for (int i = 0; i < def_pin->numAPinPartialMetalSideArea(); i++) {
            AntennaArea* ap =
                top_cell->createObject<AntennaArea>(kObjectTypePinAntennaArea);
            ap->setArea(def_pin->APinPartialMetalSideArea(i));
            if (*(def_pin->APinPartialMetalSideAreaLayer(i))) {
                ap->setLayerName(def_pin->APinPartialMetalSideAreaLayer(i));
            }
            term->addAntennaPartialMetalSideArea(ap->getId());
        }
    }
    if (def_pin->hasAPinPartialCutArea()) {
        for (int i = 0; i < def_pin->numAPinPartialCutArea(); i++) {
            AntennaArea* ap =
                top_cell->createObject<AntennaArea>(kObjectTypePinAntennaArea);
            ap->setArea(def_pin->APinPartialCutArea(i));
            if (*(def_pin->APinPartialCutAreaLayer(i))) {
                ap->setLayerName(def_pin->APinPartialCutAreaLayer(i));
            }
            term->addAntennaPartialCutArea(ap->getId());
        }
    }
    if (def_pin->hasAPinDiffArea()) {
        for (int i = 0; i < def_pin->numAPinDiffArea(); i++) {
            AntennaArea* ap =
                top_cell->createObject<AntennaArea>(kObjectTypePinAntennaArea);
            ap->setArea(def_pin->APinDiffArea(i));
            if (*(def_pin->APinDiffAreaLayer(i))) {
                ap->setLayerName(def_pin->APinDiffAreaLayer(i));
            }
            term->addAntennaDiffArea(ap->getId());
        }
    }
    for (int j = 0; j < def_pin->numAntennaModel(); j++) {
        defiPinAntennaModel* aModel = def_pin->antennaModel(j);
        AntennaModelTerm* am = new AntennaModelTerm();
        std::string oxide_string = aModel->antennaOxide();
        oxide_string = oxide_string.substr(5);
        int oxide = stoi(oxide_string);
        if (oxide < 1 || oxide > 4) {
            message->issueMsg(kError, "oxide %d out of range.\n", oxide);
        }
        if (aModel->hasAPinGateArea()) {
            for (int i = 0; i < aModel->numAPinGateArea(); i++) {
                AntennaArea* ap = top_cell->createObject<AntennaArea>(
                    kObjectTypePinAntennaArea);
                ap->setArea(aModel->APinGateArea(i));
                if (aModel->hasAPinGateAreaLayer(i)) {
                    ap->setLayerName(aModel->APinGateAreaLayer(i));
                }
                am->addAntennaGateArea(ap->getId());
            }
        }
        if (aModel->hasAPinMaxAreaCar()) {
            for (int i = 0; i < aModel->numAPinMaxAreaCar(); i++) {
                AntennaArea* ap = top_cell->createObject<AntennaArea>(
                    kObjectTypePinAntennaArea);
                ap->setArea(aModel->APinMaxAreaCar(i));
                if (aModel->hasAPinMaxAreaCarLayer(i)) {
                    ap->setLayerName(aModel->APinMaxAreaCarLayer(i));
                }
                am->addAntennaMaxAreaCar(ap->getId());
            }
        }
        if (aModel->hasAPinMaxSideAreaCar()) {
            for (int i = 0; i < aModel->numAPinMaxSideAreaCar(); i++) {
                AntennaArea* ap = top_cell->createObject<AntennaArea>(
                    kObjectTypePinAntennaArea);
                ap->setArea(aModel->APinMaxSideAreaCar(i));
                if (aModel->hasAPinMaxSideAreaCarLayer(i)) {
                    ap->setLayerName(aModel->APinMaxSideAreaCarLayer(i));
                }
                am->addAntennaMaxSideAreaCar(ap->getId());
            }
        }
        if (aModel->hasAPinMaxCutCar()) {
            for (int i = 0; i < aModel->numAPinMaxCutCar(); i++) {
                AntennaArea* ap = top_cell->createObject<AntennaArea>(
                    kObjectTypePinAntennaArea);
                ap->setArea(aModel->APinMaxCutCar(i));
                if (aModel->hasAPinMaxCutCarLayer(i)) {
                    ap->setLayerName(aModel->APinMaxCutCarLayer(i));
                }
                am->addAntennaMaxCutCar(ap->getId());
            }
        }
        term->addAntennaModelTerm(oxide, am);
    }
    if (def_pin->hasLayer()) {
        Port* p = top_cell->createObject<Port>(kObjectTypePort);
        p->setIsReal(false);  // port is not defined in def
        for (int i = 0; i < def_pin->numLayer(); i++) {
            LayerGeometry* lg =
                top_cell->createObject<LayerGeometry>(kObjectTypeLayerGeometry);
            lg->setName(def_pin->layer(i));
            if (def_pin->hasLayerSpacing(i)) {
                lg->setMinSpacing(def_pin->layerSpacing(i));
            }
            if (def_pin->hasLayerDesignRuleWidth(i)) {
                lg->setdesignRuleWidth(def_pin->layerDesignRuleWidth(i));
            }
            lg->setType(GeometryType::kRect);
            Geometry* geo =
                top_cell->createObject<Geometry>(kObjectTypeGeometry);
            int xl, yl, xh, yh;
            def_pin->bounds(i, &xl, &yl, &xh, &yh);
            Box* b = creatBox(xl, yl, xh, yh);
            geo->setBox(b);
            if (def_pin->layerMask(i)) {
                geo->setNumMask(def_pin->layerMask(i));
            }
            geo->setType(GeometryType::kRect);
            lg->addGeometry(geo->getId());
            p->addLayerGeometry(lg->getId());
        }
        for (int i = 0; i < def_pin->numPolygons(); i++) {
            LayerGeometry* lg =
                top_cell->createObject<LayerGeometry>(kObjectTypeLayerGeometry);
            lg->setName(def_pin->polygonName(i));
            if (def_pin->hasPolygonSpacing(i)) {
                lg->setMinSpacing(def_pin->polygonSpacing(i));
            }
            if (def_pin->hasPolygonDesignRuleWidth(i)) {
                lg->setdesignRuleWidth(def_pin->polygonDesignRuleWidth(i));
            }
            lg->setType(GeometryType::kPolygon);
            Geometry* geo =
                top_cell->createObject<Geometry>(kObjectTypeGeometry);
            Polygon* geoPoly = new Polygon();
            defiPoints pts = def_pin->getPolygon(i);
            for (int j = 0; j < pts.numPoints; j++) {
                Point* p = new Point(pts.x[j], pts.y[j]);
                geoPoly->addPoint(p);
            }
            geo->setPolygonID(lg->CreatePolygon(geoPoly));
            if (def_pin->polygonMask(i)) {
                geo->setNumMask(def_pin->polygonMask(i));
            }
            geo->setType(GeometryType::kPolygon);
            lg->addGeometry(geo->getId());
            p->addLayerGeometry(lg->getId());
        }
        for (int i = 0; i < def_pin->numVias(); i++) {
            LayerGeometry* lg =
                top_cell->createObject<LayerGeometry>(kObjectTypeLayerGeometry);
            lg->setType(GeometryType::kVia);
            GeometryVia* v =
                top_cell->createObject<GeometryVia>(kObjectTypeGeometryVia);
            v->setName(def_pin->viaName(i));
            Point pt(def_pin->viaPtX(i), def_pin->viaPtY(i));
            v->setPoint(pt);
            if (def_pin->viaTopMask(i) || def_pin->viaCutMask(i) ||
                def_pin->viaBottomMask(i)) {
                v->setTopMaskNum(def_pin->viaTopMask(i));
                v->setCutMaskNum(def_pin->viaCutMask(i));
                v->setBottomMaskNum(def_pin->viaBottomMask(i));
            }
            lg->addGeometryVia(v->getId());
            p->addLayerGeometry(lg->getId());
        }
        if (def_pin->hasPlacement()) {
            // set pin placement information to port
            p->setHasPlacement(true);
            bool is_placed = false;
            if (def_pin->isPlaced()) {
                is_placed = true;
                p->setStatus(PlaceStatus::kPlaced);
            } else if (def_pin->isCover()) {
                is_placed = true;
                p->setStatus(PlaceStatus::kCover);
            } else if (def_pin->isFixed()) {
                is_placed = true;
                p->setStatus(PlaceStatus::kFixed);
            } else if (def_pin->isFixed()) {
                p->setStatus(PlaceStatus::kUnplaced);
            }                 // default kUnknown
            if (is_placed) {  // set location and orient
                Point pt(def_pin->placementX(), def_pin->placementY());
                p->setLocation(pt);
                p->setOrient(convertDefIntToOrient(def_pin->orient()));
            }
        }
        p->setTermId(term->getId());
        term->addPort(p->getId());
    }
    if (def_pin->hasPort()) {
        defiPinPort* port;
        for (int j = 0; j < def_pin->numPorts(); j++) {
            port = def_pin->pinPort(j);
            Port* p = top_cell->createObject<Port>(kObjectTypePort);
            p->setIsReal(true);  // port is defined in def
            for (int i = 0; i < port->numLayer(); i++) {
                LayerGeometry* lg = top_cell->createObject<LayerGeometry>(
                    kObjectTypeLayerGeometry);
                lg->setName(port->layer(i));
                if (port->hasLayerSpacing(i)) {
                    lg->setMinSpacing(port->layerSpacing(i));
                }
                if (port->hasLayerDesignRuleWidth(i)) {
                    lg->setdesignRuleWidth(port->layerDesignRuleWidth(i));
                }
                lg->setType(GeometryType::kRect);
                Geometry* geo = 
                    top_cell->createObject<Geometry>(kObjectTypeGeometry);
                int xl, yl, xh, yh;
                port->bounds(i, &xl, &yl, &xh, &yh);
                Box* b = creatBox(xl, yl, xh, yh);
                geo->setBox(b);
                if (port->layerMask(i)) {
                    geo->setNumMask(port->layerMask(i));
                }
                geo->setType(GeometryType::kRect);
                lg->addGeometry(geo->getId());
                p->addLayerGeometry(lg->getId());
            }
            for (int i = 0; i < port->numPolygons(); i++) {
                LayerGeometry* lg = top_cell->createObject<LayerGeometry>(
                    kObjectTypeLayerGeometry);
                lg->setName(port->polygonName(i));
                if (port->hasPolygonSpacing(i)) {
                    lg->setMinSpacing(port->polygonSpacing(i));
                }
                if (port->hasPolygonDesignRuleWidth(i)) {
                    lg->setdesignRuleWidth(port->polygonDesignRuleWidth(i));
                }
                lg->setType(GeometryType::kPolygon);
                Geometry* geo =
                    top_cell->createObject<Geometry>(kObjectTypeGeometry);
                Polygon* geoPoly = new Polygon();
                defiPoints pts = port->getPolygon(i);
                for (int k = 0; k < pts.numPoints; k++) {
                    Point* p = new Point(pts.x[k], pts.y[k]);
                    geoPoly->addPoint(p);
                }
                geo->setPolygonID(lg->CreatePolygon(geoPoly));
                if (port->polygonMask(i)) {
                    geo->setNumMask(port->polygonMask(i));
                }
                geo->setType(GeometryType::kPolygon);
                lg->addGeometry(geo->getId());
                p->addLayerGeometry(lg->getId());
            }
            for (int i = 0; i < port->numVias(); i++) {
                LayerGeometry* lg = top_cell->createObject<LayerGeometry>(
                    kObjectTypeLayerGeometry);
                lg->setType(GeometryType::kVia);
                GeometryVia* v =
                    top_cell->createObject<GeometryVia>(kObjectTypeGeometryVia);
                v->setName(port->viaName(i));
                Point pt(port->viaPtX(i), port->viaPtY(i));
                v->setPoint(pt);
                if (port->viaTopMask(i) || port->viaCutMask(i) ||
                    port->viaBottomMask(i)) {
                    v->setTopMaskNum(port->viaTopMask(i));
                    v->setCutMaskNum(port->viaCutMask(i));
                    v->setBottomMaskNum(port->viaBottomMask(i));
                }
                lg->addGeometryVia(v->getId());
                p->addLayerGeometry(lg->getId());
            }
            if (port->hasPlacement()) {
                p->setHasPlacement(true);
                bool is_placed = false;
                if (port->isPlaced()) {
                    is_placed = true;
                    p->setStatus(PlaceStatus::kPlaced);
                } else if (port->isCover()) {
                    is_placed = true;
                    p->setStatus(PlaceStatus::kCover);
                } else if (port->isFixed()) {
                    is_placed = true;
                    p->setStatus(PlaceStatus::kFixed);
                } else if (port->isFixed()) {
                    p->setStatus(PlaceStatus::kUnplaced);
                }                 // default kUnknown
                if (is_placed) {  // set location and orient
                    Point pt(port->placementX(), port->placementY());
                    p->setLocation(pt);
                    p->setOrient(convertDefIntToOrient(port->orient()));
                }
            }
            p->setTermId(term->getId());
            term->addPort(p->getId());
        }
    }
    return 0;
}

}  // namespace db
}  // namespace open_edi
