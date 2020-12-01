/**
 * @file register_sdc_commands.h
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
#ifndef EDI_DB_TIMING_SDC_REGISTER_SDC_COMMANDS_H_
#define EDI_DB_TIMING_SDC_REGISTER_SDC_COMMANDS_H_

#include <tcl.h>

namespace open_edi {
namespace db {

void registerSdcCommands(Tcl_Interp *itp);

}  // namespace db
}  // namespace open_edi
#endif  // EDI_DB_TIMING_SDC_REGISTER_SDC_COMMANDS_H_
