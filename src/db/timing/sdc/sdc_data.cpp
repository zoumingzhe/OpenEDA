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


std::string SdcCurrentInstanceContainer::getName() {
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
    return os;
}


//object access commands
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

// environment commands
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

// set_case_analysis
void SdcCaseAnalysisContainer::add(const ObjectId &pin_id, const SetCaseAnalysisPtr &case_analysis_ptr) {
    assert(case_analysis_ptr);
    pin_to_case_analysis_.emplace(pin_id, case_analysis_ptr);
}

CaseAnalysisValue SdcCaseAnalysisContainer::getPinValue(const ObjectId &pin_id) {
    auto found = pin_to_case_analysis_.find(pin_id); 
    if (found != pin_to_case_analysis_.end()) {
        auto case_analysis_ptr = found->second;
        assert(case_analysis_ptr);
        if (case_analysis_ptr != nullptr) {
            return case_analysis_ptr->getValue(); 
        }
    }
    return CaseAnalysisValue::kUnknown;
}

void SdcCaseAnalysisContainer::resetPinValue(const ObjectId &pin_id, const CaseAnalysisValue &value) {
    auto found = pin_to_case_analysis_.find(pin_id); 
    if (found != pin_to_case_analysis_.end()) {
        auto case_analysis_ptr = found->second;
        assert(case_analysis_ptr);
        if (case_analysis_ptr != nullptr) {
            case_analysis_ptr->setValue(value); 
        }
    }
}

bool SdcCaseAnalysisContainer::isPinConstZero(const ObjectId &pin_id) {
    const auto& value = getPinValue(pin_id); 
    return (value == CaseAnalysisValue::kZero); 
} 

bool SdcCaseAnalysisContainer::isPinConstOne(const ObjectId &pin_id) {
    const auto& value = getPinValue(pin_id); 
    return (value == CaseAnalysisValue::kOne); 
}

std::ostream &operator<<(std::ostream &os, SdcCaseAnalysisContainer &rhs) {
    for ( auto& pin_value : rhs.pin_to_case_analysis_ ) {
        auto& pin_id = pin_value.first;
        Pin* pin_ptr = Object::addr<Pin>(pin_id); 
        assert(pin_ptr);
        if (pin_ptr == nullptr) {
            //TODO messages
            continue;
        }
        auto& case_analysis_ptr = pin_value.second;
        assert(case_analysis_ptr);
        if (case_analysis_ptr == nullptr) {
            //TODO messages
            continue;
        }
        os << "set_case_analysis " << *case_analysis_ptr << " " << pin_ptr->getName() << "\n";
    }
    return os;
}


//
}
}
