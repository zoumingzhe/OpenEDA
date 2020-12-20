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
    SdcCurrentInstanceContainer() { data_ = std::make_shared<CurrentInstance>(); }

    void setData(const CurrentInstancePtr &data) { data_ = data; };
    const CurrentInstancePtr getData() const { return data_; };
    CurrentInstancePtr getData() { return data_; };
    const std::string getInstName() const;
    const ObjectId getInstId() const { return data_->getInstId(); };
    friend std::ostream &operator<<(std::ostream &os, SdcCurrentInstanceContainer &rhs);

  private:
    CurrentInstancePtr data_;
};
using SdcCurrentInstanceContainerPtr = std::shared_ptr<SdcCurrentInstanceContainer>;

class SdcHierarchySeparatorContainer {
  public:
    SdcHierarchySeparatorContainer() { data_ = std::make_shared<SetHierarchySeparator>(); }

    void setData(const SetHierarchySeparatorPtr &data) { data_ = data; }
    const SetHierarchySeparatorPtr getData() const { return data_; }
    SetHierarchySeparatorPtr getData() { return data_; }
    const char get() const { return data_->getSeparator(); }
    friend std::ostream &operator<<(std::ostream &os, SdcHierarchySeparatorContainer &rhs);

  private:
    SetHierarchySeparatorPtr data_;
};
using SdcHierarchySeparatorContainerPtr = std::shared_ptr<SdcHierarchySeparatorContainer>;

class SdcUnitsContainer {
  public:
    SdcUnitsContainer() { data_ = std::make_shared<SetUnits>(); }
    void setData(const SetUnitsPtr &data) { data_ = data; }
    const SetUnitsPtr getData() const { return data_; }
    SetUnitsPtr getData() { return data_; }

    const float &getSdcCapacitanceUnits() const { return data_->getCapacitanceUnit(); };
    const float &getSdcResistanceUnits() const { return data_->getResistanceUnit(); };
    const float &getSdcTimeUnits() const { return data_->getTimeUnit(); };
    const float &getSdcVoltageUnits() const { return data_->getVoltageUnit(); };
    const float &getSdcCurrentUnits() const { return data_->getCurrentUnit(); };
    const float &getSdcPowerUnits() const { return data_->getPowerUnit(); };
    friend std::ostream &operator<<(std::ostream &os, SdcUnitsContainer &rhs);

  private:
    SetUnitsPtr data_;
};
using SdcUnitsContainerPtr = std::shared_ptr<SdcUnitsContainer>;

//timing constraint commands
class SdcClockContainer {
  public:
    SdcClockContainer() { data_ = std::make_shared<ClockContainerData>(); }

    void setData(const ClockContainerDataPtr &data) { data_ = data; };
    const ClockContainerDataPtr getData() const { return data_; };
    ClockContainerDataPtr getData() { return data_; };
    const std::vector<ClockPtr> &getClocks() { return data_->getClocks(); }
    const std::vector<ClockId> &getClockIds() { return data_->getClockIds(); } 
    const std::vector<std::string> &getClockNames() { return data_->getClockNames(); }

    const std::string getClockName(const ClockId &id);
    const ClockId getClockId(const std::string &name);
    const ClockPtr getClock(const ClockId &id);
    const ClockPtr getClock(const std::string &name);
    bool getClockOnPin(std::vector<ClockPtr> &clocks, const ObjectId &pin_id);
    bool isClockPin(const ObjectId &pin_id);

    friend std::ostream &operator<<(std::ostream &os, SdcClockContainer &rhs);

  private:
    ClockContainerDataPtr data_;
};
using SdcClockContainerPtr = std::shared_ptr<SdcClockContainer>;

class SdcGroupPathContainer {
  public:
    SdcGroupPathContainer() { data_ = std::make_shared<GroupPathContainerData>(); }

    void setData(const GroupPathContainerDataPtr &data) { data_ = data; };
    const GroupPathContainerDataPtr getData() const { return data_; };
    GroupPathContainerDataPtr getData() { return data_; };
    const std::vector<GroupPathPtr> &get() const { return data_->getGroupPaths(); }
    friend std::ostream &operator<<(std::ostream &os, SdcGroupPathContainer &rhs);

  private:
    GroupPathContainerDataPtr data_;
};
using SdcGroupPathContainerPtr = std::shared_ptr<SdcGroupPathContainer>;

class SdcClockGatingCheckContainer {
  public:
    SdcClockGatingCheckContainer() { data_ = std::make_shared<ClockGatingCheckContainerData>(); }

