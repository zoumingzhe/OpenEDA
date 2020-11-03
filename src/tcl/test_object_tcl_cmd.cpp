/**
 * @file  test_object_tcl_cmd.cpp
 * @date  09/03/2020 03:39:44 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "tcl/test_object_tcl_cmd.h"

namespace open_edi {
namespace tcl {

void registerTestObjectCommand(Tcl_Interp *itp) {
    Tcl_CreateCommand(itp, "test_site", siteTest, NULL, NULL);
    Tcl_CreateCommand(itp, "test_ndr_rule", nonDefaultRuleTest, NULL, NULL);
    Tcl_CreateCommand(itp, "test_units", unitsTest, NULL, NULL);
    Tcl_CreateCommand(itp, "test_property_definition", propertyDefinitionTest,
                      NULL, NULL);
    Tcl_CreateCommand(itp, "test_vector_object", vectorObjectTest, NULL, NULL);
    Tcl_CreateCommand(itp, "test_vector_object_perf", vectorObjectPerfTest,
                      NULL, NULL);
}

}  // namespace tcl
}  // namespace open_edi
