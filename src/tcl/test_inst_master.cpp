/**
 * @file  test_instMaster.cpp
 * @date  12/17/2020 06:33:22 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include "db/core/object.h"
#include "db/core/db.h"
#include "tcl/test_app.h"
#include "util/util.h"
#include "infra/command_manager.h"
#include "db/util/array.h"

namespace open_edi {
namespace tcl {

using namespace std;
using namespace open_edi::util;
using namespace open_edi::db;
using namespace open_edi::infra;

static const char * kInstMasterTestOptionNames[7] = {
      "-directory",
      "-lef",
      "-verilog",
      "-master",
      "-ref_count",
      "-verbose",
      "-detach_ref"
};

struct inputValue {
    std::string directory_;
    std::string lef_;
    std::string verilog_;
    std::string master_;
    int ref_count_;
    bool is_dir_set_;
    bool verbose_;
    bool all_;
    bool detach_ref_;
};

static void initInputValue(inputValue &input_value) {
    input_value.ref_count_ = 0;
    input_value.is_dir_set_ = false;
    input_value.verbose_ = false;
    input_value.all_ = false;
    input_value.detach_ref_ = false;
}

static bool parseArgument (
    int argc, const char **argv,
    inputValue &input_value
) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    if (cmd == nullptr) return false;
    bool res = true;

    initInputValue(input_value);
    //-directory
    if (cmd->isOptionSet(kInstMasterTestOptionNames[0])) {
        input_value.is_dir_set_ = true;
        res = cmd->getOptionValue(kInstMasterTestOptionNames[0],
                input_value.directory_);
    }
    //-lef
    if (res && cmd->isOptionSet(kInstMasterTestOptionNames[1])) {
        res = cmd->getOptionValue(kInstMasterTestOptionNames[1],
                input_value.lef_);
    }
    //-verilog
    if (res && cmd->isOptionSet(kInstMasterTestOptionNames[2])) {
        res = cmd->getOptionValue(kInstMasterTestOptionNames[2],
                input_value.verilog_);
    }
    //-master
    if (res)  {
        if (cmd->isOptionSet(kInstMasterTestOptionNames[3])) {
            res = cmd->getOptionValue(kInstMasterTestOptionNames[3],
                input_value.master_);
        } else {
            input_value.all_ = true;
        }      
    }
    //-ref_count
    if (res)  {
        res = cmd->getOptionValue(kInstMasterTestOptionNames[4],
                input_value.ref_count_);
    }
    //-verbose
    if (res && cmd->isOptionSet(kInstMasterTestOptionNames[5])) {
        input_value.verbose_ = true;
    }
    //-detach_ref
    if (res && cmd->isOptionSet(kInstMasterTestOptionNames[6])) {
        input_value.detach_ref_ = true;
    }    
    return res;
}

static bool collectMasters(
    Cell *hier_cell, 
    std::vector<Cell*> &master_vector, 
    bool is_top
) {
    ediAssert(hier_cell != nullptr);
    
    if (is_top) {
        ArrayObject<ObjectId> *stdcells = getTechLib()->getCellArray();
        if (stdcells != nullptr) {
            for (ArrayObject<ObjectId>::iterator iter = stdcells->begin();
                  iter != stdcells->end(); ++iter) {              
                ObjectId cell_id = *iter;
                if (!cell_id) continue;
                Cell *stdcell = Object::addr<Cell>(cell_id);
                if (!stdcell) continue;
                master_vector.push_back(stdcell);
            }
        }
    }
    ArrayObject<ObjectId> *masters = hier_cell->getCellArray();
    if (masters == nullptr) {
        return true;
    }
    for (ArrayObject<ObjectId>::iterator iter = masters->begin();
        iter != masters->end(); ++iter) {
        ObjectId cell_id = *iter;
        if (!cell_id) continue;
        Cell *hiercell = Object::addr<Cell>(cell_id);
        if (!hiercell) continue;
        master_vector.push_back(hiercell);
        collectMasters(hiercell, master_vector, false/*is_top*/);
    }
    return true;
}

