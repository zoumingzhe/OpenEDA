#include "li_instances.h"

namespace open_edi {
namespace gui {



LI_Instances::LI_Instances(int* scale_factor):LI_Base(scale_factor) 
{
    item_ = new LGI_Instances;

    pen_.setColor(QColor("#909090"));
}

LI_Instances::~LI_Instances() {
}

LGI_Instances* LI_Instances::getGraphicItem() {
    return item_;
}

void LI_Instances::update() {
    item_->update();
}

void LI_Instances::preDraw() {
    refreshBoundSize();

    img->fill(Qt::transparent);

    QPainter painter(img);
    painter.setPen(pen_);
    painter.setWindow(0,
                      bound_height,
                      bound_width + VIEW_SPACE,
                      -bound_height - VIEW_SPACE);

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

        painter.drawRect(QRectF(
          (insllx) / factor,
          (inslly) / factor,
          (insurx - insllx) / factor,
          (insury - inslly) / factor));
    }

    item_->setMap(img);
    item_->setItemSize(bound_width, bound_height);
}

} // namespace gui
} // namespace open_edi