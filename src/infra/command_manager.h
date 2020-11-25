/* @file  command_manager.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_INFRA_COMMAND_MANAGER_H_
#define SRC_INFRA_COMMAND_MANAGER_H_
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include "command.h"
#include <tcl.h>

namespace open_edi {
namespace infra {

class CommandManager {
 public:
    ~CommandManager() {}
    static CommandManager* getCommandManager();
    static Command* parseCommand(int argc, const char *argv[]);
    int RegisterCommand(Command* v);
    Command* createCommandByName(const char* name);
    Command* getCommandByName(const char* name);
    const std::map<std::string, Command*>* getAllCommands() const { return &commands_;}

    Command* createCommand(const char* cmd_name, const char* description, Option& opt_head);
    Command* createCommand(const char* cmd_name, const char* description, Option& opt_head, OptionGroup& group_head);

 private:
    CommandManager() { }

    static CommandManager* instance_;
    std::map<std::string, Command*> commands_;
    Tcl_Interp *current_interp_;
};

}  // namespace infra
}  // namespace open_edi

#endif  // SRC_INFRA_COMMAND_MANAGER_H_
