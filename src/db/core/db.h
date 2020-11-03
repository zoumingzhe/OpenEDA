/* @file  db.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_DB_DB_H_
#define EDI_DB_DB_H_

// include all headers in the directory
#include "db/core/attr_object.h"
#include "db/core/object.h"
#include "db/core/object_attr.h"
// model
#include "db/core/cell.h"
#include "db/core/term.h"
#include "db/core/bus.h"
// design
#include "db/core/inst.h"
#include "db/core/inst_attr.h"
#include "db/core/net.h"
#include "db/core/pin.h"
#include "db/core/pin_attr.h"
#include "db/core/special_net.h"

namespace open_edi {
namespace db {

bool initTopCell();
void resetTopCell();
Cell* getTopCell();
void setTopCell(ObjectId top_cell_id);
Cell* getCell();
Box* creatBox();
Box* creatBox(int llx, int lly, int urx, int ury);

using Version = open_edi::util::Version;
Version& getCurrentVersion();
void setCurrentVersion(Version& v);

}  // namespace db
}  // namespace open_edi
#endif
