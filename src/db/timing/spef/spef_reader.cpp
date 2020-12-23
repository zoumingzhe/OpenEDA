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
#include "db/core/timing.h"
#include "util/stream.h"

#include <iostream>
#include <sys/time.h>
#include <time.h>

#include "db/timing/spef/spef_reader.h"

uint32_t total_net_count_ = 0;
uint32_t net_with_bid_ = 0;
uint32_t net_with_loop_ = 0;
uint32_t valid_net_ = 0;
uint32_t node_without_cap_ = 0;
uint32_t total_node_ = 0;

namespace SpefReader {

SpefReader::SpefReader(std::string fileName, DesignParasitics *designParasitics)
    : net_node_map_(),
      design_parasitics_(designParasitics),
      nets_parasitics_(nullptr),
      dnet_parasitics_(nullptr),
      rnet_parasitics_(nullptr),
      cell_(nullptr),
      net_(nullptr),
      scanner_(nullptr),
      spef_file_name_(fileName),
      record_str_(""),
      par_value_(0.0),
      line_no_(1) {
    spef_field_ = designParasitics->getSpefField();
    //addDesignNetsParasitics();
}

SpefReader::~SpefReader() {
    std::cout << "SPEF totals net count: " << total_net_count_ << std::endl;
    std::cout << "SPEF valid net: " << valid_net_ << std::endl;
    std::cout << "SPEF net with bid: " << net_with_bid_ << std::endl;
    std::cout << "SPEF net with loop: " << net_with_loop_ << std::endl;
}

void SpefReader::stringDelete(const char *str) {
    if (str) {
        delete [] str;
    }
}

const char* SpefReader::stringCopy(const char *str) { 
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
        }
        addDesignNetsParasitics();
    }
    stringDelete(designName);
}

