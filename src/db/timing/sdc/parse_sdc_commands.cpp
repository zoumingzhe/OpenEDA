/**
 * @file parse_sdc_commands.cpp
 * @date 2020-11-25
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
#include "db/core/cell.h"
#include "db/core/db.h"
#include "db/timing/sdc/parse_sdc_commands.h"
#include "db/timing/sdc/sdc.h"
#include "infra/command.h"
#include "infra/command_manager.h"

namespace open_edi {
namespace db {

using Command = open_edi::infra::Command;
using CommandPtr = std::shared_ptr<Command>;
using CommandManager = open_edi::infra::CommandManager;

using StringVector = std::vector<std::string>;
using StringVectorPtr = std::shared_ptr<StringVector>;

SdcPtr getSdc() {
    SdcPtr sdc_ptr = std::make_shared<Sdc>();
    assert(sdc_ptr);
    if (sdc_ptr == nullptr) {
        //TODO messages
    }
    return sdc_ptr;
}

// general purpose commands manager

// object access commands manager

// timing constraints manager

// environment commands manager
int parseSdcSetCaseAnalysis(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    SdcPtr sdc_ptr = getSdc();
    assert(sdc_ptr);
    if (sdc_ptr == nullptr) {
        return TCL_ERROR;
    }
    SetCaseAnalysisPtr case_analysis_ptr = std::make_shared<SetCaseAnalysis>();
    assert(case_analysis_ptr);
    if (case_analysis_ptr == nullptr) {
        //TODO messages
        return TCL_ERROR;
    }
    SdcCaseAnalysisContainerPtr container_ptr = std::make_shared<SdcCaseAnalysisContainer>();
    assert(container_ptr);
    if (container_ptr == nullptr) {
        //TODO messages
        return TCL_ERROR;
    }
    sdc_ptr->set_case_analysis_container(container_ptr);
    Command* cmd = CommandManager::parseCommand(argc, argv);
    assert(cmd);
    if (cmd->isOptionSet("value")) {
        std::string value;
        bool res = cmd->getOptionValue("value", value);
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        case_analysis_ptr->setValue(value);
        message->info("get first value %s \n", value.c_str());
    }
    if (cmd->isOptionSet("port_or_pin_list")) {
        StringVectorPtr port_or_pin_list = std::make_shared<StringVector>();
        assert(port_or_pin_list);
        bool res = cmd->getOptionValue("port_or_pin_list", (StringVector**)(&port_or_pin_list));
        if (!res) {
            //TODO messages
            return TCL_ERROR;
        }
        Cell* topCell = getTopCell();
        assert(topCell);
        if (topCell == nullptr) {
            //TODO messages
            return TCL_ERROR;
        }
        for (const auto& pin_name : *port_or_pin_list) {
            //TODO DB team did not implement the API to get pin/term from name
            ObjectId pin_id = 1; 
            //Pin *pin = topCell->getPin(pin_name); 
            //if (pin == nullptr) {
            //    //TODO messages
            //    return TCL_ERROR;
            //}
            //ObjectId pin_id = pin->getId();
            container_ptr->add(pin_id, case_analysis_ptr);
            message->info("get second value %s \n", pin_name.c_str());
        }
    }
    return TCL_OK;
}

// multivoltage power commands manager

// read sdc file
int parseReadSdc(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {

    return TCL_OK;
}

int parseWriteSdc(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {

    return TCL_OK;
}


}
}
