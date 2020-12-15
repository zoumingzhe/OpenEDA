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
      node_id_(0) {
    setObjectType(ObjectType::kObjectTypeParasiticIntNode);
}

ParasiticPinNode::ParasiticPinNode()
    : ParasiticNode(),
      pin_id_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeParasiticPinNode);
}

ParasiticExtNode::ParasiticExtNode()
    : ParasiticNode(),
      node_net_id_(UNINIT_OBJECT_ID),
      node_id_(0) {
    setObjectType(ObjectType::kObjectTypeParasiticExtNode);
}

ParasiticDevice::ParasiticDevice()
    : ParasiticDevice::BaseType(),
      node1_id_(UNINIT_OBJECT_ID) {
    setObjectType(ObjectType::kObjectTypeParasiticDevice);
}

ParasiticResistor::ParasiticResistor()
    : ParasiticDevice(),
      node2_id_(UNINIT_OBJECT_ID),
      resistance_(-1.0) {
    setObjectType(ObjectType::kObjectTypeParasiticResistor);
}

ParasiticXCap::ParasiticXCap()
    : ParasiticDevice(),   
      node2_id_(UNINIT_OBJECT_ID),
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
