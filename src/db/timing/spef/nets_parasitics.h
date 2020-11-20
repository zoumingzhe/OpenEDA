/** 
 * @file nets_parasitics.h
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

#ifndef EDI_DB_TIMING_SPEF_NETS_PARASITICS_H_
#define EDI_DB_TIMING_SPEF_NETS_PARASITICS_H_

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "db/timing/spef/net_parasitics.h"
#include "db/core/object.h"
#include "db/core/net.h"
#include "db/core/term.h"
#include "db/util/symbol_page.h"


namespace open_edi {
namespace db {

class NetParasitics;

class NetsParasitics : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    NetsParasitics();

    /// @brief destructor
    ~NetsParasitics();

    /// @brief constructor
    NetsParasitics(Object *owner, IndexType id);

    /// @brief copy constructor
    NetsParasitics(NetsParasitics const &rhs);

    /// @brief move constructor
    NetsParasitics(NetsParasitics &&rhs) noexcept;

    /// @brief copy assignment
    NetsParasitics &operator=(NetsParasitics const &rhs);

    /// @brief move assignment
    NetsParasitics &operator=(NetsParasitics &&rhs) noexcept;

    void setDivider(char divider) { divider_ = divider; }
    char getDivider() const { return divider_; }
    void setDelimiter(char delimiter) { delimiter_ = delimiter; }
    char getDelimiter() const { return delimiter_; }
    void setPreBusDel(char prebusdel) { preBusDel_ = prebusdel; }
    void setSufBusDel(char sufbusdel) { sufBusDel_ = sufbusdel; }
    char getPreBusDel() const { return preBusDel_; }
    char getSufBusDel() const { return sufBusDel_; }
    void setCellId(ObjectId cellId) { cellId_ = cellId; }
    ObjectId getCellId() const { return cellId_; }
    void addDesignFlow(std::string flowStr) { designFlowVec_.push_back(flowStr); }
    //const std::vector<std::string> getDesignFlow(void) { return designFlowVec_; }
    void setTimeScale(float scale) { timeScale_ = scale; }
    float getTimeScale() const { return timeScale_; }
    void setCapScale(float scale) { capScale_ = scale; }
    float getCapScale() const { return capScale_; }
    void setResScale(float scale) { resScale_ = scale; }
    float getResScale() const { return resScale_; }
    void setInductScale(float scale) { inductScale_ = scale; }
    float getInductScale() const { return inductScale_; }
    void addNameMap(uint32_t index, SymbolIndex symIdx) { nameMap_[index] = symIdx; }
    //std::unordered_map<uint32_t, SymbolIndex> getNameMap() { return nameMap_; } 
    //void addPowerNet(ObjectId sNetId) { pwrNetVec_.push_back(sNetId); }
    //void addGroundNet(ObjectId sNetId) { gndNetVec_.push_back(sNetId); }
    bool isDigits(const char *str);
    Net* findNet(const char *netName);
    Pin* findPin(const char *pinName);
    ObjectId addNode(DNetParasitics *netParasitics, const char *nodeName);
    DNetParasitics* addDNetParasitics(ObjectId netId, float totCap);
    void addGroundCap(ObjectId netId, char *nodeName, float capValue);
    void addCouplingCap(ObjectId netId, char *nodeName1, char *nodeName2, float xCapValue);
    void addResistor(ObjectId netId, char *nodeName1, char *nodeName2, float resValue);
    RNetParasitics* addRNetParasitics(ObjectId netId, float totCap);
    //std::unordered_map<ObjectId, ObjectId> getNetParasiticsMap() { return netParasiticsMap_;}

  protected:
    /// @brief copy object
    void copy(NetsParasitics const &rhs);
    /// @brief move object
    void move(NetsParasitics &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, NetsParasitics const &rhs);

  private:
    /// Net ObjectId and NetParasitics ObjectId Map
    std::unordered_map<ObjectId, ObjectId> netParasiticsMap_;
    /// The map to save name mapping defined in SPEF
    std::unordered_map<uint32_t, SymbolIndex> nameMap_;
    /// The vector to save definition in design flow
    std::vector<std::string> designFlowVec_;
    /// The vector to save Power nets, dont to save it for now
    //std::vector<ObjectId> pwrNetVec_;
    /// The vector to save Ground nets dont to save it for now
    //std::vector<ObjectId> gndNetVec_;
    /// The vector to save ports
    std::vector<ObjectId> portsVec_;
    char divider_;
    char delimiter_;
    char preBusDel_;
    char sufBusDel_;
    ObjectId cellId_;
    float timeScale_;
    float resScale_;
    float capScale_;
    float inductScale_;
};

}  // namespace db
}  // namespace open_edi

#endif  // EDI_DB_TIMING_SPEF_NETS_PARASITICS_H_