void SpefReader::addDesignNetsParasitics() {
    Timing *timingdb = getTimingLib();
    if (timingdb != nullptr) {
        nets_parasitics_ = timingdb->createObject<NetsParasitics>(kObjectTypeNetsParasitics, timingdb->getId());
        if (nets_parasitics_ == nullptr) {
            open_edi::util::message->issueMsg(open_edi::util::kError,
                                              "Creating NetsParasitics failed.\n");
            return;
        }
        if (cell_) {
            nets_parasitics_->setCellId(cell_->getId());
            SymbolIndex spefFileIdx = timingdb->getOrCreateSymbol(spef_file_name_.c_str());
	    if (design_parasitics_) {
                design_parasitics_->addCellId(cell_->getId());
                design_parasitics_->addSpef(spefFileIdx);
	        design_parasitics_->addNetsParasitics(nets_parasitics_->getId());
	    }	 
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
    nets_parasitics_->setTimeScale(scale);
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
    nets_parasitics_->setCapScale(scale);
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
    nets_parasitics_->setResScale(scale);
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
    nets_parasitics_->setInductScale(scale);
    stringDelete(unit);
}

void SpefReader::setDesignFlow(StringVec *dsgFlow) { 
    for (auto str : *dsgFlow) {
        nets_parasitics_->addDesignFlow(std::string(str));
        stringDelete(str); 
    }
}

void SpefReader::addNameMap(const char *index, const char *name) {
    if (cell_ && nets_parasitics_) {
        SymbolIndex nameIdx = cell_->getOrCreateSymbol(name);
        if (index[0] == '*') {
	    uint32_t idx = strtoul(index+1, NULL, 0);
	    nets_parasitics_->addNameMap(idx, nameIdx);
        }
    }
    stringDelete(index);
    stringDelete(name);
}

void SpefReader::addPort(const char *name) {
    Pin *pin = findPin(name);
    if (nets_parasitics_ && pin) 
        nets_parasitics_->addPort(pin->getId());
    stringDelete(name);
}

/*void SpefReader::addPowerNet(char* name) {
    Cell* topCell = getTopCell();
    if (topCell) {
	SpecialNet* snet = topCell->getSpecialNet(name);
	if (snet) 
            nets_parasitics_->addPowerNet(snet->getId());
    }
    stringDelete(name);
}*/

float SpefReader::addParValue(float value1, float value2, float value3) {
    if (spef_field_ == 1) 
        par_value_ = value1;
    else if (spef_field_ == 2)
        par_value_ = value2;
    else if (spef_field_ == 3)
        par_value_ = value3;
    else
        par_value_ = value1;
    return par_value_;
}

Net* SpefReader::findNet(const char *name) {
    Net *net = nullptr;
    if (nets_parasitics_)
        net = nets_parasitics_->findNet(name);
    stringDelete(name);
    return net;
}

Pin* SpefReader::findPin(const char *name) {
    Pin *pin = nullptr;
    if (nets_parasitics_) 
        pin = nets_parasitics_->findPin(name);
    return pin;
}

void SpefReader::addDNetBegin(Net *net) { 
    if (nets_parasitics_ && net) { 
        net_ = net;
        dnet_parasitics_ = nets_parasitics_->addDNetParasitics(net->getId(), par_value_); 
    }
}

void SpefReader::addDNetEnd() { 
    if (dnet_parasitics_) {
        // printf("Net %s\n", net_->getName().c_str());
        // dnet_parasitics_->checkLoop();
        ++total_net_count_;
    }
    net_ = nullptr; 
    dnet_parasitics_ = nullptr;
    net_node_map_.clear(); 
}

void SpefReader::addPinNode(const char *pinName) {
    if (pinName) {
        if (dnet_parasitics_) {
            std::string pinStr = pinName;
            ObjectId pinNodeId = getParasiticNode(pinStr);
            //dnet_parasitics_->addPinNode(pinNodeId);
        }
        stringDelete(pinName);
    }
}

ObjectId SpefReader::getParasiticNode(const std::string &nodeName) {
    ObjectId paraNodeId = UNINIT_OBJECT_ID;
    if (nets_parasitics_ && dnet_parasitics_) {
        auto iter = net_node_map_.find(nodeName);
	if (iter == net_node_map_.end()) {
            paraNodeId = nets_parasitics_->createParaNode(dnet_parasitics_, nodeName.c_str());
            net_node_map_[nodeName] = paraNodeId;
        } else {
            paraNodeId = iter->second;
        }
    }
    return paraNodeId;
}

void SpefReader::addGroundCap(const char *nodeName) {
    //For gound cap only need to handle internal node as pin node handled in CONN section
    if (nodeName) {
        if (dnet_parasitics_) {
            std::string nodeStr = nodeName;
	    ObjectId paraNodeId = getParasiticNode(nodeStr);
            if (paraNodeId != UNINIT_OBJECT_ID)
                dnet_parasitics_->addGroundCap(paraNodeId, par_value_);
        }
        stringDelete(nodeName);
    }
}

void SpefReader::addCouplingCap(const char *nodeName1, const char *nodeName2) {
    if (nodeName1 && nodeName2) {
        if (dnet_parasitics_) {
            std::string node1Str = nodeName1;
            std::string node2Str = nodeName2;
	    ObjectId paraNode1Id = getParasiticNode(node1Str);
            ObjectId paraNode2Id = getParasiticNode(node2Str);
	    if (paraNode1Id != UNINIT_OBJECT_ID && paraNode2Id != UNINIT_OBJECT_ID)
	        dnet_parasitics_->addCouplingCap(paraNode1Id, paraNode2Id, par_value_);
	}
        stringDelete(nodeName1);
        stringDelete(nodeName2);
    }
}

void SpefReader::addResistor(const char *nodeName1, const char *nodeName2) {
    if (nodeName1 && nodeName2 && dnet_parasitics_) {
        if (dnet_parasitics_) {
            std::string node1Str = nodeName1;
	    std::string node2Str = nodeName2;
	    ObjectId paraNode1Id = getParasiticNode(node1Str);
            ObjectId paraNode2Id = getParasiticNode(node2Str);
            if (paraNode1Id != UNINIT_OBJECT_ID && paraNode2Id != UNINIT_OBJECT_ID)
                dnet_parasitics_->addResistor(paraNode1Id, paraNode2Id, par_value_);
	}
	stringDelete(nodeName1);
	stringDelete(nodeName2);
    }
}

void SpefReader::addRNetBegin(Net *net) {
    if (nets_parasitics_) {
	net_ = net;
        rnet_parasitics_ = nets_parasitics_->addRNetParasitics(net->getId(), par_value_);
    } 
}

void SpefReader::addRNetDrvr(const char *pinName) {
    if (pinName && rnet_parasitics_) {
        if (rnet_parasitics_) { 
	    Pin *pin = findPin(pinName);
            if (pin)
	        rnet_parasitics_->setDriverPinId(pin->getId());
	}
	stringDelete(pinName);
    }
}

void SpefReader::addPiModel(float c2, float r1, float c1) {
    if (rnet_parasitics_) {
        rnet_parasitics_->setC2(c2);
        rnet_parasitics_->setR1(r1);
	rnet_parasitics_->setC1(c1);
    }
}

bool SpefReader::parseSpefFile() {

    std::string errMsg = "Failed to open SPEF file: " + spef_file_name_;

    FILE *fspef = fopen(spef_file_name_.c_str(), "r");
    if (fspef == NULL) {
        open_edi::util::message->issueMsg(
                        open_edi::util::kError, errMsg.c_str());
        return false;
    }

    __spef_parse_begin(fspef);
    if (__spef_parse() != 0) {
        errMsg = "Failed to parse SPEF file: " + spef_file_name_;
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
