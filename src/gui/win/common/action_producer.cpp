#include "action_producer.h"
#include "action_group_manager.h"
#include "action_handler.h"
#include "util/util.h"

#include <QAction>
#include <QActionGroup>

namespace open_edi {
namespace gui {

ActionProducer::ActionProducer(QObject *parent, QObject *handler)
    : QObject(parent)
    , action_hander_(handler)
    , main_window_(parent)
{    
}

void ActionProducer::fillActionContainer(QMap<QString, QAction*> &map, ActionGroupManager *manager)
{
    QString res_path = QString::fromStdString(open_edi::util::getInstallPath()) + "/share/etc/res/tool/";
    QString res_name = res_path + "undo.svg";

    QAction *action = nullptr;

    /*************file****************************/

    action = new QAction(tr("Import Design"), manager->file_);
    action->setIcon(QIcon(res_name));
    connect(action, SIGNAL(triggered()), action_hander_, SLOT(slotImportDesign()));
    action->setObjectName("ImportDesign");
    map["ImportDesign"] = action;

    action = new QAction(tr("Save Design"), manager->file_);
    action->setIcon(QIcon(res_name));
    connect(action, SIGNAL(triggered()), action_hander_, SLOT(slotSaveDesign()));
    action->setObjectName("SaveDesign");
    map["SaveDesign"] = action;

    action = new QAction(tr("Set Preference"), manager->file_);
    action->setIcon(QIcon(res_name));
    connect(action, SIGNAL(triggered()), action_hander_, SLOT(slotSetPreference()));
    action->setObjectName("SetPreference");
    map["SetPreference"] = action;

    action = new QAction(tr("Find/Select Object"), manager->file_);
    action->setIcon(QIcon(res_name));
    connect(action, SIGNAL(triggered()), action_hander_, SLOT(slotFindSelectObject()));
    action->setObjectName("FindSelectObject");
    map["FindSelectObject"] = action;

    /*************edit****************************/

    action = new QAction(tr("Undo"), manager->edit_);
    action->setIcon(QIcon(res_name));
    connect(action, SIGNAL(triggered()), action_hander_, SLOT(slotEditUndo()));
    action->setObjectName("Undo");
    map["Undo"] = action;

    action = new QAction(tr("Redo"), manager->edit_);
    action->setIcon(QIcon(res_name));
    connect(action, SIGNAL(triggered()), action_hander_, SLOT(slotEditRedo()));
    action->setObjectName("Redo");
    map["Redo"] = action;

    action = new QAction(tr("HighLight"), manager->edit_);
    action->setIcon(QIcon(res_name));
    connect(action, SIGNAL(triggered()), action_hander_, SLOT(slotEditHighLight()));
    action->setObjectName("HighLight");
    map["HighLight"] = action;

    /*************view****************************/

    action = new QAction(tr("Zoom In"), manager->view_);
    action->setIcon(QIcon(res_name));
    connect(action, SIGNAL(triggered()), action_hander_, SLOT(slotViewZoomIn()));
    action->setObjectName("ZoomIn");
    map["ZoomIn"] = action;

    action = new QAction(tr("Zoom Out"), manager->view_);
    action->setIcon(QIcon(res_name));
    connect(action, SIGNAL(triggered()), action_hander_, SLOT(slotViewZoomOut()));
    action->setObjectName("ZoomOut");
    map["ZoomOut"] = action;
}


}
}
