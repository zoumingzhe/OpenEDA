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
    Sdc();
    Sdc(ObjectId mode_id);

    //init
    void initId();
    void initPtr();

    //get
    const ObjectId getAnalysisModeId() const { return analysis_mode_id_; }
    const ObjectId getAnalysisViewId() const { return analysis_view_id_; }
    const ObjectId getAnalysisCornerId() const { return analysis_corner_id_; }
    const ObjectId getMainLibertyId() const { return main_liberty_id_; }
    const ObjectId getMainLibertyUnitsId() const { return main_liberty_units_id_; }

    //get
    SdcCaseAnalysisContainerPtr getCaseAnalysisContainer() { return case_analysis_container_; }
    SdcCurrentDesignContainerPtr getDesignContainer() { return design_container_; }

  private:
    SdcCaseAnalysisContainerPtr case_analysis_container_;
    SdcCurrentDesignContainerPtr design_container_;

  private:
    ObjectId analysis_mode_id_;
    ObjectId analysis_view_id_;
    ObjectId analysis_corner_id_;
    ObjectId main_liberty_id_;
    ObjectId main_liberty_units_id_;
};
using SdcPtr = std::shared_ptr<Sdc>;

}
}


#endif // EDI_DB_TIMING_SDC_SDC_H_
