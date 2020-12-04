#ifndef OPT_TCL_COMMAND_H
#define OPT_TCL_COMMAND_H

#include <tcl.h>


namespace open_edi {
namespace opt {

void registerOptTclCommands(Tcl_Interp *itp);
void printMemoryInfo();

} // namespace opt
} // namespace open_edi

#endif