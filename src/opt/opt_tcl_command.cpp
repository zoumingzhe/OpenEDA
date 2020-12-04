#include "opt_tcl_command.h"
#include "OptimizeNet.h"
#include "OptimizeNets.h"

using namespace std;

namespace open_edi {
namespace opt {

static int optimizeNet(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    OptimizeNet *opt = new OptimizeNet();
    opt->optimize_net(argc,(char **)argv);
    delete opt;
    printMemoryInfo();
    return TCL_OK;
}

static int optimizeNets(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    OptimizeNets *opt = new OptimizeNets();
    if(opt->parseCommand(argc,(char **)argv)){
        cout << "cmd failed!" << endl;
        delete opt;
        return -1;
    }
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    opt->run(1,1,1);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    chrono::duration<double, std::milli> elapsed = end - begin;
    cout << "elapsed time: " << elapsed.count() << " [ms]" << endl;
    delete opt;
    printMemoryInfo();
    return TCL_OK;
}

void registerOptTclCommands(Tcl_Interp *itp) {
   Tcl_CreateCommand(itp, "optimize_net", optimizeNet, nullptr, nullptr);
   Tcl_CreateCommand(itp, "optimize_nets", optimizeNets, nullptr, nullptr);
}



void printMemoryInfo() {
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

}//namespace opt
}//namespace open_edi