  public:
    void add(const ClockGatingCheckContainerDataPtr &data) { data_ = data; }
    const SetClockGatingCheckPtr getClockGatingCheckOnPin(const ObjectId &pin_id) const;
    const SetClockGatingCheckPtr getClockGatingCheckOnCell(const ObjectId &cell_id) const;
    const SetClockGatingCheckPtr getClockGatingCheckOnClock(const ClockId &clock_id) const;
    const float getPinCheckTime(bool is_rise, bool is_high, bool is_setup, const ObjectId &pin_id) const;
    const float getCellCheckTime(bool is_rise, bool is_high, bool is_setup, const ObjectId &cell_id) const;
    const float getClockCheckTime(bool is_rise, bool is_high, bool is_setup, const ClockId &clock_id) const;

    friend std::ostream &operator<<(std::ostream &os, SdcClockGatingCheckContainer &rhs);

  private:
    ClockGatingCheckContainerDataPtr data_;
};
using SdcClockGatingCheckContainerPtr = std::shared_ptr<SdcClockGatingCheckContainer>;

class SdcClockGroupsContainer {
  public:
    SdcClockGroupsContainer() { data_ = std::make_shared<ClockGroupsContainerData>(); }

  public:
    void add(const ClockGroupsContainerDataPtr &data) { data_ = data; };
    void findClocks(std::vector<ClockId> &clocks, const ClockId &clock_id, const GroupRelationshipType &type) const;
    void getClocksRelationshipType(std::vector<GroupRelationshipType> &types, const ClockId &clock_id, const ClockId &other_clock_id) const;

    friend std::ostream &operator<<(std::ostream &os, SdcClockGatingCheckContainer &rhs);

  private:
    ClockGroupsContainerDataPtr data_;
};
using SdcClockGroupsContainerPtr = std::shared_ptr<SdcClockGroupsContainer>; 

class SdcClockLatencyContainer {
  public:
    SdcClockLatencyContainer() { data_ = std::make_shared<ClockLatencyContainerData>(); }

  public:
    void add(const ClockLatencyContainerDataPtr &data) { data_ = data; };
    void getClockLatencyOnPin(std::vector<ClockLatencyOnPinPtr> &latencys, const ObjectId &pin_id) const; 
    void getClockLatencyOnClock(std::vector<SetClockLatencyPtr> &latencys, const ClockId &clock_id) const; 

    friend std::ostream &operator<<(std::ostream &os, SdcClockLatencyContainer &rhs);

  private:
    ClockLatencyContainerDataPtr data_;
};
using SdcClockLatencyContainerPtr = std::shared_ptr<SdcClockLatencyContainer>;

class SdcSenceContainer {
  public:
    SdcSenceContainer() { data_ = std::make_shared<SenseContainerData>(); }

  public:
    void add(const SenseContainerDataPtr& data) { data_ = data; }
    const SetSensePtr getSense(const ObjectId &pin_id) const;

    friend std::ostream &operator<<(std::ostream &os, SdcSenceContainer &rhs);

  private:
    SenseContainerDataPtr data_;
};
using SdcSenceContainerPtr = std::shared_ptr<SdcSenceContainer>;

class SdcClockTransitionContainer {
  public:
    SdcClockTransitionContainer() { data_ = std::make_shared<ClockTransitionContainerData>(); }

  public:
    void add(const ClockTransitionContainerDataPtr &data) { data_ = data; }
    const SetClockTransitionPtr getTransition(const ClockId &clock_id) const;

    friend std::ostream &operator<<(std::ostream &os, SdcClockTransitionContainer &rhs);

  private:
    ClockTransitionContainerDataPtr data_;
};
using SdcClockTransitionContainerPtr = std::shared_ptr<SdcClockTransitionContainer>;

class SdcClockUncertaintyContainer {
  public:
    SdcClockUncertaintyContainer() { data_ = std::make_shared<ClockUncertaintyContainerData>(); }

  public:
    void add(const ClockUncertaintyContainerDataPtr &data) { data_ = data; }
    const InterClockUncertaintyPtr getUncertaintyOnInterClocks(const ClockId &from_clock_id, const ClockId &to_clock_id) const;
    const SetClockUncertaintyPtr getUncertaintyOnPin(const ObjectId &pin_id) const;
    const SetClockUncertaintyPtr getUncertaintyOnClock(const ClockId &clock_id) const;

    friend std::ostream &operator<<(std::ostream &os, SdcClockUncertaintyContainer &rhs);

  private:
    ClockUncertaintyContainerDataPtr data_;
};
using SdcClockUncertaintyContainerPtr = std::shared_ptr<SdcClockUncertaintyContainer>;

class SdcDataCheckContainer {
  public:
    SdcDataCheckContainer() { data_ = std::make_shared<DataCheckContainerData>(); }

  public:
    void add(const DataCheckContainerDataPtr &data) { data_ = data; }
    const SetDataCheckPtr getDataCheckOnInterPins(const ObjectId &from_pin_id, const ObjectId &to_pin_id) const;

    friend std::ostream &operator<<(std::ostream &os, SdcDataCheckContainer &rhs);

  private:
    DataCheckContainerDataPtr data_;
};
using SdcDataCheckContainerPtr = std::shared_ptr<SdcDataCheckContainer>;

