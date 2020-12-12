/** 
 * @file sdc_data.h
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

#ifndef EDI_DB_TIMING_SDC_SDC_DATA_H_
#define EDI_DB_TIMING_SDC_SDC_DATA_H_

#include "db/timing/sdc/general_purpose_commands.h"
#include "db/timing/sdc/object_access_commands.h"
#include "db/timing/sdc/timing_constraints.h"
#include "db/timing/sdc/environment_commands.h"
#include "db/timing/sdc/multivoltage_power_commands.h"
#include "db/core/db.h"

#include <set>

namespace open_edi {
namespace db {

//general purpose commands
class SdcCurrentInstanceContainer {
  public:
    SdcCurrentInstanceContainer() { current_instance_ = std::make_shared<CurrentInstance>(); }

    void add(CurrentInstancePtr instance) { current_instance_ = instance; }
    const std::string getName() const;
    const ObjectId getId() const { return current_instance_->getInstanceId(); };
    friend std::ostream &operator<<(std::ostream &os, SdcCurrentInstanceContainer &rhs);

  private:
    CurrentInstancePtr current_instance_;
};
using SdcCurrentInstanceContainerPtr = std::shared_ptr<SdcCurrentInstanceContainer>;

class SdcHierarchySeparatorContainer {
  public:
    SdcHierarchySeparatorContainer() { separator_ = std::make_shared<SetHierarchySeparator>(); }

    void add(SetHierarchySeparatorPtr separator) { separator_ = separator; }
    const char get() const { return separator_->getSeparator(); };
    friend std::ostream &operator<<(std::ostream &os, SdcHierarchySeparatorContainer &rhs);

  private:
    SetHierarchySeparatorPtr separator_;
};
using SdcHierarchySeparatorContainerPtr = std::shared_ptr<SdcHierarchySeparatorContainer>;

class SdcUnitsContainer {
  public:
    SdcUnitsContainer() { units_ = std::make_shared<SetUnits>(); }

    void add(SetUnitsPtr units) { units_ = units; };
    const float &getSdcCapacitanceUnits() const { return units_->getCapacitanceUnit(); };
    const float &getSdcResistanceUnits() const { return units_->getResistanceUnit(); };
    const float &getSdcTimeUnits() const { return units_->getTimeUnit(); };
    const float &getSdcVoltageUnits() const { return units_->getVoltageUnit(); };
    const float &getSdcCurrentUnits() const { return units_->getCurrentUnit(); };
    const float &getSdcPowerUnits() const { return units_->getPowerUnit(); };
    friend std::ostream &operator<<(std::ostream &os, SdcUnitsContainer &rhs);

  private:
    SetUnitsPtr units_;
};
using SdcUnitsContainerPtr = std::shared_ptr<SdcUnitsContainer>;

//object access commands
class SdcAllClocksContainer {
  private:
    AllClocksPtr all_locks_;
};
using SdcAllClocksContainerPtr = std::shared_ptr<SdcAllClocksContainer>;

class SdcAllInputsContainer {
  public:
    std::vector<std::string> get_all_inputs(AllInputsPtr conditions);
};
using SdcAllInputsContainerPtr = std::shared_ptr<SdcAllInputsContainer>;

class SdcAllOutputsContainer {
  private:
    std::vector<std::string> get_all_outputs(AllOutputsPtr conditions);
};
using SdcAllOutputsContainerPtr = std::shared_ptr<SdcAllOutputsContainer>;

class SdcAllRegistersContainer {
  public:
    std::vector<std::string> get_all_register_pins(AllRegistersPtr conditions);
    std::vector<std::string> get_all_register_cells(AllRegistersPtr conditions);
};
using SdcAllRegistersContainerPtr = std::shared_ptr<SdcAllRegistersContainer>;

class SdcCurrentDesignContainer {
  public:
    SdcCurrentDesignContainer() { current_design_ = std::make_shared<CurrentDesign>(); }

    void add(CurrentDesignPtr design) { current_design_ = design; }
    std::string getName();
    ObjectId getId() { return current_design_->getCellId(); };
    friend std::ostream &operator<<(std::ostream &os, SdcCurrentDesignContainer &rhs);

  private:
    CurrentDesignPtr current_design_;
};
using SdcCurrentDesignContainerPtr = std::shared_ptr<SdcCurrentDesignContainer>;

class SdcGetCellsContainer {
  public:
    std::vector<ObjectId> get_cells_ids(GetCellsPtr conditions);
    std::vector<std::string> get_cells_names(GetCellsPtr conditions);
};
using SdcGetCellContainerPtr = std::shared_ptr<SdcGetCellsContainer>;

class SdcGetClocksContainer {
  public:
    std::vector<ClockId> get_clocks_ids(GetClocksPtr conditions);
    std::vector<std::string> get_clocks_names(GetClocksPtr conditions);
};
using SdcGetClocksContainerPtr = std::shared_ptr<SdcGetClocksContainer>;

class SdcGetLibCellsContainer {
  public:
    std::vector<ObjectId> get_lib_cells_ids(GetLibCellsPtr conditions);
    std::vector<std::string> get_lib_cells_names(GetLibCellsPtr conditions);
};
using SdcGetLibCellsContainerPtr = std::shared_ptr<SdcGetLibCellsContainer>;

class SdcGetLibPinsContainer {
  public:
    std::vector<ObjectId> get_lib_pins_ids(GetLibPinsPtr conditions);
    std::vector<std::string> get_lib_pins_names(GetLibPinsPtr conditions);
};
using SdcGetLibPinsContainerPtr = std::shared_ptr<SdcGetLibPinsContainer>;

class SdcGetLibsContainer {
  public:
    std::vector<ObjectId> get_libs_ids(GetLibsPtr conditions);
    std::vector<std::string> get_libs_names(GetLibsPtr conditions);
};
using SdcGetLibsContainerPtr = std::shared_ptr<SdcGetLibsContainer>;

class SdcGetNetsContainer {
  public:
    std::vector<ObjectId> get_nets_ids(GetNetsPtr conditions);
    std::vector<std::string> get_nets_names(GetNetsPtr conditions);
};
using SdcGetNetsContainerPtr = std::shared_ptr<SdcGetNetsContainer>;

class SdcGetPinsContainer {
  public:
    std::vector<ObjectId> get_pins_ids(GetPinsPtr conditions);
    std::vector<std::string> get_pins_names(GetPinsPtr conditions);
};
using SdcGetPinsContainerPtr = std::shared_ptr<SdcGetPinsContainer>;

class SdcGetPortsContainer {
  public:
    std::vector<ObjectId> get_ports_ids(GetPortsPtr conditions);
    std::vector<std::string> get_ports_names(GetPortsPtr conditions);
}; 
using SdcGetPortsContainerPtr = std::shared_ptr<SdcGetPortsContainer>;

//timing constraint commands
class SdcClockContainer {
  public:
    SdcClockContainer() { data_ = std::make_shared<ClockContainerData>(); }

  public:
    const std::vector<ClockPtr> &getClocks() { return data_->getClocks(); }
    const std::vector<ClockId> &getClockIds() { return data_->getClockIds(); } 
    const std::vector<std::string> &getClockNames() { return data_->getClockNames(); }

    const std::string getClockName(const ClockId &id);
    const ClockId getClockId(const std::string &name);

    const ClockPtr getClock(const ClockId &id);
    const ClockPtr getClock(const std::string &name);

    const ClockPtr getClockOnPin(const ObjectId &pin_id);
    bool isClockPin(const ObjectId &pin_id);

    friend std::ostream &operator<<(std::ostream &os, SdcClockContainer &rhs);

  private:
    ClockContainerDataPtr data_;
};
using SdcClockContainerPtr = std::shared_ptr<SdcClockContainer>;

class SdcGroupPathContainer {
  public:
    void add(const GroupPathPtr &path) { paths_.emplace_back(path); }
    const std::vector<GroupPathPtr> &get() { return paths_; }

  private:
    std::vector<GroupPathPtr> paths_;
};
using SdcGroupPathContinerPtr = std::shared_ptr<SdcGroupPathContainer>;

class SdcClockGatingCheckContainer {
  public:
    void addToPin(const ObjectId pin_id, const SetClockGatingCheckPtr &check) { pin_clock_gating_check_.emplace(pin_id, check); }
    void addToInstance(const ObjectId inst_id, const SetClockGatingCheckPtr &check) { inst_clock_gating_check_.emplace(inst_id, check); }
    void addToClock(const ClockId clock_id, const SetClockGatingCheckPtr &check) { clock_to_gating_check_.emplace(clock_id, check); }

  private:
    std::unordered_map<ObjectId, SetClockGatingCheckPtr> pin_clock_gating_check_;
    std::unordered_map<ObjectId, SetClockGatingCheckPtr> inst_clock_gating_check_;
    std::unordered_map<ClockId, SetClockGatingCheckPtr> clock_to_gating_check_; 
};
using SdcClockGatingCheckContainerPtr = std::shared_ptr<SdcClockGatingCheckContainer>;

//TODO change the data structure
/*
class SdcClockGroupsContainer {
  public:
    void add(const SetClocKGroups &groups);

  private:
    std::vector<ClockGroupId> clock_groups_;
    std::unordered_map<ClockId, std::vector<ClockGroupId> > clock_in_groups_;
    std::unordered_map<ClockGroupId, ClockGroupId> physically_exclusive_groups_;
};
using SdcClockGroupsContainerPtr = std::shared_ptr<SdcClockGroupsContainer>; 
*/

