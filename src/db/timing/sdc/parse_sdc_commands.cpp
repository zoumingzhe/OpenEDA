/**
 * @file parse_sdc_commands.cpp
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
#include "db/core/cell.h"
#include "db/core/db.h"
#include "db/timing/sdc/parse_sdc_commands.h"
#include "db/timing/sdc/sdc.h"
#include "db/timing/timinglib/analysis_view.h"
#include "db/timing/timinglib/analysis_mode.h"
#include "infra/command.h"
#include "infra/command_manager.h"

namespace open_edi {
namespace db {

using Command = open_edi::infra::Command;
using CommandPtr = std::shared_ptr<Command>;
using CommandManager = open_edi::infra::CommandManager;

using StringVector = std::vector<std::string>;
using StringVectorPtr = std::shared_ptr<StringVector>;
using DoubleVector = std::vector<double>;

SdcPtr getSdc() {
    SdcPtr sdc = std::make_shared<Sdc>();
    Timing *timing_lib = getTimingLib();
    if (!timing_lib) {
        //TODO message
        return sdc;
    }
    //get first one
    size_t first_view = 0;
    AnalysisView *view = timing_lib->getAnalysisView(first_view);
    if (!view) {
        //TODO message
        return sdc;
    }
    AnalysisMode *mode = view->getAnalysisMode();
    if (!mode) {
        //TODO message
        return sdc;
    }
    return mode->getSdc();
}

// general purpose commands manager
int parseSdcCurrentInstance(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (cmd->isOptionSet("-instance")) {
        std::string instance="";
        bool res = cmd->getOptionValue("-instance", instance);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", instance.c_str());
    }
    
    return TCL_OK;
}

int parseSdcSetUnits(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (cmd->isOptionSet("-capacitance")) {
        std::string capacitance="";
        bool res = cmd->getOptionValue("-capacitance", capacitance);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", capacitance.c_str());
    }
   
    if (cmd->isOptionSet("-resistance")) {
        std::string resistance="";
        bool res = cmd->getOptionValue("-resistance", resistance);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", resistance.c_str());
    }
    if (cmd->isOptionSet("-time")) {
        std::string time="";
        bool res = cmd->getOptionValue("-time", time);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", time.c_str());
    }
    if (cmd->isOptionSet("-voltage")) {
        std::string voltage="";
        bool res = cmd->getOptionValue("-voltage", voltage);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", voltage.c_str());
    }
    if (cmd->isOptionSet("-current")) {
        std::string current="";
        bool res = cmd->getOptionValue("-current", current);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", current.c_str());
    }
    if (cmd->isOptionSet("-power")) {
        std::string power="";
        bool res = cmd->getOptionValue("-power", power);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", power.c_str());
    }

    return TCL_OK;
}
int parseSdcSetHierarchySeparator(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("separator"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("separator")) {
        std::string separator="";
        bool res = cmd->getOptionValue("separator", separator);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", separator.c_str());
    }

    return TCL_OK;
}
// object access commands manager
int parseSdcAllClocks(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if(cmd==nullptr){
        return TCL_ERROR;	
	}
    return TCL_OK;
}

int parseSdcAllInputs(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("-level_sensitive") and cmd->isOptionSet("-edge_triggered"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("-level_sensitive")) {
        bool level_sensitive=false;
        bool res = cmd->getOptionValue("-level_sensitive", level_sensitive);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", level_sensitive);
    }
    if (cmd->isOptionSet("-edge_triggered")) {
        bool edge_triggered=false;
        bool res = cmd->getOptionValue("-edge_triggered", edge_triggered);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", edge_triggered);
    }

    if (cmd->isOptionSet("-clock")) {
        StringVector clock;
        //assert(clock);
        bool res = cmd->getOptionValue("-clock", clock);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& clock_name : clock) {
  
            message->info("get second value %s \n", clock_name.c_str());
        }
    }
    return TCL_OK;
}

int parseSdcAllOutputs(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("-level_sensitive") and cmd->isOptionSet("-edge_triggered"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("-level_sensitive")) {
        bool level_sensitive=false;
        bool res = cmd->getOptionValue("-level_sensitive", level_sensitive);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", level_sensitive);
    }

    if (cmd->isOptionSet("-edge_triggered")) {
        bool edge_triggered=false;
        bool res = cmd->getOptionValue("-edge_triggered", edge_triggered);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", edge_triggered);
    }

    if (cmd->isOptionSet("-clock")) {
        StringVector clock;
        //assert(clock);
        bool res = cmd->getOptionValue("-clock", clock);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& clock_name : clock) {

            message->info("get second value %s \n", clock_name.c_str());
        }
    }
    return TCL_OK;
}

int parseSdcAllRegisters(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (cmd->isOptionSet("-no_hierarchy")) {
        bool no_hierarchy=false;
        bool res = cmd->getOptionValue("-no_hierarchy", no_hierarchy);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", no_hierarchy);
    }
    if (cmd->isOptionSet("-cells")) {
        bool cells=false;
        bool res = cmd->getOptionValue("-cells", cells);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", cells);
    }
    if (cmd->isOptionSet("-data_pins")) {
        bool data_pins=false;
        bool res = cmd->getOptionValue("-data_pins", data_pins);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", data_pins);
    }
    if (cmd->isOptionSet("-clock_pins")) {
        bool clock_pins=false;
        bool res = cmd->getOptionValue("-clock_pins", clock_pins);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", clock_pins);
    }
    if (cmd->isOptionSet("-slave_clock_pins")) {
        bool slave_clock_pins=false;
        bool res = cmd->getOptionValue("-slave_clock_pins", slave_clock_pins);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", slave_clock_pins);
    }
    if (cmd->isOptionSet("-async_pins")) {
        bool async_pins=false;
        bool res = cmd->getOptionValue("-async_pins", async_pins);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", async_pins);
    }
    if (cmd->isOptionSet("-output_pins")) {
        bool output_pins=false;
        bool res = cmd->getOptionValue("-output_pins", output_pins);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", output_pins);
    }
    if (cmd->isOptionSet("-level_sensitive")) {
        bool level_sensitive=false;
        bool res = cmd->getOptionValue("-level_sensitive", level_sensitive);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", level_sensitive);
    }
    if (cmd->isOptionSet("-edge_triggered")) {
        bool edge_triggered=false;
        bool res = cmd->getOptionValue("-edge_triggered", edge_triggered);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", edge_triggered);
    }
    if (cmd->isOptionSet("-master_slave")) {
        bool master_slave=false;
        bool res = cmd->getOptionValue("-master_slave", master_slave);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", master_slave);
    }
    if (cmd->isOptionSet("-hsc")) {
        std::string hsc="";
        bool res = cmd->getOptionValue("-hsc", hsc);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", hsc.c_str());
    }
    if (cmd->isOptionSet("-clock")) {
        StringVector clock;
        //assert(clock);
        bool res = cmd->getOptionValue("-clock", clock);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& clock_name : clock) {

            message->info("get second value %s \n", clock_name.c_str());
        }
    }

    if (cmd->isOptionSet("-rise_clock")) {
        StringVector rise_clock;
        assert(clock);
        bool res = cmd->getOptionValue("-rise_clock", rise_clock);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& rise_clock_name : rise_clock) {

            message->info("get second value %s \n", rise_clock_name.c_str());
        }
    }
    if (cmd->isOptionSet("-fall_clock")) {
        StringVector fall_clock;
        assert(clock);
        bool res = cmd->getOptionValue("-fall_clock", fall_clock);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& fall_clock_name : fall_clock) {

            message->info("get second value %s \n", fall_clock_name.c_str());
        }
    }


    return TCL_OK;
}
int parseSdcCurrentDesign(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if(cmd==nullptr){
        return TCL_ERROR;	
	}
    return TCL_OK;
}
int parseSdcGetCells(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if(!((cmd->isOptionSet("patterns") and !(cmd->isOptionSet("-of_objects"))) or (cmd->isOptionSet("-of_objects") and !(cmd->isOptionSet("-of_objects"))) or !(cmd->isOptionSet("patterns")))){
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("-hsc")) {
        std::string hsc="";
        bool res = cmd->getOptionValue("-hsc", hsc);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", hsc.c_str());
    }
    if (cmd->isOptionSet("-hierarchical")) {
        bool hierarchical=false;
        bool res = cmd->getOptionValue("-hierarchical", hierarchical);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", hierarchical);
    }
    if (cmd->isOptionSet("-nocase")) {
        bool nocase=false;
        bool res = cmd->getOptionValue("-nocase", nocase);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", nocase);
    }
    if (cmd->isOptionSet("-regexp")) {
        bool regexp=false;
        bool res = cmd->getOptionValue("-regexp", regexp);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", regexp);
    }
	if (cmd->isOptionSet("-of_objects")) {
        StringVector of_objects;
        //assert(of_objects);
        bool res = cmd->getOptionValue("-of_objects", of_objects);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& of_objects_name : of_objects) {

            message->info("get second value %s \n", of_objects_name.c_str());
        }
    }
    if (cmd->isOptionSet("-patterns")) {
        StringVector patterns;
        //assert(patterns);
        bool res = cmd->getOptionValue("-patterns", patterns);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& patterns_name : patterns) {

            message->info("get second value %s \n", patterns_name.c_str());
        }
    }
	
    return TCL_OK;
}

int parseSdcGetClocks(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (cmd->isOptionSet("-nocase")) {
        bool nocase=false;
        bool res = cmd->getOptionValue("-nocase", nocase);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", nocase);
    }
    if (cmd->isOptionSet("-regexp")) {
        bool regexp=false;
        bool res = cmd->getOptionValue("-regexp", regexp);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", regexp);
    }
    if (cmd->isOptionSet("-patterns")) {
        StringVector patterns;
        //assert(patterns);
        bool res = cmd->getOptionValue("-patterns", patterns);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& patterns_name : patterns) {

            message->info("get second value %s \n", patterns_name.c_str());
        }
    }
	
    return TCL_OK;
}
int parseSdcGetLibCells(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("patterns"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("-hsc")) {
        std::string hsc="";
        bool res = cmd->getOptionValue("-hsc", hsc);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", hsc.c_str());
    }
   
    if (cmd->isOptionSet("-nocase")) {
        bool nocase=false;
        bool res = cmd->getOptionValue("-nocase", nocase);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", nocase);
    }
    if (cmd->isOptionSet("-regexp")) {
        bool regexp=false;
        bool res = cmd->getOptionValue("-regexp", regexp);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", regexp);
    }
	
    if (cmd->isOptionSet("-patterns")) {
        StringVector patterns;
        //assert(patterns);
        bool res = cmd->getOptionValue("-patterns", patterns);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& patterns_name : patterns) {

            message->info("get second value %s \n", patterns_name.c_str());
        }
    }
	
	
    return TCL_OK;
}

int parseSdcGetLibPins(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("patterns"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("-nocase")) {
        bool nocase=false;
        bool res = cmd->getOptionValue("-nocase", nocase);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", nocase);
    }
    if (cmd->isOptionSet("-regexp")) {
        bool regexp=false;
        bool res = cmd->getOptionValue("-regexp", regexp);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", regexp);
    }
	
    if (cmd->isOptionSet("-patterns")) {
        StringVector patterns;
        //assert(patterns);
        bool res = cmd->getOptionValue("-patterns", patterns);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& patterns_name : patterns) {

            message->info("get second value %s \n", patterns_name.c_str());
        }
    }
	
	
    return TCL_OK;
}
int parseSdcGetLibs(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (cmd->isOptionSet("-nocase")) {
        bool nocase=false;
        bool res = cmd->getOptionValue("-nocase", nocase);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", nocase);
    }
    if (cmd->isOptionSet("-regexp")) {
        bool regexp=false;
        bool res = cmd->getOptionValue("-regexp", regexp);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", regexp);
    }
	
    if (cmd->isOptionSet("-patterns")) {
        StringVector patterns;
        //assert(patterns);
        bool res = cmd->getOptionValue("-patterns", patterns);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& patterns_name : patterns) {

            message->info("get second value %s \n", patterns_name.c_str());
        }
    }
    return TCL_OK;
}
int parseSdcGetNets(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!((cmd->isOptionSet("patterns") and !(cmd->isOptionSet("-of_objects"))) or (cmd->isOptionSet("-of_objects") and !(cmd->isOptionSet("-of_objects"))) or !(cmd->isOptionSet("patterns")))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("-hsc")) {
        std::string hsc="";
        bool res = cmd->getOptionValue("-hsc", hsc);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", hsc.c_str());
    }
    if (cmd->isOptionSet("-hierarchical")) {
        bool hierarchical=false;
        bool res = cmd->getOptionValue("-hierarchical", hierarchical);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", hierarchical);
    }
    if (cmd->isOptionSet("-nocase")) {
        bool nocase=false;
        bool res = cmd->getOptionValue("-nocase", nocase);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", nocase);
    }
    if (cmd->isOptionSet("-regexp")) {
        bool regexp=false;
        bool res = cmd->getOptionValue("-regexp", regexp);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", regexp);
    }
	if (cmd->isOptionSet("-of_objects")) {
        StringVector of_objects;
        //assert(of_objects);
        bool res = cmd->getOptionValue("-of_objects", of_objects);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& of_objects_name : of_objects) {

            message->info("get second value %s \n", of_objects_name.c_str());
        }
    }
    if (cmd->isOptionSet("-patterns")) {
        StringVector patterns;
        //assert(patterns);
        bool res = cmd->getOptionValue("-patterns", patterns);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& patterns_name : patterns) {

            message->info("get second value %s \n", patterns_name.c_str());
        }
    }
	
    return TCL_OK;
}
int parseSdcGetPins(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (cmd->isOptionSet("-hsc")) {
        std::string hsc="";
        bool res = cmd->getOptionValue("-hsc", hsc);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", hsc.c_str());
    }
    if (cmd->isOptionSet("-hierarchical")) {
        bool hierarchical=false;
        bool res = cmd->getOptionValue("-hierarchical", hierarchical);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", hierarchical);
    }
    if (cmd->isOptionSet("-nocase")) {
        bool nocase=false;
        bool res = cmd->getOptionValue("-nocase", nocase);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", nocase);
    }
    if (cmd->isOptionSet("-regexp")) {
        bool regexp=false;
        bool res = cmd->getOptionValue("-regexp", regexp);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", regexp);
    }
	if (cmd->isOptionSet("-of_objects")) {
        StringVector of_objects;
        //assert(of_objects);
        bool res = cmd->getOptionValue("-of_objects", of_objects);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& of_objects_name : of_objects) {

            message->info("get second value %s \n", of_objects_name.c_str());
        }
    }
    if (cmd->isOptionSet("-patterns")) {
        StringVector patterns;
        //assert(patterns);
        bool res = cmd->getOptionValue("-patterns", patterns);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& patterns_name : patterns) {

            message->info("get second value %s \n", patterns_name.c_str());
        }
    }
	
    return TCL_OK;
}
int parseSdcGetPorts(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (cmd->isOptionSet("-nocase")) {
        bool nocase=false;
        bool res = cmd->getOptionValue("-nocase", nocase);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", nocase);
    }
    if (cmd->isOptionSet("-regexp")) {
        bool regexp=false;
        bool res = cmd->getOptionValue("-regexp", regexp);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %d \n", regexp);
    }
	
    if (cmd->isOptionSet("-patterns")) {
        StringVector patterns;
        //assert(patterns);
        bool res = cmd->getOptionValue("-patterns", patterns);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& patterns_name : patterns) {

            message->info("get second value %s \n", patterns_name.c_str());
        }
    }
    return TCL_OK;
}


// multivoltage power commands manager
int parseSdcCreateVoltageArea(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!((cmd->isOptionSet("-name") and cmd->isOptionSet("cell_list")))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("-name")) {
        std::string name="";
        bool res = cmd->getOptionValue("-name", name);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", name.c_str());
    }
    if (cmd->isOptionSet("-coordinate")) {
        StringVector coordinate;
        //assert(coordinate);
        bool res = cmd->getOptionValue("-coordinate", coordinate);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& coordinate_name : coordinate) {

            message->info("get second value %s \n", coordinate_name.c_str());
        }
    }
    if (cmd->isOptionSet("-guard_band_x")) {
        DoubleVector guard_band_x;
        //assert(guard_band_x);
        bool res = cmd->getOptionValue("-guard_band_x", guard_band_x);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& guard_band_x_name : guard_band_x) {

            message->info("get second value %f \n", guard_band_x_name);
        }
    }
    if (cmd->isOptionSet("-guard_band_y")) {
        DoubleVector guard_band_y;
        //assert(guard_band_y);
        bool res = cmd->getOptionValue("-guard_band_y", guard_band_y);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& guard_band_y_name : guard_band_y) {

            message->info("get second value %f \n", guard_band_y_name);
        }
    }
    if (cmd->isOptionSet("cell_list")) {
        StringVector cell_list;
        //assert(cell_list);
        bool res = cmd->getOptionValue("cell_list", cell_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& cell_list_name : cell_list) {

            message->info("get second value %s \n", cell_list_name.c_str());
        }
    }
    return TCL_OK;
}
int parseSdcSetLevelShifterStrategy(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("-rule"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("-rule")) {
        std::string rule="";
        bool res = cmd->getOptionValue("-rule", rule);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", rule.c_str());
    }
    return TCL_OK;
}
int parseSdcSetLevelShifterThreshold(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("-voltage"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("-voltage")) {
        double voltage=0;
        bool res = cmd->getOptionValue("-voltage", voltage);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %f \n", voltage);
    }
    if (cmd->isOptionSet("-percent")) {
        double percent=0;
        bool res = cmd->getOptionValue("-percent", percent);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %f \n", percent);
    }
    
    return TCL_OK;
}
int parseSdcSetMaxDynamicPower(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("power"))) {                         //´Ë´¦Óësdc2.1.tclÇø±ðÒ»¸övalue
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("power")) {
        double power=0;
        bool res = cmd->getOptionValue("power", power);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %f \n", power);
    }
    if (cmd->isOptionSet("-unit")) {
        std::string unit="";
        bool res = cmd->getOptionValue("-unit", unit);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", unit.c_str());
    }
    
    return TCL_OK;
}
int parseSdcSetMaxLeakagePower(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
      Command* cmd = CommandManager::parseCommand(argc, argv);
      assert(cmd);
      if (!(cmd->isOptionSet("power"))) {                         //´Ë´¦Óësdc2.1.tclÇø±ðÒ»¸övalue
          return TCL_ERROR;
      }
      if (cmd->isOptionSet("power")) {
          double power=0;
          bool res = cmd->getOptionValue("power", power);
          if (!res) {
              //TODO messages
              return TCL_ERROR;
          }
          //case_analysis_ptr->setValue(value);
          message->info("get first value %f \n", power);
      }
      if (cmd->isOptionSet("-unit")) {
          std::string unit="";
          bool res = cmd->getOptionValue("-unit", unit);
          if (!res) {
              //TODO messages
              return TCL_ERROR;
          }
          //case_analysis_ptr->setValue(value);
          message->info("get first value %s \n", unit.c_str());
      }
 
      return TCL_OK;
  }
// timing constraints manager
//chenqian
//01 create_clock
int parseSdcCreateClock(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!(cmd->isOptionSet("-period") and (cmd->isOptionSet("-name") or cmd->isOptionSet("port_pin_list")))) {
		return TCL_ERROR;
	}
	
    if (cmd->isOptionSet("-period")) {
        double period = 0.0;
        bool res = cmd->getOptionValue("-period", period);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
		//Assignment
        message->info("get first value %f \n", period);
    }
	if (cmd->isOptionSet("-name")) {
		std::string name = "";
        bool res = cmd->getOptionValue("-name", name);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get second value %s \n", name.c_str());
	}
	if (cmd->isOptionSet("-comment")) {
		std::string comment = "";
        bool res = cmd->getOptionValue("-comment", comment);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get third value %s \n", comment.c_str());
	}
	if (cmd->isOptionSet("-waveform")) {
		DoubleVector waveform_list;
        bool res = cmd->getOptionValue("-waveform", waveform_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &waveform : waveform_list) {
            //TODO DB team did not implement the API to get pin/term from name
			//Assignment
            message->info("get fourth value %f \n", waveform);
        }
	}
	if (cmd->isOptionSet("port_pin_list")) {
		StringVector port_pin_list;
        bool res = cmd->getOptionValue("port_pin_list", port_pin_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &pin_name : port_pin_list) {
            //TODO DB team did not implement the API to get pin/term from name
			//Assignment
            message->info("get fifth value %s \n", pin_name.c_str());
        }
	}
	if (cmd->isOptionSet("-add")) {
		bool add = false;
        bool res = cmd->getOptionValue("-add", add);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
		//Assignment
        message->info("get sixth value %d \n", add);
	}
	
    return TCL_OK;
}
//02 create_generated_clock
int parseSdcCreateGeneratedClock(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!(	cmd->isOptionSet("-source") and 
			cmd->isOptionSet("port_pin_list") and 
			(!cmd->isOptionSet("-multiply_by")) and 
			cmd->isOptionSet("-divide_by") )) {
		return TCL_ERROR;
	}
	
    if (cmd->isOptionSet("-name")) {
        std::string name = "";
        bool res = cmd->getOptionValue("-name", name);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
		//Assignment
        message->info("get first value %s \n", name.c_str());
    }
	if (cmd->isOptionSet("-source")) {
		StringVector source_list;
        bool res = cmd->getOptionValue("-source", source_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &source : source_list) {
            //TODO DB team did not implement the API to get pin/term from name
			//Assignment
            message->info("get second value %s \n", source.c_str());
        }
	}
	if (cmd->isOptionSet("-edges")) {
		DoubleVector edges_list;
        bool res = cmd->getOptionValue("-edges", edges_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &edges : edges_list) {
            //TODO DB team did not implement the API to get pin/term from name
			//Assignment
            message->info("get third value %f \n", edges);
        }
	}
    if (cmd->isOptionSet("-divide_by")) {
        int divide_by = 0;
        bool res = cmd->getOptionValue("-divide_by", divide_by);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
		//Assignment
        message->info("get fourth value %d \n", divide_by);
    }
    if (cmd->isOptionSet("-multiply_by")) {
        int multiply_by = 0;
        bool res = cmd->getOptionValue("-multiply_by", multiply_by);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
		//Assignment
        message->info("get fifth value %d \n", multiply_by);
    }
	if (cmd->isOptionSet("-edge_shift")) {
		StringVector edge_shift_list;
        bool res = cmd->getOptionValue("-edge_shift", edge_shift_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &edge_shift : edge_shift_list) {
            //TODO DB team did not implement the API to get pin/term from name
			//Assignment
            message->info("get sixth value %s \n", edge_shift.c_str());
        }
	}
    if (cmd->isOptionSet("-duty_cycle")) {
        double duty_cycle = 0.0;
        bool res = cmd->getOptionValue("-duty_cycle", duty_cycle);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
		//Assignment
        message->info("get seventh value %f \n", duty_cycle);
    }
    if (cmd->isOptionSet("-invert")) {
        bool invert = false;
        bool res = cmd->getOptionValue("-invert", invert);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
		//Assignment
        message->info("get eighth value %d \n", invert);
    }
	if (cmd->isOptionSet("port_pin_list")) {
		StringVector port_pin_list;
        bool res = cmd->getOptionValue("port_pin_list", port_pin_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &pin_name : port_pin_list) {
            //TODO DB team did not implement the API to get pin/term from name
			//Assignment
            message->info("get ninth value %s \n", pin_name.c_str());
        }
	}
    if (cmd->isOptionSet("-add")) {
        bool add = false;
        bool res = cmd->getOptionValue("-add", add);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
		//Assignment
        message->info("get tenth value %d \n", add);
    }
    if (cmd->isOptionSet("-comment")) {
        std::string comment = "";
        bool res = cmd->getOptionValue("-comment", comment);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
		//Assignment
        message->info("get eleventh value %s \n", comment.c_str());
    }
	if (cmd->isOptionSet("-master_clock")) {
		StringVector master_clock_list;
        bool res = cmd->getOptionValue("-master_clock", master_clock_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &master_clock : master_clock_list) {
            //TODO DB team did not implement the API to get pin/term from name
			//Assignment
            message->info("get twelfth value %s \n", master_clock.c_str());
        }
	}
	return TCL_OK;
}
//03 group_path
int parseSdcGroupPath(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    
	//constraint
    // junliu changed
	if (!(  (cmd->isOptionSet("-name") and (!cmd->isOptionSet("-default"))) or 
			(cmd->isOptionSet("-default") and (!cmd->isOptionSet("-name"))) or 
			(!cmd->isOptionSet("-name")) and 
			(cmd->isOptionSet("-from") xor cmd->isOptionSet("-rise_from") xor cmd->isOptionSet("-fall_from")))) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("-name")) {
        std::string name = "";
        bool res = cmd->getOptionValue("-name", name);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
		//Assignment
        message->info("get first value %s \n", name.c_str());
    }
    if (cmd->isOptionSet("-default")) {
        bool default_ = false;
        bool res = cmd->getOptionValue("-default", default_);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
		//Assignment
        message->info("get second value %d \n", default_);
    }
    if (cmd->isOptionSet("-weight")) {
        double weight = 0.0;
        bool res = cmd->getOptionValue("-weight", weight);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
		//Assignment
        message->info("get third value %f \n", weight);
    }
	if (cmd->isOptionSet("-from")) {
		StringVector from_list;
        bool res = cmd->getOptionValue("-from", from_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &from : from_list) {
            //TODO DB team did not implement the API to get pin/term from name
			//Assignment
            message->info("get fourth value %s \n", from.c_str());
        }
	}
	if (cmd->isOptionSet("-rise_from")) {
		StringVector rise_from_list;
        bool res = cmd->getOptionValue("-rise_from", rise_from_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &rise_from : rise_from_list) {
            //TODO DB team did not implement the API to get pin/term from name
			//Assignment
            message->info("get fifth value %s \n", rise_from.c_str());
        }
	}
	if (cmd->isOptionSet("-fall_from")) {
		StringVector fall_from_list;
        bool res = cmd->getOptionValue("-fall_from", fall_from_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &fall_from : fall_from_list) {
            //TODO DB team did not implement the API to get pin/term from name
			//Assignment
            message->info("get sixth value %s \n", fall_from.c_str());
        }
	}
	if (cmd->isOptionSet("-to")) {
		StringVector to_list;
        bool res = cmd->getOptionValue("-to", to_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &to : to_list) {
            //TODO DB team did not implement the API to get pin/term from name
			//Assignment
            message->info("get seventh value %s \n", to.c_str());
        }
	}
	if (cmd->isOptionSet("-rise_to")) {
		StringVector rise_to_list;
        bool res = cmd->getOptionValue("-rise_to", rise_to_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &rise_to : rise_to_list) {
            //TODO DB team did not implement the API to get pin/term from name
			//Assignment
            message->info("get eighth value %s \n", rise_to.c_str());
        }
	}
	if (cmd->isOptionSet("-fall_to")) {
		StringVector fall_to_list;
        bool res = cmd->getOptionValue("-fall_to", fall_to_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &fall_to : fall_to_list) {
            //TODO DB team did not implement the API to get pin/term from name
			//Assignment
            message->info("get ninth value %s \n", fall_to.c_str());
        }
	}
	if (cmd->isOptionSet("-through")) {
		StringVector through_list;
        bool res = cmd->getOptionValue("-through", through_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &through : through_list) {
            //TODO DB team did not implement the API to get pin/term from name
			//Assignment
            message->info("get tenth value %s \n", through.c_str());
        }
	}
	if (cmd->isOptionSet("-rise_through")) {
		StringVector rise_through_list;
        bool res = cmd->getOptionValue("-rise_through", rise_through_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &rise_through : rise_through_list) {
            //TODO DB team did not implement the API to get pin/term from name
			//Assignment
            message->info("get eleventh value %s \n", rise_through.c_str());
        }
	}
	if (cmd->isOptionSet("-fall_through")) {
		StringVector fall_through_list;
        bool res = cmd->getOptionValue("-fall_through", fall_through_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &fall_through : fall_through_list) {
            //TODO DB team did not implement the API to get pin/term from name
			//Assignment
            message->info("get twelfth value %s \n", fall_through.c_str());
        }
	}
    if (cmd->isOptionSet("-comment")) {
        std::string comment = "";
        bool res = cmd->getOptionValue("-comment", comment);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
		//Assignment
        message->info("get thirteenth value %s \n", comment.c_str());
    }
	
	return TCL_OK;
}
//04 set_clock_gating_check
int parseSdcSetClockGatingCheck(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!(	(cmd->isOptionSet("-setup") or cmd->isOptionSet("-hold") or cmd->isOptionSet("-high") or cmd->isOptionSet("-low")) and
			(!(cmd->isOptionSet("-high") and cmd->isOptionSet("-low")))
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("-setup")) {
		double setup = 0.0;
		bool res = cmd->getOptionValue("-setup", setup);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get first value %f \n", setup);
	}
	if (cmd->isOptionSet("-hold")) {
		double hold = 0.0;
		bool res = cmd->getOptionValue("-hold", hold);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get second value %f \n", hold);
	}
	if (cmd->isOptionSet("-rise")) {
		bool rise = false;
		bool res = cmd->getOptionValue("-rise", rise);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get third value %d \n", rise);
	}
	if (cmd->isOptionSet("-fall")) {
		bool fall = false;
		bool res = cmd->getOptionValue("-fall", fall);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fourth value %d \n", fall);
	}
	if (cmd->isOptionSet("-high")) {
		bool high = false;
		bool res = cmd->getOptionValue("-high", high);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fifth value %d \n", high);
	}
	if (cmd->isOptionSet("-low")) {
		bool low = false;
		bool res = cmd->getOptionValue("-low", low);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get sixth value %d \n", low);
	}
	if (cmd->isOptionSet("object_list")) {
		StringVector object_list;
		bool res = cmd->getOptionValue("object_list", object_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &object : object_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get seventh value %s \n", object.c_str());
		}
	}
	
	return TCL_OK;
}
//05 set_clock_groups
int parseSdcSetClockGroups(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!(	cmd->isOptionSet("-physically_exclusive") xor 
			cmd->isOptionSet("-logically_exclusive") xor 
			cmd->isOptionSet("-asynchronous")
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("-name")) {
		std::string name = "";
		bool res = cmd->getOptionValue("-name", name);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get first value %s \n", name.c_str());
	}
	if (cmd->isOptionSet("-physically_exclusive")) {
		bool physically_exclusive = false;
		bool res = cmd->getOptionValue("-physically_exclusive", physically_exclusive);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get second value %d \n", physically_exclusive);
	}
	if (cmd->isOptionSet("-logically_exclusive")) {
		bool logically_exclusive = false;
		bool res = cmd->getOptionValue("-logically_exclusive", logically_exclusive);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get third value %d \n", logically_exclusive);
	}
	if (cmd->isOptionSet("-asynchronous")) {
		bool asynchronous = false;
		bool res = cmd->getOptionValue("-asynchronous", asynchronous);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fourth value %d \n", asynchronous);
	}
	if (cmd->isOptionSet("-allow_paths")) {
		bool allow_paths = false;
		bool res = cmd->getOptionValue("-allow_paths", allow_paths);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fifth value %d \n", allow_paths);
	}
	if (cmd->isOptionSet("-group")) {
		StringVector group_list;
		bool res = cmd->getOptionValue("-group", group_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &group : group_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get sixth value %s \n", group.c_str());
		}
	}
	if (cmd->isOptionSet("-comment")) {
		std::string comment = "";
		bool res = cmd->getOptionValue("-comment", comment);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get seventh value %s \n", comment.c_str());
	}
	return TCL_OK;
}
//06 set_clock_latency
int parseSdcSetClockLatency(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!(	cmd->isOptionSet("delay") and 
			cmd->isOptionSet("object_list") 
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("delay")) {
		double delay = 0.0;
		bool res = cmd->getOptionValue("delay", delay);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get first value %f \n", delay);
	}
	if (cmd->isOptionSet("object_list")) {
		StringVector object_list;
		bool res = cmd->getOptionValue("object_list", object_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &object : object_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get second value %s \n", object.c_str());
		}
	}
	if (cmd->isOptionSet("-rise")) {
		bool rise = false;
		bool res = cmd->getOptionValue("-rise", rise);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get third value %d \n", rise);
	}
	if (cmd->isOptionSet("-fall")) {
		bool fall = false;
		bool res = cmd->getOptionValue("-fall", fall);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fourth value %d \n", fall);
	}
	if (cmd->isOptionSet("-min")) {
		bool min = false;
		bool res = cmd->getOptionValue("-min", min);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fifth value %d \n", min);
	}
	if (cmd->isOptionSet("-max")) {
		bool max = false;
		bool res = cmd->getOptionValue("-max", max);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get sixth value %d \n", max);
	}
	if (cmd->isOptionSet("-dynamic")) {
		bool dynamic = false;
		bool res = cmd->getOptionValue("-dynamic", dynamic);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get seventh value %d \n", dynamic);
	}
	if (cmd->isOptionSet("-source")) {
		bool source = false;
		bool res = cmd->getOptionValue("-source", source);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get eighth value %d \n", source);
	}
	if (cmd->isOptionSet("-early")) {
		bool early = false;
		bool res = cmd->getOptionValue("-early", early);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get ninth value %d \n", early);
	}
	if (cmd->isOptionSet("-late")) {
		bool late = false;
		bool res = cmd->getOptionValue("-late", late);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get tenth value %d \n", late);
	}
	if (cmd->isOptionSet("-clock")) {
		StringVector clock_list;
		bool res = cmd->getOptionValue("-clock", clock_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &clock : clock_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get eleventh value %s \n", clock.c_str());
		}
	}
	
	return TCL_OK;
}
//07 set_sense
int parseSdcSetSense(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!(	cmd->isOptionSet("object_list") and 
			(cmd->isOptionSet("-positive") xor 
			 cmd->isOptionSet("-negative") xor 
			 cmd->isOptionSet("-pulse") xor 
			 cmd->isOptionSet("-stop_propagation") xor 
			 (cmd->isOptionSet("-non_unate") and cmd->isOptionSet("-clocks")))
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("-type")) {
		StringVector type_list;
		bool res = cmd->getOptionValue("-type", type_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &type : type_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get first value %s \n", type.c_str());
		}
	}
	if (cmd->isOptionSet("-non_unate")) {
		bool non_unate = false;
		bool res = cmd->getOptionValue("-non_unate", non_unate);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get second value %d \n", non_unate);
	}
	if (cmd->isOptionSet("-clocks")) {
		StringVector clocks_list;
		bool res = cmd->getOptionValue("-clocks", clocks_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &clocks : clocks_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get third value %s \n", clocks.c_str());
		}
	}
	if (cmd->isOptionSet("-positive")) {
		bool positive = false;
		bool res = cmd->getOptionValue("-positive", positive);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fourth value %d \n", positive);
	}
	if (cmd->isOptionSet("-negative")) {
		bool negative = false;
		bool res = cmd->getOptionValue("-negative", negative);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fifth value %d \n", negative);
	}
	if (cmd->isOptionSet("-clock_leaf")) {
		bool clock_leaf = false;
		bool res = cmd->getOptionValue("-clock_leaf", clock_leaf);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get sixth value %d \n", clock_leaf);
	}
	if (cmd->isOptionSet("-stop_propagation")) {
		bool stop_propagation = false;
		bool res = cmd->getOptionValue("-stop_propagation", stop_propagation);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get seventh value %d \n", stop_propagation);
	}
	if (cmd->isOptionSet("-pulse")) {
		StringVector pulse_list;
		bool res = cmd->getOptionValue("-pulse", pulse_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &pulse : pulse_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get eighth value %s \n", pulse.c_str());
		}
	}
	if (cmd->isOptionSet("object_list")) {
		StringVector object_list;
		bool res = cmd->getOptionValue("object_list", object_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &object : object_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get ninth value %s \n", object.c_str());
		}
	}
	return TCL_OK;
}
//08 set_clock_transition
int parseSdcSetClockTransition(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!(	cmd->isOptionSet("transition") and 
			cmd->isOptionSet("clock_list") and 
			(!(cmd->isOptionSet("-rise") and cmd->isOptionSet("-fall")))
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("transition")) {
		double transition = 0.0;
		bool res = cmd->getOptionValue("transition", transition);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get first value %f \n", transition);
	}
	if (cmd->isOptionSet("clock_list")) {
		StringVector clock_list;
		bool res = cmd->getOptionValue("clock_list", clock_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &clock : clock_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get second value %s \n", clock.c_str());
		}
	}
	if (cmd->isOptionSet("-rise")) {
		bool rise = false;
		bool res = cmd->getOptionValue("-rise", rise);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get third value %d \n", rise);
	}
	if (cmd->isOptionSet("-fall")) {
		bool fall = false;
		bool res = cmd->getOptionValue("-fall", fall);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fourth value %d \n", fall);
	}
	if (cmd->isOptionSet("-min")) {
		bool min = false;
		bool res = cmd->getOptionValue("-min", min);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fifth value %d \n", min);
	}
	if (cmd->isOptionSet("-max")) {
		bool max = false;
		bool res = cmd->getOptionValue("-max", max);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get sixth value %d \n", max);
	}
	return TCL_OK;
}
//09 set_clock_uncertainty
int parseSdcSetClockUncertainty(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!(	cmd->isOptionSet("uncertainty") and 
			cmd->isOptionSet("object_list") xor 
			((cmd->isOptionSet("-from") xor cmd->isOptionSet("-rise_from") xor cmd->isOptionSet("-fall_from")) and
			(cmd->isOptionSet("-to") xor cmd->isOptionSet("-rise_to") xor cmd->isOptionSet("-fall_to")))
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("uncertainty")) {
		double uncertainty = 0.0;
		bool res = cmd->getOptionValue("uncertainty", uncertainty);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get first value %f \n", uncertainty);
	}
	if (cmd->isOptionSet("-from")) {
		StringVector from_list;
		bool res = cmd->getOptionValue("-from", from_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &from : from_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get second value %s \n", from.c_str());
		}
	}
	if (cmd->isOptionSet("-to")) {
		StringVector to_list;
		bool res = cmd->getOptionValue("-to", to_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &to : to_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get third value %s \n", to.c_str());
		}
	}
	if (cmd->isOptionSet("-rise_to")) {
		StringVector rise_to_list;
		bool res = cmd->getOptionValue("-rise_to", rise_to_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &rise_to : rise_to_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get fourth value %s \n", rise_to.c_str());
		}
	}
	if (cmd->isOptionSet("-fall_to")) {
		StringVector fall_to_list;
		bool res = cmd->getOptionValue("-fall_to", fall_to_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &fall_to : fall_to_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get fifth value %s \n", fall_to.c_str());
		}
	}
	if (cmd->isOptionSet("-rise_from")) {
		StringVector rise_from_list;
		bool res = cmd->getOptionValue("-rise_from", rise_from_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &rise_from : rise_from_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get sixth value %s \n", rise_from.c_str());
		}
	}
	if (cmd->isOptionSet("-fall_from")) {
		StringVector fall_from_list;
		bool res = cmd->getOptionValue("-fall_from", fall_from_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &fall_from : fall_from_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get seventh value %s \n", fall_from.c_str());
		}
	}
	if (cmd->isOptionSet("-rise")) {
		bool rise = false;
		bool res = cmd->getOptionValue("-rise", rise);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get eighth value %d \n", rise);
	}
	if (cmd->isOptionSet("-fall")) {
		bool fall = false;
		bool res = cmd->getOptionValue("-fall", fall);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get ninth value %d \n", fall);
	}
	if (cmd->isOptionSet("-setup")) {
		bool setup = false;
		bool res = cmd->getOptionValue("-setup", setup);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get tenth value %d \n", setup);
	}
	if (cmd->isOptionSet("-hold")) {
		bool hold = false;
		bool res = cmd->getOptionValue("-hold", hold);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get eleventh value %d \n", hold);
	}
	if (cmd->isOptionSet("object_list")) {
		StringVector object_list;
		bool res = cmd->getOptionValue("object_list", object_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &object : object_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get twelfth value %s \n", object.c_str());
		}
	}
	
	return TCL_OK;
}
//10 set_data_check
int parseSdcSetDataCheck(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!(	(cmd->isOptionSet("-rise_from") or cmd->isOptionSet("-from") or cmd->isOptionSet("-fall_from")) and
			(cmd->isOptionSet("-to") or cmd->isOptionSet("-rise_to") or cmd->isOptionSet("-fall_to"))
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("-from")) {
		StringVector from_list;
		bool res = cmd->getOptionValue("-from", from_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &from : from_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get first value %s \n", from.c_str());
		}
	}
	if (cmd->isOptionSet("-rise_from")) {
		StringVector rise_from_list;
		bool res = cmd->getOptionValue("-rise_from", rise_from_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &rise_from : rise_from_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get second value %s \n", rise_from.c_str());
		}
	}
	if (cmd->isOptionSet("-fall_from")) {
		StringVector fall_from_list;
		bool res = cmd->getOptionValue("-fall_from", fall_from_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &fall_from : fall_from_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get third value %s \n", fall_from.c_str());
		}
	}
	if (cmd->isOptionSet("-to")) {
		StringVector to_list;
		bool res = cmd->getOptionValue("-to", to_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &to : to_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get fourth value %s \n", to.c_str());
		}
	}
	if (cmd->isOptionSet("-rise_to")) {
		StringVector rise_to_list;
		bool res = cmd->getOptionValue("-rise_to", rise_to_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &rise_to : rise_to_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get fifth value %s \n", rise_to.c_str());
		}
	}
	if (cmd->isOptionSet("-fall_to")) {
		StringVector fall_to_list;
		bool res = cmd->getOptionValue("-fall_to", fall_to_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &fall_to : fall_to_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get sixth value %s \n", fall_to.c_str());
		}
	}
	if (cmd->isOptionSet("-rise")) {
		bool rise = false;
		bool res = cmd->getOptionValue("-rise", rise);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get seventh value %d \n", rise);
	}
	if (cmd->isOptionSet("-fall")) {
		bool fall = false;
		bool res = cmd->getOptionValue("-fall", fall);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get eighth value %d \n", fall);
	}
	if (cmd->isOptionSet("-setup")) {
		bool setup = false;
		bool res = cmd->getOptionValue("-setup", setup);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get ninth value %d \n", setup);
	}
	if (cmd->isOptionSet("-hold")) {
		bool hold = false;
		bool res = cmd->getOptionValue("-hold", hold);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get tenth value %d \n", hold);
	}
	if (cmd->isOptionSet("-clock")) {
		StringVector clock_list;
		bool res = cmd->getOptionValue("-clock", clock_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &clock : clock_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get eleventh value %s \n", clock.c_str());
		}
	}
	if (cmd->isOptionSet("value")) {
		double value = 0.0;
		bool res = cmd->getOptionValue("value", value);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get twelfth value %f \n", value);
	}
	
	return TCL_OK;
}
//11 set_disable_timing
int parseSdcSetDisableTiming(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!( cmd->isOptionSet("object_list") and 
		   (!(cmd->isOptionSet("-to") xor cmd->isOptionSet("-from")))
	
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("object_list")) {
		StringVector object_list;
		bool res = cmd->getOptionValue("object_list", object_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &object : object_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get first value %s \n", object.c_str());
		}
	}
	if (cmd->isOptionSet("-from")) {
		std::string from = "";
		bool res = cmd->getOptionValue("-from", from);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get second value %s \n", from.c_str());
	}
	if (cmd->isOptionSet("-to")) {
		std::string to = "";
		bool res = cmd->getOptionValue("-to", to);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get third value %s \n", to.c_str());
	}
	return TCL_OK;
}
//12 set_false_path
int parseSdcSetFalsePath(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!( (cmd->isOptionSet("-from") or cmd->isOptionSet("-to") or cmd->isOptionSet("-through")) and
			(!(cmd->isOptionSet("-rise") and cmd->isOptionSet("-fall"))) and
			(!(cmd->isOptionSet("-setup") and cmd->isOptionSet("-hold")))
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("-setup")) {
		bool setup = false;
		bool res = cmd->getOptionValue("-setup", setup);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get first value %d \n", setup);
	}
	if (cmd->isOptionSet("-hold")) {
		bool hold = false;
		bool res = cmd->getOptionValue("-hold", hold);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get second value %d \n", hold);
	}
	if (cmd->isOptionSet("-rise")) {
		bool rise = false;
		bool res = cmd->getOptionValue("-rise", rise);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get third value %d \n", rise);
	}
	if (cmd->isOptionSet("-fall")) {
		bool fall = false;
		bool res = cmd->getOptionValue("-fall", fall);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fourth value %d \n", fall);
	}
	if (cmd->isOptionSet("-from")) {
		StringVector from_list;
		bool res = cmd->getOptionValue("-from", from_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &from : from_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get fifth value %s \n", from.c_str());
		}
	}
	if (cmd->isOptionSet("-rise_from")) {
		StringVector rise_from_list;
		bool res = cmd->getOptionValue("-rise_from", rise_from_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &rise_from : rise_from_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get sixth value %s \n", rise_from.c_str());
		}
	}
	if (cmd->isOptionSet("-fall_from")) {
		StringVector fall_from_list;
		bool res = cmd->getOptionValue("-fall_from", fall_from_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &fall_from : fall_from_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get seventh value %s \n", fall_from.c_str());
		}
	}
	if (cmd->isOptionSet("-to")) {
		StringVector to_list;
		bool res = cmd->getOptionValue("-to", to_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &to : to_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get eighth value %s \n", to.c_str());
		}
	}
	if (cmd->isOptionSet("-rise_to")) {
		StringVector rise_to_list;
		bool res = cmd->getOptionValue("-rise_to", rise_to_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &rise_to : rise_to_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get ninth value %s \n", rise_to.c_str());
		}
	}
	if (cmd->isOptionSet("-fall_to")) {
		StringVector fall_to_list;
		bool res = cmd->getOptionValue("-fall_to", fall_to_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &fall_to : fall_to_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get tenth value %s \n", fall_to.c_str());
		}
	}
	if (cmd->isOptionSet("-through")) {
		StringVector through_list;
		bool res = cmd->getOptionValue("-through", through_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &through : through_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get eleventh value %s \n", through.c_str());
		}
	}
	if (cmd->isOptionSet("-fall_throough")) {
		StringVector fall_throough_list;
		bool res = cmd->getOptionValue("-fall_throough", fall_throough_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &fall_throough : fall_throough_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get twelfth value %s \n", fall_throough.c_str());
		}
	}
	if (cmd->isOptionSet("-rise_throough")) {
		StringVector rise_throough_list;
		bool res = cmd->getOptionValue("-rise_throough", rise_throough_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &rise_throough : rise_throough_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get thirteenth value %s \n", rise_throough.c_str());
		}
	}
	if (cmd->isOptionSet("-comment")) {
		std::string comment = "";
		bool res = cmd->getOptionValue("-comment", comment);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fourteenth value %s \n", comment.c_str());
	}
	
	return TCL_OK;
}
//13 set_ideal_latency
int parseSdcSetIdealLatency(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!( cmd->isOptionSet("value") and cmd->isOptionSet("object_list")
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("value")) {
		double value = 0.0;
		bool res = cmd->getOptionValue("value", value);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get first value %f \n", value);
	}
	if (cmd->isOptionSet("object_list")) {
		StringVector object_list;
		bool res = cmd->getOptionValue("object_list", object_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &object : object_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get second value %s \n", object.c_str());
		}
	}
	if (cmd->isOptionSet("-rise")) {
		bool rise = false;
		bool res = cmd->getOptionValue("-rise", rise);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get third value %d \n", rise);
	}
	if (cmd->isOptionSet("-fall")) {
		bool fall = false;
		bool res = cmd->getOptionValue("-fall", fall);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fourth value %d \n", fall);
	}
	if (cmd->isOptionSet("-min")) {
		bool min = false;
		bool res = cmd->getOptionValue("-min", min);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fifth value %d \n", min);
	}
	if (cmd->isOptionSet("-max")) {
		bool max = false;
		bool res = cmd->getOptionValue("-max", max);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get sixth value %d \n", max);
	}
	return TCL_OK;
}
//14 set_ideal_network
int parseSdcSetIdealNetwork(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!( cmd->isOptionSet("object_list")
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("object_list")) {
		StringVector object_list;
		bool res = cmd->getOptionValue("object_list", object_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &object : object_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get first value %s \n", object.c_str());
		}
	}
	if (cmd->isOptionSet("-no_propagate")) {
		bool no_propagate = false;
		bool res = cmd->getOptionValue("-no_propagate", no_propagate);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get second value %d \n", no_propagate);
	}
	return TCL_OK;
}
//15 set_ideal_transition
int parseSdcSetIdealTransition(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!( cmd->isOptionSet("value") and cmd->isOptionSet("object_list")
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("value")) {
		double value = 0.0;
		bool res = cmd->getOptionValue("value", value);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get first value %f \n", value);
	}
	if (cmd->isOptionSet("object_list")) {
		StringVector object_list;
		bool res = cmd->getOptionValue("object_list", object_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &object : object_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get second value %s \n", object.c_str());
		}
	}
	if (cmd->isOptionSet("-rise")) {
		bool rise = false;
		bool res = cmd->getOptionValue("-rise", rise);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get third value %d \n", rise);
	}
	if (cmd->isOptionSet("-fall")) {
		bool fall = false;
		bool res = cmd->getOptionValue("-fall", fall);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fourth value %d \n", fall);
	}
	if (cmd->isOptionSet("-min")) {
		bool min = false;
		bool res = cmd->getOptionValue("-min", min);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fifth value %d \n", min);
	}
	if (cmd->isOptionSet("-max")) {
		bool max = false;
		bool res = cmd->getOptionValue("-max", max);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get sixth value %d \n", max);
	}
	
	return TCL_OK;
}
//16 set_input_delay
int parseSdcSetInputDelay(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!( cmd->isOptionSet("delay_value") and 
		   cmd->isOptionSet("port_pin_list") and
		   (!(
		   (cmd->isOptionSet("-clock_fall") or cmd->isOptionSet("-level_sensitive")) and
		   (!(cmd->isOptionSet("-clock")))
		   ))
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("-clock")) {
		StringVector clock_list;
		bool res = cmd->getOptionValue("-clock", clock_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &clock : clock_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get first value %s \n", clock.c_str());
		}
	}
	if (cmd->isOptionSet("-clock_fall")) {
		bool clock_fall = false;
		bool res = cmd->getOptionValue("-clock_fall", clock_fall);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get second value %d \n", clock_fall);
	}
	if (cmd->isOptionSet("-level_sensitive")) {
		bool level_sensitive = false;
		bool res = cmd->getOptionValue("-level_sensitive", level_sensitive);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get third value %d \n", level_sensitive);
	}
	if (cmd->isOptionSet("-rise")) {
		bool rise = false;
		bool res = cmd->getOptionValue("-rise", rise);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fourth value %d \n", rise);
	}
	if (cmd->isOptionSet("-fall")) {
		bool fall = false;
		bool res = cmd->getOptionValue("-fall", fall);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fifth value %d \n", fall);
	}
	if (cmd->isOptionSet("-min")) {
		bool min = false;
		bool res = cmd->getOptionValue("-min", min);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get sixth value %d \n", min);
	}
	if (cmd->isOptionSet("-max")) {
		bool max = false;
		bool res = cmd->getOptionValue("-max", max);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get seventh value %d \n", max);
	}
	if (cmd->isOptionSet("-add_delay")) {
		bool add_delay = false;
		bool res = cmd->getOptionValue("-add_delay", add_delay);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get eighth value %d \n", add_delay);
	}
	if (cmd->isOptionSet("delay_value")) {
		double delay_value = 0.0;
		bool res = cmd->getOptionValue("delay_value", delay_value);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get ninth value %f \n", delay_value);
	}
	if (cmd->isOptionSet("-reference_pin")) {
		StringVector reference_pin_list;
		bool res = cmd->getOptionValue("-reference_pin", reference_pin_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &reference_pin : reference_pin_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get tenth value %s \n", reference_pin.c_str());
		}
	}
	if (cmd->isOptionSet("port_pin_list")) {
		StringVector port_pin_list;
		bool res = cmd->getOptionValue("port_pin_list", port_pin_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &port_pin : port_pin_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get eleventh value %s \n", port_pin.c_str());
		}
	}
	if (cmd->isOptionSet("-network_latency_included")) {
		bool network_latency_included = false;
		bool res = cmd->getOptionValue("-network_latency_included", network_latency_included);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get twelfth value %d \n", network_latency_included);
	}
	if (cmd->isOptionSet("-source_latency_included")) {
		bool source_latency_included = false;
		bool res = cmd->getOptionValue("-source_latency_included", source_latency_included);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get thirteenth value %d \n", source_latency_included);
	}
	return TCL_OK;
}
//17 set_max_delay
int parseSdcSetMaxDelay(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!( cmd->isOptionSet("delay_value") and 
		   (!(cmd->isOptionSet("-rise") and cmd->isOptionSet("-fall")))
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("delay_value")) {
		double delay_value = 0.0;
		bool res = cmd->getOptionValue("delay_value", delay_value);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get first value %f \n", delay_value);
	}
	if (cmd->isOptionSet("-rise")) {
		bool rise = false;
		bool res = cmd->getOptionValue("-rise", rise);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get second value %d \n", rise);
	}
	if (cmd->isOptionSet("-fall")) {
		bool fall = false;
		bool res = cmd->getOptionValue("-fall", fall);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get third value %d \n", fall);
	}
	if (cmd->isOptionSet("-from")) {
		StringVector from_list;
		bool res = cmd->getOptionValue("-from", from_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &from : from_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get fourth value %s \n", from.c_str());
		}
	}
	if (cmd->isOptionSet("-rise_from")) {
		StringVector rise_from_list;
		bool res = cmd->getOptionValue("-rise_from", rise_from_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &rise_from : rise_from_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get fifth value %s \n", rise_from.c_str());
		}
	}
	if (cmd->isOptionSet("-fall_from")) {
		StringVector fall_from_list;
		bool res = cmd->getOptionValue("-fall_from", fall_from_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &fall_from : fall_from_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get sixth value %s \n", fall_from.c_str());
		}
	}
	if (cmd->isOptionSet("-to")) {
		StringVector to_list;
		bool res = cmd->getOptionValue("-to", to_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &to : to_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get seventh value %s \n", to.c_str());
		}
	}
	if (cmd->isOptionSet("-rise_to")) {
		StringVector rise_to_list;
		bool res = cmd->getOptionValue("-rise_to", rise_to_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &rise_to : rise_to_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get eighth value %s \n", rise_to.c_str());
		}
	}
	if (cmd->isOptionSet("-fall_to")) {
		StringVector fall_to_list;
		bool res = cmd->getOptionValue("-fall_to", fall_to_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &fall_to : fall_to_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get ninth value %s \n", fall_to.c_str());
		}
	}
	if (cmd->isOptionSet("-through")) {
		StringVector through_list;
		bool res = cmd->getOptionValue("-through", through_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &through : through_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get tenth value %s \n", through.c_str());
		}
	}
	if (cmd->isOptionSet("-fall_through")) {
		StringVector fall_through_list;
		bool res = cmd->getOptionValue("-fall_through", fall_through_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &fall_through : fall_through_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get eleventh value %s \n", fall_through.c_str());
		}
	}
	if (cmd->isOptionSet("-rise_through")) {
		StringVector rise_through_list;
		bool res = cmd->getOptionValue("-rise_through", rise_through_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &rise_through : rise_through_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get twelfth value %s \n", rise_through.c_str());
		}
	}
	if (cmd->isOptionSet("-ignore_clock_latency")) {
		bool ignore_clock_latency = false;
		bool res = cmd->getOptionValue("-ignore_clock_latency", ignore_clock_latency);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get thirteenth value %d \n", ignore_clock_latency);
	}
	if (cmd->isOptionSet("-comment")) {
		std::string comment = "";
		bool res = cmd->getOptionValue("-comment", comment);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fourteenth value %s \n", comment.c_str());
	}
	
	return TCL_OK;
}
//18 set_max_time_borrow
int parseSdcSetMaxTimeBorrow(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!( cmd->isOptionSet("delay_value") and 
		   cmd->isOptionSet("object_list")
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("delay_value")) {
		double delay_value = 0.0;
		bool res = cmd->getOptionValue("delay_value", delay_value);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get first value %f \n", delay_value);
	}
	if (cmd->isOptionSet("object_list")) {
		StringVector object_list;
		bool res = cmd->getOptionValue("object_list", object_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &object : object_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get second value %s \n", object.c_str());
		}
	}
	return TCL_OK;
}
//19 set_min_delay
int parseSdcSetMinDelay(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!( cmd->isOptionSet("delay_value") and 
		   (!(cmd->isOptionSet("-rise") and cmd->isOptionSet("-fall")))
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("delay_value")) {
		double delay_value = 0.0;
		bool res = cmd->getOptionValue("delay_value", delay_value);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get first value %f \n", delay_value);
	}
	if (cmd->isOptionSet("-rise")) {
		bool rise = false;
		bool res = cmd->getOptionValue("-rise", rise);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get second value %d \n", rise);
	}
	if (cmd->isOptionSet("-fall")) {
		bool fall = false;
		bool res = cmd->getOptionValue("-fall", fall);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get third value %d \n", fall);
	}
	if (cmd->isOptionSet("-from")) {
		StringVector from_list;
		bool res = cmd->getOptionValue("-from", from_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &from : from_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get fourth value %s \n", from.c_str());
		}
	}
	if (cmd->isOptionSet("-rise_from")) {
		StringVector rise_from_list;
		bool res = cmd->getOptionValue("-rise_from", rise_from_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &rise_from : rise_from_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get fifth value %s \n", rise_from.c_str());
		}
	}
	if (cmd->isOptionSet("-fall_from")) {
		StringVector fall_from_list;
		bool res = cmd->getOptionValue("-fall_from", fall_from_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &fall_from : fall_from_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get sixth value %s \n", fall_from.c_str());
		}
	}
	if (cmd->isOptionSet("-to")) {
		StringVector to_list;
		bool res = cmd->getOptionValue("-to", to_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &to : to_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get seventh value %s \n", to.c_str());
		}
	}
	if (cmd->isOptionSet("-rise_to")) {
		StringVector rise_to_list;
		bool res = cmd->getOptionValue("-rise_to", rise_to_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &rise_to : rise_to_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get eighth value %s \n", rise_to.c_str());
		}
	}
	if (cmd->isOptionSet("-fall_to")) {
		StringVector fall_to_list;
		bool res = cmd->getOptionValue("-fall_to", fall_to_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &fall_to : fall_to_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get ninth value %s \n", fall_to.c_str());
		}
	}
	if (cmd->isOptionSet("-through")) {
		StringVector through_list;
		bool res = cmd->getOptionValue("-through", through_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &through : through_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get tenth value %s \n", through.c_str());
		}
	}
	if (cmd->isOptionSet("-fall_through")) {
		StringVector fall_through_list;
		bool res = cmd->getOptionValue("-fall_through", fall_through_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &fall_through : fall_through_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get eleventh value %s \n", fall_through.c_str());
		}
	}
	if (cmd->isOptionSet("-rise_through")) {
		StringVector rise_through_list;
		bool res = cmd->getOptionValue("-rise_through", rise_through_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &rise_through : rise_through_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get twelfth value %s \n", rise_through.c_str());
		}
	}
	if (cmd->isOptionSet("-ignore_clock_latency")) {
		bool ignore_clock_latency = false;
		bool res = cmd->getOptionValue("-ignore_clock_latency", ignore_clock_latency);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get thirteenth value %d \n", ignore_clock_latency);
	}
	if (cmd->isOptionSet("-comment")) {
		std::string comment = "";
		bool res = cmd->getOptionValue("-comment", comment);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fourteenth value %s \n", comment.c_str());
	}
	return TCL_OK;
}
//20 set_min_pulse_width
int parseSdcSetMinPulseWidth(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!( cmd->isOptionSet("value") 
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("value")) {
		double value = 0.0;
		bool res = cmd->getOptionValue("value", value);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get first value %f \n", value);
	}
	if (cmd->isOptionSet("object_list")) {
		StringVector object_list;
		bool res = cmd->getOptionValue("object_list", object_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &object : object_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get second value %s \n", object.c_str());
		}
	}
	if (cmd->isOptionSet("-low")) {
		bool low = false;
		bool res = cmd->getOptionValue("-low", low);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get third value %d \n", low);
	}
	if (cmd->isOptionSet("-high")) {
		bool high = false;
		bool res = cmd->getOptionValue("-high", high);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fourth value %d \n", high);
	}
	return TCL_OK;
}
//21 set_multicycle_path
int parseSdcSetMulticyclePath(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!( cmd->isOptionSet("path_multiplier") 
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("path_multiplier")) {
		int path_multiplier = 0;
		bool res = cmd->getOptionValue("path_multiplier", path_multiplier);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get first value %d \n", path_multiplier);
	}
	if (cmd->isOptionSet("-setup")) {
		bool setup = false;
		bool res = cmd->getOptionValue("-setup", setup);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get second value %d \n", setup);
	}
	if (cmd->isOptionSet("-hold")) {
		bool hold = false;
		bool res = cmd->getOptionValue("-hold", hold);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get third value %d \n", hold);
	}
	if (cmd->isOptionSet("-rise")) {
		bool rise = false;
		bool res = cmd->getOptionValue("-rise", rise);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fourth value %d \n", rise);
	}
	if (cmd->isOptionSet("-fall")) {
		bool fall = false;
		bool res = cmd->getOptionValue("-fall", fall);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fifth value %d \n", fall);
	}
	if (cmd->isOptionSet("-start")) {
		bool start = false;
		bool res = cmd->getOptionValue("-start", start);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get sixth value %d \n", start);
	}
	if (cmd->isOptionSet("-end")) {
		bool end = false;
		bool res = cmd->getOptionValue("-end", end);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get seventh value %d \n", end);
	}
	if (cmd->isOptionSet("-from")) {
		StringVector from_list;
		bool res = cmd->getOptionValue("-from", from_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &from : from_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get eighth value %s \n", from.c_str());
		}
	}
	if (cmd->isOptionSet("-to")) {
		StringVector to_list;
		bool res = cmd->getOptionValue("-to", to_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &to : to_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get ninth value %s \n", to.c_str());
		}
	}
	if (cmd->isOptionSet("-rise_to")) {
		StringVector rise_to_list;
		bool res = cmd->getOptionValue("-rise_to", rise_to_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &rise_to : rise_to_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get tenth value %s \n", rise_to.c_str());
		}
	}
	if (cmd->isOptionSet("-fall_to")) {
		StringVector fall_to_list;
		bool res = cmd->getOptionValue("-fall_to", fall_to_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &fall_to : fall_to_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get eleventh value %s \n", fall_to.c_str());
		}
	}
	if (cmd->isOptionSet("-rise_from")) {
		StringVector rise_from_list;
		bool res = cmd->getOptionValue("-rise_from", rise_from_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &rise_from : rise_from_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get twelfth value %s \n", rise_from.c_str());
		}
	}
	if (cmd->isOptionSet("-fall_from")) {
		StringVector fall_from_list;
		bool res = cmd->getOptionValue("-fall_from", fall_from_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &fall_from : fall_from_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get thirteenth value %s \n", fall_from.c_str());
		}
	}
	if (cmd->isOptionSet("-rise_through")) {
		StringVector rise_through_list;
		bool res = cmd->getOptionValue("-rise_through", rise_through_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &rise_through : rise_through_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get fourteenth value %s \n", rise_through.c_str());
		}
	}
	if (cmd->isOptionSet("-fall_through")) {
		StringVector fall_through_list;
		bool res = cmd->getOptionValue("-fall_through", fall_through_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &fall_through : fall_through_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get fifteenth value %s \n", fall_through.c_str());
		}
	}
	if (cmd->isOptionSet("-through")) {
		StringVector through_list;
		bool res = cmd->getOptionValue("-through", through_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &through : through_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get sixteenth value %s \n", through.c_str());
		}
	}
	if (cmd->isOptionSet("-comment")) {
		std::string comment = "";
		bool res = cmd->getOptionValue("-comment", comment);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get seventeenth value %s \n", comment.c_str());
	}
	
	return TCL_OK;
}
//22 set_output_delay
int parseSdcSetOutputDelay(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!( cmd->isOptionSet("delay_value") and
		   cmd->isOptionSet("port_pin_list") and 
		   (!(
		   (cmd->isOptionSet("-clock_fall") or
		   cmd->isOptionSet("-level_sensitive")) and 
		   (!(cmd->isOptionSet("-clock")))
		   ))
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("-clock")) {
		StringVector clock_list;
		bool res = cmd->getOptionValue("-clock", clock_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &clock : clock_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get first value %s \n", clock.c_str());
		}
	}
	if (cmd->isOptionSet("-clock_fall")) {
		bool clock_fall = false;
		bool res = cmd->getOptionValue("-clock_fall", clock_fall);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get second value %d \n", clock_fall);
	}
	if (cmd->isOptionSet("-level_sensitive")) {
		bool level_sensitive = false;
		bool res = cmd->getOptionValue("-level_sensitive", level_sensitive);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get third value %d \n", level_sensitive);
	}
	if (cmd->isOptionSet("-rise")) {
		bool rise = false;
		bool res = cmd->getOptionValue("-rise", rise);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fourth value %d \n", rise);
	}
	if (cmd->isOptionSet("-fall")) {
		bool fall = false;
		bool res = cmd->getOptionValue("-fall", fall);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get fifth value %d \n", fall);
	}
	if (cmd->isOptionSet("-min")) {
		bool min = false;
		bool res = cmd->getOptionValue("-min", min);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get sixth value %d \n", min);
	}
	if (cmd->isOptionSet("-max")) {
		bool max = false;
		bool res = cmd->getOptionValue("-max", max);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get seventh value %d \n", max);
	}
	if (cmd->isOptionSet("-add_delay")) {
		bool add_delay = false;
		bool res = cmd->getOptionValue("-add_delay", add_delay);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get eighth value %d \n", add_delay);
	}
	if (cmd->isOptionSet("delay_value")) {
		double delay_value = 0.0;
		bool res = cmd->getOptionValue("delay_value", delay_value);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get ninth value %f \n", delay_value);
	}
	if (cmd->isOptionSet("-reference_pin")) {
		StringVector reference_pin_list;
		bool res = cmd->getOptionValue("-reference_pin", reference_pin_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &reference_pin : reference_pin_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get tenth value %s \n", reference_pin.c_str());
		}
	}
	if (cmd->isOptionSet("port_pin_list")) {
		StringVector port_pin_list;
		bool res = cmd->getOptionValue("port_pin_list", port_pin_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &port_pin : port_pin_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get eleventh value %s \n", port_pin.c_str());
		}
	}
	if (cmd->isOptionSet("-network_latency_included")) {
		bool network_latency_included = false;
		bool res = cmd->getOptionValue("-network_latency_included", network_latency_included);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get twelfth value %d \n", network_latency_included);
	}
	if (cmd->isOptionSet("-source_latency_included")) {
		bool source_latency_included = false;
		bool res = cmd->getOptionValue("-source_latency_included", source_latency_included);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		//Assignment
		message->info("get thirteenth value %d \n", source_latency_included);
	}
	
	return TCL_OK;
}
//23 set_propagated_clock
int parseSdcSetPropagatedClock(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
	Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
	
	//constraint
	if (!( cmd->isOptionSet("object_list")
	)) {
		return TCL_ERROR;
	}
	
	if (cmd->isOptionSet("object_list")) {
		StringVector object_list;
		bool res = cmd->getOptionValue("object_list", object_list);
		if (!res) {
			//TODO messages
			return TCL_ERROR;
		}
		for (const auto &object : object_list) {
			//TODO DB team did not implement the API to get pin/term from name
			//Assignment
			message->info("get first value %s \n", object.c_str());
		}
	}
	
	return TCL_OK;
}

// environment commands manager
int parseSdcSetCaseAnalysis(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    SdcPtr sdc = getSdc();
    assert(sdc);
    if (!sdc) {
        //TODO messages
        return TCL_ERROR;
    }
    auto case_analysis_container = sdc->getCaseAnalysisContainer();
    if (!case_analysis_container) {
        //TODO messages
        return TCL_ERROR;
    }
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    SetCaseAnalysis case_analysis;
    if (cmd->isOptionSet("value")) {
        std::string value;
        bool res = cmd->getOptionValue("value", value);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        case_analysis.setValue(value);
        message->info("get first value %s \n", value.c_str());
    }
    if (cmd->isOptionSet("port_or_pin_list")) {
        StringVectorPtr port_or_pin_list = std::make_shared<StringVector>();
        assert(port_or_pin_list);
        bool res = cmd->getOptionValue("port_or_pin_list", (StringVector**)(&port_or_pin_list));
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        Cell* topCell = getTopCell();
        assert(topCell);
        if (topCell == nullptr) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& pin_name : *port_or_pin_list) {
            ObjectId pin_id = 1; 
            //Pin *pin = topCell->getPin(pin_name); 
            //if (pin == nullptr) {
            //    //TODO messages
            //    return TCL_ERROR;
            //}
            //ObjectId pin_id = pin->getId();
            case_analysis_container->add(pin_id, case_analysis);
            message->info("get second value %s \n", pin_name.c_str());
        }
    }
    return TCL_OK;
}

int parseSdcSetDrive(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("resistance") and cmd->isOptionSet("port_list"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("resistance")) {
        double resistance = 0.0;
        bool res = cmd->getOptionValue("resistance", resistance);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %f \n", resistance);
    }
    if (cmd->isOptionSet("port_list")) {
        StringVector port_list;
        bool res = cmd->getOptionValue("port_list", port_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &port : port_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get seccond value %s \n", port.c_str());
        }
    }
    if (cmd->isOptionSet("-rise")) {
        bool rise = false;
        bool res = cmd->getOptionValue("-rise", rise);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get third value %d \n", rise);
    }
    if (cmd->isOptionSet("-fall")) {
        bool fall = false;
        bool res = cmd->getOptionValue("-fall", fall);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get fourth value %d \n", fall);
    }
    if (cmd->isOptionSet("-min")) {
        bool min = false;
        bool res = cmd->getOptionValue("-min", min);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get fifth value %d \n", min);
    }
    if (cmd->isOptionSet("-max")) {
        bool max = false;
        bool res = cmd->getOptionValue("-max", max);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get sixth value %d \n", max);
    }
    return TCL_OK;
}
int parseSdcSetDrivingCell(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("port_list") and cmd->isOptionSet("-lib_cell"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("-lib_cell")) {
        std::string lib_cell = "";
        bool res = cmd->getOptionValue("-lib_cell", lib_cell);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %s \n", lib_cell.c_str());
    }
    if (cmd->isOptionSet("-rise")) {
        bool rise = false;
        bool res = cmd->getOptionValue("-rise", rise);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
            //Assignment
        message->info("get seccond value %d \n",rise);
        }
    if (cmd->isOptionSet("-fall")) {
        bool fall = false;
        bool res = cmd->getOptionValue("-fall", fall);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get third value %d \n", fall);
    }
    if (cmd->isOptionSet("-library")) {
        std::string library = "";
        bool res = cmd->getOptionValue("-library", library);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get fourth value %s \n", library.c_str());
    }
    if (cmd->isOptionSet("-pin")) {
        std::string pin = "";
        bool res = cmd->getOptionValue("-pin", pin);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get fifth value %s \n", pin.c_str());
    }
    if (cmd->isOptionSet("-from_pin")) {
        std::string from_pin = "";
        bool res = cmd->getOptionValue("-from_pin", from_pin);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get sixth value %s \n", from_pin.c_str());
    }
    if (cmd->isOptionSet("-dont_scale")) {
        bool dont_scale = false;
        bool res = cmd->getOptionValue("-dont_scale", dont_scale);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get seventh value %d \n", dont_scale);
    }
    if (cmd->isOptionSet("-no_design_rule")) {
        bool no_design_rule = false;
        bool res = cmd->getOptionValue("-no_design_rule", no_design_rule);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get eighth value %d \n", no_design_rule);
    }
    if (cmd->isOptionSet("-input_transition_rise")) {
        double input_transition_rise = 0.0;
        bool res = cmd->getOptionValue("-input_transition_rise", input_transition_rise);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get ninth value %f \n", input_transition_rise);
    }
    if (cmd->isOptionSet("-input_transition_fall")) {
        double input_transition_fall = 0.0;
        bool res = cmd->getOptionValue("-input_transition_fall", input_transition_fall);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get tenth value %f \n", input_transition_fall);
    }
    if (cmd->isOptionSet("port_list")) {
        StringVector port_list;
        bool res = cmd->getOptionValue("port_list", port_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &port : port_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get eleventh value %s \n", port.c_str());
        }
    }
    if (cmd->isOptionSet("-min")) {
        bool min = false;
        bool res = cmd->getOptionValue("-min", min);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get twelfth value %d \n", min);
    }
    if (cmd->isOptionSet("-max")) {
        bool max = false;
        bool res = cmd->getOptionValue("-max", max);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get thirteenth value %d \n", max);
    }
    if (cmd->isOptionSet("-clock")) {
        StringVector clock_list;
        bool res = cmd->getOptionValue("-clock", clock_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &clock : clock_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get fourteenth value %s \n", clock.c_str());
        }
    }
    if (cmd->isOptionSet("-clock_fall")) {
        bool clock_fall = false;
        bool res = cmd->getOptionValue("-clock_fall", clock_fall);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get fifteenth value %d \n", clock_fall);
    }
    return TCL_OK;
}
int parseSdcSetFanoutLoad(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("value") and cmd->isOptionSet("port_list"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("value")) {
        double value = 0.0;
        bool res = cmd->getOptionValue("value", value);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %f \n", value);
    }
    if (cmd->isOptionSet("port_list")) {
        StringVector port_list;
        bool res = cmd->getOptionValue("port_list", port_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto port : port_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get seccond value %s \n", port.c_str());
        }
    }
    return TCL_OK;
}
int parseSdcSetInputTransition(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("transition") and cmd->isOptionSet("port_list"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("-rise")) {
        bool rise = false;
        bool res = cmd->getOptionValue("-rise", rise);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %d \n", rise);
    }
    if (cmd->isOptionSet("-fall")) {
        bool fall = false;
        bool res = cmd->getOptionValue("-fall", fall);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get seccond value %d \n", fall);
    }
    if (cmd->isOptionSet("-min")) {
        bool min = false;
        bool res = cmd->getOptionValue("-min", min);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get third value %d \n", min);
    }
    if (cmd->isOptionSet("-max")) {
        bool max = false;
        bool res = cmd->getOptionValue("-max", max);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get fourth value %d \n", max);
    }
    if (cmd->isOptionSet("transition")) {
        double transition = 0.0;
        bool res = cmd->getOptionValue("transition", transition);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get fifth value %f \n", transition);
    }
    if (cmd->isOptionSet("port_list")) {
        StringVector port_list;
        bool res = cmd->getOptionValue("port_list", port_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &port : port_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get sixth value %s \n", port.c_str());
        }
    }
    if (cmd->isOptionSet("-clock")) {
        StringVector clock_list;
        bool res = cmd->getOptionValue("-clock", clock_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &clock : clock_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get seventh value %s \n", clock.c_str());
        }
    }
    if (cmd->isOptionSet("-clock_fall")) {
        bool clock_fall = false;
        bool res = cmd->getOptionValue("-clock_fall", clock_fall);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get eighth value %d \n", clock_fall);
    }
    return TCL_OK;
}
int parseSdcSetLoad(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("value") and cmd->isOptionSet("objects"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("-min")) {
        bool min = false;
        bool res = cmd->getOptionValue("-min", min);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %d \n", min);
    }
    if (cmd->isOptionSet("-max")) {
        bool max = false;
        bool res = cmd->getOptionValue("-max", max);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get seccond value %d \n", max);
    }
    if (cmd->isOptionSet("-substract_pin_load")) {
        bool substract_pin_load = false;
        bool res = cmd->getOptionValue("-substract_pin_load", substract_pin_load);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get third value %d \n", substract_pin_load);
    }
    if (cmd->isOptionSet("-pin_load")) {
        bool pin_load = false;
        bool res = cmd->getOptionValue("-pin_load", pin_load);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get fourth value %d \n", pin_load);
    }
    if (cmd->isOptionSet("-wire_load")) {
        bool wire_load = false;
        bool res = cmd->getOptionValue("-wire_load", wire_load);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get fifth value %d \n", wire_load);
    }
    if (cmd->isOptionSet("value")) {
        double value = 0.0;
        bool res = cmd->getOptionValue("value", value);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get sixth value %f \n", value);
    }
    if (cmd->isOptionSet("objects")) {
        StringVector objects_list;
        bool res = cmd->getOptionValue("objects", objects_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &objects : objects_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get seventh value %s \n", objects.c_str());
        }
    }
    return TCL_OK;
}
int parseSdcSetLogicDc(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("port_list"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("port_list")) {
        StringVector port_list;
        bool res = cmd->getOptionValue("port_list", port_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& port : port_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get first value %s \n", port.c_str());
        }
    }
   
    return TCL_OK;
}
int parseSdcSetLogicOne(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("port_list"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("port_list")) {
        StringVector port_list;
        bool res = cmd->getOptionValue("port_list", port_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& port : port_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get first value %s \n", port.c_str());
        }
    }

    return TCL_OK;
}
int parseSdcSetLogicZero(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("port_list"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("port_list")) {
        StringVector port_list;
        bool res = cmd->getOptionValue("port_list", port_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& port : port_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get first value %s \n", port.c_str());
        }
    }

    return TCL_OK;
}
int parseSdcSetMaxArea(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("area_value"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("area_value")) {
        double area_value = 0.0;
        bool res = cmd->getOptionValue("area_value", area_value);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %f \n", area_value);
    }

    return TCL_OK;
}
int parseSdcSetMaxCapacitance(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("capacitance_value") and cmd->isOptionSet("object_list"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("capacitance_value")) {
        double capacitance_value = 0.0;
        bool res = cmd->getOptionValue("capacitance_value", capacitance_value);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %f \n", capacitance_value);
    }
    if (cmd->isOptionSet("object_list")) {
        StringVector object_list;
        bool res = cmd->getOptionValue("object_list", object_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &object : object_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get seccond value %s \n", object.c_str());
        }
    }
    return TCL_OK;
}
int parseSdcSetMaxFanout(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("fanout_value") and cmd->isOptionSet("object_list"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("fanout_value")) {
        double fanout_value = 0.0;
        bool res = cmd->getOptionValue("fanout_value", fanout_value);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %f \n", fanout_value);
    }
    if (cmd->isOptionSet("object_list")) {
        StringVector object_list;
        bool res = cmd->getOptionValue("object_list", object_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &object : object_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get seccond value %s \n", object.c_str());
        }
    }
    return TCL_OK;
}
int parseSdcSetMaxTransition(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("transition_value") and cmd->isOptionSet("object_list"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("transition_value")) {
        double transition_value = 0.0;
        bool res = cmd->getOptionValue("transition_value", transition_value);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %f \n", transition_value);
    }
    if (cmd->isOptionSet("-clock_path")) {
        bool clock_path = false;
        bool res = cmd->getOptionValue("-clock_path", clock_path);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get seccond value %d \n", clock_path);
    }
    if (cmd->isOptionSet("-fall")) {
        bool fall = false;
        bool res = cmd->getOptionValue("-fall", fall);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get third value %d \n", fall);
    }
    if (cmd->isOptionSet("-rise")) {
        bool rise = false;
        bool res = cmd->getOptionValue("-rise", rise);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get fourth value %d \n", rise);
    }
    if (cmd->isOptionSet("object_list")) {
        StringVector object_list;
        bool res = cmd->getOptionValue("object_list", object_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &object : object_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get fifth value %s \n", object.c_str());
        }
    }
    return TCL_OK;
}
int parseSdcSetMinCapacitance(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("capacitance_value") and cmd->isOptionSet("object_list"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("capacitance_value")) {
        double capacitance_value = 0.0;
        bool res = cmd->getOptionValue("capacitance_value", capacitance_value);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %f \n", capacitance_value);
    }
    if (cmd->isOptionSet("object_list")) {
        StringVector object_list;
        bool res = cmd->getOptionValue("object_list", object_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &object : object_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get seccond value %s \n", object.c_str());
        }
    }
    return TCL_OK;
}
int parseSdcSetOperatingConditions(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("-analysis_type"))) {
        std::string analysis_type = "";
        bool res = cmd->getOptionValue("-analysis_type", analysis_type);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %s \n", analysis_type.c_str());
    }
    if (cmd->isOptionSet("library")) {
        StringVector library_list;
        bool res = cmd->getOptionValue("library", library_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &library : library_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get seccond value %s \n", library.c_str());
        }
    }
    if (cmd->isOptionSet("-max")) {
        std::string max = "";
        bool res = cmd->getOptionValue("-max", max);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get third value %s \n", max.c_str());
    }
    if (cmd->isOptionSet("-min")) {
        std::string min = "";
        bool res = cmd->getOptionValue("-min", min);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get fourth value %s \n", min.c_str());
    }
    if (cmd->isOptionSet("-max_library")) {
        StringVector max_library_list;
        bool res = cmd->getOptionValue("-max_library", max_library_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &max_library : max_library_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get fifth value %s \n", max_library.c_str());
        }
    }
    if (cmd->isOptionSet("-object_list")) {
        StringVector object_list;
        bool res = cmd->getOptionValue("-object_list", object_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto & object : object_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get sixth value %s \n", object.c_str());
        }
    }
    if (cmd->isOptionSet("-min_library")) {
        StringVector min_library_list;
        bool res = cmd->getOptionValue("-min_library", min_library_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &min_library : min_library_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get seventh value %s \n", min_library.c_str());
        }
    }
    if (cmd->isOptionSet("condition")) {
        std::string condition = "";
        bool res = cmd->getOptionValue("condition", condition);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get eighth value %s \n", condition.c_str());
    }
   
    return TCL_OK;
}
int parseSdcSetPortFanoutNumber(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("fanout_number") and cmd->isOptionSet("port_list"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("fanout_number")) {
        int fanout_number = 0;
        bool res = cmd->getOptionValue("fanout_number", fanout_number);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %d \n", fanout_number);
    }
    if (cmd->isOptionSet("port_list")) {
        StringVector port_list;
        bool res = cmd->getOptionValue("port_list", port_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &port : port_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get seccond value %s \n", port.c_str());
        }
    }
    return TCL_OK;
}
int parseSdcSetResistance(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("value") and cmd->isOptionSet("net_list"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("value")) {
        double value = 0.0;
        bool res = cmd->getOptionValue("value", value);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %f \n", value);
    }
    if (cmd->isOptionSet("net_list")) {
        StringVector net_list;
        bool res = cmd->getOptionValue("net_list", net_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &net : net_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get seccond value %s \n", net.c_str());
        }
    } 
    if (cmd->isOptionSet("-min")) {
        bool min = false;
        bool res = cmd->getOptionValue("-min", min);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get third value %d \n", min);
    }
    if (cmd->isOptionSet("-max")) {
        bool max = false;
        bool res = cmd->getOptionValue("-max", max);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get fourth value %d \n", max);
    }
    return TCL_OK;
}
int parseSdcSetTimingDerate(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    //junliu changed
    if (!( (cmd->isOptionSet("derate_value") and (cmd->isOptionSet("-early") 
        and !(cmd->isOptionSet("-late"))) 
        or (cmd->isOptionSet("-late") and !(cmd->isOptionSet("-early")))))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("derate_value")) {
        double derate_value = 0.0;
        bool res = cmd->getOptionValue("derate_value", derate_value);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %f \n", derate_value);
    }
    if (cmd->isOptionSet("object_list")) {
        StringVector object_list;
        bool res = cmd->getOptionValue("object_list", object_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &object : object_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get seccond value %s \n", object.c_str());
        }
    }
    if (cmd->isOptionSet("-min")) {
        bool min = false;
        bool res = cmd->getOptionValue("-min", min);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get third value %d \n", min);
    }
    if (cmd->isOptionSet("-max")) {
        bool max = false;
        bool res = cmd->getOptionValue("-max", max);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get fourth value %d \n", max);
    }
    if (cmd->isOptionSet("-rise")) {
        bool rise = false;
        bool res = cmd->getOptionValue("-rise", rise);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get fifth value %d \n", rise);
    }
    if (cmd->isOptionSet("-fall")) {
        bool fall = false;
        bool res = cmd->getOptionValue("-fall", fall);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get sixth value %d \n", fall);
    }
    if (cmd->isOptionSet("-early ")) {
        bool early = false;
        bool res = cmd->getOptionValue("-early", early);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get seventh value %d \n", early);
    }
    if (cmd->isOptionSet("-late")) {
        bool late = false;
        bool res = cmd->getOptionValue("-late", late);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get eighth value %d \n", late);
    }
    if (cmd->isOptionSet("-static")) {
        bool set_static = false;
        bool res = cmd->getOptionValue("-static", set_static);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get ninth value %d \n", set_static);
    }
    if (cmd->isOptionSet("-dynamic")) {
        bool dynamic = false;
        bool res = cmd->getOptionValue("-dynamic", dynamic);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get tenth value %d \n", dynamic);
    }
    if (cmd->isOptionSet("-increment")) {
        bool increment = false;
        bool res = cmd->getOptionValue("-increment", increment);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get eleventh value %d \n", increment);
    }
    if (cmd->isOptionSet("-clock")) {
        bool clock = false;
        bool res = cmd->getOptionValue("-clock", clock);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get twelfth value %d \n", clock);
    }
    if (cmd->isOptionSet("-data")) {
        bool data = false;
        bool res = cmd->getOptionValue("-data", data);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get thirteenth value %d \n", data);
    }
    if (cmd->isOptionSet("-net_delay")) {
        bool net_delay = false;
        bool res = cmd->getOptionValue("-net_delay", net_delay);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get fourteenth value %d \n", net_delay);
    }
    if (cmd->isOptionSet("-cell_delay")) {
        bool cell_delay = false;
        bool res = cmd->getOptionValue("-cell_delay", cell_delay);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get fifteenth value %d \n", cell_delay);
    }
    if (cmd->isOptionSet("-cell_check")) {
        bool cell_check = false;
        bool res = cmd->getOptionValue("-cell_check", cell_check);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get sixteenth value %d \n", cell_check);
    }
    return TCL_OK;
}
int parseSdcSetVoltage(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("value"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("value")) {
        double value = 0.0;
        bool res = cmd->getOptionValue("value", value);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %f \n", value);
    }
    if (cmd->isOptionSet("-object_list")) {
        StringVector object_list;
        bool res = cmd->getOptionValue("-object_list", object_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &object : object_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get seccond value %s \n", object.c_str());
        }
    }
    if (cmd->isOptionSet("-min")) {
        double min = 0.0;
        bool res = cmd->getOptionValue("-min", min);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get third value %f \n", min);
    }
    return TCL_OK;
}

int parseSdcSetWireLoadMinBlockSize(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("size"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("size")) {
        double size = 0.0;
        bool res = cmd->getOptionValue("size", size);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %f \n", size);
    }
    return TCL_OK;
}
int parseSdcSetWireLoadMode(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("mode_name"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("mode_name")) {
        std::string mode_name = "";
        bool res = cmd->getOptionValue("mode_name", mode_name);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %s \n", mode_name.c_str());
    }
    return TCL_OK;
}
int parseSdcSetWireLoadModel(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("-name"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("-name")) {
        std::string name = "";
        bool res = cmd->getOptionValue("-name", name);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %s \n", name.c_str());
    }
    if (cmd->isOptionSet("-library")) {
        StringVector library_list;
        bool res = cmd->getOptionValue("-library", library_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &library : library_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get seccond value %s \n", library.c_str());
        }
    }
    if (cmd->isOptionSet("-min")) {
        bool min = false;
        bool res = cmd->getOptionValue("-min", min);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get third value %d \n", min);
    }
    if (cmd->isOptionSet("-max")) {
        bool max = false;
        bool res = cmd->getOptionValue("-max", max);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get fourth value %d \n", max);
    }
    if (cmd->isOptionSet("object_list")) {
        StringVector object_list;
        bool res = cmd->getOptionValue("object_list", object_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto &object : object_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get fifth value %s \n", object.c_str());
        }
    }
    return TCL_OK;
}
int parseSdcSetWireLoadSelectionGroup(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (!(cmd->isOptionSet("group_name"))) {
        return TCL_ERROR;
    }
    if (cmd->isOptionSet("group_name")) {
        std::string group_name = "";
        bool res = cmd->getOptionValue("group_name", group_name);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get first value %s \n", group_name.c_str());
    }
    if (cmd->isOptionSet("-library")) {
        StringVector library_list;
        bool res = cmd->getOptionValue("-library", library_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& library : library_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get seccond value %s \n", library.c_str());
        }
    }
    if (cmd->isOptionSet("-min")) {
        bool min = false;
        bool res = cmd->getOptionValue("-min", min);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get third value %d \n", min);
    }
    if (cmd->isOptionSet("-max")) {
        bool max = false;
        bool res = cmd->getOptionValue("-max", max);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        //Assignment
        message->info("get fourth value %d \n", max);
    }
    if (cmd->isOptionSet("object_list")) {
        StringVector object_list;
        bool res = cmd->getOptionValue("object_list", object_list);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& object : object_list) {
            //TODO DB team did not implement the API to get pin/term from name
            //Assignment
            message->info("get fifth value %s \n", object.c_str());
        }
    }
    return TCL_OK;
}

// read sdc file
int parseReadSdc(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {

    return TCL_OK;
}

int parseWriteSdc(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {

    return TCL_OK;
}


}
}
