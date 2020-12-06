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
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <string>

#include "util/checksum.h"
#include "util/util.h"
#include "util/io_handler.h"

namespace open_edi {
namespace db {

// Class ReadDesign
bool ReadDesign::__preWork() {
    if (is_top_) {
        Cell *top_cell = getTopCell();
        if (top_cell) {
            resetTopCell();
            MemPool::destroyMemPool();
        }
        MemPool::initMemPool();
    }
    return true;
}

bool ReadDesign::__readSymFile(
    SymbolTable *symbol_table, 
    std::string &filename
) {
    std::string sym_file = filename;
    sym_file.append(kSymFilePostFix);
    std::ifstream in_symfile(sym_file.c_str(), std::ifstream::binary);
    if (!in_symfile.good()) {
        util::message->issueMsg(kMsgCategoryDB, 
            OpenFileError, kError, sym_file.c_str());
        //std::cout << "Failed to open sym file " << sym_file << ".\n";
        return false;
    }
    in_symfile.seekg(0, in_symfile.beg);
    symbol_table->readFromFile(in_symfile, getDebug());
    in_symfile.close();
    return true;
}

bool ReadDesign::__readPolyFile(
    PolygonTable *polygon_table, 
    std::string &filename
) {
    std::string poly_file = filename;
    poly_file.append(kPolyFilePostFix);
    std::ifstream in_polyfile(poly_file.c_str(), std::ifstream::binary);
    if (!in_polyfile.good()) {
        util::message->issueMsg(kMsgCategoryDB, 
            OpenFileError, kError, poly_file.c_str());
        //std::cout << "Failed to open poly file " << poly_file << ".\n";
        return false;
    }
    in_polyfile.seekg(0, in_polyfile.beg);
    polygon_table->readFromFile(in_polyfile, getDebug());
    in_polyfile.close();
    return true;
}

bool ReadDesign::__readDBFile(
    MemPagePool *pool, 
    std::string &filename
) {
    std::string db_file = filename;
    db_file.append(kDBFilePostFix);
    // open:
    std::ifstream in_dbfile(db_file.c_str(), std::ifstream::binary);
    if (!in_dbfile) {
        util::message->issueMsg(kMsgCategoryDB, 
            OpenFileError, kError, db_file.c_str());
        //std::cout << "Failed to open DB file " << db_file << ".\n";
        return false;
    }
    in_dbfile.seekg(0, in_dbfile.beg);
    // read version:
    v_.readFromFile(in_dbfile, getDebug());
    // read into mem pool:
    size_t pool_id = 0;
    // TODO(luoying): pool_id is unused in object ID.
    in_dbfile.read(reinterpret_cast<char *>(&(pool_id)), sizeof(size_t));
    in_dbfile.read(reinterpret_cast<char *>(&(current_id_)), sizeof(ObjectId));

    //pool_ = MemPool::newPagePool();
    MemPool::insertPagePool(current_id_, pool);
    pool->readFromFile(in_dbfile, getDebug());
    if (getDebug()) {
        pool->printUsage();
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
          util::message->issueMsg(kMsgCategoryDB, 
                CheckSumOk, kInfo);          
            //std::cout << "Succeeded in checksum.\n";
        }
    } else {
        util::message->issueMsg(kMsgCategoryDB, 
                CheckSumError, kError, sum, ref_value);
        //std::cout << "Failed in checksum: " << sum << " VS ref_value "
                  //<< ref_value << std::endl;
        return false;
    }

