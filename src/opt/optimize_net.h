#ifndef OPTIMIZENET_H
#define OPTIMIZENET_H

#include "io_file.h"
#include "van.h"

#include <unistd.h>
#include <string>
#include <chrono>
#include <iostream>
#include <iomanip>

namespace open_edi {
namespace opt {

class OptimizeNet {
  public:
    OptimizeNet();
    OptimizeNet(std::string output_dir);
    ~OptimizeNet();

    int optimize_net (int argc, char **argv);
    int optimize_net (double r0, double c0, int id,
                const std::vector<Node *> &nodes_array,
                const std::vector<Buffer> &buffers,
                const std::vector<Buffer> &drivers,
                uint64_t used_id);
  private:
    IOFILE *io_;
    std::string output_dir_;
    Van *van_;

    void printBufferSolution(BufferNode *root,uint64_t id_upBound, std::ofstream &fout);
    void outputSolution(const std::string output_file, std::vector<VanSizing *> &solutions, uint64_t id_upBound, bool all);
};

}//namespace opt
}//namespace open_edi

#endif
