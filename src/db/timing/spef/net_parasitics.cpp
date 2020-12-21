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

extern uint32_t net_with_bid_;

namespace open_edi {
namespace db {

NetParasitics::NetParasitics()
    : NetParasitics::BaseType(),
      net_id_(UNINIT_OBJECT_ID),
      total_cap_(0.0) {
    setObjectType(ObjectType::kObjectTypeNetParasitics);
}

NetParasitics::~NetParasitics() {
}

DNetParasitics::DNetParasitics()
    : NetParasitics(),
      gcap_vec_id_(UNINIT_OBJECT_ID),
      xcap_vec_id_(UNINIT_OBJECT_ID),
      res_vec_id_(UNINIT_OBJECT_ID),
      roots_(nullptr),
      adjacent_map_(nullptr),
      node_gcap_map_(nullptr) {
    setObjectType(ObjectType::kObjectTypeDNetParasitics);
}

DNetParasitics::~DNetParasitics()
{
    delete roots_;
    delete adjacent_map_;
    delete node_gcap_map_;
}

///Don't need to save it as we could get connecting pins directly from net
/*void DNetParasitics::addPinNode(ObjectId pinNodeId) {
    ArrayObject<ObjectId> *vct = nullptr;
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        if (pin_node_vec_id_ == UNINIT_OBJECT_ID) {
            vct = timingdb->createObject< ArrayObject<ObjectId> >(kObjectTypeArray, timingdb->getId());
            if (vct != nullptr) {
                vct->setPool(timingdb->getPool());
                vct->reserve(5);
                pin_node_vec_id_ = vct->getId();
            }
        } else 
            vct = addr< ArrayObject<ObjectId> >(pin_node_vec_id_);
                
        if (vct != nullptr)
            vct->pushBack(pinNodeId);    
    }
}*/

ObjectId DNetParasitics::createPinNode(ObjectId pinId) {
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
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        auto gCap = timingdb->createObject<ParasiticCap>(kObjectTypeParasiticCap, timingdb->getId());
        if (gCap) {
            gCap->setOwner(this);
	    gCap->setNode1Id(nodeId);
	    gCap->setCapacitance(capValue);

	    ArrayObject<ObjectId> *vct = nullptr;
            if (gcap_vec_id_ == UNINIT_OBJECT_ID) {
                vct = timingdb->createObject< ArrayObject<ObjectId> >(kObjectTypeArray, timingdb->getId());
                if (vct == nullptr)
		    return;
                vct->setPool(timingdb->getPool());
                vct->reserve(20);
                gcap_vec_id_ = vct->getId();
            } else {
                vct = addr< ArrayObject<ObjectId> >(gcap_vec_id_);
            }
            if (vct != nullptr)
                vct->pushBack(gCap->getId());
        }
    }
}

void DNetParasitics::addCouplingCap(ObjectId node1Id, ObjectId node2Id, float capValue) {
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        auto xCap = timingdb->createObject<ParasiticXCap>(kObjectTypeParasiticXCap, timingdb->getId());
        if (xCap) {
            xCap->setOwner(this);
            xCap->setNode1Id(node1Id);
	    xCap->setNode2Id(node2Id);
            xCap->setCapacitance(capValue);

	    ArrayObject<ObjectId> *vct = nullptr;
            if (xcap_vec_id_ == UNINIT_OBJECT_ID) {
                vct = timingdb->createObject< ArrayObject<ObjectId> >(kObjectTypeArray, timingdb->getId());
                if (vct == nullptr)
                    return;
                vct->setPool(timingdb->getPool());
                vct->reserve(10);
                xcap_vec_id_ = vct->getId();
            } else {
                vct = addr< ArrayObject<ObjectId> >(xcap_vec_id_);
            }
            if (vct != nullptr)
                vct->pushBack(xCap->getId());
        }
    }
}

void DNetParasitics::addResistor(ObjectId node1Id, ObjectId node2Id, float resValue) {
    Timing *timingdb = getTimingLib();
    if (timingdb) {
        auto res = timingdb->createObject<ParasiticResistor>(kObjectTypeParasiticResistor, timingdb->getId());
        if (res) {
            res->setOwner(this);
            res->setNode1Id(node1Id);
	    res->setNode2Id(node2Id);
            res->setResistance(resValue);

	    ArrayObject<ObjectId> *vct = nullptr;
            if (res_vec_id_ == UNINIT_OBJECT_ID) {
                vct = timingdb->createObject< ArrayObject<ObjectId> >(kObjectTypeArray, timingdb->getId());
                if (vct == nullptr)
                    return;
                vct->setPool(timingdb->getPool());
                vct->reserve(20);
                res_vec_id_ = vct->getId();
            } else {
                vct = addr< ArrayObject<ObjectId> >(res_vec_id_);
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

void DNetParasitics::addRoot(ParasiticPinNode *node) {
    if (!roots_) {
        roots_ = new std::vector<ParasiticPinNode*>();
    }
    roots_->push_back(node);
    if (roots_->size() == 2) {
        ++net_with_bid_;
    }
}

}  // namespace db
}  // namespace open_edi
