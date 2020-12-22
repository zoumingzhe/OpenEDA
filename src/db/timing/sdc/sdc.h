/** 
 * @file sdc.h
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

#ifndef EDI_DB_TIMING_SDC_SDC_H_
#define EDI_DB_TIMING_SDC_SDC_H_

#include "db/timing/sdc/sdc_data.h"

namespace open_edi {
namespace db {

class Sdc {
  public:
    Sdc();
    Sdc(ObjectId mode_id);
    Sdc(const Sdc &rhs)=delete;
    Sdc &operator=(const Sdc &rhs)=delete;
    ~Sdc()=default;

    //init
    void initId();
    void initPtr();

    //get
    const ObjectId getAnalysisModeId() const { return analysis_mode_id_; }
    const ObjectId getAnalysisViewId() const { return analysis_view_id_; }
    const ObjectId getAnalysisCornerId() const { return analysis_corner_id_; }
    const ObjectId getMainLibertyId() const { return main_liberty_id_; }
    const ObjectId getMainLibertyUnitsId() const { return main_liberty_units_id_; }

    //get
    SdcCurrentInstanceContainerPtr getCurrentInstanceContainer() { return inst_container_; }
    SdcHierarchySeparatorContainerPtr getHierarchySeparatorContainer() { return separator_container_; }
    SdcUnitsContainerPtr getUnitsContainer() { return units_container_; }

    SdcClockContainerPtr getClockContainer() { return clock_container_; }
    SdcGroupPathContainerPtr getGroupPathContainer() { return group_path_container_; }
    SdcClockGatingCheckContainerPtr getClockGatingCheckContainer() { return clock_gating_check_container_; }
    SdcClockGroupsContainerPtr getClockGroupsContainer() { return clock_groups_container_; }
    SdcClockLatencyContainerPtr getClockLatencyContainer() { return clock_latency_container_; }
    SdcSenseContainerPtr getSenseContainer() { return sense_container_; }
    SdcClockTransitionContainerPtr getClockTransitionContainer() { return clock_transition_container_; }
    SdcClockUncertaintyContainerPtr getClockUncertaintyContainer() { return clock_uncertainty_container_; }
    SdcDataCheckContainerPtr getDataCheckContainer() { return data_check_container_; }
    SdcDisableTimingContainerPtr getDisableTimingContainer() { return disable_timing_container_; }
    SdcFalsePathContainerPtr getFalsePathContainer() { return false_path_container_; }
    SdcIdealLatencyContainerPtr getIdealLatencyContainer() { return ideal_latency_container_; }
    SdcIdealNetworkContainerPtr getIdealNetworkContainer() { return ideal_network_container_; }
    SdcIdealTransitionContainerPtr getIdealTransitionContainer() { return ideal_transition_container_; }
    SdcInputDelayContainerPtr getInputDelayContainer() { return input_delay_container_; }
    SdcMaxDelayContainerPtr getMaxDelayContainer() { return max_delay_container_; }
    SdcMaxTimeBorrowContainerPtr getMaxTimeBorrowContainer() { return max_time_borrow_container_; }
    SdcMinDelayContainerPtr getMinDelayContainer() { return min_delay_container_; }
    SdcMinPulseWidthContainerPtr getMinPulseWidthContainer() { return min_pulse_width_container_; }
    SdcMulticyclePathContainerPtr getMulticyclePathContainer() { return multicycle_path_container_; }
    SdcOutputDelayContainerPtr getOutputDelayContainer() { return output_delay_container_; }
    SdcPropagatedClockContainerPtr getPropagatedClockContainer() { return propagated_clock_container_; }

    SdcCaseAnalysisContainerPtr getCaseAnalysisContainer() { return case_analysis_container_; }
    SdcDriveContainerPtr getDriveContainer() { return drive_container_; }
    SdcDrivingCellContainerPtr getDrivingCellContainer() { return driving_cell_container_; }
    SdcFanoutLoadContainerPtr getFanoutLoadContainer() { return fanout_load_container_; }
    SdcInputTransitionContainerPtr getInputTransitionContainer() { return input_transition_container_; }
    SdcLoadContainerPtr getLoadContainer() { return load_container_; }
    SdcLogicContainerPtr getLogicContainer() { return logic_container_; }
    SdcMaxAreaContainerPtr getMaxAreaContainer() { return max_area_container_; }
    SdcMaxFanoutContainerPtr getMaxFanoutContainer() { return max_fanout_container_;}
    SdcMaxTransitionContainerPtr getMaxTransitionContainer() { return max_transition_container_; }
    SdcMinCapacitanceContainerPtr getMinCapacitanceContainer() { return min_capacitance_container_; }
    SdcOperatingConditionsContainerPtr getOperatingConditionsContainer() { return operating_conditions_container_; }
    SdcPortFanoutNumberContainerPtr getPortFanoutNumberContainer() { return port_fanout_number_container_; }
    SdcResistanceContainerPtr getResistanceContainer() { return resistance_container_; }
    SdcTimingDerateContainerPtr getTimingDerateContainer() { return timing_derate_container_; }
    SdcVoltageContainerPtr getVoltageContainer() { return voltage_container_;}
    SdcWireLoadMinBlockSizeContainerPtr getWireLoadMinBlockSizeContainer() { return wire_load_min_block_size_container_; }
    SdcWireLoadModeContainerPtr getWireLoadModeContainer() { return wire_load_mode_container_; }
    SdcWireLoadModelContainerPtr getWireLoadModelContainer() { return wire_load_model_container_; }
    SdcWireLoadSelectionGroupContainerPtr getWireLoadSelectionGroupContainer() { return wire_load_selection_group_container_; }

    SdcVoltageAreaContainerPtr getVoltageAreaContainer() { return voltage_area_container_; };
    SdcLevelShifterStrategyContainerPtr getLevelShifterStrategyContainer() { return level_shifter_strategy_container_; }
    SdcLevelShifterThresholdContainerPtr getLevelShifterThresholdContainer() { return level_shifter_threshold_container_; }
    SdcMaxDynamicPowerContainerPtr getMaxDynamicPowerContainer() { return max_dynamic_power_container_; }

    SdcAllClocksContainerPtr getAllClocksContainer() { return all_clocks_container_; }
    SdcAllInputsContainerPtr getAllInputsContainer() { return all_inputs_container_; }
    SdcAllRegistersContainerPtr getAllRegistersContainer() { return all_registers_container_; }
    SdcCurrentDesignContainerPtr getCurrentDesignContainer() { return design_container_; }
    SdcGetCellsContainerPtr getCellsContainer() { return get_cells_container_; }
    SdcGetClocksContainerPtr GetClocksContainer() { return get_clocks_container_; }
    SdcGetLibCellsContainerPtr GetLibCellsContainer() { return get_lib_cells_container_; }
    SdcGetLibPinsContainerPtr GetLibPinsContainer() { return get_lib_pins_container_; }
    SdcGetLibsContainerPtr GetLibsContainer() { return get_libs_container_; }
    SdcGetNetsContainerPtr GetNetsContainer() { return get_nets_container_; }
    SdcGetPinsContainerPtr GetPinsContainer() { return get_pins_container_; }
    SdcGetPortsContainerPtr GetPortsContainer() { return get_ports_container_; }

  private:
    SdcCurrentInstanceContainerPtr inst_container_;
    SdcHierarchySeparatorContainerPtr separator_container_;
    SdcUnitsContainerPtr units_container_;

    SdcClockContainerPtr clock_container_;
    SdcGroupPathContainerPtr group_path_container_;
    SdcClockGatingCheckContainerPtr clock_gating_check_container_;
    SdcClockGroupsContainerPtr clock_groups_container_;
    SdcClockLatencyContainerPtr clock_latency_container_;
    SdcSenseContainerPtr sense_container_;
    SdcClockTransitionContainerPtr clock_transition_container_;
    SdcClockUncertaintyContainerPtr clock_uncertainty_container_;
    SdcDataCheckContainerPtr data_check_container_;
    SdcDisableTimingContainerPtr disable_timing_container_;
    SdcFalsePathContainerPtr false_path_container_;
    SdcIdealLatencyContainerPtr ideal_latency_container_;
    SdcIdealNetworkContainerPtr ideal_network_container_;
    SdcIdealTransitionContainerPtr ideal_transition_container_;
    SdcInputDelayContainerPtr input_delay_container_;
    SdcMaxDelayContainerPtr max_delay_container_;
    SdcMaxTimeBorrowContainerPtr max_time_borrow_container_;
    SdcMinDelayContainerPtr min_delay_container_;
    SdcMinPulseWidthContainerPtr min_pulse_width_container_;
    SdcMulticyclePathContainerPtr multicycle_path_container_;
    SdcOutputDelayContainerPtr output_delay_container_;
    SdcPropagatedClockContainerPtr propagated_clock_container_;

    SdcCaseAnalysisContainerPtr case_analysis_container_;
    SdcDriveContainerPtr drive_container_;
    SdcDrivingCellContainerPtr driving_cell_container_;
    SdcFanoutLoadContainerPtr fanout_load_container_;
    SdcInputTransitionContainerPtr input_transition_container_;
    SdcLoadContainerPtr load_container_;
    SdcLogicContainerPtr logic_container_;
    SdcMaxAreaContainerPtr max_area_container_;
    SdcMaxFanoutContainerPtr max_fanout_container_;
    SdcMaxTransitionContainerPtr max_transition_container_;
    SdcMinCapacitanceContainerPtr min_capacitance_container_;
    SdcOperatingConditionsContainerPtr operating_conditions_container_;
    SdcPortFanoutNumberContainerPtr port_fanout_number_container_;
    SdcResistanceContainerPtr resistance_container_;
    SdcTimingDerateContainerPtr timing_derate_container_;
    SdcVoltageContainerPtr voltage_container_;
    SdcWireLoadMinBlockSizeContainerPtr wire_load_min_block_size_container_;
    SdcWireLoadModeContainerPtr wire_load_mode_container_;
    SdcWireLoadModelContainerPtr wire_load_model_container_;
    SdcWireLoadSelectionGroupContainerPtr wire_load_selection_group_container_;

    SdcVoltageAreaContainerPtr voltage_area_container_;
    SdcLevelShifterStrategyContainerPtr level_shifter_strategy_container_;
    SdcLevelShifterThresholdContainerPtr level_shifter_threshold_container_;
    SdcMaxDynamicPowerContainerPtr max_dynamic_power_container_;

    SdcAllClocksContainerPtr all_clocks_container_;
    SdcAllInputsContainerPtr all_inputs_container_;
    SdcAllRegistersContainerPtr all_registers_container_;
    SdcCurrentDesignContainerPtr design_container_;
    SdcGetCellsContainerPtr get_cells_container_;
    SdcGetClocksContainerPtr get_clocks_container_;
    SdcGetLibCellsContainerPtr get_lib_cells_container_;
    SdcGetLibPinsContainerPtr get_lib_pins_container_;
    SdcGetLibsContainerPtr get_libs_container_;
    SdcGetNetsContainerPtr get_nets_container_;
    SdcGetPinsContainerPtr get_pins_container_;
    SdcGetPortsContainerPtr get_ports_container_;

  private:
    ObjectId analysis_mode_id_;
    ObjectId analysis_view_id_;
    ObjectId analysis_corner_id_;
    ObjectId main_liberty_id_;
    ObjectId main_liberty_units_id_;
};
using SdcPtr = std::shared_ptr<Sdc>;

}
}


#endif // EDI_DB_TIMING_SDC_SDC_H_
