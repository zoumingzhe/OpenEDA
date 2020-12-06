/**
 * @file   k_reorder.h
 * @author Chenzhong Luo
 * @date   Sep 2020
 */

#ifndef _DREAMPLACE_OEM_K_REORDER_H
#define _DREAMPLACE_OEM_K_REORDER_H

#include <vector>
#include <limits>
#include "utility/src/Msg.h"
#include "flow/src/common_place_DB.h"

DREAMPLACE_BEGIN_NAMESPACE

#ifdef _CUDA_FOUND
int kreorderCUDARun(DetailedPlaceDB<int>& db, int K, int maxIters, int numThreads);
#endif
/// @brief a wrapper class of required data for edi db
class KReorder
{
  public:
    KReorder()
    {
      db_ = CommonPlaceDB::getPlaceDBInstance();
      is_gpu_ = db_->getPara().isGPU();
      num_threads_ = db_->getPara().getNumThreads();
    }
    ~KReorder()
    {}
    KReorder(const KReorder&) = delete;            // no copy
    KReorder &operator=(const KReorder&) = delete; // no copy

    bool           isCommonDBReady()          const { return db_ && db_->isCommonDBReady();   }
    // interface 
    void run();

  private:
    void run_cpu();
    void run_gpu();
  private:
    CommonPlaceDB* db_;
    //params; default refer to k_reorder.py, KReorder::__init__. ./10/8.
    //default in BasicPlace.py, 4/2/params.num_threads.
    int K_ = 4;
    int max_iters_ = 2;
    int num_threads_ = 8;
    bool is_gpu_ = true;
};

DREAMPLACE_END_NAMESPACE

#endif
