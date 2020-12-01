#ifndef IO_H
#define IO_H

#include <fstream>
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <math.h>
#include <string>
#include "Def.h"

#define DEBUG_IO 0
#if DEBUG_IO
#include <iostream>
#endif

namespace open_edi {
namespace opt {

class IO {
  public:
    IO();
    ~IO();

    std::vector<Node *> nodes_array;

    void readBufferLibrary(const std::string file_name, std::vector<Buffer> &buffers);
    int readInputTree(const std::string file_name, std::vector<Buffer> &drivers);
    void destroyTree();

    double getR0();
    double getC0();
  private:
    std::ifstream fin_;
    std::ofstream fout_;
    std::unordered_map<uint64_t, Node *> nodes_;

    double r0_;     //wire unit resistance
    double c0_;     //wire unit capacitance
    uint64_t used_id_;
};

}//namespace opt
}//namespace open_edi

#endif