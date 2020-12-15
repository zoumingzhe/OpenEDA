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
      name_map_(),
      revert_name_map_(),
      revert_ports_map_(),
      netparasitics_vec_id_(UNINIT_OBJECT_ID),
      nameid_vec_id_(UNINIT_OBJECT_ID),
      symidx_vec_id_(UNINIT_OBJECT_ID),
      design_flow_vec_id_(UNINIT_OBJECT_ID),
      ports_vec_id_(UNINIT_OBJECT_ID),
      cell_id_(UNINIT_OBJECT_ID),
      time_scale_(1.0),
      res_scale_(1.0),
      cap_scale_(1.0),
      induct_scale_(1.0),
      divider_('\0'),
      delimiter_('\0'),
      pre_bus_del_('\0'),
      suf_bus_del_('\0') {
    setObjectType(ObjectType::kObjectTypeNetsParasitics);
}

NetsParasitics::~NetsParasitics() {
}

NetsParasitics::NetsParasitics(Object* owner, NetsParasitics::IndexType id)
    : NetsParasitics::BaseType(owner, id),
      name_map_(),
      revert_name_map_(),
      revert_ports_map_(),
      netparasitics_vec_id_(UNINIT_OBJECT_ID),
      nameid_vec_id_(UNINIT_OBJECT_ID),
      symidx_vec_id_(UNINIT_OBJECT_ID),
      design_flow_vec_id_(UNINIT_OBJECT_ID),
      ports_vec_id_(UNINIT_OBJECT_ID),
      cell_id_(UNINIT_OBJECT_ID),
      time_scale_(1.0),
      res_scale_(1.0),
      cap_scale_(1.0),
      induct_scale_(1.0),
      divider_('\0'),
      delimiter_('\0'),
      pre_bus_del_('\0'),
      suf_bus_del_('\0') {
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

    name_map_ = rhs.name_map_;
    revert_name_map_ = rhs.revert_name_map_;
    revert_ports_map_ = rhs.revert_ports_map_;
    netparasitics_vec_id_ = rhs.netparasitics_vec_id_;
    nameid_vec_id_ = rhs.nameid_vec_id_;
    symidx_vec_id_ = rhs.symidx_vec_id_;
    design_flow_vec_id_ = rhs.design_flow_vec_id_;
    ports_vec_id_ = rhs.ports_vec_id_;
    cell_id_ = rhs.cell_id_;
    time_scale_ = rhs.time_scale_;
    res_scale_ = rhs.res_scale_;
    cap_scale_ = rhs.cap_scale_;
    induct_scale_ = rhs.induct_scale_;
    divider_ = rhs.divider_;
    delimiter_ = rhs.delimiter_;
    pre_bus_del_ = rhs.pre_bus_del_;
    suf_bus_del_ = rhs.suf_bus_del_;
}

void NetsParasitics::move(NetsParasitics&& rhs) {
    this->BaseType::move(std::move(rhs));
    name_map_ = std::move(rhs.name_map_);
    revert_name_map_ = std::move(rhs.revert_name_map_);
    revert_ports_map_ = std::move(rhs.revert_ports_map_);
    netparasitics_vec_id_ = std::move(rhs.netparasitics_vec_id_);
    nameid_vec_id_ = std::move(rhs.nameid_vec_id_);
    symidx_vec_id_ = std::move(rhs.symidx_vec_id_);
    design_flow_vec_id_ = std::move(rhs.design_flow_vec_id_);
    ports_vec_id_ = std::move(rhs.ports_vec_id_);
    cell_id_ = std::move(rhs.cell_id_);
    time_scale_ = std::move(rhs.time_scale_);
    res_scale_ = std::move(rhs.res_scale_);
    cap_scale_ = std::move(rhs.cap_scale_);
    induct_scale_ = std::move(rhs.induct_scale_);
    divider_ = std::move(rhs.divider_);
    delimiter_ = std::move(rhs.delimiter_);
    pre_bus_del_ = std::move(rhs.pre_bus_del_);
    suf_bus_del_ = std::move(rhs.suf_bus_del_);
    rhs.name_map_.clear();
    rhs.revert_name_map_.clear();
    rhs.revert_ports_map_.clear();
    
}

