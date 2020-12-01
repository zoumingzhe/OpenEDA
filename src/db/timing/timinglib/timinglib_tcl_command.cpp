
/**
 * @file timinglib_tcl_command.cpp
 * @date 2020-09-14
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
#include "db/timing/timinglib/timinglib_tcl_command.h"

#include <unistd.h>

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "db/core/cell.h"
#include "db/core/db.h"
#include "db/timing/timinglib/analysis_corner.h"
#include "db/timing/timinglib/analysis_mode.h"
#include "db/timing/timinglib/analysis_view.h"
#include "db/timing/timinglib/libset.h"
#include "db/timing/timinglib/timinglib_cell.h"
#include "db/timing/timinglib/timinglib_lib.h"
#include "db/timing/timinglib/timinglib_libbuilder.h"
#include "db/timing/timinglib/timinglib_libsyn.h"
#include "db/timing/timinglib/timinglib_pgterm.h"
#include "db/timing/timinglib/timinglib_term.h"
#include "util/stream.h"
#include "util/util.h"

namespace open_edi {
namespace db {

bool buildTermMapping(Cell *cell, const std::vector<TCell *> &tcells) {
    Cell *topCell = getTopCell();
    if (topCell == nullptr) return false;

    uint64_t size = cell->getNumOfTerms();
    std::unordered_map<std::string, Term *> term_map;
    std::list<std::string> term_names;
    std::string str = "";
    for (uint64_t index = 0; index < size; ++index) {
        auto p = cell->getTerm(index);
        if (p != nullptr) {
            str = p->getName();
            term_names.emplace_back(str);
            term_map[str] = p;
        }
    }

    std::unordered_map<TCell *, std::list<std::string> > cell_tterm_names_map;
    std::unordered_map<TCell *, std::unordered_map<std::string, TTerm *> >
        cell_tterm_map;
    for (auto tcell : tcells) {
        auto tterms = tcell->getTerms();
        std::list<std::string> &tterm_names = cell_tterm_names_map[tcell];
        std::unordered_map<std::string, TTerm *> &tterm_map =
            cell_tterm_map[tcell];
        for (auto tterm : tterms) {
            if (tterm != nullptr) {
                str = tterm->getName();
                tterm_names.emplace_back(str);
                tterm_map[str] = tterm;
            }
        }
        for (auto &name : tterm_names) {
            if (term_map.find(name) == term_map.end()) {
                auto term = topCell->createObject<Term>(kObjectTypeTerm);
                if (term) {
                    cell->addTerm(term->getId());
                    term_names.emplace_back(name);
                    term_map[name] = term;
                } else {
                    open_edi::util::message->issueMsg("TIMINGLIB", 1, kError,
                                                      "term", str.c_str());
                    return false;
                }
            }
        }
    }

    std::unordered_map<TCell *, std::vector<TTerm *> > tcell_reset_terms_map;

    for (auto iter : cell_tterm_names_map) {
        TCell *cell = iter.first;
        std::list<std::string> &tterm_names = cell_tterm_names_map[cell];
        std::unordered_map<std::string, TTerm *> &tterm_map =
            cell_tterm_map[cell];
        for (auto &name : term_names) {
            if (tterm_map.find(name) != tterm_map.end()) {
                TTerm *term = tterm_map[name];
                if (term) tcell_reset_terms_map[cell].emplace_back(term);
            } else {
                TTerm *term = cell->getOrCreateTerm(name);
                if (term == nullptr) {
                    open_edi::util::message->issueMsg("TIMINGLIB", 1, kError,
                                                      "tterm", name.c_str());
                    return false;
                }
                term->setName(name);
                term->setDummy(true);
                tterm_names.emplace_back(name);
                tterm_map[name] = term;
                tcell_reset_terms_map[cell].emplace_back(term);
            }
        }
    }

    std::vector<Term *> reset_terms;
    reset_terms.reserve(term_names.size());
    for (auto &name : term_names) {
        Term *t = term_map[name];
        if (t) reset_terms.emplace_back(t);
    }
    cell->resetTerms(reset_terms);

    for (auto iter : tcell_reset_terms_map) {
        TCell *cell = iter.first;
        std::vector<TTerm *> &terms = iter.second;
        cell->resetTerms(terms);
    }

    return true;
}
void buildTermMapping() {
    Cell *topCell = getTopCell();
    if (topCell == nullptr) return;

    std::unordered_multimap<std::string, Cell *> cell_map;
    std::unordered_set<std::string> cell_names;
    std::function<void(Cell *, std::unordered_multimap<std::string, Cell *> *,
                       std::unordered_set<std::string> *)>
        getCells;
    getCells = [&getCells](
                   Cell *cell,
                   std::unordered_multimap<std::string, Cell *> *cell_map,
                   std::unordered_set<std::string> *cell_names) {
        uint64_t size = cell->getNumOfCells();
        for (uint64_t index = 0; index < size; ++index) {
            auto c = cell->getCell(index);
            if (c != nullptr) {
                cell_map->insert(std::make_pair(c->getName(), c));
                cell_names->insert(c->getName());
                getCells(c, cell_map, cell_names);
            }
        }
    };

    getCells(topCell, &cell_map, &cell_names);
    if (cell_map.empty()) return;

    Timing *timing_lib = getTimingLib();
    std::unordered_multimap<std::string, TCell *> tcell_map;
    uint64_t size = timing_lib->getNumOfAnalysisViews();
    for (auto index = 0; index < size; ++index) {
        AnalysisView *view = timing_lib->getAnalysisView(index);
        if (view == nullptr) continue;
        auto corner = view->getAnalysisCorner();
        if (corner == nullptr) continue;
        auto libset = corner->getLibset();
        if (libset == nullptr) continue;
        auto libs = libset->getTimingLibs();
        for (auto lib : libs) {
            auto cells = lib->getTimingCells();
            for (auto cell : cells) {
                if (cell)
                    tcell_map.insert(std::make_pair(cell->getName(), cell));
            }
        }
    }
    if (tcell_map.empty()) return;

    for (auto &str : cell_names) {
        auto iter_cell = cell_map.equal_range(str);
        std::vector<Cell *> cells;
        uint64_t cur_cell_count = 0;
        for (auto t = iter_cell.first; t != iter_cell.second; ++t) {
            cells.emplace_back(t->second);
            ++cur_cell_count;
        }
        if (cur_cell_count > 1) {
            open_edi::util::message->issueMsg("TIMINGLIB", 2, kError, "cell",
                                              str.c_str());
            continue;
        }

        auto iter_tcell = tcell_map.equal_range(str);
        std::vector<TCell *> tcells;
        for (auto t = iter_tcell.first; t != iter_tcell.second; ++t) {
            tcells.emplace_back(t->second);
        }
        if (tcells.empty()) continue;
        bool ret = buildTermMapping(cells[0], tcells);
        if (!ret) return;
    }
}

bool parseLib(LibSet *libset, const std::string &file,
              const std::string &dump_lib_file, const std::string &dump_db_file,
              const std::string &dump_log_file, bool clearFileContent) {
    if (libset == nullptr) return false;

    Timinglib::LibBuilder builder(libset);
    Timinglib::LibSyn libSyn(&builder);

    std::string parseLogStr = "";
    bool ret = libSyn.parseLibertyFile(file.c_str(), &parseLogStr);
    if (ret == true) {
        ret = libSyn.isLibertySyntaxValid();
    } else {
        return false;
    }

    if (dump_lib_file != "") {
        if (false ==
            libSyn.dumpLibFile(dump_lib_file.c_str(), clearFileContent))
            return false;
    }
    uint8_t endl_c = '\n';
    if (dump_log_file != "") {
        // open_edi::util::message->info("Write file %s...\n",
        //                              dump_log_file.c_str());
        std::ios_base::openmode mode = std::ios::out;
        if (clearFileContent)
            mode = mode | std::ios::trunc;
        else
            mode = mode | std::ios::app;
        std::ofstream os_log(dump_log_file.c_str(), mode);
        if (os_log.fail()) {
            open_edi::util::message->issueMsg("TIMINGLIB", 3, kError,
                                              dump_log_file.c_str());
            return false;
        }
        os_log << parseLogStr << endl_c;
        os_log.close();
        // open_edi::util::message->info("Write file %s finished.\n",
        //                              dump_log_file.c_str());
    }
    if (dump_db_file != "") {
        // open_edi::util::message->info("Write file %s...\n",
        //                              dump_db_file.c_str());
        std::ios_base::openmode mode = std::ios::out;
        if (clearFileContent)
            mode = mode | std::ios::trunc;
        else
            mode = mode | std::ios::app;
        OStream<std::ofstream> os(dump_db_file.c_str(), mode);
        if (!os.isOpen()) {
            open_edi::util::message->issueMsg("TIMINGLIB", 3, kError,
                                              dump_db_file.c_str());
            return false;
        }
        os << *libset << endl_c;
        os.close();
        // open_edi::util::message->info("Write file %s...\n",
        //                              dump_db_file.c_str());
    }

    // open_edi::util::message->info("Building term mapping...\n");
    buildTermMapping();
    // open_edi::util::message->info("Building term mapping finished.\n");

    return true;
}

int readTimingLibCommand(ClientData cld, Tcl_Interp *itp, int argc,
                         const char *argv[]) {
    if (argc > 1) {
        std::vector<std::string> files;
        std::string dump_lib_file = "";
        std::string dump_db_file = "";
        std::string dump_log_file = "";
        for (int i = 1; i < argc; ++i) {
            if (!strcmp(argv[i], "-dump_lib")) {
                if ((i + 1) < argc) {
                    dump_lib_file = argv[++i];
                } else {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
            } else if (!strcmp(argv[i], "-dump_db")) {
                if ((i + 1) < argc) {
                    dump_db_file = argv[++i];
                } else {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
            } else if (!strcmp(argv[i], "-dump_log")) {
                if ((i + 1) < argc) {
                    dump_log_file = argv[++i];
                } else {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
            } else {
                files.emplace_back(argv[i]);
            }
        }
        size_t lib_file_count = files.size();
        if (lib_file_count == 0) {
            open_edi::util::message->issueMsg("TIMINGLIB", 4, kError,
                                              "liberty");
            return TCL_ERROR;
        }
        for (int i = 0; i < lib_file_count; ++i) {
            if (access(files[i].c_str(), F_OK) == -1) {
                open_edi::util::message->issueMsg(
                    "TIMINGLIB", 5, open_edi::util::kError, files[i].c_str());
                return TCL_ERROR;
            }
        }
        Timing *timing_lib = getTimingLib();
        if (timing_lib == nullptr) {
            open_edi::util::message->issueMsg(
                "TIMINGLIB", 6, open_edi::util::kError, "top container",
                "reading timing library");
            return TCL_ERROR;
        }

        std::string default_name = "default";
        auto view = timing_lib->getAnalysisView(default_name);
        AnalysisCorner *corner = nullptr;
        if (view == nullptr) {
            auto mode = timing_lib->createAnalysisMode(default_name);
            if (mode == nullptr) {
                open_edi::util::message->issueMsg(
                    "TIMINGLIB", 1, open_edi::util::kError, "default", "mode");
                return TCL_ERROR;
            }
            corner = timing_lib->createAnalysisCorner(default_name);
            if (corner == nullptr) {
                open_edi::util::message->issueMsg("TIMINGLIB", 1,
                                                  open_edi::util::kError,
                                                  "default", "corner");
                return TCL_ERROR;
            }
            view = timing_lib->createAnalysisView(default_name);
            if (view == nullptr) {
                open_edi::util::message->issueMsg(
                    "TIMINGLIB", 1, open_edi::util::kError, "default", "view");
                return TCL_ERROR;
            }

            view->setAnalysisMode(mode->getId());
            view->setAnalysisCorner(corner->getId());
            view->setActive(true);
            view->setSetup(true);
            view->setHold(true);
            timing_lib->addActiveSetupView(view->getId());
            timing_lib->addActiveHoldView(view->getId());
        } else {
            corner = view->getAnalysisCorner();
        }
        LibSet *libset = corner->getLibset();
        if (libset == nullptr) {
            libset = Object::createObject<LibSet>(kObjectTypeLibSet,
                                                  timing_lib->getId());
            if (libset == nullptr) {
                open_edi::util::message->issueMsg("TIMINGLIB", 1,
                                                  open_edi::util::kError,
                                                  "default", "libset");
                return TCL_ERROR;
            }
            libset->setName(corner->getName());
            corner->setLibset(libset->getId());
        }

        if (lib_file_count != 0)
            open_edi::util::message->info("\nReading Timing Library\n");
        bool clearFileContent = true;
        bool bSuccess = true;
        for (int i = 0; i < lib_file_count; ++i) {
            if (i != 0) clearFileContent = false;

            if (false == parseLib(libset, files[i], dump_lib_file, dump_db_file,
                                  dump_log_file, clearFileContent)) {
                bSuccess = false;
                break;
            }
        }
        if (lib_file_count != 0) {
            if (bSuccess) {
                open_edi::util::message->info(
                    "\nRead Timing Library successfully.\n");
                return TCL_OK;
            } else {
                open_edi::util::message->issueMsg("TIMINGLIB", 9,
                                                  open_edi::util::kError,
                                                  "read Timing Library");
                return TCL_ERROR;
            }
        }
    } else {
        open_edi::util::message->issueMsg("TIMINGLIB", 4, kError, "liberty");
        return TCL_ERROR;
    }

    return TCL_OK;
}

typedef struct analysisViewArgs {
    std::string name = "";
    std::string mode = "";
    std::string corner = "";
} AnalysisViewArgs;

void printAnalysisViewCommandHelp() {
    open_edi::util::message->info("create_analysis_view:\n");
    open_edi::util::message->info("                     -name xxx\n");
    open_edi::util::message->info("                     -mode xxx\n");
    open_edi::util::message->info("                     -corner xxx\n");
    open_edi::util::message->info("                     -help\n");
}

int createAnalysisViewCommand(ClientData cld, Tcl_Interp *itp, int argc,
                              const char *argv[]) {
    AnalysisViewArgs args;
    if (argc == 2 && !strcmp(argv[1], "-help")) {
        printAnalysisViewCommandHelp();
        return TCL_OK;
    }
    if (argc == 7) {
        for (int i = 1; i < argc; ++i) {
            if (!strcmp(argv[i], "-name")) {
                if ((i + 1) < argc) {
                    args.name = argv[++i];
                } else {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }

            } else if (!strcmp(argv[i], "-mode")) {
                if ((i + 1) < argc) {
                    args.mode = argv[++i];
                } else {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
            } else if (!strcmp(argv[i], "-corner")) {
                if ((i + 1) < argc) {
                    args.corner = argv[++i];
                } else {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
            } else {
                open_edi::util::message->issueMsg("TIMINGLIB", 16,
                                                  open_edi::util::kError);
                return TCL_ERROR;
            }
        }

        Timing *timing_lib = getTimingLib();
        if (timing_lib == nullptr) {
            open_edi::util::message->issueMsg(
                "TIMINGLIB", 6, open_edi::util::kError, "top container",
                "creating analysis view");
            return TCL_ERROR;
        }

        if (args.name == "default") {
            open_edi::util::message->issueMsg(
                "TIMINGLIB", 10, open_edi::util::kError, "default");
            return TCL_ERROR;
        }

        // find AnalysisMode by mode
        AnalysisMode *analysis_mode = timing_lib->getAnalysisMode(args.mode);
        if (analysis_mode == nullptr) {
            open_edi::util::message->issueMsg("TIMINGLIB", 11,
                                              open_edi::util::kError, "mode",
                                              args.mode.c_str());
            return TCL_ERROR;
        }
        auto mode = Object::createObject<AnalysisMode>(kObjectTypeAnalysisMode,
                                                       timing_lib->getId());
        if (mode == nullptr) {
            open_edi::util::message->issueMsg("TIMINGLIB", 1,
                                              open_edi::util::kError, "mode");
            return TCL_ERROR;
        }
        *mode = *analysis_mode;

        // find AnalysisCorner by corner
        AnalysisCorner *analysis_corner =
            timing_lib->getAnalysisCorner(args.corner);
        if (analysis_corner == nullptr) {
            open_edi::util::message->issueMsg("TIMINGLIB", 11,
                                              open_edi::util::kError, "corner",
                                              args.corner.c_str());
            return TCL_ERROR;
        }
        auto corner = Object::createObject<AnalysisCorner>(
            kObjectTypeAnalysisCorner, timing_lib->getId());
        if (corner == nullptr) {
            open_edi::util::message->issueMsg("TIMINGLIB", 1,
                                              open_edi::util::kError, "corner");
            return TCL_ERROR;
        }
        *corner = *analysis_corner;

        AnalysisView *analysis_view = timing_lib->createAnalysisView(args.name);
        if (analysis_view == nullptr) {
            open_edi::util::message->issueMsg(
                "TIMINGLIB", 1, open_edi::util::kError, "analysis view",
                args.name.c_str());
            return TCL_ERROR;
        }
        mode->setOwner(analysis_view->getId());
        analysis_view->setAnalysisMode(mode->getId());

        corner->setOwner(analysis_view->getId());
        analysis_view->setAnalysisCorner(corner->getId());

        open_edi::util::message->info("Creating view %s successfully.\n",
                                      args.name.c_str());

        return TCL_OK;
    } else {
        open_edi::util::message->issueMsg("TIMINGLIB", 16,
                                          open_edi::util::kError);
        return TCL_ERROR;
    }

    return TCL_OK;
}

typedef struct analysisModeArgs {
    std::string name = "";
    std::string constraint_file = "";
} AnalysisModeArgs;

void printAnalysisModeCommandHelp() {
    open_edi::util::message->info("create_analysis_mode:\n");
    open_edi::util::message->info("                     -name xxx\n");
    open_edi::util::message->info(
        "                     -constraint_file xxx\n");
    open_edi::util::message->info("                     -help\n");
}
int createAnalysisModeCommand(ClientData cld, Tcl_Interp *itp, int argc,
                              const char *argv[]) {
    if (argc == 2 && !strcmp(argv[1], "-help")) {
        printAnalysisModeCommandHelp();
        return TCL_OK;
    }
    if (argc == 5) {
        AnalysisModeArgs args;
        for (int i = 1; i < argc; ++i) {
            if (!strcmp(argv[i], "-name")) {
                if ((i + 1) < argc) {
                    args.name = argv[++i];
                } else {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
            } else if (!strcmp(argv[i], "-constraint_file")) {
                if ((i + 1) < argc) {
                    args.constraint_file = argv[++i];
                } else {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
            } else {
                open_edi::util::message->issueMsg("TIMINGLIB", 16,
                                                  open_edi::util::kError);
                return TCL_ERROR;
            }
        }
        if (access(args.constraint_file.c_str(), F_OK) == -1) {
            open_edi::util::message->issueMsg("TIMINGLIB", 5,
                                              open_edi::util::kError,
                                              args.constraint_file.c_str());
            return TCL_ERROR;
        }
        Timing *timing_lib = getTimingLib();
        if (timing_lib == nullptr) {
            open_edi::util::message->issueMsg(
                "TIMINGLIB", 6, open_edi::util::kError, "top container",
                "creating analysis mode");
            return TCL_ERROR;
        }

        if (args.name == "default") {
            open_edi::util::message->issueMsg(
                "TIMINGLIB", 10, open_edi::util::kError, "default");
            return TCL_ERROR;
        }

        AnalysisMode *mode = timing_lib->createAnalysisMode(args.name);
        if (mode == nullptr) {
            open_edi::util::message->issueMsg("TIMINGLIB", 1,
                                              open_edi::util::kError, "mode",
                                              args.name.c_str());
            return TCL_ERROR;
        }
        mode->addConstraintFile(args.constraint_file);
        mode->createSdc();

        open_edi::util::message->info("Creating mode %s successfully.\n",
                                      args.name.c_str());

        return TCL_OK;
    } else {
        open_edi::util::message->issueMsg("TIMINGLIB", 16,
                                          open_edi::util::kError);
        return TCL_ERROR;
    }
    return TCL_OK;
}

typedef struct analysisCornerArgs {
    std::string name = "";
    std::string rc_tech = "";
    std::string lib_set = "";
} AnalysisCornerArgs;

void printAnalysisCornerCommandHelp() {
    open_edi::util::message->info("create_analysis_corner:\n");
    open_edi::util::message->info("                      -name xxx\n");
    open_edi::util::message->info("                      -rc_tech xxx\n");
    open_edi::util::message->info("                      -lib_set xxx\n");
    open_edi::util::message->info("                      -help\n");
}
int createAnalysisCornerCommand(ClientData cld, Tcl_Interp *itp, int argc,
                                const char *argv[]) {
    if (argc == 2 && !strcmp(argv[1], "-help")) {
        printAnalysisCornerCommandHelp();
        return TCL_OK;
    }
    if (argc >= 7) {
        AnalysisCornerArgs args;
        std::string dump_lib_file = "";
        std::string dump_db_file = "";
        std::string dump_log_file = "";
        for (int i = 1; i < argc; ++i) {
            if (!strcmp(argv[i], "-name")) {
                if ((i + 1) < argc) {
                    args.name = argv[++i];
                } else {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
            } else if (!strcmp(argv[i], "-rc_tech")) {
                if ((i + 1) < argc) {
                    args.rc_tech = argv[++i];
                } else {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
            } else if (!strcmp(argv[i], "-lib_set")) {
                if ((i + 1) < argc) {
                    args.lib_set = argv[++i];
                } else {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
            } else if (!strcmp(argv[i], "-dump_lib")) {
                if ((i + 1) < argc) {
                    dump_lib_file = argv[++i];
                } else {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
            } else if (!strcmp(argv[i], "-dump_db")) {
                if ((i + 1) < argc) {
                    dump_db_file = argv[++i];
                } else {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
            } else if (!strcmp(argv[i], "-dump_log")) {
                if ((i + 1) < argc) {
                    dump_log_file = argv[++i];
                } else {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
            } else {
                open_edi::util::message->issueMsg("TIMINGLIB", 16,
                                                  open_edi::util::kError);
                return TCL_ERROR;
            }
        }
        if (access(args.rc_tech.c_str(), F_OK) == -1) {
            open_edi::util::message->issueMsg(
                "TIMINGLIB", 5, open_edi::util::kError, args.rc_tech.c_str());
            return TCL_ERROR;
        }
        if (access(args.lib_set.c_str(), F_OK) == -1) {
            open_edi::util::message->issueMsg(
                "TIMINGLIB", 5, open_edi::util::kError, args.lib_set.c_str());
            return TCL_ERROR;
        }
        Timing *timing_lib = getTimingLib();
        if (timing_lib == nullptr) {
            open_edi::util::message->issueMsg(
                "TIMINGLIB", 6, open_edi::util::kError, "top container",
                "creating anlysis corner");
            return TCL_ERROR;
        }

        if (args.name == "default") {
            open_edi::util::message->issueMsg(
                "TIMINGLIB", 10, open_edi::util::kError, "default");
            return TCL_ERROR;
        }

        LibSet *libset = Object::createObject<LibSet>(kObjectTypeLibSet,
                                                      timing_lib->getId());
        if (libset == nullptr) {
            open_edi::util::message->issueMsg("TIMINGLIB", 1,
                                              open_edi::util::kError, "libset",
                                              args.lib_set.c_str());
            return TCL_ERROR;
        }
        libset->setName(args.name);

        open_edi::util::message->info("\nReading Timing Library\n");
        if (false == parseLib(libset, args.lib_set, dump_lib_file, dump_db_file,
                              dump_log_file, true)) {
            // open_edi::util::message->issueMsg("TIMINGLIB", 9,
            //    open_edi::util::kError, "read Timing Library");
            open_edi::util::message->issueMsg("TIMINGLIB", 1,
                                              open_edi::util::kError, "corner",
                                              args.name.c_str());
            return TCL_ERROR;
        }
        open_edi::util::message->info("\nRead Timing Library successfully.\n");

        AnalysisCorner *corner = timing_lib->createAnalysisCorner(args.name);
        if (corner == nullptr) {
            open_edi::util::message->issueMsg("TIMINGLIB", 1,
                                              open_edi::util::kError, "corner",
                                              args.name.c_str());
            return TCL_ERROR;
        }
        corner->setRcTechFile(args.rc_tech);
        corner->setLibset(libset->getId());

        open_edi::util::message->info("Creating corner %s successfully.\n",
                                      args.name.c_str());

        return TCL_OK;
    } else {
        open_edi::util::message->issueMsg("TIMINGLIB", 16,
                                          open_edi::util::kError);
        return TCL_ERROR;
    }
    return TCL_OK;
}

typedef struct setAnalysisViewStatusArgs {
    bool active = false;
    bool setup = false;
    bool hold = false;
    bool max_tran = false;
    bool max_cap = false;
    bool min_cap = false;
    bool leakage_power = false;
    bool dynamic_power = false;
    bool cell_em = false;
    bool signal_em = false;
    std::string view_name = "";
} SetAnalysisViewStatusArgs;

void printSetAnalysisViewStatusCommandHelp() {
    open_edi::util::message->info("set_analysis_view_status:\n");
    open_edi::util::message->info(
        "                         -active true|false\n");
    open_edi::util::message->info(
        "                         -setup true|false\n");
    open_edi::util::message->info(
        "                         -hold true|false\n");
    open_edi::util::message->info(
        "                         -max_tran true|false\n");
    open_edi::util::message->info(
        "                         -max_cap true|false\n");
    open_edi::util::message->info(
        "                         -min_cap true|false\n");
    open_edi::util::message->info(
        "                         -leakage_power true|false\n");
    open_edi::util::message->info(
        "                         -dynamic_power true|false\n");
    open_edi::util::message->info(
        "                         -cell_em true|false\n");
    open_edi::util::message->info(
        "                         -signal_em true|false\n");
    open_edi::util::message->info("                         -view view_name\n");
    open_edi::util::message->info("                         -help\n");
}
int setAnalysisViewStatusCommand(ClientData cld, Tcl_Interp *itp, int argc,
                                 const char *argv[]) {
    auto getBool = [](const char *optionName, const char *value,
                      bool *retValue) {
        if (!strcmp(value, "true")) {
            *retValue = true;
            return true;
        } else if (!strcmp(value, "false")) {
            *retValue = false;
            return true;
        } else {
            open_edi::util::message->issueMsg(
                "TIMINGLIB", 12, open_edi::util::kError, value, optionName);
            return false;
        }
    };

    if (argc == 2 && !strcmp(argv[1], "-help")) {
        printSetAnalysisViewStatusCommandHelp();
        return TCL_OK;
    }
    if (argc == 23) {
        SetAnalysisViewStatusArgs args;
        for (int i = 1; i < argc; ++i) {
            if (!strcmp(argv[i], "-active")) {
                if ((i + 1) >= argc) {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
                if (!getBool(argv[i], argv[i + 1], &(args.active)))
                    return TCL_ERROR;
                ++i;
            } else if (!strcmp(argv[i], "-setup")) {
                if ((i + 1) >= argc) {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
                if (!getBool(argv[i], argv[i + 1], &(args.setup)))
                    return TCL_ERROR;
                ++i;
            } else if (!strcmp(argv[i], "-hold")) {
                if ((i + 1) >= argc) {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
                if (!getBool(argv[i], argv[i + 1], &(args.hold)))
                    return TCL_ERROR;
                ++i;
            } else if (!strcmp(argv[i], "-max_tran")) {
                if ((i + 1) >= argc) {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
                if (!getBool(argv[i], argv[i + 1], &(args.max_tran)))
                    return TCL_ERROR;
                ++i;
            } else if (!strcmp(argv[i], "-max_cap")) {
                if ((i + 1) >= argc) {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
                if (!getBool(argv[i], argv[i + 1], &(args.max_cap)))
                    return TCL_ERROR;
                ++i;
            } else if (!strcmp(argv[i], "-min_cap")) {
                if ((i + 1) >= argc) {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
                if (!getBool(argv[i], argv[i + 1], &(args.min_cap)))
                    return TCL_ERROR;
                ++i;
            } else if (!strcmp(argv[i], "-leakage_power")) {
                if ((i + 1) >= argc) {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
                if (!getBool(argv[i], argv[i + 1], &(args.leakage_power)))
                    return TCL_ERROR;
                ++i;
            } else if (!strcmp(argv[i], "-dynamic_power")) {
                if ((i + 1) >= argc) {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
                if (!getBool(argv[i], argv[i + 1], &(args.dynamic_power)))
                    return TCL_ERROR;
                ++i;
            } else if (!strcmp(argv[i], "-cell_em")) {
                if ((i + 1) >= argc) {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
                if (!getBool(argv[i], argv[i + 1], &(args.cell_em)))
                    return TCL_ERROR;
                ++i;
            } else if (!strcmp(argv[i], "-signal_em")) {
                if ((i + 1) >= argc) {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
                if (!getBool(argv[i], argv[i + 1], &(args.signal_em)))
                    return TCL_ERROR;
                ++i;
            } else if (!strcmp(argv[i], "-view")) {
                if ((i + 1) >= argc) {
                    open_edi::util::message->issueMsg("TIMINGLIB", 7, kError,
                                                      argv[i]);
                    return TCL_ERROR;
                }
                ++i;
                args.view_name = argv[i];
            } else {
                open_edi::util::message->issueMsg("TIMINGLIB", 16,
                                                  open_edi::util::kError);
                return TCL_ERROR;
            }
        }
        Timing *timing_lib = getTimingLib();
        if (timing_lib == nullptr) {
            open_edi::util::message->issueMsg(
                "TIMINGLIB", 6, open_edi::util::kError, "top container",
                "set anlysis view status");
            return TCL_ERROR;
        }

        // find AnalysisView by view name
        AnalysisView *view = timing_lib->getAnalysisView(args.view_name);
        if (view == nullptr) {
            open_edi::util::message->issueMsg("TIMINGLIB", 11,
                                              open_edi::util::kError, "view",
                                              args.view_name.c_str());
            return TCL_ERROR;
        }
        view->setActive(args.active);
        view->setSetup(args.setup);
        view->setHold(args.hold);
        view->setMaxTran(args.max_tran);
        view->setMaxCap(args.max_cap);
        view->setMinCap(args.min_cap);
        view->setLeakagePower(args.leakage_power);
        view->setDynamicPower(args.dynamic_power);
        view->setCellEm(args.cell_em);
        view->setSignalEm(args.signal_em);
        if (args.active && args.setup) {
            timing_lib->addActiveSetupView(view->getId());
        }
        if (args.active && args.hold) {
            timing_lib->addActiveHoldView(view->getId());
        }

        open_edi::util::message->info("Setting view %s status successfully.\n",
                                      args.view_name.c_str());

        return TCL_OK;
    } else {
        open_edi::util::message->issueMsg("TIMINGLIB", 16,
                                          open_edi::util::kError);
        return TCL_ERROR;
    }

    return TCL_OK;
}

}  // namespace db
}  // namespace open_edi
