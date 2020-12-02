/** 
 * @file parasitic_device.h
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

#ifndef EDI_DB_TIMING_SPEF_PARASITIC_DEVICE_H_
#define EDI_DB_TIMING_SPEF_PARASITIC_DEVICE_H_

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "db/core/object.h"
#include "db/util/array.h"
#include "util/data_traits.h"

namespace open_edi {
namespace db {

class ParasiticNode : public Object {
  public:
    using BaseType = Object;
    
    ParasiticNode();
    ~ParasiticNode() {}

    //virtual bool isIntNode() const { return false; }
    //virtual bool isPinNode() const { return false; }
    //virtual bool isExtNode() const { return false; }
};

class ParasiticIntNode : public ParasiticNode {
  public:
    ParasiticIntNode();
    ~ParasiticIntNode() {}

    //virtual bool isIntNode() const { return true; } 
    void setIntNodeId(uint32_t nodeId) { nodeId_ = nodeId; }
    uint32_t getIntNodeId() const { return nodeId_; }

  private:
    uint32_t nodeId_; 
};

class ParasiticPinNode : public ParasiticNode {
  public:
    ParasiticPinNode();
    ~ParasiticPinNode() {}
 
    //virtual bool isPinNode() const { return true; }
    void setPinId(ObjectId pinId) { pinId_ = pinId; }
    ObjectId getPinId() const { return pinId_; }

  private:
    ObjectId pinId_;
};

class ParasiticExtNode : public ParasiticNode {
  public:
    ParasiticExtNode();
    ~ParasiticExtNode() {}

    //virtual bool isExtNode() const { return true; }
    void setExtNetId(ObjectId nodeNetId) { nodeNetId_ = nodeNetId; }
    ObjectId getExtNetId() const { return nodeNetId_; }
    void setExtNodeId(uint32_t nodeId) { nodeId_ = nodeId; }
    uint32_t getExtNodeId() const { return nodeId_; }

  private:
    ObjectId nodeNetId_;
    uint32_t nodeId_;
};

class ParasiticDevice : public Object {
  public:
    using BaseType = Object;

    ParasiticDevice();
    ~ParasiticDevice() {}

    //virtual bool isResistor() const { return false; }
    //virtual bool isGroundCap() const { return false; }
    //virtual bool isXCap() const { return false; }
    void setNode1Id(ObjectId node1Id) { node1Id_ = node1Id; }
    ObjectId getNode1Id() const { return node1Id_; }

  private:
    ObjectId node1Id_;
};

class ParasiticResistor : public ParasiticDevice {
  public:
    ParasiticResistor();
    ~ParasiticResistor();

    //virtual bool isResistor() const { return true; }
    void setResistance(float resValue) { resistance_ = resValue; }
    float getResistance() const { return resistance_; }
    void setNode2Id(ObjectId node2Id) { node2Id_ = node2Id; }
    ObjectId getNode2Id() const { return node2Id_; }

  private:
    ObjectId node2Id_;
    float resistance_;
};

class ParasiticXCap : public ParasiticDevice {
  public:
    ParasiticXCap();
    ~ParasiticXCap();

    //virtual bool isXCap() const { return true; }
    void setCapacitance(float capValue) { capacitance_ = capValue; }
    float getCapacitance() const { return capacitance_; }
    void setNode2Id(ObjectId node2Id) { node2Id_ = node2Id; }
    ObjectId getNode2Id() const { return node2Id_; }

   private:
    ObjectId node2Id_;
    float capacitance_; 
};

class ParasiticCap : public ParasiticDevice {
  public:
    ParasiticCap();
    ~ParasiticCap();

    //virtual bool isGroundCap() const { return true; }
    void setCapacitance(float capValue) { capacitance_ = capValue; }
    float getCapacitance() const { return capacitance_; }

   private:
    float capacitance_;
};

}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_TIMING_SPEF_PARASITICS_DEVICE_H_
