/** 
 * @file  checksum.h
 * @date  
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_UTIL_CHECKSUM_H_
#define EDI_UTIL_CHECKSUM_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <boost/filesystem.hpp>

namespace open_edi {
namespace util {

class CheckSum {
  public:
    uint32_t summary(const std::string & filename, int len, bool debug = false);
    uint32_t summary(const unsigned char *buff, int len, bool debug = false);
    bool check(uint32_t sum_value, uint32_t ref_value, bool debug = false);

  private:
    uint16_t __from32to16(uint32_t x);
    //DATA
};

}  // namespace util
}  // namespace open_edi

#endif


