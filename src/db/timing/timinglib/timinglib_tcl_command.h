/**
 * @file timinglib_tcl_command.h
 * @date 2020-09-14
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
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TCL_COMMAND_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TCL_COMMAND_H_

#include <tcl.h>

namespace open_edi {
namespace db {

int readTimingLibCommand(ClientData cld, Tcl_Interp *itp, int argc,
                         const char *argv[]);
int createAnalysisViewCommand(ClientData cld, Tcl_Interp *itp, int argc,
                              const char *argv[]);
int createAnalysisModeCommand(ClientData cld, Tcl_Interp *itp, int argc,
                              const char *argv[]);
int createAnalysisCornerCommand(ClientData cld, Tcl_Interp *itp, int argc,
                                const char *argv[]);
int setAnalysisViewStatusCommand(ClientData cld, Tcl_Interp *itp, int argc,
                                 const char *argv[]);

}  // namespace db
}  // namespace open_edi
#endif  // SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_TCL_COMMAND_H_
