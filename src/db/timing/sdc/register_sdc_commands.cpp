/**
 * @file register_sdc_commands.cpp
 * @date 2020-11-25
 * @brief
 *
 * Copyright (C) 2020 NIIC EDA
 *
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 *
 * of the BSD license.  See the LICENSE file for details.
 */

#include "db/timing/sdc/register_sdc_commands.h"
#include "db/timing/sdc/create_sdc_commands.h"
#include "db/timing/sdc/parse_sdc_commands.h"

namespace open_edi {
namespace db {

// general purpose commands
void bindSdcGeneralPurposeCommandsToParser(Tcl_Interp *itp) {
}

// object access commands
void bindSdcObjectAccessCommandsToParser(Tcl_Interp *itp) {

}

// timing constraints
void bindSdcTimingConstraintsToParser(Tcl_Interp *itp) {


}

// environment commands 
void bindSdcEnvironmentCommandsToParser(Tcl_Interp *itp) {
    Tcl_CreateCommand(itp, "set_case_analysis", parseSdcSetCaseAnalysis, NULL, NULL);
}

// multivoltage power commands
void bindSdcMultivoltagePowerCommandsToParser(Tcl_Interp *itp) {


}

void bindSdcCommandsToParser(Tcl_Interp *itp) {
    bindSdcGeneralPurposeCommandsToParser(itp);
    bindSdcObjectAccessCommandsToParser(itp);
    bindSdcTimingConstraintsToParser(itp);
    bindSdcEnvironmentCommandsToParser(itp);
    bindSdcMultivoltagePowerCommandsToParser(itp);
}

// main register tcl commands
void registerSdcCommands(Tcl_Interp *itp) {
    createSdcCommands();
    bindSdcCommandsToParser(itp);
}


}
}
