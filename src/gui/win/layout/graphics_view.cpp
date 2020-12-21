#include "graphics_view.h"
#include <QTime>

namespace open_edi {
namespace gui {

GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent) {

    setMinimumSize(700 + VIEW_SPACE, 500 + VIEW_SPACE);

    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setMouseTracking(true);

    li_manager = new LI_Manager(&scale_factor_);

    scene_ = new GraphicsScene;

    for (auto item : li_manager->getLiList()) {
        scene_->addItem(item->getGraphicItem());
    }

    setScene(scene_);

    view_width_  = width() - VIEW_SPACE;
    view_height_ = height() - VIEW_SPACE;

    // setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

GraphicsView::~GraphicsView() {
}

void GraphicsView::readData() {
    slotReadData();
}

void GraphicsView::__zoom(qreal value) {

    qreal s = value;
    view_width_ *= s;
    view_height_ *= s;

    slotReadData();
}

void GraphicsView::slotZoomIn(bool) {
    __zoom(0.8);
}

void GraphicsView::slotZoomOut(bool) {
    __zoom(1.2);
}

void GraphicsView::slotReadData() {

    QTime time;
    time.start();

    auto tc = open_edi::db::getTopCell();

    auto  poly = tc->getFloorplan()->getDieAreaPolygon();
    float factor_x, factor_y;

    auto numPoints = poly->getNumPoints();

    //find die area
    for (int i = 0; i < numPoints; i++) {
        auto point   = poly->getPoint(i);
        auto point_x = point.getX();
        auto point_y = point.getY();

        // printf("pointx %d, pointy %d\n", point_x, point_y);
        if (point_x && point_y) {
            factor_x      = (point_x) / view_width_;
            factor_y      = (point_y) / view_height_;
            scale_factor_ = factor_x > factor_y ?
                              qCeil(factor_x) :
                              qCeil(factor_y);
            die_area_w_ = qCeil((point_x) / scale_factor_);
            die_area_h_ = qCeil((point_y) / scale_factor_);
        }
    }

    scene_->setSceneRect((-die_area_w_) / 2,
                         (-die_area_h_) / 2,
                         die_area_w_,
                         die_area_h_);

    li_manager->preDrawAllItems();

    viewport()->update();

    auto time_elapsed = time.elapsed();
    printf("elapsed time %d (ms)\n", time_elapsed);
}

void GraphicsView::setPinsVisible(bool visible) {
    static bool v = !v;
    li_pins->setVisible(v);
}

void GraphicsView::wheelEvent(QWheelEvent* event) {
    qreal fatory = qPow(1.1, event->delta() / 240.0);
    zoomBy_(fatory);
}

void GraphicsView::mouseMoveEvent(QMouseEvent* event) {
    auto point = mapToScene(event->pos());
    auto x     = (point.rx() + scene_->width() / 2) * scale_factor_;
    auto y     = -(point.ry() - scene_->height() / 2) * scale_factor_;
    emit sendPos(x, y);
}

void GraphicsView::zoomBy_(qreal factor) {
    // scale(factor, factor);
    __zoom(factor);
}

} // namespace gui
} // namespace open_edi