static int instMasterTest(ClientData cld, Tcl_Interp *itp, int argc,
                       const char *argv[]) {
    inputValue input_value;
    if (!parseArgument(argc, argv, input_value)) {
        message->issueMsg(kError, "Failed in option parser\n");
        return TCL_ERROR;
    }
    Cell *top = getTopCell();
    if (top->getNameIndex() == kInvalidSymbolIndex) {
        if (!input_value.is_dir_set_) {
            input_value.directory_ = ".";
        }
        std::string cmd_name;
        if (input_value.lef_.compare("") != 0) {
            std::string lef_name = input_value.directory_;
            lef_name.append("/");
            lef_name.append(input_value.lef_);
            cmd_name = "read_lef ";
            cmd_name.append(lef_name);
            
            Tcl_Eval(itp, cmd_name.c_str());
        }
        if (input_value.verilog_.compare("") != 0) {
            std::string verilog_name = input_value.directory_;
            verilog_name.append("/");
            verilog_name.append(input_value.verilog_);
            cmd_name = "read_verilog ";
            cmd_name.append(verilog_name);
            Tcl_Eval(itp, cmd_name.c_str());
        }
    }
    // top cell is ready:
    top = getTopCell();
    if (top == nullptr) {
        std::cout << "Error: failed to get top cell.\n";
        return TCL_ERROR;
    }

    std::vector<Cell *> master_vector;

    if (!input_value.all_) {
        Cell *master = top->getCell(input_value.master_);
        if (master == nullptr) {
            std::cout << "Error: failed to get master.\n";
            return TCL_ERROR;
        }
        master_vector.push_back(master);
    } else {
        collectMasters(top, master_vector, true/*is_top*/);
    }
    
    for (auto master : master_vector) {
        if (master != nullptr) {
            int ref_count = master->getRefCount();
            if (!input_value.all_) {
                if (ref_count != input_value.ref_count_) {
                    std::cout << "ERROR: ref_count " 
                      << ref_count << " differs from golden " 
                      << input_value.ref_count_ << ".\n";
                } else if (input_value.verbose_) {
                    std::cout << "INFO: checking is passed.\n";
                }
            }
            if (!ref_count) continue;

            ArrayObject<ObjectId> *ref_insts = master->getRefInstArray();
            for (int i = 0; i < ref_count; ++i) {
                ObjectId inst_id = (*ref_insts)[i];
                if (!inst_id) continue;
                Inst *inst_obj = Object::addr<Inst>(inst_id);
                if (inst_obj == nullptr) continue;
                if (i == 0) {
                    std::cout << "Master " << master->getName() 
                      << " instatiated by " << ref_count << " instance(s): ";
                }
                std::cout << "#" << inst_obj->getId() << "#" 
                      << inst_obj->getName() << " ";
            }
            std::cout << ".\n";
            if (!input_value.detach_ref_) continue;
            for (int i = 0; i < ref_count; ++i) {
                ObjectId inst_id = (*ref_insts)[i];
                if (!inst_id) continue;
                Inst *inst_obj = Object::addr<Inst>(inst_id);
                if (inst_obj == nullptr) continue;
                if (input_value.verbose_) {
                    std::cout << "DEBUG: removing instance " << inst_obj->getName() << ".\n";
                }
                Object::deleteObject<Inst>(inst_obj);
            }
        }
    }

    return TCL_OK;
}

void registerTestInstMaster(Tcl_Interp *itp) {
    CommandManager* cmd_manager = CommandManager::getCommandManager();
    Command* test_command = cmd_manager->createCommand(itp, instMasterTest,
            "__unittest_inst_master", "unit test for instance and master relationship\n",
            ( cmd_manager->createOption(kInstMasterTestOptionNames[0],
                          OptionDataType::kString, false,
                          "file directory; default is current directory.")
            + cmd_manager->createOption(kInstMasterTestOptionNames[1],
                          OptionDataType::kString, false,
                          "lef file name")
            + cmd_manager->createOption(kInstMasterTestOptionNames[2],
                          OptionDataType::kString, false,
                          "verilog file name")
            + cmd_manager->createOption(kInstMasterTestOptionNames[3],
                          OptionDataType::kString, false,
                          "master name")
            + cmd_manager->createOption(kInstMasterTestOptionNames[4],
                          OptionDataType::kInt, false,
                          "expected count")
            + cmd_manager->createOption(kInstMasterTestOptionNames[5],
                          OptionDataType::kBool, false,
                          "verbose message")
            + cmd_manager->createOption(kInstMasterTestOptionNames[6],
                          OptionDataType::kBool, false,
                          "removed instatiated inst from master")            
            ));
    //hide this command:
    //test_command->setIsPublic(false);
}

}  // namespace tcl
}  // namespace open_edi
