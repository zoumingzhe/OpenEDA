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
#include <iostream>
#include <map>

#include "db/core/db.h"
#include "db/core/timing.h"
#include "util/stream.h"
#include "util/util_mem.h"

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
    : NetParasitics(),
      pinNodeVecId_(UNINIT_OBJECT_ID),
      gCapVecId_(UNINIT_OBJECT_ID),
      xCapVecId_(UNINIT_OBJECT_ID),
      resVecId_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeDNetParasitics);
}

void DNetParasitics::addPinNode(ObjectId pinNodeId) {
    ArrayObject<ObjectId> *vct = nullptr;
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        if (pinNodeVecId_ == UNINIT_OBJECT_ID) {
            vct = timingdb->createObject< ArrayObject<ObjectId> >(kObjectTypeArray, timingdb->getId());
            if (vct != nullptr) {
                vct->setPool(timingdb->getPool());
                vct->reserve(5);
                pinNodeVecId_ = vct->getId();
            }
        } else 
            vct = addr< ArrayObject<ObjectId> >(pinNodeVecId_);
                
        if (vct != nullptr)
            vct->pushBack(pinNodeId);    
    }
}

ObjectId DNetParasitics::createPinNode(ObjectId pinId) {
    //Cell *timingdb = getTopCell();
    //Cell *timingdb = Object::addr<Cell>(cellId);
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        auto pinNode = timingdb->createObject<ParasiticPinNode>(kObjectTypeParasiticPinNode, timingdb->getId());
        if (pinNode) {
            pinNode->setOwner(this);
	    pinNode->setPinId(pinId);
	    return pinNode->getId();
        }
    }
    return UNINIT_OBJECT_ID;
}

ObjectId DNetParasitics::createIntNode(uint32_t intNodeId) {
    //Cell *timingdb = getTopCell();
    //Cell *timingdb = Object::addr<Cell>(cellId);
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        auto intNode = timingdb->createObject<ParasiticIntNode>(kObjectTypeParasiticIntNode, timingdb->getId());
        if (intNode) {
            intNode->setOwner(this);
            intNode->setIntNodeId(intNodeId);
            return intNode->getId();
        }
    }
    return UNINIT_OBJECT_ID;
}

ObjectId DNetParasitics::createExtNode(ObjectId netId, uint32_t extNodeId) {
    //Cell *timingdb = getTopCell();
    //Cell *timingdb = Object::addr<Cell>(cellId);
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        auto extNode = timingdb->createObject<ParasiticExtNode>(kObjectTypeParasiticExtNode, timingdb->getId());
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
    //Cell *timingdb = getTopCell();
    //Cell *timingdb = Object::addr<Cell>(cellId);
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        auto gCap = timingdb->createObject<ParasiticCap>(kObjectTypeParasiticCap, timingdb->getId());
        if (gCap) {
            gCap->setOwner(this);
	    gCap->setNode1Id(nodeId);
	    gCap->setCapacitance(capValue);

	    ArrayObject<ObjectId> *vct = nullptr;
            if (gCapVecId_ == UNINIT_OBJECT_ID) {
                vct = timingdb->createObject< ArrayObject<ObjectId> >(kObjectTypeArray, timingdb->getId());
                if (vct == nullptr)
		    return;
                vct->setPool(timingdb->getPool());
                vct->reserve(50);
                gCapVecId_ = vct->getId();
            } else {
                vct = addr< ArrayObject<ObjectId> >(gCapVecId_);
            }
            if (vct != nullptr)
                vct->pushBack(gCap->getId());
        }
    }
}

void DNetParasitics::addCouplingCap(ObjectId node1Id, ObjectId node2Id, float capValue) {
    //Cell *timingdb = getTopCell();
    //Cell *timingdb = Object::addr<Cell>(cellId);
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        auto xCap = timingdb->createObject<ParasiticXCap>(kObjectTypeParasiticXCap, timingdb->getId());
        if (xCap) {
            xCap->setOwner(this);
            xCap->setNode1Id(node1Id);
	    xCap->setNode2Id(node2Id);
            xCap->setCapacitance(capValue);

	    ArrayObject<ObjectId> *vct = nullptr;
            if (xCapVecId_ == UNINIT_OBJECT_ID) {
                vct = timingdb->createObject< ArrayObject<ObjectId> >(kObjectTypeArray, timingdb->getId());
                if (vct == nullptr)
                    return;
                vct->setPool(timingdb->getPool());
                vct->reserve(20);
                gCapVecId_ = vct->getId();
            } else {
                vct = addr< ArrayObject<ObjectId> >(xCapVecId_);
            }
            if (vct != nullptr)
                vct->pushBack(xCap->getId());
        }
    }
}

void DNetParasitics::addResistor(ObjectId node1Id, ObjectId node2Id, float resValue) {
    //Cell *timingdb = getTopCell();
    //Cell *timingdb = Object::addr<Cell>(cellId);
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        auto res = timingdb->createObject<ParasiticResistor>(kObjectTypeParasiticResistor, timingdb->getId());
        if (res) {
            res->setOwner(this);
            res->setNode1Id(node1Id);
	    res->setNode2Id(node2Id);
            res->setResistance(resValue);

	    ArrayObject<ObjectId> *vct = nullptr;
            if (resVecId_ == UNINIT_OBJECT_ID) {
                vct = timingdb->createObject< ArrayObject<ObjectId> >(kObjectTypeArray, timingdb->getId());
                if (vct == nullptr)
                    return;
                vct->setPool(timingdb->getPool());
                vct->reserve(50);
                resVecId_ = vct->getId();
            } else {
                vct = addr< ArrayObject<ObjectId> >(resVecId_);
            }
            if (vct != nullptr)
                vct->pushBack(res->getId());
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
