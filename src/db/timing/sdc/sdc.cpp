/** 
* @file sdc.cpp
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

#include "db/timing/sdc/sdc.h"
#include "db/timing/timinglib/libset.h"
#include "db/timing/timinglib/analysis_view.h"
#include "db/timing/timinglib/analysis_corner.h"
#include "db/timing/timinglib/analysis_mode.h"
#include "db/timing/timinglib/timinglib_lib.h"

namespace open_edi {
namespace db {

Sdc::Sdc() {
    initPtr();
}

Sdc::Sdc(ObjectId mode_id) : analysis_mode_id_(mode_id) {
    initId();
    initPtr();
}

void Sdc::initId() {
    AnalysisMode* mode = Object::addr<AnalysisMode>(analysis_mode_id_);
    if (!mode) {
        //TODO error messages
        return;
    }
    analysis_view_id_ = mode->getOwnerId(); 
    AnalysisView* view = Object::addr<AnalysisView>(analysis_view_id_);
    if (!view) {
        //TODO error messages
        return;
    }
    AnalysisCorner* corner = view->getAnalysisCorner();
    if (!corner) {
        //TODO error messages
        return;
    }
    analysis_corner_id_ = corner->getId(); 
    LibSet* lib_set = corner->getLibset();
    if (!lib_set) {
        //TODO error messages 
        return;
    }
    const std::vector<TLib*>& lib_vec = lib_set->getTimingLibs();
    if (lib_vec.size() == 0) {
        //TODO error messages
        return;
    }
    //it always for loop to get the lib vector in libsets, pick the first one as main liberty
    TLib* lib =  lib_vec.front(); 
    if (!lib) {
        //TODO error messages
        return;
    }
    main_liberty_id_ = lib->getId();
    TUnits* units = lib->getUnits();
    main_liberty_units_id_ = units->getId();
}

void Sdc::initPtr() {
    inst_container_ = std::make_shared<SdcCurrentInstanceContainer>();
    separator_container_ = std::make_shared<SdcHierarchySeparatorContainer>();
    units_container_ = std::make_shared<SdcUnitsContainer>();

    clock_container_ = std::make_shared<SdcClockContainer>();
    group_path_container_ = std::make_shared<SdcGroupPathContainer>();
    clock_gating_check_container_ = std::make_shared<SdcClockGatingCheckContainer>();
    clock_groups_container_ = std::make_shared<SdcClockGroupsContainer>();
    clock_latency_container_ = std::make_shared<SdcClockLatencyContainer>();
    sence_container_ = std::make_shared<SdcSenceContainer>();
    clock_transition_container_ = std::make_shared<SdcClockTransitionContainer>();
    clock_uncertainty_container_ = std::make_shared<SdcClockUncertaintyContainer>();
    data_check_container_ = std::make_shared<SdcDataCheckContainer>();
    disable_timing_container_ = std::make_shared<SdcDisableTimingContainer>();
    false_path_container_ = std::make_shared<SdcFalsePathContainer>();
    ideal_latency_container_ = std::make_shared<SdcIdealLatencyContainer>();
    ideal_network_container_ = std::make_shared<SdcIdealNetworkContainer>();
    ideal_transition_container_ = std::make_shared<SdcIdealTransitionContainer>();
    input_delay_container_ = std::make_shared<SdcInputDelayContainer>();
    max_delay_container_ = std::make_shared<SdcMaxDelayContainer>();
    max_time_borrow_container_ = std::make_shared<SdcMaxTimeBorrowContainer>();
    min_delay_container_ = std::make_shared<SdcMinDelayContainer>();
    min_pulse_width_container_ = std::make_shared<SdcMinPulseWidthContainer>();
    multicycle_path_container_ = std::make_shared<SdcMulticyclePathContainer>();
    output_delay_container_ = std::make_shared<SdcOutputDelayContainer>();
    propagated_clock_container_ = std::make_shared<SdcPropagatedClockContainer>();

    case_analysis_container_ = std::make_shared<SdcCaseAnalysisContainer>();
    drive_container_ = std::make_shared<SdcDriveContainer>();
    driving_cell_container_ = std::make_shared<SdcDrivingCellContainer>();
    fanout_load_container_ = std::make_shared<SdcFanoutLoadContainer>();
    input_transition_container_ = std::make_shared<SdcInputTransitionContainer>();
    load_container_ = std::make_shared<SdcLoadContainer>();
    logic_container_ = std::make_shared<SdcLogicContainer>();
    max_area_container_ = std::make_shared<SdcMaxAreaContainer>();
    max_fanout_container_ = std::make_shared<SdcMaxFanoutContainer>();
    max_transition_container_ = std::make_shared<SdcMaxTransitionContainer>();
    min_capacitance_container_ = std::make_shared<SdcMinCapacitanceContainer>();
    operating_conditions_container_ = std::make_shared<SdcOperatingConditionsContainer>();
    port_fanout_number_container_ = std::make_shared<SdcPortFanoutNumberContainer>();
    resistance_container_ = std::make_shared<SdcResistanceContainer>();
    timing_derate_container_ = std::make_shared<SdcTimingDerateContainer>();
    voltage_container_ = std::make_shared<SdcVoltageContainer>();
    wire_load_min_block_size_container_ = std::make_shared<SdcWireLoadMinBlockSizeContainer>();
    wire_load_mode_container_ = std::make_shared<SdcWireLoadModeContainer>();
    wire_load_model_container_ = std::make_shared<SdcWireLoadModelContainer>();
    wire_load_selection_group_container_ = std::make_shared<SdcWireLoadSelectionGroupContainer>();

    voltage_area_container_ = std::make_shared<SdcVoltageAreaContainer>();
    level_shifter_strategy_container_ = std::make_shared<SdcLevelShifterStrategyContainer>();
    level_shifter_threshold_container_ = std::make_shared<SdcLevelShifterThresholdContainer>();
    max_dynamic_power_container_ = std::make_shared<SdcMaxDynamicPowerContainer>();

    all_clocks_container_ = std::make_shared<SdcAllClocksContainer>();
    all_inputs_container_ = std::make_shared<SdcAllInputsContainer>();
    all_registers_container_ = std::make_shared<SdcAllRegistersContainer>();
    design_container_ = std::make_shared<SdcCurrentDesignContainer>();
    get_cells_container_ = std::make_shared<SdcGetCellsContainer>();
    get_clocks_container_ = std::make_shared<SdcGetClocksContainer>();
    get_lib_cells_container_ = std::make_shared<SdcGetLibCellsContainer>();
    get_lib_pins_container_ = std::make_shared<SdcGetLibPinsContainer>();
    get_libs_container_ = std::make_shared<SdcGetLibsContainer>();
    get_nets_container_ = std::make_shared<SdcGetNetsContainer>();
    get_pins_container_ = std::make_shared<SdcGetPinsContainer>();
    get_ports_container_ = std::make_shared<SdcGetPortsContainer>();

}


}
}
