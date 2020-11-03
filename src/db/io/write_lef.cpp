/* @file  write_lef.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include <stdio.h>

#include "db/io/write_lef.h"
#include "db/tech/export_tech.h"

namespace open_edi {
namespace db {

int writeLef(int argc, const char **argv) {
    int num_out_file = 0;
    char *lef_file_name = nullptr;
    argc--;
    argv++;
    while (argc--) {
        if (argv[0][0] != '-') {
            if (num_out_file >= 1) {
                fprintf(stderr, "ERROR: too many output files, max = 1.\n");
                return 2;
            }
            lef_file_name = *(const_cast<char **>(argv));
            ++num_out_file;
        } else {
            fprintf(stderr, "ERROR: Illegal command line option: '%s'\n",
                    *argv);
            return 2;
        }

        argv++;
    }

    if (0 == num_out_file) {
        message->issueMsg(kError, "cannot find output DEF file name.\n");
        return 2;
    }
    message->info("\nWriting LEF\n");
    fflush(stdout);

    ExportTechLef dump(lef_file_name);
    dump.exportAll();

    message->info("\nWrite LEF successfully.\n");
    return 0;
}

}  // namespace db
}  // namespace open_edi
