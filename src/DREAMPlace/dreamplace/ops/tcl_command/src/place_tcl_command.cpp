
/* @file  place_tcl_command.cpp
 * @date  Oct 2020
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NiiCEDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include <tcl.h>
#include "flow/src/main_place.h"
#include "tcl_command/src/place_tcl_command.h"

DREAMPLACE_BEGIN_NAMESPACE

// place_design
static int placeDesignCommand(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[])
{
  dreamplacePrint(kINFO, "Starting place_design\n");
  if (argc < 2) {
    
    dreamplacePrint(kINFO, "error out\n");
    //return TCL_ERROR;
  }
  for (int i = 1; i < argc; ++i) 
  {
    if (!strcasecmp(argv[i], "-json_file")) {
      i++;
    } else if (!strcasecmp(argv[i], "-step")) {
      i++;
    } 
  }
  // temporary input
  int num_bins_x = 1;
  int num_bins_y = 1;
  int flow_steps = 0xFF;
  bool save_db   = false;
  bool gpu       = true;
  Para para(num_bins_x, num_bins_y, flow_steps, save_db, gpu);
  MainPlace place(para);
  place.run();
  return TCL_OK;
} // end of place_design

static int calcWLCommand(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[])
{
  dreamplacePrint(kINFO, "Starting calculate_wire_length\n");
  if (argc < 2) {
    return TCL_ERROR;
  }
  return TCL_OK;
}

void registerPlaceTclCommands(Tcl_Interp *itp)
{
  Tcl_CreateCommand(itp, "place_deigsn", placeDesignCommand, NULL, NULL);
  Tcl_CreateCommand(itp, "calculate_wire_legth", calcWLCommand, NULL, NULL);
}

DREAMPLACE_END_NAMESPACE
