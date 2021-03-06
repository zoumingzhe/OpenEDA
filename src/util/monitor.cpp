/* @file  monitor.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "util/monitor.h"

namespace open_edi {
namespace util {

MonitorManager kMonitorManager;

Monitor::Monitor() {
    if ( gettimeofday(&start_time_, NULL) < 0 ) {
        message->issueMsg(kError, "Monitor: gettimeofday return code %d\n",
                                                                        errno);
    }
    state_ = kMonitorRunning;
}

void Monitor::recordCurrentInfo() {
    current_info_.recordElapsedTime(start_time_);
}

void MonitorInformation::recordElapsedTime(struct timeval start_time) {
        struct timeval current_time;
        if ( gettimeofday(&current_time, NULL) < 0 ) {
            message->issueMsg(kError, "recordElapsedTime: gettimeofday"
                                      "return code %d\n", errno);
        }
        elapsed_time_ = (current_time.tv_sec - start_time.tv_sec) +
                    static_cast<double>(current_time.tv_usec -
                                               start_time.tv_usec) / 1000000.0;
    }


MonitorInformation & Monitor::getCurrentInfo() {
    if (state_ == kMonitorRunning) {
        recordCurrentInfo();
    }
    return current_info_;
}

void Monitor::pause() {
    state_ = kMonitorPaused;
    recordCurrentInfo();
}

void Monitor::reset() {
    if ( gettimeofday(&start_time_, NULL) < 0 ) {
        message->issueMsg(kError, "Monitor: gettimeofday return code %d\n",
                                                                        errno);
    }
    state_ = kMonitorRunning;
}

MonitorManager::MonitorManager() {
    id_ = 0;
}

MonitorId MonitorManager::createMonitor() {
    Monitor* monitor = new Monitor;
    monitor_map_[id_] = monitor;

    return id_++;
}

bool MonitorManager::pauseMonitor(MonitorId monitor_id) {
    auto it = monitor_map_.find(monitor_id);
    if (it != monitor_map_.end()) {
        Monitor *monitor = it->second;
        monitor->pause();
        monitor->recordCurrentInfo();

        return true;
    }

    return false;
}

bool MonitorManager::outputMonitor(MonitorId monitor_id,
                                   ResourceTypes resource_types,
                                   const char* prefix,
                                   const char* suffix) {
    auto it = monitor_map_.find(monitor_id);
    if (it != monitor_map_.end()) {
        Monitor *monitor = it->second;
        MonitorInformation current_info = monitor->getCurrentInfo();

        message->info("%s Elapsed Time(s): %.6f %s",
                prefix, current_info.getElapsedTime(), suffix);
        fflush(stdout);
        return true;
    }
    message->issueMsg(kError, "ouputMonitor: cannot find monitor by id %d\n",
                                               monitor_id);
    return false;
}
bool MonitorManager::resetMonitor(MonitorId monitor_id) {
    auto it = monitor_map_.find(monitor_id);
    if (it != monitor_map_.end()) {
        it->second->reset();
        return true;
    }
    return false;
}

bool MonitorManager::destroyMonitor(MonitorId monitor_id) {
    auto it = monitor_map_.find(monitor_id);
    if (it != monitor_map_.end()) {
        monitor_map_.erase(it);
        return true;
    }

    return false;
}

}  // namespace util
}  // namespace open_edi
