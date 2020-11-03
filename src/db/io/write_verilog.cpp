/* @file  write_verilog.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include <zlib.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <queue>

#include "db/io/write_verilog.h"
#include "db/core/db.h"
#include "db/core/object.h"
#include "db/util/array.h"
#include "util/message.h"

namespace open_edi {
namespace db {

class GzipStreambuf : public std::stringbuf {
 public:
    GzipStreambuf() {}
    virtual ~GzipStreambuf() {
        sync();
        gzclose(gzf);
    }
    bool open(const std::string &file_name) {
        gzf = gzopen(file_name.c_str(), "wb");
        return gzf != nullptr;
    }
    int sync() override {
        gzwrite(gzf, reinterpret_cast<const void *>(str().c_str()),
                unsigned(str().size()));
        str("");
        return 0;
    }
 private:
    gzFile gzf = nullptr;
};

class GzipOstream : public std::ostream  {
 public:
    GzipOstream() : std::ostream(nullptr) {
        rdbuf(&outbuf);
    }
    bool open(const std::string &file_name) {
        return outbuf.open(file_name);
    }
 private:
    GzipStreambuf outbuf;
};

static bool writeHeader(std::ostream *out_stream) {
    *out_stream << "/////////////////////////////////////////////"<< std::endl;
    *out_stream << "// Created by: NIICEDA tool" << std::endl;
    *out_stream << "/////////////////////////////////////////////"<< std::endl;

    return true;
}

static bool writeModule(std::ostream *out_stream, Cell *cell) {
    *out_stream << "module " << cell->getName() << " (";
    // term
    ObjectId terms = cell->getTerms();
    Term *term = nullptr;
    ArrayObject<ObjectId> *term_vector =
                                   cell->addr< ArrayObject<ObjectId> >(terms);
    if (!term_vector) {
        message->issueMsg(kError,
                "cannot get term vector from id %d of cell %s.\n",
                terms, cell->getName().c_str());
        return false;
    }
    bool first = true;
    for (int i = 0; i < term_vector->getSize(); ++i) {
        term = cell->addr<Term>((*term_vector)[i]);
        if (!term) {
            message->issueMsg(kError,
                    "cannot get term from id %d of cell %s.\n",
                    (*term_vector)[i], cell->getName().c_str());
            return false;
        }
        if (first) {
            first = false;
            *out_stream << " " << term->getName();
        } else {
            *out_stream << ", " << term->getName();
        }
    }
    *out_stream << " );" << std::endl;

    for (int i = 0; i < term_vector->getSize(); ++i) {
        term = cell->addr<Term>((*term_vector)[i]);
        if (!term) {
            message->issueMsg(kError,
                    "cannot get term from id %d of cell %s.\n",
                    (*term_vector)[i], cell->getName().c_str());
            return false;
        }
        std::string direction_str = term->getDirectionStr();
        toLower(direction_str);
        *out_stream << "  " << direction_str;
        if (term->getHasRange()) {
            *out_stream << " [" << term->getRangeHigh() << ":"
                        << term->getRangeLow() << "]";
        }
        *out_stream << " " << term->getName() << ";" << std::endl;
    }

    // bus
    ObjectId buses = cell->getBuses();
    if (buses > 0) {
        Bus *bus = nullptr;
        ArrayObject<ObjectId> *bus_vector
                                  = cell->addr< ArrayObject<ObjectId> >(buses);
        if (!bus_vector) {
            message->issueMsg(kError,
                    "cannot get bus vector from id %d of cell %s.\n",
                    buses, cell->getName().c_str());
            return false;
        }
        for (int i = 0; i < bus_vector->getSize(); ++i) {
            bus = cell->addr<Bus>((*bus_vector)[i]);
            if (!bus) {
                message->issueMsg(kError,
                        "cannot get bus from id %d of cell %s.\n",
                        (*bus_vector)[i], cell->getName().c_str());
                return false;
            }
            if (bus->getIsTri()) {
                *out_stream << "  tri " << "[" << bus->getRangeLeft() << ":"
                            << bus->getRangeRight() << "]";
            } else {
                *out_stream << "  wire " << "[" << bus->getRangeLeft() << ":"
                            << bus->getRangeRight() << "]";
            }
            *out_stream << " " << bus->getName() << ";" << std::endl;
        }
    }
    // net
    ObjectId nets = cell->getNets();
    if ( nets > 0 ) {
        Net *net = nullptr;
        ArrayObject<ObjectId> *net_vector
                                   = cell->addr< ArrayObject<ObjectId> >(nets);
        if (!net_vector) {
            message->issueMsg(kError,
                    "cannot get net vector from id %d of cell %s.\n",
                    nets, cell->getName().c_str());
            return false;
        }
        for (int i = 0; i < net_vector->getSize(); ++i) {
            net = cell->addr<Net>((*net_vector)[i]);
            if (!net) {
                message->issueMsg(kError,
                        "cannot get net from id %d of cell %s.\n",
                        (*net_vector)[i], cell->getName().c_str());
                return false;
            }
            if (!(net->getIsBusNet()) && !(net->getIsOfBus())
                    && !(net->getIsFromTerm())) {
                if (NetType::kNetTypeTri == net->getType()) {
                    *out_stream << "  tri " << net->getName()
                                << ";"<< std::endl;
                } else {
                    *out_stream << "  wire " << net->getName()
                                << ";"<< std::endl;
                }
            }
            AssignType assign_type = net->getAssignType();
            switch (assign_type) {
                case kAssignTypeNet: {
                    ObjectId assign_net_id = net->getAssignNet();
                    if (assign_net_id > 0) {
                        Net *assign_net = cell->addr<Net>(assign_net_id);
                        if (!assign_net) {
                            message->issueMsg(kError,
                                    "cannot get net from id %d of cell %s.\n",
                                    assign_net_id, cell->getName().c_str());
                            return false;
                        }
                        *out_stream << "  assign " << net->getName() << " = "
                                  << assign_net->getName() << ";" << std::endl;
                    }
                    }
                    break;
                case kAssignTypeInt: {
                    int int_constant = net->getAssignInt();
                    if (0 == int_constant) {
                        *out_stream << "  supply0 " << net->getName() 
                                    << ";" << std::endl;
                    } else if (1 == int_constant) {
                        *out_stream << "  supply1 " << net->getName() 
                                    << ";" << std::endl;
                    } else {
                        *out_stream << "  assign " << net->getName() << " = "
                                  << int_constant << ";" << std::endl;
                    }
                    }
                    break;
                case kAssignTypeReal: {
                    double real_constant = net->getAssignReal();
                    *out_stream << "  assign " << net->getName() << " = "
                              << real_constant << ";" << std::endl;
                    }
                    break;
                default:
                    break;
            }
            
        }
    }

    // instance
    ObjectId insts = cell->getInstances();
    if ( insts > 0 ) {
        Inst *inst = nullptr;
        ArrayObject<ObjectId> *inst_vector
                                  = cell->addr< ArrayObject<ObjectId> >(insts);
        if (!inst_vector) {
            message->issueMsg(kError,
                    "cannot get instance vector from id %d of cell %s.\n",
                    insts, cell->getName().c_str());
            return false;
        }
        for (int i = 0; i < inst_vector->getSize(); ++i) {
            inst = cell->addr<Inst>((*inst_vector)[i]);
            if (!inst) {
                message->issueMsg(kError,
                        "cannot get instance from id %d of cell %s.\n",
                        (*inst_vector)[i], cell->getName().c_str());
                return false;
            }
            Cell *master = inst->getMaster();
            if (!master) {
                message->issueMsg(kError,
                        "cannot get master cell for instance %s.\n",
                        inst->getName().c_str());
                return false;
            }

            *out_stream << "  " << master->getName() << " "
                        << inst->getName() << " (";
            ObjectId pins = inst->getPins();
            VectorObject8 *pins_vector = cell->addr<VectorObject8>(pins);
            first = true;
            for (int i = 0; i < pins_vector->totalSize(); i++) {
                ObjectId pin_id = (*pins_vector)[i];
                Pin *pin = cell->addr<Pin>(pin_id);
                if (!pin) {
                    message->issueMsg(kError,
                            "cannot get pin from id %d of instance %s.\n",
                            pin_id, inst->getName().c_str());
                    return false;
                }
                if (first) {
                    first = false;
                    *out_stream << " .";
                } else {
                    *out_stream << ", .";
                }
                *out_stream << pin->getName() << "(";

                if (false == pin->getIsConnectNets()) {
                    Net *net = pin->getNet();
                    if (net) {
                        *out_stream << net->getName();
                    }
                } else {
                    *out_stream << "{ ";
                    ArrayObject<ObjectId> *net_array = pin->getNetArray();
                    ArrayObject<ObjectId>::iterator iter;
                    Net *net = nullptr;
                    bool first_net = true;
                    for (iter = net_array ->begin(); iter != net_array->end();
                            ++iter) {
                        net = cell->addr<Net>(*iter);
                        if (net) {
                            if (!first_net) {
                                *out_stream << ", ";
                            }
                            *out_stream << net->getName();
                            first_net = false;
                        }
                    }
                    *out_stream << " }";
                }
                *out_stream << ")";
            }
            *out_stream << " );"<< std::endl;
        }
    }

    *out_stream << "endmodule" << std::endl << std::endl;
    return true;
}

static bool writeModules(std::ostream *out_stream) {
    Cell *top_cell = getTopCell();
    Cell *cell = nullptr;
    ArrayObject<ObjectId> *vct = nullptr;

    ObjectId cells = top_cell->getCells();

    if (cells == 0) {
        return true;
    } else {
        vct = top_cell->addr< ArrayObject<ObjectId> >(cells);
    }
    if (vct) {
        for (int i = 0; i < vct->getSize(); ++i) {
            cell = top_cell->addr<Cell>((*vct)[i]);
            if (!cell) {
                message->issueMsg(kError, "Cannot find cell with id %d.\n", i);
                continue;
            }
            if (cell->getCellType() == CellType::kHierCell) {
                writeModule(out_stream, cell);
            }
        }
    } else {
        message->issueMsg(kError, "Get cell vector failed.\n");
        return false;
    }
    return true;
}

int writeVerilog(int argc, const char **argv) {
    FILE *fp = NULL;

    if (argc != 2) {
        message->issueMsg(kError, "Usage: write_verilog filename\n");
        return 1;
    }

    std::string file_name = "";
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] != '-') {
            file_name = argv[i];
        }
    }
    if (file_name.empty()) {
        message->issueMsg(kError, "No file name.\n");
        return 1;
    }
    std::ostream *out_stream = nullptr;
    if (file_name.size() > 3 &&
        file_name.compare(file_name.size()-3, std::string::npos, ".gz") == 0) {
        GzipOstream *gzip_ostream = new GzipOstream;
        if (!gzip_ostream->open(file_name)) {
            delete gzip_ostream;
            message->issueMsg(kError,
                    "Cannot open file %s for writing: %s\n",
                    file_name.c_str(), strerror(errno));
            return 1;
        }
        out_stream = gzip_ostream;
    } else {
        std::ofstream *of_stream = new std::ofstream;
        of_stream->open(file_name.c_str(), std::ofstream::trunc);
        if (of_stream->fail()) {
            delete of_stream;
            message->issueMsg(kError,
                    "Cannot open file %s for writing: %s\n",
                    file_name.c_str(), strerror(errno));
            return 1;
        }
        out_stream = of_stream;
    }

    message->info("\nWriting Verilog\n");
    fflush(stdout);

    if (!writeHeader(out_stream)) {
        message->issueMsg(kError, "Write verilog header failed.\n");
        return 1;
    }

    if (!writeModules(out_stream)) {
        message->issueMsg(kError, "Write verilog module failed.\n");
        return 1;
    }

    delete out_stream;
    message->info("\nWrite Verilog successfully.\n");

    return 0;
}

}  // namespace db
}  // namespace open_edi
