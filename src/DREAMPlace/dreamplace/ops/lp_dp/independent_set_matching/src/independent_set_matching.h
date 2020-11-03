/**
 * @file   independent_set_matching.h
 * @author Chenzhong Luo
 * @date   Oct 2020
 */

#ifndef _DREAMPLACE_OEM_INDEPENDENT_SET_MATCHING_H
#define _DREAMPLACE_OEM_INDEPENDENT_SET_MATCHING_H


#include <vector>
#include <limits>
#include "utility/src/Msg.h"
#include "flow/src/common_place_DB.h"

DREAMPLACE_BEGIN_NAMESPACE

/// @brief a wrapper class of required data for edi db
class IndependentSetMatching
{
  public:
    IndependentSetMatching() 
    {
      CommonPlaceDB* db_ = CommonPlaceDB::getPlaceDBInstance();
      is_gpu_ = db_->getPara().isGPU();
      num_threads_ = db_->getPara().getNumThreads();
    }
    ~IndependentSetMatching()
    {
    }
    IndependentSetMatching(const IndependentSetMatching&) = delete;            // no copy
    IndependentSetMatching &operator=(const IndependentSetMatching&) = delete; // no copy

    // interface 
    void run();

  private:
    void run_cpu();
    void run_gpu();
  private:
    CommonPlaceDB* db_;
    //params; default in independent_set_matching.py, IndependentSetMatching::__init__. ./././'concurrent'/8.
    //default in BasicPlace.py, 2048/128/50/'concurrent'/params.num_threads.
    int batch_size_ = 2048;
    int set_size_ = 128;
    int max_iters_ = 50;
    int num_threads_ = 8;
    bool is_gpu_ = true;
    bool alg_concurrent_ = true;
};

DREAMPLACE_END_NAMESPACE

#endif
