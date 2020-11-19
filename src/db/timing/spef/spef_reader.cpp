/** 
 * @file spef_reader.cpp
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

#include "db/core/db.h"
#include "util/stream.h"

#include "db/timing/spef/spef_reader.h"

namespace SpefReader {

SpefReader::SpefReader(std::string fileName, DesignParasitics *designParasitics)
    : spefFileName_(fileName),
      recordStr_(""),
      cell_(nullptr),
      designParasitics_(designParasitics),
      netsParasitics_(nullptr),
      parValue_(0.0),
      net_(nullptr),
      dnetParasitics_(nullptr),
      rnetParasitics_(nullptr),
      lineNo_(0),
      netNodeMap_(),
      scanner_(nullptr) {
    spefField_ = designParasitics->getSpefField();
}

SpefReader::~SpefReader() {
}

char* SpefReader::stringCopy(const char *str)
{ 
    if (str) {
        char *copy = new char[strlen(str) + 1];
        strcpy(copy, str);
        return copy;
    } else 
        return nullptr;
}

bool SpefReader::isDigits(const char *str)
{
  for (const char *s = str; *s; s++) {
    if (!isdigit(*s))
      return false;
  }
  return true;
}

void SpefReader::setCell(const char *designName) {
    Cell* topCell = getTopCell();
    if (topCell) {
	cell_ = topCell->getCell(std::string(designName));
        if (cell_ == nullptr) {
	    open_edi::util::message->issueMsg(open_edi::util::kError,
                                              "Failed to find SPEF cell in design.\n");
            //return TCL_ERROR;
        }    
    }
    //return TCL_OK;
}

void SpefReader::addDesignNetsParasitics() {
    Cell* topCell = getTopCell();
    if (topCell) {
        netsParasitics_ = topCell->createObject<NetsParasitics>(kObjectTypeNetsParasitics);
        if (netsParasitics_ == nullptr) {
            open_edi::util::message->issueMsg(open_edi::util::kError,
                                              "Creating NetsParasitics failed.\n");
            return;
        }
        SymbolIndex spefFileIdx = topCell->getOrCreateSymbol(spefFileName_.c_str());
	if (designParasitics_ && cell_) {
            designParasitics_->addSpefMap(cell_->getId(), spefFileIdx);
	    designParasitics_->addParasiticsMap(cell_->getId(), netsParasitics_->getId());
	}	 
    }
}

void SpefReader::setTimeUnit(float digits, const char* unit) {
    float scale = 1.0;
    if (strcmp(unit, "NS") == 0)
        scale = digits*1.0e-9;
    else if (strcmp(unit, "PS") == 0)
        scale = digits*1.0e-12;
    else {
        open_edi::util::message->issueMsg(open_edi::util::kError,
                                              "Incorrect time unit defined in SPEF.\n");
    }
    netsParasitics_->setTimeUnit(scale);
    stringDelete(unit);
}

void SpefReader::setCapUnit(float digits, const char* unit) {
    float scale = 1.0;
    if (strcmp(unit, "PF") == 0)
        scale = digits*1.0e-12;
    else if (strcmp(unit, "FF") == 0)
        scale = digits*1.0e-15;
    else {
        open_edi::util::message->issueMsg(open_edi::util::kError,
                                              "Incorrect capacitance unit defined in SPEF.\n");
    }
    netsParasitics_->setCapUnit(scale);
    stringDelete(unit);
}

void SpefReader::setResUnit(float digits, const char* unit) {
    float scale = 1.0;
    if (strcmp(unit, "OHM") == 0)
        scale = digits*1.0;
    else if (strcmp(unit, "KOHM") == 0)
        scale = digits*1.0e3;
    else {
        open_edi::util::message->issueMsg(open_edi::util::kError,
                                              "Incorrect resistance unit defined in SPEF.\n");
    }
    netsParasitics_->setResUnit(scale);
    stringDelete(unit);
}
    
void SpefReader::setInductUnit(float digits, const char* unit) {
    float scale = 1.0;
    if (strcmp(unit, "HENRY") == 0)
        scale = digits*1.0e-12;
    else if (strcmp(unit, "MH") == 0)
        scale = digits*1.0e-3;
    else if (strcmp(unit, "UH") == 0)
	scale = digits*1.0e-6;
    else {
        open_edi::util::message->issueMsg(open_edi::util::kError,
                                              "Incorrect inductance unit defined in SPEF.\n");
    }
    netsParasitics_->setInductUnit(scale);
    stringDelete(unit);
}

void SpefReader::addNameMap(char* index, char* name) {
    Cell* topCell = getTopCell();
    if (topCell) {
        SymbolIndex nameIdx = topCell->getOrCreateSymbol(name); 
	uint32_t idx = strtoul(index, NULL, 0);
	netsParasitics_->addNameMap(idx, nameIdx);
    }
    stringDelete(index); 
}

void SpefReader::addPGNet(char* name) {
    Cell* topCell = getTopCell();
    if (topCell) {
	SpecialNet* snet = topCell->getSpecialNet(name);
	if (snet) 
            netsParasitics_->addPGNet(snet->getId());
    }
    stringDelete(name);
}

float SpefReader::addParValue(float value1, float value2, float value3) {
    if (spefField_ == 1) 
        parValue_ = value1;
    else if (spefField_ == 2)
        parValue_ = value2;
    else if (spefField_ == 3)
        parValue_ = value3;
    else
        parValue_ = value1;
    return parValue_;
}

Net* SpefReader::findNet(char *name) {
    Net *net = nullptr;
    if (netsParasitics_)
        net = netsParasitics_->findNet(name);
    stringDelete(name);
    return net;
}

Pin* SpefReader::findPin(char *name) {
    Pin *pin = nullptr;
    if (netsParasitics_) 
        pin = netsParasitics_->findPin(name);
    stringDelete(name);
    return pin;
}

void SpefReader::addDNetBegin(Net *net) { 
    if (netsParasitics_) 
        dnetParasitics_ = netsParasitics_->addDNetParasitics(net_->getId(), parValue_); 
}

void SpefReader::addDNetEnd() { 
    net_ = nullptr; 
    dnetParasitics_ = nullptr;
    for(auto it=netNodeMap_.begin(); it!=netNodeMap_.end(); ++it)
        stringDelete(it->first);
    netNodeMap_.clear(); 
}

void SpefReader::addPinNode(Pin *pin) {
    if (pin) {
        const char *pinName = pin->getName().c_str();
        ObjectId pinNodeId = getParasiticNode(pinName);
        dnetParasitics_->addPinNode(pinNodeId);
    }
}

ObjectId SpefReader::getParasiticNode(const char *nodeName) {
    ObjectId paraNodeId = UNINIT_OBJECT_ID;
    if (netsParasitics_ && dnetParasitics_) {
	if (netNodeMap_.find(nodeName) == netNodeMap_.end()) {
            paraNodeId = netsParasitics_->addNode(dnetParasitics_, nodeName);
            if (paraNodeId != UNINIT_OBJECT_ID)
                netNodeMap_[nodeName] = paraNodeId;
        } else
            paraNodeId = netNodeMap_[nodeName];
    }
    return paraNodeId;
     
}

void SpefReader::addGroundCap(char *nodeName) {
    //For gound cap only need to handle internal node as pin node handled in CONN section
    if (dnetParasitics_) {
	ObjectId paraNodeId = getParasiticNode(nodeName);
        if (paraNodeId != UNINIT_OBJECT_ID)
            dnetParasitics_->addGroundCap(paraNodeId, parValue_);
    }
}

void SpefReader::addCouplingCap(char *nodeName1, char *nodeName2) {
    if (dnetParasitics_) {
	ObjectId paraNode1Id = getParasiticNode(nodeName1);
        ObjectId paraNode2Id = getParasiticNode(nodeName2);
	if (paraNode1Id != UNINIT_OBJECT_ID && paraNode2Id != UNINIT_OBJECT_ID)
	    dnetParasitics_->addCouplingCap(paraNode1Id, paraNode2Id, parValue_);
    }
}

void SpefReader::addResistor(char *nodeName1, char *nodeName2) {
    if (dnetParasitics_) {
	ObjectId paraNode1Id = getParasiticNode(nodeName1);
        ObjectId paraNode2Id = getParasiticNode(nodeName2);
        if (paraNode1Id != UNINIT_OBJECT_ID && paraNode2Id != UNINIT_OBJECT_ID)
            dnetParasitics_->addResistor(paraNode1Id, paraNode2Id, parValue_);
    }
}

void SpefReader::addRNetBegin(Net *net) {
    if (netsParasitics_)
        rnetParasitics_ = netsParasitics_->addRNetParasitics(net_->getId(), parValue_);
}

void SpefReader::addPiModel(float c2, float r1, float c1) {
    if (rnetParasitics_) {
        rnetParasitics_->setC2(c2);
        rnetParasitics_->setR1(r1);
	rnetParasitics_->setC1(c1);
    }
}

bool SpefReader::parseSpefFile() {

    std::string errMsg = "Failed to open SPEF file: " + spefFileName_;

    FILE *fspef = fopen(spefFileName_.c_str(), "r");
    if (fspef == NULL) {
        open_edi::util::message->issueMsg(
                        open_edi::util::kError, errMsg.c_str());
        return false;
    }

    __spef_parse_begin(fspef);
    if (__spef_parse() != 0) {
	open_edi::util::message->issueMsg(
                        open_edi::util::kError, errMsg.c_str());

        __spef_parse_end(fspef);
        fclose(fspef);
        return false;
    }
    __spef_parse_end(fspef); 
    fclose(fspef);
    
    return true;
}

}  // namespace SpefReader
