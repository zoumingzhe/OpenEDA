/**
 * @file parse_sdc_commands_manager.h
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

#ifndef EDI_DB_TIMING_SDC_PARSE_SDC_COMMANDS_H_
#define EDI_DB_TIMING_SDC_PARSE_SDC_COMMANDS_H_

#include "tcl.h"

namespace open_edi {
namespace db {


// general purpose commands manager

// object access commands manager


// timing constraints manager

// environment commands
int parseSdcSetCaseAnalysis(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);

// multivoltage power commands manager


// read write sdc
int parseReadSdc(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseWriteSdc(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);

};

}
#endif //EDI_DB_TIMING_SDC_PARSE_SDC_COMMANDS_H_