class SdcDisableTimingContainer {
  public:
    SdcDisableTimingContainer() { data_ = std::make_shared<DisableTimingContainerData>(); }

  public:
    void add(const DisableTimingContainerDataPtr &data) { data_ = data; }
    bool hasDisableTiming(const ObjectId &from_pin_id, const ObjectId &to_pin_id) const;
    bool hasDisableTimingArcInInst(const ObjectId &inst_id) const;

    friend std::ostream &operator<<(std::ostream &os, SdcDisableTimingContainer &rhs);

  private:
    DisableTimingContainerDataPtr data_;
};
using SdcDisableTimingContainerPtr = std::shared_ptr<SdcDisableTimingContainer>;

class SdcFalsePathContainer {
  public:
    SdcFalsePathContainer() { data_ = std::make_shared<FalsePathContainerData>(); }

  public:
    void add(const FalsePathContainerDataPtr &data) { data_ = data; }
    void get(std::vector<SetFalsePathPtr> &paths) { paths = data_->getFalsePaths(); }

    friend std::ostream &operator<<(std::ostream &os, SdcFalsePathContainer &rhs);

  private:
    FalsePathContainerDataPtr data_;
};
using SdcFalsePathContainerPtr = std::shared_ptr<SdcFalsePathContainer>; 

class SdcIdealLatencyContainer {
  public:
    SdcIdealLatencyContainer() { data_ = std::make_shared<IdealLatencyContainerData>(); }

  public:
    void add(const IdealLatencyContainerDataPtr &data) { data_ = data; } 
    const SetIdealLatencyPtr getPinLatency(const ObjectId &pin_id) const;

    friend std::ostream &operator<<(std::ostream &os, SdcIdealLatencyContainer &rhs);

  private:
    IdealLatencyContainerDataPtr data_;
}; 
using SdcIdealLatencyContainerPtr = std::shared_ptr<SdcIdealLatencyContainer>;

class SdcIdealNetworkContainer {
  public:
    SdcIdealNetworkContainer() { data_ = std::make_shared<SetIdealNetwork>(); }

  public:
    void add(const SetIdealNetworkPtr& data) { data_ = data; }
    //TODO
    bool isPinIdealSource(const ObjectId &pin_id) const;

    friend std::ostream &operator<<(std::ostream &os, SdcIdealNetworkContainer &rhs);

  private:
    SetIdealNetworkPtr data_;
};
using SdcIdealNetworkContainerPtr = std::shared_ptr<SdcIdealNetworkContainer>;

class SdcIdealTransitionContainer {
  public:
    SdcIdealTransitionContainer() { data_ = std::make_shared<IdealTransitionContainerData>(); }

  public:
    void add(const IdealTransitionContainerDataPtr &data) { data_ = data; }
    const SetIdealTransitionPtr getPinTransition(const ObjectId &pin_id) const;

    friend std::ostream &operator<<(std::ostream &os, SdcIdealTransitionContainer &rhs);

  private:
    IdealTransitionContainerDataPtr data_;
};
using SdcIdealTransitionContainerPtr = std::shared_ptr<SdcIdealTransitionContainer>;

class SdcInputDelayContainer {
  public:
    SdcInputDelayContainer() { data_ = std::make_shared<InputDelayContainerData>(); }

  public:
    void add(const InputDelayContainerDataPtr &data) { data_ = data; }
    const SetInputDelayPtr getPinInputDelay(const ObjectId &pin_id) const;

    friend std::ostream &operator<<(std::ostream &os, SdcInputDelayContainer &rhs);

  private:
    InputDelayContainerDataPtr data_;
};
using SdcInputDelayContainerPtr = std::shared_ptr<SdcInputDelayContainer>;

class SdcMaxDelayContainer {
  public:
    SdcMaxDelayContainer() { data_ = std::make_shared<MaxDelayContainerData>(); }

  public:
    void add(const MaxDelayContainerDataPtr &data) { data_ = data; }
    void getMaxDelays(std::vector<SetMaxDelayPtr> &delays) const { delays = data_->getMaxDelays(); }

    friend std::ostream &operator<<(std::ostream &os, SdcMaxDelayContainer &rhs);

  private:
    MaxDelayContainerDataPtr data_;
};
using SdcMaxDelayContainerPtr = std::shared_ptr<SdcMaxDelayContainer>;

class SdcMaxTimeBorrowContainer {
  public:
    SdcMaxTimeBorrowContainer() { data_ = std::make_shared<MaxTimeBorrowContainerData>(); }

  public:
    void add(const MaxTimeBorrowContainerDataPtr &data) { data_ = data; }
    const float getMaxTimeBorrowOnPin(const ObjectId &pin_id); 
    const float getMaxTimeBorrowOnInst(const ObjectId &inst_id); 
    const float getMaxTimeBorrowOnClock(const ClockId &clock_id); 

