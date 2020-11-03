/**
 * @file  test_units.cpp
 * @date  08/21/2020 06:33:22 PM CST
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

enum UnitsTestArgument {
    kCapacitanceFactor = 1,
    kCurrentFactor,
    kPowerFactor,
    kResistanceFactor,
    kVoltageFactor,
    kTimeFactor,
    kFrequencyFactor,
    kLengthFactor,
    kCapacitanceUnits,
    kCurrentUnits,
    kPowerUnits,
    kResistanceUnits,
    kVoltageUnits,
    kTimeUnits,
    kFrequencyUnits,
    kLengthUnits,
    kUnknown
};

bool parseUnitsArgument(int argc, const char *argv[], Units &testObj) {
    bool invalidArgument = false;
    for (int i = 1; i < argc; ++i) {
        const char *value = argv[i];
        switch (i) {
            case kCapacitanceFactor:
                testObj.setCapacitanceFactor(atoi(value));
                break;
            case kCurrentFactor:
                testObj.setCurrentFactor(atoi(value));
                break;
            case kPowerFactor:
                testObj.setPowerFactor(atoi(value));
                break;
            case kResistanceFactor:
                testObj.setResistanceFactor(atoi(value));
                break;
            case kVoltageFactor:
                testObj.setVoltageFactor(atoi(value));
                break;
            case kTimeFactor:
                testObj.setTimeFactor(atoi(value));
                break;
            case kFrequencyFactor:
                testObj.setFrequencyFactor(atoi(value));
                break;
            case kLengthFactor:
                testObj.setLengthFactor(atoi(value));
                break;
            case kCapacitanceUnits:
                testObj.setCapacitanceUnits(value);
                break;
            case kCurrentUnits:
                testObj.setCurrentUnits(value);
                break;
            case kPowerUnits:
                testObj.setPowerUnits(value);
                break;
            case kResistanceUnits:
                testObj.setResistanceUnits(value);
                break;
            case kVoltageUnits:
                testObj.setVoltageUnits(value);
                break;
            case kTimeUnits:
                testObj.setTimeUnits(value);
                break;
            case kFrequencyUnits:
                testObj.setFrequencyUnits(value);
                break;
            case kLengthUnits:
                testObj.setLengthUnits(value);
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

int unitsTest(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Units testObj;
    parseUnitsArgument(argc, argv, testObj);

    uint8_t endl_c = '\n';
    OStream<std::ofstream> os("test_units.txt", std::ios::out | std::ios::app);
    os << testObj << endl_c;
    os.close();
    return TCL_OK;
}

}  // namespace tcl
}  // namespace open_edi
