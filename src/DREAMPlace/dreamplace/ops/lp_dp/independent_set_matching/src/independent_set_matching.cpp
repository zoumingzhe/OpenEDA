/**
 * @file   independent_set_matching.cpp
 * @author Chenzhong Luo
 * @date   Sep 2020
 */

#include <vector>
#include <limits>
#include "utility/src/Msg.h"
#include "lp_dp/independent_set_matching/src/independent_set_matching.h"
#include "utility/src/DetailedPlaceDB.h"
//#include "utility/src/utils.cuh"

DREAMPLACE_BEGIN_NAMESPACE


template <typename T>
void independentSetMatchingCPULauncher(DetailedPlaceDB<T> db, 
        int batch_size, int set_size, int max_iters, int num_threads);
//sequential
template <typename T>
void independentSetMatchingCPULauncher(DetailedPlaceDB<T> db, 
        int set_size, int max_iters);
template <typename T>
int independentSetMatchingCUDALauncher(DetailedPlaceDB<T> db, int batch_size, int set_size, int max_iters, int num_threads);

void IndependentSetMatching::run_cpu()
{
    //make DetailedPlaceDB db.
    DetailedPlaceDB<int> db;
    db_->initDetailedPlaceDB(db, false); //cpu version.
    if (alg_concurrent_)  //concurrent
    {
        independentSetMatchingCPULauncher(db, batch_size_, set_size_, max_iters_, num_threads_);
    }
    else
    {
        independentSetMatchingCPULauncher(db, set_size_, max_iters_);
    }
    //update common db_ using db.x, db.y
    //todo: db_->update(db.x, db.y);
    db_->freeDetailedPlaceDB(db, false);

    return;
}

void IndependentSetMatching::run_gpu()
{
    //make DetailedPlaceDB db.
    DetailedPlaceDB<int> db;
    db_->initDetailedPlaceDB(db, true); //gpu version.
#ifdef _CUDA_FOUND
    independentSetMatchingCUDALauncher(db, batch_size_, set_size_, max_iters_, num_threads_);
#endif
    //update common db_ using db.x, db.y
    /*
    int* cpu_dbx; int* cpu_dby;
    allocateCopyCPU(cpu_dbx, db.x, db.num_nodes, int);
    allocateCopyCPU(cpu_dby, db.y, db.num_nodes, int);
    */
    //todo: db_->update(db.x, db.y, true);
    /*
    destroyCPU(cpu_dbx);
    destroyCPU(cpu_dby);
    */
    db_->freeDetailedPlaceDB(db, true);

    return;
}

void IndependentSetMatching::run()
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
    dreamplacePrint(kINFO, "Independent set matcing time: %g ms\n", get_timer_period()*(total_time_stop-total_time_start));
    return;
}


DREAMPLACE_END_NAMESPACE
