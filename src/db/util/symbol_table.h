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

#ifndef _SYMBOLTABLE_H_
#define _SYMBOLTABLE_H_

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "db/util/symbol_page.h"
#include "util/util.h"
#include "db/core/object.h"

namespace open_edi {
namespace db {

using SymbolIndex = open_edi::util::SymbolIndex;
using MemPool = open_edi::util::MemPool;

const uint64_t kInvalidSymbolIndex = 0;

class SymbolTable {
  public:
    template <class T>
    T *getObjectByTypeAndName(ObjectType type, std::string &name);
    SymbolIndex isSymbolInTable(std::string name);
    SymbolIndex getOrCreateSymbol(const char *name, bool check = true);

    std::string &getSymbolByIndex(SymbolIndex index);
    uint64_t getSymbolCount();

    bool insertReference(const char *name, ObjectId owner);
    bool addReference(SymbolIndex index, ObjectId owner);
    bool removeReference(SymbolIndex symbol_index, ObjectId owner);
    bool eliminateReferences();
    std::vector<ObjectId> &getReferences(SymbolIndex index);

    SymbolTable(/* args */);
    ~SymbolTable();

    void writeToFile(IOManager &io_manager, bool debug = false);
    void readFromFile(IOManager &io_manager, bool debug = false);

    template <typename T>
    class referenceIterator {
      public:
        referenceIterator() {};

        referenceIterator& operator++() {
            ++iter_;
            return *this;
        }

        referenceIterator operator++(int) {
            referenceIterator tmp = *this;
            iter_++;
            return tmp;
        };

        T* operator*() {
            T* o = MemPool::getObjectPtr<T>(*iter_);
            if (o && o->getIsValid()) return o;

            while (iter_ != vec_.end()) {
                iter_++;
                o = MemPool::getObjectPtr<T>(*iter_);
                if (o && o->getIsValid()) return o;
            }
            return nullptr;
        };

        referenceIterator& begin(SymbolTable *table, SymbolIndex &index) {
            vec_ = table->getReferences(index);
            iter_ = vec_.begin();
            return *this;
        }

        referenceIterator& begin(SymbolTable *table, std::string &symbol) {
            SymbolIndex index = table->getOrCreateSymbol(symbol.c_str());
            return begin(table, index);
        }

        bool end() {return (vec_.empty() || iter_ == vec_.end());}

      private:
        std::vector<ObjectId> vec_;
        std::vector<ObjectId>::iterator iter_;
    };

  private:
    std::vector<SymbolPage *> symbol_pages_;
    std::vector<long> non_reference_symbols_;
    std::unordered_map<std::string, SymbolIndex> hash_;
    uint64_t page_count_;
    uint64_t symbol_count_;
};


template <class T>
T *SymbolTable::getObjectByTypeAndName(ObjectType type, std::string &name) {
    referenceIterator<T> iter;
    iter.begin(this, name);
    for (; !iter.end(); iter++) {
        T* o = *iter;
        if (o && o->getIsValid() && o->getObjectType() == type) {
            return o;
        }
    }

    return nullptr;
}

}  // namespace db 
}  // namespace open_edi

#endif
