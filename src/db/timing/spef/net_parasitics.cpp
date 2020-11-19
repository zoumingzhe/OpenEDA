/** 
 * @file net_parasitics.cpp
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

#include "db/timing/spef/net_parasitics.h"

#include <map>

#include "db/core/db.h"
#include "util/stream.h"

namespace open_edi {
namespace db {

NetParasitics::NetParasitics()
    : NetParasitics::BaseType(),
      netId_(UNINIT_OBJECT_ID),
      totalCap_(0.0) {
    setObjectType(ObjectType::kObjectTypeNetParasitics);
}

NetParasitics::~NetParasitics() {
}

DNetParasitics::DNetParasitics()
    : NetParasitics() {
    setObjectType(ObjectType::kObjectTypeDNetParasitics);
}

ObjectId DNetParasitics::addPinNode(ObjectId pinId, bool isExtPin) {
    Cell *topCell = getTopCell();
    if (topCell) {
        auto pinNode = topCell->createObject<ParasiticPinNode>(kObjectTypeParasiticPinNode);
        if (pinNode) {
            pinNode->setOwner(this);
	    pinNode->setPinId(pinId);
	    if (!isExtPin) //Only add internal pin node
	        pinNodeVec_.push_back(pinNode->getId());
	    return pinNode->getId();
        }
    }
    return UNINIT_OBJECT_ID;
}

ObjectId DNetParasitics::addIntNode(uint32_t intNodeId) {
    Cell *topCell = getTopCell();
    if (topCell) {
        auto intNode = topCell->createObject<ParasiticIntNode>(kObjectTypeParasiticIntNode);
        if (intNode) {
            intNode->setOwner(this);
            intNode->setIntNodeId(intNodeId);
            return intNode->getId();
        }
    }
    return UNINIT_OBJECT_ID;
}

ObjectId DNetParasitics::addExtNode(ObjectId netId, uint32_t extNodeId) {
    Cell *topCell = getTopCell();
    if (topCell) {
        auto extNode = topCell->createObject<ParasiticExtNode>(kObjectTypeParasiticExtNode);
        if (extNode) {
            extNode->setOwner(this);
	    extNode->setExtNetId(netId);
            extNode->setExtNodeId(extNodeId);
            return extNode->getId();
        }
    }
    return UNINIT_OBJECT_ID;
}

void DNetParasitics::addGroundCap(ObjectId nodeId, float capValue) {
    Cell *topCell = getTopCell();
    if (topCell) {
        auto gCap = topCell->createObject<ParasiticCap>(kObjectTypeParasiticCap);
        if (gCap) {
            gCap->setOwner(this);
	    gCap->setNode1Id(nodeId);
	    gCap->setCapacitance(capValue);
	    gCapVec_.push_back(gCap->getId());
        }
    }
}

void DNetParasitics::addCouplingCap(ObjectId node1Id, ObjectId node2Id, float capValue) {
    Cell *topCell = getTopCell();
    if (topCell) {
        auto xCap = topCell->createObject<ParasiticXCap>(kObjectTypeParasiticXCap);
        if (xCap) {
            xCap->setOwner(this);
            xCap->setNode1Id(node1Id);
	    xCap->setNode2Id(node2Id);
            xCap->setCapacitance(capValue);
            xCapVec_.push_back(xCap->getId());
        }
    }
}

void DNetParasitics::addResistor(ObjectId node1Id, ObjectId node2Id, float resValue) {
    Cell *topCell = getTopCell();
    if (topCell) {
        auto res = topCell->createObject<ParasiticResistor>(kObjectTypeParasiticResistor);
        if (res) {
            res->setOwner(this);
            res->setNode1Id(node1Id);
	    res->setNode2Id(node2Id);
            res->setResistance(resValue);
            resVec_.push_back(res->getId());
        }
    }
}

RNetParasitics::RNetParasitics()
    : NetParasitics(),
      c2_(0.0),
      r1_(0.0),
      c1_(0.0) {
    setObjectType(ObjectType::kObjectTypeRNetParasitics);
}

}  // namespace db
}  // namespace open_edi
