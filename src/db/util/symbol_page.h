/** 
 * @file  symbol_page.h
 * @date  Sep 7, 2020
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef _SYMBOLPAGE_H_
#define _SYMBOLPAGE_H_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <array>

#include "util/util.h"

namespace open_edi {
namespace db {

using ObjectId = open_edi::util::ObjectId;

#define SYMTBL_ARRAY_SIZE 4096

class SymbolPage
{
public:
    SymbolPage();
    ~SymbolPage();

    int32_t size();

    int32_t addSymbol(const char *name);
    bool addSymbol(int32_t index, const char *name);
    int32_t addSymbol(std::string &name);
    bool addSymbol(int32_t index, std::string &name);

    std::string &getSymbol(int32_t index);

    bool addSymbolReference(int32_t index, ObjectId ref);
    bool removeSymbolReference(int32_t index, ObjectId ref);

    int32_t getReferenceCount(int32_t index);
    std::vector<ObjectId> &getReferences(int32_t index);

    void writeToFile(std::ofstream &outfile, bool debug = false);
    void readFromFile(std::ifstream &infile, bool debug = false);

  private:
    std::array<std::string, SYMTBL_ARRAY_SIZE> symbols_;
    std::array<std::vector<ObjectId>,SYMTBL_ARRAY_SIZE> references_;
    uint32_t symbols_size_;
};

}  // namespace db 
}  // namespace open_edi
#endif