void NetsParasitics::addDesignFlow(std::string flowStr) {
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        ArrayObject<SymbolIndex> *vctSym = nullptr;
        if (design_flow_vec_id_ == UNINIT_OBJECT_ID) {
            vctSym = timingdb->createObject< ArrayObject<SymbolIndex> >(kObjectTypeArray, timingdb->getId());
            if (vctSym == nullptr)
                return;
            vctSym->setPool(timingdb->getPool());
	    vctSym->reserve(5);
            design_flow_vec_id_ = vctSym->getId();
        } else {
            vctSym = addr< ArrayObject<SymbolIndex> >(design_flow_vec_id_);
        }
        if (vctSym != nullptr) {
	    SymbolIndex symIdx = timingdb->getOrCreateSymbol(flowStr.c_str());
            vctSym->pushBack(symIdx);
	}
    }

}

void NetsParasitics::addNameMapIdx(uint32_t index) {
    Timing *timingdb = getTimingLib();
    if (timingdb) {
	ArrayObject<uint32_t> *vctIdx = nullptr;
	if (nameid_vec_id_ == UNINIT_OBJECT_ID) {
	    vctIdx = timingdb->createObject< ArrayObject<uint32_t> >(kObjectTypeArray, timingdb->getId());
            if (vctIdx == nullptr)
                return;
            vctIdx->setPool(timingdb->getPool());
	    vctIdx->reserve(1000);
            nameid_vec_id_ = vctIdx->getId();
	} else {
	    vctIdx = addr< ArrayObject<uint32_t> >(nameid_vec_id_);
	}
        if (vctIdx != nullptr)
            vctIdx->pushBack(index);
    }
}

void NetsParasitics::addNameMapSymIdx(SymbolIndex symIdx) {
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        ArrayObject<SymbolIndex> *vctSym = nullptr; 
        if (symidx_vec_id_ == UNINIT_OBJECT_ID) {
            vctSym = timingdb->createObject< ArrayObject<SymbolIndex> >(kObjectTypeArray, timingdb->getId());
            if (vctSym == nullptr)
                return;
            vctSym->setPool(timingdb->getPool());
	    vctSym->reserve(1000);
            symidx_vec_id_ = vctSym->getId();
        } else {
            vctSym = addr< ArrayObject<SymbolIndex> >(symidx_vec_id_);
        }
        if (vctSym != nullptr)
            vctSym->pushBack(symIdx);
    }
}


void NetsParasitics::addNameMap(uint32_t index, SymbolIndex symIdx) { 
    name_map_[index] = symIdx; 
 
    addNameMapIdx(index);   
    addNameMapSymIdx(symIdx);
}
    
void NetsParasitics::addPort(ObjectId portId) { 
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        ArrayObject<ObjectId> *vctPort = nullptr;
        if (ports_vec_id_ == UNINIT_OBJECT_ID) {
            vctPort = timingdb->createObject< ArrayObject<ObjectId> >(kObjectTypeArray, timingdb->getId());
            if (vctPort == nullptr)
                return;
            vctPort->setPool(timingdb->getPool());
	    vctPort->reserve(100);
            ports_vec_id_ = vctPort->getId();
        } else {
            vctPort = addr< ArrayObject<ObjectId> >(ports_vec_id_);
        }
        if (vctPort != nullptr)
            vctPort->pushBack(portId);
    }

}

