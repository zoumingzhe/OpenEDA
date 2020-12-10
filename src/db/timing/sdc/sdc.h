/** 
 * @file sdc.h
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

#ifndef EDI_DB_TIMING_SDC_SDC_H_
#define EDI_DB_TIMING_SDC_SDC_H_

#include "db/timing/sdc/sdc_data.h"

namespace open_edi {
namespace db {

class Sdc {
  public:
    //TODO construct function to init smart pointers
    Sdc(ObjectId mode_id);

    //init
    void init();

    //get
    ObjectId getAnalysisModeId() { return analysis_mode_id_; }
    ObjectId getAnalysisViewId() { return analysis_view_id_; }
    ObjectId getAnalysisCornerId() { return analyais_corner_id_; }
    ObjectId getMainLibertyId() { return main_liberty_id_; }
    ObjectId getMainLibertyUnitsId() { return main_liberty_units_id_; }

    //set
    void set_case_analysis_container(SdcCaseAnalysisContainerPtr ptr) { case_analysis_container_ptr_ = ptr; }


    //get
    SdcCaseAnalysisContainerPtr get_case_analysis_container() { return case_analysis_container_ptr_; }

  private:
    SdcCaseAnalysisContainerPtr case_analysis_container_ptr_;

  private:
    ObjectId analysis_mode_id_;
    ObjectId analysis_view_id_;
    ObjectId analyais_corner_id_;
    ObjectId main_liberty_id_;
    ObjectId main_liberty_units_id_;
};
using SdcPtr = std::shared_ptr<Sdc>;

}
}


#endif // EDI_DB_TIMING_SDC_SDC_H_