    friend std::ostream &operator<<(std::ostream &os, SdcMaxTimeBorrowContainer &rhs);

  private:
    MaxTimeBorrowContainerDataPtr data_;
};
using SdcMaxTimeBorrowContainerPtr = std::shared_ptr<SdcMaxTimeBorrowContainer>;

class SdcMinDelayContainer {
  public:
    SdcMinDelayContainer() { data_ = std::make_shared<MinDelayContainerData>(); }

  public:
    void add(const MinDelayContainerDataPtr &data) { data_ = data; }
    void get(std::vector<SetMinDelayPtr> &delays) { delays = data_->getMinDelays(); }

    friend std::ostream &operator<<(std::ostream &os, SdcMinDelayContainer &rhs);

  private:
    MinDelayContainerDataPtr data_;
};
using SdcMinDelayContainerPtr = std::shared_ptr<SdcMinDelayContainer>;

class SdcMinPulseWidthContainer {
  public:
    SdcMinPulseWidthContainer() { data_ = std::make_shared<MinPulseWidthData>(); }

  public:
    void add(const MinPulseWidthDataPtr &data) { data_ = data; }
    const SetMinPulseWidthPtr getMinPulseWidthOnPin(const ObjectId &pin_id) const;
    const SetMinPulseWidthPtr getMinPulseWidthOnInst(const ObjectId &inst_id) const;
    const SetMinPulseWidthPtr getMinPulseWidthOnClock(const ObjectId &clock_id) const;

    friend std::ostream &operator<<(std::ostream &os, SdcMinPulseWidthContainer &rhs);

  private:
    MinPulseWidthDataPtr data_;
};
using SdcMinPulseWidthContainerPtr = std::shared_ptr<SdcMinPulseWidthContainer>;

class SdcMulticyclePathContainer {
  public:
    SdcMulticyclePathContainer() { data_ = std::make_shared<MulticyclePathContainerData>(); }

  public:
    void add(const MulticyclePathContainerDataPtr &data) { data_ = data; }
    void get(std::vector<SetMulticyclePathPtr> &paths) const { paths = data_->getMultiCyclePaths(); }

    friend std::ostream &operator<<(std::ostream &os, SdcMulticyclePathContainer &rhs);

  private:
    MulticyclePathContainerDataPtr data_;
};
using SdcMulticyclePathContainerPtr = std::shared_ptr<SdcMulticyclePathContainer>;

class SdcOutputDelayContainer {
  public:
    SdcOutputDelayContainer() { data_ = std::make_shared<OutputDelayContainerData>(); }

  public:
    void add(const OutputDelayContainerDataPtr &data) { data_ = data; }
    const SetOutputDelayPtr getPinOutputDelay(const ObjectId &pin_id) const;

    friend std::ostream &operator<<(std::ostream &os, SdcOutputDelayContainer &rhs);

  private:
    OutputDelayContainerDataPtr data_;
};
using SdcOutputDelayContainerPtr = std::shared_ptr<SdcOutputDelayContainer>;

class SdcPropagatedClockContainer {
  public:
    SdcPropagatedClockContainer() { data_ = std::make_shared<SetPropagatedClock>(); }

    void setData(const SetPropagatedClockPtr &data) { data_ = data; };
    const SetPropagatedClockPtr getData() const { return data_; };
    SetPropagatedClockPtr getData() { return data_; };
    bool isPropagatedClock(const ClockPtr &clock) const { return clock->isPropagated(); };
    bool isPropagatedPin(const ObjectId &object_id) const;
    friend std::ostream &operator<<(std::ostream &os, SdcPropagatedClockContainer &rhs);

  private:
    SetPropagatedClockPtr data_;
};
using SdcPropagatedClockContainerPtr = std::shared_ptr<SdcPropagatedClockContainer>;


//environment commands
class SdcCaseAnalysisContainer {
  public:
    SdcCaseAnalysisContainer() { data_ = std::make_shared<CaseAnalysisCotnainerData>(); }

  public:
    void add(const ObjectId &pin_id, const SetCaseAnalysis &case_analysis) { data_->add(pin_id, case_analysis); }

    const CaseAnalysisCotnainerDataPtr &getData() const { return data_; }
    const CaseAnalysisValue getPinValue(const ObjectId &pin_id) const;
    bool isPinConstZero(const ObjectId &pin_id) const;
    bool isPinConstOne(const ObjectId &pin_id) const;

    friend std::ostream &operator<<(std::ostream &os, SdcCaseAnalysisContainer &rhs);

  private:
    CaseAnalysisCotnainerDataPtr data_;
};
using SdcCaseAnalysisContainerPtr = std::shared_ptr<SdcCaseAnalysisContainer>;

class SdcDriveContainer {
  public:
    SdcDriveContainer() { data_ = std::make_shared<DriveContainerData>(); }

