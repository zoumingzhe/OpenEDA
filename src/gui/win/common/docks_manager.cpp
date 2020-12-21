#include "docks_manager.h"
#include "../main_window.h"
#include "../widget/layer/layer_widget.h"
#include <QDockWidget>
#include <QTextEdit>

namespace open_edi {
namespace gui {

DocksManager::DocksManager(QObject *parent)
    : QObject(parent)
{
    main_window_ = qobject_cast<MainWindow*>(parent);
}


void DocksManager::createDockWidgets()
{
    QDockWidget* layersDock = new QDockWidget(main_window_);
    layersDock->setObjectName("LayersDock");
    layersDock->setWindowTitle(tr("Layer"));
    LayerWidget* layerWidget = new LayerWidget(layersDock);
    layerWidget->setFocusPolicy(Qt::NoFocus);
    layersDock->setWidget(layerWidget);

    QDockWidget* componentDock = new QDockWidget(main_window_);
    componentDock->setObjectName("ComponentDock");
    componentDock->setWindowTitle(tr("Component"));
    QTextEdit *edit1 = new QTextEdit;
    componentDock->setWidget(edit1);

    QDockWidget* designBroswer = new QDockWidget(main_window_);
    designBroswer->setObjectName("DesignBroswer");
    designBroswer->setWindowTitle(tr("Design Broswer"));
    QTextEdit *edit2 = new QTextEdit;
    designBroswer->setWidget(edit2);

    QDockWidget* floorplanBox = new QDockWidget(main_window_);
    floorplanBox->setObjectName("FloorplanBox");
    floorplanBox->setWindowTitle(tr("Floorplan Box"));
    QTextEdit *edit3 = new QTextEdit;
    floorplanBox->setWidget(edit3);

    QDockWidget* worldView = new QDockWidget(main_window_);
    worldView->setObjectName("WorldView");
    worldView->setWindowTitle(tr("World View"));
    QTextEdit *edit4 = new QTextEdit;
    worldView->setWidget(edit4);

    QDockWidget* commonAction = new QDockWidget(main_window_);
    commonAction->setObjectName("CommonAction");
    commonAction->setWindowTitle(tr("Common Action"));
    QTextEdit *edit5 = new QTextEdit;
    commonAction->setWidget(edit5);

    main_window_->addDockWidget(Qt::LeftDockWidgetArea, designBroswer);
    main_window_->addDockWidget(Qt::LeftDockWidgetArea, floorplanBox);
    main_window_->addDockWidget(Qt::LeftDockWidgetArea, commonAction);

    main_window_->addDockWidget(Qt::RightDockWidgetArea, layersDock);
    main_window_->addDockWidget(Qt::RightDockWidgetArea, componentDock);
    main_window_->addDockWidget(Qt::RightDockWidgetArea, worldView);
}


}
}
