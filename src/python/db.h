/**
 * File              : db.h
 * Author            : Yibo Lin <yibolin@pku.edu.cn>
 * Date              : 04.24.2020
 * Last Modified Date: 04.24.2020
 * Last Modified By  : Yibo Lin <yibolin@pku.edu.cn>
 */

#ifndef EDI_PYTHON_DB_H_
#define EDI_PYTHON_DB_H_

#include "db/core/db.h"
#include "python/util.h"

using Object = EDI_NAMESPACE::Object;
using ObjectAttr = EDI_NAMESPACE::ObjectAttr;
// using AttrObject = EDI_NAMESPACE::AttrObject;

using Database = EDI_NAMESPACE::Database;

using Design = EDI_NAMESPACE::Design;

using InstAttr = EDI_NAMESPACE::InstAttr;
using NetAttr = EDI_NAMESPACE::NetAttr;
using PinAttr = EDI_NAMESPACE::PinAttr;

using Inst = EDI_NAMESPACE::Inst;
using Net = EDI_NAMESPACE::Net;
using Pin = EDI_NAMESPACE::Pin;

using Cell = EDI_NAMESPACE::Cell;
using Term = EDI_NAMESPACE::Term;

#endif
