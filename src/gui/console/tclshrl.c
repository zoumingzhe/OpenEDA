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


extern int Tclreadline_Init(Tcl_Interp *interp);
extern int Tclreadline_SafeInit(Tcl_Interp *interp);
extern void TclsetInstallPath(const char* installPath);

int
TclreadlineAppInit(Tcl_Interp* interp, const char* installPath)
{
    char rcPath[0xff] = {0};
    char initPath[0xff] = {0};

    int status;
    // if (TCL_ERROR == Tcl_Init(interp)) {
    //     return TCL_ERROR;
    // }

    TclsetInstallPath(installPath);

    if (TCL_ERROR == Tclreadline_Init(interp)) {
        return TCL_ERROR;
    }
    Tcl_StaticPackage(interp, "tclreadline",
                      Tclreadline_Init, Tclreadline_SafeInit);

    strcpy(rcPath,installPath);
    strcat(rcPath,"/src/gui/console/lib/config.tclshrc");
    Tcl_SetVar(interp, "tcl_rcFileName", rcPath, TCL_GLOBAL_ONLY);

    
    strcpy(initPath,installPath);
    strcat(initPath,"/src/gui/console/lib/tclreadlineInit.tcl");
    if(access(initPath,F_OK) != 0) {
        memset(initPath, 0, 255);
        strcpy(initPath,installPath);
        strcat(initPath,"/include/src/gui/console/lib/tclreadlineInit.tcl");
    }
    
    if ((status = Tcl_EvalFile(interp, initPath))) {
        fprintf(stderr, "(TclreadlineAppInit) unable to eval %s\n", initPath);
        exit (EXIT_FAILURE);
    }
    return TCL_OK;
}

// int
// main(int argc, char *argv[])
// {
//     Tcl_Main(argc, argv, TclreadlineAppInit);
//     return EXIT_SUCCESS;
// }

