/* @file  db_tcl_command.h
 * @date  Aug 2020
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NiiCEDA Inc.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_DB_DB_TCL_COMMAND_H_
#define EDI_DB_DB_TCL_COMMAND_H_

#include <tcl.h>
#include "util/util.h"

namespace open_edi {
namespace db {

void registerDatabaseTclCommands(Tcl_Interp *itp);

} // namespace db
} // namespace open_edi
#endif // EDI_DB_DB_TCL_COMMAND_H_

