/**
 * @file  map_reduce.h
 * @date  11/11/2020 01:28:36 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_UTIL_MAP_REDUCE_H_
#define EDI_UTIL_MAP_REDUCE_H_

#include <assert.h>
#include <pthread.h>
#include <thread>
#include <semaphore.h>

#include <string>
#include <vector>

//#include "utility/assert.h"

namespace open_edi {
namespace util {

class MTApp {
  public:
    static const int kMaxDemand = 128;

    MTApp() : demand_cnt_(0) {}
    virtual ~MTApp() {}

    virtual void runDemand() {}
    void joinDemands();

    int demand_cnt_;
    pthread_t demands_[kMaxDemand];
};

struct MTAppInput {};
struct MTTask {};

class MTQueue {
  public:
    MTQueue(int max_size);
    ~MTQueue();

    int size() { return size_; }
    bool full() const { return size_ == max_size_; }
    bool empty() const { return size_ == 0; }
    void begin();
    void end();
    void set_ended(bool f) { ended_ = f; }

    MTTask* front();
    MTTask* back();

    void push(MTTask* task);
    void push(std::vector<MTTask*> tasks);
    MTTask* pop();

    void reset();
    int empty_count() { return empty_count_; }

  private:
    MTTask** buffer_;
    int max_size_;
    int size_;
    int front_;
    int back_;
    pthread_mutex_t mutex_;
    pthread_cond_t cv_;
    bool ended_;
    int empty_count_;
};

class MTMRApp : public MTApp {
  public:
    static const int kMaxMapper = 1;
    static const int kMaxWorker = 256;
    static const int kMaxReducer = 64;
    static const int kMaxQueue = 1500000;

    MTMRApp();
    ~MTMRApp();

    MTAppInput* input() { return input_; }
    void set_input(MTAppInput* input) { input_ = input; }

    static void* mapperFunc(void* arg);

    static void* workerFunc(void* arg);
    static void* reducerFunc(void* arg);
    virtual void* runMapper() = 0;
    virtual void* runWorker() = 0;
    virtual void* runReducer() = 0;

    void joinMappers();
    void joinWorkers();
    void joinReducer();
    int reducer_cnt() { return reducer_cnt_; }
    void set_reducer_cnt(int r) { reducer_cnt_ = r; }

    virtual void preRun() = 0;
    void run(int mapper_cnt, int worker_cnt, int reducer_cnt = 1);
    virtual void postRun() = 0;

    double total_elapsed_time() const { return total_elapsed_time_; }
    double total_cpu_time() const { return total_cpu_time_; }

    friend class DPServer;

  protected:
    MTAppInput* input_;
    MTQueue task_queue_;                        // Mapper/Worker Queue
    MTQueue solution_queue_;                    // Worker/Reducer Queue
    std::vector<MTQueue*> solution_queue_vec_;  // Worker/Reducer Queue vec
    int mapper_cnt_;
    int worker_cnt_;
    int reducer_cnt_;
    pthread_t mappers_[kMaxMapper];
    pthread_t workers_[kMaxWorker];
    pthread_t reducer_[kMaxReducer];
    pthread_mutex_t mutex_;
    pthread_cond_t cv_;
    double total_elapsed_time_;
    double total_cpu_time_;
};
}  // namespace util
}  // namespace open_edi
#endif  // EDI_UTIL_MAP_REDUCE_H_
