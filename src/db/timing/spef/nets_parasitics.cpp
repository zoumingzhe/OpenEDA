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

#include <stdio.h>
#include <time.h>
#include "stdlib.h"
#include "db/core/db.h"
#include "db/core/timing.h"
#include "util/stream.h"

namespace open_edi {
namespace db {

NetsParasitics::NetsParasitics()
    : NetsParasitics::BaseType(),
      netParasiticsMap_(),
      nameMap_(),
      revertNameMap_(),
      revertPortsMap_(),
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
      revertNameMap_(),
      revertPortsMap_(),
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
    revertNameMap_ = rhs.revertNameMap_;
    revertPortsMap_ = rhs.revertPortsMap_;
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
    revertNameMap_ = std::move(rhs.revertNameMap_);
    revertPortsMap_ = std::move(rhs.revertPortsMap_);
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

Net* NetsParasitics::getNetBySymbol(SymbolIndex index) {
    Cell *cell = Object::addr<Cell>(cellId_);
    if (cell) {
        std::vector<ObjectId> objectVec = cell->getSymbolTable()->getReferences(index);
        for (auto obj : objectVec) {
            Net *net = Object::addr<Net>(obj);
            if (net && net->getObjectType() == kObjectTypeNet) {
                return net;
            }
        }
    }
    return nullptr;
}   

Net* NetsParasitics::findNet(const char *netName) {
    //Cell *topCell = getTopCell();   //Need to use current cell in future
    Cell *cell = Object::addr<Cell>(cellId_);
    Net *net = nullptr;
    std::string netStr = netName;

    if (cell && netName) {
	if (netName[0] == '*') {
	    uint32_t idx = strtoul(netName+1, NULL, 0);
	    if (nameMap_.find(idx) != nameMap_.end()) {
		net = getNetBySymbol(nameMap_[idx]);
            } 
        } else 
	    net = cell->getNet(netStr); 
    }
    /*if (net == nullptr) {
        std::string errMsg = "Can't find net " + netStr + " in the design and its parasitics will be ignored.\n";
        open_edi::util::message->issueMsg(open_edi::util::kError, errMsg.c_str());
    }*/
    return net;
}

Pin* NetsParasitics::getPinBySymbol(SymbolIndex index, const std::string& pinName) {
    Cell *cell = Object::addr<Cell>(cellId_);
    if (cell) {
        std::vector<ObjectId> objectVec = cell->getSymbolTable()->getReferences(index);
	for (auto obj : objectVec) {
	    Inst *inst = Object::addr<Inst>(obj);
            if (inst && inst->getObjectType() == kObjectTypeInst) {
		return inst->getPin(pinName);	
	    }
	}
    }
    return nullptr;
}

Pin* NetsParasitics::getPortBySymbol(SymbolIndex index) {
    Cell *cell = Object::addr<Cell>(cellId_);
    if (cell) {
        std::vector<ObjectId> objectVec = cell->getSymbolTable()->getReferences(index);
        for (auto obj : objectVec) {
            Pin *pin = Object::addr<Pin>(obj);
            if (pin && pin->getObjectType() == kObjectTypePin) {
                return pin;
            }
        }
    }
    return nullptr;
}

Pin* NetsParasitics::findPin(const char *pinName) {
    Cell *cell = Object::addr<Cell>(cellId_);
    if (cell && pinName) {
        std::string pinStr = pinName;
	std::size_t found = pinStr.find_last_of(delimiter_);
	if (found != std::string::npos) {
            if (pinName[0] == '*') {
                uint32_t idx = strtoul(pinStr.substr(1, found).c_str(), NULL, 0);
                if (nameMap_.find(idx) != nameMap_.end()) 
		    return getPinBySymbol(nameMap_[idx], pinStr.substr(found+1));
            } else {  //Name map doesn't exist
		Inst *inst = cell->getInstance(pinStr.substr(0, found));
		if (inst)
		    return inst->getPin(pinStr.substr(found+1));
	    }
        } else {  //Handle IO pin
            if (pinName[0] == '*') {
		uint32_t idx = strtoul(pinName+1, NULL, 0);
		if (nameMap_.find(idx) != nameMap_.end()) {
		    return getPortBySymbol(nameMap_[idx]);
                } 
	    } else 
                return cell->getIOPin(pinStr); 
        }
    }
    return nullptr;
}

ObjectId  NetsParasitics::createParaNode(DNetParasitics *netParasitics, const char *nodeName) {
    //Cell *topCell = getTopCell();   //Need to use current cell in future
    Cell *cell = Object::addr<Cell>(cellId_);
    if (cell && nodeName) {
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
                    return netParasitics->createIntNode(intNodeId);
		} else {  //External node
		    uint32_t extNodeId = strtoul(nodeStr.substr(found+1).c_str(), NULL, 0);
		    return netParasitics->createExtNode(net->getId(), extNodeId);
		}
            } else { //Pin node
		pin = findPin(nodeName);
		if (pin != nullptr) {
		    ObjectId pinNodeId = netParasitics->createPinNode(pin->getId());
                    /*if (pin->getNet()->getId() == netParasitics->getNetId()) { //Add internal pin node
                        netParasitics->addPinNode(pinNodeId);
		    }*/
                    return pinNodeId;
		}
	    }
        } else { //To handle IO pin
	    pin = findPin(nodeName);
            if (pin != nullptr) {
		ObjectId pinNodeId = netParasitics->createPinNode(pin->getId());
                /*if (pin->getNet()->getId() == netParasitics->getNetId()) { //Add internal pin node
                    netParasitics->addPinNode(pinNodeId);
                }*/
		return pinNodeId;
	    } 
	}
    }
    return UNINIT_OBJECT_ID;
}

