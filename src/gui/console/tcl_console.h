#ifndef EDI_TCL_CONSOLE_H_
#define EDI_TCL_CONSOLE_H_

#include <tcl.h>
#include "tclreadline.h"
namespace open_edi {
namespace gui {

int TclreadlineAppInit(Tcl_Interp* interp, const char* install_path);

} // namespace gui
} // namespace open_edi
#endif