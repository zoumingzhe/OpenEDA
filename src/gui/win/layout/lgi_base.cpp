#include "lgi_base.h"
#include <QPainter>

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

QRectF LGI_Base::boundingRect() const {
    return QRectF(0, 0, w, h);
}

void LGI_Base::paint(QPainter*                       painter,
                     const QStyleOptionGraphicsItem* option,
                     QWidget*                        widget) {

    if (img) {
        painter->drawPixmap(-w / 2, -h / 2, *img);
    }
}

} // namespace gui
} // namespace open_edi
