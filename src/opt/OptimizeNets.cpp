#include <unistd.h>
#include "OptimizeNets.h"
#include "OptimizeNet.h"

using namespace std;

namespace open_edi {
namespace opt {

OptimizeNets::OptimizeNets() {
    j_ = 1;
    io_ = NULL;
}

OptimizeNets::~OptimizeNets() {
    if(io_){
        io_->destroyTree();
        delete io_;
    }
}

int OptimizeNets::parseCommand(int argc, char *argv[]) {
    int param;
    string buffer_file, net_file, output_file;
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
                output_file.assign(optarg);
                break;
            case 'j':
                j_ = max(j_,atoi(optarg));
                break;
            default:
                break;
        }
    }
    io_ = new IO();
    io_->readBufferLibrary(buffer_file,buffers_);
    int error = io_->readInputTree(net_file,drivers_);
    if(error==-1){
        return -1;
    }
    r0_ = io_->getR0();
    c0_ = io_->getC0();
    return 0;
}

void OptimizeNets::preRun() {
    worker_cnt_ = j_;
    for(int i=0;i<simulate_nets;i++){
        vector<Node *> array;
        //deep copy
        io_->getTreeCopy(array);
        p_input_.push_back(array);
    }
    #if DEBUG_NETS
    cout << "prepare to run " << worker_cnt_ << "x!" << endl;
    #endif
}

void *OptimizeNets::runMapper() {
    for(int i=0;i<simulate_nets;i++){
        #if DEBUG_NETS
        cout << "mapper[" << i << "]" << endl;
        #endif
        task_queue_.push(new Net(i));
    }
    return NULL;
}

void *OptimizeNets::runWorker() {
    while(true){
        Net *net = static_cast<Net *>(task_queue_.pop());
        if(!net){
            break;
        }
        #if DEBUG_NETS
        cout << "worker[" << net->net_id << "]" << endl;
        #endif
        OptimizeNet *opt = new OptimizeNet();
        int result = opt->optimize_net(r0_,c0_,net->net_id,p_input_[net->net_id],buffers_,drivers_);
        solution_queue_.push(new VanSolution(result));
        delete opt;
        delete net;
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
    j_ = 1;
    // TODO
    // destroy copy tree
    if(io_){
        io_->destroyTree();
        delete io_;
    }
    io_ = NULL;
}

}//namespace opt
}//namespace open_edi