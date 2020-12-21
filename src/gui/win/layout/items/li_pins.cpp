#include "li_pins.h"

namespace open_edi {
namespace gui {
LI_Pins::LI_Pins(int* scale_factor) : LI_Base(scale_factor) {
    item_ = new LGI_Pins;
    item_->setLiBase(this);
    pen_.setColor(QColor(0, 0, 0xff, 0xff));
    brush_ = QBrush(QColor(0, 0, 0xff, 0xff), Qt::Dense5Pattern);
}

LI_Pins::~LI_Pins() {
}

LGI_Pins* LI_Pins::getGraphicItem() {
    return item_;
}

void LI_Pins::update() {
    item_->update();
}

void LI_Pins::draw(QPainter* painter) {
    painter->setPen(pen_);
    painter->setBrush(brush_);
    LI_Base::draw(painter);
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

                        float pin_llx, pin_lly, pin_urx, pin_ury;

                        // printf("port getOrient %d\n", port->getOrient());
                        auto width  = box_urx - box_llx;
                        auto height = box_ury - box_lly;

#if DRAW_MODE == 1
                        QPainter painter(img);
                        if (width >= factor || height >= factor) {
                            pin_llx = ((box_llx + location.getX()) / factor);
                            pin_lly = ((box_lly + location.getY()) / factor);
                            pin_urx = ((box_urx + location.getX()) / factor);
                            pin_ury = ((box_ury + location.getY()) / factor);

                            painter.setWindow(0,
                                              bound_height,
                                              bound_width + VIEW_SPACE,
                                              -bound_height - VIEW_SPACE);

                            painter.setPen(pen_);

                            painter.fillRect(QRectF(pin_llx,
                                                    pin_lly,
                                                    pin_urx - pin_llx,
                                                    pin_ury - pin_lly),
                                             QBrush(QColor(0, 0, 0xff, 0xff), Qt::Dense5Pattern));
                            //left
                            painter.drawLine(QPointF(pin_llx, pin_lly), QPointF(pin_llx, pin_ury));
                            //top
                            painter.drawLine(QPointF(pin_llx, pin_ury), QPointF(pin_urx, pin_ury));
                            //right
                            painter.drawLine(QPointF(pin_urx, pin_ury), QPointF(pin_urx, pin_lly));
                            //bottom
                            painter.drawLine(QPointF(pin_llx, pin_lly), QPointF(pin_urx, pin_lly));

                        } else {
                            painter.drawPoint(QPointF(pin_llx, pin_lly));
                        }

#elif DRAW_MODE == 2
                        if (width >= factor || height >= factor) {
                            pin_llx = qCeil((box_llx + location.getX()) / factor);
                            pin_lly = qCeil((box_lly + location.getY()) / factor);
                            pin_urx = qCeil((box_urx + location.getX()) / factor);
                            pin_ury = qCeil((box_ury + location.getY()) / factor);
                            painter_path.addRect(QRectF(pin_llx,
                                                        pin_lly,
                                                        pin_urx - pin_llx,
                                                        pin_ury - pin_lly));
                            //left
                            painter_path.moveTo(QPointF(pin_llx, pin_lly));
                            painter_path.lineTo(QPointF(pin_llx, pin_ury));
                            //top
                            painter_path.moveTo(QPointF(pin_llx, pin_ury));
                            painter_path.lineTo(QPointF(pin_urx, pin_ury));
                            //right
                            painter_path.moveTo(QPointF(pin_urx, pin_ury));
                            painter_path.lineTo(QPointF(pin_urx, pin_lly));
                            //bottom
                            painter_path.moveTo(QPointF(pin_llx, pin_lly));
                            painter_path.lineTo(QPointF(pin_urx, pin_lly));
                        }
#endif
                    }
                }
            }
        }
    }
}

void LI_Pins::preDraw() {

    refreshBoundSize();

    item_->setVisible(!visible);

    // if (!visible) {

    //     return;
    // }

#if DRAW_MODE == 1
    img->fill(Qt::transparent);
#elif DRAW_MODE == 2
    painter_path = QPainterPath();
#endif

    auto     tc               = open_edi::db::getTopCell();
    uint64_t num_components   = tc->getNumOfInsts();
    auto     components       = tc->getInstances();
    auto     component_vector = open_edi::db::Object::addr<open_edi::db::ArrayObject<open_edi::db::ObjectId>>(components);
    // printf("COMPONENTS %d ;\n", num_components);
    for (auto iter = component_vector->begin(); iter != component_vector->end(); ++iter) {
        auto instance = open_edi::db::Object::addr<open_edi::db::Inst>(*iter);
        __drawPins(*instance);
    }

    item_->setMap(img);
    item_->setItemSize(bound_width, bound_height);
}

} // namespace gui
} // namespace open_edi