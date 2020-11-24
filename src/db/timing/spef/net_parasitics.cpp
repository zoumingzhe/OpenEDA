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

ObjectId DNetParasitics::addPinNode(ObjectId pinId, ObjectId cellId, bool isExtPin) {
    Cell *cell = getTopCell();
    //Cell *cell = Object::addr<Cell>(cellId);
    if (cell) {
        auto pinNode = cell->createObject<ParasiticPinNode>(kObjectTypeParasiticPinNode);
        if (pinNode) {
            pinNode->setOwner(this);
	    pinNode->setPinId(pinId);
	    
	    if (!isExtPin) { //Only add internal pin node
		ArrayObject<ObjectId> *vct = nullptr;
                if (pinNodeVecId_ == UNINIT_OBJECT_ID) {
		    vct = cell->createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
		    if (vct != nullptr) {
			vct->setPool(cell->getPool());
			vct->reserve(5);
			pinNodeVecId_ = vct->getId();
		    }
		} else {
		    vct = addr< ArrayObject<ObjectId> >(pinNodeVecId_);
		}
                if (vct != nullptr)
                    vct->pushBack(pinNode->getId());
            }
	    return pinNode->getId();
        }
    }
    return UNINIT_OBJECT_ID;
}

ObjectId DNetParasitics::addIntNode(ObjectId cellId, uint32_t intNodeId) {
    Cell *cell = getTopCell();
    //Cell *cell = Object::addr<Cell>(cellId);
    if (cell) {
        auto intNode = cell->createObject<ParasiticIntNode>(kObjectTypeParasiticIntNode);
        if (intNode) {
            intNode->setOwner(this);
            intNode->setIntNodeId(intNodeId);
            return intNode->getId();
        }
    }
    return UNINIT_OBJECT_ID;
}

ObjectId DNetParasitics::addExtNode(ObjectId netId, ObjectId cellId, uint32_t extNodeId) {
    Cell *cell = getTopCell();
    //Cell *cell = Object::addr<Cell>(cellId);
    if (cell) {
        auto extNode = cell->createObject<ParasiticExtNode>(kObjectTypeParasiticExtNode);
        if (extNode) {
            extNode->setOwner(this);
	    extNode->setExtNetId(netId);
            extNode->setExtNodeId(extNodeId);
            return extNode->getId();
        }
    }
    return UNINIT_OBJECT_ID;
}

void DNetParasitics::addGroundCap(ObjectId nodeId, ObjectId cellId, float capValue) {
    Cell *cell = getTopCell();
    //Cell *cell = Object::addr<Cell>(cellId);
    if (cell) {
        auto gCap = cell->createObject<ParasiticCap>(kObjectTypeParasiticCap);
        if (gCap) {
            gCap->setOwner(this);
	    gCap->setNode1Id(nodeId);
	    gCap->setCapacitance(capValue);

	    ArrayObject<ObjectId> *vct = nullptr;
            if (gCapVecId_ == UNINIT_OBJECT_ID) {
                vct = cell->createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
                if (vct == nullptr)
		    return;
                vct->setPool(cell->getPool());
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

void DNetParasitics::addCouplingCap(ObjectId node1Id, ObjectId node2Id, ObjectId cellId, float capValue) {
    Cell *cell = getTopCell();
    //Cell *cell = Object::addr<Cell>(cellId);
    if (cell) {
        auto xCap = cell->createObject<ParasiticXCap>(kObjectTypeParasiticXCap);
        if (xCap) {
            xCap->setOwner(this);
            xCap->setNode1Id(node1Id);
	    xCap->setNode2Id(node2Id);
            xCap->setCapacitance(capValue);

	    ArrayObject<ObjectId> *vct = nullptr;
            if (xCapVecId_ == UNINIT_OBJECT_ID) {
                vct = cell->createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
                if (vct == nullptr)
                    return;
                vct->setPool(cell->getPool());
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

void DNetParasitics::addResistor(ObjectId node1Id, ObjectId node2Id, ObjectId cellId, float resValue) {
    Cell *cell = getTopCell();
    //Cell *cell = Object::addr<Cell>(cellId);
    if (cell) {
        auto res = cell->createObject<ParasiticResistor>(kObjectTypeParasiticResistor);
        if (res) {
            res->setOwner(this);
            res->setNode1Id(node1Id);
	    res->setNode2Id(node2Id);
            res->setResistance(resValue);

	    ArrayObject<ObjectId> *vct = nullptr;
            if (resVecId_ == UNINIT_OBJECT_ID) {
                vct = cell->createObject< ArrayObject<ObjectId> >(kObjectTypeArray);
                if (vct == nullptr)
                    return;
                vct->setPool(cell->getPool());
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
