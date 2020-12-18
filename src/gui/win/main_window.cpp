#include "main_window.h"
#include "widget/ribbon/ribbon.h"
#include "widget/ribbon/ribbon_title_bar.h"
#include "widget/ribbon/ribbon_page.h"
#include "widget/ribbon/ribbon_group.h"
#include "widget/mdi_window.h"
#include "common/action_handler.h"
#include "common/action_producer.h"
#include "common/action_group_manager.h"
#include "common/docks_manager.h"
#include "widget/mdi_window.h"
#include "util/util.h"

#include <QComboBox>
#include <QSpinBox>

namespace open_edi {
namespace gui {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , view_(new QGraphicsView)
    , mdi_area_(nullptr)
    , current_subwindow_(nullptr)
    , ribbon_(nullptr)
{
    setObjectName("MainWindow");
    setAcceptDrops(true);

    init();
    addActions();
    createCentralWindow();
    statusBar();
}

MainWindow::~MainWindow()
{
}


void MainWindow::init()
{
    ribbon_ = new RibbonMenuBar(this);
    setMenuBar(ribbon_);
    ribbon_->setFixedHeight(180);

    action_handler_ = new ActionHandler(this);
    action_handler_->setView(view_);
    action_manager_ = new ActionGroupManager(this);
    ActionProducer* producer = new ActionProducer(this, action_handler_);
    producer->fillActionContainer(action_map_, action_manager_);

    DocksManager* docks = new DocksManager(this);
    docks->createDockWidgets();

}

void MainWindow::addActions()
{
    QString res_path = QString::fromStdString(open_edi::util::getInstallPath()) + "/share/etc/res/tool/";
    QString res_name = res_path + "undo.svg";

    RibbonTitleBar* title_bar = ribbon_->getTitleBar();
    title_bar->addQuickAction(new QAction(QIcon(res_name), tr("undo")));
    title_bar->addQuickAction(new QAction(QIcon(res_name), tr("undo")));
    title_bar->addQuickAction(new QAction(QIcon(res_name), tr("undo")));
    title_bar->addSystemAction(new QAction(QIcon(res_name), tr("undo")));
    title_bar->addSystemAction(new QAction(QIcon(res_name), tr("undo")));
    title_bar->addSystemAction(new QAction(QIcon(res_name), tr("undo")));

    RibbonPage* page = ribbon_->addPage(tr("Common"));
    RibbonGroup* group = page->addGroup(tr("File"));
    group->addAction(action_map_["ImportDesign"], Qt::ToolButtonTextUnderIcon);
    group->addAction(action_map_["ZoomIn"], Qt::ToolButtonTextBesideIcon);

    group = page->addGroup(tr("View"));
    group->addAction(new QAction(QIcon(res_name), tr("undo")), Qt::ToolButtonTextUnderIcon);
    group->addAction(new QAction(QIcon(res_name), tr("undo")), 0,0,1,1);
    group->addAction(new QAction(QIcon(res_name), tr("undo")), 0,1,1,1);
    group->addAction(new QAction(QIcon(res_name), tr("undo")), 0,2,1,1);
    group->addAction(new QAction(QIcon(res_name), tr("undo")), 1,0,1,1);
    group->addAction(new QAction(QIcon(res_name), tr("undo")), 1,1,1,1);
    group->addAction(new QAction(QIcon(res_name), tr("undo")), 1,2,1,1);

    group = page->addGroup(tr("Misc"));
    group->addWidget(new QComboBox, 0,0,1,3);
    group->addWidget(new QSpinBox, 1,0,1,3);

    ribbon_->addPage(tr("FloorPlan"));
    ribbon_->addPage(tr("Timing"));
}


void MainWindow::createCentralWindow()
{
    mdi_area_ = new QMdiArea(this);
    setCentralWidget(mdi_area_);
    mdi_area_->setViewMode(QMdiArea::TabbedView);
    mdi_area_->setTabPosition(QTabWidget::South);
    mdi_area_->setTabsClosable(true);
    mdi_area_->setTabsMovable(true);
    mdi_area_->setMaximumWidth(800);

    MDIWindow* window = new MDIWindow(tr("Layout"));
    mdi_area_->addSubWindow(window);
    mdi_area_->addSubWindow(new MDIWindow(tr("Schematic")));
}

}
}
