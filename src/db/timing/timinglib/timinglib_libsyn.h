/**
 * @file timinglib_libsyn.h
 * @date 2020-09-01
 * @brief
 *
 * Copyright (C) 2020 NIIC EDA
 *
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 *
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBSYN_H_
#define SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBSYN_H_

#include <cstddef>
#include <istream>
#include <string>

#include "db/timing/timinglib/timinglib_libstrtab.h"
#include "db/timing/timinglib/timinglib_si2dr.h"
#include "db/timing/timinglib/timinglib_structs.h"

namespace Timinglib {
class LibBuilder;
class LibAnalysis;

class LibSyn {
  public:
    explicit LibSyn(LibBuilder *lb);

    virtual ~LibSyn();

    bool parseLibertyFile(const char *const filename,
                          std::string *parseLogStr = nullptr);

    bool isLibertySyntaxValid(void);

    bool dumpLibFile(const char *const filename, bool clearFileContent = true);

    LibAnalysis *getAnalysis(void);

    bool parse_desc_file();
    void generateAttrEnumHeader();
    void generateGroupEnumHeader();
    void generateSyntaxDeclsHAndCPP();
    void generateAttrAndGroupLookup();

    void *getScanner(void);

  private:
    void __lib_scan_begin(FILE *fp);
    void __lib_scan_end(FILE *fp);
    int __lib_parse();
    void __desc_scan_begin(FILE *fp);
    void __desc_scan_end(FILE *fp);
    int __desc_parse();

    LibAnalysis *analysis_ = nullptr;
    LibBuilder *libbuilder_ = nullptr;

    void *scanner_ = nullptr;
};

} /* end namespace Timinglib */
#endif /* SRC_DB_TIMING_TIMINGLIB_TIMINGLIB_LIBSYN_H_*/
