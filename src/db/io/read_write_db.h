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
const char kSymFilePostFix[] = ".sym";
const char kPolyFilePostFix[] = ".poly";
const char kPropFilePostFix[] = ".prop";
const int OK = 0;     // equals to TCL_OK
const int ERROR = 1;  // equals to TCL_ERROR

class ReadDesign {
 public:
    explicit ReadDesign(const std::string &name)
        : cell_name_(name),
          symbol_table_(nullptr),
          polygon_table_(nullptr),
          pool_(nullptr),
          debug_(false) {}

    int run();

    bool getDebug() { return debug_; }
    void setDebug(bool v) { debug_ = v; }

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

    bool __readSymFile(void);
    bool __readPolyFile(void);
    bool __readDBFile(void);
    // bool __readPropFile(void);
    bool __setCurrentTopCell(void);

    // DATA
    std::string cell_name_;
    ObjectId top_cell_id_;
    // std::string file_name_;
    SymbolTable *symbol_table_;
    PolygonTable *polygon_table_;
    MemPagePool *pool_;
    Version v_;
    bool debug_;
};

class WriteDesign {
 public:
    /// @brief default constructor
    WriteDesign();
    explicit WriteDesign(const std::string &name)
        : saved_name_(name),
          top_cell_(nullptr),
          cell_name_(""),
          debug_(false) {}

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

    bool __writeDBFile(void);
    bool __writePolyFile(void);
    bool __writeSymFile(void);
    // bool __writePropFile(void);
    bool __setAfterWrite(void);
    // DATA
    std::string saved_name_;
    Cell *top_cell_;
    std::string cell_name_;
    bool debug_;
};

}  // namespace db
}  // namespace open_edi

#endif  // SRC_DB_IO_READ_WRITE_DB_H_
