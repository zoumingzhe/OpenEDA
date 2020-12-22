#include "li_base.h"

namespace open_edi {
namespace gui {

void LI_Base::refreshBoundSize() {
    if (bound_width != getGraphicItem()->scene()->width()
        || bound_height != getGraphicItem()->scene()->height()) {
        bound_width  = getGraphicItem()->scene()->width();
        bound_height = getGraphicItem()->scene()->height();

        QTransform matrix;
        matrix.translate(-bound_width / 2, -bound_height / 2);
        getGraphicItem()->setTransform(matrix);
#if DRAW_MODE == 1
        if (img) {
            delete img;
            img = new QPixmap(bound_width, bound_height);
        } else {
            img = new QPixmap(bound_width, bound_height);
        }
#endif
    }
}

void LI_Base::setVisible(bool visible) {
    this->visible = visible;
}

void LI_Base::draw(QPainter* painter) {
#if DRAW_MODE == 1
    if (img) {
        painter->drawPixmap(0, 0, *img);
    }
#elif DRAW_MODE == 2
    painter->drawPath(painter_path);
#endif
    printf("draw mode %s\n", 
    DRAW_MODE==IMG_MODE?"image mode":
    DRAW_MODE==PATH_MODE?"path mode":
    "no draw mode"
    );
}
} // namespace gui
} // namespace open_edi