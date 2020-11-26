/* @file  monitor.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_UTIL_MONITOR_H_
#define SRC_UTIL_MONITOR_H_

#include <stdio.h>
#include <sys/time.h>
#include <unordered_map>

#include "util/message.h"

namespace open_edi {
namespace util {

using MonitorId = uint32_t;
using ResourceTypes = uint32_t;

const uint32_t kMaxNumMonitorId = UINT_MAX - 1;
const uint32_t kInvalidMonitorId = 0;

enum ResourceType : uint32_t {
    kElapsedTime    = 0x00000001,  // current time - start time
    kCpuTime        = 0x00000002,  //
    kVmPeak         = 0x00000004,  // VmPeak in /proc/<pid>/status
    kVmRss          = 0x00000008,  // VmRSS  in /proc/<pid>/status
    kUnkown
};

class MonitorInformation {
  public:
    MonitorInformation() {}
    ~MonitorInformation() {}

    void recordElapsedTime(struct timeval start_time);

    double getElapsedTime() { return elapsed_time_; }

    void setCpuTime(double cpu_time) { cpu_time_ = cpu_time; }
    double getCpuTime() { return cpu_time_; }

    void setVmPeak(uint64_t vmPeak) { vmPeak_ = vmPeak; }
    uint64_t getVmPeak() { return vmPeak_; }

    void setVmRss(uint64_t vmRss) { vmRss_ = vmRss; }
    uint64_t getVmRss() { return vmRss_; }

  private:
    double elapsed_time_;  // μs, duration time
    double cpu_time_;      // μs, real cpu time
    uint64_t vmPeak_;  // kB, Peak virtual memory size.
    uint64_t vmSize_;  // kB, Virtual memory size.
    uint64_t vmHWM_;   // kB, Peak resident set size ("high water mark").
    uint64_t vmRss_;   // kB, Resident set size.
};

class Monitor {
  public:
     Monitor();

     struct timeval getStartTime() {return start_time_;}
     MonitorInformation getStartInfo() {return start_info_;}
     void recordCurrentInfo();
     MonitorInformation & getCurrentInfo();
     void pause();
     void reset();

     enum MonitorState {
         kMonitorRunning,
         kMonitorPaused,
         kUnknown
     };

  private:
     MonitorState   state_;
     struct timeval start_time_;
     MonitorInformation start_info_;
     MonitorInformation current_info_;
};

class MonitorManager {
  public:
     MonitorManager();
     ~MonitorManager();
     MonitorId createMonitor();
     Monitor* queryMonitor(MonitorId monitor_id) {
         return monitor_map_[monitor_id];
     }
     bool outputMonitor(MonitorId monitor_id, ResourceTypes resource_types,
             const char* description, bool has_return = true);
     bool outputMonitor(MonitorId monitor_id, ResourceTypes resource_types,
             FILE *fp, const char* description, bool has_return = true)
         { return true; }
     bool outputMonitor(MonitorId monitor_id, ResourceTypes resource_types,
             std::ofstream *fp, const char* description, bool has_return = true)
         { return true; }
     bool pauseMonitor(MonitorId monitor_id);
     bool resumeMonitor(MonitorId monitor_id) {return true;};
     bool resetMonitor(MonitorId monitor_id);
     bool destroyMonitor(MonitorId monitor_id);

  private:
     MonitorId id_;
     MonitorId unused_num_id_;
     std::unordered_map<MonitorId, Monitor*> monitor_map_;
};


extern MonitorId createMonitor();
extern Monitor* queryMonitor(MonitorId monitor_id);
extern bool outputMonitor(MonitorId monitor_id, ResourceTypes resource_types,
        const char* description, bool has_return = true);
extern bool outputMonitor(MonitorId monitor_id, ResourceTypes resource_types,
        FILE *fp, const char* description, bool has_return = true);
extern bool outputMonitor(MonitorId monitor_id, ResourceTypes resource_types,
        std::ofstream *ofs, const char* description, bool has_return = true);
extern bool pauseMonitor(MonitorId monitor_id);
extern bool resumeMonitor(MonitorId monitor_id);
extern bool resetMonitor(MonitorId monitor_id);
extern bool destroyMonitor(MonitorId monitor_id);

}  // namespace util
}  // namespace open_edi

#endif  // SRC_UTIL_MONITOR_H_
