#include <QPainter>
#include <QTime>

#include "lgi_base.h"

namespace open_edi {
namespace gui {

LGI_Base::LGI_Base(/* args */) {
}

LGI_Base::~LGI_Base() {
}

void LGI_Base::setItemSize(int w, int h) {
    this->w = w;
    this->h = h;
}

void LGI_Base::setMap(QPixmap* img) {
    this->img = img;
}

void LGI_Base::setLiBase(LI_Base* li_base) {
    this->li_base = li_base;
}

QRectF LGI_Base::boundingRect() const {
    return QRectF(0 , 0 , w, h);
}

void LGI_Base::paint(QPainter*                       painter,
                     const QStyleOptionGraphicsItem* option,
                     QWidget*                        widget) {
    QTime time;
    time.start();

    li_base->draw(painter);
    auto time_elapsed = time.elapsed();
    printf("paint elapsed time %d (ms)\n", time_elapsed);
}

} // namespace gui
} // namespace open_edi
