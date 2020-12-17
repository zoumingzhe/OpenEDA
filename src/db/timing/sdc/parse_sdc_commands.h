/**
 * @file parse_sdc_commands_manager.h
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

#ifndef EDI_DB_TIMING_SDC_PARSE_SDC_COMMANDS_H_
#define EDI_DB_TIMING_SDC_PARSE_SDC_COMMANDS_H_

#include "tcl.h"

namespace open_edi {
namespace db {


// general purpose commands manager
int parseSdcCurrentInstance(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetUnits(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetHierarchySeparator(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);

// object access commands manager
int parseSdcAllClocks(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcAllInputs(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcAllOutputs(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcAllRegisters(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcCurrentDesign(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcGetCells(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcGetClocks(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcGetLibCells(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcGetLibPins(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcGetLibs(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcGetNets(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcGetPins(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcGetPorts(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);

// timing constraints manager
int parseSdcCreateClock(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcCreateGeneratedClock(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcGroupPath(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetClockGatingCheck(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetClockGroups(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetClockLatency(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetSense(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetClockTransition(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetClockUncertainty(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetDataCheck(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetDisableTiming(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetFalsePath(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetIdealLatency(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetIdealNetwork(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetIdealTransition(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetInputDelay(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetMaxDelay(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetMaxTimeBorrow(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetMinDelay(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetMinPulseWidth(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetMulticyclePath(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetOutputDelay(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetPropagatedClock(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);

// environment commands
int parseSdcSetCaseAnalysis(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetDrive(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetDrivingCell(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetFanoutLoad(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetInputTransition(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetLoad(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetLogicDc(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetLogicOne(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetLogicZero(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetMaxArea(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetMaxCapacitance(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetMaxFanout(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetMaxTransition(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetMinCapacitance(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetOperatingConditions(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetPortFanoutNumber(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetResistance(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetTimingDerate(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetVoltage(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetWireLoadMinBlockSize(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetWireLoadMode(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetWireLoadModel(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);
int parseSdcSetWireLoadSelectionGroup(ClientData cld, Tcl_Interp* itp, int argc, const char* argv[]);

// multivoltage power commands manager
int parseSdcCreateVoltageArea(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetLevelShifterStrategy(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetLevelShifterThreshold(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetMaxDynamicPower(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseSdcSetMaxLeakagePower(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);


// read write sdc
int parseReadSdc(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
int parseWriteSdc(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);

};

}
#endif //EDI_DB_TIMING_SDC_PARSE_SDC_COMMANDS_H_
