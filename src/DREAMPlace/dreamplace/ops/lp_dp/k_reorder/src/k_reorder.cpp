/**
 * @file   k_reorder.cpp
 * @author Chenzhong Luo
 * @date   Sep 2020
 */

#include <vector>
#include <limits>
#include "utility/src/Msg.h"
#include "lp_dp/k_reorder/src/k_reorder.h"
#include "utility/src/DetailedPlaceDB.h"
//#include "utility/src/utils.cuh"

DREAMPLACE_BEGIN_NAMESPACE

/// @brief k-reorder algorithm for detailed placement 
template <typename T>
int kreorderCUDALauncher(DetailedPlaceDB<T> db, int K, int max_iters, int num_threads);
template <typename T>
int kreorderCPULauncher(DetailedPlaceDB<T>& db, int K, int max_iters, int num_threads);

void KReorder::run_cpu()
{
    //make DetailedPlaceDB db.
    DetailedPlaceDB<int> db;
    db_->initDetailedPlaceDB(db, false); //cpu version.
    kreorderCPULauncher(db, K_, max_iters_, num_threads_);
    //update common db_ using db.x, db.y
    //todo: db_->update(db.x, db.y);
    db_->freeDetailedPlaceDB(db, false);

    return;
}

void KReorder::run_gpu()
{
    //make DetailedPlaceDB db.
    DetailedPlaceDB<int> db;
    db_->initDetailedPlaceDB(db, true); //gpu version.
#ifdef _CUDA_FOUND
    kreorderCUDALauncher(db, K_, max_iters_, num_threads_);
#endif
    //update common db_ using db.x, db.y
    /*
    int* cpu_dbx; int* cpu_dby;
    allocateCopyCPU(cpu_dbx, db.x, db.num_nodes, int);
    allocateCopyCPU(cpu_dby, db.y, db.num_nodes, int);
    */
    //todo: db_->updateGPU(db.x, db.y);
    /*
    destroyCPU(cpu_dbx);
    destroyCPU(cpu_dby);
    */
    db_->freeDetailedPlaceDB(db, true);

    return;
}

void KReorder::run()
{
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
    dreamplacePrint(kINFO, "K-reorder time: %g ms\n", get_timer_period()*(total_time_stop-total_time_start));
    return;
}


DREAMPLACE_END_NAMESPACE