  public:
    void add(const ObjectId &pin_id, const SetDrivePtr &drive) { data_->add(pin_id, drive); }
    //TODO
    const float getPinDriveResistance(const ObjectId &pin_id, bool is_rise, bool is_min) const;

    friend std::ostream &operator<<(std::ostream &os, SdcDriveContainer &rhs);

  private:
    DriveContainerDataPtr data_;
};
using SdcDriveContainerPtr = std::shared_ptr<SdcDriveContainer>;

class SdcDrivingCellContainer {
  public:
    SdcDrivingCellContainer() { data_ = std::make_shared<DrivingCellData>(); }

  public:
    void add(const ObjectId &pin_id, const SetDrivingCellPtr &cell) { data_->add(pin_id, cell); }

    friend std::ostream &operator<<(std::ostream &os, SdcDrivingCellContainer &rhs);

  private:
    DrivingCellDataPtr data_;
};
using SdcDrivingCellContainerPtr = std::shared_ptr<SdcDrivingCellContainer>;

class SdcFanoutLoadContainer {
  public:
    SdcFanoutLoadContainer() { data_ = std::make_shared<FanoutLoadContainerData>(); }

  public:
    void add(const ObjectId &pin_id, const SetFanoutLoadPtr &fanout) { data_->add(pin_id, fanout); }

    friend std::ostream &operator<<(std::ostream &os, SdcFanoutLoadContainer &rhs);

  private:
    FanoutLoadContainerDataPtr data_;
};
using SdcFanoutLoadContainerPtr = std::shared_ptr<SdcFanoutLoadContainer>;

class SdcInputTransitionContainer {
  public:
    SdcInputTransitionContainer() { data_ = std::make_shared<InputTransitionData>(); }

  public:
    void add(const ObjectId &pin_id, const SetInputTransitionPtr &transition) { data_->add(pin_id, transition); }

    friend std::ostream &operator<<(std::ostream &os, SdcInputTransitionContainer &rhs);

  private:
    InputTransitionDataPtr data_;
};
using SdcInputTransitionContainerPtr = std::shared_ptr<SdcInputTransitionContainer>;

class SdcLoadContainer {
  public:
    SdcLoadContainer() { data_ = std::make_shared<LoadContainerData>(); }

  public:
    void addToPin(const ObjectId &pin_id, const SetLoadPtr &load) { data_->addToPin(pin_id, load); }
    void addToNet(const ObjectId &net_id, const SetLoadPtr &load) { data_->addToNet(net_id, load); }

    friend std::ostream &operator<<(std::ostream &os, SdcLoadContainer &rhs);

  private:
    LoadContainerDataPtr data_;
};
using SdcLoadContainerPtr = std::shared_ptr<SdcLoadContainer>;

class SdcLogicContainer {
  public:
    SdcLogicContainer() { data_ = std::make_shared<LogicContainerData>(); }

  public:
    void add(const ObjectId &pin_id, const SetLogic &logic) { data_->add(pin_id, logic); };

    friend std::ostream &operator<<(std::ostream &os, SdcLogicContainer &rhs);

  private:
    LogicContainerDataPtr data_;
};
using SdcLogicContainerPtr = std::shared_ptr<SdcLogicContainer>;

class SdcMaxAreaContainer {
  public:
    SdcMaxAreaContainer() { data_ = std::make_shared<MaxAreaContainerData>(); }

  public:
    void add(const ObjectId &cell_id, const SetMaxAreaPtr &area) { data_->add(cell_id, area); }

    friend std::ostream &operator<<(std::ostream &os, SdcMaxAreaContainer &rhs);

  private:
    MaxAreaContainerDataPtr data_;
};
using SdcMaxAreaContainerPtr = std::shared_ptr<SdcMaxAreaContainer>;

class SdcMaxCapacitanceContainer {
  public:
    SdcMaxCapacitanceContainer() { data_ = std::make_shared<MaxCapacitanceContainerData>();}

  public:
    void addToPin(const ObjectId &pin_id, const SetMaxCapacitancePtr &cap) { data_->addToPin(pin_id, cap); }
    void addToCell(const ObjectId &cell_id, const SetMaxCapacitancePtr &cap) { data_->addToCell(cell_id, cap); }

    friend std::ostream &operator<<(std::ostream &os, SdcMaxCapacitanceContainer &rhs);

  private:
    MaxCapacitanceContainerDataPtr data_;
};
using SdcMaxCapacitanceContainerPtr = std::shared_ptr<SdcMaxCapacitanceContainer>;

class SdcMaxFanoutContainer {
  public:
    SdcMaxFanoutContainer() { data_ = std::make_shared<MaxFanoutContainerData>(); }

  public:
    void addToPin(const ObjectId &pin_id, const SetMaxFanoutPtr &fanout) { data_->addToPin(pin_id, fanout); }
    void addToCell(const ObjectId &cell_id, const SetMaxFanoutPtr &fanout) { data_->addToCell(cell_id, fanout); }

