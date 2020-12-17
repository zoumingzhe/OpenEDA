/** 
 * @file sdc_data.cpp
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

#include "db/timing/sdc/sdc_data.h"

namespace open_edi {
namespace db {

//general purpose commands
const std::string SdcCurrentInstanceContainer::getName() const {
    const ObjectId &inst_id = current_instance_->getInstanceId();
    const Inst* inst = Object::addr<Inst>(inst_id);
    if (inst) {
        return inst->getName();
    }
    //TODO return top?
    return "";
}

std::ostream &operator<<(std::ostream &os, SdcCurrentInstanceContainer &rhs) {
    os << "current_instance " << rhs.getName() << "\n"; 
    return os;
}

std::ostream &operator<<(std::ostream &os, SdcHierarchySeparatorContainer &rhs) {
    os << "set_hierarchy_separator " << rhs.get() << "\n";
    return os;
}

std::ostream &operator<<(std::ostream &os, SdcUnitsContainer &rhs) {
    os << "set_units "; 
    os << "-time " << rhs.getSdcTimeUnits() << "s";
    os << "-capacitance " << rhs.getSdcCapacitanceUnits() << "F";
    os << "-current " << rhs.getSdcCurrentUnits() << "A";
    os << "-voltage " << rhs.getSdcVoltageUnits() << "V";
    os << "-resistance " << rhs.getSdcResistanceUnits() << "Ohm";
    os << "-power " << rhs.getSdcPowerUnits() << "W";
    os << "\n";
    return os;
}


// timing constraints
// create_clock
const std::string SdcClockContainer::getClockName(const ClockId &id) {
    const auto &clock_names = data_->getClockNames();
    if (id<0 or id>=(ClockId)(data_->getClockNum())) {
        //TODO error message
        return "";
    }
    return clock_names[id]; 
}

const ClockId SdcClockContainer::getClockId(const std::string &name) {
    const auto &name_to_id = data_->getNameToId();
    const auto &found = name_to_id.find(name);
    if (found == name_to_id.end()) {
        //TODO error message
        return kInvalidClockId;
    }
    return found->second;
}

const ClockPtr SdcClockContainer::getClock(const ClockId &id) {
    const auto &clocks = data_->getClocks();
    if (id<0 or id>=(ClockId)(data_->getClockNum())) {
        //TODO error message
        return nullptr;
    }
    return clocks[id];
}

const ClockPtr SdcClockContainer::getClock(const std::string &name) {
    const auto &name_to_id = data_->getNameToId();
    const auto &found = name_to_id.find(name);
    if (found == name_to_id.end()) {
        //TODO error message
        return nullptr;
    }
    const auto &id = found->second; 
    if (id<0 or id>=(ClockId)(data_->getClockNum())) {
        //TODO error message
        return nullptr;
    }
    const auto &clocks = data_->getClocks();
    return clocks[id];
}

const ClockPtr SdcClockContainer::getClockOnPin(const ObjectId &pin_id) {
    const auto &pin_clock_map = data_->getPinClockMap();
    const auto &found = pin_clock_map.find(pin_id);
    if (found == pin_clock_map.end()) {
        //TODO error message
        return nullptr;
    }
    return found->second;
}

bool SdcClockContainer::isClockPin(const ObjectId &pin_id) {
    const auto &pin_clock_map = data_->getPinClockMap();
    const auto &found = pin_clock_map.find(pin_id);
    if (found == pin_clock_map.end()) {
        return false;
    }
    return true;
}

std::ostream &operator<<(std::ostream &os, SdcClockContainer &rhs) {
    for (const auto &id_to_create_clock : rhs.data_->getCreateClocks()) {
        const auto &clock_id = id_to_create_clock.first; 
        const auto &create_clock = id_to_create_clock.second; 
        const auto &clock = rhs.getClock(clock_id);
        os  << "create_clock ";
        os  << "-period " << clock->getPeriod()
            << "-name " << clock->getName()
            << "-comment " << create_clock->getComment();
        os  << "-waveform {";
        copy(clock->getWaveform().begin(), clock->getWaveform().end(), std::ostream_iterator<float>(os, " "));
        os  << " } ";
        if (create_clock->isAdd()) {
            os  << "-add ";
        }
        for (const auto &clock_pin: rhs.data_->getPinClockMap()) {
            const auto &other_clock = clock_pin.second;
            if (!other_clock) {
                // error messages
                continue;
            }
            if (other_clock->getId() == clock_id) {
                const ObjectId &pin_id = clock_pin.first; 
                Pin* pin = Object::addr<Pin>(pin_id);
                if (!pin) {
                    // error messages
                    continue;
                }
                const auto &pin_name = pin->getName();
                os << pin_name << " ";
            }
        }
        os << "\n";
    }
    for (const auto &id_to_generated_clock : rhs.data_->getCreateGeneratedClocks()) {
        const auto &clock_id = id_to_generated_clock.first; 
        const auto &generated_clock = id_to_generated_clock.second; 
        const auto &clock = rhs.getClock(clock_id);
        os  << "create_generated_clock ";
        os  << "-name " << clock->getName();
        os  << "-source ";
        for (const auto &pin_id: generated_clock->getSourceMasterPins()) {
            Pin* pin = Object::addr<Pin>(pin_id);
            if (!pin) {
                // error messages
                continue;
            }
            const auto &pin_name = pin->getName();
            os << pin_name << " ";
        }
        os  << "-divided_by " << generated_clock->getDividedBy() 
            << "-multiply_by " << generated_clock->getMultiplyBy();
        os  << "-edges {" ;
        copy(generated_clock->getEdges().begin(), generated_clock->getEdges().end(), std::ostream_iterator<int>(os, " "));
        os  << " } "; 
        if (generated_clock->isCombinational()) {
            os  << "-combinational ";
        }
        os  << "-duty_cycle " << generated_clock->getDutyCycle(); 
        if (generated_clock->isInvert()) {
            os  << "-invert ";
        }
        os  << "-edge_shift {";
        copy(generated_clock->getEdgeShifts().begin(), generated_clock->getEdgeShifts().end(), std::ostream_iterator<float>(os, " "));
        os  << " } ";
        if (generated_clock->isAdd()) {
            os  << "-add ";
        }
        const auto &master_clock = generated_clock->getMasterClock();
        os  << "-master_clock " << rhs.getClockName(master_clock);
        os  << "-comment " << generated_clock->getComment();
        for (const auto &clock_pin: rhs.data_->getPinClockMap()) {
            const auto &other_clock = clock_pin.second;
            if (!other_clock) {
                // error messages
                continue;
            }
            if (other_clock->getId() == clock_id) {
                const ObjectId &pin_id = clock_pin.first; 
                Pin* pin = Object::addr<Pin>(pin_id);
                if (!pin) {
                    // error messages
                    continue;
                }
                const auto &pin_name = pin->getName();
                os << pin_name << " ";
            }
        }
        os << "\n";
    }
    return os;
}

const SetClockGatingCheckPtr SdcClockGatingCheckContainer::getClockGatingCheckOnPin(const ObjectId &pin_id) const {
    const auto &pin_to_check = data_->getPinToCheck();
    const auto &found = pin_to_check.find(pin_id); 
    if (found == pin_to_check.end()) {
        return nullptr;
    }
    return found->second;
}

const SetClockGatingCheckPtr  SdcClockGatingCheckContainer::getClockGatingCheckOnCell(const ObjectId &cell_id) const {
    const auto &cell_to_check = data_->getCellToCheck();
    const auto &found = cell_to_check.find(cell_id);
    if (found == cell_to_check.end()) {
        return nullptr;
    }
    return found->second;
}

const SetClockGatingCheckPtr SdcClockGatingCheckContainer::getClockGatingCheckOnClock(const ClockId &clock_id) const {
    const auto &clock_to_check = data_->getClockToCheck();
    const auto &found = clock_to_check.find(clock_id);
    if (found == clock_to_check.end()) {
        return nullptr;
    }
    return found->second;
}

const float SdcClockGatingCheckContainer::getPinCheckTime(bool is_rise, bool is_high, bool is_setup, const ObjectId &pin_id) const {
    const auto &check = getClockGatingCheckOnPin(pin_id);
    if (!check) {
        // error messages
        return 0.0;
    }
    if (check->getRise()!=is_rise or check->getHigh()!=is_high) {
        return 0.0;
    }
    return is_setup ? check->getSetup() : check->getHold();
}

const float SdcClockGatingCheckContainer::getCellCheckTime(bool is_rise, bool is_high, bool is_setup, const ObjectId &cell_id) const {
    const auto &check = getClockGatingCheckOnCell(cell_id);
    if (!check) {
        // error messages
        return 0.0;
    }
    if (check->getRise()!=is_rise or check->getHigh()!=is_high) {
        return 0.0;
    }
    return is_setup ? check->getSetup() : check->getHold();
}

const float SdcClockGatingCheckContainer::getClockCheckTime(bool is_rise, bool is_high, bool is_setup, const ClockId &clock_id) const {
    const auto &check = getClockGatingCheckOnClock(clock_id);
    if (!check) {
        // error messages
        return 0.0;
    }
    if (check->getRise()!=is_rise or check->getHigh()!=is_high) {
        return 0.0;
    }
    return is_setup ? check->getSetup() : check->getHold();
}

void SdcClockGroupsContainer::findClocks(std::vector<ClockId> &clocks, const ClockId &clock_id, const GroupRelationshipType &type) const {
    clocks.clear();
    const auto &clock_to_groups = data_->getClockToGroup();
    const auto &groups = data_->getGroups();
    const auto &group_num = data_->getClockGroupNum();
    const auto &range = clock_to_groups.equal_range(clock_id);
    for (auto it = range.first; it != range.second; ++it) {
        const auto &group = it->second;
        if (group->getType() != type) {
            continue;
        }
        const auto &group_id = group->getId();
        bool search_right_group = group->isLinkToRight();
        auto right_group_id=group_id+1;
        for (; search_right_group; ++right_group_id) {
           if (right_group_id >= group_num) {
               //error message
               continue;
           }
           const auto &right_group = groups[right_group_id];
           const auto &right_clocks = right_group->getClockIds();
           clocks.insert(clocks.end(), right_clocks.begin(), right_clocks.end());
           search_right_group = right_group->isLinkToRight(); 
        }
        bool search_left_group = group->isLinkToLeft();
        auto left_group_id=group_id-1;
        for (; search_left_group; --left_group_id) {
           if (left_group_id < 0) {
               //error message
               continue;
           }
           const auto &left_group = groups[left_group_id];
           const auto &left_clocks = left_group->getClockIds();
           clocks.insert(clocks.end(), left_clocks.begin(), left_clocks.end());
           search_left_group = left_group->isLinkToLeft(); 
        }
    }
}

void SdcClockGroupsContainer::getClocksRelationshipType(std::vector<GroupRelationshipType> &types, const ClockId &clock_id, const ClockId &other_clock_id) const {
    //Multiple relationship types can exist between two clocks. When this happens, the relationships obey the following precedence. In this function,  return the relationship vector.
    //Physically exclusive
    //Asynchronous
    //Logically exclusive
    const auto &clock_to_groups = data_->getClockToGroup();
    const auto &groups = data_->getGroups();
    const auto &group_num = data_->getClockGroupNum();
    const auto &range = clock_to_groups.equal_range(clock_id);
    for (auto it = range.first; it != range.second; ++it) {
        const auto &group = it->second;
        const auto &type = group->getType();
        const auto &group_id = group->getId();
        bool search_right_group = group->isLinkToRight();
        auto right_group_id=group_id+1;
        bool found = false;
        for (; search_right_group; ++right_group_id) {
           if (right_group_id >= group_num) {
               //error message
               continue;
           }
           const auto &right_group = groups[right_group_id];
           const auto &right_clocks = right_group->getClockIds();
           found = std::binary_search(right_clocks.begin(), right_clocks.end(), other_clock_id);
           if (found) {
               types.emplace_back(type);
               break;
           }
           search_right_group = right_group->isLinkToRight(); 
        }
        if (found) {
            continue;
        }
        bool search_left_group = group->isLinkToLeft();
        auto left_group_id=group_id-1;
        for (; search_left_group; --left_group_id) {
           if (left_group_id < 0) {
               //error message
               continue;
           }
           const auto &left_group = groups[left_group_id];
           const auto &left_clocks = left_group->getClockIds();
           const bool found = std::binary_search(left_clocks.begin(), left_clocks.end(), other_clock_id);
           if (found) {
               types.emplace_back(type);
               break;
           }
           search_left_group = left_group->isLinkToLeft(); 
        }
    }
}


void SdcClockLatencyContainer::getClockLatencyOnPin(std::vector<ClockLatencyOnPinPtr> &latencys, const ObjectId &pin_id) const {
    const auto &pin_to_latency = data_->getPinToLatency();
    const auto &range = pin_to_latency.equal_range(pin_id);
    for (auto it = range.first; it != range.second; ++it) {
        const auto &latency = it->second;
        latencys.emplace_back(latency);
    }
} 

void SdcClockLatencyContainer::getClockLatencyOnClock(std::vector<SetClockLatencyPtr> &latencys, const ClockId &clock_id) const {
    const auto &clock_to_latency = data_->getClockToLatency();
    const auto &range = clock_to_latency.equal_range(clock_id);
    for (auto it = range.first; it != range.second; ++it) {
        const auto &latency = it->second;
        latencys.emplace_back(latency);
    }
}

const SetSensePtr SdcSenceContainer::getSense(const ObjectId &pin_id) const {
    const auto &pin_sense = data_->getPinSense();
    const auto &found = pin_sense.find(pin_id);
    if (found == pin_sense.end()) {
        return nullptr;
    }
    return found->second;
}

const SetClockTransitionPtr SdcClockTransitionContainer::getTransition(const ClockId &clock_id) const {
    const auto &clock_transitions = data_->getClockTransitions();
    const auto &found = clock_transitions.find(clock_id);
    if (found == clock_transitions.end()) {
        return nullptr;
    }
    return found->second;
}

const InterClockUncertaintyPtr SdcClockUncertaintyContainer::getUncertaintyOnInterClocks(const ClockId &from_clock_id, const ClockId &to_clock_id) const {
    const auto &clock_pair = std::make_pair(from_clock_id, to_clock_id);
    const auto &interclock_uncertainty = data_->getInterClockUncertainty(); 
    const auto &found = interclock_uncertainty.find(clock_pair);
    if (found == interclock_uncertainty.end()) {
        return nullptr;
    }
    return found->second;
}

const SetClockUncertaintyPtr SdcClockUncertaintyContainer:: getUncertaintyOnPin(const ObjectId &pin_id) const {
    const auto &pin_uncertainty = data_->getPinUncertainty();
    const auto &found = pin_uncertainty.find(pin_id);
    if (found == pin_uncertainty.end()) {
        return nullptr;
    }
    return found->second;
}

const SetClockUncertaintyPtr SdcClockUncertaintyContainer::getUncertaintyOnClock(const ClockId &clock_id) const {
    const auto &clock_uncertainty = data_->getClockUncertainty();
    const auto &found = clock_uncertainty.find(clock_id);
    if (found == clock_uncertainty.end()) {
        return nullptr;
    }
    return found->second;
}

const SetDataCheckPtr SdcDataCheckContainer::getDataCheckOnInterPins(const ObjectId &from_pin_id, const ObjectId &to_pin_id) const {
    const auto &data_check = data_->getInterPinDataCheck();
    const auto &pin_pair = std::make_pair(from_pin_id, to_pin_id);
    const auto &found = data_check.find(pin_pair);
    if (found == data_check.end()) {
        return nullptr;
    }
    return found->second;
}

bool SdcDisableTimingContainer::hasDisableTiming(const ObjectId &from_pin_id, const ObjectId &to_pin_id) const {
    const auto &from_pin = Object::addr<Pin>(from_pin_id); 
    if (!from_pin) {
        // error messages
        return false;
    }
    const auto &to_pin = Object::addr<Pin>(to_pin_id); 
    if (!to_pin) {
        // error messages
        return false;
    }
    const auto &from_inst = from_pin->getInst(); 
    const auto &to_inst = to_pin->getInst(); 
    if (!from_inst or !to_inst) {
        // error messages
        return false;
    }
    if (from_inst->getId() != to_inst->getId()) {
        // error messages
        return false;
    }
    const auto &inst_id = from_inst->getId();
    const auto &inst_disable_arcs = data_->getInstDisableArcs(); 
    const auto &found_inst = inst_disable_arcs.find(inst_id);
    if (found_inst != inst_disable_arcs.end()) {
        const SetDisableTimingPtr &disable_arcs = found_inst->second;
        if (disable_arcs->isSetAllArcs()) {
            return true;
        }
        if ((from_pin_id == disable_arcs->getFromId()) and to_pin_id == disable_arcs->getToId()) {
            return true;
        }
    }
    const auto &cell = from_inst->getMaster();
    if (!cell) {
        // error messages
        return false;
    }
    const auto &cell_id = cell->getId();
    const auto &cell_disable_arcs = data_->getCellDisableArcs();
    const auto &found_cell = cell_disable_arcs.find(cell_id);
    if (found_cell != cell_disable_arcs.end()) {
        const SetDisableTimingPtr &disable_arcs = found_cell->second;
        if (disable_arcs->isSetAllArcs()) {
            return true;
        }
        const auto &from_term = from_pin->getTerm();
        const auto &to_term = to_pin->getTerm();
        if (!from_term or !to_term) {
            //error messages
            return false;
        }
        const auto &from_term_id = from_term->getId();
        const auto &to_term_id = to_term->getId();
        if ((from_term_id == disable_arcs->getFromId()) and to_term_id == disable_arcs->getToId()) {
            return true;
        }
    }
    return false;
}

bool SdcDisableTimingContainer::hasDisableTimingArcInInst(const ObjectId &inst_id) const {
    const auto &inst_disable_arcs = data_->getInstDisableArcs(); 
    const auto &found_inst = inst_disable_arcs.find(inst_id);
    if (found_inst != inst_disable_arcs.end()) {
        return true;
    }
    const auto &inst = Object::addr<Inst>(inst_id);
    const auto &cell = inst->getMaster(); 
    if (!cell) {
        // error messages
        return false;
    }
    const auto &cell_id = cell->getId();
    const auto &cell_disable_arcs = data_->getCellDisableArcs();
    const auto &found_cell = cell_disable_arcs.find(cell_id);
    if (found_cell != cell_disable_arcs.end()) {
        return true;
    }
    return false;
}

const SetIdealLatencyPtr SdcIdealLatencyContainer::getPinLatency(const ObjectId &pin_id) const {
    const auto &pin_to_latency = data_->getPinToLatency();
    const auto &found = pin_to_latency.find(pin_id);
    if (found == pin_to_latency.end()) {
        return nullptr;
    }
    return found->second;
}

const SetIdealTransitionPtr SdcIdealTransitionContainer::getPinTransition(const ObjectId &pin_id) const {
    const auto &pin_ideal_transitions = data_->getPinIdealTransitions();
    const auto &found = pin_ideal_transitions.find(pin_id);
    if (found == pin_ideal_transitions.end()) {
        return nullptr;
    }
    return found->second;
}

const SetInputDelayPtr SdcInputDelayContainer::getPinInputDelay(const ObjectId &pin_id) const {
    const auto &pin_input_delays = data_->getPinInputDelays();
    const auto &found = pin_input_delays.find(pin_id);
    if (found == pin_input_delays.end()) {
        return nullptr;
    }
    return found->second;
}

const float SdcMaxTimeBorrowContainer::getMaxTimeBorrowOnPin(const ObjectId &pin_id) {
    const auto &pin_max_time_borrow = data_->getPinMaxTimeBorrow();
    const auto &found = pin_max_time_borrow.find(pin_id);
    if (found == pin_max_time_borrow.end()) {
        return 0.0;
    }
    const auto &time_borrow = found->second;
    if (!time_borrow) {
        //error messages
        return 0.0;
    }
    return time_borrow->getValue();
}

const float SdcMaxTimeBorrowContainer::getMaxTimeBorrowOnInst(const ObjectId &inst_id) {
    const auto &inst_max_time_borrow = data_->getInstMaxTimeBorrow();
    const auto &found = inst_max_time_borrow.find(inst_id);
    if (found == inst_max_time_borrow.end()) {
        return 0.0;
    }
    const auto &time_borrow = found->second;
    if (!time_borrow) {
        //error messages
        return 0.0;
    }
    return time_borrow->getValue();
} 

const float SdcMaxTimeBorrowContainer::getMaxTimeBorrowOnClock(const ClockId &clock_id) {
    const auto &clock_max_time_borrow = data_->getClockMaxTimeBorrow();
    const auto &found = clock_max_time_borrow.find(clock_id);
    if (found == clock_max_time_borrow.end()) {
        return 0.0;
    }
    const auto &time_borrow = found->second;
    if (!time_borrow) {
        //error messages
        return 0.0;
    }
    return time_borrow->getValue();
} 

const SetMinPulseWidthPtr SdcMinPulseWidthContainer::getMinPulseWidthOnPin(const ObjectId &pin_id) const {
    const auto &pin_min_pulse_width = data_->getPinMinPulseWidth();
    const auto &found = pin_min_pulse_width.find(pin_id);
    if (found == pin_min_pulse_width.end()) {
        return nullptr;
    }
    return found->second;
}

const SetMinPulseWidthPtr SdcMinPulseWidthContainer::getMinPulseWidthOnInst(const ObjectId &inst_id) const {
    const auto &inst_min_pulse_width = data_->getInstMinPulseWidth();
    const auto &found = inst_min_pulse_width.find(inst_id);
    if (found == inst_min_pulse_width.end()) {
        for (const auto &cell_to_min_pulse_width : data_->getCellMinPulseWidth()) {
            const auto &cell_id = cell_to_min_pulse_width.first;
            const auto &min_pulse_width = cell_to_min_pulse_width.second;
            //TODO
            //if (isInstInCell(inst_id, cell_id)) {
            //    return min_pulse_width;
            //}
        }
        return nullptr;
    }
    return found->second;
}

const SetMinPulseWidthPtr SdcMinPulseWidthContainer::getMinPulseWidthOnClock(const ObjectId &clock_id) const {
    const auto &clock_min_pulse_width = data_->getClockMinPulseWidth();
    const auto &found = clock_min_pulse_width.find(clock_id);
    if (found == clock_min_pulse_width.end()) {
        return nullptr;
    }
    return found->second;
}

const SetOutputDelayPtr SdcOutputDelayContainer::getPinOutputDelay(const ObjectId &pin_id) const {
    const auto &pin_output_delays = data_->getPinOutputDelays();
    const auto &found = pin_output_delays.find(pin_id);
    if (found == pin_output_delays.end()) {
        return nullptr;
    }
    return found->second;
}

bool SdcPropagatedClockContainer::isPropagatedClock(const ClockId &clock_id) const {
    const auto &clocks = data_->getClocks();
    const auto &found = clocks.find(clock_id);
    if (found != clocks.end()) {
        return true;
    }
    return false;
}

bool SdcPropagatedClockContainer::isPropagatedPin(const ObjectId &pin_id) const {
    const auto &pins = data_->getPins();
    const auto &found = pins.find(pin_id);
    if (found != pins.end()) {
        return true;
    }
    return false;
}

// environment commands
// set_case_analysis
const CaseAnalysisValue SdcCaseAnalysisContainer::getPinValue(const ObjectId &pin_id) const {
    const auto &pin_to_case_analysis = data_->getPinToCaseAnalysis();
    const auto &found = pin_to_case_analysis.find(pin_id); 
    if (found != pin_to_case_analysis.end()) {
        const auto &case_analysis = found->second;
        return case_analysis.getValue(); 
    }
    return CaseAnalysisValue::kUnknown;
}

bool SdcCaseAnalysisContainer::isPinConstZero(const ObjectId &pin_id) const {
    const auto& value = getPinValue(pin_id); 
    return (value == CaseAnalysisValue::kZero); 
} 

bool SdcCaseAnalysisContainer::isPinConstOne(const ObjectId &pin_id) const {
    const auto& value = getPinValue(pin_id); 
    return (value == CaseAnalysisValue::kOne); 
}

std::ostream &operator<<(std::ostream &os, SdcCaseAnalysisContainer &rhs) {
    const auto &pin_to_case_analysis = rhs.data_->getPinToCaseAnalysis();
    for ( const auto &pin_value : pin_to_case_analysis ) {
        const auto &pin_id = pin_value.first;
        const auto &pin = Object::addr<Pin>(pin_id); 
        if (!pin) {
            //TODO messages
            continue;
        }
        const auto& case_analysis = pin_value.second;
        os << "set_case_analysis " << to_string(case_analysis.getValue()) << " " << pin->getName() << "\n";
    }
    return os;
}


//multivoltage power commands
const CreateVoltageAreaPtr SdcVoltageAreaContainer::getCellVoltageArea(const ObjectId &cell_id) const {
   const auto &cell_voltage_area = data_->getCellVoltageArea(); 
   const auto &found = cell_voltage_area.find(cell_id);
   if (found == cell_voltage_area.end()) {
       // messages;
       return nullptr;
   }
   return found->second;
}

std::ostream &operator<<(std::ostream &os, SdcVoltageAreaContainer &rhs) {
    if (!rhs.data_) {
        // error messages
        return os;
    }
    const auto &cell_voltage_area_map = rhs.data_->getCellVoltageArea(); 
    for (const auto &cell_to_area : cell_voltage_area_map) {
        const auto &cell_id = cell_to_area.first;
        const auto &voltage_area = cell_to_area.second; 
        if (!voltage_area) {
            // error message
            continue;
        }
        const auto &box_vector = voltage_area->getCoordinates();
        const auto &cell = Object::addr<Cell>(cell_id);
        if (!cell) {
            //error message
            continue;
        }
        const auto &cell_name = cell->getName();
        os  << "create_voltage_area "
            << "-name " << voltage_area->getName();
        os  << "-corrdinate "; 
        for (const auto &box : box_vector) {
            if (!box) {
                // error messages
                continue;
            }
            os << "{ " << box->getLLX() << " " << box->getLLY() << " " << box->getURX() << " " << box->getURY() << " } "; 
        }
        os  << "-guard_band_x " << voltage_area->getGuardBandX()
            << "-guard_band_y " << voltage_area->getGuardBandY()
            << cell_name;
        os  << "\n";
    }
    return os;
}


std::ostream &operator<<(std::ostream &os, SdcLevelShifterStrategyContainer &rhs) {
    os << "set_level_shifter_strategy " << toString(rhs.getLevelShifterStrategy()) << "\n";
    return os;
}

std::ostream &operator<<(std::ostream &os, SdcLevelShifterThresholdContainer &rhs) {
    os  << "set_level_shifter_threshold ";
    os  << "-voltage " << rhs.getVoltage()
        << "-percent " << rhs.getPercent() 
        << "\n";
    return os;
}

const float SdcMaxDynamicPowerContainer::getCellPower(const ObjectId &cell_id) const {
    if (!data_) {
        // error message
        return 0.0; 
    }
    const auto &cell_power = data_->getDynamicPower();
    const auto &found = cell_power.find(cell_id);
    if (found == cell_power.end()) {
        // message
        return 0.0;
    }
    const auto &power = found->second;
    if (!power) {
        // message
        return 0.0;
    }
    return power->getPowerValue();
}

std::ostream &operator<<(std::ostream &os, SdcMaxDynamicPowerContainer &rhs) {
    if (!rhs.data_) {
        // error message
        return os; 
    }
    const auto &cell_power_map = rhs.data_->getDynamicPower();
    for (const auto &cell_to_power : cell_power_map) {
        const auto &cell_id = cell_to_power.first;
        const auto &cell = Object::addr<Cell>(cell_id);
        if (!cell) {
            // message
            continue;
        }
        const auto &cell_name = cell->getName();
        const auto &power_value = cell_to_power.second;
        os  << "set_max_dynamic_power ";
        os  << "-power " << power_value 
            << "-unit " << "W" 
            << "#cell_name " << cell_name 
            << "\n";
    }
    return os;
}

const float SdcMaxLeakagePowerContainer::getCellPower(const ObjectId &cell_id) const {
    if (!data_) {
        // error message
        return 0.0; 
    }
    const auto &cell_power = data_->getLeakagePower();
    const auto &found = cell_power.find(cell_id);
    if (found == cell_power.end()) {
        // message
        return 0.0;
    }
    const auto &power = found->second;
    if (!power) {
        // message
        return 0.0;
    }
    return power->getPowerValue();
}

std::ostream &operator<<(std::ostream &os, SdcMaxLeakagePowerContainer &rhs) {
    if (!rhs.data_) {
        // error message
        return os; 
    }
    const auto &cell_power_map = rhs.data_->getLeakagePower();
    for (const auto &cell_to_power : cell_power_map) {
        const auto &cell_id = cell_to_power.first;
        const auto &cell = Object::addr<Cell>(cell_id);
        if (!cell) {
            // message
            continue;
        }
        const auto &cell_name = cell->getName();
        const auto &power_value = cell_to_power.second;
        os  << "set_max_leakage_power ";
        os  << "-power " << power_value 
            << "-unit " << "W" 
            << "#cell_name " << cell_name 
            << "\n";
    }
    return os;
}



//object access commands
std::ostream &operator<<(std::ostream &os, SdcAllClocksContainer &rhs) {
    os << "all_clocks ";
    os << "# {";
    const auto &clock_names = rhs.get_all_clocks();
    copy(clock_names.begin(), clock_names.end(), std::ostream_iterator<std::string>(os, " "));
    os << " }";
    os << "\n";
    return os;
}

std::string SdcCurrentDesignContainer::getName() {
    const ObjectId &cell_id = current_design_->getCellId(); 
    Cell* cell = Object::addr<Cell>(cell_id);
    if (cell) {
        return cell->getName();
    }
    //TODO return error messages
    return "";
}

std::ostream &operator<<(std::ostream &os, SdcCurrentDesignContainer &rhs) {
    os << "current_design " << rhs.getName() << "\n";
    return os;
}


}
}
