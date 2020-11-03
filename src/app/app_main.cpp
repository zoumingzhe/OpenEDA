/* @file  app_main.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
/// @file appMain.cpp

#include "util/util.h"
#include "db/core/db.h"
#include "db/core/inst.h"
#include "db/core/net.h"
#include "db/core/db_tcl_command.h"
#include "tcl/test_app.h"
#include "gui/gui_tcl_command.h"






/// @brief registerSystemCommands 
///
/// @param itp
static void registerSystemCommands(Tcl_Interp *itp)
{
    open_edi::db::registerDatabaseTclCommands(itp);

    open_edi::tcl::registerTestCommands(itp);

}
/************************************
 * 1. Call initialization procedures for various packages used by the application.
 * Each initialization procedure adds new commands to interp for its package and performs
 * other package-specific initialization.
 * 2. Process command-line arguments, which can be accessed from the Tcl variables argv and argv0 in
 *interp.
 * 3. Invoke a startup script to initialize the application.
 * 4. Use the routines Tcl_SetStartupScript and Tcl_GetStartupScript to set or query the file
 * and encoding that the active Tcl_Main or Tk_Main routine will use as a startup script.
 ************************************/
int Tcl_AppInit(Tcl_Interp *itp) {

    if (Tcl_Init(itp) == TCL_ERROR) {
        return TCL_ERROR;
    }
    //add enhancement console
    open_edi::gui::tclConsoleInit(itp);
    registerSystemCommands(itp);

    return TCL_OK;
}

/// @brief appInit, put all application initializations here.
///
/// @param argc
/// @param argv[]
///
/// @return 
static bool initApplication(int argc, char *argv[]) {

    open_edi::util::setAppPath(argv[0]);
    open_edi::util::utilInit();
    open_edi::util::MemPool::initMemPool(); // initial MemPool
    open_edi::db::initTopCell(); // temporary put it here, TODO: remove it

    return true;
}


/************************************
 * Make application specific main entrance to make main() function neat.
 ************************************/
int appMain(int argc, char *argv[]) {


    initApplication(argc, argv);

    Tcl_Main(argc, argv, Tcl_AppInit);

    return 0;
}

/************************************
 * main() function.
 ************************************/
int main(int argc, char *argv[]) {
    
    appMain(argc, argv);

    // note that application never reachs here.
    exit(0);
}
