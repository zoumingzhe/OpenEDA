#ifndef OPTIMIZENETS_H
#define OPTIMIZENETS_H


#include "util/map_reduce.h"
#include "io_db.h"

#define DEBUG_NETS 1
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
    void runOptimize();

  protected:
    class NetTask : public util::MTTask {
      public:
        NetTask(int id) : net_id(id) {};
        int net_id;
    };

  private:
    int j_;
    IODB *io_;
    std::string output_file_;
    std::vector<Buffer> buffers_;
    std::vector<Buffer> drivers_;
    std::vector<std::vector<Node *>> p_input_;
    double r0_;
    double c0_;
    uint32_t current_level_;
    std::unordered_map<uint64_t,InstNode *> inst_nodes_;

    virtual void preRun();
    virtual void* runMapper();
    virtual void* runWorker();
    virtual void* runReducer();
    virtual void postRun();

    inline BufferNode *deepCopyBufferNode(BufferNode *node);
    BufferNode *deepCopyBufferLocation(BufferNode *location);
    VanResult *cloneSolution(VanSizing *solution);
    VanResult *cloneSolutions(std::vector<VanSizing *> &solutions);
};

}//namespace opt
}//namespace open_edi


#endif
