/** 
* @file sdc.cpp
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

#include "db/timing/sdc/sdc.h"
#include "db/timing/timinglib/libset.h"
#include "db/timing/timinglib/analysis_view.h"
#include "db/timing/timinglib/analysis_corner.h"
#include "db/timing/timinglib/analysis_mode.h"
#include "db/timing/timinglib/timinglib_lib.h"

namespace open_edi {
namespace db {


Sdc::Sdc(ObjectId mode_id) : analysis_mode_id_(mode_id) {
    initIds();
}

void Sdc::initIds() {
    AnalysisMode* mode = Object::addr<AnalysisMode>(analysis_mode_id_);
    if (mode == nullptr) {
        //TODO error messages
        return;
    }
    analysis_view_id_ = mode->getOwnerId(); 
    AnalysisView* view = Object::addr<AnalysisView>(analysis_view_id_);
    if (view == nullptr) {
        //TODO error messages
        return;
    }
    AnalysisCorner* corner = view->getAnalysisCorner();
    if (corner == nullptr) {
        //TODO error messages
        return;
    }
    analysis_corner_id_ = corner->getId(); 
    LibSet* lib_set = corner->getLibset();
    if (lib_set == nullptr) {
        //TODO error messages 
        return;
    }
    //TODO it always for loop to get the lib vector in libset.cpp, need just the first
    const std::vector<TLib*>& lib_vec = lib_set->getTimingLibs();
    if (lib_vec.size() == 0) {
        //TODO error messages
        return;
    }
    TLib* lib =  lib_vec.front(); 
    if (lib == nullptr) {
        //TODO error messages
        return;
    }
    main_liberty_id_ = lib->getId();
    TUnits* units = lib->getUnits();
    main_liberty_units_id_ = units->getId();
}



}
}
