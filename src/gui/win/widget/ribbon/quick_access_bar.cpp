
#include "quick_access_bar.h"
#include "ribbon_button.h"
#include <QMouseEvent>

namespace open_edi {
namespace gui {

QuickAccessBar::QuickAccessBar(QWidget* parent)
    : QToolBar(parent)
    , menu_(nullptr)
    , separ_action_(nullptr)
{
    setObjectName("QuickAccessBar");
    init();
}

QuickAccessBar::~QuickAccessBar()
{
}

void QuickAccessBar::init()
{
    RibbonButton *arrowButton = new RibbonButton();
    arrowButton->setIcon(QIcon(":/res/tool/redo.svg"));
    arrowButton->setToolTip(tr("Quick Access"));
    connect(arrowButton, SIGNAL(menuActionClicked(QMouseEvent*)),
            this, SLOT(quickMenuriggered(QMouseEvent*)));

    arrow_action_ = addWidget(arrowButton);
    menu_ = new QMenu(arrowButton);
    menu_->setTitle(tr("Customize Quick Access Bar"));
    separ_action_ = menu_->addSeparator();
}

void QuickAccessBar::quickMenuriggered(QMouseEvent *e)
{
   RibbonButton *button = qobject_cast<RibbonButton*>(sender());
   if(button){
       int x = e->x();
       int y = e->y();
       QPoint pos = e->globalPos();
       pos.setX(pos.x() - x);
       pos.setY(pos.y() - y + button->height());
       menu_->popup(pos);
   }
}

void QuickAccessBar::menuTriggered(QMouseEvent *e)
{
    RibbonButton *button = qobject_cast<RibbonButton*>(sender());
    if(button){
        QAction *action = button->defaultAction();
        QMenu *menu = action->menu();
        if(!menu)
            return;

        int x = e->x();
        int y = e->y();
        QPoint pos = e->globalPos();
        pos.setX(pos.x() - x);
        pos.setY(pos.y() - y + button->height());
        menu->popup(pos);
    }
}

void QuickAccessBar::addAction(QAction* action){

    QAction *checkAction = new QAction(action->text(), this);
    checkAction->setCheckable(true);
    checkAction->setChecked(true);

//    RibbonButton *button = new RibbonButton();
//    button->setIcon(action->icon());
//    button->setDefaultAction(action);
//    connect(button, SIGNAL(menuActionClicked(QMouseEvent*)), this, SLOT(menuTriggered(QMouseEvent*)));

    menu_->insertAction(separ_action_, checkAction);
 //   connect(checkAction, SIGNAL(toggled(bool)), button, SLOT(setvisible(bool)));

    insertAction(arrow_action_, action);
}

QAction* QuickAccessBar::addAction(const QIcon &icon, const QString &text)
{
    QAction *action = new QAction(icon, text);
    addAction(action);
    return action;
}

void QuickAccessBar::setArrowVisible(bool visible)
{
    arrow_action_->setVisible(visible);
}

}
}
