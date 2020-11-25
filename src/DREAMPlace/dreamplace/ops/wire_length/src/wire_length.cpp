/* @file  wire_length.cpp
 * @date  Nov 2020
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NiiCEDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
/*************************************************************************/                         
#include "wire_length.h"
#include "utility/src/DetailedPlaceDB.h"

DREAMPLACE_BEGIN_NAMESPACE


double WireLength::calcHPWLInCPU() 
{
  double wl = 0.0;
  return wl;
}
double WireLength::calcHPWL() 
{
  if (isGPU()) {
    return calcHPWLInGPU();
  } else {
    return calcHPWLInCPU();
  } 
}
double WireLength::calcMST() 
{
  
  double wl = 0.0; 
  return wl;
}

void WireLength::run() 
{
  double total_length = 0.0;
  if (getType() == kHPWL) {
    total_length = calcHPWL();
  } else if (getType() == kMST) {
    total_length = calcMST();
  }
  dreamplacePrint(kINFO, "Total wire lenght(%s) : %.6f nm\n", 
                  total_length, kWLTypeName[getType()].c_str());
}

DREAMPLACE_END_NAMESPACE
