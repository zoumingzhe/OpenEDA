/**
 * @file create_sdc_commands.cpp
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

#include "db/timing/sdc/create_sdc_commands.h"
#include "infra/command_manager.h"

namespace open_edi {
namespace db {

constexpr bool kcreateSuccess = true;
constexpr bool kcreateFailed = false;

using Command = open_edi::infra::Command;
using CommandManager = open_edi::infra::CommandManager;
using Option = open_edi::infra::Option;
using OptionDataType = open_edi::infra::OptionDataType;

// general purpose commands
bool createSdcGeneralPurposeCommands() {

    return kcreateSuccess;
}

// object access commands
bool createSdcObjectAccessCommands() {

    return kcreateSuccess;
}

// timing constraints
bool createSdcTimingConstraints() {

    return kcreateSuccess;
}

// environment commands 
bool createSdcEnvironmentCommands() {
    //TODO command create success check
    CommandManager* cmd_manager = CommandManager::getCommandManager();

    Command* cmd = cmd_manager->createCommand(
        "set_case_analysis", "\n", 
            *(new Option("value", OptionDataType::kString, true, "\n")) + 
            *(new Option("port_or_pin_list", OptionDataType::kStringList, true, "\n"))
    );
    assert(cmd);
    return kcreateSuccess;
}

// multivoltage power commands
bool createSdcMultivoltagePowerCommands() {

    return kcreateSuccess;
}

// main create manager
bool createSdcCommands() {
    return {
        createSdcGeneralPurposeCommands() and
        createSdcObjectAccessCommands() and
        createSdcTimingConstraints() and
        createSdcEnvironmentCommands() and
        createSdcMultivoltagePowerCommands()
    };
}


}
}
