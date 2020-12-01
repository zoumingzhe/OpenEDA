#include "opt_tcl_command.h"
#include "Optimize.h"

namespace open_edi {
namespace opt {

static int optimizeNet(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[]) {
    Optimize *opt = new Optimize();
    opt->optimize_net(argc,(char **)argv);
    delete opt;
    return TCL_OK;
}

void registerOptTclCommands(Tcl_Interp *itp) {
   Tcl_CreateCommand(itp, "optimize_net", optimizeNet, nullptr, nullptr);
}


}//namespace opt
}//namespace open_edi