
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

//#include <gperftools/profiler.h>
#include "flow/src/main_place.h"
#include "tcl_command/src/place_tcl_command.h"
#include "infra/command_manager.h"
#include "wire_length/src/wire_length.h"

DREAMPLACE_BEGIN_NAMESPACE
using namespace open_edi::infra;

// place_design
static int placeDesignCommand(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[])
{
  dreamplacePrint(kINFO, "Starting place_design\n");
  std::string jsonFile ;
  int flow_steps = 0x1FF;
  if (argc > 1) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    if (nullptr == cmd) {
      dreamplacePrint(kINFO, "error out\n");
    }
    /*if (cmd->isOptionSet("-json_file")) {
      bool res = cmd->getOptionValue("-json_file", jsonFile);
      message->info("get option %s int data %d \n", "-json_file", jsonFile);
    }*/

    if (cmd->isOptionSet("-global_place")) {
      bool value_bool;
      bool res = cmd->getOptionValue("-global_place", value_bool);
      if (value_bool == true) {
        message->info("get option -global_place bool data true \n");
      } else {
        flow_steps &= 0xFF;
        message->info("get option -global_place bool data false \n");
      }
    }
    if (cmd->isOptionSet("-detail_place")) {
      bool value_bool;
      bool res = cmd->getOptionValue("-detail_place", value_bool);
      if (value_bool == true) {
        message->info("get option -detail_place bool data true \n");
      } else {
        flow_steps &= 0x100;
        message->info("get option -detail_place bool data false \n");
      }
    }
  }
  // temporary input
  int num_bins_x = 1;
  int num_bins_y = 1;
  bool save_db   = false;
  bool gpu       = true;
  Para para(num_bins_x, num_bins_y, flow_steps, save_db, gpu, jsonFile);
  MainPlace place(para);
  place.run();
  return TCL_OK;
} // end of place_design

static void registerPlaceDesignManager()
{
  CommandManager* cmd_manager = CommandManager::getCommandManager();
//  Command* command = cmd_manager->createCommand("place_design", "Placement full flow", 
//                        *(new Option("-detail_place", OptionDataType::kBool, false, "turn on detail place\n"))
//                      + *(new Option("-global_place", OptionDataType::kBool, false, "turn on global place\n"))
//                      + *(new Option("-json_file", OptionDataType::kString, false, "specify jason file\n"))
//                     );
}

static int calcWLCommand(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[])
{
  dreamplacePrint(kINFO, "Starting calculate_wire_length\n");
  int value_e = 0;
  if (argc > 1) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    if (cmd->isOptionSet("-mode")) {
      bool res = cmd->getOptionValue("-mode", value_e);
      message->info("get option -mode enum data %d \n", value_e);
    }
  }
  bool isGPU = false;
  WireLength wl(isGPU);
  wl.run();

  return TCL_OK;
}

static void registerWLManager()
{
  CommandManager* cmd_manager = CommandManager::getCommandManager();
  std::vector<std::string>* enums = new std::vector<std::string>();
  enums->push_back("HPWL");
  enums->push_back("MST");
  enums->push_back("FLUTE");
  enums->push_back("ALL");
  //Command* command = cmd_manager->createCommand("report_wire_length", "report wire legth",
  //                   *(new Option("-mode", OptionDataType::kEnum, false, enums, "set mode of report_wire_length\n"))
  //                   );
}

void registerPlaceTclCommands(Tcl_Interp *itp)
{
  registerPlaceDesignManager();
  Tcl_CreateCommand(itp, "place_design", placeDesignCommand, NULL, NULL);
  registerWLManager();
  Tcl_CreateCommand(itp, "report_wire_length", calcWLCommand, NULL, NULL);
}

DREAMPLACE_END_NAMESPACE
