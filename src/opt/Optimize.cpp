#include "Optimize.h"

using namespace std;

namespace open_edi {
namespace opt {

Optimize::Optimize() {
    io_ = NULL;
    van_ = NULL;
}

Optimize::~Optimize() {
    if(van_)
        delete van_;
    if(io_){
        io_->destroyTree();
        delete io_;
    }
    printMemoryInfo();
}

void Optimize::printMemoryInfo() {
    ifstream fin("/proc/self/status");
    if(!fin){
        cout << "memory info unknown!" << endl;
    }else{
        string part;
        double value;
        string memory_unit[4] = {" [KB]"," [MB]"," [GB]", " [TB]"};
        cout << fixed;
        while(fin >> part){
            if(part == "VmHWM:"){
                fin >> value;
                int index = 0;
                while(value>900){
                    value /= 1024;
                    index++;
                }
                value<2 ? cout<<setprecision(2) : cout<<setprecision(1);
                cout << "peak real memory usage: " << value << memory_unit[index] << endl;
            }else if(part == "VmPeak:"){
                fin >> value;
                int index = 0;
                while(value>900){
                    value /= 1024;
                    index++;
                }
                value<2 ? cout<<setprecision(2) : cout<<setprecision(1);
                cout << "peak virtual memory usage: " << value << memory_unit[index] << endl;
            }
        }
        fin.close();
        cout.unsetf(ios::fixed);
        cout.precision(6);
    }
}

void Optimize::printBufferSolution(BufferNode *root,uint64_t id_upBound, ofstream &fout) {
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

void Optimize::outputSolution(const string output_file, vector<VanSizing *> &solutions, uint64_t id_upBound, bool all) {
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

int Optimize::optimize_net (int argc, char **argv) {
    for(int i=0;i<argc;i++){
        cout << "param[" << i << "]:" << argv[i] << endl;
    }
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
    std::chrono::duration<double, std::milli> elapsed = end - begin;
    cout << "elapsed time: " << elapsed.count() << " [ms]" << endl;
    vector<VanSizing *> solutions;
    van_->getSolutions(solutions);
    outputSolution(output_file,solutions,buffers.size(),outputAllResults);
    return 0;
}


}//namespace opt
}//namespace open_edi