#include "ribbon_button.h"
#include <QMouseEvent>
#include <QAction>

namespace open_edi {
namespace gui {

RibbonButton::RibbonButton(QWidget *parent):QToolButton (parent)
{
    setObjectName("RibbonToolButton");
    setToolButtonStyle(Qt::ToolButtonIconOnly);
    setAutoRaise(true);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

RibbonButton::~RibbonButton(){

}

void RibbonButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        QAction *action = defaultAction();
        if(action && action->menu()){
            emit menuActionClicked(e);
        }

        update();
    }
}

}
}
