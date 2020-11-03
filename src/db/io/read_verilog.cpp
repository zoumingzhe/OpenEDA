/* @file  read_verilog.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include <string.h>
#include <stdio.h>
#include <queue>
#include <vector>
#include <string>
#include <map>

#include "db/io/read_verilog.h"
#include "db/core/db.h"
#include "parser/verilog/kernel/register.h"
#include "parser/verilog/kernel/log.h"

#include "util/message.h"

namespace open_edi {

namespace db {
Yosys::RTLIL::Design *yosys_design = NULL;
bool kFirstRunReadVerilog = false;
std::map<Inst*, std::string> kInstMasterMap;

int kModuleNameHeaderLength = 10;

int readVerilog(int argc, const char **argv) {
    FILE *fp = NULL;

    std::vector<std::string> args;
    for (int i = 0; i < argc; ++i) {
        std::string token = argv[i];
        if (i > 0) {
            if (argv[i][0] != '-') {
                fp = fopen(argv[i], "r");
                if (NULL == fp) {
                    message->issueMsg(kError, "Cannot open file %s\n", argv[i]);
                } else {
                    fclose(fp);
                    args.push_back(token);
                }
            } else {
                args.push_back(token);
            }
        } else {
            args.push_back(token);
        }
    }
    if (args.size() <= 1) {
        message->issueMsg(kError, "No file name.\n");
        return 1;
    }

    if (!kFirstRunReadVerilog) {
        kFirstRunReadVerilog = true;
        Yosys::log_files.push_back(stdout);
        Yosys::Pass::init_register();
    }

    if (NULL == yosys_design) {
        yosys_design = new Yosys::RTLIL::Design;
    }

    message->info("\nReading Verilog\n");
    fflush(stdout);

    auto state = Yosys::pass_register[args[0]]->pre_execute();
    Yosys::pass_register[args[0]]->execute(args, yosys_design);
    Yosys::pass_register[args[0]]->post_execute(state);

    message->info("\nRead Verilog successfully.\n");

    return 0;
}

static std::string id2vl(std::string txt) {
    if (txt.size() > 1 && txt[0] == '\\') {
        txt = txt.substr(1);
    }
    for (size_t i = 0; i < txt.size(); i++) {
        if ('A' <= txt[i] && txt[i] <= 'Z') continue;
        if ('a' <= txt[i] && txt[i] <= 'z') continue;
        if ('0' <= txt[i] && txt[i] <= '9') continue;
        if (txt[i] == '_') continue;
        txt = "\\" + txt + " ";
        break;
    }
    return txt;
}

static bool getIntConstant(struct Yosys::AST::AstNode *current_ast,
                           int &constant) {
    int bound = 0;
    if (current_ast->type == Yosys::AST::AstNodeType::AST_NEG &&
        current_ast->children[0]->type == 
                                     Yosys::AST::AstNodeType::AST_CONSTANT) {
        bound = -1 * current_ast->children[0]->integer;
    } else if (current_ast->type == Yosys::AST::AstNodeType::AST_CONSTANT) {
        bound = current_ast->integer;
    } else {
        return false;
    }
    constant = bound;

    return true;
}

static bool getRealConstant(struct Yosys::AST::AstNode *current_ast,
                            double &constant) {
    double bound = 0.0;
    if (current_ast->type == Yosys::AST::AstNodeType::AST_NEG && 
        current_ast->children[0]->type ==
                                      Yosys::AST::AstNodeType::AST_REALVALUE) {
        bound = -1 * current_ast->children[0]->realvalue;
    } else if (current_ast->type == Yosys::AST::AstNodeType::AST_REALVALUE) {
        bound = current_ast->realvalue;
    } else {
        return false;
    }
    constant = bound;

    return true;
}

static std::string getIdWithRange(struct Yosys::AST::AstNode *ast_node) {
    std::string name;
    int int_constant = 0;
    if (ast_node->type == Yosys::AST::AstNodeType::AST_IDENTIFIER) {
        name = name + id2vl(ast_node->str);
        for (auto child : ast_node->children) {
            if (child->type == Yosys::AST::AstNodeType::AST_RANGE) {
                for (auto child_child : child->children) {
                    bool result = getIntConstant(child_child, int_constant);
                    if (result) {
                        name = name + "[" + std::to_string(int_constant) + "]";
                    }
                    break;
                }
                break;
            }
        }
    }
    return name;
}
static bool readVerilogWireToDB(Cell *hcell,
                                struct Yosys::AST::AstNode *current_ast) {
    Term *term = nullptr;
    Net  *net = nullptr;
    Bus  *bus = nullptr;
    int32_t range_left = INT_MAX, range_right = INT_MIN;
    std::string wire_name;
    wire_name = id2vl(current_ast->str);
    bool is_term = false;
    bool is_tri = current_ast->is_tri;

    if (current_ast->port_id > 0) {
        // is port
        is_term = true;
        term = hcell->createTerm(wire_name);
        if (!term) {
            message->issueMsg(kError,
                    "create term %s failed for module %s.\n",
                    wire_name.c_str(), hcell->getName().c_str());
            return false;
        }

        if (current_ast->is_input && current_ast->is_output) {
            term->setDirection(SignalDirection::kInout);
        } else if (current_ast->is_input) {
            term->setDirection(SignalDirection::kInput);
        } else if (current_ast->is_output) {
            term->setDirection(SignalDirection::kOutput);
        }
    }
    for (auto child : current_ast->children) {
        if (child->type == Yosys::AST::AstNodeType::AST_RANGE) {
            if (child->children.size() < 2) {
                message->issueMsg(kError,
                        "range misses msb or lsb for port or net %s.\n",
                        wire_name.c_str());
                return false;
            }
            getIntConstant(child->children[0], range_left);
            getIntConstant(child->children[1], range_right);

            if (term) {
                term->setHasRange(true);
                if (range_left < range_right) {
                    term->setRangeLow(range_left);
                    term->setRangeHigh(range_right);
                } else {
                    term->setRangeLow(range_right);
                    term->setRangeHigh(range_left);
                }
            } else {
                bus = hcell->createBus(wire_name);
                if (!bus) {
                    message->issueMsg(kError,
                            "create bus %s failed for module %s.\n",
                            wire_name.c_str(), hcell->getName().c_str());
                    return false;
                }
                bus->setRangeLeft(range_left);
                bus->setRangeRight(range_right);
                if (is_tri) {
                    bus->setIsTri(true);
                }
            }
        }
    }
    // create net
    std::vector<std::string> net_names;
    std::vector<std::string> bus_net_names;
    if (range_left != INT_MAX || range_right != INT_MIN) {
        std::string net_name = wire_name;
        net = hcell->createNet(net_name);
        if (!net) {
                message->issueMsg(kError,
                        "create net %s failed for module %s.\n",
                        net_name.c_str(), hcell->getName().c_str());
                return false;
        }
        net->setIsBusNet(true);
        if (is_term) {
            net->setIsFromTerm(true);
        }
        if (is_tri) {
            net->setType(NetType::kNetTypeTri);
        }

        int range_low = range_right;
        int range_high = range_left;

        if (range_left < range_right) {
            range_low = range_left;
            range_high = range_right;
        }
        for (int32_t i = range_low; i <= range_high; ++i) {
            net_name = wire_name + "[" + std::to_string(i) + "]";
            bus_net_names.push_back(net_name);
        }
    } else {
        net_names.push_back(wire_name);
    }
    for (std::string net_name : net_names) {
        net = hcell->createNet(net_name);
        if (!net) {
                message->issueMsg(kError,
                        "create net %s failed for module %s.\n",
                        net_name.c_str(), hcell->getName().c_str());
                return false;
        }
        if (is_term) {
            // a term should has pin and net with the same name
            Pin *pin = hcell->createIOPin(net_name);
            if (!pin) {
                message->issueMsg(kError,
                        "create pin %s failed for module %s.\n",
                        net_name.c_str(), hcell->getName().c_str());
                return false;
            }
            pin->setTerm(term);
            pin->setNet(net);
            net->addPin(pin);
            net->setIsFromTerm(true);
        }
    }
    for (std::string net_name : bus_net_names) {
        net = hcell->createNet(net_name);
        if (!net) {
                message->issueMsg(kError,
                        "create net %s failed for module %s.\n",
                        net_name.c_str(), hcell->getName().c_str());
                return false;
        }
        net->setIsOfBus(true);
        if (is_term) {
            // a term should has pin and net with the same name
            Pin *pin = hcell->createIOPin(net_name);
            if (!pin) {
                message->issueMsg(kError,
                        "create pin %s failed for module %s.\n",
                        net_name.c_str(), hcell->getName().c_str());
                return false;
            }
            pin->setTerm(term);
            pin->setNet(net);
            net->addPin(pin);
            net->setIsFromTerm(true);
        }
    }
    return true;
}

static bool readVerilogInstToDB(Cell *hcell,
                                struct Yosys::AST::AstNode *current_ast) {
    std::string inst_name = id2vl(current_ast->str);
    std::string cell_name = "";
    Cell *cell = nullptr;
    Inst *inst = hcell->createInstance(inst_name);
    Pin  *pin = nullptr;
    if (!inst) {
        message->issueMsg(kError,
                "create instance %s failed for module %s.\n",
                inst_name.c_str(), hcell->getName().c_str());
        return false;
    }
    inst->setOwner(hcell->getId());

    for (auto child : current_ast->children) {
        if (child->type == Yosys::AST::AstNodeType::AST_CELLTYPE) {
            cell_name = id2vl(child->str);
            cell = getTopCell()->getCell(cell_name);
            if (cell) {
                inst->setMaster(cell->getId());
            } else {
                kInstMasterMap[inst] = cell_name;
            }
        } else if (child->type == Yosys::AST::AstNodeType::AST_ARGUMENT) {
            std::string pin_name = id2vl(child->str);
            pin = nullptr;
            if (cell) {
                pin = inst->createInstancePin(pin_name);
            } else {
                pin = inst->createInstancePinWithoutMaster(pin_name);
            }
            if (!pin) {
                message->issueMsg(kError,
                "create pin %s failed for instance %s in module %s.\n",
                        pin_name.c_str(), inst_name.c_str(),
                        hcell->getName().c_str());
                break;
            }
            for (auto child_child : child->children) {
                if (child_child->type ==
                                    Yosys::AST::AstNodeType::AST_IDENTIFIER) {
                    std::string net_name = getIdWithRange(child_child);
                    Net *net = hcell->getNet(net_name);
                    if (!net) {
                        message->issueMsg(kError,
                                "cannot find net %s in module %s.\n",
                                net_name.c_str(), hcell->getName().c_str());
                        break;
                    }
                    pin->setNet(net);
                    net->addPin(pin);
                    break;
                } else if (child_child->type ==
                                    Yosys::AST::AstNodeType::AST_CONCAT) {
                    std::string net_name;
                    for (auto child_child_child : child_child->children) {
                        net_name = getIdWithRange(child_child_child);
                        Net *net = hcell->getNet(net_name);
                        if (!net) {
                            message->issueMsg(kError,
                                    "cannot find net %s in module %s.\n",
                                    net_name.c_str(), hcell->getName().c_str());
                            break;
                        }
                        pin->addNet(net);
                        net->addPin(pin);
                    }
                }
            }
        }
    }
    return true;
}

static bool readVerilogAssignToDB(Cell *hcell,
                                struct Yosys::AST::AstNode *current_ast) {
    bool is_int = false;
    bool is_real = false;
    int  int_constant = 0;
    double real_constant = 0.0;
    std::string left_net_name = "";
    std::string right_net_name = "";
    Net *left_net = nullptr;
    Net *right_net = nullptr;

    if (current_ast->children.size() == 2) {
        left_net_name = getIdWithRange(current_ast->children[0]);
        if (current_ast->children[1]->type
                == Yosys::AST::AstNodeType::AST_IDENTIFIER) {
            right_net_name = getIdWithRange(current_ast->children[1]);
        } else {
            is_int = getIntConstant(current_ast->children[1], int_constant);
            if (!is_int) {
                is_real = getRealConstant(current_ast->children[1],
                                                              real_constant);
                if (!is_real) {
                    return false;
                }
            }
        }
        left_net = hcell->getNet(left_net_name);
        if (!left_net) {
            message->issueMsg(kWarn,
                "cannot find net %s in assign of module %s.\n",
                    left_net_name.c_str(),
                    hcell->getName().c_str());
            return false;
        }
        if (!right_net_name.empty()) {
            right_net = hcell->getNet(right_net_name);
            if (!right_net) {
                message->issueMsg(kWarn,
                    "cannot find net %s in assign of module %s.\n",
                        right_net_name.c_str(),
                        hcell->getName().c_str());
                return false;
            }
            left_net->addAssignNet(right_net->getId());
        } else if (is_int) {
            left_net->addAssignConstant(int_constant);
        } else if (is_real) {
            left_net->addAssignConstant(real_constant);
        } else {
            return false;
        }
    }

    return true;
}
 

static void findMasterForInst() {
    Cell *top_cell = getTopCell();
    for (auto it : kInstMasterMap) {
        Inst *inst = it.first;
        std::string cell_name = it.second;
        Cell *cell = top_cell->getCell(cell_name);
        if (!cell) {
            message->issueMsg(kWarn, "cannot find cell %s.\n",
                              cell_name.c_str());
            continue;
        }

        inst->setMaster(cell->getId());

        ObjectId pins = inst->getPins();
        VectorObject8 *pins_vector = cell->addr<VectorObject8>(pins);
        VectorObject8 *pgpin_vector = nullptr;
        for (int i = 0; i < pins_vector->totalSize(); i++) {
            ObjectId pin_id = (*pins_vector)[i];
            Pin *pin = cell->addr<Pin>(pin_id);
            Term *term = cell->getTerm(pin->getName());
            if (!term) {
                message->issueMsg(kError,
                        "cell %s does't has term %s.\n",
                        cell_name.c_str(), pin->getName().c_str());
                break;
            }
            pin->setTerm(term);
        }
    }
}

bool readVerilogToDB(struct Yosys::AST::AstNode *ast_node) {
    if (!ast_node) {
        message->issueMsg(kError, "input ast node is null\n");
        return false;
    }

    std::queue<Yosys::AST::AstNode*> ast_nodes;
    ast_nodes.push(ast_node);

    Cell *top_cell = getTopCell();
    Cell *current_hcell = nullptr;
    Inst *inst = nullptr;
    std::string module_name;
    struct Yosys::AST::AstNode *current_ast = nullptr;

    for (auto child : ast_node->children) {
        ast_nodes.push(child);
        while (!ast_nodes.empty()) {
            current_ast = ast_nodes.front();
            ast_nodes.pop();
            switch (current_ast->type) {
                case Yosys::AST::AstNodeType::AST_MODULE:
                    module_name = id2vl(current_ast->str);
                    current_hcell = top_cell->createCell(module_name, true);
                    if (!current_hcell) {
                        message->issueMsg(kError,
                                "Create cell failed for module %s.\n",
                                module_name.c_str());
                        MemPool::setCurrentPagePool(getTopCell()->getPool());
                        return false;
                    }
                    for (auto child_child : current_ast->children) {
                        if (child_child->type ==
                                Yosys::AST::AstNodeType::AST_WIRE ||
                            child_child->type ==
                                Yosys::AST::AstNodeType::AST_CELL ||
                            child_child->type ==
                                Yosys::AST::AstNodeType::AST_ASSIGN) {
                            ast_nodes.push(child_child);
                        }
                    }
                    break;
                case Yosys::AST::AstNodeType::AST_WIRE:
                    readVerilogWireToDB(current_hcell, current_ast);
                    break;
                case Yosys::AST::AstNodeType::AST_CELL:
                    readVerilogInstToDB(current_hcell, current_ast);
                    break;
                case Yosys::AST::AstNodeType::AST_ASSIGN:
                    readVerilogAssignToDB(current_hcell, current_ast);
                    break;
                default:
                    break;
            }
        }
    }
    findMasterForInst();
    MemPool::setCurrentPagePool(getTopCell()->getPool());
    return true;
}

}  // namespace db
}  // namespace open_edi
