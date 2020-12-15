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
};

class ParasiticIntNode : public ParasiticNode {
  public:
    ParasiticIntNode();
    ~ParasiticIntNode() {}

    void setIntNodeId(uint32_t nodeId) { node_id_ = nodeId; }
    uint32_t getIntNodeId() const { return node_id_; }

  private:
    uint32_t node_id_; 
};

class ParasiticPinNode : public ParasiticNode {
  public:
    ParasiticPinNode();
    ~ParasiticPinNode() {}
 
    void setPinId(ObjectId pinId) { pin_id_ = pinId; }
    ObjectId getPinId() const { return pin_id_; }

  private:
    ObjectId pin_id_;
};

class ParasiticExtNode : public ParasiticNode {
  public:
    ParasiticExtNode();
    ~ParasiticExtNode() {}

    void setExtNetId(ObjectId nodeNetId) { node_net_id_ = nodeNetId; }
    ObjectId getExtNetId() const { return node_net_id_; }
    void setExtNodeId(uint32_t nodeId) { node_id_ = nodeId; }
    uint32_t getExtNodeId() const { return node_id_; }

  private:
    ObjectId node_net_id_;
    uint32_t node_id_;
};

class ParasiticDevice : public Object {
  public:
    using BaseType = Object;

    ParasiticDevice();
    ~ParasiticDevice() {}

    void setNode1Id(ObjectId node1Id) { node1_id_ = node1Id; }
    ObjectId getNode1Id() const { return node1_id_; }

  private:
    ObjectId node1_id_;
};

class ParasiticResistor : public ParasiticDevice {
  public:
    ParasiticResistor();
    ~ParasiticResistor();

    void setResistance(float resValue) { resistance_ = resValue; }
    float getResistance() const { return resistance_; }
    void setNode2Id(ObjectId node2Id) { node2_id_ = node2Id; }
    ObjectId getNode2Id() const { return node2_id_; }

  private:
    ObjectId node2_id_;
    float resistance_;
};

class ParasiticXCap : public ParasiticDevice {
  public:
    ParasiticXCap();
    ~ParasiticXCap();

    void setCapacitance(float capValue) { capacitance_ = capValue; }
    float getCapacitance() const { return capacitance_; }
    void setNode2Id(ObjectId node2Id) { node2_id_ = node2Id; }
    ObjectId getNode2Id() const { return node2_id_; }

   private:
    ObjectId node2_id_;
    float capacitance_; 
};

class ParasiticCap : public ParasiticDevice {
  public:
    ParasiticCap();
    ~ParasiticCap();

    void setCapacitance(float capValue) { capacitance_ = capValue; }
    float getCapacitance() const { return capacitance_; }

   private:
    float capacitance_;
};

struct OptParaNode {
    /// ParasiticNode
    ObjectId    node;
    ObjectId    parent;
    float       resistance;
    float       capacitance;
};

// class ParasiticEdge {
//   public:
//     ParasiticEdge(ParasiticVertex *vtx, float resistance) :
//         vertex_(vtx), resistance_(resistance) {};
//     ParasiticVertex* getNext() const;
//   private:
//     ParasiticVertex *vertex_;
//     float resistance_;
// };
// 
// class ParasiticVertex {
//   private:
//     ParasiticNode *node_;
//     std::list<ParasiticEdge> edges_;
// };

}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_TIMING_SPEF_PARASITICS_DEVICE_H_
