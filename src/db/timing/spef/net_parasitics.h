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
    virtual bool isReduced() const { return false; }
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
    std::vector<ObjectId> getPinNode() const { return pinNodeVec_; }
    std::vector<ObjectId> getGroundCap() const { return gCapVec_; }
    std::vector<ObjectId> getCouplingCap() const { return xCapVec_; }
    std::vector<ObjectId> getResistor() const { return resVec_; } 

    ObjectId addPinNode(ObjectId pinId, bool isExtPin = false);
    ObjectId addIntNode(uint32_t intNodeId);
    ObjectId addExtNode(ObjectId netId, uint32_t extNodeId);
    void addGroundCap(ObjectId nodeId, float capValue);
    void addCouplingCap(ObjectId node1Id, ObjectId node2Id, float capValue);
    void addResistor(ObjectId node1Id, ObjectId node2Id, float resValue);
    

  protected:
    /// @brief overload output stream
    //friend OStreamBase &operator<<(OStreamBase &os, DNetParasitics const &rhs);

  private:
    /// The vector to store Pin Node pointer belongs to this net
    std::vector<ObjectId> pinNodeVec_;
    /// The vector to store Grounded Cap pointer belongs to this net
    std::vector<ObjectId> gCapVec_;
    /// The vector to store Coupling Cap pointer belongs to this net
    std::vector<ObjectId> xCapVec_;
    /// The vector to store Resistor pointer belongs to this net
    std::vector<ObjectId> resVec_;
};

class RNetParasitics : public NetParasitics {
  public:
    RNetParasitics();
    ~RNetParasitics();
     virtual bool isReduced() const { return true; }
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
