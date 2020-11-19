/** 
 * @file nets_parasitics.cpp
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

#include "db/timing/spef/nets_parasitics.h"

#include "stdlib.h"
#include "db/core/db.h"
#include "util/stream.h"

namespace open_edi {
namespace db {

NetsParasitics::NetsParasitics()
    : NetsParasitics::BaseType(),
      netParasiticsMap_(),
      nameMap_(),
      designFlowVec_(10,""),
      pgNetVec_(10,UNINIT_OBJECT_ID),
      portsVec_(100,UNINIT_OBJECT_ID),
      divider_('\0'),
      delimiter_('\0'),
      preBusDel_('\0'),
      sufBusDel_('\0'),
      cellId_(UNINIT_OBJECT_ID),
      timeScale_(1.0),
      resScale_(1.0),
      capScale_(1.0),
      inductScale_(1.0) {
    setObjectType(ObjectType::kObjectTypeNetsParasitics);
}

NetsParasitics::~NetsParasitics() {
}

NetsParasitics::NetsParasitics(Object* owner, NetsParasitics::IndexType id)
    : NetsParasitics::BaseType(owner, id),
      netParasiticsMap_(),
      nameMap_(),
      designFlowVec_(10,""),
      pgNetVec_(10,UNINIT_OBJECT_ID),
      portsVec_(100,UNINIT_OBJECT_ID),
      divider_('\0'),
      delimiter_('\0'),
      preBusDel_('\0'),
      sufBusDel_('\0'),
      cellId_(UNINIT_OBJECT_ID),
      timeScale_(1.0),
      resScale_(1.0),
      capScale_(1.0),
      inductScale_(1.0) {
    setObjectType(ObjectType::kObjectTypeNetsParasitics);
}

NetsParasitics::NetsParasitics(NetsParasitics const& rhs) { copy(rhs); }

NetsParasitics::NetsParasitics(NetsParasitics&& rhs) noexcept { move(std::move(rhs)); }

NetsParasitics& NetsParasitics::operator=(NetsParasitics const& rhs) {
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

NetsParasitics& NetsParasitics::operator=(NetsParasitics&& rhs) noexcept {
    if (this != &rhs) {
        move(std::move(rhs));
    }
    return *this;
}

void NetsParasitics::copy(NetsParasitics const& rhs) {
    this->BaseType::copy(rhs);

    netParasiticsMap_ = rhs.netParasiticsMap_;
    nameMap_ = rhs.nameMap_;
    designFlowVec_ = rhs.designFlowVec_;
    pgNetVec_ = rhs.pgNetVec_;
    portsVec_ = rhs.portsVec_;
    divider_ = rhs.divider_;
    delimiter_ = rhs.delimiter_;
    preBusDel_ = rhs.preBusDel_;
    sufBusDel_ = rhs.sufBusDel_;
    cellId_ = rhs.cellId_;
    timeScale_ = rhs.timeScale_;
    resScale_ = rhs.resScale_;
    capScale_ = rhs.capScale_;
    inductScale_ = rhs.inductScale_;
}

void NetsParasitics::move(NetsParasitics&& rhs) {
    this->BaseType::move(std::move(rhs));
    netParasiticsMap_ = std::move(rhs.netParasiticsMap_);
    nameMap_ = std::move(rhs.nameMap_);
    designFlowVec_ = std::move(rhs.designFlowVec_);
    pgNetVec_ = std::move(rhs.pgNetVec_);
    portsVec_ = std::move(rhs.portsVec_);
    divider_ = std::move(rhs.divider_);
    delimiter_ = std::move(rhs.delimiter_);
    preBusDel_ = std::move(rhs.preBusDel_);
    sufBusDel_ = std::move(rhs.sufBusDel_);
    cellId_ = std::move(rhs.cellId_);
    timeScale_ = std::move(rhs.timeScale_);
    resScale_ = std::move(rhs.resScale_);
    capScale_ = std::move(rhs.capScale_);
    inductScale_ = std::move(rhs.inductScale_);
    rhs.netParasiticsMap_.clear();
    rhs.nameMap_.clear();
}

bool NetsParasitics::isDigits(const char *str)
{
  for (const char *s = str; *s; s++) {
    if (!isdigit(*s))
      return false;
  }
  return true;
}

Net* NetsParasitics::findNet(const char *netName) {
    Cell *topCell = getTopCell();   //Need to use current cell in future
    if (topCell && netName) {
	std::string netStr = netName;
	if (netName[0] == '*') {
	    uint32_t idx = strtoul(netName+1, NULL, 0);
	    if (nameMap_.find(idx) != nameMap_.end()) {
	        netStr = topCell->getSymbolByIndex(nameMap_[idx]);
		return topCell->getNet(netStr);
            } 
        } else {
	    return topCell->getNet(netStr); 
	}
    }
    return nullptr;
}

Pin* NetsParasitics::findPin(const char *pinName) {
    Cell *topCell = getTopCell();   //Need to use current cell in future
    if (topCell && pinName) {
        std::string pinStr = pinName;
	std::size_t found = pinStr.find_last_of(delimiter_);
	if (found != std::string::npos) {
            if (pinName[0] == '*') {
                uint32_t idx = strtoul(pinStr.substr(1, found).c_str(), NULL, 0);
                if (nameMap_.find(idx) != nameMap_.end()) {
		    std::string instStr = topCell->getSymbolByIndex(nameMap_[idx]);
		    Inst *inst = topCell->getInstance(instStr);
                    if (inst) 
			return inst->getPin(pinStr.substr(found+1));
                }
            } else {  //Name map doesn't exist
		Inst *inst = topCell->getInstance(pinStr.substr(0, found));
		if (inst)
		    return inst->getPin(pinStr.substr(found+1));
	    }
        } else {  //Handle IO pin
            if (pinName[0] == '*') {
		uint32_t idx = strtoul(pinName, NULL, 0);
		if (nameMap_.find(idx) != nameMap_.end()) {
		    std::string ioPinStr = topCell->getSymbolByIndex(nameMap_[idx]);
		    return topCell->getIOPin(ioPinStr);
                } 
	    } else 
                return topCell->getIOPin(pinStr); 
        }
    }
    return nullptr;
}

ObjectId  NetsParasitics::addNode(DNetParasitics *netParasitics, const char *nodeName) {
    Cell *topCell = getTopCell();   //Need to use current cell in future
    if (topCell && nodeName) {
        std::string nodeStr = nodeName;
        std::size_t found = nodeStr.find_last_of(delimiter_);
        Pin *pin = nullptr;
        Net *net = nullptr;
        if (found != std::string::npos) {
	    if (isDigits(nodeStr.substr(found+1).c_str())) {  //Never see use all number for term name
                net = findNet(nodeStr.substr(0, found).c_str());
                if (net == nullptr)
                    return UNINIT_OBJECT_ID;
		else if (net->getId() == netParasitics->getNetId()) { //Internal node
		    uint32_t intNodeId = strtoul(nodeStr.substr(found+1).c_str(), NULL, 0);
                    return netParasitics->addIntNode(intNodeId);
		} else {  //External node
		    uint32_t extNodeId = strtoul(nodeStr.substr(found+1).c_str(), NULL, 0);
		    return netParasitics->addExtNode(net->getId(), extNodeId);
		}
            } else { //Pin node
		pin = findPin(nodeName);
		if (pin != nullptr) {
		    if (pin->getNet()->getId() != netParasitics->getNetId()) //Add External pin node
			return netParasitics->addPinNode(pin->getId(), true);  
		} 
	    }
        } else { //To handle IO pin
	    pin = findPin(nodeName);
            if (pin != nullptr) {
                if (pin->getNet()->getId() != netParasitics->getNetId()) //Add External pin node
                    return netParasitics->addPinNode(pin->getId(), true);
            } 
	}
    }
    return UNINIT_OBJECT_ID;
}

DNetParasitics* NetsParasitics::addDNetParasitics(ObjectId netId, float totCap) {
    Cell *topCell = getTopCell();
    if (topCell) {
	auto netPara = topCell->createObject<DNetParasitics>(kObjectTypeDNetParasitics);
        if (netPara) {
	    netPara->setOwner(this);
            netPara->setNetId(netId);
	    netPara->setNetTotalCap(totCap);
            ObjectId netParaId = netPara->getId();
            netParasiticsMap_[netId] = netParaId;
            return netPara;
	}
    }
    return nullptr;
}

RNetParasitics* NetsParasitics::addRNetParasitics(ObjectId netId, float totCap) {
    Cell *topCell = getTopCell();
    if (topCell) {
        auto netPara = topCell->createObject<RNetParasitics>(kObjectTypeRNetParasitics);
        if (netPara) {
            netPara->setOwner(this);
            netPara->setNetId(netId);
            netPara->setNetTotalCap(totCap);
            ObjectId netParaId = netPara->getId();
            netParasiticsMap_[netId] = netParaId;
            return netPara;
        }
    }
    return nullptr;
}

OStreamBase& operator<<(OStreamBase& os, NetsParasitics const& rhs) {
    os << DataTypeName(className(rhs)) << DataBegin("(");

    NetsParasitics::BaseType const& base = rhs;
    os << base << DataDelimiter();
  
    return os;
}

}  // namespace db
}  // namespace open_edi
