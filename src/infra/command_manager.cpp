
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cstdio>
#include <mutex>
#include <stack>

#include "command_manager.h"
#include "db/util/box.h"

namespace open_edi {
namespace infra {
//  using namespace opengeo;

std::mutex infra_mutex_;

CommandManager* CommandManager::instance_ = nullptr;

CommandManager* CommandManager::getCommandManager() {
    if (instance_ == nullptr) {
        if (infra_mutex_.try_lock()) {
            instance_ = new CommandManager();
            infra_mutex_.unlock();
        }
    }
    return instance_;
}

int CommandManager::RegisterCommand(Command* v) {
    std::map<std::string, Command*>::iterator iter = commands_.find(v->getName());
    if (iter == commands_.end()) {
        commands_[v->getName()] = v;
        return 0;
    } else {
        return -1;
    }
}


Command* CommandManager::getCommandByName(const char* name) {
    std::map<std::string, Command*>::iterator iter = commands_.find(name);
    if (iter != commands_.end())
        return iter->second;
    else
        return nullptr;
}

Command* CommandManager::createCommandByName(const char* name) {
    Command* command = new Command();
    command->setName(name);
    RegisterCommand(command);
    return command;
}

Command* CommandManager::parseCommand(int argc, const char *argv[]) {
    Command* cmd = getCommandManager()->getCommandByName(argv[0]);
    if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-help")) {
        message->info(cmd->getDescription().c_str());
        for (int i = 0; i < cmd->getOptionNum(); i++) {
            Option *opt = cmd->getOption(i);
            if (opt == nullptr) {
                message->issueMsg("INFRA", kGetOptionFail, kError);
            }
            std::string head = opt->getName();
            switch (opt->getType())
            {
            case OptionDataType::kNone:
                break;
            case OptionDataType::kBool:
            {
                head += " <bool>";
                break;
            }
            case OptionDataType::kInt:
            {
                head += " <int>";
                break;
            }
            case OptionDataType::kDouble:
            {
                head += " <double>";
                break;
            }
            case OptionDataType::kString:
            {
                head += " <string>";
                break;
            }
            case OptionDataType::kEnum:
            {
                head += " <enum>";
                break;
            }
            case OptionDataType::kPoint:
            {
                head += " <point>";
                break;
            }
            case OptionDataType::kRect:
            {
                head += " <rect>";
                break;
            }
            case OptionDataType::kIntList:
            {
                head += " <int list>";
                break;
            }
            case OptionDataType::kDoubleList:
            {
                head += " <double list>";
                break;
            }
            case OptionDataType::kStringList:
            {
                head += " <string list>";
                break;
            }
            default:
            {
                break;
            }
            }
            int max_size = 30;
            if (head.size() > max_size) {
                head += "\n";
                message->info(head.c_str());
                std::string body;
                for (int i = 0; i < max_size; i++) {
                    body += " ";
                }
                body += opt->getDescription().c_str();
                //body += "\n";
                message->info(body.c_str());
            } else {
                int blank_size = max_size - head.size();
                for (int i = 0; i < blank_size; i++) {
                    head += " ";
                }
                head += opt->getDescription().c_str();
                //head += "\n";
                message->info(head.c_str());
            }
        }
        return nullptr;
    }
    if (cmd != nullptr) {
        int res = cmd->parser(argc, argv);
        if (res != 0) {
            message->info("mesage parse error\n");
            return cmd;   // whether return nulllptr
        }
    } else {
        message->info("command not recoginized \n");
    }
    return cmd;
}

Command* CommandManager::createCommand(const char* cmd_name, const char* description, Option& opt_head) {
    Command* command = new Command();
    std::string name_cmd = cmd_name;
    command->setName(cmd_name);
    command->setDescription(description);
    RegisterCommand(command);
    Option* opt_ptr = &opt_head;
    std::stack<Option*> opt_stack;  // to reverse the sequence in the command
    while (opt_ptr) {
        opt_stack.push(opt_ptr);
        opt_ptr = opt_ptr->getNext();
    }
    while (!opt_stack.empty()) {
        command->addOption(opt_stack.top());
        //  message->info("add opt %s in create command \n", opt_stack.top()->getName().c_str());
        opt_stack.pop();
    }

    return command;
}

Command* CommandManager::createCommand(const char* cmd_name, const char* description, Option& opt_head, OptionGroup& group_head) {
    Command* command = new Command();
    std::string name_cmd = cmd_name;
    command->setName(cmd_name);
    command->setDescription(description);
    RegisterCommand(command);
    Option* opt_ptr = &opt_head;
    std::stack<Option*> opt_stack;  // to reverse the sequence in the command
    while (opt_ptr) {
        opt_stack.push(opt_ptr);
        opt_ptr = opt_ptr->getNext();
    }
    while (!opt_stack.empty()) {
        command->addOption(opt_stack.top());
        //  message->info("add opt %s in create command \n", opt_stack.top()->getName().c_str());
        opt_stack.pop();
    }

    OptionGroup* group_ptr = &group_head;
    while (group_ptr) {
        Option* opt1 = command->getOption(group_ptr->getOpt1Name().c_str());
        if (opt1 == nullptr)
            message->issueMsg("INFRA", kGroupGetOptionFail, kError, group_ptr->getOpt1Name().c_str());
        group_ptr->setOpt1Ptr(opt1);
        Option* opt2 = command->getOption(group_ptr->getOpt2Name().c_str());
        if (opt2 == nullptr)
            message->issueMsg("INFRA", kGroupGetOptionFail, kError, group_ptr->getOpt2Name().c_str());
        group_ptr->setOpt2Ptr(opt2);

        command->addGroup(group_ptr);
        group_ptr = group_ptr->getNext();
    }

    return command;
}

}  //  namespace infra
}  //  namespace open_edi
