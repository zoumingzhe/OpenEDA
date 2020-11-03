#ifndef GUI_TCL_COMMAND_H
#define GUI_TCL_COMMAND_H

#include <tcl.h>


namespace open_edi {
namespace gui {

void registerGuiTclCommands(Tcl_Interp *itp);
void setDisplayFlag(bool isDisplay);
void tclConsoleInit(Tcl_Interp *itp);
} // namespace db
} // namespace open_edi
#endif // GUI_TCL_COMMAND_H
