/*************************************************************************
    > File Name: global_place.h
    > Author: Chenzhong Luo
    > Date: Nov 2020
 ************************************************************************/

#ifndef _DREAMPLACE_GLOBAL_PLACE_H
#define _DREAMPLACE_GLOBAL_PLACE_H

#include <vector>
#include <limits>
#include "flow/src/common_place_DB.h"
#include "utility/src/Msg.h"

DREAMPLACE_BEGIN_NAMESPACE

/// @brief a wrapper class of place main flow
class GlobalPlace
{
  public:
    GlobalPlace(Para& para)
    {
      db_ = CommonPlaceDB::getPlaceDBInstance(para);
    }
    GlobalPlace()
    {
      db_ = CommonPlaceDB::getPlaceDBInstance();
    }
    ~GlobalPlace()
    {}
    GlobalPlace(const GlobalPlace&)            = delete; // no copy
    GlobalPlace &operator=(const GlobalPlace&) = delete; // no copy

    const Para& getPara()              const { return db_->getPara();                  }
    int         getFlowSteps()         const { return getPara().getFlowSteps();        }
    bool        isDBReady()            const { return (db_ && db_->isCommonDBReady()); }

    int run();  // flow run
  private:
    CommonPlaceDB* db_;
};

DREAMPLACE_END_NAMESPACE

#endif

