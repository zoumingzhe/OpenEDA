#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#include "IO.h"
#include "Van.h"

#include <unistd.h>
#include <string>
#include <chrono>
#include <iostream>
#include <iomanip>

namespace open_edi {
namespace opt {

class Optimize {
  public:
    Optimize();
    ~Optimize();

    int optimize_net (int argc, char **argv);
  private:
    IO *io_;
    Van *van_;

    void printMemoryInfo();
    void printBufferSolution(BufferNode *root,uint64_t id_upBound, std::ofstream &fout);
    void outputSolution(const std::string output_file, std::vector<VanSizing *> &solutions, uint64_t id_upBound, bool all);
};

}//namespace opt
}//namespace open_edi

#endif