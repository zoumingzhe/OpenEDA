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
      lineNo_(1),
      netNodeMap_(),
      scanner_(nullptr) {
    spefField_ = designParasitics->getSpefField();
    //addDesignNetsParasitics();
}

SpefReader::~SpefReader() {
}

void SpefReader::stringDelete(const char *str) {
    if (str) {
        delete [] str;
    }
}

char* SpefReader::stringCopy(const char *str) { 
    if (str) {
        char *copy = new char[strlen(str) + 1];
        strcpy(copy, str);
        return copy;
    } else 
        return nullptr;
}

bool SpefReader::isDigits(const char *str) {
  for (const char *s = str; *s; s++) {
    if (!isdigit(*s))
      return false;
  }
  return true;
}

void SpefReader::setCell(const char *designName) {
    Cell* topCell = getTopCell();
    if (topCell != nullptr) {
	cell_ = topCell->getCell(std::string(designName));
        if (cell_ == nullptr) {
            cell_ == topCell;  //Use top cell for the time being
	    open_edi::util::message->issueMsg(open_edi::util::kError,
                                              "Failed to find SPEF cell in design.\n");
            return;
        }
        addDesignNetsParasitics();
    }
}

void SpefReader::addDesignNetsParasitics() {
    if (cell_ != nullptr) {
        netsParasitics_ = cell_->createObject<NetsParasitics>(kObjectTypeNetsParasitics);
        if (netsParasitics_ == nullptr) {
            open_edi::util::message->issueMsg(open_edi::util::kError,
                                              "Creating NetsParasitics failed.\n");
            return;
        }
        netsParasitics_->setCellId(cell_->getId());
        SymbolIndex spefFileIdx = cell_->getOrCreateSymbol(spefFileName_.c_str());
	if (designParasitics_ && cell_) {
            designParasitics_->addSpefMap(cell_->getId(), spefFileIdx);
	    designParasitics_->addParasiticsMap(cell_->getId(), netsParasitics_->getId());
	}	 
    }
}

void SpefReader::setTimeScale(float digits, const char* unit) {
    float scale = 1.0;
    if (strcmp(unit, "NS") == 0)
        scale = digits*1.0e-9;
    else if (strcmp(unit, "PS") == 0)
        scale = digits*1.0e-12;
    else {
        open_edi::util::message->issueMsg(open_edi::util::kError,
                                              "Incorrect time unit defined in SPEF.\n");
    }
    netsParasitics_->setTimeScale(scale);
    stringDelete(unit);
}

void SpefReader::setCapScale(float digits, const char* unit) {
    float scale = 1.0;
    if (strcmp(unit, "PF") == 0)
        scale = digits*1.0e-12;
    else if (strcmp(unit, "FF") == 0)
        scale = digits*1.0e-15;
    else {
        open_edi::util::message->issueMsg(open_edi::util::kError,
                                              "Incorrect capacitance unit defined in SPEF.\n");
    }
    netsParasitics_->setCapScale(scale);
    stringDelete(unit);
}

void SpefReader::setResScale(float digits, const char* unit) {
    float scale = 1.0;
    if (strcmp(unit, "OHM") == 0)
        scale = digits*1.0;
    else if (strcmp(unit, "KOHM") == 0)
        scale = digits*1.0e3;
    else {
        open_edi::util::message->issueMsg(open_edi::util::kError,
                                              "Incorrect resistance unit defined in SPEF.\n");
    }
    netsParasitics_->setResScale(scale);
    stringDelete(unit);
}
    
void SpefReader::setInductScale(float digits, const char* unit) {
    float scale = 1.0;
    if (strcmp(unit, "HENRY") == 0)
        scale = digits;
    else if (strcmp(unit, "MH") == 0)
        scale = digits*1.0e-3;
    else if (strcmp(unit, "UH") == 0)
	scale = digits*1.0e-6;
    else {
        open_edi::util::message->issueMsg(open_edi::util::kError,
                                              "Incorrect inductance unit defined in SPEF.\n");
    }
    netsParasitics_->setInductScale(scale);
    stringDelete(unit);
}

void SpefReader::addNameMap(char *index, char *name) {
    if (cell_ && netsParasitics_) {
        SymbolIndex nameIdx = cell_->getOrCreateSymbol(name);
        if (index[0] == '*') {
	    uint32_t idx = strtoul(index+1, NULL, 0);
	    netsParasitics_->addNameMap(idx, nameIdx);
        }
    }
    stringDelete(index); 
}