class SdcClockLatencyContainer {
  public:
    void add(const SetClockLatency &latency);

  private:
    std::unordered_map<ObjectId, std::vector<SetClockLatency> > pin_clock_latency_;
    std::unordered_map<ClockId, std::vector<SetClockLatency> > clock_latency_;
};
using SdcClockLatencyContainerPtr = std::shared_ptr<SdcClockLatencyContainer>;

class SdcSenceContainer {

  private:
    std::unordered_map<ClockPinPairPtr, SetSense> clock_pin_sense_; 
};
using SdcSenceContainerPtr = std::shared_ptr<SdcSenceContainer>;

class SdcClockTransitionContainer {

  private:
    std::unordered_map<ClockId, SetClockTransitionPtr> clock_transitions_;
};
using SdcClockTransitionContainerPtr = std::shared_ptr<SdcClockTransitionContainer>;

class SdcClockUncertaintyContainer {

  private:
    std::unordered_map<ClockPairPtr, SetClockUncertaintyPtr> clock_to_clock_uncertainty_;
    std::unordered_map<ObjectId, SetClockUncertaintyPtr> pin_to_uncertainty_;
};
using SdcClockUncertaintyContainerPtr = std::shared_ptr<SdcClockUncertaintyContainer>;

class SdcDataCheckContainer {

