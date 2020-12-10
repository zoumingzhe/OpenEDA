/*************************************************************************
    > File Name: lpdp_place.h
    > Author: Fei
    > Mail:
    > Created Time: Fri 25 Sep 2020 03:20:14 PM CDT
 ************************************************************************/

#ifndef _DREAMPLACE_LPDP_PLACE_H
#define _DREAMPLACE_LPDP_PLACE_H

#include <vector>
#include <limits>
#include "flow/src/common_place_DB.h"
#include "lp_dp/greedy_legalize/src/greedy_legalize.h"
#include "lp_dp/macro_legalize/src/macro_legalize.h"
#include "lp_dp/abacus_legalize/src/abacus_legalize.h"
#include "lp_dp/legality_check/src/legality_check.h"
#include "lp_dp/global_swap/src/global_swap.h"
#include "lp_dp/k_reorder/src/k_reorder.h"
#include "lp_dp/independent_set_matching/src/independent_set_matching.h"
#include "utility/src/Msg.h"

DREAMPLACE_BEGIN_NAMESPACE

#define PLACE_DISPATCH_CUSTOM_TYPES(TYPE, NAME, ...)                                   \
      [&] {                                                                            \
          switch (TYPE) {                                                              \
                AT_PRIVATE_CASE_TYPE(at::ScalarType::Float, float, __VA_ARGS__)        \
                AT_PRIVATE_CASE_TYPE(at::ScalarType::Double, double, __VA_ARGS__)      \
                AT_PRIVATE_CASE_TYPE(at::ScalarType::Int, int, __VA_ARGS__)            \
                default:                                                               \
                  AT_ERROR(#NAME, " not implemented for '", at::toString(TYPE), "'");  \
                  }                                                                    \
           }()

/// @brief a wrapper class of place main flow
class LpdpPlace
{
  public:
    LpdpPlace(Para& para)
    {
      db_ = CommonPlaceDB::getPlaceDBInstance(para);
    }
    LpdpPlace()
    {
      db_ = CommonPlaceDB::getPlaceDBInstance();
    }
    ~LpdpPlace()
    {}
    LpdpPlace(const LpdpPlace&)            = delete; // no copy
    LpdpPlace &operator=(const LpdpPlace&) = delete; // no copy

    const Para& getPara()              const { return db_->getPara();                  }
    int         getFlowSteps()         const { return getPara().getFlowSteps();        }
    bool        isDBReady()            const { return (db_ && db_->isCommonDBReady()); }

    void run();  // flow run
  private:
    CommonPlaceDB* db_;
};

DREAMPLACE_END_NAMESPACE

#endif