void NetsParasitics::addNetParasitics(ObjectId netParaId) {
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        ArrayObject<ObjectId> *vctNetPara = nullptr;
        if (netparasitics_vec_id_ == UNINIT_OBJECT_ID) {
            vctNetPara = timingdb->createObject< ArrayObject<ObjectId> >(kObjectTypeArray, timingdb->getId());
            if (vctNetPara == nullptr)
                return;
            vctNetPara->setPool(timingdb->getPool());
            vctNetPara->reserve(1000);
            netparasitics_vec_id_ = vctNetPara->getId();
        } else {
            vctNetPara = addr< ArrayObject<ObjectId> >(netparasitics_vec_id_);
        }
        if (vctNetPara != nullptr)
            vctNetPara->pushBack(netParaId);
    }

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
    Cell *cell = Object::addr<Cell>(cell_id_);
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
    Cell *cell = Object::addr<Cell>(cell_id_);
    Net *net = nullptr;
    std::string netStr = netName;

    if (cell && netName) {
	if (netName[0] == '*') {
	    uint32_t idx = strtoul(netName+1, NULL, 0);
	    if (name_map_.find(idx) != name_map_.end()) {
		net = getNetBySymbol(name_map_[idx]);
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
    Cell *cell = Object::addr<Cell>(cell_id_);
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
    Cell *cell = Object::addr<Cell>(cell_id_);
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
    Cell *cell = Object::addr<Cell>(cell_id_);
    if (cell && pinName) {
        std::string pinStr = pinName;
	std::size_t found = pinStr.find_last_of(delimiter_);
	if (found != std::string::npos) {
            if (pinName[0] == '*') {
                uint32_t idx = strtoul(pinStr.substr(1, found).c_str(), NULL, 0);
                if (name_map_.find(idx) != name_map_.end()) 
		    return getPinBySymbol(name_map_[idx], pinStr.substr(found+1));
            } else {  //Name map doesn't exist
		Inst *inst = cell->getInstance(pinStr.substr(0, found));
		if (inst)
		    return inst->getPin(pinStr.substr(found+1));
	    }
        } else {  //Handle IO pin
            if (pinName[0] == '*') {
		uint32_t idx = strtoul(pinName+1, NULL, 0);
		if (name_map_.find(idx) != name_map_.end()) {
		    return getPortBySymbol(name_map_[idx]);
                } 
	    } else 
                return cell->getIOPin(pinStr); 
        }
    }
    return nullptr;
}

ObjectId  NetsParasitics::createParaNode(DNetParasitics *netParasitics, const char *nodeName) {
    Cell *cell = Object::addr<Cell>(cell_id_);
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
                    if (pin->getDirection() == SignalDirection::kOutput || pin->getDirection() == SignalDirection::kInout) {
                        auto pin_addr = Object::addr<ParasiticPinNode>(pinNodeId);
                        netParasitics->addRoot(pin_addr);
                    }
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
    Timing *timingdb = getTimingLib();
    if (timingdb) {
	auto netPara = timingdb->createObject<DNetParasitics>(kObjectTypeDNetParasitics, timingdb->getId());
        if (netPara) {
	    netPara->setOwner(this);
            netPara->setNetId(netId);
	    netPara->setNetTotalCap(totCap);
            ObjectId netParaId = netPara->getId();
            //addNet(netId);
            addNetParasitics(netParaId);
            return netPara;
	}
    }
    return nullptr;
}

RNetParasitics* NetsParasitics::addRNetParasitics(ObjectId netId, float totCap) {
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        auto netPara = timingdb->createObject<RNetParasitics>(kObjectTypeRNetParasitics, timingdb->getId());
        if (netPara) {
            netPara->setOwner(this);
            netPara->setNetId(netId);
            netPara->setNetTotalCap(totCap);
            ObjectId netParaId = netPara->getId();
            //addNet(netId);
	    addNetParasitics(netParaId);
            return netPara;
        }
    }
    return nullptr;
}

///Functions for SPEF dumpping
void NetsParasitics::clearTempMapContent() {
    revert_name_map_.clear();
    revert_ports_map_.clear();
}


std::string NetsParasitics::getNetDumpName(Net *net) {
    std::string netName = net->getName();
    if (revert_name_map_.find(netName) != revert_name_map_.end())
        netName = "*" + std::to_string(revert_name_map_[netName]);   
    return netName;
}

std::string NetsParasitics::getCellDumpName(Cell *cell) { //for RNet
    std::string cellName = cell->getName();
    if (revert_name_map_.find(cellName) != revert_name_map_.end())
        cellName = "*" + std::to_string(revert_name_map_[cellName]);
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
    if (revert_ports_map_.find(pinName) != revert_ports_map_.end())
        pinName = "*" + std::to_string(revert_ports_map_[pinName]);
    else {
	Inst *inst = pin->getInst();
        if (inst) {
	    std::string instName = inst->getName();
            if (revert_name_map_.find(instName) != revert_name_map_.end()) {
                pinName = "*" + std::to_string(revert_name_map_[instName]);
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
    Timing *timingdb = getTimingLib();
    os << ("*SPEF \"IEEE 1481-2009\"\n");

    Cell *cell = Object::addr<Cell>(cell_id_);
    if (cell) {
        std::string cellName = cell->getName();  //Waiting for DB team to fix this issue
        os << ("*DESIGN \"") << (cellName) << ("\"\n");
    }

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
    if (design_flow_vec_id_ != UNINIT_OBJECT_ID) {
	ArrayObject<SymbolIndex> *vct =  Object::addr< ArrayObject<SymbolIndex> >(design_flow_vec_id_);
	if (vct) {
            for (auto obj : *vct) {
		if (timingdb) {
		    std::string flwStr = timingdb->getSymbolByIndex(obj);
                    os << (" ") << ("\"") << (flwStr) << ("\"");
		}
	    }
	}
    }
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
     if (nameid_vec_id_ != UNINIT_OBJECT_ID && 
	 symidx_vec_id_ != UNINIT_OBJECT_ID) {
        Cell *cell = Object::addr<Cell>(cell_id_);
        os << ("*NAME_MAP\n\n");
        ArrayObject<uint32_t> *vctUint =  Object::addr< ArrayObject<uint32_t> >(nameid_vec_id_);
	ArrayObject<SymbolIndex> *vctSym =  Object::addr< ArrayObject<SymbolIndex> >(symidx_vec_id_);
	if (vctUint && vctSym) {
            for (size_t i=0; i<vctUint->getSize(); i++) {
                std::string name = cell->getSymbolByIndex((*vctSym)[i]);
                os << ("*") << (std::to_string((*vctUint)[i]));
                os << (" ") << (name) << ("\n");
                revert_name_map_[name] = (*vctUint)[i];
            }
	}
        os << ("\n\n");
    }
}

void NetsParasitics::dumpPorts(std::ofstream& os) {
    if (ports_vec_id_ != UNINIT_OBJECT_ID) {
        os << ("*PORTS\n\n");
	ArrayObject<ObjectId> *vctObj =  Object::addr< ArrayObject<ObjectId> >(ports_vec_id_);
	if (vctObj) {
            for (auto obj : *vctObj) {
                Pin *pin = Object::addr<Pin>(obj);
                std::string pinName = pin->getName();
                if (revert_name_map_.find(pinName) != revert_name_map_.end()) {  //To use index instead of port name directly
		    revert_ports_map_[pinName] = revert_name_map_[pinName];
                    pinName = std::to_string(revert_name_map_[pinName]);
                }

                os << ("*") << (pinName) << (" ");
                os << (getTermDirDumpName(pin)) << ("\n");
            }
	}
        os << ("\n");
    }
}

void NetsParasitics::dumpDNetConn(std::ofstream& os, DNetParasitics *dNetPara) {
    os << ("*CONN\n");
    ObjectId netId = dNetPara->getNetId(); 
    if (netId != UNINIT_OBJECT_ID) {
        Net *net = Object::addr<Net>(netId);
        if (net) {
	    ArrayObject<ObjectId> *objVector = net->getPinArray();
            for (auto obj : *objVector) {
                Pin *pin = Object::addr<Pin>(obj);
                if (pin != nullptr) {
                    std::string pinName = pin->getName();
                    if (revert_ports_map_.find(pinName) != revert_ports_map_.end())
                        os << ("*P ");
                    else
                        os << ("*I ");

                    os << (getPinDumpName(pin)) << (" ");
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
    if (netparasitics_vec_id_ != UNINIT_OBJECT_ID) {
        ArrayObject<ObjectId> *objVector = addr< ArrayObject<ObjectId> >(netparasitics_vec_id_);
        for (auto obj : *objVector) {
            NetParasitics *unObj = Object::addr<NetParasitics>(obj);  //Need to check further
            if (unObj->getObjectType() == ObjectType::kObjectTypeDNetParasitics) {
                DNetParasitics *dNetPara = Object::addr<DNetParasitics>(obj);
                dumpDNet(os, dNetPara);
            } else if (unObj->getObjectType() == ObjectType::kObjectTypeRNetParasitics) {
                RNetParasitics *rNetPara = Object::addr<RNetParasitics>(obj);
                dumpRNet(os, rNetPara);
            }
        }
    }
}

std::ofstream& operator<<(std::ofstream& os, NetsParasitics &rhs) {
 
    rhs.dumpSpefHeader(os);
   
    rhs.dumpNameMap(os);

    rhs.dumpPorts(os);

    rhs.dumpNets(os);

    rhs.clearTempMapContent();

    return os;
}

void DNetParasitics::buildParasiticForest() {
    assert(adjacent_map_);
    auto size = adjacent_map_->size();
    for (auto root : *roots_) {
        std::vector<OptParaNode> tree;
        tree.reserve(size);
        size_t index = 0;
        tree.push_back({root->getId(), UNINIT_OBJECT_ID, 0, 0});
        for (int i = 0; i < size; ++i) {
            // To reduce runtime memory, using pre-reversed vector to do queueing.
            OptParaNode cur_node = tree[i];
            auto node_addr = Object::addr<ParasiticNode>(cur_node.node);
            auto resistances = (*adjacent_map_)[node_addr];
            for (auto res : resistances) {
                auto next_addr = node_addr->getId() == res->getNode1Id() ?
                                   Object::addr<ParasiticNode>(res->getNode2Id()) :
                                   Object::addr<ParasiticNode>(res->getNode1Id());
                tree.push_back({next_addr->getId(), /* node */
                                           cur_node.node,                            /* parent_node */
                                           res->getResistance(),       /* resistance */
                                           (*node_gcap_map_)[next_addr]       /* capacitance */
                                           });
            }
        }
        parasitic_forest_.push_back(std::move(tree));
    }
}

void DNetParasitics::addAdjacentEdge(ParasiticNode *from, ParasiticResistor *resistance) {
    auto iter = adjacent_map_->find(from);
    if (iter == adjacent_map_->end()) {
        (*adjacent_map_)[from] = {resistance};
    } else {
        iter->second.push_back(resistance);
    }
}

void DNetParasitics::prepareGraphData() {
    assert(!node_gcap_map_);
    assert(!adjacent_map_);
    node_gcap_map_ = new std::unordered_map<ParasiticNode*, float>();
    adjacent_map_ = new std::unordered_map<ParasiticNode*, std::list<ParasiticResistor*>>();

    auto gcap_vec = Object::addr<ArrayObject<ObjectId>>(gcap_vec_id_);
    for (auto gcap_id : (*gcap_vec)) {
        auto gcap = Object::addr<ParasiticCap>(gcap_id);
        auto node = Object::addr<ParasiticNode>(gcap->getNode1Id());
        (*node_gcap_map_)[node] = gcap->getCapacitance();
    }
    auto res_vec = Object::addr<ArrayObject<ObjectId>>(res_vec_id_);
    for (auto res_id : (*res_vec)) {
        auto res = Object::addr<ParasiticResistor>(res_id);
        auto node1_addr = Object::addr<ParasiticNode>(res->getNode1Id());
        auto node2_addr = Object::addr<ParasiticNode>(res->getNode2Id());

        // non-directed
        addAdjacentEdge(node1_addr, res);
        addAdjacentEdge(node2_addr, res);
    }
}

void DNetParasitics::clearGraphData() { 
    delete adjacent_map_; 
    delete node_gcap_map_;
    adjacent_map_ = nullptr; 
    node_gcap_map_ = nullptr;
}

std::vector<std::vector<OptParaNode>> DNetParasitics::getParasiticForest() {
    if (!hasParasiticForest()) {
        prepareGraphData();
        buildParasiticForest();
        clearGraphData();
    }
    return parasitic_forest_;
}

void DNetParasitics::checkLoop() {
    auto gcaps = Object::addr<ArrayObject<ObjectId>>(gcap_vec_id_);
    auto res = Object::addr<ArrayObject<ObjectId>>(res_vec_id_);
    if (gcaps->getSize() != res->getSize() - 1) {
        printf("C size: %lld R size %lld\n", (long long)gcaps->getSize(), (long long)res->getSize());
    }
}

}  // namespace db
}  // namespace open_edi