  private:
    std::unordered_map<PinPairPtr, SetDataCheckPtr> pin_to_pin_data_check_;
};
using SdcDataCheckContainerPtr = std::shared_ptr<SdcDataCheckContainer>;

class SdcDisableTimingContinaer {
  public:
    void add(const SetDisableTiming &disable_timing);

  private:
    std::unordered_map<ObjectId, ObjectId> pin_to_pin_;
    std::unordered_map<ObjectId, ObjectId> term_to_term_;
};
using SdcDisableTimingContainerPtr = std::shared_ptr<SdcDisableTimingContinaer>;

class SdcFalsePathContainer {
  public:
    void add(const SetFalsePath& path);
  private:
    std::vector<SetFalsePathPtr> false_paths_;
};
using SdcFalsePathContainerPtr = std::shared_ptr<SdcFalsePathContainer>; 

class SdcIdealLatencyContainer {

  private:
    std::vector<ObjectId> pins_;
}; 
using SdcIdealLatencyContainerPtr = std::shared_ptr<SdcIdealLatencyContainer>;

class SdcIdealNetworkContainer {
  public:
    void add(SetIdealNetwork& network);

  private:
    std::vector<ObjectId> pins_;
    std::vector<ObjectId> nets_;
}; 
using SdcIdealNetworkContainerPtr = std::shared_ptr<SdcIdealNetworkContainer>;

