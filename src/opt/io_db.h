#ifndef IO_DB_H
#define IO_DB_H

#include <fstream>
#include <queue>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <math.h>
#include <string>
#include "optimize.h"
#include "db/core/db.h"



#define DEBUG_IO 0
#if DEBUG_IO
#include <iostream>
#endif

namespace open_edi {
namespace opt {

class IODB {
  public:
    IODB();
    ~IODB();

    void readBufferLibrary(const std::string file_name, std::vector<Buffer> &buffers);
    int readInputTree(const std::string file_name, std::vector<Buffer> &drivers);

    double getR0();
    double getC0();
    size_t getTotalLevel();
    void getNodesByLevel(uint32_t level, std::vector<std::vector<Node *>> &nodes_array);
  private:
    std::ifstream fin_;
    std::ofstream fout_;
    std::unordered_map<std::string,db::Inst *> insts_;
    std::unordered_map<db::Net *,std::vector<Node *>> net_nodes_;
    std::vector<std::set<db::Net *>> nets_level_;

    double r0_;     //wire unit resistance
    double c0_;     //wire unit capacitance
    
    void destroyTree();
};

}//namespace opt
}//namespace open_edi

#endif