    friend std::ostream &operator<<(std::ostream &os, SdcMaxFanoutContainer &rhs);

  private:
    MaxFanoutContainerDataPtr data_;
};
using SdcMaxFanoutContainerPtr = std::shared_ptr<SdcMaxFanoutContainer>;

class SdcMaxTransitionContainer {
  public:
    SdcMaxTransitionContainer() { data_ = std::make_shared<MaxTransitionContainerData>(); }

  public:
    //NOTICE: clock path transition will set to pins
    void addToClock(const ClockId &clock_id, const SetMaxTransitionPtr &transition) { data_->addToClock(clock_id, transition); }
    void addToCell(const ObjectId &cell_id, const SetMaxTransitionPtr &transition) { data_->addToCell(cell_id, transition); }
    void addToPin(const ObjectId &pin_id, const SetMaxTransitionPtr &transition) { data_->addToPin(pin_id, transition); }

    friend std::ostream &operator<<(std::ostream &os, SdcMaxTransitionContainer &rhs);

  private:
    MaxTransitionContainerDataPtr data_;
};
using SdcMaxTransitionContainerPtr = std::shared_ptr<SdcMaxTransitionContainer>;

class SdcMinCapacitanceContainer {
  public:
    SdcMinCapacitanceContainer() { data_ = std::make_shared<MinCapacitanceContainerData>(); }

  public:
    void addToPin(const ObjectId &pin_id, const SetMinCapacitancePtr &cap) { data_->addToPin(pin_id, cap); }
    void addToCell(const ObjectId &cell_id, const SetMinCapacitancePtr &cap) { data_->addToCell(cell_id, cap); }

    friend std::ostream &operator<<(std::ostream &os, SdcMinCapacitanceContainer &rhs);

  private:
    MinCapacitanceContainerDataPtr data_;
};
using SdcMinCapacitanceContainerPtr = std::shared_ptr<SdcMinCapacitanceContainer>;

class SdcOperatingConditionsContainer {
  public:
    SdcOperatingConditionsContainer() { data_ = std::make_shared<OperatingConditionsContainerData>(); }

  public:
    void add(const ObjectId &cell_id, const SetOperatingConditionsPtr &condition) { data_->add(cell_id, condition); }

    friend std::ostream &operator<<(std::ostream &os, SdcOperatingConditionsContainer &rhs);

  private:
    OperatingConditionsContainerDataPtr data_;
};
using SdcOperatingConditionsContainerPtr = std::shared_ptr<SdcOperatingConditionsContainer>;

class SdcPortFanoutNumberContainer {
  public:
    SdcPortFanoutNumberContainer() { data_ = std::make_shared<PortFanoutNumberContainerData>(); }

  public:
    void add(const ObjectId &pin_id, const SetPortFanoutNumberPtr &fanout) { data_->add(pin_id, fanout); }

    friend std::ostream &operator<<(std::ostream &os, SdcPortFanoutNumberContainer &rhs);

  private:
    PortFanoutNumberContainerDataPtr data_;
}; 
using SdcPortFanoutNumberContainerPtr = std::shared_ptr<SdcPortFanoutNumberContainer>;

class SdcResistanceContainer {
  public:
    SdcResistanceContainer() { data_ = std::make_shared<ResistanceContainerData>(); }

  public:
    void add(const ObjectId &net_id, const SetResistancePtr &res) { data_->add(net_id, res); }

    friend std::ostream &operator<<(std::ostream &os, SdcResistanceContainer &rhs);

  private:
    ResistanceContainerDataPtr data_;
};
using SdcResistanceContainerPtr = std::shared_ptr<SdcResistanceContainer>;

class SdcTimingDerateContainer {
  public:
    SdcTimingDerateContainer() { data_ = std::make_shared<TimingDerateContainerData>(); }

  public:
    void addToNet(const ObjectId &net_id, const SetTimingDeratePtr &derate) { data_->addToNet(net_id, derate); }
    void addToInst(const ObjectId &inst_id, const SetTimingDeratePtr &derate) { data_->addToInst(inst_id, derate); }
    void addToCell(const ObjectId &cell_id, const SetTimingDeratePtr &derate) { data_->addToCell(cell_id, derate); }

    friend std::ostream &operator<<(std::ostream &os, SdcTimingDerateContainer &rhs);

  private:
    TimingDerateContainerDataPtr data_;
};
using SdcTimingDerateContainerPtr = std::shared_ptr<SdcTimingDerateContainer>;

class SdcVoltageContainer {
  public:
    SdcVoltageContainer() { data_ = std::make_shared<VoltageContainerData>(); }
  
  public:
    void addToNet(const ObjectId &net_id, const SetVoltagePtr &voltage) { data_->addToNet(net_id, voltage); }
    void addToPin(const ObjectId &pin_id, const SetVoltagePtr &voltage) { data_->addToPin(pin_id, voltage); }

