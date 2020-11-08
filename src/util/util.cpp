/* @file  message.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include "util/util.h"

namespace open_edi {
namespace util {

std::string app_path;
std::string install_path;

std::string getexepath() {
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::string(result, (count > 0) ? count : 0);
}

int setAppPath(const char* in_path) {
    app_path = getexepath();

    return 0;
}

const char* getAppPath() { return app_path.c_str(); }

const char* getInstallPath() {
    size_t pos = app_path.rfind("open-edi");
    if (pos != std::string::npos) {
        install_path = app_path.substr(0, pos + 9);
    } else {
        size_t pos = app_path.rfind("openeda");
        install_path = app_path.substr(0,pos-4);
    }

    return install_path.c_str();
}

static std::string util_space_str;
const char* getSpaceStr(uint32_t num_spaces) {
    util_space_str.assign("");
    for (uint32_t i = 0; i < num_spaces; ++i) {
        util_space_str.append(" ");
    }
    return util_space_str.c_str();
}

static int MessageInit() {
    message = new Message();
    // get tool path
    std::string path;
    std::string util_msg_path;
    std::string timinglib_msg_path;

    path = getInstallPath();

    util_msg_path = path + "src/util/util.msg";
    if (access(util_msg_path.c_str(), F_OK) != 0) {
        util_msg_path = path + "/include/src/util/util.msg";
    }

    timinglib_msg_path = path + "src/db/timing/timinglib/timinglib.msg";
    if (access(timinglib_msg_path.c_str(), F_OK) != 0) {
        timinglib_msg_path = path + "/include/src/db/timing/timinglib/timinglib.msg";
    }

    message->registerMsgFile(
        util_msg_path.c_str());  // register util message file
    message->registerMsgFile(
        timinglib_msg_path.c_str());  // register timinglib message file
    return 0;
}

int utilInit() {
    MessageInit();
    return 0;
}


static int kVmrssLine = -1;


/// @brief get current process's physical memory
///
/// @param pid
///
/// @return VmRSS memory with kB
static int getMem() {
    pid_t pid = getpid();
    char file_name[64] = {0};
    FILE *fp = nullptr;
    char line_buff[256] = {0};
    snprintf(file_name, sizeof(file_name), "/proc/%d/status", pid);
    fp = fopen (file_name, "r");
    int i;
    char name[32];
    char file_unit[32];
    int vmrss;
    if (-1 == kVmrssLine) {
        kVmrssLine = 0;
        while (NULL != fgets(line_buff, sizeof(line_buff), fp)) {
            ++kVmrssLine;
            sscanf(line_buff, "%s %d %s", name, &vmrss, file_unit);
            if (0 == strncmp(name, "VmRSS", 5)) {
                break;
            }
        }
    } else {
        for (i = 0; i < kVmrssLine - 1; i++)
        {
            char* ret = fgets (line_buff, sizeof(line_buff), fp);
        }
        char* ret1 = fgets (line_buff, sizeof(line_buff), fp);
        sscanf(line_buff, "%s %d %s", name, &vmrss, file_unit);
    }
    fclose(fp);
    if (0 == strncmp(file_unit, "mB", 2)) {
        vmrss *= 1024;
    } else if (0 == strncmp(file_unit, "tB", 2)) {
        vmrss *= 1024 * 1024;
    } else if (0 == strncmp(file_unit, "B", 1)) {
        vmrss /= 1024;
    }
    return vmrss;
}

static void* processBar(void* arg) {
    clock_t start, current;
    uint32_t  duration;
    int vmrss = 0;
    start = clock();
    while (true) {
        sleep(1);
        current = clock();
        duration = (uint32_t)(current - start) / CLOCKS_PER_SEC;
        vmrss = getMem();
        message->info("Elapsed Time(s): %5d    Physical Memory(kB) %10d\r",
                duration, vmrss);
        fflush(stdout);
    }
}

int runCommandWithProcessBar(command_t command, int argc, const char **argv) {
    int result = 0;
    pthread_t process_bar_thread = 0;
    if (0 == process_bar_thread) {
        pthread_create(&process_bar_thread, NULL, processBar, NULL);
    }

    if (0 != command(argc, argv)) {
        result = 1;
    }

    if (0 != process_bar_thread) {
        pthread_cancel(process_bar_thread);
    }

    return result;
}

}  // namespace util
}  // namespace open_edi
