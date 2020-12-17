/* @file  place_tcl_command.h
 * @date  Oct 2020
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NiiCEDA Inc.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_PLACE_TCL_COMMAND_H_
#define EDI_PLACE_TCL_COMMAND_H_

#include <tcl.h>
#include "util/util.h"
#include "utility/src/Msg.h"

DREAMPLACE_BEGIN_NAMESPACE

const int kGPU  = 0;
const int kCPU  = 1;
const int kAUTO = 2;
void registerPlaceTclCommands(Tcl_Interp *itp);

DREAMPLACE_END_NAMESPACE

#endif // EDI_PLACE_TCL_COMMAND_H_

