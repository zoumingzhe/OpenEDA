#ifndef OPTIMIZENETS_H
#define OPTIMIZENETS_H

#include "util/map_reduce.h"
#include "IO.h"

#define DEBUG_NETS 0
#if DEBUG_NETS
#include <iostream>
#endif



namespace open_edi {
namespace opt {

class OptimizeNets : public util::MTMRApp {
  public:
    OptimizeNets();
    ~OptimizeNets();

    int parseCommand(int argc, char *argv[]);

  protected:
    class Net : public util::MTTask {
      public:
        Net(int id) : net_id(id) {};
        int net_id;
    };
    class VanSolution : public util::MTTask {
      public:
        VanSolution(int solution) : van_solution(solution) {};
        int van_solution;
    };
  private:
    int j_;
    IO *io_;
    std::vector<Buffer> buffers_;
    std::vector<Buffer> drivers_;
    std::vector<std::vector<Node *>> p_input_;
    double r0_;
    double c0_;

    int simulate_nets = 2;

    virtual void preRun();
    virtual void* runMapper();
    virtual void* runWorker();
    virtual void* runReducer();
    virtual void postRun();
};

}//namespace opt
}//namespace open_edi


#endif