class SdcIdealTransitionContainer {

  private:
    std::unordered_map<ObjectId, SetIdealTransitionPtr> pin_ideal_transitions_;
}; 
using SdcIdealTransitionContainerPtr = std::shared_ptr<SdcIdealTransitionContainer>;

class SdcInputDelayContainer {

  private:
    std::unordered_map<PinPairPtr, SetInputDelayPtr> pin_input_delays_;
};
using SdcInputDelayContainerPtr = std::shared_ptr<SdcInputDelayContainer>;

class SdcMaxDelayContainer {

  private:
    std::vector<SetMaxDelay> max_delays_;
};
using SdcMaxDelayContainerPtr = std::shared_ptr<SdcMaxDelayContainer>;

class SdcMaxTimeBorrowContainer {

  private:
    std::unordered_map<ObjectId, SetMaxTimeBorrowPtr> pin_max_time_borrow_;
    std::unordered_map<ClockId, SetMaxTimeBorrowPtr> clock_max_time_borrow_;
};
using SdcMaxTimeBorrowContainerPtr = std::shared_ptr<SdcMaxTimeBorrowContainer>;

class SdcMinPulseWidthContainer {

  private:
    std::unordered_map<ObjectId, SetMinPulseWidthPtr> pin_min_pulse_width_;
    std::unordered_map<ClockId, SetMinPulseWidthPtr> clock_min_pulse_width_;
};
using SdcMinPulseWidthContainerPtr = std::shared_ptr<SdcMinPulseWidthContainer>;

class SdcMulticyclePathContainer {

  private:
    std::vector<SetMulticyclePathPtr> multi_cycle_paths_;
};
using SdcMulticyclePathContainerPtr = std::shared_ptr<SdcMulticyclePathContainer>;

class SdcOutputDelayContainer {

  private:
    std::unordered_map<PinPairPtr, SetOutputDelayPtr> pin_output_delays_;
};
using SdcOutputDelayContainerPtr = std::shared_ptr<SdcOutputDelayContainer>;

class SdcPropagatedClockContainer {
  private:
    std::set<ObjectId> pins_;
    std::set<ClockId> clocks_;
};
using SdcPropagatedClockContainerPtr = std::shared_ptr<SdcPropagatedClockContainer>;


//environment commands
using PinCaseAnalysisMap = std::unordered_map<ObjectId, SetCaseAnalysisPtr>;
class SdcCaseAnalysisContainer {
  public:
    void add(const ObjectId &pin_id, const SetCaseAnalysisPtr &case_analysis_ptr);
    const PinCaseAnalysisMap& get() {return pin_to_case_analysis_;}

    CaseAnalysisValue getPinValue(const ObjectId &pin_id);
    void resetPinValue(const ObjectId &pin_id, const CaseAnalysisValue &value);

    bool isPinConstZero(const ObjectId &pin_id);
    bool isPinConstOne(const ObjectId &pin_id);

    friend std::ostream &operator<<(std::ostream &os, SdcCaseAnalysisContainer &rhs);

  private:
    PinCaseAnalysisMap pin_to_case_analysis_;
};
using SdcCaseAnalysisContainerPtr = std::shared_ptr<SdcCaseAnalysisContainer>;

class SdcDriveContainer {
  private:
    std::unordered_map<ObjectId, SetDrivePtr> pin_drives_;
};
using SdcDriveContainerPtr = std::shared_ptr<SdcDriveContainer>;

class SdcDrivingCellContainer {
  private:
    std::unordered_map<ObjectId, SetDrivingCellPtr> port_driving_cell_;
};
using SdcDrivingCellContainerPtr = std::shared_ptr<SdcDrivingCellContainer>;

