#include "li_die_area.h"
#include <QPainter>

namespace open_edi {
namespace gui {

LI_DieArea::LI_DieArea(int* scale_factor) : LI_Base(scale_factor) {
    item_ = new LGI_DieArea;
    item_->setLiBase(this);
    pen_.setColor(QColor("#909090"));

    type = kDieArea;
}

LI_DieArea::~LI_DieArea() {
}

void LI_DieArea::preDraw() {
    refreshBoundSize();
#if DRAW_MODE== 1
    img->fill(Qt::transparent);
    QPainter painter(img);
    painter.setPen(pen_);
    painter.setWindow(0,
                      bound_height,
                      bound_width + VIEW_SPACE,
                      -bound_height - VIEW_SPACE);
    painter.drawRect(QRectF(0, 0, bound_width, bound_height));
#elif DRAW_MODE== 2
    painter_path = QPainterPath();
    painter_path.addRect(QRectF(0, 0, bound_width, bound_height));
#endif
    item_->setMap(img);
    item_->setItemSize(bound_width, bound_height);
}

LGI_DieArea* LI_DieArea::getGraphicItem() {
    return item_;
}

void LI_DieArea::update() {
    item_->update();
}

void LI_DieArea::draw(QPainter* painter) 
{  
    painter->setPen(pen_);
    LI_Base::draw(painter);
}

} // namespace gui
} // namespace open_edi