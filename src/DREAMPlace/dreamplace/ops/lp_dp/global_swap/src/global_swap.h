/**
 * @file   global_swap.h
 * @author Chenzhong Luo
 * @date   Sep 2020
 */

#ifndef _DREAMPLACE_OEM_GLOBAL_SWAP_H
#define _DREAMPLACE_OEM_GLOBAL_SWAP_H

#include <vector>
#include <limits>
#include "utility/src/Msg.h"
#include "flow/src/common_place_DB.h"

DREAMPLACE_BEGIN_NAMESPACE

/// @brief a wrapper class of required data for edi db
class GlobalSwap
{
  public:
    GlobalSwap()
    {
        CommonPlaceDB* db_ = CommonPlaceDB::getPlaceDBInstance();
        is_gpu_ = db_->getPara().isGPU();
        num_threads_ = db_->getPara().getNumThreads();
    }
    ~GlobalSwap()
    {
    }
    GlobalSwap(const GlobalSwap&) = delete;            // no copy
    GlobalSwap &operator=(const GlobalSwap&) = delete; // no copy

    // interface 
    void run();

  private:
    void run_cpu();
    void run_gpu();
  private:
    CommonPlaceDB* db_;
    //params; default refer to global_swap.py, GlobalSwap::__init__. 32/10/8.
    //default in BasicPlace.py, 256/2/params.num_threads.
    int batch_size_ = 256;
    int max_iters_ = 2;
    int num_threads_ = 8;
    bool is_gpu_ = true;
    bool alg_concurrent_ = true;
};

DREAMPLACE_END_NAMESPACE

#endif
