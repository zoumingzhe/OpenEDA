
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

#include <codecvt>
#include <libgen.h>
#include "flow/src/main_place.h"
#include "tcl_command/src/place_tcl_command.h"
#include "infra/command_manager.h"
#include "wire_length/src/wire_length.h"


DREAMPLACE_BEGIN_NAMESPACE
using namespace open_edi::infra;

static int placeDesignMain(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);

static int placeDesignWrap(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
 
  //write lef def out for dream place
  Tcl_Eval(itp, "write_lef place_input_db.lef");
  Tcl_Eval(itp, "write_def place_input_db.def");
  
  char dest[PATH_MAX];
  memset(dest,0,sizeof(dest)); // readlink does not null terminate!
  if (readlink("/proc/self/exe", dest, PATH_MAX) == -1) {
    perror("readlink");
    return -1;
  } else {
    char* pathCopy = strdup(dest);
    std::string srcdir(dirname(pathCopy));
    std::string dreamPlaceBinPath = srcdir + "/../dreamplace";
    //std::cout<<dreamPlaceBinPath<<std::endl;
    
    std::string dreamPlaceBin = dreamPlaceBinPath + "/Placer.py";
    //std::cout<<dreamPlaceBin<<std::endl;
    
    std::string jsonfile = dreamPlaceBinPath + "/run.json";
    //std::cout<<jsonfile<<std::endl;
 
    std::string cmd = "python " + dreamPlaceBin + " " + jsonfile + " | tee place.log";
    //std::cout<<cmd<<std::endl;

    Tcl_Eval(itp, cmd.c_str());
    Tcl_Eval(itp, "read_def place_output_db.def");

  }
  return TCL_OK;
}

static int placeDesignCommand(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
  dreamplacePrint(kINFO, "Starting place_design\n");
  
  bool isNormalFlow = true;
  //TODO: 
  //Command* cmd = CommandManager::parseCommand(argc, argv);
  //   if (cmd->isOptionSet("-wrapflow")) //default is true 
  //   bool res = cmd->getOptionValue("-global_place", value_bool);
  //   if(!res) {
  //   isNormalFlow = true;
  //   }
  
  int s = TCL_OK;
  if(isNormalFlow) {
    s = placeDesignMain(cld, itp, argc, argv);
  } else {
    s = placeDesignWrap(cld, itp, argc, argv);
  }
  dreamplacePrint(kINFO, "End place_design\n");
  return s;
}

// place_design
static int placeDesignMain(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[])
{
  std::string jsonFile;
  int flow_steps = kFullPlace;
  int processor = kAUTO;;
  if (argc > 1) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    if (nullptr == cmd) {
      dreamplacePrint(kINFO, "error out\n");
    }
    if (cmd->isOptionSet("-json_file")) {
      bool res = cmd->getOptionValue("-json_file", jsonFile);
      message->info("get option %s string data %s \n", "-json_file", jsonFile.c_str());
    }

    if (cmd->isOptionSet("-global_place")) {
      bool value_bool;
      bool res = cmd->getOptionValue("-global_place", value_bool);
      if (value_bool == true) {
        message->info("get option -global_place bool data true \n");
      } else {
        flow_steps &= kDetailPlace;
        message->info("get option -global_place bool data false \n");
      }
    }
    if (cmd->isOptionSet("-detail_place")) {
      bool value_bool;
      bool res = cmd->getOptionValue("-detail_place", value_bool);
      if (value_bool == true) {
        message->info("get option -detail_place bool data true \n");
      } else {
        flow_steps &= kGlobalPlace;
        message->info("get option -detail_place bool data false \n");
      }
    }
    if (cmd->isOptionSet("-enable_processor")) {
      bool res = cmd->getOptionValue("-enable_processor", processor);
      message->info("get option -mode enum data %d \n", processor);
    }
  }
  // temporary input
  int num_bins_x = 1;
  int num_bins_y = 1;
  bool save_db   = false;

  // specify processor by option and CUDA_FOUND
  bool gpu       = false;
#ifdef _CUDA_FOUND
  if (processor == kAUTO || processor == kGPU) {
    gpu = true;
  }
#endif  

  Para para(num_bins_x, num_bins_y, flow_steps, save_db, gpu, jsonFile);
  MainPlace place(para);
  place.run();
  return TCL_OK;
} // end of place_design

static void registerPlaceDesignManager(Tcl_Interp *itp)
{
  CommandManager* cmd_manager = CommandManager::getCommandManager();
  Command* command = cmd_manager->createCommand(itp, placeDesignCommand, "place_design", "Placement full flow", 
                        cmd_manager->createOption("-detail_place", OptionDataType::kBool, false, "turn on detail place\n")
                      + cmd_manager->createOption("-global_place", OptionDataType::kBool, false, "turn on global place\n")
                      + cmd_manager->createOption("-json_file", OptionDataType::kString, false, "specify jason file\n")
                      + cmd_manager->createOption("-enable_processor", OptionDataType::kEnum, false, "CPU GPU AUTO", "specify processor to run place\n")
                     );
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

static void registerWLManager(Tcl_Interp *itp)
{
  CommandManager* cmd_manager = CommandManager::getCommandManager();
  Command* command = cmd_manager->createCommand(itp, calcWLCommand, "report_wire_length", "report wire legth",
                     cmd_manager->createOption("-mode", OptionDataType::kEnum, false, "HPWL MST FLUTE ALL", "set mode of report_wire_length\n")
                     );
}

void registerPlaceTclCommands(Tcl_Interp *itp)
{
  registerPlaceDesignManager(itp);
  //Tcl_CreateCommand(itp, "place_design", placeDesignCommand, NULL, NULL);
  registerWLManager(itp);
  //Tcl_CreateCommand(itp, "report_wire_length", calcWLCommand, NULL, NULL);
}

DREAMPLACE_END_NAMESPACE
