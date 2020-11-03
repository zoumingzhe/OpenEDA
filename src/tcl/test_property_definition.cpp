/**
 * @file  test_property_definition.cpp
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

enum PropertyDefinitionTestArgument {
    kPropertyCount = 1,
    kPropertyType,
    kPropertyName,
    kPropertyDataType,
    kPropertyHasRange,
    kPropertyRangeMin,
    kPropertyRangeMax,
    kPropertyValue,
    kUnknown
};

bool parsePropertyDefinitionArgument(
    int argc, const char *argv[],
    std::vector<PropertyDefinition *> &test_objects) {
    bool invalid_argument = false;
    int prop_count = 0;
    PropertyDefinition test_object;

    for (int i = 1; i < argc; ++i) {
        switch (i) {
            case kPropertyCount:
                prop_count = atoi(argv[i]);
                if (prop_count < 1) {
                    invalid_argument = true;
                }
                test_objects.reserve(prop_count);
                break;
            case kPropertyType:
                test_object.setPropType(toEnumByString<PropType>(argv[i]));
                break;
            case kPropertyName:
                test_object.setPropName(argv[i]);
                break;
            case kPropertyDataType:
                test_object.setDataType(toEnumByString<PropDataType>(argv[i]));
                break;
            case kPropertyHasRange: {
                bool has_range = (strcmp(argv[i], "true") ? false : true);
                test_object.setHasRange(has_range);
            } break;
            case kPropertyRangeMin: {
                if (!test_object.getHasRange()) {
                    break;
                }
                if (test_object.getDataType() == PropDataType::kInt) {
                    test_object.setIntRangeMin(atoi(argv[i]));
                } else if (test_object.getDataType() == PropDataType::kReal) {
                    test_object.setRealRangeMin(strtod(argv[i], NULL));
                }
                break;
            }
            case kPropertyRangeMax: {
                if (!test_object.getHasRange()) {
                    break;
                }
                if (test_object.getDataType() == PropDataType::kInt) {
                    test_object.setIntRangeMax(atoi(argv[i]));
                } else if (test_object.getDataType() == PropDataType::kReal) {
                    test_object.setRealRangeMax(strtod(argv[i], NULL));
                }
                break;
            }
            case kPropertyValue: {
                PropDataType type = test_object.getDataType();
                switch (type) {
                    case PropDataType::kInt:
                        test_object.setIntValue(atoi(argv[i]));
                        break;
                    case PropDataType::kReal:
                        test_object.setRealValue(strtod(argv[i], NULL));
                        break;
                    default:
                        test_object.setStringValue(argv[i]);
                        break;
                }
            } break;
            default:
                invalid_argument = true;
                break;
        }
        if (invalid_argument) {
            break;
        }
    }

    for (int i = 0; i < prop_count; ++i) {
        test_objects.push_back(new PropertyDefinition(test_object));
    }
    return !invalid_argument;
}

int propertyDefinitionTest(ClientData cld, Tcl_Interp *itp, int argc,
                           const char *argv[]) {
    std::vector<PropertyDefinition *> test_objects;
    if (!parsePropertyDefinitionArgument(argc, argv, test_objects)) {
        cout << "Error: syntax error for command test_property_definitions."
             << endl;
        return TCL_OK;
    }

    int size = test_objects.size();
    uint8_t endl_c = '\n';
    OStream<std::ofstream> os("test_property_definition.txt",
                              std::ios::out | std::ios::app);
    for (const auto v : test_objects) {
        os << *v << endl_c;
    }
    os.close();
    for (auto v : test_objects) {
        delete v;
    }
    return TCL_OK;
}

}  // namespace tcl
}  // namespace open_edi
