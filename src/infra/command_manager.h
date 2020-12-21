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

typedef int(*commandCallback)(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);

class CommandManager {
 public:
    ~CommandManager() {}
    static CommandManager* getCommandManager();
    static Command* parseCommand(int argc, const char *argv[]);
    int RegisterCommand(Command* v);
    Command* createCommandByName(const char* name);
    Command* getCommandByName(const char* name);
    const std::map<std::string, Command*>* getAllCommands() const { return &commands_;}

    Command* createCommand(Tcl_Interp *itp, commandCallback cb, const char* cmd_name, const char* description, Option& opt_head);
    Command* createCommand(Tcl_Interp *itp, commandCallback cb, const char* cmd_name, const char* description, Option& opt_head, OptionGroup& group_head);
    Option& createOption(const char* name, OptionDataType type, bool is_required, const char* description);
    Option& createOption(const char* name, OptionDataType type, bool is_required, std::vector<std::string>* v, const char* description);
    Option& createOption(const char* name, OptionDataType type, bool is_required, const char* v, const char* description);
    Option& createOption(const char* name, OptionDataType type, bool is_required, bool v, const char* description);
    Option& createOption(const char* name, OptionDataType type, bool is_required, int v, const char* description, int min = 0, int max = 0);
    Option& createOption(const char* name, OptionDataType type, bool is_required, double v, const char* description, double min = 0, double max = 0);
    OptionGroup& createOptionGroup(const char* name1, const char* name2, OptionRelation r);

 private:
    CommandManager() { }

    static CommandManager* instance_;
    std::map<std::string, Command*> commands_;
    Tcl_Interp *current_interp_;
};

}  // namespace infra
}  // namespace open_edi

#endif  // SRC_INFRA_COMMAND_MANAGER_H_
