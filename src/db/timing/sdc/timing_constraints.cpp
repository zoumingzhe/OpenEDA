/** 
 * @file timing_constraints.cpp
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

#include "db/timing/sdc/timing_constraints.h"

namespace open_edi {
namespace db {

bool CreateGeneratedClock::addSourceMasterPin(const std::string &pin_name) {
    const auto &pin = getPinByFullName(pin_name); 
    if (!pin) {
        return false;
    }
    const auto &pin_id = pin->getId();
    source_master_pins_.emplace_back(pin_id);
    return true;
}

void ClockContainerData::addClock(ClockPtr &clock, CreateClockPtr &create_clock) {
    const ClockId &clock_id = clock_ids_.size();
    clock->setId(clock_id);
    clock_ids_.emplace_back(clock_id);
    const std::string &clock_name = clock->getName();
    clock_names_.emplace_back(clock_name);
    name_to_id_.emplace(clock_name, clock_id);
    clocks_.emplace_back(clock);
    create_clocks_.emplace(clock_id, create_clock);
}

void ClockContainerData::addClock(ClockPtr &clock, CreateGeneratedClockPtr &create_generated_clock) {
    const ClockId &clock_id = clock->getId();
    const std::string &clock_name = clock->getName();
    clock_ids_.emplace_back(clock_id);
    clock_names_.emplace_back(clock_name);
    name_to_id_.emplace(clock_name, clock_id);
    clocks_.emplace_back(clock);
    create_generated_clocks_.emplace(clock_id, create_generated_clock);
}

bool ClockContainerData::addClockPin(const std::string &pin_name, const ClockPtr &clock) {
    const auto &pin = getPinByFullName(pin_name);
    if (!pin) {
        return false;
    }
    const auto &pin_id = pin->getId();
    if (!clock->isAdd()) {
        auto found = pin_clock_map_.find(pin_id);
        if (found != pin_clock_map_.end()) {
            found->second = clock;
            return true;
        }
    }
    pin_clock_map_.emplace(pin_id, clock);
    return true;
}

ExceptionPath::ExceptionPath() {
    from_ = std::make_shared<PathNodes>(); 
    to_ = std::make_shared<PathNodes>();
    through_ = std::make_shared<PathThroughNodes>();
}

GroupPath::GroupPath() {
    from_ = std::make_shared<PathNodes>(); 
    to_ = std::make_shared<PathNodes>();
    through_ = std::make_shared<PathThroughNodes>();
}

bool GroupPath::addFromPinNode(const std::string &pin_name) {
    const auto &pin = getPinByFullName(pin_name);
    if (!pin) {
        return false;
    }
    const auto &pin_id = pin->getId();
    from_->addPin(pin_id);
    return true;
}

bool GroupPath::addFromInstNode(const std::string &inst_name) {
    const auto &top_cell = getTopCell();
    const auto &inst = top_cell->getInstance(inst_name);
    if (!inst) {
        return false;
    }
    const auto &inst_id = inst->getId();
    from_->addInst(inst_id);
    return true;
}

void GroupPath::addFromClockNode(const ClockId &clock_id) {
   from_->addClock(clock_id); 
}

void GroupPath::setRiseFrom() {
    from_->setRise();
}

void GroupPath::setFallFrom() {
    from_->setFall();
}

void GroupPath::setRiseFallFrom() {
    from_->setRise();
    from_->setFall();
}

bool GroupPath::addToPinNode(const std::string &pin_name) {
    const auto &pin = getPinByFullName(pin_name);
    if (!pin) {
        return false;
    }
    const auto &pin_id = pin->getId();
    to_->addPin(pin_id);
    return true;
}

bool GroupPath::addToInstNode(const std::string &inst_name) {
    const auto &top_cell = getTopCell();
    const auto &inst = top_cell->getInstance(inst_name);
    if (!inst) {
        return false;
    }
    const auto &inst_id = inst->getId();
    to_->addInst(inst_id);
    return true;
}

void GroupPath::addToClockNode(const ClockId &clock_id) {
   to_->addClock(clock_id); 
}

void GroupPath::setRiseTo() {
    to_->setRise();
}

void GroupPath::setFallTo() {
    to_->setFall();
}

void GroupPath::setRiseFallTo() {
    to_->setRise();
    to_->setFall();
}

bool GroupPath::addThroughPinNode(const std::string &pin_name) {
    const auto &pin = getPinByFullName(pin_name);
    if (!pin) {
        return false;
    }
    const auto &pin_id = pin->getId();
    through_->addPin(pin_id);
    return true;
}

bool GroupPath::addThroughInstNode(const std::string &inst_name) {
    const auto &top_cell = getTopCell();
    const auto &inst = top_cell->getInstance(inst_name);
    if (!inst) {
        return false;
    }
    const auto &inst_id = inst->getId();
    through_->addInst(inst_id);
    return true;
}

void GroupPath::addThroughClockNode(const ClockId &clock_id) {
   through_->addClock(clock_id); 
}

void GroupPath::setRiseThrough() {
    through_->setRise();
}

void GroupPath::setFallThrough() {
    through_->setFall();
}

void GroupPath::setRiseFallThrough() {
    through_->setRise();
    through_->setFall();
}

void SetClockGatingCheck::checkFlags() {
    if (!isRise() && !isFall()) {
        setRise();
        setFall();
    }
}

bool ClockGatingCheckContainerData::addToPin(const std::string &pin_name, const SetClockGatingCheckPtr &check) {
    const auto &pin = getPinByFullName(pin_name);
    if (!pin) {
        return false;
    }
    const auto &pin_id = pin->getId();
    pin_to_check_.emplace(pin_id, check);
    return true;
}

bool ClockGatingCheckContainerData::addToInst(const std::string &inst_name, const SetClockGatingCheckPtr &check) {
    const auto &top_cell = getTopCell(); 
    const auto &inst = top_cell->getInstance(inst_name); 
    if (!inst) {
        return false;
    }
    const auto &inst_id = inst->getId();
    inst_to_check_.emplace(inst_id, check);
    return true;
}

std::string to_string(const RelationshipType &value) {
    switch (value) {
        case RelationshipType::kPhysicallyExclusive :
            return "physically_exclusive";
        case RelationshipType::kAsynchronousAllowPaths :
            return "asynchronous_and_allow_paths";
        case RelationshipType::kAsynchronous :
            return "asynchronous";
        case RelationshipType::kLogicallyExclusive :
            return "logically_exclusive";
        defalut:
            return "unknown";
    }
    return "unknown";
}

void ClockGroupsContainerData::addClockRelationship(const UnorderedClockPair &clock_pair, const RelationshipType &relation) {
    //Priority from highest to lowest: 
    //Physically exclusive (kPhysicallyExclusive=0)
    //Asynchronous allow paths (kAsynchronousAllowPaths=1)
    //Asynchronous (kAsynchronous=2)
    //Logically exclusive (kLogicallyExclusive=3)
    auto found = clock_relationship_.find(clock_pair);
    if (found != clock_relationship_.end()) {
        const RelationshipType &exist_relation = found->second;
        if (exist_relation < relation) {
            found->second = relation;
        }
    }
    clock_relationship_.emplace(clock_pair, relation);
}

void ClockGroupsContainerData::setRelationBetweenClockGroups(const std::vector<ClockId> &lhs, const std::vector<ClockId> &rhs, const RelationshipType &relation) {
    for (const auto &clock_id : lhs) {
        for (const auto &other_clock_id : rhs) {
            if (clock_id == other_clock_id) {
                //error messages
                continue;
            }
            UnorderedClockPair clock_pair(clock_id, other_clock_id);
            addClockRelationship(clock_pair, relation);
        }
    }
}

void ClockGroupsContainerData::buildClockRelationship(const std::vector<ClockId> &all_clocks) {
    for (const auto &set_clock_groups : all_groups_) {
        const auto &groups = set_clock_groups->getGroups();
        const auto &group_num = groups.size();
        assert(!group_num);
        const RelationshipType &relation = set_clock_groups->getRelationType();
        if (group_num == 1) { //clocks in the group has relationship with all other clocks in the current design
            setRelationBetweenClockGroups(groups[0], all_clocks, relation);
        }
        if (group_num > 1) {
            size_t i = 0;
            for (; i < group_num-1; ++i) {
                size_t j = i+1;
                for (; j < group_num; ++j) {
                    setRelationBetweenClockGroups(groups[i], groups[j], relation);
                }
            }
        }
    }
}





bool SetPropagatedClock::addToPin(const std::string &pin_name) {
    const auto &pin = getPinByFullName(pin_name);
    if (!pin) {
        return false;
    }
    const auto &pin_id = pin->getId();
    pins_.emplace(pin_id);
    return true;
}



}
}
