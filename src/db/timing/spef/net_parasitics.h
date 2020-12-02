/** 
 * @file net_parasitics.h
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

#ifndef EDI_DB_TIMING_SPEF_NET_PARASITICS_H_
#define EDI_DB_TIMING_SPEF_NET_PARASITICS_H_

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "db/core/object.h"
#include "db/util/array.h"
#include "util/data_traits.h"
#include "parasitic_device.h"

namespace open_edi {
namespace db {


class NetParasitics : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    NetParasitics();
    /// @brief destructor
    ~NetParasitics();
    //virtual bool isReduced() const { return false; }
    ObjectId getNetId() const { return netId_; }
    void setNetId(ObjectId netId) { netId_ = netId; } 
    float getNetTotalCap() const { return totalCap_; }
    void setNetTotalCap(float totCap) { totalCap_ = totCap; }

  private:
    ObjectId netId_;
    float totalCap_;
};

class DNetParasitics : public NetParasitics {
  public:
    DNetParasitics();
    ~DNetParasitics();
    ObjectId getPinNodeVecId() const { return pinNodeVecId_; }
    ObjectId getGroundCapVecId() const { return gCapVecId_; }
    ObjectId getCouplingCapVecId() const { return xCapVecId_; }
    ObjectId getResistorVecId() const { return resVecId_; } 

    ObjectId createPinNode(ObjectId pinId);
    ObjectId createIntNode(uint32_t intNodeId);
    ObjectId createExtNode(ObjectId netId, uint32_t extNodeId);
    void addPinNode(ObjectId pinNodeId);
    void addGroundCap(ObjectId nodeId, float capValue);
    void addCouplingCap(ObjectId node1Id, ObjectId node2Id, float capValue);
    void addResistor(ObjectId node1Id, ObjectId node2Id, float resValue);
    

  protected:
    /// @brief overload output stream
    //friend OStreamBase &operator<<(OStreamBase &os, DNetParasitics const &rhs);

  private:
    /// The vector to store Pin Node pointer belongs to this net
    ObjectId pinNodeVecId_;
    /// The vector to store Grounded Cap pointer belongs to this net
    ObjectId gCapVecId_;
    /// The vector to store Coupling Cap pointer belongs to this net
    ObjectId xCapVecId_;
    /// The vector to store Resistor pointer belongs to this net
    ObjectId resVecId_;
};

class RNetParasitics : public NetParasitics {
  public:
    RNetParasitics();
    ~RNetParasitics();
     //virtual bool isReduced() const { return true; }
     void setDriverPinId(ObjectId drvrPinId) { drvrPinId_ = drvrPinId; }
     ObjectId getDriverPinId() const { return drvrPinId_; }
     void setC2(float c2) { c2_ = c2; }
     void setR1(float r1) { r1_ = r1; }
     void setC1(float c1) { c1_ = c1; }
     float getC2() const { return c2_; }
     float getR1() const { return r1_; }
     float getC1() const { return c1_; }

  protected:
    /// @brief overload output stream
    //friend OStreamBase &operator<<(OStreamBase &os, RNetParasitics const &rhs);  

  private:
    /// Driver pin ObjectId
    ObjectId drvrPinId_;
    float c2_;
    float r1_;
    float c1_;
};

}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_TIMING_SPEF_NETS_PARASITICS_H_