class SdcFanoutLoadContainer {
  private:
    std::unordered_map<ObjectId, SetFanoutLoadPtr> pin_fanout_load_;
};
using SdcFanoutLoadContainerPtr = std::shared_ptr<SdcFanoutLoadContainer>;

class SdcInputTransitionContainer {
  private:
    std::unordered_map<ObjectId, SetInputTransitionPtr> port_input_transitions_;
};
using SdcInputTransitionContainerPtr = std::shared_ptr<SdcInputTransitionContainer>;

class SdcLoadContainer {

  private:
    std::unordered_map<ObjectId, SetLoadPtr> pin_load_caps_;
    std::unordered_map<ObjectId, SetLoadPtr> net_load_caps_;
};
using SdcLoadContainerPtr = std::shared_ptr<SdcLoadContainer>;

class SdcLogicContainer {

  private:
    std::unordered_map<ObjectId, SetLogicPtr> port_logic_value_;
};
using SdcLogicContainerPtr = std::shared_ptr<SdcLogicContainer>;

class SdcMaxAreaContainer : public SetMaxArea {

};
using SdcMaxAreaContainerPtr = std::shared_ptr<SdcMaxAreaContainer>;

class SdcMaxCapacitanceContainer {
  private:
    std::unordered_map<ObjectId, SetMaxCapacitancePtr> pin_max_caps_; 
    std::unordered_map<ObjectId, SetMaxCapacitancePtr> cell_max_caps_; 
};
using SdcMaxCapacitanceContainerPtr = std::shared_ptr<SdcMaxCapacitanceContainer>;

class SdcMaxFanoutContainer {
  private:
    std::unordered_map<ObjectId, SetMaxFanoutPtr> pin_max_fanout_;
    std::unordered_map<ObjectId, SetMaxFanoutPtr> cell_max_fanout_;
};
using SdcMaxFanoutContainerPtr = std::shared_ptr<SdcMaxFanoutContainer>;

class SdcMaxTransitionContainer {
  private:
    std::unordered_map<ClockId, SetMaxTransitionPtr> clock_max_transition_;
    std::unordered_map<ObjectId, SetMaxTransitionPtr> cell_max_transition_;
    std::unordered_map<ObjectId, SetMaxTransitionPtr> pin_max_transition_;
};
using SdcMaxTransitionContainerPtr = std::shared_ptr<SdcMaxTransitionContainer>;

class SdcMinCapacitanceContainer {
  private:
    std::unordered_map<ObjectId, SetMinCapacitancePtr> pin_min_caps_; 
    std::unordered_map<ObjectId, SetMinCapacitancePtr> cell_min_caps_; 
};
using SdcMinCapacitanceContainerPtr = std::shared_ptr<SdcMinCapacitanceContainer>;

class SdcOperatingConditionsContainer {
  private:
    std::vector<SetOperatingConditionsPtr> operate_conditions_;
};
using SdcOperatingConditionsContainerPtr = std::shared_ptr<SdcOperatingConditionsContainer>;

class SdcPortFanoutNumberContainer {
  private:
    std::unordered_map<ObjectId, SetPortFanoutNumberPtr> port_fanout_number_;
}; 
using SdcPortFanoutNumberContainerPtr = std::shared_ptr<SdcPortFanoutNumberContainer>;

class SdcResistanceContainer {
  private:
    std::unordered_map<ObjectId, SetResistancePtr> net_resistance_;
};
using SdcResistanceContainerPtr = std::shared_ptr<SdcResistanceContainer>;

class SdcTimingDerateContainer {
  private:
    std::unordered_map<ObjectId, SetTimingDeratePtr> net_timing_derate_;
    std::unordered_map<ObjectId, SetTimingDeratePtr> inst_timing_derate_;
    std::unordered_map<ObjectId, SetTimingDeratePtr> cell_timing_derate_;
};
using SdcTimingDerateContainerPtr = std::shared_ptr<SdcTimingDerateContainer>;

