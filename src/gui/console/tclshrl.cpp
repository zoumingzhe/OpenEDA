 /* ================================================================== *
  * FILE: tclshrl.c
  * $Id: b4713753d4536865a148877773f7a0df7275e8ab $
  * ---
  * tclreadline -- gnu readline for tcl
  * https://github.com/flightaware/tclreadline/
  * Copyright (c) 1998 - 2014, Johannes Zellner <johannes@zellner.org>
  * This software is copyright under the BSD license.
  * ================================================================== */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <tcl.h>

namespace open_edi {
namespace gui {

extern int Tclreadline_Init(Tcl_Interp *interp);
extern int Tclreadline_SafeInit(Tcl_Interp *interp);
extern void TclsetHistoryPath(const char* historyPath);

int
TclreadlineAppInit(Tcl_Interp* interp, const char* installPath)
{
    char rcPath[0xff] = {0};
    char setupPath[0xff] = {0};
    char completerPath[0xff] = {0};
    int status;


    if (TCL_ERROR == Tclreadline_Init(interp)) {
        return TCL_ERROR;
    }
    Tcl_StaticPackage(interp, "tclreadline",
                      Tclreadline_Init, Tclreadline_SafeInit);

    TclsetHistoryPath(installPath);


    strcpy(rcPath,installPath);
    strcat(rcPath,"share/etc/tcl/init.tcl");

    Tcl_SetVar(interp, "tcl_rcFileName", rcPath, TCL_GLOBAL_ONLY);
    Tcl_Eval(interp,"::tclreadline::readline customcompleter ::tclreadline::ScriptCompleter");


    strcpy(setupPath,installPath);
    strcat(setupPath,"share/etc/tcl/tclreadlineSetup.tcl");

    if ((status = Tcl_EvalFile(interp,setupPath))) {
        fprintf(stderr, "(tclreadlineSetup) unable to eval %s\n", setupPath);
        exit (EXIT_FAILURE);       
    }

    strcpy(completerPath,installPath);
    strcat(completerPath,"share/etc/tcl/tclreadlineCompleter.tcl");

    if ((status = Tcl_EvalFile(interp,completerPath))) {
        fprintf(stderr, "(tclreadlineCompleter) unable to eval %s\n", completerPath);
        exit (EXIT_FAILURE);       
    }

    return TCL_OK;
}

} // namespace gui
} // namespace open_edi

