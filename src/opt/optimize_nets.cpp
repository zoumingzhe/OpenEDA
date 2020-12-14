#include <unistd.h>
#include "optimize_nets.h"
#include "optimize_net.h"

using namespace std;

namespace open_edi {
namespace opt {

OptimizeNets::OptimizeNets() {
    j_ = 1;
    io_ = NULL;
    current_level_ = 0;
}

OptimizeNets::~OptimizeNets() {
    if(io_){
        delete io_;
    }
}

int OptimizeNets::parseCommand(int argc, char *argv[]) {
    int param;
    string buffer_file, net_file;
    bool outputAllResults = false;
    while ( (param = getopt(argc, argv, "an:b:o:j::")) != -1 ) {
        switch (param) {
            case 'a':
                outputAllResults = true;
                break;
            case 'n':
                net_file.assign(optarg);
                break;
            case 'b':
                buffer_file.assign(optarg);
                break;
            case 'o':
                output_dir_.assign(optarg);
                break;
            case 'j':
                j_ = max(j_,atoi(optarg));
                break;
            default:
                break;
        }
    }
    io_ = new IODB();
    io_->readBufferLibrary(buffer_file,buffers_);
    int error = io_->readInputTree(net_file,drivers_);
    if(error==-1){
        return -1;
    }
    r0_ = io_->getR0();
    c0_ = io_->getC0();
    return 0;
}

void OptimizeNets::runOptimize() {
    //size_t totalLevel = io_->getTotalLevel();
    size_t totalLevel = 1;
    current_level_ = 0;
    while(current_level_ < totalLevel) {
        run(1,1,1);
    }
}

void OptimizeNets::preRun() {
    worker_cnt_ = j_;
    io_->getNodesByLevel(current_level_,p_input_);
    current_level_++;
    #if DEBUG_NETS
    cout << "prepare to run " << worker_cnt_ << "x!" << endl;
    #endif
}

void *OptimizeNets::runMapper() {
    for(size_t i=0;i<p_input_.size();i++){
        #if DEBUG_NETS
        cout << "mapper[" << i << "]" << endl;
        #endif
        task_queue_.push(new NetTask(i));
    }
    return NULL;
}

void *OptimizeNets::runWorker() {
    while(true){
        NetTask *net = static_cast<NetTask *>(task_queue_.pop());
        if(!net){
            break;
        }
        #if DEBUG_NETS
        cout << "worker[" << net->net_id << "]" << endl;
        #endif
        OptimizeNet *opt = new OptimizeNet(output_dir_);
        vector<Node *> array = p_input_[net->net_id];
        int result = opt->optimize_net(r0_,c0_,net->net_id,array,buffers_,drivers_);
        solution_queue_.push(new VanSolution(result));
        delete opt;
        delete net;
        for(auto &node : array){
            delete node;
        }
    }
    return NULL;
}

void *OptimizeNets::runReducer() {
    while(true){
        VanSolution *solution = static_cast<VanSolution *>(solution_queue_.pop());
        if(!solution){
            break;
        }
        #if DEBUG_NETS
        cout << "solution:" << solution->van_solution << endl;
        #endif
        delete solution;
    }
    return NULL;
}

void OptimizeNets::postRun() {
    #if DEBUG_NETS
    cout << "run OK" << endl;
    #endif
}

}//namespace opt
}//namespace open_edi