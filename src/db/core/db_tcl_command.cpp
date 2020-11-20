
/* @file  db_tcl_command.cpp
 * @date  Aug 2020
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NiiCEDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "db/core/db_tcl_command.h"

#include <gperftools/profiler.h>

#include "db/core/db.h"
#include "db/io/read_def.h"
#include "db/io/read_lef.h"
#include "db/io/read_write_db.h"
#include "db/io/read_verilog.h"
#include "db/io/write_def.h"
#include "db/io/write_lef.h"
#include "db/io/write_verilog.h"
#include "db/timing/timinglib/timinglib_tcl_command.h"
#include "db/timing/spef/spef_tcl_command.h"
#include "util/util.h"

namespace open_edi {
namespace db {

static bool kIsLEFReaded = false;

// read LEF file
static int readLefCommand(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    kIsLEFReaded = false;
    int result = util::runCommandWithProcessBar(readLef, argc, argv);
    if (0 == result) {
        kIsLEFReaded = true;
    }
    return result;
}

static int writeLefCommand(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    return util::runCommandWithProcessBar(writeLef, argc, argv);
}

// read DEF file
static int readDefCommand(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    return util::runCommandWithProcessBar(readDef, argc, argv);
}

static int writeDefCommand(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    return util::runCommandWithProcessBar(writeDef, argc, argv);
}

// read verilog file
static int readVerilogCommand(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    if (!kIsLEFReaded) {
        message->issueMsg(kError, "read_lef command should be run first.\n");
        return TCL_ERROR;
    }
    return util::runCommandWithProcessBar(readVerilog, argc, argv);
}

// write verilog file
static int writeVerilogCommand(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    return util::runCommandWithProcessBar(writeVerilog, argc, argv);
}

// read db from disk
enum readWriteDBArgument { kRWDBDBFile = 1, kRWDBDebug = 2, kRWDBUnknown };

static int readDBCommand(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    std::string cell_name;
    bool debug = false;

    for (int i = 1; i < argc; ++i) {
        switch (i) {
            case kRWDBDBFile:
                cell_name = argv[i];
                break;
            case kRWDBDebug:
                debug = true;  // no matter what value. set debug mode.
                break;
            case kRWDBUnknown:
            default:
                break;
        }
    }

    ReadDesign read_design(cell_name);
    read_design.setDebug(debug);
    return read_design.run();
}
// end of read_design

// write db to disk
static int writeDBCommand(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    std::string cell_name;
    bool debug = false;

    for (int i = 1; i < argc; ++i) {
        switch (i) {
            case kRWDBDBFile:
                cell_name = argv[i];
                break;
            case kRWDBDebug:
                debug = true;  // no matter what value. set debug mode.
                break;
            case kRWDBUnknown:
            default:
                break;
        }
    }

    if (!cell_name.compare("")) {
        message->issueMsg(kError, "Invalid DB file name.\n");
        return TCL_ERROR;
    }

    Cell *top_cell = getTopCell();
    if (!top_cell || !top_cell->getPool()) {
        message->issueMsg(kError, "Failed to get top cell.\n");
        return TCL_ERROR;
    }
    WriteDesign write_design(cell_name);
    write_design.setDebug(debug);
    return write_design.run();
}
// end of write_design

// create a cell -- internal command for testing.
static int createCellCommand(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    if (argc < 2) {
        return TCL_ERROR;
    }
    std::string top_cell_name = argv[1];
    Cell *top_cell = getTopCell();
    if (!top_cell) {
        initTopCell();
    }

    if (!(top_cell = getTopCell()) || !top_cell->getPool()) {
        message->issueMsg(kError, "Failed to init top cell.\n");
        return TCL_ERROR;
    }
    top_cell->setName(top_cell_name);
    std::string subcellname("SUB");
    std::string inst1name("sub1");
    std::string inst2name("sub2");

    std::string powername("VDD");
    std::string groundname("VSS");
    std::string inputname("IN");
    std::string outputname("OUT");
    std::string apinname("A");
    std::string ypinname("Y");
    std::string net1name("net1");
    std::string net2name("net2");
    std::string net3name("net3");

    Cell *subcell = top_cell->createCell(subcellname, false);
    // just for test:
    top_cell->createCell(inst1name, false);
    top_cell->createCell(inst2name, false);
    top_cell->createCell(powername, false);
    top_cell->createCell(groundname, false);
    top_cell->createCell(inputname, false);
    top_cell->createCell(outputname, false);
    top_cell->createCell(apinname, false);
    top_cell->createCell(ypinname, false);
    top_cell->createCell(net1name, false);
    top_cell->createCell(net2name, false);
    top_cell->createCell(net3name, false);

    Polygon *poly = new Polygon;
    const uint32_t num_pts = 100;
    for (uint32_t i = 0; i < num_pts; ++i) {
        Point *pt = new Point(i, i + 1);
        poly->addPoint(pt);
    }
    top_cell->getPolygonTable()->addPolygon(poly);

#if 0    
    Pin *top_pin_power = top_cell->createIOPin(powername);
    Pin *top_pin_ground = top_cell->createIOPin(groundname);
    Pin *top_pin_in = top_cell->createIOPin(inputname);
    Pin *top_pin_out = top_cell->createIOPin(outputname);
    Term *term_a = subcell->createTerm(apinname);
    Term *term_y = subcell->createTerm(ypinname);
    Term *term_power = subcell->createTerm(powername);
    Term *term_ground = subcell->createTerm(groundname);
    Inst *instance1 = top_cell->createInstance(inst1name);
    Inst *instance2 = top_cell->createInstance(inst2name);
    instance1->addCell(subcell);
    instance2->addCell(subcell);
    Pin *pin1_a = instance1->createInstancePin(apinname);
    Pin *pin1_y = instance1->createInstancePin(ypinname);
    Pin *pin1_power = instance1->createInstancePin(powername);
    Pin *pin1_ground = instance1->createInstancePin(groundname);
    Pin *pin2_a = instance2->createInstancePin(apinname);
    Pin *pin2_y = instance2->createInstancePin(ypinname);
    Pin *pin2_power = instance2->createInstancePin(powername);
    Pin *pin2_ground = instance2->createInstancePin(groundname);
    Net *net1 = top_cell->createNet(net1name);
    Net *net2 = top_cell->createNet(net2name);
    Net *net3 = top_cell->createNet(net3name);
    Net *power = top_cell->createNet(powername);
    Net *ground = top_cell->createNet(groundname);
    net1->addPin(top_pin_in);    
    net1->addPin(pin1_a);
    net2->addPin(pin1_y);
    net2->addPin(pin2_a);
    net3->addPin(pin2_y);
    net3->addPin(top_pin_out);
    power->addPin(top_pin_power);    
    power->addPin(pin1_power);
    power->addPin(pin2_power);
    ground->addPin(pin1_ground);
    ground->addPin(pin2_ground);
    ground->addPin(top_pin_ground);
    
    pin1_a->setNet(net1);
    pin1_y->setNet(net2);
    pin1_power->setNet(power);
    pin1_ground->setNet(ground);
    pin2_a->setNet(net2);
    pin2_y->setNet(net3);
    pin2_power->setNet(power);
    pin2_ground->setNet(ground);
#endif

    return TCL_OK;
}
// end of __create_cell

// report_cell
static int reportCellCommand(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Cell *top_cell = getTopCell();
    if (!top_cell || !top_cell->getPool()) {
        message->issueMsg(kError, "Failed to get top cell.\n");
        return TCL_ERROR;
    }

    MemPagePool *pool = top_cell->getPool();
    pool->printUsage();
#if 0    
    std::cout << "DEBUGINFO: report_cell top cell name " << top_cell->getName() << std::endl;
    std::cout << "DEBUGINFO: report_cell " << (long)top_cell << " first 8 bytes  " << ((long *)(top_cell))[0] << std::endl;
    std::cout << "DEBUGINFO: report_cell id: " << top_cell->getId() << " first 8 bytes  " << ((long *)(top_cell))[0] << std::endl;
#endif
    std::string top_cell_name(top_cell->getName());
    top_cell_name.append(".rpt");
    uint8_t endl_c = '\n';
    OStream<std::ofstream> os(top_cell_name.c_str(), std::ios::out | std::ios::app);
    os << "top_cell " << top_cell->getName().c_str() << endl_c;
    os << *top_cell << endl_c;

    SymbolTable *symbol_table = top_cell->getSymbolTable();
    int symbol_count = symbol_table->getSymbolCount();

    for (int i = 0; i < symbol_count; ++i) {
        std::string &symbol_name = symbol_table->getSymbolByIndex(i);

        Pin *pin = top_cell->getIOPin(symbol_name);
        if (pin != nullptr) {
            os << "top_pin " << symbol_name.c_str() << endl_c;
            os << *pin << endl_c;
        }
        Cell *subcell = top_cell->getCell(symbol_name);
        if (subcell != nullptr) {
            os << "subcell " << symbol_name.c_str() << endl_c;
            os << *subcell << endl_c;
            for (int j = 0; j < symbol_count; ++j) {
                std::string &term_name = symbol_table->getSymbolByIndex(j);

                Term *term = subcell->getTerm(term_name);
                if (term != nullptr) {
                    os << "subcell-term " << term_name.c_str() << endl_c;
                    os << *term << endl_c;
                }
            }
        }

        Inst *instance = top_cell->getInstance(symbol_name);
        if (instance != nullptr) {
            os << "instance " << symbol_name.c_str() << endl_c;
            os << *instance << endl_c;
            for (int j = 0; j < symbol_count; ++j) {
                std::string &pin_name = symbol_table->getSymbolByIndex(j);
                Pin *pin = instance->getPin(pin_name);
                if (pin != nullptr) {
                    os << "subcell-pin " << pin_name.c_str() << endl_c;
                    os << *pin << endl_c;
                }
            }
        }
        Net *net = top_cell->getNet(symbol_name);
        if (net != nullptr) {
            os << "net " << symbol_name.c_str() << endl_c;
            //  os << *net << endl_c;
        }
    }
    os.close();

    return TCL_OK;
}
// end of report_cell

void registerDatabaseTclCommands(Tcl_Interp *itp) {
    Tcl_CreateCommand(itp, "read_lef", readLefCommand, NULL, NULL);
    Tcl_CreateCommand(itp, "write_lef", writeLefCommand, NULL, NULL);
    Tcl_CreateCommand(itp, "read_def", readDefCommand, NULL, NULL);
    Tcl_CreateCommand(itp, "write_def", writeDefCommand, NULL, NULL);
    Tcl_CreateCommand(itp, "read_verilog", readVerilogCommand, NULL, NULL);
    Tcl_CreateCommand(itp, "write_verilog", writeVerilogCommand, NULL, NULL);
    Tcl_CreateCommand(itp, "read_timing_library", readTimingLibCommand, NULL, NULL);
    Tcl_CreateCommand(itp, "create_analysis_view", createAnalysisViewCommand, NULL, NULL);
    Tcl_CreateCommand(itp, "create_analysis_mode", createAnalysisModeCommand, NULL, NULL);
    Tcl_CreateCommand(itp, "create_analysis_corner", createAnalysisCornerCommand, NULL, NULL);
    Tcl_CreateCommand(itp, "set_analysis_view_status", setAnalysisViewStatusCommand, NULL, NULL);
    Tcl_CreateCommand(itp, "read_spef", readSpefCommand, NULL, NULL);
    Tcl_CreateCommand(itp, "write_spef", writeSpefCommand, NULL, NULL);
    Tcl_CreateCommand(itp, "read_design", readDBCommand, NULL, NULL);
    Tcl_CreateCommand(itp, "write_design", writeDBCommand, NULL, NULL);
    // testing commands. TODO: remove them.
    Tcl_CreateCommand(itp, "__create_cell", createCellCommand, NULL, NULL);
    Tcl_CreateCommand(itp, "__report_cell", reportCellCommand, NULL, NULL);
}

} // namespace db
} // namespace open_edi
