#include <unistd.h>
#include "optimize_nets.h"
#include "optimize_net.h"

using namespace std;
using namespace open_edi::db;

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
                output_file_.assign(optarg);
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
    size_t totalLevel = io_->getTotalLevel();
    current_level_ = 0;
    while(current_level_ < totalLevel) {
        #if DEBUG_NETS
        cout << "running level " << current_level_ << endl;
        #endif
        run(1,1,1);
    }
    io_->writeResult(output_file_,inst_nodes_);
}

void OptimizeNets::preRun() {
    io_->createInstNodes(inst_nodes_,current_level_);
    io_->getNodesByLevel(current_level_,p_input_);
    worker_cnt_ = min((size_t)j_,p_input_.size());
    current_level_++;
}

void *OptimizeNets::runMapper() {
    for(size_t i=0;i<p_input_.size();i++){
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
        // update RAT
        vector<Node *> array = p_input_[net->net_id];
        for(auto &node : array){
            if(node->type != CANDIDATE){// SINK
                break;
            }
            Pin *pin = Object::addr<Pin>(node->id);
            InstNode *inst_node = inst_nodes_[pin->getInst()->getId()];
            if(node->solutions[0]==NULL){
                cout << "strange pin:" << pin->getInst()->getName() << " " << pin->getName() << " with id/type:" << node->id << "/" << (int)node->type << endl;
            }
            double rat = node->solutions[0]->time;
            for(auto &pin_node : inst_node->out_pins){
                VanResult *s = pin_node.second->best_solution;
                if(s){
                    rat = min(rat,s->time);
                }
            }
            node->solutions[0]->time = rat-inst_node->delay;
            if(node->id==1125899915791184UL){
                cout << "used" << endl;
            }
        }
        OptimizeNet *opt = new OptimizeNet();
        Pin *pin = Object::addr<Pin>(array.back()->id);//SOURCE
        InstNode *inst_node = inst_nodes_[pin->getInst()->getId()];
        vector<VanSizing *> solutions;
        if(current_level_ == io_->getTotalLevel()){
            opt->optimize_net(r0_,c0_,array,buffers_,drivers_,solutions);
            inst_node->out_pins[pin->getId()]->solutions = cloneSolutions(solutions);
        }else{
            vector<Buffer> drivers;
            drivers.emplace_back(false,0,inst_node->delay,0,0);
            opt->optimize_net(r0_,c0_,array,buffers_,drivers,solutions);
            if(solutions.size()>0){
                inst_node->out_pins[pin->getId()]->best_solution = cloneSolution(solutions[0]);
            }
        }
        delete opt;
        delete net;
    }
    return NULL;
}

void *OptimizeNets::runReducer() {
    return NULL;
}

void OptimizeNets::postRun() {
    return;
}



inline BufferNode *OptimizeNets::deepCopyBufferNode(BufferNode *node) {
    BufferNode *copy_node = new BufferNode();
    copy_node->bufferType = node->bufferType;
    copy_node->id_at = node->id_at;
    copy_node->id_to = node->id_to;
    return copy_node;
}

BufferNode *OptimizeNets::deepCopyBufferLocation(BufferNode *location) {
    unordered_map<BufferNode *,BufferNode *> src2copy;
    queue<BufferNode *> nodes;
    if(location){
        nodes.push(location);
        BufferNode *copy_location = deepCopyBufferNode(location);
        src2copy[location] = copy_location;
    }
    while(nodes.size()>0){
        BufferNode *tmp = nodes.front();
        nodes.pop();
        BufferNode *copy_tmp = src2copy[tmp];
        if(tmp->left){
            nodes.push(tmp->left);
            BufferNode *copy_left = deepCopyBufferNode(tmp->left);
            src2copy[tmp->left] = copy_left;
            copy_tmp->left = copy_left;
        }
        if(tmp->right){
            nodes.push(tmp->right);
            BufferNode *copy_right = deepCopyBufferNode(tmp->right);
            src2copy[tmp->right] = copy_right;
            copy_tmp->left = copy_right;
        }
    }
    return src2copy[location];
}

VanResult *OptimizeNets::cloneSolution(VanSizing *solution) {
    VanResult *result = new VanResult(solution);
    result->buffer_location = deepCopyBufferLocation(solution->buffer_location);
    return result;
}

VanResult *OptimizeNets::cloneSolutions(vector<VanSizing *> &solutions) {
    VanResult head;
    head.next = NULL;
    VanResult *current = &head;
    for(auto &solution : solutions){
        current->next = cloneSolution(solution);
        current = current->next;
    }
    return head.next;
}

}//namespace opt
}//namespace open_edi
