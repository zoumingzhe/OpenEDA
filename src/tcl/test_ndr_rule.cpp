/**
 * @file  test_ndr_rule.cpp
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

#include "db/core/db.h"
#include "db/util/vector_object_var.h"
#include "tcl/test_object_tcl_cmd.h"
#include "util/util.h"

namespace open_edi {
namespace tcl {

using namespace std;
using namespace open_edi::util;
using namespace open_edi::db;

enum NonDefaultRuleTestArgument {
    kNonDefaultRuleName = 1,
    kNonDefaultRuleHardSpacing,
    kNonDefaultRuleSize,
    kNonDefaultRuleLayer,
    kLayerWidth,
    kLayerDiagWidth,
    kLayerSpacing,
    kLayerWireExt,
    kMinCutsLayer,
    kMinCutsNum,
    kUnknown
};

bool parseNonDefaultRuleArgument(int argc, const char *argv[],
                                 NonDefaultRule &testObj) {
    bool invalidArgument = false;
    NonDefaultRuleLayer *layer;
    NonDefaultRuleMinCuts *min_cuts;
    int size = 0;
    Cell *current_top_cell = getTopCell();
    if (!current_top_cell) return false;

    for (int i = 1; i < argc; ++i) {
        switch (i) {
            case kNonDefaultRuleName:
                testObj.setName(argv[i]);
                break;
            case kNonDefaultRuleHardSpacing:
                testObj.setHardSpacing(strcmp(argv[i], "true") ? false : true);
                break;
            case kNonDefaultRuleSize:
                size = atoi(argv[i]);
                testObj.setLayerSize(size);
                testObj.setMinCutsSize(size);
                testObj.setUseViaSize(size);
                testObj.setUseViaRuleSize(size);
                break;
            case kNonDefaultRuleLayer:
                layer = current_top_cell->createObject<NonDefaultRuleLayer>(
                    kObjectTypeNonDefaultRuleLayer);
                layer->setName(argv[i]);
                break;
            case kLayerWidth:
                layer->setWidth(atoi(argv[i]));
                break;
            case kLayerDiagWidth:
                layer->setDiagWidth(atoi(argv[i]));
                break;
            case kLayerSpacing:
                layer->setSpacing(atoi(argv[i]));
                break;
            case kLayerWireExt:
                layer->setWireExt(atoi(argv[i]));
                break;
            case kMinCutsLayer:
                min_cuts =
                    current_top_cell->createObject<NonDefaultRuleMinCuts>(
                        kObjectTypeNonDefaultRuleMinCuts);
                min_cuts->setName(argv[i]);
                break;
            case kMinCutsNum:
                min_cuts->setNumCuts(atoi(argv[i]));
                break;
            default:
                invalidArgument = true;
                break;
        }
        if (invalidArgument) {
            break;
        }
    }

    PropertyDefinition *prop_definition =
        current_top_cell->createObject<PropertyDefinition>(
            kObjectTypePropertyDefinition);
    prop_definition->setPropType(toEnumByString<PropType>("NONDEFAULTRULE"));
    prop_definition->setDataType(toEnumByString<PropDataType>("REAL"));
    prop_definition->setPropName("test_property");
    // prop_definition->setHasRange(true);
    prop_definition->setRealRangeMin(1.0);
    prop_definition->setRealRangeMax(3.0);
    prop_definition->setRealValue(2.93456);

    Property *prop =
        current_top_cell->createObject<Property>(kObjectTypeProperty);
    prop->setDefinitionId(prop_definition->getId());
    prop->setRealValue(2.9);
    // this function will be no-op because the value is out-of-range.
    prop->setRealValue(3.5);

    for (int i = 0; i < size; ++i) {
        testObj.addLayer(layer->getId());
        testObj.addMinCuts(min_cuts->getId());
        testObj.addUseVia(1);
        testObj.addUseViaRule(1);
        testObj.addProperty(prop->getId());
    }
    return true;
}

int nonDefaultRuleTest(ClientData cld, Tcl_Interp *itp, int argc,
                       const char *argv[]) {
    NonDefaultRule *testObj;
    MemPagePool *pool = getTopCell()->getPool();
    if (!pool) return TCL_ERROR;
    pool->printUsage();

    testObj = getTopCell()->createObject<NonDefaultRule>(kObjectTypeRule);
    if (!parseNonDefaultRuleArgument(argc, argv, *testObj)) return TCL_ERROR;
    uint8_t endl_c = '\n';
    OStream<std::ofstream> os("test_ndr_rule.txt",
                              std::ios::out | std::ios::app);
    os << *testObj << endl_c;
    kSparsePair = kSparseMap.equal_range(IdType(testObj->getId(),
                                                         kObjectTypeProperty));
    for (kSparseIt = kSparsePair.first; kSparseIt != kSparsePair.second;
                                                             ++kSparseIt) {
        Property *prop = Object::addr<Property>(kSparseIt->second);
        os << *prop << endl_c;
    }
    os.close();

    pool->printUsage();

    return TCL_OK;
}

}  // namespace tcl
}  // namespace open_edi
