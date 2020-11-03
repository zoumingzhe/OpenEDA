/* @file  read_verilog.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_DB_IO_READ_VERILOG_H_
#define SRC_DB_IO_READ_VERILOG_H_

#include "frontends/ast/ast.h"

namespace open_edi {
namespace db {
    int readVerilog(int argc, const char **argv);
    bool readVerilogToDB(Yosys::AST::AstNode *ast_node);
}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_IO_READ_VERILOG_H_
