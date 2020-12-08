/**
 * @file   global_swap.cpp
 * @author Chenzhong Luo
 * @date   Oct 2020
 */

#include <vector>
#include <limits>
#include "utility/src/Msg.h"
#include "lp_dp/global_swap/src/global_swap.h"
#include "utility/src/DetailedPlaceDB.h"
//#include "utility/src/utils.cuh"

DREAMPLACE_BEGIN_NAMESPACE

void GlobalSwap::run_cpu()
{
    //make DetailedPlaceDB db.
    DetailedPlaceDB<int> db;
    db_->initDetailedPlaceDB(db); //cpu version.
    if (alg_concurrent_)
    {
      globalSwapCPURun(db, batch_size_, max_iters_, num_threads_);
    }
    else
    {
      globalSwapCPURun(db, max_iters_);
    }
    db_->updateXY(db.x, db.y);
    db_->freeDetailedPlaceDB(db);

    return;
}

void GlobalSwap::run_gpu()
{
    //make DetailedPlaceDB db.
    DetailedPlaceDB<int> db;
    db_->initDetailedPlaceDB(db); //gpu version.
#ifdef _CUDA_FOUND
    globalSwapCUDARun(db, batch_size_, max_iters_, num_threads_);
    db_->updateXYGPU(db.x, db.y);
#endif
    db_->freeDetailedPlaceDB(db);

    return;
}

void GlobalSwap::run()
{
    if (!isCommonDBReady()) {
      return;
    }
    hr_clock_rep total_time_start, total_time_stop;
    total_time_start = get_globaltime(); 
    if (is_gpu_)
    {
        run_gpu();
    }
    else
    {
        run_cpu();
    }
    total_time_stop = get_globaltime();
    dreamplacePrint(kINFO, "Global swap time: %g ms\n", get_timer_period()*(total_time_stop-total_time_start));
    return;
}


DREAMPLACE_END_NAMESPACE
