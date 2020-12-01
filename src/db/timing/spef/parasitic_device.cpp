/** 
 * @file parasitic_device.cpp
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

#include "db/timing/spef/parasitic_device.h"

namespace open_edi {
namespace db {

ParasiticNode::ParasiticNode()
    : ParasiticNode::BaseType() {
    setObjectType(ObjectType::kObjectTypeParasiticNode);
}

ParasiticIntNode::ParasiticIntNode()
    : ParasiticNode(),
      nodeId_(0) {
    setObjectType(ObjectType::kObjectTypeParasiticIntNode);
}

ParasiticPinNode::ParasiticPinNode()
    : ParasiticNode(),
      pinId_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeParasiticPinNode);
}

ParasiticExtNode::ParasiticExtNode()
    : ParasiticNode(),
      nodeNetId_(UNINIT_OBJECT_ID),
      nodeId_(0) {
    setObjectType(ObjectType::kObjectTypeParasiticExtNode);
}

ParasiticDevice::ParasiticDevice()
    : ParasiticDevice::BaseType(),
      node1Id_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeParasiticDevice);
}

ParasiticResistor::ParasiticResistor()
    : ParasiticDevice(),
      node2Id_(UNINIT_OBJECT_ID),
      resistance_(-1.0) {
    setObjectType(ObjectType::kObjectTypeParasiticResistor);
}

ParasiticXCap::ParasiticXCap()
    : ParasiticDevice(),   
      node2Id_(UNINIT_OBJECT_ID),
      capacitance_(-1.0) {
    setObjectType(ObjectType::kObjectTypeParasiticXCap);
}

ParasiticCap::ParasiticCap()
    : ParasiticDevice(),
      capacitance_(-1.0) {
    setObjectType(ObjectType::kObjectTypeParasiticCap);
}  

}  // namespace db
}  // namespace open_edi