    friend std::ostream &operator<<(std::ostream &os, SdcVoltageContainer &rhs);

  private:
    VoltageContainerDataPtr data_;
};
using SdcVoltageContainerPtr = std::shared_ptr<SdcVoltageContainer>;

class SdcWireLoadMinBlockSizeContainer {
  public:
    SdcWireLoadMinBlockSizeContainer() { data_ = std::make_shared<WireLoadMinBlockSizeContainerData>(); }

  public:
    void add(const ObjectId &cell_id, const SetWireLoadMinBlockSizePtr &size) { data_->add(cell_id, size); }

  private:
    WireLoadMinBlockSizeContainerDataPtr data_;
};
using SdcWireLoadMinBlockSizeContainerPtr = std::shared_ptr<SdcWireLoadMinBlockSizeContainer>;

class SdcWireLoadModeContainer {
  public:
    SdcWireLoadModeContainer() { data_ = std::make_shared<WireLoadModeContainerData>(); }

  public:
    void add(const ObjectId &cell_id, const SetWireLoadMode &mode) { data_->add(cell_id, mode); }

    friend std::ostream &operator<<(std::ostream &os, SdcWireLoadModeContainer &rhs);

  private:
    WireLoadModeContainerDataPtr data_;
};
using SdcWireLoadModeContainerPtr = std::shared_ptr<SdcWireLoadModeContainer>; 

class SdcWireLoadModelContainer {
  public:
    SdcWireLoadModelContainer() { data_ = std::make_shared<WireLoadModelContainerData>(); }

  public:
    void addToCell(const ObjectId &cell_id, const SetWireLoadModelPtr &model) { data_->addToCell(cell_id, model); }
    void addToPin(const ObjectId &pin_id, const SetWireLoadModelPtr &model) { data_->addToPin(pin_id, model); }
    
    friend std::ostream &operator<<(std::ostream &os, SdcWireLoadModelContainer &rhs);

  private:
    WireLoadModelContainerDataPtr data_;
};
using SdcWireLoadModelContainerPtr = std::shared_ptr<SdcWireLoadModelContainer>;

class SdcWireLoadSelectionGroupContainer {
  public:
    SdcWireLoadSelectionGroupContainer() { data_ = std::make_shared<WireLoadSelectionGroupContainerData>(); }

  public:
    void add(const ObjectId &cell_id, const SetWireLoadSelectionGroupPtr &group) { data_->add(cell_id, group); }

    friend std::ostream &operator<<(std::ostream &os, SdcWireLoadSelectionGroupContainer &rhs);

  private:
    WireLoadSelectionGroupContainerDataPtr data_;
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
  public:
    SdcMaxLeakagePowerContainer() { data_ = std::make_shared<MaxLeakagePowerContainerData>(); }

  public:
    void add(const MaxLeakagePowerContainerDataPtr &data) { data_ = data; }
    const float getCellPower(const ObjectId &cell_id) const;

    friend std::ostream &operator<<(std::ostream &os, SdcMaxLeakagePowerContainer &rhs);

  private:
    MaxLeakagePowerContainerDataPtr data_;
};
using SdcMaxLeakagePowerContainerPtr = std::shared_ptr<SdcMaxLeakagePowerContainer>; 

//object access commands
class SdcAllClocksContainer {
  public:
    SdcAllClocksContainer() { data_ = std::make_shared<SdcClockContainer>(); }
    SdcAllClocksContainer(const SdcClockContainerPtr &data) { data_ = std::make_shared<SdcClockContainer>(); }

    const std::vector<std::string> &get_all_clocks() const { return data_->getClockNames(); };
    friend std::ostream &operator<<(std::ostream &os, SdcAllClocksContainer &rhs);

  private:
    SdcClockContainerPtr data_;
};
using SdcAllClocksContainerPtr = std::shared_ptr<SdcAllClocksContainer>;

class SdcAllInputsContainer {
  public:
    SdcAllInputsContainer() { data_ = std::make_shared<SdcClockContainer>(); }
    SdcAllInputsContainer(const SdcClockContainerPtr &data) { data_ = std::make_shared<SdcClockContainer>(); }

    const std::vector<std::string> &get_all_inputs(const AllInputsPtr &conditions) const;
    friend std::ostream &operator<<(std::ostream &os, SdcAllInputsContainer &rhs);

  private:
    SdcClockContainerPtr data_;
};
using SdcAllInputsContainerPtr = std::shared_ptr<SdcAllInputsContainer>;

class SdcAllOutputsContainer {
  public:
    SdcAllOutputsContainer() { data_ = std::make_shared<SdcClockContainer>(); }
    SdcAllOutputsContainer(const SdcClockContainerPtr &data) { data_ = std::make_shared<SdcClockContainer>(); }