void SpefReader::addPort(char *name) {
    Pin *pin = findPin(name);
    if (netsParasitics_ && pin) 
        netsParasitics_->addPort(pin->getId());
}

/*void SpefReader::addPowerNet(char* name) {
    Cell* topCell = getTopCell();
    if (topCell) {
	SpecialNet* snet = topCell->getSpecialNet(name);
	if (snet) 
            netsParasitics_->addPowerNet(snet->getId());
    }
    stringDelete(name);
}*/

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
    if (netsParasitics_ && net) { 
        net_ = net;
        dnetParasitics_ = netsParasitics_->addDNetParasitics(net->getId(), parValue_); 
    }
}

void SpefReader::addDNetEnd() { 
    net_ = nullptr; 
    dnetParasitics_ = nullptr;
    netNodeMap_.clear(); 
}

void SpefReader::addPinNode(char *pinName) {
    if (pinName && dnetParasitics_) {
        std::string pinStr = pinName;
        stringDelete(pinName);
        ObjectId pinNodeId = getParasiticNode(pinStr);
        dnetParasitics_->addPinNode(pinNodeId, cell_->getId());
    }
}

ObjectId SpefReader::getParasiticNode(std::string nodeName) {
    ObjectId paraNodeId = UNINIT_OBJECT_ID;
    if (netsParasitics_ && dnetParasitics_) {
	if (netNodeMap_.find(nodeName) == netNodeMap_.end()) {
            paraNodeId = netsParasitics_->addNode(dnetParasitics_, nodeName.c_str());
            if (paraNodeId != UNINIT_OBJECT_ID)
                netNodeMap_[nodeName] = paraNodeId;
        } else
            paraNodeId = netNodeMap_[nodeName];
    }
    return paraNodeId;
     
}

void SpefReader::addGroundCap(char *nodeName) {
    //For gound cap only need to handle internal node as pin node handled in CONN section
    if (nodeName && dnetParasitics_) {
        std::string nodeStr = nodeName;
        stringDelete(nodeName);
	ObjectId paraNodeId = getParasiticNode(nodeStr);
        if (paraNodeId != UNINIT_OBJECT_ID)
            dnetParasitics_->addGroundCap(paraNodeId, cell_->getId(), parValue_);
    }
}

void SpefReader::addCouplingCap(char *nodeName1, char *nodeName2) {
    if (nodeName1 && nodeName2 && dnetParasitics_) {
        std::string node1Str = nodeName1;
        std::string node2Str = nodeName2;
        stringDelete(nodeName1);
	stringDelete(nodeName2);
	ObjectId paraNode1Id = getParasiticNode(nodeName1);
        ObjectId paraNode2Id = getParasiticNode(nodeName2);
	if (paraNode1Id != UNINIT_OBJECT_ID && paraNode2Id != UNINIT_OBJECT_ID)
	    dnetParasitics_->addCouplingCap(paraNode1Id, paraNode2Id, cell_->getId(), parValue_);
    }
}

void SpefReader::addResistor(char *nodeName1, char *nodeName2) {
    if (nodeName1 && nodeName2 && dnetParasitics_) {
        std::string node1Str = nodeName1;
	std::string node2Str = nodeName2;
	stringDelete(nodeName1);
	stringDelete(nodeName2);
	ObjectId paraNode1Id = getParasiticNode(nodeName1);
        ObjectId paraNode2Id = getParasiticNode(nodeName2);
        if (paraNode1Id != UNINIT_OBJECT_ID && paraNode2Id != UNINIT_OBJECT_ID)
            dnetParasitics_->addResistor(paraNode1Id, paraNode2Id, cell_->getId(), parValue_);
    }
}

void SpefReader::addRNetBegin(Net *net) {
    if (netsParasitics_) {
	net_ = net;
        rnetParasitics_ = netsParasitics_->addRNetParasitics(net->getId(), parValue_);
    } 
}

void SpefReader::addRNetDrvr(char *pinName) {
    if (pinName && rnetParasitics_) { 
	Pin *pin = findPin(pinName);
        stringDelete(pinName);
        if (pin)
	    rnetParasitics_->setDriverPinId(pin->getId());
    }
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
        errMsg = "Failed to parse SPEF file: " + spefFileName_;
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
