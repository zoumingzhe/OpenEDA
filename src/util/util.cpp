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
        std::string from_open = app_path.substr(pos);
        size_t slash_pos = from_open.find("/");
        install_path = app_path.substr(0, pos + slash_pos + 1);
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
    std::string infra_msg_path;

    path = getInstallPath();

    util_msg_path = path + "src/util/util.msg";
    if (access(util_msg_path.c_str(), F_OK) != 0) {
        util_msg_path = path + "/include/src/util/util.msg";
    }

    timinglib_msg_path = path + "src/db/timing/timinglib/timinglib.msg";
    if (access(timinglib_msg_path.c_str(), F_OK) != 0) {
        timinglib_msg_path = path + "/include/src/db/timing/timinglib/timinglib.msg";
    }

    infra_msg_path = path + "src/infra/infra.msg";

    message->registerMsgFile(
        util_msg_path.c_str());  // register util message file
    message->registerMsgFile(
        timinglib_msg_path.c_str());  // register timinglib message file
    message->registerMsgFile(
        infra_msg_path.c_str());  // register timinglib message file
    // register db message file
    {
        std::string msg_path = path + "src/db/db.msg";
        if (access(msg_path.c_str(), F_OK) != 0) {
            msg_path = path + "/include/src/db/db.msg";
        }
        message->registerMsgFile(
            msg_path.c_str());
    }
    return 0;
}

int utilInit() {
    MessageInit();
    return 0;
}

void* processBar(void* arg) {
    clock_t start, current;
    uint32_t  duration;
    start = clock();
    while (true) {
        sleep(1);
        current = clock();
        duration = (uint32_t)(current - start) / CLOCKS_PER_SEC;
        message->info("Elapsed Time(s): %d                       \r", duration);
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
