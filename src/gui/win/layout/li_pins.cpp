#include "li_pins.h"

namespace open_edi {
namespace gui {
LI_Pins::LI_Pins(int* scale_factor) : LI_Base(scale_factor) {
    item_ = new LGI_Pins;

    pen_.setColor(QColor(0, 0, 0xff, 0xff));
}

LI_Pins::~LI_Pins() {
}

LGI_Pins* LI_Pins::getGraphicItem() {
    return item_;
}

void LI_Pins::update() {
    item_->update();
}

void LI_Pins::__drawPins(open_edi::db::Inst& ins) {
    if (ins.numPins()) {
        // printf("instance number of pins %d\n", ins.numPins());

        auto pins        = ins.getPins();
        auto location    = ins.getLocation();
        auto pins_vector = open_edi::db::Object::addr<open_edi::db::ArrayObject<open_edi::db::ObjectId>>(pins);
        auto factor      = *scale_factor;
        for (auto iter = pins_vector->begin(); iter != pins_vector->end(); ++iter) {
            auto pin      = open_edi::db::Object::addr<open_edi::db::Pin>(*iter);
            auto term     = pin->getTerm();
            auto port_num = term->getPortNum();
            // printf("PORTS %d ;\n", port_num);

            for (int i = 0; i < port_num; i++) {
                auto port               = term->getPort(i);
                auto layer_geometry_num = port->getLayerGeometryNum();
                // printf("layer_geometry_num %d ;\n", layer_geometry_num);
                for (int j = 0; j < layer_geometry_num; j++) {
                    auto laryer_geometry = port->getLayerGeometry(j);
                    // printf("port location x %d y %d\n", location.getX(), location.getY());
                    // printf("getVecNum %d ;\n", laryer_geometry->getVecNum());
                    for (int k = 0; k < laryer_geometry->getVecNum(); k++) {
                        auto geo     = laryer_geometry->getGeometry(k);
                        auto box     = geo->getBox();
                        auto box_llx = box.getLLX();
                        auto box_lly = box.getLLY();
                        auto box_urx = box.getURX();
                        auto box_ury = box.getURY();

                        int pin_llx, pin_lly, pin_urx, pin_ury;

                        // printf("port getOrient %d\n", port->getOrient());

                        pin_llx = qCeil((box_llx + location.getX()) / factor);
                        pin_lly = qCeil((box_lly + location.getY()) / factor);
                        pin_urx = qCeil((box_urx + location.getX()) / factor);
                        pin_ury = qCeil((box_ury + location.getY()) / factor);

                        if (img) {
                            QPainter painter(img);
                            painter.setWindow(0,
                                              bound_height,
                                              bound_width + VIEW_SPACE,
                                              -bound_height - VIEW_SPACE);

                            painter.setPen(pen_);

                            painter.fillRect(pin_llx,
                                             pin_lly,
                                             pin_urx - pin_llx,
                                             pin_ury - pin_lly,
                                             QBrush(QColor(0, 0, 0xff, 0xff), Qt::Dense5Pattern));
                            //left
                            painter.drawLine(QPointF(pin_llx, pin_lly), QPointF(pin_llx, pin_ury));
                            //top
                            painter.drawLine(QPointF(pin_llx, pin_ury), QPointF(pin_urx, pin_ury));
                            //right
                            painter.drawLine(QPointF(pin_urx, pin_ury), QPointF(pin_urx, pin_lly));
                            //bottom
                            painter.drawLine(QPointF(pin_llx, pin_lly), QPointF(pin_urx, pin_lly));
                        }
                    }
                }
            }
        }
    }
}

void LI_Pins::preDraw() {

    refreshBoundSize();

    item_->setVisible(visible);

    if (!visible) {

        return;
    }

    if (img) {
        img->fill(Qt::transparent);
        auto     tc               = open_edi::db::getTopCell();
        uint64_t num_components   = tc->getNumOfInsts();
        auto     components       = tc->getInstances();
        auto     component_vector = open_edi::db::Object::addr<open_edi::db::ArrayObject<open_edi::db::ObjectId>>(components);
        // printf("COMPONENTS %d ;\n", num_components);
        for (auto iter = component_vector->begin(); iter != component_vector->end(); ++iter) {
            auto instance = open_edi::db::Object::addr<open_edi::db::Inst>(*iter);
            __drawPins(*instance);
        }
    }

    item_->setMap(img);
    item_->setItemSize(bound_width, bound_height);
}

} // namespace gui
} // namespace open_edi