/**
 * @file  read_write_db.h
 * @date
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef SRC_DB_IO_READ_WRITE_DB_H_
#define SRC_DB_IO_READ_WRITE_DB_H_

#include <string>

#include "db/core/db.h"
#include "db/util/symbol_table.h"
#include "util/util.h"

namespace open_edi {
namespace db {

using Version = open_edi::util::Version;

const char kDBFilePostFix[] = ".db";
const char kSymFilePostFix[] = ".sym.lz4";
const char kPolyFilePostFix[] = ".poly.lz4";
const char kPropFilePostFix[] = ".prop";
const char kLibSubDirName[] = "/Libs";
const char kTechLibName[] =  "lef";
const char kTimingLibName[] =  "liberty";
const int OK = 0;     // equals to TCL_OK
const int ERROR = 1;  // equals to TCL_ERROR

const char kMsgCategoryDB[] = "DB";
enum readWriteDesignMsgId {
    ReadDesignOk = 100,
    WriteDesignOk = 101,
    OpenFileError = 102,
    CheckSumError = 103,
    CheckSumOk = 104,
    ReadDesignOkVerbose = 105,
    ReadDesignInitError = 106,
    CreateDirError = 107,
    WriteFileError = 108,
    RenameCellVerbose = 109
};

class ReadDesign {
 public:
    explicit ReadDesign(const std::string &name) {
        current_id_ = 0;
        is_top_ = false;
        debug_ = false;
        read_dir_name_ = name;
        size_t find_pos = name.rfind('/');
        if (std::string::npos != find_pos) {
            read_cell_name_ = name.substr(find_pos + 1, std::string::npos);
        } else {
            read_cell_name_ = name;
        }
    }

    int run();

    bool getDebug() { return debug_; }
    void setDebug(bool v) { debug_ = v; }
    void setTop(void) { is_top_ = true; }

 private:
    ReadDesign() {}
    /// @brief copy constructor
    ReadDesign(ReadDesign const &rhs) {}
    /// @brief move constructor
    ReadDesign(ReadDesign &&rhs) noexcept {}
    /// @brief copy assignment
    ReadDesign &operator=(ReadDesign const &rhs) { return *this; }
    /// @brief move constructor
    ReadDesign &operator=(ReadDesign &&rhs) noexcept { return *this; }

    bool __readDBFile(MemPagePool *pool, std::string &filename);
    bool __readPolyFile(PolygonTable *polygon_table, std::string &filename);
    bool __readSymFile(SymbolTable *symbol_table, std::string &filename);

    bool __readCell(void);
    bool __readTechLib(void);
    bool __readTimingLib(void);

    // bool __readPropFile(void);
    bool __preWork(void);
    bool __postWork(void);
    // DATA
    std::string read_dir_name_;
    std::string read_cell_name_;
    ObjectId current_id_;
    Version v_;
    bool is_top_;
    bool debug_;
};

class WriteDesign {
 public:
    /// @brief default constructor
    WriteDesign();
    explicit WriteDesign(const std::string &name) {
        original_cell_name_ = "";
        write_cell_ = nullptr;
        current_id_ = 0;
        debug_ = false;
        write_dir_name_ = name;
        size_t find_pos = name.rfind('/');
        if (std::string::npos != find_pos) {
            write_cell_name_ = name.substr(find_pos + 1, std::string::npos);
        } else {
            write_cell_name_ = name;
        }
    }

    int run();

    bool getDebug() { return debug_; }
    void setDebug(bool v) { debug_ = v; }

 private:
    /// @brief copy constructor
    WriteDesign(WriteDesign const &rhs) {}
    /// @brief move constructor
    WriteDesign(WriteDesign &&rhs) noexcept {}
    /// @brief copy assignment
    WriteDesign &operator=(WriteDesign const &rhs) { return *this; }
    /// @brief move constructor
    WriteDesign &operator=(WriteDesign &&rhs) noexcept { return *this; }

    bool __writeDBFile(MemPagePool *pool, std::string &filename);
    bool __writePolyFile(PolygonTable *polygon_table, std::string &filename);
    bool __writeSymFile(SymbolTable *symbol_table, std::string &filename);

    bool __writeCell(void);
    bool __writeTechLib(void);
    bool __writeTimingLib(void);
    // bool __writePropFile(void);
    bool __preWork(void);
    bool __postWork(void);
    bool __createDir(const char *dir_name);

    // DATA
    std::string original_cell_name_;
    std::string write_dir_name_;
    std::string write_cell_name_;
    Cell *write_cell_;
    ObjectId current_id_;
    bool debug_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_IO_READ_WRITE_DB_H_