class SdcVoltageContainer {
  private:
    std::unordered_map<ObjectId, SetVoltagePtr> net_voltage_;
};
using SdcVoltageContainerPtr = std::shared_ptr<SdcVoltageContainer>;

class SdcWireLoadModeContainer {
  private:
    std::unordered_map<ObjectId, SetWireLoadModePtr> cell_wire_load_mode_;
};
using SdcWireLoadModeContainerPtr = std::shared_ptr<SdcWireLoadModeContainer>; 

class SdcWireLoadModelContainer {
  private:
    std::unordered_map<ObjectId, SetWireLoadModelPtr> cell_wire_load_model_;
    std::unordered_map<ObjectId, SetWireLoadModelPtr> port_wire_load_model_;
};
using SdcWireLoadModelContainerPtr = std::shared_ptr<SdcWireLoadModelContainer>;

class SdcWireLoadSelectionGroupContainer {
  private:
    std::vector<SetWireLoadSelectionGroup> selections_;
};
using SdcWireLoadSelectionGroupContainerPtr= std::shared_ptr<SdcWireLoadSelectionGroupContainer>;


//multivoltage power commands
class SdcVoltageAreaContainer {
  public:
    SdcVoltageAreaContainer() { data_ = std::make_shared<VoltageAreaContainerData>(); }

  public:
    //TODO get origin data functions
    const CreateVoltageAreaPtr getCellVoltageArea(const ObjectId &id) const;

    friend std::ostream &operator<<(std::ostream &os, SdcVoltageAreaContainer &rhs);

  private:
    VoltageAreaContainerDataPtr data_;
};
using SdcVoltageAreaContainerPtr = std::shared_ptr<SdcVoltageAreaContainer>;

class SdcLevelShifterStrategyContainer {
  public:
    void add(const SetLevelShifterStrategy &data) { data_ = data; }
    const LevelShifterStrategy &getLevelShifterStrategy() const { return data_.getRule(); }
    friend std::ostream &operator<<(std::ostream &os, SdcLevelShifterStrategyContainer &rhs);

  private:
    SetLevelShifterStrategy data_;
};
using SdcLevelShifterStrategyContainerPtr = std::shared_ptr<SdcLevelShifterStrategyContainer>;

class SdcLevelShifterThresholdContainer {
  public:
    SdcLevelShifterThresholdContainer() { data_ = std::make_shared<SetLevelShifterThreshold>(); }

  public:
    void add(const SetLevelShifterThresholdPtr &data) { data_ = data; }
    const float &getVoltage() const { return data_->getVoltage(); }
    const float &getPercent() const { return data_->getPercent(); } 
    friend std::ostream &operator<<(std::ostream &os, SdcLevelShifterThresholdContainer &rhs);

  private:
    SetLevelShifterThresholdPtr data_;
}; 
using SdcLevelShifterThresholdContainerPtr = std::shared_ptr<SdcLevelShifterThresholdContainer>;

class SdcMaxDynamicPowerContainer {
  public:
    SdcMaxDynamicPowerContainer() { data_ = std::make_shared<MaxDynamicPowerContainerData>(); }

  public:
    void add(const MaxDynamicPowerContainerDataPtr &data) { data_ = data; }
    const float getCellPower(const ObjectId &cell_id) const;

    friend std::ostream &operator<<(std::ostream &os, SdcMaxDynamicPowerContainer &rhs);

  private:
    MaxDynamicPowerContainerDataPtr data_;
};
using SdcMaxDynamicPowerContainerPtr = std::shared_ptr<SdcMaxDynamicPowerContainer>; 

class SdcMaxLeakagePowerContainer {
  private:
    std::unordered_map<ObjectId, SetMaxLeakagePowerPtr> leakage_power_;
};
using SdcMaxLeakagePowerContainerPtr = std::shared_ptr<SdcMaxLeakagePowerContainer>;


}
}
#endif //EDI_DB_TIMING_SDC_SDC_DATA_H_ 