DNetParasitics* NetsParasitics::addDNetParasitics(ObjectId netId, float totCap) {
    //Cell *topCell = getTopCell();
    //Cell *cell = Object::addr<Cell>(cellId_);
    Timing *timingdb = getTimingLib();
    if (timingdb) {
	auto netPara = timingdb->createObject<DNetParasitics>(kObjectTypeDNetParasitics, timingdb->getId());
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
    //Cell *topCell = getTopCell();
    //Cell *cell = Object::addr<Cell>(cellId_);
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        auto netPara = timingdb->createObject<RNetParasitics>(kObjectTypeRNetParasitics, timingdb->getId());
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

///Functions for SPEF dumpping
std::string NetsParasitics::getNetDumpName(Net *net) {
    std::string netName = net->getName();
    if (revertNameMap_.find(netName) != revertNameMap_.end())
        netName = "*" + std::to_string(revertNameMap_[netName]);   
    return netName;
}

std::string NetsParasitics::getCellDumpName(Cell *cell) { //for RNet
    std::string cellName = cell->getName();
    if (revertNameMap_.find(cellName) != revertNameMap_.end())
        cellName = "*" + std::to_string(revertNameMap_[cellName]);
    return cellName;
}

std::string NetsParasitics::getIntNodeDumpName(Net *net, ParasiticIntNode *intNode) {
    std::string intNodeName = getNetDumpName(net);
    intNodeName += std::string(1,getDelimiter());
    intNodeName += std::to_string(intNode->getIntNodeId());
    return intNodeName; 
}

std::string NetsParasitics::getPinDumpName(Pin *pin) {
    std::string pinName = pin->getName();
    if (revertPortsMap_.find(pinName) != revertPortsMap_.end())
        pinName = "*" + std::to_string(revertPortsMap_[pinName]);
    else {
	Inst *inst = pin->getInst();
        if (inst) {
	    std::string instName = inst->getName();
            if (revertNameMap_.find(instName) != revertNameMap_.end()) {
                pinName = "*" + std::to_string(revertNameMap_[instName]);
                pinName += std::string(1,getDelimiter());
                pinName += pin->getTerm()->getName();
            }
	}
    }
    return pinName;
}

std::string NetsParasitics::getTermDirDumpName(Pin *pin) {
    std::string termDirStr = "B";
    Term *term = pin->getTerm();
    if (term) {
        if (term->getDirectionStr() == "Input")
            termDirStr = "I";
        else if (term->getDirectionStr() == "Output")
            termDirStr = "O";
    }
    return termDirStr;
}

std::string NetsParasitics::getExtNodeDumpName(ParasiticExtNode *extNode) {
    Net *net = Object::addr<Net>(extNode->getExtNetId());
    std::string extNodeName = getNetDumpName(net);
    extNodeName += std::to_string(delimiter_); 
    extNodeName += std::to_string(extNode->getExtNodeId());
    return extNodeName;
}

std::string NetsParasitics::getNodeDumpName(Net *net, ObjectId objId) {
    ParasiticNode *unObj = Object::addr<ParasiticNode>(objId);
    std::string dumpName = "";
    if (unObj->getObjectType() == kObjectTypeParasiticIntNode) {
        ParasiticIntNode *intNode = Object::addr<ParasiticIntNode>(objId);
        dumpName = getIntNodeDumpName(net, intNode);
    } else if (unObj->getObjectType() == kObjectTypeParasiticPinNode) {
        ParasiticPinNode *pinNode = Object::addr<ParasiticPinNode>(objId);
        Pin *pin = Object::addr<Pin>(pinNode->getPinId());
        dumpName = getPinDumpName(pin);
    } else if (unObj->getObjectType() == kObjectTypeParasiticExtNode) {
        ParasiticExtNode *extNode = Object::addr<ParasiticExtNode>(objId);
        dumpName = getExtNodeDumpName(extNode);
    }
    return dumpName;
}

void NetsParasitics::dumpSpefHeader(std::ofstream& os) {
    os << ("*SPEF \"IEEE 1481-2009\"\n");

    Cell *cell = Object::addr<Cell>(cellId_);
    std::string cellName = cell->getName();  //Waiting for DB team to fix this issue
    os << ("*DESIGN \"") << (cellName) << ("\"\n");

    time_t rawtime;
    struct tm *timeinfo;
    char timeStr[100];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timeStr, 100, "%a %h %d %T %Y", timeinfo);
    os << ("*DATE \"") << (timeStr) << ("\"\n");
    os << ("*VENDOR \"NIIC EDA\"\n");
    os << ("*PROGRAM \"openEDA\"\n");
    os << ("*VERSION \"1.0\"\n");
    os << ("*DESIGN_FLOW");
    for (auto &str : designFlowVec_) 
        os << (" ") << ("\"") << (str) << ("\"");

    os << ("\n");
    os << ("*DIVIDER ") << (std::string(1,getDivider())) << ("\n");
    os << ("*DELIMITER ") << (std::string(1,getDelimiter())) << ("\n");
    os << ("*BUS_DELIMITER ") << (std::string(1,getPreBusDel()));
    if (getSufBusDel() != '\0')
        os << (std::string(1,getSufBusDel()));
    os << ("\n");
    os << ("*T_UNIT ") << (std::to_string(getTimeScale()*1e12)) << (" PS\n");
    os << ("*C_UNIT ") << (std::to_string(getCapScale()*1e12)) << (" PF\n");
    os << ("*R_UNIT ") << (std::to_string(getResScale())) << (" OHM\n");
    os << ("*L_UNIT ") << (std::to_string(getInductScale())) << (" HENRY\n\n");
}

void NetsParasitics::dumpNameMap(std::ofstream& os) {
     if (!nameMap_.empty()) {
        Cell *cell = Object::addr<Cell>(cellId_);
        os << ("*NAME_MAP\n\n");
        for (auto obj : nameMap_) {
            std::string name = cell->getSymbolByIndex(obj.second);
            os << ("*") << (std::to_string(obj.first));
            os << (" ") << (name) << ("\n");
            revertNameMap_[name] = obj.first;
        }
        os << ("\n\n");
    }
}

void NetsParasitics::dumpPorts(std::ofstream& os) {
    if (!portsVec_.empty()) {
        os << ("*PORTS\n\n");
        for (auto obj : portsVec_) {
            Pin *pin = Object::addr<Pin>(obj);
            std::string pinName = pin->getName();
            if (revertNameMap_.find(pinName) != revertNameMap_.end()) {  //To use index instead of port name directly
		revertPortsMap_[pinName] = revertNameMap_[pinName];
                pinName = std::to_string(revertNameMap_[pinName]);
            }

            os << ("*") << (pinName) << (" ");
            os << (getTermDirDumpName(pin)) << ("\n");
        }
        os << ("\n");
    }
}

void NetsParasitics::dumpDNetConn(std::ofstream& os, DNetParasitics *dNetPara) {
    os << ("*CONN\n");
    ObjectId pinNodeVecId = dNetPara->getPinNodeVecId(); 
    if (pinNodeVecId != UNINIT_OBJECT_ID) {
	ArrayObject<ObjectId> *objVector = addr< ArrayObject<ObjectId> >(pinNodeVecId);
        for (auto obj : *objVector) {
            ParasiticPinNode *pinNode = Object::addr<ParasiticPinNode>(obj);
            if (pinNode) {
                Pin *pin = Object::addr<Pin>(pinNode->getPinId());
                if (pin != nullptr) {
                    std::string pinName = pin->getName();
                    if (revertPortsMap_.find(pinName) != revertPortsMap_.end())
                        os << ("*P ");
                    else
                        os << ("*I ");

                    os << (getPinDumpName(pin)) << (" "); // << ("\n");
                    os << (getTermDirDumpName(pin)) << ("\n");
	        }
	    }
        }
    }
    os << ("\n");
}

void NetsParasitics::dumpDNetCap(std::ofstream& os, DNetParasitics *dNetPara) {
    Net *net = Object::addr<Net>(dNetPara->getNetId());
    os << ("*CAP\n\n");
    uint32_t capNo = 0;
    ObjectId gndCapVecId = dNetPara->getGroundCapVecId();
    if (gndCapVecId != UNINIT_OBJECT_ID) {
	ArrayObject<ObjectId> *objVector = addr< ArrayObject<ObjectId> >(gndCapVecId);	
        for (auto obj : *objVector) {
            capNo++;
            ParasiticCap *gCap = Object::addr<ParasiticCap>(obj);
            os << (std::to_string(capNo)) << (" ");
            os << (getNodeDumpName(net, gCap->getNode1Id()));
            os << (" ") << (std::to_string(gCap->getCapacitance())) << ("\n");
        }
    }

    ObjectId xCapVecId = dNetPara->getCouplingCapVecId();
    if (xCapVecId != UNINIT_OBJECT_ID) {
	ArrayObject<ObjectId> *objVector = addr< ArrayObject<ObjectId> >(xCapVecId);
        for (auto obj : *objVector) {
            capNo++;
            ParasiticXCap *xCap = Object::addr<ParasiticXCap>(obj);
            os << (std::to_string(capNo)) << (" ");
            os << (getNodeDumpName(net, xCap->getNode1Id())) << (" ");
            os << (getNodeDumpName(net, xCap->getNode2Id()));
            os << (" ") << (std::to_string(xCap->getCapacitance())) << ("\n");
        }
    }
    os << ("\n"); 
}

void NetsParasitics::dumpDNetRes(std::ofstream& os, DNetParasitics *dNetPara) {
    Net *net = Object::addr<Net>(dNetPara->getNetId());
    os << ("*RES\n\n");
    uint32_t resNo = 0;
    ObjectId resVecId = dNetPara->getResistorVecId();
    if (resVecId != UNINIT_OBJECT_ID) {
	ArrayObject<ObjectId> *objVector = addr< ArrayObject<ObjectId> >(resVecId);
        for (auto obj : *objVector) {
            resNo++;
	    ParasiticResistor *res = Object::addr<ParasiticResistor>(obj);
            os << (std::to_string(resNo)) << (" ");
            os << (getNodeDumpName(net, res->getNode1Id())) << (" ");
            os << (getNodeDumpName(net, res->getNode2Id()));
            os << (" ") << (std::to_string(res->getResistance())) << ("\n");
        }
    }
    os << ("\n");
}

void NetsParasitics::dumpDNet(std::ofstream& os, DNetParasitics *dNetPara) {
    Net *net = Object::addr<Net>(dNetPara->getNetId());
    std::string netName = getNetDumpName(net);
    os << ("*D_NET ") << (netName) << (" ");
    os << (std::to_string(dNetPara->getNetTotalCap())) << ("\n\n"); 

    dumpDNetConn(os, dNetPara);
    dumpDNetCap(os, dNetPara);
    dumpDNetRes(os, dNetPara);   
    os << ("*END\n\n");
}

void NetsParasitics::dumpRNet(std::ofstream& os, RNetParasitics *rNetPara) {
    Net *net = Object::addr<Net>(rNetPara->getNetId());
    std::string netName = getNetDumpName(net);
    os << ("*R_NET ") << (netName);
    os << (std::to_string(rNetPara->getNetTotalCap())) << ("\n\n");

    Pin *pin = Object::addr<Pin>(rNetPara->getDriverPinId());   
    Cell *cell = pin->getInst()->getMaster(); 
    os << ("*DRIVER ") << (getPinDumpName(pin)) << ("\n");
    os << ("*CELL ") << (getCellDumpName(cell)) << ("\n");
    os << ("*C2_R1_C1 ") << (std::to_string(rNetPara->getC2()));
    os << (" ") << (std::to_string(rNetPara->getR1()));
    os << (" ") << (std::to_string(rNetPara->getC1()));
    os << ("\n");
    
    os << ("*END\n\n");
}

void NetsParasitics::dumpNets(std::ofstream& os) {
    for (auto obj : netParasiticsMap_) {
        Net *net = Object::addr<Net>(obj.first);
        NetParasitics *unObj = Object::addr<NetParasitics>(obj.second);  //Need to check further
        if (unObj->getObjectType() == ObjectType::kObjectTypeDNetParasitics) {
            DNetParasitics *dNetPara = Object::addr<DNetParasitics>(obj.second);
            dumpDNet(os, dNetPara);
        } else if (unObj->getObjectType() == ObjectType::kObjectTypeRNetParasitics) {
            RNetParasitics *rNetPara = Object::addr<RNetParasitics>(obj.second);
            dumpRNet(os, rNetPara);
        }
    }
}

std::ofstream& operator<<(std::ofstream& os, NetsParasitics &rhs) {
 
    rhs.dumpSpefHeader(os);
   
    rhs.dumpNameMap(os);

    rhs.dumpPorts(os);

    rhs.dumpNets(os);

    return os;
}

}  // namespace db
}  // namespace open_edi
