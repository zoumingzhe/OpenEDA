#include "elabel.h"
#include <QMouseEvent>

namespace open_edi {
namespace gui {

ELabel::ELabel(QWidget *parent)
    : QLabel(parent)
{
    is_Show_ = false;
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFocusPolicy(Qt::NoFocus);
    //setMouseTracking(true);
}

void ELabel::setPixmap(const QPixmap &pixmap)
{
     pixmap_ = pixmap;
     setPixmap(pixmap_);
     update();
}

void ELabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(is_Show_){
            emit hide();
            is_Show_ = false;
        }else{
            emit show();
            is_Show_ = true;
        }
    }
}

void ELabel::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    setCursor(Qt::ArrowCursor);
}

void  ELabel::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    setCursor(Qt::PointingHandCursor);
}


}
}

