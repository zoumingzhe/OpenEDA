/** 
 * @file spef_reader.h
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

#ifndef EDI_DB_TIMING_SPEF_SPEF_READER_H_
#define EDI_DB_TIMING_SPEF_SPEF_READER_H_

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <ctype.h>
#include <iostream>

#include "stdlib.h"
#include "db/core/object.h"
#include "db/util/array.h"
#include "util/data_traits.h"
#include "db/timing/spef/design_parasitics.h"
#include "db/timing/spef/nets_parasitics.h"

namespace open_edi {
namespace db {
  class DesignParasitics;
  class NetsParasitics;
}
}
using namespace open_edi::db;

namespace SpefReader {

typedef std::vector<const char*> StringVec;

class SpefReader {
  public:
    SpefReader(std::string fileName, DesignParasitics *designParasitics);
    ~SpefReader();

    void stringDelete(const char *str);
    void recordChar(const char ch) { recordStr_ += ch; }
    const char* getRecordStr() { return stringCopy(recordStr_.c_str()); }
    void clearRecordStr() { recordStr_.clear(); }
    const char* stringCopy(const char *str);
    bool isDigits(const char *str);
    Cell* getCell() const { return cell_; }
    void setCell(const char *designName);
    void addDesignNetsParasitics();
    void setDivider(const char divider) { if (netsParasitics_) netsParasitics_->setDivider(divider); }
    void setDelimiter(const char delimiter) { if (netsParasitics_) netsParasitics_->setDelimiter(delimiter); }
    void setPreBusDel(const char prebusdel) { if (netsParasitics_) netsParasitics_->setPreBusDel(prebusdel); }
    void setSufBusDel(const char sufbusdel) { if (netsParasitics_) netsParasitics_->setSufBusDel(sufbusdel); }
    void setTimeScale(float digits, const char *unit);
    void setCapScale(float digits, const char *unit);
    void setResScale(float digits, const char *unit);
    void setInductScale(float digits, const char *unit);
    void setDesignFlow(StringVec *dsgFlow);
    void addNameMap(const char *index, const char *name);
    void addPort(const char *name);
    float addParValue(float value1) { parValue_ = value1; return parValue_; }
    float addParValue(float value1, float value2, float value3);
    Net* findNet(const char *name);
    Pin* findPin(const char *name);
    void addDNetBegin(Net *net);
    void addDNetEnd();
    void addPinNode(const char *pinName);
    ObjectId getParasiticNode(std::string nodeName);
    void addGroundCap(const char *nodeName);
    void addCouplingCap(const char *nodeName1, const char *nodeName2);
    void addResistor(const char *nodeName1, const char *nodeName2);
    void addRNetBegin(Net *net);
    void addRNetDrvr(const char *pinName);
    void addPiModel(float c2, float r1, float c1); 
    void addRNetEnd() { net_ = nullptr; rnetParasitics_ = nullptr; netNodeMap_.clear(); }
    void setSpefField(uint8_t spefFiled) { spefField_ = spefFiled; }
    uint8_t getSpefField() const { return spefField_; }
    void incrLineNo() { lineNo_++; }
    uint32_t getLineNo() const { return lineNo_; }
    std::string getSpefFile() const { return spefFileName_; }
    bool parseSpefFile();

  private:
    void __spef_parse_begin(FILE *fp);
    void __spef_parse_end(FILE *fp);
    int  __spef_parse();

    std::string spefFileName_;
    std::string recordStr_;
    Cell *cell_;
    DesignParasitics *designParasitics_;
    NetsParasitics *netsParasitics_;
    float parValue_;
    Net *net_;
    DNetParasitics *dnetParasitics_;
    RNetParasitics *rnetParasitics_;
    uint8_t spefField_;
    uint32_t lineNo_;
    std::unordered_map<std::string, ObjectId> netNodeMap_; //use to check if node created for net
    void *scanner_;
    //for test
    std::string tempStr_; 
};

}  // namespace SpefReader

#endif  // EDI_DB_TIMING_SPEF_SPEF_READER_H_
