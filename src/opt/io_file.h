#ifndef IO_FILE_H
#define IO_FILE_H

#include <fstream>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <math.h>
#include <string>
#include "optimize.h"

#define DEBUG_IO 0
#if DEBUG_IO
#include <iostream>
#endif

namespace open_edi {
namespace opt {

class IOFILE {
  public:
    IOFILE();
    ~IOFILE();

    std::vector<Node *> nodes_array;

    void readBufferLibrary(const std::string file_name, std::vector<Buffer> &buffers);
    int readInputTree(const std::string file_name, std::vector<Buffer> &drivers);

    double getR0();
    double getC0();
    uint64_t used_id_;
    void getTreeCopy(std::vector<Node *> &array);
  private:
    std::ifstream fin_;
    std::ofstream fout_;
    std::unordered_map<uint64_t, Node *> nodes_;

    double r0_;     //wire unit resistance
    double c0_;     //wire unit capacitance
    
    void destroyTree();
};

}//namespace opt
}//namespace open_edi

#endif
