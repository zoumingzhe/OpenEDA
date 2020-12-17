/** 
 * @file object_access_commands.cpp
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

#include "db/timing/sdc/object_access_commands.h"
#include "db/core/db.h"

namespace open_edi {
namespace db {


CurrentDesign::CurrentDesign() {
    init();
}

void CurrentDesign::init() {
    Cell *top_cell = getTopCell();
    if (!top_cell) {
        //error messages
        return;
    }
    cell_id_ = top_cell->getId();
}

bool CurrentDesign::switchToCell(const std::string &cell_name) const { //sdc2.1 not support
    if (cell_name == "") {
        //keep on top cell
        return true;
    }
    return false;
}


}
}

