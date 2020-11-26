/**
 * @file  read_write.cpp
 * @date
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/io/read_write_db.h"

#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>

#include "util/checksum.h"
#include "util/util.h"

namespace open_edi {
namespace db {

// Class ReadDesign
bool ReadDesign::__readSymFile() {
    std::string sym_file = cell_name_;
    sym_file.append(kSymFilePostFix);
    symbol_table_ = new SymbolTable;
    std::ifstream in_symfile(sym_file.c_str(), std::ifstream::binary);
    if (!in_symfile.good()) {
        message->issueMsg(kError, "Failed to open symbol file %s.\n",
                          sym_file.c_str());
        return false;
    }
    in_symfile.seekg(0, in_symfile.beg);
    symbol_table_->readFromFile(in_symfile, getDebug());
    in_symfile.close();
    return true;
}

bool ReadDesign::__readPolyFile() {
    std::string poly_file = cell_name_;
    poly_file.append(kPolyFilePostFix);
    polygon_table_ = new PolygonTable;
    std::ifstream in_polyfile(poly_file.c_str(), std::ifstream::binary);
    if (!in_polyfile.good()) {
        message->issueMsg(kError, "Failed to open symbol file %s.\n",
                          poly_file.c_str());
        return false;
    }
    in_polyfile.seekg(0, in_polyfile.beg);
    polygon_table_->readFromFile(in_polyfile, getDebug());
    in_polyfile.close();
    return true;
}

bool ReadDesign::__readDBFile() {
    std::string db_file = cell_name_;
    db_file.append(kDBFilePostFix);
    // open:
    std::ifstream in_dbfile(db_file.c_str(), std::ifstream::binary);
    if (!in_dbfile) {
        message->issueMsg(kError, "Failed to open design file %s.\n",
                          db_file.c_str());
        return false;
    }
    in_dbfile.seekg(0, in_dbfile.beg);
    // read version:
    v_.readFromFile(in_dbfile, getDebug());
    // read into mem pool:
    size_t pool_id = 0;
    // TODO(luoying): pool_id is unused in object ID.
    in_dbfile.read(reinterpret_cast<char *>(&(pool_id)), sizeof(size_t));
    in_dbfile.read(reinterpret_cast<char *>(&(top_cell_id_)), sizeof(ObjectId));
    Cell *top_cell = getTopCell();
    if (top_cell) {
        MemPool::destroyMemPool();  // here, we need to set the right mempool
                                    // for the new db.
        resetTopCell();
    }
    MemPool::initMemPool();
    pool_ = MemPool::newPagePool();
    pool_->readFromFile(in_dbfile, getDebug());
    if (getDebug()) {
        pool_->printUsage();
    }
    // read checksum:
    CheckSum csum;
    uint32_t file_header_size = 0;
    uint32_t ref_value = 0;
    in_dbfile.read(reinterpret_cast<char *>(&file_header_size),
            sizeof(uint32_t));
    in_dbfile.read(reinterpret_cast<char *>(&ref_value), sizeof(uint32_t));
    // close:
    in_dbfile.close();
    // check checksum:
    uint32_t sum = csum.summary(db_file, file_header_size, getDebug());
    if (csum.check(sum, ref_value, getDebug())) {
        if (getDebug()) {
            std::cout << "Succeeded in checksum.\n";
        }
    } else {
        std::cout << "Failed in checksum: " << sum << " VS ref_value "
                  << ref_value << std::endl;
        return false;
    }

    return true;
}

bool ReadDesign::__setCurrentTopCell() {
    // set top cell and its tables.
    setTopCell(top_cell_id_);
    Cell *top_cell = getTopCell();
    top_cell->setPool(pool_);
    top_cell->setSymbolTable(symbol_table_);
    top_cell->setPolygonTable(polygon_table_);
    setCurrentVersion(v_);
    if (getDebug()) {
        std::cout << "DEBUGINFO: read_design top cell name "
                  << top_cell->getName() << " with id#" << top_cell->getId()
                  << std::endl;
    }
    if (!top_cell || !top_cell->getPool()) {
        message->issueMsg(kError, "Failed to init top cell.\n");
        return false;
    }

    return true;
}

int ReadDesign::run() {
    if (!__readSymFile() || !__readPolyFile() || !__readDBFile() ||
        !__setCurrentTopCell()) {
        return ERROR;
    }
    std::cout << "INFO: Successfully read design " << cell_name_ << ".\n";
    return OK;
}

// Class WriteDesign
bool WriteDesign::__writeDBFile() {
    std::string db_file = saved_name_;
    db_file.append(kDBFilePostFix);
    // open:
    std::ofstream out_dbfile(db_file.c_str(), std::ofstream::binary);
    if (out_dbfile.good() == false) {
        message->issueMsg(kError, "Failed to open output db file %s.\n",
                          db_file.c_str());
        return false;
    }
    // write version:
    Version &v = getCurrentVersion();
    v.writeToFile(out_dbfile, getDebug());
    // check cell name:
    top_cell_ = getTopCell();
    cell_name_ = top_cell_->getName();
    if (cell_name_.compare(saved_name_) != 0) {
        if (getDebug()) {
            std::cout << "DEBUGINFO: rename top cell name " << cell_name_
                      << " to " << saved_name_ << std::endl;
        }
        top_cell_->setName(saved_name_);
    }
    // write mem pool:
    MemPagePool *pool = top_cell_->getPool();
    size_t pool_id = pool->getPoolNo();
    ObjectId top_cell_id = top_cell_->getId();
    out_dbfile.write(reinterpret_cast<char *>(&pool_id), sizeof(size_t));
    out_dbfile.write(reinterpret_cast<char *>(&top_cell_id), sizeof(ObjectId));
    pool->writeHeaderToFile(out_dbfile, getDebug());
    uint32_t file_header_size = out_dbfile.tellp();
    pool->writeContentToFile(out_dbfile, getDebug());
    out_dbfile.flush();
    // write checksum:
    CheckSum csum;
    uint32_t sum = csum.summary(db_file, file_header_size, getDebug());
    out_dbfile.write(reinterpret_cast<char *>(&file_header_size),
                                                     sizeof(uint32_t));
    out_dbfile.write(reinterpret_cast<char *>(&sum), sizeof(uint32_t));
    // close:
    out_dbfile.close();
    if (getDebug()) {
        pool->printUsage();
    }
    return true;
}

bool WriteDesign::__writePolyFile() {
    std::string poly_file = saved_name_;
    poly_file.append(kPolyFilePostFix);
    PolygonTable *polygon_table = top_cell_->getPolygonTable();
    std::ofstream out_polyfile(poly_file.c_str(), std::ofstream::binary);
    if (out_polyfile.good() == false) {
        message->issueMsg(kError, "Failed to open output polygon file %s.\n",
                          poly_file.c_str());
        return false;
    }
    polygon_table->writeToFile(out_polyfile, getDebug());
    out_polyfile.close();
    return true;
}

bool WriteDesign::__writeSymFile() {
    std::string sym_file = saved_name_;
    sym_file.append(kSymFilePostFix);
    SymbolTable *symbol_table = top_cell_->getSymbolTable();
    std::ofstream out_symfile(sym_file.c_str(), std::ofstream::binary);
    if (out_symfile.good() == false) {
        message->issueMsg(kError, "Failed to open output symbol file %s.\n",
                          sym_file.c_str());
        return false;
    }
    symbol_table->writeToFile(out_symfile, getDebug());
    out_symfile.close();
    return true;
}

bool WriteDesign::__setAfterWrite() {
    if (cell_name_.compare(saved_name_) != 0) {
        top_cell_->setName(cell_name_);
    }
    return true;
}

int WriteDesign::run() {
    if (!__writeDBFile()) {
        return ERROR;
    }

    if (!__writePolyFile()) {
        return ERROR;
    }

    if (!__writeSymFile()) {
        return ERROR;
    }

    if (!__setAfterWrite()) {
        return ERROR;
    }
    std::cout << "INFO: Successfully write design " << saved_name_ << ".\n";
    return OK;
}

}  // namespace db
}  // namespace open_edi