    const std::vector<std::string> &get_all_outputs(const AllOutputsPtr &conditions) const;
    friend std::ostream &operator<<(std::ostream &os, SdcAllOutputsContainer &rhs);

  private:
    SdcClockContainerPtr data_;
};
using SdcAllOutputsContainerPtr = std::shared_ptr<SdcAllOutputsContainer>;

class SdcAllRegistersContainer {
  public:
    SdcAllRegistersContainer() { data_ = std::make_shared<SdcClockContainer>(); }
    SdcAllRegistersContainer(const SdcClockContainerPtr &data) { data_ = std::make_shared<SdcClockContainer>(); }

    const std::vector<std::string> &get_all_registers(const AllRegistersPtr &conditions) const;
    friend std::ostream &operator<<(std::ostream &os, SdcAllRegistersContainer &rhs);

  private:
    SdcClockContainerPtr data_;
};
using SdcAllRegistersContainerPtr = std::shared_ptr<SdcAllRegistersContainer>;

class SdcCurrentDesignContainer {
  public:
    SdcCurrentDesignContainer() { data_ = std::make_shared<CurrentDesign>(); }

    void setData(const CurrentDesignPtr &data) { data_ = data; };
    const CurrentDesignPtr getData() const { return data_; };
    CurrentDesignPtr getData() { return data_; };

    const std::string getDesignName() const;
    const ObjectId getDesignId() const{ return data_->getCellId(); };
    friend std::ostream &operator<<(std::ostream &os, SdcCurrentDesignContainer &rhs);

  private:
    CurrentDesignPtr data_;
};
using SdcCurrentDesignContainerPtr = std::shared_ptr<SdcCurrentDesignContainer>;

class SdcGetCellsContainer {
  public:
    const std::vector<std::string> &get_cells_names(const GetCellsPtr &conditions) const;
    friend std::ostream &operator<<(std::ostream &os, SdcGetCellsContainer &rhs);

  private:
    SdcCurrentDesignContainer current_design_;
    SdcCurrentInstanceContainer current_instance_;
};
using SdcGetCellsContainerPtr = std::shared_ptr<SdcGetCellsContainer>;

class SdcGetClocksContainer {
  public:
    const std::vector<std::string> &get_clocks_names(const GetClocksPtr &conditions) const;
    friend std::ostream &operator<<(std::ostream &os, SdcGetClocksContainer &rhs);
};
using SdcGetClocksContainerPtr = std::shared_ptr<SdcGetClocksContainer>;

class SdcGetLibCellsContainer {
  public:
    const std::vector<std::string> &get_lib_cells_names(const GetLibCellsPtr &conditions) const;
    friend std::ostream &operator<<(std::ostream &os, SdcGetLibCellsContainer &rhs);
};
using SdcGetLibCellsContainerPtr = std::shared_ptr<SdcGetLibCellsContainer>;

class SdcGetLibPinsContainer {
  public:
    const std::vector<std::string> &get_lib_pins_names(const GetLibPinsPtr &conditions) const;
    friend std::ostream &operator<<(std::ostream &os, SdcGetLibPinsContainer &rhs);
};
using SdcGetLibPinsContainerPtr = std::shared_ptr<SdcGetLibPinsContainer>;

class SdcGetLibsContainer {
  public:
    const std::vector<std::string> &get_libs_names(const GetLibsPtr &conditions) const;
    friend std::ostream &operator<<(std::ostream &os, SdcGetLibsContainer &rhs);
};
using SdcGetLibsContainerPtr = std::shared_ptr<SdcGetLibsContainer>;

class SdcGetNetsContainer {
  public:
    const std::vector<ObjectId> &get_nets_ids(const GetNetsPtr &conditions) const;
    const std::vector<std::string> &get_nets_names(const GetNetsPtr &conditions) const;
    friend std::ostream &operator<<(std::ostream &os, SdcGetLibsContainer &rhs);
};
using SdcGetNetsContainerPtr = std::shared_ptr<SdcGetNetsContainer>;

class SdcGetPinsContainer {
  public:
    const std::vector<ObjectId> &get_pins_ids(const GetPinsPtr &conditions) const;
    const std::vector<std::string> &get_pins_names(const GetPinsPtr &conditions) const;
    friend std::ostream &operator<<(std::ostream &os, SdcGetPinsContainer &rhs);
};
using SdcGetPinsContainerPtr = std::shared_ptr<SdcGetPinsContainer>;

class SdcGetPortsContainer {
  public:
    const std::vector<ObjectId> &get_ports_ids(const GetPortsPtr &conditions) const;
    const std::vector<std::string> &get_ports_names(const GetPortsPtr &conditions) const;
}; 
using SdcGetPortsContainerPtr = std::shared_ptr<SdcGetPortsContainer>;





}
}
#endif //EDI_DB_TIMING_SDC_SDC_DATA_H_ 
