/**
 * @file  test_object_tcl_cmd.h
 * @date  08/21/2020 06:47:53 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef EDI_TCL_TEST_OBJECT_H_
#define EDI_TCL_TEST_OBJECT_H_
#include <tcl.h>

#include "db/core/db.h"
#include "db/tech/ndr_rule.h"
#include "db/tech/property.h"
#include "db/tech/site.h"
#include "db/tech/units.h"
#include "db/util/property_definition.h"
#include "util/util.h"

namespace open_edi {
namespace tcl {

using namespace open_edi::db;
// NDR Rule
bool parseNonDefaultRuleArgument(int argc, const char *argv[],
                                 NonDefaultRule &testObj);
int nonDefaultRuleTest(ClientData cld, Tcl_Interp *itp, int argc,
                       const char *argv[]);
// Property Definition
bool parsePropertyDefinitionArgument(
    int argc, const char *argv[],
    std::vector<PropertyDefinition *> &test_objects);
int propertyDefinitionTest(ClientData cld, Tcl_Interp *itp, int argc,
                           const char *argv[]);
// Site:
bool parseSiteArgument(int argc, const char *argv[], Site &testObj);
int siteTest(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
// Units:
bool parseUnitsArgument(int argc, const char *argv[], Units &testObj);
int unitsTest(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]);
// VectorObject:
bool parseVectorObjectArgument(int argc, const char *argv[], int *size);
int vectorObjectTest(ClientData cld, Tcl_Interp *itp, int argc,
                     const char *argv[]);
//
int vectorObjectPerfTest(ClientData cld, Tcl_Interp *itp, int argc,
                         const char *argv[]);

// registration:
void registerTestObjectCommand(Tcl_Interp *itp);

}  // namespace tcl
}  // namespace open_edi

#endif
