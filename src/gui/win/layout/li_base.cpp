#include "li_base.h"

namespace open_edi {
namespace gui {


void LI_Base::refreshBoundSize() {
    if (bound_width != getGraphicItem()->scene()->width()
        || bound_height != getGraphicItem()->scene()->height()) {
        bound_width  = getGraphicItem()->scene()->width();
        bound_height = getGraphicItem()->scene()->height();
        if (img) {
            delete img;
            img = new QPixmap(bound_width, bound_height);
        } else {
            img = new QPixmap(bound_width, bound_height);
        }
    }
}

void LI_Base::setVisible(bool visible) {
    this->visible = visible;
}
} // namespace gui
} // namespace open_edi