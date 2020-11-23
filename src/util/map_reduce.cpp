/**
 * @file  map_reduce.cpp
 * @date  11/11/2020 01:38:36 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "util/map_reduce.h"

namespace open_edi {
namespace util {

MTQueue::MTQueue(int max_size)
    : max_size_(max_size), size_(0), front_(0), back_(0), ended_(false) {
    pthread_mutex_init(&mutex_, NULL);
    pthread_cond_init(&cv_, NULL);
    buffer_ = new MTTask*[max_size];
    for (int i = 0; i < max_size_; i++) {
        buffer_[i] = NULL;
    }
    empty_count_ = 0;
}

MTQueue::~MTQueue() {
    pthread_mutex_lock(&mutex_);
    delete[] buffer_;
    pthread_mutex_unlock(&mutex_);
}

MTTask* MTQueue::front() {
    pthread_mutex_lock(&mutex_);
    MTTask* task = buffer_[front_];
    pthread_mutex_unlock(&mutex_);
    return task;
}

MTTask* MTQueue::back() {
    pthread_mutex_lock(&mutex_);
    MTTask* task = buffer_[back_];
    pthread_mutex_unlock(&mutex_);
    return task;
}

void MTQueue::push(MTTask* task) {
    pthread_mutex_lock(&mutex_);
    while (full()) {
        pthread_cond_wait(&cv_, &mutex_);
    }
    buffer_[back_++] = task;
    size_++;
    back_ %= max_size_;
    if (size_ == 1) {
        pthread_cond_broadcast(&cv_);
        // pthread_cond_signal(&cv_);
    }
    pthread_mutex_unlock(&mutex_);
}

void MTQueue::push(std::vector<MTTask*> tasks) {
    pthread_mutex_lock(&mutex_);

    for (int i = 0; i < static_cast<int>(tasks.size()); ++i) {
        while (full()) {
            pthread_cond_wait(&cv_, &mutex_);
        }

        buffer_[back_++] = tasks[i];
        size_++;
        back_ %= max_size_;
        if (size_ == 1) {
            pthread_cond_broadcast(&cv_);
        }
    }

    pthread_mutex_unlock(&mutex_);
}

MTTask* MTQueue::pop() {
    pthread_mutex_lock(&mutex_);
    while (empty() && !ended_) {
        empty_count_++;
        pthread_cond_wait(&cv_, &mutex_);
    }
    if (empty()) {
        if (!ended_) assert(0);
        pthread_mutex_unlock(&mutex_);
        return NULL;
    }
    MTTask* task = buffer_[front_++];
    front_ %= max_size_;
    size_--;
    if (size_ == max_size_ - 1) {
        pthread_cond_broadcast(&cv_);
    }
    pthread_mutex_unlock(&mutex_);
    return task;
}

void MTQueue::begin() { ended_ = false; }

void MTQueue::end() {
    pthread_mutex_lock(&mutex_);
    ended_ = true;
    pthread_mutex_unlock(&mutex_);
    pthread_cond_broadcast(&cv_);
}

void MTQueue::reset() {
    size_ = 0;
    front_ = 0;
    back_ = 0;
    ended_ = false;
    pthread_mutex_init(&mutex_, NULL);
    pthread_cond_init(&cv_, NULL);
    for (int i = 0; i < max_size_; i++) {
        buffer_[i] = NULL;
    }
    empty_count_ = 0;
}

void MTMRApp::run(int mapper_cnt, int worker_cnt, int reducer_cnt) {
    if (mapper_cnt < 0) mapper_cnt = 1;
    if (mapper_cnt > kMaxMapper) mapper_cnt = kMaxMapper;
    mapper_cnt_ = mapper_cnt;
    if (worker_cnt < 0) worker_cnt = 1;
    if (worker_cnt > kMaxWorker) worker_cnt = kMaxWorker;
    worker_cnt_ = worker_cnt;
    if (reducer_cnt < 0) reducer_cnt = 1;
    if (reducer_cnt > kMaxReducer) reducer_cnt = kMaxReducer;
    reducer_cnt_ = reducer_cnt;

    task_queue_.reset();
    solution_queue_.reset();

    preRun();

    for (int i = 0; i < mapper_cnt; i++) {
        pthread_create(&mappers_[i], NULL, mapperFunc, this);
    }
    // joinMappers();

    pthread_attr_t attr;
    struct sched_param param;
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    // rs = pthread_attr_getschedparam(&attr, &param);
    param.__sched_priority = 10;
    pthread_attr_setschedparam(&attr, &param);
    // rs = pthread_attr_getschedparam(&attr, &param);
    // nimbus_info("priority = %d\n", param.__sched_priority);

    for (int i = 0; i < worker_cnt; i++) {
        pthread_create(&workers_[i], NULL, workerFunc, this);
    }

    pthread_attr_destroy(&attr);

    for (int i = 0; i < reducer_cnt; ++i) {
        pthread_create(&reducer_[i], NULL, reducerFunc, this);
    }

    joinMappers();

    task_queue_.end();

    joinWorkers();

    solution_queue_.end();
    for (int i = 0; i < reducer_cnt; ++i) {
        solution_queue_vec_[i]->end();
    }

    joinReducer();

    // fixed bug by sifei
    task_queue_.set_ended(false);
    solution_queue_.set_ended(false);
    for (int i = 0; i < reducer_cnt; ++i) {
        solution_queue_vec_[i]->set_ended(false);
    }

    postRun();
}

void MTApp::joinDemands() {
    for (int i = 0; i < demand_cnt_; i++) {
        pthread_join(demands_[i], NULL);
    }
    demand_cnt_ = 0;
}

MTMRApp::MTMRApp()
    : MTApp(),
      input_(NULL),
      task_queue_(kMaxQueue),
      solution_queue_(kMaxQueue),
      mapper_cnt_(0),
      worker_cnt_(0),
      reducer_cnt_(0),
      total_elapsed_time_(0.0),
      total_cpu_time_(0.0) {
    pthread_mutex_init(&mutex_, NULL);
    solution_queue_vec_.resize(kMaxReducer);
    for (int i = 0; i < kMaxReducer; ++i) {
        solution_queue_vec_[i] = new MTQueue(kMaxQueue >> 3);
    }
}

MTMRApp::~MTMRApp() {
    for (int i = 0; i < kMaxReducer; ++i) {
        if (solution_queue_vec_[i]) {
            delete solution_queue_vec_[i];
        }
    }
}

void* MTMRApp::mapperFunc(void* arg) {
    MTMRApp* app = reinterpret_cast<MTMRApp*>(arg);
    return app->runMapper();
}

void* MTMRApp::workerFunc(void* arg) {
    MTMRApp* app = reinterpret_cast<MTMRApp*>(arg);
    return app->runWorker();
}

void* MTMRApp::reducerFunc(void* arg) {
    MTMRApp* app = reinterpret_cast<MTMRApp*>(arg);
    return app->runReducer();
}

void MTMRApp::joinMappers() {
    for (int i = 0; i < mapper_cnt_; i++) {
        pthread_join(mappers_[i], NULL);
    }
    mapper_cnt_ = 0;
}

void MTMRApp::joinWorkers() {
    for (int i = 0; i < worker_cnt_; i++) {
        pthread_join(workers_[i], NULL);
    }
    worker_cnt_ = 0;
}

void MTMRApp::joinReducer() {
    for (int i = 0; i < reducer_cnt_; ++i) {
        pthread_join(reducer_[i], NULL);
    }
    // reducer_cnt_ = 0;
}

}  // namespace util
}  // namespace open_edi