/* @file  util.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_UTIL_UTIL_H_
#define EDI_UTIL_UTIL_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <boost/filesystem.hpp>

#include "util/namespace.h"
#include "util/data_traits.h"
#include "util/enums.h"
#include "util/message.h"
#include "util/point.h"
#include "util/map_reduce.h"
#include "util/polygon_table.h"
#include "util/stream.h"
#include "util/util_mem.h"
#include "util/version.h"
#include "util/monitor.h"

namespace open_edi {
namespace util {

int setAppPath(const char* path);
const char* getAppPath();
const char* getInstallPath();
int utilInit();

const char* getSpaceStr(uint32_t num_spaces);

typedef int (*command_t)(int, const char **);
void *processBar(void *arg);
int runCommandWithProcessBar(command_t command, int argc, const char **argv);
uint32_t calcThreadNumber(uint64_t num_tasks);
}  // namespace util
}  // namespace open_edi

#endif
