/**
 * @file register_sdc_commands.cpp
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

#include "db/timing/sdc/register_sdc_commands.h"
#include "db/timing/sdc/create_sdc_commands.h"
#include "db/timing/sdc/parse_sdc_commands.h"

namespace open_edi {
namespace db {

// general purpose commands
void bindSdcGeneralPurposeCommandsToParser(Tcl_Interp *itp) {
    Tcl_CreateCommand(itp, "current_instance", parseSdcCurrentInstance, NULL, NULL);
    Tcl_CreateCommand(itp, "set_hierarchy_separator", parseSdcSetHierarchySeparator, NULL, NULL);
    Tcl_CreateCommand(itp, "set_units", parseSdcSetUnits, NULL, NULL);
}

// object access commands
void bindSdcObjectAccessCommandsToParser(Tcl_Interp *itp) {
    Tcl_CreateCommand(itp, "all_clocks", parseSdcAllClocks, NULL, NULL);
    Tcl_CreateCommand(itp, "all_inputs", parseSdcAllInputs, NULL, NULL);
    Tcl_CreateCommand(itp, "all_outputs", parseSdcAllOutputs, NULL, NULL);
    Tcl_CreateCommand(itp, "all_registers", parseSdcAllRegisters, NULL, NULL);
    Tcl_CreateCommand(itp, "current_design", parseSdcCurrentDesign, NULL, NULL);
    Tcl_CreateCommand(itp, "get_cells", parseSdcGetCells, NULL, NULL);
    Tcl_CreateCommand(itp, "get_clocks", parseSdcGetClocks, NULL, NULL);
    Tcl_CreateCommand(itp, "get_lib_cells", parseSdcGetLibCells, NULL, NULL);
    Tcl_CreateCommand(itp, "get_lib_pins", parseSdcGetLibPins, NULL, NULL);
    Tcl_CreateCommand(itp, "get_libs", parseSdcGetLibs, NULL, NULL);
    Tcl_CreateCommand(itp, "get_nets", parseSdcGetNets, NULL, NULL);
    Tcl_CreateCommand(itp, "get_pins", parseSdcGetPins, NULL, NULL);
    Tcl_CreateCommand(itp, "get_ports", parseSdcGetPorts, NULL, NULL);

}

// timing constraints
void bindSdcTimingConstraintsToParser(Tcl_Interp *itp) {

	Tcl_CreateCommand(itp, "create_clock", parseSdcCreateClock, NULL, NULL);
	Tcl_CreateCommand(itp, "create_generated_clock", parseSdcCreateGeneratedClock, NULL, NULL);
	Tcl_CreateCommand(itp, "group_path", parseSdcGroupPath, NULL, NULL);
	
	Tcl_CreateCommand(itp, "set_clock_gating_check", parseSdcSetClockGatingCheck, NULL, NULL);
	Tcl_CreateCommand(itp, "set_clock_groups", parseSdcSetClockGroups, NULL, NULL);
	Tcl_CreateCommand(itp, "set_clock_latency", parseSdcSetClockLatency, NULL, NULL);
	
	Tcl_CreateCommand(itp, "set_sense", parseSdcSetSense, NULL, NULL);
	Tcl_CreateCommand(itp, "set_clock_transition", parseSdcSetClockTransition, NULL, NULL);
	Tcl_CreateCommand(itp, "set_clock_uncertainty", parseSdcSetClockUncertainty, NULL, NULL);
	
	Tcl_CreateCommand(itp, "set_data_check", parseSdcSetDataCheck, NULL, NULL);
	Tcl_CreateCommand(itp, "set_disable_timing", parseSdcSetDisableTiming, NULL, NULL);
	Tcl_CreateCommand(itp, "set_false_path", parseSdcSetFalsePath, NULL, NULL);
	Tcl_CreateCommand(itp, "set_ideal_latency", parseSdcSetIdealLatency, NULL, NULL);
	
	Tcl_CreateCommand(itp, "set_ideal_network", parseSdcSetIdealNetwork, NULL, NULL);
	Tcl_CreateCommand(itp, "set_ideal_transition", parseSdcSetIdealTransition, NULL, NULL);
	Tcl_CreateCommand(itp, "set_input_delay", parseSdcSetInputDelay, NULL, NULL);
	
	Tcl_CreateCommand(itp, "set_max_delay", parseSdcSetMaxDelay, NULL, NULL);
	Tcl_CreateCommand(itp, "set_max_time_borrow", parseSdcSetMaxTimeBorrow, NULL, NULL);
	Tcl_CreateCommand(itp, "set_min_delay", parseSdcSetMinDelay, NULL, NULL);
	
	Tcl_CreateCommand(itp, "set_min_pulse_width", parseSdcSetMinPulseWidth, NULL, NULL);
	Tcl_CreateCommand(itp, "set_multicycle_path", parseSdcSetMulticyclePath, NULL, NULL);
	Tcl_CreateCommand(itp, "set_output_delay", parseSdcSetOutputDelay, NULL, NULL);
	Tcl_CreateCommand(itp, "set_propagated_clock", parseSdcSetPropagatedClock, NULL, NULL);
	//chenqianend

}

// environment commands 
void bindSdcEnvironmentCommandsToParser(Tcl_Interp *itp) {
    Tcl_CreateCommand(itp, "set_case_analysis", parseSdcSetCaseAnalysis, NULL, NULL);
    Tcl_CreateCommand(itp, "set_drive", parseSdcSetDrive, NULL, NULL);
    Tcl_CreateCommand(itp, "set_driving_cell", parseSdcSetDrivingCell, NULL, NULL);
    Tcl_CreateCommand(itp, "set_fanout_load", parseSdcSetFanoutLoad, NULL, NULL);
    Tcl_CreateCommand(itp, "set_input_transition", parseSdcSetInputTransition, NULL, NULL);
    Tcl_CreateCommand(itp, "set_load", parseSdcSetLoad, NULL, NULL);
    Tcl_CreateCommand(itp, "set_logic_dc", parseSdcSetLogicDc, NULL, NULL);
    Tcl_CreateCommand(itp, "set_logic_one", parseSdcSetLogicOne, NULL, NULL);
    Tcl_CreateCommand(itp, "set_logic_zero", parseSdcSetLogicZero, NULL, NULL);
    Tcl_CreateCommand(itp, "set_max_area", parseSdcSetMaxArea, NULL, NULL);
    Tcl_CreateCommand(itp, "set_max_capacitance", parseSdcSetMaxCapacitance, NULL, NULL);
    Tcl_CreateCommand(itp, "set_max_fanout", parseSdcSetMaxFanout, NULL, NULL);
    Tcl_CreateCommand(itp, "set_max_transition", parseSdcSetMaxTransition, NULL, NULL);
    Tcl_CreateCommand(itp, "set_min_capacitance", parseSdcSetMinCapacitance, NULL, NULL);
    Tcl_CreateCommand(itp, "set_operating_conditions", parseSdcSetOperatingConditions, NULL, NULL);
    Tcl_CreateCommand(itp, "set_port_fanout_number", parseSdcSetPortFanoutNumber, NULL, NULL);
    Tcl_CreateCommand(itp, "set_resistance", parseSdcSetResistance, NULL, NULL);
    Tcl_CreateCommand(itp, "set_timing_derate", parseSdcSetTimingDerate, NULL, NULL);
    Tcl_CreateCommand(itp, "set_voltage", parseSdcSetVoltage, NULL, NULL);
    Tcl_CreateCommand(itp, "set_wire_load_min_block_size", parseSdcSetWireLoadMinBlockSize, NULL, NULL);
    Tcl_CreateCommand(itp, "set_wire_load_mode", parseSdcSetWireLoadMode, NULL, NULL);
    Tcl_CreateCommand(itp, "set_wire_load_model", parseSdcSetWireLoadModel, NULL, NULL);
    Tcl_CreateCommand(itp, "set_wire_load_selection_group", parseSdcSetWireLoadSelectionGroup, NULL, NULL);
}

// multivoltage power commands
void bindSdcMultivoltagePowerCommandsToParser(Tcl_Interp *itp) {
    Tcl_CreateCommand(itp, "create_voltage_area", parseSdcCreateVoltageArea, NULL, NULL);
    Tcl_CreateCommand(itp, "set_level_shifter_strategy", parseSdcSetLevelShifterStrategy, NULL, NULL);
    Tcl_CreateCommand(itp, "set_level_shifter_threshold", parseSdcSetLevelShifterThreshold, NULL, NULL);
    Tcl_CreateCommand(itp, "set_max_dynamic_power", parseSdcSetMaxDynamicPower, NULL, NULL);
    Tcl_CreateCommand(itp, "set_max_leakage_power", parseSdcSetMaxLeakagePower, NULL, NULL);
}

void bindSdcCommandsToParser(Tcl_Interp *itp) {
    bindSdcGeneralPurposeCommandsToParser(itp);
    bindSdcObjectAccessCommandsToParser(itp);
    bindSdcTimingConstraintsToParser(itp);
    bindSdcEnvironmentCommandsToParser(itp);
    bindSdcMultivoltagePowerCommandsToParser(itp);
}

// main register tcl commands
void registerSdcCommands(Tcl_Interp *itp) {
    createSdcCommands();
    bindSdcCommandsToParser(itp);
}


}
}
