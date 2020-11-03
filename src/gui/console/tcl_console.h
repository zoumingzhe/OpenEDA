#ifndef EDI_TCL_CONSOLE_H_
#define EDI_TCL_CONSOLE_H_

#include <tcl.h>
#include "tclreadline.h"

int TclreadlineAppInit(Tcl_Interp* interp, const char* install_path);

#endif