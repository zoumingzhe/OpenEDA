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

/// @brief global swap algorithm for detailed placement 
template <typename T>
int globalSwapCUDALauncher(DetailedPlaceDB<T> db, int batch_size, int max_iters, int num_threads);
// concurrent
template <typename T>
int globalSwapCPULauncher(DetailedPlaceDB<T> db, int batch_size, int max_iters, int num_threads);
template <typename T>
int globalSwapCPULauncher(DetailedPlaceDB<T> db, int max_iters);

void GlobalSwap::run_cpu()
{
    //make DetailedPlaceDB db.
    DetailedPlaceDB<int> db;
    db_->initDetailedPlaceDB(db, false); //cpu version.
    if (alg_concurrent_)
    {
        //globalSwapCPULauncher(db, batch_size_, max_iters_, num_threads_);
    }
    else
    {
        //globalSwapCPULauncher(db, max_iters_);
    }
    //update common db_ using db.x, db.y
    //todo: db_->update(db.x, db.y);
    db_->freeDetailedPlaceDB(db, false);

    return;
}

void GlobalSwap::run_gpu()
{
    //make DetailedPlaceDB db.
    DetailedPlaceDB<int> db;
    db_->initDetailedPlaceDB(db, true); //gpu version.
#ifdef _CUDA_FOUND
    //globalSwapCUDALauncher(db, batch_size_, max_iters_, num_threads_);
#endif
    //update common db_ using db.x, db.y
    /*
    int* cpu_dbx; int* cpu_dby;
    allocateCopyCPU(cpu_dbx, db.x, db.num_nodes, int);
    allocateCopyCPU(cpu_dby, db.y, db.num_nodes, int);
    */
    //todo: db_->update(db.x, db.y);
    /*
    destroyCPU(cpu_dbx);
    destroyCPU(cpu_dby);
    */
    db_->freeDetailedPlaceDB(db, true);

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
