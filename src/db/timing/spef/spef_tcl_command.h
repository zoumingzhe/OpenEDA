/**
 * @file spef_tcl_command.h
 * @date 2020-11-02
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
#ifndef EDI_DB_TIMING_SPEF_SPEF_TCL_COMMAND_H_
#define EDI_DB_TIMING_SPEF_SPEF_TCL_COMMAND_H_

#include <tcl.h>

namespace open_edi {
namespace db {

//bool parseSpefFile(const std::string &file, AnalysisCorner *corner);
int readSpefCommand(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
//void printReadSpefCommandHelp();
int writeSpefCommand(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
//void printWriteSpefCommandHelp();


}  // namespace db
}  // namespace open_edi
#endif  // EDI_DB_TIMING_SPEF_SPEF_TCL_COMMAND_H_
