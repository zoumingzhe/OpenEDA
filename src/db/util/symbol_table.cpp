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

#include "db/util/symbol_table.h"
#include <fstream>
#include <iostream>

namespace open_edi {
namespace db {

using namespace open_edi::util;

/// @brief SymbolTable 
SymbolTable::SymbolTable(/* args */)
{
    symbol_count_ = 0;
    symbol_pages_.push_back(new(SymbolPage));
    page_count_ = 1;

}

/// @brief ~SymbolTable 
SymbolTable::~SymbolTable()
{
    for (auto &sp : symbol_pages_) {
        delete sp;
    }
    symbol_pages_.clear();
    non_reference_symbols_.clear();
    hash_.clear();
    symbol_count_ = 0;
    page_count_ = 0;
}

/// @brief isSymbolInTable 
///
/// @param name
///
/// @return symbol index upon success, otherwise -1.
int64_t SymbolTable::isSymbolInTable(std::string name)
{
    std::unordered_map<std::string, SymbolIndex>::const_iterator
        got = hash_.find(name);
    return ((got != hash_.end()) ? got->second : -1);
}

/// @brief getOrCreateSymbol 
///
/// @param name
/// @param no_check
///
/// @return SymbolIndex, -1 upon failure.
SymbolIndex SymbolTable::getOrCreateSymbol(const char *name, bool check)
{
    int32_t array_index = 0;
    int64_t symbol_index = -1;

    if (check) {
        symbol_index = isSymbolInTable(name);
        if (symbol_index != -1) {
            return symbol_index;
        }
    }

    int current_page = symbol_pages_.size()-1;

    if (current_page < 0) {
        message->issueMsg(kError, "There are not symbol pages.\n");
        return symbol_index;
    }

    if (symbol_pages_[current_page]->size() == SYMTBL_ARRAY_SIZE) {
        symbol_pages_.push_back(new(SymbolPage));
        page_count_++;
        current_page++;
    }

    array_index = symbol_pages_[current_page]->addSymbol(name);
    symbol_index = current_page*SYMTBL_ARRAY_SIZE + array_index;
    hash_.insert({name,symbol_index});
    symbol_count_++;

    return symbol_index;
}

/// @brief getSymbolByIndex 
///
/// @param index
///
/// @return 
std::string &SymbolTable::getSymbolByIndex(SymbolIndex index)
{
    static std::string kSymtblDft = std::string("");

    if ((index<0) || (index>symbol_count_))
    {
        return kSymtblDft;
    }

    int64_t page_num = index/SYMTBL_ARRAY_SIZE;
    int32_t array_num = index%SYMTBL_ARRAY_SIZE;

    return symbol_pages_[page_num]->getSymbol(array_num);
}

/// @brief getSymbolCount 
///
/// @return 
uint64_t SymbolTable::getSymbolCount()
{
    return symbol_count_;
}

/// @brief insertReference 
///
/// @param name
/// @param owner
///
/// @return 
bool SymbolTable::insertReference(const char *name, ObjectId owner)
{
    int64_t index = getOrCreateSymbol(name);
    if (index == -1) return false;

    int page_num = index/SYMTBL_ARRAY_SIZE;
    int array_num = index%SYMTBL_ARRAY_SIZE;
    symbol_pages_[page_num]->addSymbolReference(array_num, owner);

    return 1;
}

bool SymbolTable::addReference(SymbolIndex index, ObjectId owner)
{
    if (index == -1) return false;

    int page_num = index/SYMTBL_ARRAY_SIZE;
    int array_num = index%SYMTBL_ARRAY_SIZE;
    
    symbol_pages_[page_num]->addSymbolReference(array_num, owner);

    return 1;
}

/// @brief removeReference 
///
/// @param symbol_index
/// @param owner
///
/// @return 
bool SymbolTable::removeReference(SymbolIndex symbol_index, ObjectId owner)
{
    if ((symbol_index < 0) || (symbol_index > symbol_count_))
    {
        return false;
    }

    int64_t page_num = symbol_index/SYMTBL_ARRAY_SIZE;
    int32_t array_num = symbol_index%SYMTBL_ARRAY_SIZE;

    bool removed = symbol_pages_[page_num]->removeSymbolReference(array_num, owner);

    if (removed && symbol_pages_[page_num]->getReferenceCount(array_num) == 0) {
        non_reference_symbols_.push_back(symbol_index);
    }
    
    return removed;
}

/// @brief eliminateReferences 
///
/// @return 
bool SymbolTable::eliminateReferences()
{
    int64_t page_num;
    int32_t array_num;

    for (long i = 0; i < symbol_count_; i++)
    {
        page_num = i/SYMTBL_ARRAY_SIZE;
        array_num = i%SYMTBL_ARRAY_SIZE;

        if (symbol_pages_[page_num]->getReferenceCount(array_num) == 0)
            non_reference_symbols_.push_back(i);
    }
    return non_reference_symbols_.size();
}

std::vector<ObjectId> &SymbolTable::getReferences(SymbolIndex index)
{
    ediAssert(index != -1); 
    int page_num = index/SYMTBL_ARRAY_SIZE;
    int array_num = index%SYMTBL_ARRAY_SIZE;
    return symbol_pages_[page_num]->getReferences(array_num);
}
/// @brief  
///
/// @return 
void SymbolTable::writeToFile(std::ofstream & outfile, bool debug)
{
    if (!outfile) {
        return;
    }
    //1. symbol pages count + symbols count
    outfile.write((char *) &(page_count_), sizeof(uint64_t));
    outfile.write((char *) &(symbol_count_), sizeof(uint64_t));

    //2. write page one by one:
    for (auto &sp : symbol_pages_) {
      sp->writeToFile(outfile, debug);
    }
}

/// @brief  readFromFile
///
/// @return 
void SymbolTable::readFromFile(std::ifstream & infile, bool debug)
{
    if (!infile) {
        return;
    }
    //1. symbol pages count + symbols count
    infile.read((char *) &(page_count_), sizeof(uint64_t));
    infile.read((char *) &(symbol_count_), sizeof(uint64_t));
    //2. fill page info:
    for (int32_t i = 0; i < page_count_; ++i) {
        if (i != 0) {
            SymbolPage * symbol_page = new SymbolPage;
            symbol_page->readFromFile(infile, debug);
            symbol_pages_.push_back(symbol_page);
        } else {
            //during initialization, one page has been allocated.
            SymbolPage * symbol_page = symbol_pages_[0]; 
            symbol_page->readFromFile(infile, debug);
        }
    }
    //3. fill hash info:
    for (int32_t i = 0; i < page_count_; ++i) { //i is page idx
        SymbolPage * symbol_page = symbol_pages_[i];
        for (int32_t j = 0; j < symbol_page->size(); ++j) { //j is array idx
            std::string &symbol_name = symbol_page->getSymbol(j);
            uint64_t symbol_index = i*SYMTBL_ARRAY_SIZE + j;
            hash_.insert({symbol_name.c_str(),symbol_index});
        }
    }    
}

}  // namespace db 
}  // namespace open_edi
