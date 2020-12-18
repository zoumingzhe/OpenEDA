/**
 * @file  test_transform.cpp
 * @date  12/11/2020 03:33:22 PM CST
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

#include "db/core/db.h"
#include "tcl/test_app.h"
#include "util/util.h"
#include "infra/command_manager.h"

namespace open_edi {
namespace tcl {

using namespace std;
using namespace open_edi::util;
using namespace open_edi::db;
using namespace open_edi::infra;

static const char * kTransformTestOptionNames[6] = {
      "-directory",
      "-lef",
      "-def",
      "-pin",
      "-bbox",
      "-verbose"
};

struct inputValue {
    bool is_dir_set_;
    bool verbose_;
    std::string directory_;
    std::string lef_;
    std::string def_;
    std::string pin_;
    Box *box_;
};

static bool parseArgument (
    int argc, const char **argv,
    inputValue &input_value
) {
    Command* cmd = CommandManager::parseCommand(argc, argv);
    if (cmd == nullptr) return false;

    bool res = true;
    input_value.is_dir_set_ = false;
    input_value.verbose_ = false;
    //-directory
    if (cmd->isOptionSet(kTransformTestOptionNames[0])) {
        input_value.is_dir_set_ = true;
        res = cmd->getOptionValue(kTransformTestOptionNames[0],
                input_value.directory_);
    }
    //-lef
    if (res) {
        res = cmd->getOptionValue(kTransformTestOptionNames[1],
                input_value.lef_);
    }
    //-def
    if (res) {
        res = cmd->getOptionValue(kTransformTestOptionNames[2],
                input_value.def_);
    }
    //-pin
    if (res)  {
        res = cmd->getOptionValue(kTransformTestOptionNames[3],
                input_value.pin_);
    }
    //-box
    if (res)  {
        //TODO? getOptionValue for box &?
        res = cmd->getOptionValue(kTransformTestOptionNames[4],
                &input_value.box_);
    }
    //-verbose
    if (res && cmd->isOptionSet(kTransformTestOptionNames[5])) {
        input_value.verbose_ = true;
    }
    return res;
}

static int transformTest(ClientData cld, Tcl_Interp *itp, int argc,
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
        std::string lef_name = input_value.directory_;
        lef_name.append("/");
        lef_name.append(input_value.lef_);
        cmd_name = "read_lef ";
        cmd_name.append(lef_name);
        
        Tcl_Eval(itp, cmd_name.c_str());

        std::string def_name = input_value.directory_;
        def_name.append("/");
        def_name.append(input_value.def_);
        cmd_name = "read_def ";
        cmd_name.append(def_name);
        
        Tcl_Eval(itp, cmd_name.c_str());
    }
    // top cell is ready:
    top = getTopCell();
    if (top == nullptr) {
        std::cout << "Error: failed to get top cell.\n";
        return TCL_ERROR;
    }
    size_t last_slash = input_value.pin_.rfind("/");
    Pin *pin = nullptr;
    Inst * inst = nullptr;
    if (last_slash == std::string::npos) {
        std::cout << "Info: searching IO pin " << input_value.pin_
          << " to check bbox.\n";
        pin = top->getIOPin(input_value.pin_);
    } else {
        std::string pin_name = input_value.pin_.substr(last_slash + 1);
        std::string inst_name = input_value.pin_.substr(0, last_slash);
        std::cout << "Info: searching pin " <<  pin_name
          << " on inst "  << inst_name << " to check bbox.\n";
        inst = top->getInstance(inst_name);
        if (inst != nullptr) {
            pin = inst->getPin(pin_name);
        } else {
            std::cout << "Error: failed to get inst " << inst_name << ".\n";
            return TCL_ERROR;
        }
    }
    if (pin != nullptr) {
        std::vector<Box> box_vector;
        Box compared_box = *input_value.box_;
        pin->getBoxVector(box_vector);
        for (auto box : box_vector) {
            if (box.getLLX() == compared_box.getLLX() &&
                box.getLLY() == compared_box.getLLY() &&
                box.getURX() == compared_box.getURX() &&
                box.getURY() == compared_box.getURY()) {
                std::cout << "Info: transform is expected.\n";
                break;
            } else if (input_value.verbose_) {
                std::cout << "Debug: transformed box: ( "<< box.getLLX()
                  << " " << box.getLLY() << " " << box.getURX()
                  << " " << box.getURY() << " ) not as expected ( " 
                  << compared_box.getLLX() << " " << compared_box.getLLY()
                  << " " << compared_box.getURX() << " "
                  << compared_box.getURY() << " )\n";
            }
        }
    }
    if (input_value.verbose_ && inst) {
        inst->printPinGeoms();
    }
    //allocated in getData();
    delete input_value.box_;
    return TCL_OK;
}

void registerTestTransform(Tcl_Interp *itp) {
    CommandManager* cmd_manager = CommandManager::getCommandManager();
    Command* test_command = cmd_manager->createCommand(itp, transformTest,
            "__unittest_transform", "unit test for transform: check pin's bbox\n",
            ( cmd_manager->createOption(kTransformTestOptionNames[0],
                          OptionDataType::kString, false,
                          "file directory; default is current directory.")
            + cmd_manager->createOption(kTransformTestOptionNames[1],
                          OptionDataType::kString, true,
                          "lef file name")
            + cmd_manager->createOption(kTransformTestOptionNames[2],
                          OptionDataType::kString, true,
                          "def file name")
            + cmd_manager->createOption(kTransformTestOptionNames[3],
                          OptionDataType::kString, true,
                          "pin name")
            + cmd_manager->createOption(kTransformTestOptionNames[4],
                          OptionDataType::kRect, true,
                          "expected box")
            + cmd_manager->createOption(kTransformTestOptionNames[5],
                          OptionDataType::kBool, false,
                          "verbose message")
            ));
    //hide this command:
    //test_command->setIsPublic(false);
}

}  // namespace tcl
}  // namespace open_edi
