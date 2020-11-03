/**
 * @file timinglib_libsyn.cpp
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
#include <dirent.h>
#include <string.h>

#include <cassert>
#include <fstream>
#define NAMLEN(dirent) strlen((dirent)->d_name)

#include "db/timing/timinglib/group_enum.h"
#include "db/timing/timinglib/timinglib_libanalysis.h"
#include "db/timing/timinglib/timinglib_libbuilder.h"
#include "db/timing/timinglib/timinglib_libsyn.h"
#include "timinglib_libparser.tab.hh"
#include "timinglib_syntaxparser.tab.hh"

namespace Timinglib {

LibSyn::LibSyn(LibBuilder *lb) : libbuilder_(lb) {
    if (libbuilder_) libbuilder_->set_lib_syn(this);
}

LibSyn::~LibSyn() {
    scanner_ = nullptr;
    delete analysis_;
    analysis_ = nullptr;
}

bool LibSyn::parseLibertyFile(const char *const filename,
                              std::string *parseLogStr /*=nullptr*/) {
    assert(filename != nullptr);

    if (analysis_ == nullptr) analysis_ = new LibAnalysis(libbuilder_);

    analysis_->setLibertyFileName(filename);
    analysis_->setLibertyParseLogStr(parseLogStr);

    si2drErrorT err;
    analysis_->si2drPIInit(&err);
    analysis_->si2drPISetNocheckMode(&err);

    si2drMessageHandlerT msg = analysis_->si2drPIGetMessageHandler(&err);
    std::string str = string_format("Reading %s...", filename);
    (analysis_->*msg)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR,
                      si2drStringT(str.c_str()), &err);

    FILE *fp = fp = fopen(filename, "r");
    if (fp == nullptr) {
        str = string_format("Could not open %s for parsing.", filename);
        (analysis_->*msg)(kSI2DR_SEVERITY_ERR, kSI2DR_NO_ERROR,
                          si2drStringT(str.c_str()), &err);
        return false;
    }

    __lib_scan_begin(fp);
    const int accept(0);
    if (__lib_parse() != accept) {
        if (msg != nullptr) {
            str = string_format("Parsing %s failed.", filename);
            (analysis_->*msg)(kSI2DR_SEVERITY_ERR, kSI2DR_NO_ERROR,
                              si2drStringT(str.c_str()), &err);
        }
        __lib_scan_end(fp);
        fclose(fp);
        return false;
    }

    if (msg != nullptr) {
        str = string_format("Parsing %s finished.", filename);
        (analysis_->*msg)(kSI2DR_SEVERITY_NOTE, kSI2DR_NO_ERROR,
                          si2drStringT(str.c_str()), &err);
    }
    __lib_scan_end(fp);
    fclose(fp);

    return true;
}

bool LibSyn::isLibertySyntaxValid(void) {
    if (analysis_ != nullptr) return analysis_->isLibertySyntaxValid();

    return true;
}

void LibSyn::dumpLibFile(const char *const filename,
                         bool clearFileContent /*=true*/) {
    if (analysis_ == nullptr) return;
    analysis_->dumpLibFile(filename, clearFileContent);
}

LibAnalysis *LibSyn::getAnalysis(void) { return analysis_; }

bool LibSyn::parse_desc_file() {
    if (analysis_ == nullptr) analysis_ = new LibAnalysis(libbuilder_);

    scandata *sd = analysis_->getScandata();

    DIR *dirp;
    struct dirent *dp;
    sd->libsynt_allgroups =
        LibHash::timinglib_hash_create_hash_table(503, 1, 0);
    sd->libsynt_attrs = LibHash::timinglib_hash_create_hash_table(503, 1, 0);
    dirp = opendir("./desc");
    while ((dp = readdir(dirp)) != NULL) {
        if (strncmp(dp->d_name, "syntax.", 7) == 0 && strlen(dp->d_name) > 8) {
            char fname[1000];
            /* foreach file in the desc dir */
            /* form the master hash tables */
            sd->libsynt_techs[sd->libsynt_tech_count].group_hash =
                LibHash::timinglib_hash_create_hash_table(503, 1, 0);
            sd->libsynt_groups =
                sd->libsynt_techs[sd->libsynt_tech_count].group_hash;
            sd->libsynt_techs[sd->libsynt_tech_count].name =
                static_cast<char *>(malloc(NAMLEN(dp) + 1));
            snprintf(sd->libsynt_techs[sd->libsynt_tech_count].name,
                     NAMLEN(dp) + 1, "%s", dp->d_name + 7);
            char *p =
                strchr(sd->libsynt_techs[sd->libsynt_tech_count].name, '.');
            if (*p) *p = 0;
            /* parse the file right here  -- the tech count gets bumped when
                            the file is parsed */
            snprintf(fname, sizeof(fname), "desc/%s", dp->d_name);
            if (strstr(dp->d_name, "desc~") != NULL) continue;
            printf("Processing %s\n", fname);
            FILE *fp = fopen(fname, "r");
            if (fp == nullptr) {
                closedir(dirp);
                return false;
            }
            __desc_scan_begin(fp);
            const int accept(0);
            if (__desc_parse() != accept) {
                printf("Parse failed!!\n");
                __desc_scan_end(fp);
                fclose(fp);
                closedir(dirp);
                return false;
            }
            __desc_scan_end(fp);
            fclose(fp);
        }
    }
    closedir(dirp);
    sd->libsynt_techs[sd->libsynt_tech_count].name = 0;
    return true;
}

void LibSyn::generateAttrEnumHeader() {
    if (analysis_ != nullptr) analysis_->generateAttrEnumHeader();
}

void LibSyn::generateGroupEnumHeader() {
    if (analysis_ != nullptr) analysis_->generateGroupEnumHeader();
}

void LibSyn::generateSyntaxDeclsHAndCPP() {
    if (analysis_ != nullptr) analysis_->generateSyntaxDeclsHAndCPP();
}
void LibSyn::generateAttrAndGroupLookup() {
    if (analysis_ != nullptr) analysis_->generateAttrAndGroupLookup();
}
void *LibSyn::getScanner(void) { return scanner_; }
}  // namespace Timinglib