    return true;
}

bool ReadDesign::__postWork() {
    if (getDebug() && is_top_) {
        Cell *top_cell = getTopCell();
        Tech *tech_lib = getTechLib();
        Timing *timing_lib = getTimingLib();
        ediAssert(top_cell != nullptr && 
                  tech_lib != nullptr &&
                  timing_lib != nullptr);
        util::message->issueMsg(kMsgCategoryDB, 
                ReadDesignOkVerbose, kDebug,
                top_cell->getName().c_str(),
                top_cell->getId(),
                tech_lib->getId(),
                static_cast<int>(tech_lib->getObjectType()),
                timing_lib->getId(),
                static_cast<int>(timing_lib->getObjectType()));  
    }
    return true;
}

bool ReadDesign::__readCell() {
    std::string dirname = cell_name_;
    std::string filename(dirname);
    filename.append("/");
    filename.append(cell_name_);

    StorageUtil *storage_util = new StorageUtil(0);
    if (!__readSymFile(storage_util->getSymbolTable(), filename) ||
        !__readPolyFile(storage_util->getPolygonTable(), filename) ||
        !__readDBFile(storage_util->getPool(), filename)) {
        delete storage_util;
        return false;
    }
    if (is_top_) {
        setTopCell(current_id_);
        setCurrentVersion(v_);
    }
    Cell *read_cell = (Cell *)Object::addr<Cell>(current_id_);
    ediAssert(read_cell != nullptr);
    read_cell->setStorageUtil(storage_util);
    if (!read_cell->getPool()) {
        util::message->issueMsg(kMsgCategoryDB, 
                ReadDesignInitError, kError, "cell");
        //std::cout << "Failed to init cell.\n";
        delete storage_util;
        return false;
    }
    return true;    
}

bool ReadDesign::__readTechLib() {
    if (!is_top_) {
        return true;
    }
    std::string dirname = cell_name_;
    dirname.append(kLibSubDirName);
    std::string filename(dirname);
    filename.append("/");
    filename.append(kTechLibName);

    StorageUtil *storage_util = new StorageUtil(0);
    if (!__readSymFile(storage_util->getSymbolTable(), filename) ||
        !__readPolyFile(storage_util->getPolygonTable(), filename) ||
        !__readDBFile(storage_util->getPool(), filename)) {
        delete storage_util;
        return false;
    }
    setTechLib(current_id_);
    Tech *tech_lib = getTechLib();
    if (!tech_lib) {
        util::message->issueMsg(kMsgCategoryDB, 
                ReadDesignInitError, kError, "tech lib");      
        //std::cout << "Failed to init tech lib.\n";
        delete storage_util;
        return false;        
    }
    tech_lib->setStorageUtil(storage_util);
    return true;
}

bool ReadDesign::__readTimingLib() {
    if (!is_top_) {
        return true;
    }
    std::string dirname = cell_name_;
    dirname.append(kLibSubDirName);
    std::string filename(dirname);
    filename.append("/");
    filename.append(kTimingLibName);

    StorageUtil *storage_util = new StorageUtil(0);
    if (!__readSymFile(storage_util->getSymbolTable(), filename) ||
        !__readPolyFile(storage_util->getPolygonTable(), filename) ||
        !__readDBFile(storage_util->getPool(), filename)) {
        delete storage_util;
        return false;
    }
    setTimingLib(current_id_);
    Timing *timing_lib = getTimingLib();
    if (!timing_lib) {
        util::message->issueMsg(kMsgCategoryDB, 
                ReadDesignInitError, kError, "timing lib");      
        //std::cout << "Failed to init timing lib.\n";
        delete storage_util;
        return false;        
    }
    timing_lib->setStorageUtil(storage_util);
    return true;    
}

int ReadDesign::run() {
    if (!__preWork()) {
        return ERROR;
    }
    if (!__readCell() || !__readTechLib() || !__readTimingLib()) {
        return ERROR;
    }
    if (!__postWork()) {
        return ERROR;
    }
    util::message->issueMsg(kMsgCategoryDB, 
        ReadDesignOk, kInfo, cell_name_.c_str());
    //std::cout << "INFO: Successfully read design " << cell_name_ << ".\n";
    return OK;
}

// Class WriteDesign
bool WriteDesign::__createDir(const char *dir_name) {
    DIR *dir = opendir(dir_name);
    if (!dir) {
        if (mkdir(dir_name, 0755) != 0) {
            util::message->issueMsg(kMsgCategoryDB, 
                CreateDirError, kError, dir_name);            
            //std::cout << "ERROR: Failed to create dir " 
              //<< dir_name << " for save_design.\n";
            return false;
        }
    }
    closedir(dir);
    return true;
}

bool WriteDesign::__preWork() {
    // TODO Need renaming?
    write_cell_ = getTopCell();
    original_cell_name_ = write_cell_->getName();
    if (original_cell_name_.compare(saved_name_) != 0) {
        if (getDebug()) {
            util::message->issueMsg(kMsgCategoryDB, 
                RenameCellVerbose, kDebug, 
                original_cell_name_.c_str(),
                saved_name_.c_str());          
            //std::cout << "DEBUGINFO: rename top cell name " 
                      //<< original_cell_name_
                      //<< " to " << saved_name_ << std::endl;
        }
        write_cell_->setName(saved_name_);
    }
    std::string dirname = getTopCell()->getName();
    if (!__createDir(dirname.c_str())) {
        return false;
    }
    dirname.append(kLibSubDirName);
    if (!__createDir(dirname.c_str())) {
        return false;
    }
    return true;
}

bool WriteDesign::__writeDBFile(
    MemPagePool *pool, 
    std::string &filename
) {
    ediAssert(pool != nullptr);
    std::string db_file = filename;
    db_file.append(kDBFilePostFix);

    IOHandler io_handler;
    if (false == io_handler.open(db_file.c_str(), "wb")) {
        message->issueMsg(kError, "Failed to open output db file %s.\n",
                          db_file.c_str());
        return false;
    }

    // write version:
    Version &v = getCurrentVersion();

    v.writeToFile(io_handler, getDebug());
    /*
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
    size_t pool_id = pool->getPoolNo();
    out_dbfile.write(reinterpret_cast<char *>(&pool_id), sizeof(size_t));
    out_dbfile.write(reinterpret_cast<char *>(&current_id_), sizeof(ObjectId));
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
    */
    io_handler.close();
    return true;
}

bool WriteDesign::__writePolyFile( PolygonTable *polygon_table, 
                                                      std::string &filename) {
    ediAssert(polygon_table != nullptr);
    std::string poly_file = filename;
    poly_file.append(kPolyFilePostFix);
    std::ofstream out_polyfile(poly_file.c_str(), std::ofstream::binary);
    if (out_polyfile.good() == false) {
        util::message->issueMsg(kMsgCategoryDB, 
            OpenFileError, kError, poly_file.c_str());      
        //std::cout << "ERROR: Failed to open output polygon file "
                  //<< poly_file << ".\n";      
        return false;
    }
    polygon_table->writeToFile(out_polyfile, getDebug());
    out_polyfile.close();
    return true;
}

bool WriteDesign::__writeSymFile(
    SymbolTable *symbol_table, 
    std::string &filename
) {
    ediAssert(symbol_table != nullptr);
    std::string sym_file = filename;
    sym_file.append(kSymFilePostFix);
    std::ofstream out_symfile(sym_file.c_str(), std::ofstream::binary);
    if (out_symfile.good() == false) {
        util::message->issueMsg(kMsgCategoryDB, 
            OpenFileError, kError, sym_file.c_str());
        //std::cout << "ERROR: Failed to open output symbol file "
                  //<< sym_file << ".\n";
        return false;
    }
    symbol_table->writeToFile(out_symfile, getDebug());
    out_symfile.close();
    return true;
}

bool WriteDesign::__postWork() {
    if (original_cell_name_.compare(saved_name_) != 0) {
        write_cell_->setName(original_cell_name_);
    }
    return true;
}

bool WriteDesign::__writeCell() {
    std::string dirname = getTopCell()->getName();
    std::string filename(dirname);
    filename.append("/");
    filename.append(saved_name_);

    current_id_ = write_cell_->getId();
    MemPagePool *pool = write_cell_->getPool();
    if (!__writeDBFile(pool, filename)) {
        util::message->issueMsg(kMsgCategoryDB, 
            WriteFileError, kError, "DB file", 
            write_cell_->getName().c_str());      
        //std::cout << "ERROR: failed to write DB file for cell.\n";
        return false;
    }
    PolygonTable *polygon_table = write_cell_->getPolygonTable();
    if (!__writePolyFile(polygon_table, filename)) {
        util::message->issueMsg(kMsgCategoryDB, 
            WriteFileError, kError, "polygon table", 
            write_cell_->getName().c_str());      
        //std::cout << "ERROR: failed to write polygon table for cell.\n";
        return false;
    }
    SymbolTable *symbol_table = write_cell_->getSymbolTable();
    if (!__writeSymFile(symbol_table, filename)) {
        util::message->issueMsg(kMsgCategoryDB, 
            WriteFileError, kError, "symbol table", 
            write_cell_->getName().c_str());      
        //std::cout << "ERROR: failed to write symbol table for cell.\n";
        return false;
    }
    return true;
}

bool WriteDesign::__writeTechLib() {
    std::string dirname = getTopCell()->getName();
    dirname.append(kLibSubDirName);
    std::string filename(dirname);
    filename.append("/");
    filename.append(kTechLibName);

    current_id_ = getRoot()->getTechLib()->getId();
    MemPagePool *pool = getRoot()->getTechLib()->getPool();
    if (!__writeDBFile(pool, filename)) {
        util::message->issueMsg(kMsgCategoryDB, 
            WriteFileError, kError, "DB file", "tech lib");      
        return false;
    }
    PolygonTable *polygon_table = getRoot()->getTechLib()->getPolygonTable();
    if (!__writePolyFile(polygon_table, filename)) {
        util::message->issueMsg(kMsgCategoryDB, 
            WriteFileError, kError, "polygon table", "tech lib");      
        return false;
    }
    SymbolTable *symbol_table = getRoot()->getTechLib()->getSymbolTable();
    if (!__writeSymFile(symbol_table, filename)) {
        util::message->issueMsg(kMsgCategoryDB, 
            WriteFileError, kError, "symbol table", "tech lib");      
        return false;
    }
    return true;
}

bool WriteDesign::__writeTimingLib() {
    std::string dirname = getTopCell()->getName();
    dirname.append(kLibSubDirName);
    std::string filename(dirname);  
    filename.append("/");
    filename.append(kTimingLibName);  

    current_id_ = getRoot()->getTimingLib()->getId();
    MemPagePool *pool = getRoot()->getTimingLib()->getPool();
    if (!__writeDBFile(pool, filename)) {
        util::message->issueMsg(kMsgCategoryDB, 
            WriteFileError, kError, "DB file", "timing lib");      
        return false;
    }
    PolygonTable *polygon_table = getRoot()->getTimingLib()->getPolygonTable();
    if (!__writePolyFile(polygon_table, filename)) {
        util::message->issueMsg(kMsgCategoryDB, 
            WriteFileError, kError, "polygon table", "timing lib");      
        return false;
    }
    SymbolTable *symbol_table = getRoot()->getTimingLib()->getSymbolTable();
    if (!__writeSymFile(symbol_table, filename)) {
        util::message->issueMsg(kMsgCategoryDB, 
            WriteFileError, kError, "symbol table", "timing lib");      
        return false;
    }
    return true;  
}

int WriteDesign::run() {
    if (!__preWork()) {
        return ERROR;
    }
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
    if (!__postWork()) {
        return ERROR;
    }
    util::message->issueMsg(kMsgCategoryDB, 
        WriteDesignOk, kInfo, saved_name_.c_str());
    //std::cout << "INFO: Successfully write design " << saved_name_ << ".\n";
    return OK;
}

}  // namespace db
}  // namespace open_edi
