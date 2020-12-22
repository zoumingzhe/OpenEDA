#include "li_instances.h"

namespace open_edi {
namespace gui {

LI_Instances::LI_Instances(int* scale_factor) : LI_Base(scale_factor) {
    item_ = new LGI_Instances;
    item_->setLiBase(this);
    pen_.setColor(QColor("#909090"));
    type    = kInstance;
    li_pins = new LI_Pins(scale_factor);
}

LI_Instances::~LI_Instances() {
}

LGI_Instances* LI_Instances::getGraphicItem() {
    return item_;
}

void LI_Instances::update() {
    item_->update();
}

void LI_Instances::draw(QPainter* painter) {
    painter->setPen(pen_);
    LI_Base::draw(painter);
}

void LI_Instances::preDraw() {
    refreshBoundSize();
    li_pins->refreshBoundSize();
    li_pins->fillImage();

#if DRAW_MODE == 1
    img->fill(Qt::transparent);
    QPainter painter(img);
    painter.setPen(pen_);
    painter.setWindow(0,
                      bound_height,
                      bound_width + VIEW_SPACE,
                      -bound_height - VIEW_SPACE);
#elif DRAW_MODE == 2
    painter_path = QPainterPath();
#endif

    auto     tc               = open_edi::db::getTopCell();
    uint64_t num_components   = tc->getNumOfInsts();
    auto     components       = tc->getInstances();
    auto     component_vector = open_edi::db::Object::addr<open_edi::db::ArrayObject<open_edi::db::ObjectId>>(components);

    auto factor = *scale_factor;
    // printf("COMPONENTS %d ;\n", num_components);
    for (auto iter = component_vector->begin(); iter != component_vector->end(); ++iter) {
        auto instance = open_edi::db::Object::addr<open_edi::db::Inst>(*iter);
        auto insbox   = instance->getBox();
        auto insllx   = insbox.getLLX();
        auto inslly   = insbox.getLLY();
        auto insurx   = insbox.getURX();
        auto insury   = insbox.getURY();
        auto width    = insurx - insllx;
        auto height   = insury - inslly;
#if DRAW_MODE == 1
        if (width >= factor >> 2 || height >= factor >> 2) {
            painter.drawRect(QRectF(
              (insllx) / factor,
              (inslly) / factor,
              width / factor,
              height / factor));
            if (width > factor * 4 && height > factor * 4) {
                li_pins->drawPins(*instance);
            }
        }
#elif DRAW_MODE == 2
        if (width >= factor || height >= factor) {
            painter_path.addRect(QRectF(
              (insllx) / factor,
              (inslly) / factor,
              width / factor,
              height / factor));
        }
#endif
    }

    item_->setMap(img);
    item_->setItemSize(bound_width, bound_height);

    li_pins->getGraphicItem()->setMap(img);
    li_pins->getGraphicItem()->setItemSize(bound_width, bound_height);
}

} // namespace gui
} // namespace open_edi