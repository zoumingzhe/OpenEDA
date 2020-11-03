#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QLabel>
#include <QStackedWidget>
#include "graphic_view.h"
#include "control_panel_widget.h"
#include "graphic_scene.h"
#include "log_widget.h"
#include "tool_bar.h"

class ToolBar;
class GraphicView;
class GraphicScene;

namespace open_edi {
namespace gui {

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initMenus();
    void initTools();
    void initControlPanel();
    void initDockWidgets();
    void initStatusBar();
    void initCentralWidget();
    virtual void closeEvent(QCloseEvent *e) override;
private:

    QStackedWidget           *main_stacked_wgt_;
    ToolBar             *h_tool_bar_;
    ToolBar             *v_tool_bar_;
    ControlPanelWidget   *control_panel_wgt_;
    GraphicScene         *layout_graphic_scene;
    GraphicView          *layout_graphic_view;
    LogWidget            *log_wgt_;
};


}
}
#endif // MAINWINDOW_H
