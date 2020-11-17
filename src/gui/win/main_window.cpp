#include "main_window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QLineEdit>
#include <QSpacerItem>
#include <QDialog>

namespace open_edi {
namespace gui {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
   setObjectName("MainWindow");
   setWindowTitle("Main Window");

   initCentralWidget();
   initMenus();
   initTools();
   initDockWidgets();
   initStatusBar();
}

MainWindow::~MainWindow()
{
    delete main_stacked_wgt_;
    main_stacked_wgt_ = nullptr;
    delete h_tool_bar_;
    h_tool_bar_ = nullptr;
    delete  v_tool_bar_;
    v_tool_bar_ = nullptr;
    delete control_panel_wgt_;
    control_panel_wgt_ = nullptr;
    delete  layout_graphic_scene;
    layout_graphic_scene = nullptr;
    delete  layout_graphic_view;
    layout_graphic_view = nullptr;
    delete  log_wgt_;
    log_wgt_ = nullptr;
}

void MainWindow::initCentralWidget()
{
    main_stacked_wgt_ = new QStackedWidget(this);
    setCentralWidget(main_stacked_wgt_);

    layout_graphic_view = new GraphicView;
    layout_graphic_scene = new GraphicScene;
    layout_graphic_view->setScene(layout_graphic_scene);
    layout_graphic_view->setMinimumSize(700,500);
    main_stacked_wgt_->addWidget(layout_graphic_view);
}

void MainWindow::initMenus()
{
    QMenu *menuMenu = menuBar()->addMenu(tr("File"));
    menuMenu->addAction(tr("Import Design"));
    menuMenu->addAction(tr("Save Design"));
    menuMenu->addSeparator();
    menuMenu->addAction(tr("Exit"));

    QMenu *menuEdit = menuBar()->addMenu(tr("Edit"));
    menuEdit->addAction(tr("Undo"));
    menuEdit->addAction(tr("Redo"));
    menuEdit->addAction(tr("Copy"));
    menuEdit->addAction(tr("Move"));
    menuEdit->addAction(tr("Wirte"));
    menuEdit->addAction(tr("Pin"));

    QMenu *menuEco = menuBar()->addMenu(tr("Eco"));
    menuEco->addAction(tr("Add Buffer"));
    menuEco->addAction(tr("Add Repeater"));
    menuEco->addAction(tr("Size Cell"));
    menuEco->addAction(tr("Remove Repeater"));
    menuEco->addAction(tr("Remove Buffer"));

    QMenu *menuView = menuBar()->addMenu(tr("View"));
    menuView->addAction(tr("Congestion Map"));
    menuView->addAction(tr("Density Map"));
    menuView->addAction(tr("Pin Density Map"));
    menuView->addAction(tr("Redraw"));
    menuView->addAction(tr("Zoom In"));
    menuView->addAction(tr("Zoom Out"));
    menuView->addAction(tr("Zoom Fit"));
    menuView->addAction(tr("Zoom to Select"));

    QMenu *menuCheck = menuBar()->addMenu(tr("Check"));
    menuCheck->addAction(tr("Check DRC"));
    menuCheck->addAction(tr("Check Connectivity"));
    menuCheck->addAction(tr("Check Process Antenna"));
    menuCheck->addAction(tr("Check AC Limit"));
    menuCheck->addAction(tr("Check Power Via"));
    menuCheck->addAction(tr("Check Design"));
    menuCheck->addAction(tr("Check Placement"));

    QMenu *menuPlace = menuBar()->addMenu(tr("Place"));
    menuPlace->addAction(tr("Place Standard Cell"));
    QMenu *menu2Physical = menuPlace->addMenu(tr("Physical Cell"));
    menu2Physical->addAction(tr("Add Well Tap"));
    menu2Physical->addAction(tr("Add End Cap"));
    menu2Physical->addAction(tr("Add Filler"));
    menu2Physical->addAction(tr("Add Tie Hi/Low"));
    menu2Physical->addAction(tr("Remove Filler"));
    menuPlace->addAction(tr("Place Spare Cell"));
    QMenu *menu2Power = menuPlace->addMenu(tr("Physical Cell"));
    menu2Power->addAction(tr("Add Power Switch"));
    menu2Power->addAction(tr("Add Level Shifter"));
    menu2Power->addAction(tr("Add Isolation"));
    menuPlace->addAction(tr("Incremental Place"));
    menuPlace->addAction(tr("Legalize Placement"));
    menuPlace->addAction(tr("Scan Chain Reorder"));
    menuPlace->addAction(tr("Add Place Blockage"));
    menuPlace->addAction(tr("Create Place Region"));
    menuPlace->addAction(tr("Add Cell Padding"));

    QMenu *menuRoute = menuBar()->addMenu(tr("Route"));
    menuRoute->addAction(tr("Route Global"));
    menuRoute->addAction(tr("Route Track"));
    menuRoute->addAction(tr("Route Detail"));
    menuRoute->addAction(tr("Create Routing Rule"));
    menuRoute->addAction(tr("Route Design"));
    menuRoute->addAction(tr("Route Clock"));
    menuRoute->addAction(tr("Route 2nd PG net"));
    menuRoute->addAction(tr("Create Routing Blockage"));
    menuRoute->addAction(tr("Add Metal Fill"));
    menuRoute->addAction(tr("Add Via Fill"));
    menuRoute->addAction(tr("Remove Metal Fill"));
    menuRoute->addAction(tr("Remove Via Fill"));
    menuRoute->addAction(tr("Report Congestion"));
    menuRoute->addAction(tr("Special Route"));

    QMenu *menuTools = menuBar()->addMenu(tr("Tools"));
    menuTools->addAction(tr("Clock Tree Debugger"));
    menuTools->addAction(tr("Timing Analysis Window"));
    menuTools->addAction(tr("Schematic Viewer"));
    menuTools->addAction(tr("Design Browser"));
    menuTools->addAction(tr("Violation Browser"));

    QMenu *menuFloorplan= menuBar()->addMenu(tr("Floorplan"));
    menuFloorplan->addAction(tr("Initialize Floorplan"));
    menuFloorplan->addAction(tr("Create Row"));
    menuFloorplan->addAction(tr("Cut Row"));
    menuFloorplan->addAction(tr("Floorplan Toolbox"));

    QMenu *menuHelp = menuBar()->addMenu(tr("Help"));
    menuHelp->addAction(tr("User Guide"));
    menuHelp->addAction(tr("Command Reference"));
    menuHelp->addAction(tr("About"));

}

void MainWindow::initTools()
{
    h_tool_bar_ = new ToolBar("Horizonal Tool Bar", this);
    v_tool_bar_ = new ToolBar("Vertical Tool Bar", this);
    addToolBar(Qt::TopToolBarArea, h_tool_bar_);
    addToolBar(Qt::LeftToolBarArea, v_tool_bar_);
}

void MainWindow::initDockWidgets()
{
    control_panel_wgt_ = new ControlPanelWidget(tr("Layer Control"), this, Qt::WindowFlags(nullptr));
    addDockWidget(Qt::RightDockWidgetArea, control_panel_wgt_);
    log_wgt_ = new LogWidget(tr("Log"), this, Qt::WindowFlags(nullptr));
    log_wgt_->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
    addDockWidget(Qt::BottomDockWidgetArea, log_wgt_);

    QMenu *menuWindow = menuBar()->addMenu(tr("Window"));
    menuWindow->addAction(control_panel_wgt_->toggleViewAction());
    menuWindow->addAction(log_wgt_->toggleViewAction());

    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
}

void MainWindow::initStatusBar()
{
   QLineEdit *mouseLocate = new QLineEdit;
   mouseLocate->setMaximumWidth(150);
   mouseLocate->setReadOnly(true);
   statusBar()->addPermanentWidget(mouseLocate);
}


void MainWindow::closeEvent(QCloseEvent *e)
{
    hide();
    e ->ignore();
}


}
}
