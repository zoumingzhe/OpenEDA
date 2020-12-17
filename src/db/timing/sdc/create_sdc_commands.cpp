/**
 * @file create_sdc_commands.cpp
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

#include "db/timing/sdc/create_sdc_commands.h"
#include "infra/command_manager.h"

namespace open_edi {
namespace db {

constexpr bool kcreateSuccess = true;
constexpr bool kcreateFailed = false;

using Command = open_edi::infra::Command;
using CommandManager = open_edi::infra::CommandManager;
using Option = open_edi::infra::Option;
using OptionDataType = open_edi::infra::OptionDataType;

// general purpose commands
bool createSdcGeneralPurposeCommands() {
    CommandManager* cmd_manager = CommandManager::getCommandManager();

    Command* cmd = cmd_manager->createCommand(
	"current_instance","\n",
            *(new Option("-instance",OptionDataType::kString,false,"\n"))+
            *(new Option("value",OptionDataType::kDouble,false,"\n"))        //wrong with src/app/openeda,it's a hypothetical argument
    );
    assert(cmd);

    cmd = cmd_manager->createCommand(
	"set_hierarchy_separator","\n",
	    *(new Option("separator",OptionDataType::kString,true,"\n"))
    );
    assert(cmd);

    cmd = cmd_manager->createCommand(
	"set_units","\n",
	    *(new Option("-capacitance",OptionDataType::kString,false,"\n"))+
	    *(new Option("-resistance",OptionDataType::kString,false,"\n"))+
	    *(new Option("-time",OptionDataType::kString,false,"\n"))+
	    *(new Option("-voltage",OptionDataType::kString,false,"\n"))+
	    *(new Option("-current",OptionDataType::kString,false,"\n"))+
	    *(new Option("-power",OptionDataType::kString,false,"\n"))
    );
    assert(cmd);
    return kcreateSuccess;
}

// object access commands
bool createSdcObjectAccessCommands() {
    CommandManager* cmd_manager = CommandManager::getCommandManager();

    Command* cmd = cmd_manager->createCommand(
    "all_clocks","\n",
	    *(new Option())                //Ò»¸öargumentÒ²Ã»ÓÐ ²écreateCommand¶¨Òå 
    );
    assert(cmd);

    cmd = cmd_manager->createCommand(
	"all_inputs","\n",
	    *(new Option("-level_sensitive",OptionDataType::kBool,false,"\n"))+
	    *(new Option("-edge_triggered",OptionDataType::kBool,false,"\n"))+
	    *(new Option("-clock",OptionDataType::kStringList,false,"\n"))
    );
    assert(cmd);

    cmd = cmd_manager->createCommand(
	"all_outputs","\n",
        *(new Option("-level_sensitive",OptionDataType::kBool,false,"\n"))+
        *(new Option("-edge_triggered",OptionDataType::kBool,false,"\n"))+
        *(new Option("-clock",OptionDataType::kBool,false,"\n"))
    );
    assert(cmd);

    cmd = cmd_manager->createCommand(
	"all_registers","\n",
	    *(new Option("-no_hierarchy",OptionDataType::kBool,false,"\n"))+
        *(new Option("-hsc",OptionDataType::kString,false,"\n"))+
	    *(new Option("-clock",OptionDataType::kStringList,false,"\n"))+
        *(new Option("-rise_clock",OptionDataType::kStringList,false,"\n"))+
	    *(new Option("-fall_clock",OptionDataType::kStringList,false,"\n"))+
        *(new Option("-cells",OptionDataType::kBool,false,"\n"))+
	    *(new Option("-data_pins",OptionDataType::kBool,false,"\n"))+
        *(new Option("-clock_pins",OptionDataType::kBool,false,"\n"))+
	    *(new Option("-slave_clock_pins",OptionDataType::kBool,false,"\n"))+
        *(new Option("-async_pins",OptionDataType::kBool,false,"\n"))+
	    *(new Option("-output_pins",OptionDataType::kBool,false,"\n"))+
        *(new Option("-master_slave",OptionDataType::kBool,false,"\n"))+
	    *(new Option("-level_sensitive",OptionDataType::kBool,false,"\n"))+
        *(new Option("-edge_triggered",OptionDataType::kBool,false,"\n"))
     );
     assert(cmd);

     cmd = cmd_manager->createCommand(
        "current_design","\n",
            *(new Option())               //ÎÞoption 
     );
     assert(cmd);

     cmd = cmd_manager->createCommand(
        "get_cells","\n",
	    *(new Option("-hierarchical",OptionDataType::kBool,false,"\n"))+
        *(new Option("-regexp",OptionDataType::kBool,false,"\n"))+
	    *(new Option("-nocase",OptionDataType::kBool,false,"\n"))+
	    *(new Option("-of_objects",OptionDataType::kBool,false,"\n"))+
	    *(new Option("patterns",OptionDataType::kStringList,true,"\n"))
     );
     assert(cmd);

     cmd = cmd_manager->createCommand(
	"get_clocks","\n",
            *(new Option("-regexp",OptionDataType::kBool,false,"\n"))+
            *(new Option("-nocase",OptionDataType::kBool,false,"\n"))+
            *(new Option("patterns",OptionDataType::kStringList,true,"\n"))
     );
     assert(cmd);

     cmd = cmd_manager->createCommand(
	"get_lib_cells","\n",
	    *(new Option("-regexp",OptionDataType::kBool,false,"\n"))+
        *(new Option("-nocase",OptionDataType::kBool,false,"\n"))+
        *(new Option("patterns",OptionDataType::kStringList,true,"\n"))+
	    *(new Option("-hsc",OptionDataType::kString,false,"\n"))
     );
     assert(cmd);

     cmd = cmd_manager->createCommand(
	"get_libs_pins","\n",
        *(new Option("-regexp",OptionDataType::kBool,false,"\n"))+
        *(new Option("-nocase",OptionDataType::kBool,false,"\n"))+
        *(new Option("patterns",OptionDataType::kStringList,true,"\n"))
     );
     assert(cmd);

     cmd = cmd_manager->createCommand(
	"get_libs","\n",
        *(new Option("-regexp",OptionDataType::kBool,false,"\n"))+
        *(new Option("-nocase",OptionDataType::kBool,false,"\n"))+
        *(new Option("patterns",OptionDataType::kStringList,true,"\n"))
     );
     assert(cmd);

     cmd = cmd_manager->createCommand(
    "get_nets","\n",
        *(new Option("-hierarchical",OptionDataType::kBool,false,"\n"))+
        *(new Option("-regexp",OptionDataType::kBool,false,"\n"))+
        *(new Option("-nocase",OptionDataType::kBool,false,"\n"))+
        *(new Option("-of_objects",OptionDataType::kStringList,false,"\n"))+
        *(new Option("patterns",OptionDataType::kStringList,true,"\n"))+
        *(new Option("-hsc",OptionDataType::kString,false,"\n"))
     );
     assert(cmd);

     cmd = cmd_manager->createCommand(
	"get_pins","\n",
        *(new Option("-hierarchical",OptionDataType::kBool,false,"\n"))+
        *(new Option("-regexp",OptionDataType::kBool,false,"\n"))+
        *(new Option("-nocase",OptionDataType::kBool,false,"\n"))+
        *(new Option("-of_objects",OptionDataType::kStringList,false,"\n"))+
        *(new Option("patterns",OptionDataType::kStringList,true,"\n"))+
        *(new Option("-hsc",OptionDataType::kString,false,"\n"))
     );
     assert(cmd);

     cmd = cmd_manager->createCommand(
	"get_ports","\n",
        *(new Option("-regexp",OptionDataType::kBool,false,"\n"))+
        *(new Option("-nocase",OptionDataType::kBool,false,"\n"))+
        *(new Option("patterns",OptionDataType::kStringList,true,"\n"))
    );
    assert(cmd);
    return kcreateSuccess;
}

// timing constraints
bool createSdcTimingConstraints() {

    //TODO command create success check
    CommandManager* cmd_manager = CommandManager::getCommandManager();

	//chenqian
	//01 create_clock
	Command* cmd = cmd_manager->createCommand(
		   "create_clock", "\n", 
		   *(new Option("-period", OptionDataType::kDouble, true, "\n")) + 
		   *(new Option("-name", OptionDataType::kString, false, "\n")) + 
		   *(new Option("-comment", OptionDataType::kString, false, "\n")) + 
		   *(new Option("-waveform", OptionDataType::kDoubleList, false, "\n")) + 
		   *(new Option("port_pin_list", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-add", OptionDataType::kBool, false, "\n"))
		   
	);
    assert(cmd);
	//02 create_generated_clock
	cmd = cmd_manager->createCommand(
		   "create_generated_clock", "\n", 
		   *(new Option("-name", OptionDataType::kString, false, "\n")) + 
		   *(new Option("-source", OptionDataType::kStringList, true, "\n")) + 
		   *(new Option("-edges", OptionDataType::kDoubleList, false, "\n")) + 
		   *(new Option("-divide_by", OptionDataType::kInt, false, "\n")) + 
		   *(new Option("-multiply_by", OptionDataType::kInt, false, "\n")) + 
		   *(new Option("-edge_shift", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-duty_cycle", OptionDataType::kDouble, false, "\n")) + 
		   *(new Option("-invert", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("port_pin_list", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-add", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-comment", OptionDataType::kString, false, "\n")) + 
		   *(new Option("-master_clock", OptionDataType::kStringList, false, "\n"))
		   
	   );
    assert(cmd);
	//03 group_path
	cmd = cmd_manager->createCommand(
		   "group_path", "\n", 
		   *(new Option("-name", OptionDataType::kString, false, "\n")) + 
		   *(new Option("-default", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-weight", OptionDataType::kDouble, false, "\n")) + 
		   *(new Option("-from", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-rise_from", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-fall_from", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-to", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-rise_to", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-fall_to", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-through", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-rise_through", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-fall_through", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-comment", OptionDataType::kString, false, "\n"))
		   
	   );
    assert(cmd);
	//04 set_clock_gating_check
	cmd = cmd_manager->createCommand(
		   "set_clock_gating_check", "\n", 
		   *(new Option("-setup", OptionDataType::kDouble, false, "\n")) + 
		   *(new Option("-hold", OptionDataType::kDouble, false, "\n")) + 
		   *(new Option("-rise", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-fall", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-high", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-low", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("object_list", OptionDataType::kStringList, false, "\n"))
		   
	   );
    assert(cmd);
	//05 set_clock_groups
	cmd = cmd_manager->createCommand(
		   "set_clock_groups", "\n", 
		   *(new Option("-name", OptionDataType::kString, false, "\n")) + 
		   *(new Option("-physically_exclusive", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-logically_exclusive", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-asynchronous", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-allow_paths", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-group", OptionDataType::kStringList, true, "\n")) + 
		   *(new Option("-comment", OptionDataType::kString, false, "\n"))
		   
	   );
    assert(cmd);
	//06 set_clock_latency
	cmd = cmd_manager->createCommand(
		   "set_clock_latency", "\n", 
		   *(new Option("delay", OptionDataType::kDouble, true, "\n")) + 
		   *(new Option("object_list", OptionDataType::kStringList, true, "\n")) + 
		   *(new Option("-rise", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-fall", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-min", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-max", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-dynamic", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-source", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-early", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-late", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-clock", OptionDataType::kStringList, false, "\n"))
		   
	   );
    assert(cmd);
	//07 set_sense
	cmd = cmd_manager->createCommand(
		   "set_sense", "\n", 
		   *(new Option("-type", OptionDataType::kEnum, false, "\n")) + 
		   *(new Option("-non_unate", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-clocks", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-positive", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-negative", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-clock_leaf", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-stop_propagation", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-pulse", OptionDataType::kEnum, false, "\n")) + 
		   *(new Option("object_list", OptionDataType::kStringList, true, "\n"))
		   
	   );
    assert(cmd);
	//08 set_clock_transition
	cmd = cmd_manager->createCommand(
		   "set_clock_transition", "\n", 
		   *(new Option("transition", OptionDataType::kDouble, true, "\n")) + 
		   *(new Option("clock_list", OptionDataType::kStringList, true, "\n")) + 
		   *(new Option("-rise", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-fall", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-min", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-max", OptionDataType::kBool, false, "\n"))
   
	   );
    assert(cmd);
	//09 set_clock_uncertainty
	cmd = cmd_manager->createCommand(
		   "set_clock_uncertainty", "\n", 
		   *(new Option("uncertainty", OptionDataType::kDouble, true, "\n")) + 
		   *(new Option("-from", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-to", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-rise_to", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-fall_to", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-rise_from", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-fall_from", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-rise", OptionDataType::kBool, false, "\n")) +
		   *(new Option("-fall", OptionDataType::kBool, false, "\n")) +
		   *(new Option("-setup", OptionDataType::kBool, false, "\n")) +
		   *(new Option("-hold", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("object_list", OptionDataType::kStringList, false, "\n"))
   
	   );
    assert(cmd);
	//10 set_data_check
	cmd = cmd_manager->createCommand(
		   "set_data_check", "\n", 
		   *(new Option("-from", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-rise_from", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-fall_from", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-to", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-rise_to", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-fall_to", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-rise", OptionDataType::kBool, false, "\n")) +
		   *(new Option("-fall", OptionDataType::kBool, false, "\n")) +
		   *(new Option("-setup", OptionDataType::kBool, false, "\n")) +
		   *(new Option("-hold", OptionDataType::kBool, false, "\n")) +
		   *(new Option("-clock", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("value", OptionDataType::kDouble, true, "\n"))
   
	   );
    assert(cmd);
	//11 set_disable_timing
	cmd = cmd_manager->createCommand(
		   "set_disable_timing", "\n", 
		   *(new Option("object_list", OptionDataType::kStringList, true, "\n")) + 
		   *(new Option("-from", OptionDataType::kString, false, "\n")) + 
		   *(new Option("-to", OptionDataType::kString, false, "\n"))
   
	   );
    assert(cmd);
	//12 set_false_path
	cmd = cmd_manager->createCommand(
		   "set_false_path", "\n", 
		   *(new Option("-setup", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-hold", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-rise", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-fall", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-from", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-rise_from", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-fall_from", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-to", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-rise_to", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-fall_to", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-through", OptionDataType::kStringList, true, "\n")) + 
		   *(new Option("-fall_throough", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-rise_throough", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-comment", OptionDataType::kString, false, "\n"))
   
	   );
    assert(cmd);
	//13 set_ideal_latency
	cmd = cmd_manager->createCommand(
		   "set_ideal_latency", "\n", 
		   *(new Option("value", OptionDataType::kDouble, true, "\n")) + 
		   *(new Option("object_list", OptionDataType::kStringList, true, "\n")) + 
		   *(new Option("-rise", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-fall", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-min", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-max", OptionDataType::kBool, false, "\n"))
   
	   );
    assert(cmd);
	//14 set_ideal_network
	cmd = cmd_manager->createCommand(
		   "set_ideal_network", "\n", 
		   *(new Option("object_list", OptionDataType::kStringList, true, "\n")) + 
		   *(new Option("-no_propagate", OptionDataType::kBool, false, "\n"))
   
	   );
    assert(cmd);
	//15 set_ideal_transition
	cmd = cmd_manager->createCommand(
		   "set_ideal_transition", "\n", 
		   *(new Option("value", OptionDataType::kDouble, true, "\n")) + 
		   *(new Option("object_list", OptionDataType::kStringList, true, "\n")) + 
		   *(new Option("-rise", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-fall", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-min", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-max", OptionDataType::kBool, false, "\n"))
   
	   );
    assert(cmd);
	//16 set_input_delay
	cmd = cmd_manager->createCommand(
		   "set_input_delay", "\n", 
		   *(new Option("-clock", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-clock_fall", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-level_sensitive", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-rise", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-fall", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-min", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-max", OptionDataType::kBool, false, "\n")) +
		   *(new Option("-add_delay", OptionDataType::kBool, false, "\n")) +
		   *(new Option("delay_value", OptionDataType::kDouble,  true, "\n")) +
		   *(new Option("-reference_pin", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("port_pin_list", OptionDataType::kStringList, true, "\n")) +
		   *(new Option("-network_latency_included", OptionDataType::kBool, false, "\n")) +
		   *(new Option("-source_latency_included", OptionDataType::kBool, false, "\n"))
   
	   );
    assert(cmd);
	//17 set_max_delay
	cmd = cmd_manager->createCommand(
		   "set_max_delay", "\n", 
		   *(new Option("delay_value", OptionDataType::kDouble, true, "\n")) + 
		   *(new Option("-rise", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-fall", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-from", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-rise_from", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-fall_from", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-to", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-rise_to", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-fall_to", OptionDataType::kStringList,  false, "\n")) +
		   *(new Option("-through", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-fall_through", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-rise_through", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-ignore_clock_latency", OptionDataType::kBool, false, "\n")) +
		   *(new Option("-comment", OptionDataType::kString, false, "\n"))
   
	   );
    assert(cmd);
	//18 set_max_time_borrow
	cmd = cmd_manager->createCommand(
		   "set_max_time_borrow", "\n", 
		   *(new Option("delay_value", OptionDataType::kDouble, true, "\n")) + 
		   *(new Option("object_list", OptionDataType::kStringList, true, "\n"))
   
	   );
    assert(cmd);
	//19 set_min_delay
	cmd = cmd_manager->createCommand(
		   "set_min_delay", "\n", 
		   *(new Option("delay_value", OptionDataType::kDouble, true, "\n")) + 
		   *(new Option("-rise", OptionDataType::kBool, false, "\n")) +
		   *(new Option("-fall", OptionDataType::kBool, false, "\n")) +
		   *(new Option("-from", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-rise_from", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-fall_from", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-to", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-rise_to", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-fall_to", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-through", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-fall_through", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-rise_through", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-ignore_clock_latency", OptionDataType::kBool, false, "\n")) +
		   *(new Option("-comment", OptionDataType::kString, false, "\n"))
   
	   );
    assert(cmd);
	//20 set_min_pulse_width
	cmd = cmd_manager->createCommand(
		   "set_min_pulse_width", "\n", 
		   *(new Option("value", OptionDataType::kDouble, true, "\n")) + 
		   *(new Option("object_list", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-low", OptionDataType::kBool, false, "\n")) +
		   *(new Option("-high", OptionDataType::kBool, false, "\n"))
   
	   );
    assert(cmd);
	//21 set_multicycle_path
	cmd = cmd_manager->createCommand(
		   "set_multicycle_path", "\n", 
		   *(new Option("path_multiplier", OptionDataType::kInt, true, "\n")) + 
		   *(new Option("-setup", OptionDataType::kBool, false, "\n")) +
		   *(new Option("-hold", OptionDataType::kBool, false, "\n")) +
		   *(new Option("-rise", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-fall", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-start", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-end", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-from", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-to", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-rise_to", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-fall_to", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-rise_from", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-fall_from", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-rise_through", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-fall_through", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-through", OptionDataType::kStringList, false, "\n")) +
		   *(new Option("-comment", OptionDataType::kString, false, "\n"))
   
	   );
    assert(cmd);
	//22 set_output_delay
	cmd = cmd_manager->createCommand(
		   "set_output_delay", "\n", 
		   *(new Option("-clock", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("-clock_fall", OptionDataType::kBool, false, "\n")) +
		   *(new Option("-level_sensitive", OptionDataType::kBool, false, "\n")) +
		   *(new Option("-rise", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-fall", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-min", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-max", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-add_delay", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("delay_value", OptionDataType::kDouble, true, "\n")) + 
		   *(new Option("-reference_pin", OptionDataType::kStringList, false, "\n")) + 
		   *(new Option("port_pin_list", OptionDataType::kStringList, true, "\n")) + 
		   *(new Option("-network_latency_included", OptionDataType::kBool, false, "\n")) + 
		   *(new Option("-source_latency_included", OptionDataType::kBool, false, "\n"))
   
	   );
    assert(cmd);
	//23 set_propagated_clock
	cmd = cmd_manager->createCommand(
		   "set_propagated_clock", "\n", 
		   *(new Option("object_list", OptionDataType::kStringList, true, "\n"))
   
	   );
    assert(cmd);
	//chenqianend
	
    return kcreateSuccess;
}

// environment commands 
bool createSdcEnvironmentCommands() {
    //TODO command create success check
    CommandManager* cmd_manager = CommandManager::getCommandManager();

    Command* cmd = cmd_manager->createCommand(
        "set_case_analysis", "\n", 
            *(new Option("value", OptionDataType::kString, true, "\n")) + 
            *(new Option("port_or_pin_list", OptionDataType::kStringList, true, "\n"))
    );
    assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_drive", "\n",
		*(new Option("resistance", OptionDataType::kDouble, true, "\n")) +
		*(new Option("port_list", OptionDataType::kStringList, true, "\n")) +     
		*(new Option("-rise", OptionDataType::kBool, false, "\n")) +
		*(new Option("-fall", OptionDataType::kBool, false, "\n")) +
		*(new Option("-min", OptionDataType::kBool, false, "\n")) +
		*(new Option("-max", OptionDataType::kBool, false, "\n"))
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_driving_cell", "\n",
		*(new Option("-lib_cell", OptionDataType::kString, false, "\n")) +
		*(new Option("-rise", OptionDataType::kBool, false, "\n")) +
		*(new Option("-fall", OptionDataType::kBool, false, "\n")) +
		*(new Option("-library", OptionDataType::kString, false, "\n")) +
		*(new Option("-pin", OptionDataType::kString, false, "\n")) +
		*(new Option("-from_pin", OptionDataType::kString, false, "\n")) +
		*(new Option("-dont_scale", OptionDataType::kBool, false, "\n")) +
		*(new Option("-no_design_rule", OptionDataType::kBool, false, "\n")) +
		*(new Option("-input_transition_rise", OptionDataType::kDouble, false, "\n")) +
		*(new Option("-input_transition_fall", OptionDataType::kDouble, false, "\n")) +
		*(new Option("port_list", OptionDataType::kStringList, false, "\n")) +     
		*(new Option("-min", OptionDataType::kBool, false, "\n")) +
		*(new Option("-max", OptionDataType::kBool, false, "\n")) +
		*(new Option("-clock", OptionDataType::kStringList, false, "\n")) +     
		*(new Option("-clock_fall", OptionDataType::kBool, false, "\n"))
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_fanout_load", "\n",
		*(new Option("value", OptionDataType::kDouble, true, "\n")) +
		*(new Option("port_list", OptionDataType::kStringList, true, "\n"))      
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_input_transition", "\n",
		*(new Option("-rise", OptionDataType::kBool, false, "\n")) +
		*(new Option("-fall", OptionDataType::kBool, false, "\n")) +
		*(new Option("-min", OptionDataType::kBool, false, "\n")) +
		*(new Option("-max", OptionDataType::kBool, false, "\n")) +
		*(new Option("transition", OptionDataType::kDouble, true, "\n")) +
		*(new Option("port_list", OptionDataType::kStringList, true, "\n")) +      
		*(new Option("-clock", OptionDataType::kStringList, false, "\n")) +     
		*(new Option("-clock_fall", OptionDataType::kBool, false, "\n"))
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_load", "\n",
		*(new Option("-min", OptionDataType::kBool, false, "\n")) +
		*(new Option("-max", OptionDataType::kBool, false, "\n")) +
		*(new Option("-substract_pin_load", OptionDataType::kBool, false, "\n")) +
		*(new Option("-pin_load", OptionDataType::kBool, false, "\n")) +
		*(new Option("-pin_load", OptionDataType::kBool, false, "\n")) +
		*(new Option("value", OptionDataType::kDouble, true, "\n")) +
		*(new Option("objects", OptionDataType::kStringList, true, "\n"))      
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_logic_dc", "\n",
		*(new Option("port_list", OptionDataType::kStringList, true, "\n"))      
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_logic_one", "\n",
		*(new Option("port_list", OptionDataType::kStringList, true, "\n"))      
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_logic_zero", "\n",
		*(new Option("port_list", OptionDataType::kStringList, true, "\n"))      
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_max_area", "\n",
		*(new Option("area_value", OptionDataType::kDouble, true, "\n"))      
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_max_capacitance", "\n",
		*(new Option("value", OptionDataType::kDouble, true, "\n")) +
		*(new Option("object_list", OptionDataType::kStringList, true, "\n"))      
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_max_fanout", "\n",
		*(new Option("fanout_value", OptionDataType::kDouble, true, "\n")) +
		*(new Option("object_list", OptionDataType::kStringList, true, "\n"))      
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_max_transition", "\n",
		*(new Option("transition_value", OptionDataType::kDouble, true, "\n")) +
		*(new Option("-clock_path", OptionDataType::kBool, false, "\n")) +
		*(new Option("-fall", OptionDataType::kBool, false, "\n")) +
		*(new Option("-rise", OptionDataType::kBool, false, "\n")) +
		*(new Option("object_list", OptionDataType::kStringList, true, "\n"))      
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_min_capacitance", "\n",
		*(new Option("capacitance_value", OptionDataType::kDouble, true, "\n")) +
		*(new Option("object_list", OptionDataType::kStringList, true, "\n"))      
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_operating_conditions", "\n",
		*(new Option("-analysis_type", OptionDataType::kString, false, "\n")) +
		*(new Option("-library", OptionDataType::kStringList, false, "\n")) +     
		*(new Option("-max", OptionDataType::kString, false, "\n")) +
		*(new Option("-min", OptionDataType::kString, false, "\n")) +
		*(new Option("-max_library", OptionDataType::kStringList, false, "\n")) +      
		*(new Option("-object_list", OptionDataType::kStringList, false, "\n")) +     
		*(new Option("-min_library", OptionDataType::kStringList, false, "\n")) +     
		*(new Option("condition", OptionDataType::kString, false, "\n"))
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_port_fanout_number", "\n",
		*(new Option("fanout_number", OptionDataType::kInt, true, "\n")) +
		*(new Option("port_list", OptionDataType::kStringList, true, "\n"))      
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_resistance", "\n",
		*(new Option("value", OptionDataType::kDouble, true, "\n")) +
		*(new Option("net_list", OptionDataType::kStringList, true, "\n")) +      
		*(new Option("-min", OptionDataType::kBool, false, "\n")) +
		*(new Option("-max", OptionDataType::kBool, false, "\n"))
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_timing_derate", "\n",
		*(new Option("derate_value", OptionDataType::kDouble, true, "\n")) +
		*(new Option("object_list", OptionDataType::kStringList, false, "\n")) +     
		*(new Option("-min", OptionDataType::kBool, false, "\n")) +
		*(new Option("-max", OptionDataType::kBool, false, "\n")) +
		*(new Option("-rise", OptionDataType::kBool, false, "\n")) +
		*(new Option("-fall", OptionDataType::kBool, false, "\n")) +
		*(new Option("-early", OptionDataType::kBool, false, "\n")) +
		*(new Option("-late", OptionDataType::kBool, false, "\n")) +
		*(new Option("-static", OptionDataType::kBool, false, "\n")) +
		*(new Option("-dynamic", OptionDataType::kBool, false, "\n")) +
		*(new Option("-increment", OptionDataType::kBool, false, "\n")) +
		*(new Option("-clock", OptionDataType::kBool, false, "\n")) +
		*(new Option("-data", OptionDataType::kBool, false, "\n")) +
		*(new Option("-net_delay", OptionDataType::kBool, false, "\n")) +
		*(new Option("-cell_delay", OptionDataType::kBool, false, "\n")) +
		*(new Option("-cell_check", OptionDataType::kBool, false, "\n"))
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_voltage", "\n",
		*(new Option("value", OptionDataType::kDouble, false, "\n")) +
		*(new Option("-object_list", OptionDataType::kStringList, false, "\n")) +     
		*(new Option("-min", OptionDataType::kDouble, true, "\n"))
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_wire_load_min_block_size", "\n",
		*(new Option("size", OptionDataType::kDouble, false, "\n"))
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_wire_load_mode", "\n",
		*(new Option("mode_name", OptionDataType::kString, false, "\n"))
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_wire_load_model", "\n",
		*(new Option("-name", OptionDataType::kString, true, "\n")) +
		*(new Option("-library", OptionDataType::kStringList, false, "\n")) +
		*(new Option("-min", OptionDataType::kBool, false, "\n")) +
		*(new Option("-max", OptionDataType::kBool, false, "\n")) +
		*(new Option("object_list", OptionDataType::kStringList, false, "\n"))
	);
	assert(cmd);
	cmd = cmd_manager->createCommand(
		"set_wire_load_selection_group", "\n",
		*(new Option("-group_name", OptionDataType::kString, true, "\n")) +
		*(new Option("-library", OptionDataType::kStringList, false, "\n")) +
		*(new Option("-min", OptionDataType::kBool, false, "\n")) +
		*(new Option("-max", OptionDataType::kBool, false, "\n")) +
		*(new Option("object_list", OptionDataType::kStringList, false, "\n"))
	);
	assert(cmd);

	return kcreateSuccess;
}

// multivoltage power commands
bool createSdcMultivoltagePowerCommands() {

    CommandManager* cmd_manager = CommandManager::getCommandManager();

    Command* cmd = cmd_manager->createCommand(
        "create_voltage_area","\n",
            *(new Option("-name",OptionDataType::kString,false,"\n"))+
            *(new Option("-coordinate",OptionDataType::kStringList,false,"\n"))+
            *(new Option("-guard_band_x",OptionDataType::kDoubleList,false,"\n"))+
            *(new Option("-guard_band_y",OptionDataType::kDoubleList,false,"\n"))+
            *(new Option("cell_list",OptionDataType::kStringList,true,"\n"))
    );
    assert(cmd);

    cmd = cmd_manager->createCommand(
        "set_level_shifter_strategy","\n",
	    *(new Option("-rule",OptionDataType::kString,false,"\n"))
    );
    assert(cmd);

    cmd = cmd_manager->createCommand(
	"set_level_shifter_threshold","\n",
            *(new Option("-voltage",OptionDataType::kString,false,"\n"))+
	    *(new Option("-percent",OptionDataType::kString,false,"\n"))
    );
    assert(cmd);

    cmd = cmd_manager->createCommand(
        "set_max_dynamic_power","\n",
            *(new Option("power_value",OptionDataType::kString,true,"\n"))+
            *(new Option("-unit",OptionDataType::kString,false,"\n"))
    );
    assert(cmd);

    cmd = cmd_manager->createCommand(
	"set_max_leakage_power","\n",
            *(new Option("power_value",OptionDataType::kString,true,"\n"))+
	    *(new Option("-unit",OptionDataType::kString,false,"\n"))
    );
    assert(cmd);
    return kcreateSuccess;
}

// main create manager
bool createSdcCommands() {
    return {
        createSdcGeneralPurposeCommands() and
        createSdcObjectAccessCommands() and
        createSdcTimingConstraints() and
        createSdcEnvironmentCommands() and
        createSdcMultivoltagePowerCommands()
    };
}

};
};
