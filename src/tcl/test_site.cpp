/**
 * @file  test_site.cpp
 * @date  08/13/2020 06:33:22 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include <fstream>
#include <iostream>
#include <sstream>

#include "tcl/test_object_tcl_cmd.h"
#include "util/util.h"

namespace open_edi {
namespace tcl {

using namespace std;
using namespace open_edi::util;
using namespace open_edi::db;

enum SiteTestArgument {
    kSiteName = 1,
    kSiteClass,
    kSiteSymmetry,
    kOrient,
    kSiteWidth,
    kSiteHeight,
    kIsDefault,
    kRowPattern,
    kUnknown
};

bool parseSiteArgument(int argc, const char *argv[], Site &testObj) {
    bool invalidArgument = false;
    for (int i = 1; i < argc; ++i) {
        switch (i) {
            case kSiteName:
                testObj.setName(argv[i]);
                break;
            case kSiteClass:
                testObj.setClass(argv[i]);
                break;
            case kSiteSymmetry:
                testObj.setSymmetry(argv[i]);
                break;
            case kOrient:
                testObj.setOrientation(argv[i]);
                break;
            case kIsDefault: {
                bool isDefault = (strcmp(argv[i], "true") ? false : true);
                testObj.setIsDefault(isDefault);
            } break;
            case kSiteWidth:
                testObj.setWidth(atoi(argv[i]));
                break;
            case kSiteHeight:
                testObj.setHeight(atoi(argv[i]));
                break;
            case kRowPattern:
                // TODO....
                // testObj.(argv[i]);
                break;
            default:
                invalidArgument = true;
                break;
        }
        if (invalidArgument) {
            break;
        }
    }
    return true;
}

int siteTest(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Site testObj;
    if (getTopCell() == nullptr) {
        return TCL_ERROR;
    }
    parseSiteArgument(argc, argv, testObj);
    // SiteAttr testSiteAttrObj;
    testObj.setPatternSize(toInteger(Orient::kUnknown));
    for (int i = 0; i < toInteger(Orient::kUnknown); ++i) {
        stringstream nameStream;
        nameStream << "existing_site_" << i;
        SitePatternPair *pairObj = getTopCell()->createObject<SitePatternPair>(
            kObjectTypeSitePatternPair);
        ;
        pairObj->setName(nameStream.str().c_str());
        pairObj->setOrientation(toEnum<Orient, int>(i));
        testObj.addSitePattern(pairObj->getId());
    }
    // testObj.setAttr(&testSiteAttrObj);

    uint8_t endl_c = '\n';
    OStream<std::ofstream> os("test_site.txt", std::ios::out | std::ios::app);
    os << testObj << endl_c;
    os.close();
    return TCL_OK;
}

}  // namespace tcl
}  // namespace open_edi
