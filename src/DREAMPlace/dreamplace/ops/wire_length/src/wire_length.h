/* @file  wire_lenght.h
 * @date  Oct 2020
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NiiCEDA Inc.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
/*************************************************************************/                         
#ifndef EDI_WIRE_LENGTH_H_
#define EDI_WIRE_LENGTH_H_

#include <vector>
#include <limits>
#include "util/util.h"
#include "utility/src/Msg.h"
#include "flow/src/common_place_DB.h"

DREAMPLACE_BEGIN_NAMESPACE
enum WLType 
{
  kDEFAULT = 0,
  kHPWL = 1,
  kMST = 2,
};

std::string kWLTypeName[3] = {"none", "HPWL", "MST"};

class WireLength
{
  public:
    WireLength(bool gpu) : gpu_(gpu)
    {
      db_ = CommonPlaceDB::getPlaceDBInstance();
    }
    ~WireLength()
    {
      CommonPlaceDB::freePlaceDBInstance();
    }
    WireLength(const WireLength&)            = delete; // no copy
    WireLength &operator=(const WireLength&) = delete; // no copy

    WLType      getType()              const { return type_;   }
    bool        isGPU()		       const { return gpu_;    }
    bool        isDBReady()            const { return (db_ && db_->isCommonDBReady()); }
    double      calcHPWLInCPU();
    double      calcHPWLInGPU();
    double      calcHPWL();
    double      calcMST();

    void run();  // flow run
  private:
    
    CommonPlaceDB* db_ = nullptr;
    WLType type_       = kDEFAULT;
    bool gpu_          = false;
};


DREAMPLACE_END_NAMESPACE

#endif // EDI_WIRE_LENGTH_H_

