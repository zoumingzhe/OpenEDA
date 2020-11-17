#ifndef GUI_TCL_COMMAND_H
#define GUI_TCL_COMMAND_H

#include <tcl.h>
#include <QApplication>
#include <QCoreApplication>


namespace open_edi {
namespace gui {

void registerGuiTclCommands(Tcl_Interp *itp);
void tclConsoleInit(Tcl_Interp *itp);
QCoreApplication* startQt(int &argc, char *argv[]);
} // namespace gui
} // namespace open_edi
#endif // GUI_TCL_COMMAND_H
