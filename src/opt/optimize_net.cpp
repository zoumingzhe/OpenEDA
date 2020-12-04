#include "optimize_net.h"

using namespace std;

namespace open_edi {
namespace opt {

OptimizeNet::OptimizeNet() {
    io_ = NULL;
    van_ = NULL;
}

OptimizeNet::~OptimizeNet() {
    if(van_)
        delete van_;
    if(io_){
        io_->destroyTree();
        delete io_;
    }
}

void OptimizeNet::printBufferSolution(BufferNode *root,uint64_t id_upBound, ofstream &fout) {
    queue<BufferNode *> nodes;
    if(root)
        nodes.push(root);
    while(nodes.size()>0){
        BufferNode *tmp = nodes.front();
        nodes.pop();
        if (tmp->bufferType < id_upBound) {
            fout << "start " << tmp->id_at << " end " << tmp->id_to << " buffertype " << tmp->bufferType << endl;
        }
        if(tmp->left){
            nodes.push(tmp->left);
        }
        if(tmp->right){
            nodes.push(tmp->right);
        }
    }
}

void OptimizeNet::outputSolution(const string output_file, vector<VanSizing *> &solutions, uint64_t id_upBound, bool all) {
    ofstream fout(output_file,fstream::out);
    if ( !fout ) {
        cout << "open file '" << output_file << "' failed!" << endl;
        return;
    }
    if ( solutions.size() == 0 ) {
        fout << "no solution available!" << endl;
    }
    VanSizing *best_solution = solutions[0];
    int solution_count = 0;
    double qmax = best_solution->time;
    for (auto &solution : solutions) {
        if ( qmax < solution->time ){
            qmax = solution->time;
            best_solution = solution;
        }
        solution_count++;
    }
    fout << "total solutions:" << solution_count << endl;
    fout << "best(t,c,a)=(" << best_solution->time << "," << best_solution->capacitance << "," << best_solution->area << ")" << endl;
    printBufferSolution(best_solution->buffer_location,id_upBound,fout);
    if (all) {
        fout << endl << endl << "all solutions:" << endl;
        for (auto &solution : solutions) {
            fout << "(t,c,a)=(" << solution->time << "," << solution->capacitance << "," << solution->area << ")" << endl;
        }
    }
    //output buffer_location
    fout.close();
}

int OptimizeNet::optimize_net (int argc, char **argv) {
    int param;
    string buffer_file, net_file, output_file;
    bool outputAllResults = false;
    while ( (param = getopt(argc, argv, "an:b:o:")) != -1 ) {
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
            default:
                break;
        }
    }
    vector<Buffer> buffers;
    vector<Buffer> drivers;
    io_ = new IO();
    io_->readBufferLibrary(buffer_file,buffers);
    int error = io_->readInputTree(net_file,drivers);
    if(error==-1){
        cout << "cannot open file:" << net_file << endl;
        return -1;
    }
    double r0 = io_->getR0();
    double c0 = io_->getC0();
    van_ = new Van(r0,c0);
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    van_->optimization(io_->nodes_array,buffers,drivers);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    chrono::duration<double, std::milli> elapsed = end - begin;
    cout << "elapsed time: " << elapsed.count() << " [ms]" << endl;
    vector<VanSizing *> solutions;
    van_->getSolutions(solutions);
    outputSolution(output_file,solutions,buffers.size(),outputAllResults);
    return 0;
}

int OptimizeNet::optimize_net (double r0, double c0, int id,
                            const std::vector<Node *> &nodes_array,
                            const std::vector<Buffer> &buffers,
                            const std::vector<Buffer> &drivers) {
    van_ = new Van(r0,c0);
    van_->optimization(nodes_array,buffers,drivers);
    vector<VanSizing *> solutions;
    van_->getSolutions(solutions);
    outputSolution("/home/yemy/Documents/testbench/result"+to_string(id)+".out",solutions,buffers.size(),false);
    return id+1;
}


}//namespace opt
}//namespace open_edi