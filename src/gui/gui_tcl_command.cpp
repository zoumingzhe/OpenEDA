
#include "gui_tcl_command.h"
#include "main_window.h"
#include "util/util.h"
extern "C"{
#include "console/tcl_console.h"
}

namespace open_edi {
namespace gui {

static MainWindow *w = nullptr;
static bool display;

static int showGUI(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[])
{
    if(display)
    {
        if(w == nullptr)
        {
            w = new MainWindow(nullptr);
        }
        w->show();
    }
    else
    {
        printf("can not show gui in no-gui mode.\n");
    }
    return TCL_OK;
}

// Temporarily put her. TODO: move it after main window is added in.
static int hideGUI(ClientData cld, Tcl_Interp *itp, int argc, const char *argv[])
{
    if(display)
    {
        w->hide();
    }

    return TCL_OK;
}


void registerGuiTclCommands(Tcl_Interp *itp)
{
   Tcl_CreateCommand(itp, "show_gui", showGUI, nullptr, nullptr);
   Tcl_CreateCommand(itp, "hide_gui", hideGUI, nullptr, nullptr);
}

void setDisplayFlag(bool isDisplay)
{
    display = isDisplay;
}

void tclConsoleInit(Tcl_Interp *itp)
{
    TclreadlineAppInit(itp,open_edi::util::getInstallPath());
}


} // namespace db
} // namespace open_edi
