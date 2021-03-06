/**
 * @file spef_tcl_command.cpp
 * @date 2020-11-02
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

#include "db/timing/spef/spef_tcl_command.h"
#include "db/core/cell.h"
#include "db/core/db.h"
#include "db/core/timing.h"
#include "db/timing/timinglib/analysis_corner.h"
#include "db/timing/timinglib/analysis_mode.h"
#include "db/timing/timinglib/analysis_view.h"
#include "db/timing/spef/spef_reader.h"

#include <utility>
#include <fstream>
#include <iostream>
#include <sstream>
#include "util/stream.h"
#include "util/util.h"

namespace open_edi {
namespace db {

void printReadSpefCommandHelp() {
    open_edi::util::message->info("read_spef:\n");
    open_edi::util::message->info("         -corner xxx\n");
    open_edi::util::message->info("         <filename list>\n");
    open_edi::util::message->info("         -help\n");
}

int parseSpefFile(const std::string &file, DesignParasitics *designParasitics) {
    
    SpefReader::SpefReader spefReader(file, designParasitics);
    if (!spefReader.parseSpefFile()) 
        return TCL_ERROR;
    return TCL_OK;
}

int readSpefCommand(ClientData cld, Tcl_Interp *itp, int argc,
                         const char *argv[]) {

    if (argc == 2 && !strcasecmp(argv[1], "-help")) {
        printReadSpefCommandHelp();
        return TCL_OK;
    }

    if (argc > 1) {
        std::vector<std::string> spefFiles;
        std::string cornerName = "";
        for (int i = 1; i < argc; ++i) {
            if (!strcmp(argv[i], "-corner")) {
                if ((i + 1) < argc) {
                    cornerName = argv[++i];
                }
            } else if (!strcmp(argv[i], "-help")) {
                continue;
            } else {
                spefFiles.emplace_back(argv[i]);
            }
        }
        if (spefFiles.empty()) {
            open_edi::util::message->issueMsg(
                open_edi::util::kError, "Please specify at least one SPEF file.");
            return TCL_ERROR;
        }
        Timing *timingdb = getTimingLib();
        if (timingdb == nullptr) {
            open_edi::util::message->issueMsg(
                open_edi::util::kError,
                "Cannot find top cell when reading SPEF file.");
            return TCL_ERROR;
        }
        AnalysisCorner *corner = nullptr;
        if (cornerName == "") {
            std::string default_name = "default";
            auto view = timingdb->getAnalysisView(default_name);
            if (view == nullptr) {
                auto mode = timingdb->createAnalysisMode(default_name);
                if (mode == nullptr) {
                    open_edi::util::message->issueMsg(
                        open_edi::util::kError, "Create default mode failed.");
                    return TCL_ERROR;
                }
                corner = timingdb->createAnalysisCorner(default_name);
                if (corner == nullptr) {
                    open_edi::util::message->issueMsg(
                        open_edi::util::kError, "Create default corner failed.");
                    return TCL_ERROR;
                }
                view = timingdb->createAnalysisView(default_name);
                if (view == nullptr) {
                    open_edi::util::message->issueMsg(
                        open_edi::util::kError, "Create default view failed.");
                    return TCL_ERROR;
                }

                view->setAnalysisMode(mode->getId());
                view->setAnalysisCorner(corner->getId());
                view->setActive(true);
                view->setSetup(true);
                view->setHold(true);
                timingdb->addActiveSetupView(view->getId());
                timingdb->addActiveHoldView(view->getId());
            } else {
                corner = view->getAnalysisCorner();
                if (corner == nullptr) {
                    corner = timingdb->createAnalysisCorner(default_name);
                    if (corner == nullptr) {
                        open_edi::util::message->issueMsg(
                            open_edi::util::kError, "Create default view failed.");
                        return TCL_ERROR;
                    }
                }
            }
        } else {
            corner = timingdb->getAnalysisCorner(cornerName);
            if (corner == nullptr) {
                open_edi::util::message->issueMsg(
                    open_edi::util::kError,
                    "The corner specified doesn't exist.");
                return TCL_ERROR;
            }
        }
        DesignParasitics *dsnParasitics = corner->getDesignParasitics();
        if (dsnParasitics == nullptr) {
	    dsnParasitics = timingdb->createObject<DesignParasitics>(kObjectTypeDesignParasitics, timingdb->getId());
            if (dsnParasitics == nullptr) {
                open_edi::util::message->issueMsg(
                    open_edi::util::kError,
                    "Create spef database failed.");
                return TCL_ERROR;
	    }
            corner->setDesignParasitics(dsnParasitics->getId());
        }
        open_edi::util::message->info("\nReading SPEF file\n");
        for (auto spefFile : spefFiles) {
            if ( parseSpefFile(spefFile, dsnParasitics) == TCL_ERROR ) {
                std::string errMsg = "Failed to parse SPEF file: " + spefFile;       
                open_edi::util::message->issueMsg(
                    open_edi::util::kError,
                    errMsg.c_str());
                return TCL_ERROR;
            }
        }
        open_edi::util::message->info("\nRead SPEF file successfully.\n");
    }  else {
        open_edi::util::message->issueMsg(
            open_edi::util::kError,
            "Please specify one SPEF file.");
        return TCL_ERROR;
    }

    return TCL_OK;
}

void printWriteSpefCommandHelp() {
    open_edi::util::message->info("write_spef:\n");
    open_edi::util::message->info("         -corner xxx\n");
    open_edi::util::message->info("         <filename>\n");
    open_edi::util::message->info("         -help\n");
}

int writeSpefCommand(ClientData cld, Tcl_Interp *itp, int argc,
                         const char *argv[]) {

    if (argc == 2 && !strcasecmp(argv[1], "-help")) {
        printWriteSpefCommandHelp();
        return TCL_OK;
    }

    if (argc > 1) {
	std::vector<std::string> outFiles;
        std::string cornerName = "";
        for (int i = 1; i < argc; ++i) {
            if (!strcmp(argv[i], "-corner")) {
                if ((i + 1) < argc) {
                    cornerName = argv[++i];
                }
            } else if (!strcmp(argv[i], "-help")) {
                continue;
            } else {
                outFiles.emplace_back(argv[i]);
            }
        }
        if (outFiles.empty()) {
            open_edi::util::message->issueMsg(
                open_edi::util::kError, "Please specify output SPEF file.");
            return TCL_ERROR;
        } else if (outFiles.size() > 1) {
	    open_edi::util::message->issueMsg(
                open_edi::util::kError, "Only one file is allowed.");
	    return TCL_ERROR;
        }
        Timing *timingdb = getTimingLib();
        if (timingdb == nullptr) {
            open_edi::util::message->issueMsg(
                open_edi::util::kError,
                "Cannot find top cell when writing SPEF file.");
            return TCL_ERROR;
        }
        AnalysisCorner *corner = nullptr;
        if (cornerName == "") {
            std::string default_name = "default";
            corner = timingdb->getAnalysisCorner(default_name);
	    if (corner == nullptr) {
                open_edi::util::message->issueMsg(
                        open_edi::util::kError, "Create default corner failed.");
                return TCL_ERROR;
            }
	} else {
            corner = timingdb->getAnalysisCorner(cornerName);
	    if (corner == nullptr) {
                open_edi::util::message->issueMsg(
                        open_edi::util::kError, "Can't find specified corner in design.");
                return TCL_ERROR;
	    }
	}
        DesignParasitics *dsgPara = corner->getDesignParasitics();
        if (dsgPara) {
	    open_edi::util::message->info("Write spef file %s...\n", outFiles[0].c_str()); 
            
	    std::ios_base::openmode mode = std::ios::out | std::ios::trunc;
            std::ofstream os(outFiles[0].c_str(), mode);
            os << *dsgPara;
            os.close();

	    open_edi::util::message->info("Write spef file %s finished\n", outFiles[0].c_str());
        }
    }

    return TCL_OK;
}

} // namespace db
} // namespace open_edi

