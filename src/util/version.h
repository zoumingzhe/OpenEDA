/** 
 * @file  version.h
 * @date  09/27/2020 08:14:01 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_DB_TECH_VERSION_H_
#define EDI_DB_TECH_VERSION_H_

#include "util/util.h"

namespace open_edi {
namespace util {

class Version {
  public:
    Version();
    ~Version();
    
    void init();
    void reset();
    void set(Version & v);
    const std::string &getVersionString();
    void writeToFile(std::ofstream & outfile, bool debug = false);
    void readFromFile(std::ifstream & infile, bool debug = false);
    
  private:
    const char kHeaderChar = 'r';
    const char kVersionDelimiter = '.';

    int major_;
    int minor_;
    int revision_;
    std::string version_string_;
};

}  // namespace util
}  // namespace open_edi

#endif


