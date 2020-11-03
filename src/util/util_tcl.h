/* @file  util_tcl.hpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
/// @file utilTcl.hpp

#ifndef __EDI_UTIL_TCL_HPP__
#define __EDI_UTIL_TCL_HPP__

#include <map>
#include <tcl.h>
#include "util/namespace.h"

namespace open_edi {
namespace util {

class TclMgr {
public:
    TclMgr() {}
    ~TclMgr() {}
public:
    //std::map<int tid, Tcl_Interp *> itps;
private:
};

}  // namespace util
}  // namespace open_edi

#endif
