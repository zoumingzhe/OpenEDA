/*************************************************************************
    > File Name: DetailedPlaceDBUtils.h
    > Author: Fei
    > Mail:
    > Created Time: Fri 25 Sep 2020 03:20:14 PM CDT
 ************************************************************************/

#ifndef _DREAMPLACE_OEM_MAIN_PLACE_H
#define _DREAMPLACE_OEM_MAIN_PLACE_H

#include <vector>
#include <limits>
#include "flow/src/common_place_DB.h"
#include "flow/src/global_place.h"
#include "flow/src/lpdp_place.h"
#include "lp_dp/greedy_legalize/src/greedy_legalize.h"
#include "lp_dp/macro_legalize/src/macro_legalize.h"
#include "lp_dp/abacus_legalize/src/abacus_legalize.h"
#include "lp_dp/legality_check/src/legality_check.h"
#include "lp_dp/k_reorder/src/k_reorder.h"
#include "lp_dp/global_swap/src/global_swap.h"
#include "lp_dp/independent_set_matching/src/independent_set_matching.h"
#include "utility/src/Msg.h"

DREAMPLACE_BEGIN_NAMESPACE

/// @brief a wrapper class of place main flow
class MainPlace
{
  public:
    MainPlace(Para& para)
    {
      db_ = CommonPlaceDB::getPlaceDBInstance(para);
    }
    ~MainPlace()
    {
      CommonPlaceDB::freePlaceDBInstance();
    }
    MainPlace(const MainPlace&)            = delete; // no copy
    MainPlace &operator=(const MainPlace&) = delete; // no copy

    const Para& getPara()              const { return db_->getPara();                  }
    int         getFlowSteps()         const { return getPara().getFlowSteps();        }
    bool        isDBReady()            const { return (db_ && db_->isCommonDBReady()); }

    void run();  // flow run
  private:
    CommonPlaceDB* db_;
};

// init class static object

DREAMPLACE_END_NAMESPACE

#endif
