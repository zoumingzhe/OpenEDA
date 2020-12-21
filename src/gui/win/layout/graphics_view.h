#ifndef LAYOUT_GRAPHICS_VIEW_H
#define LAYOUT_GRAPHICS_VIEW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainterPath>
#include <QWheelEvent>
#include <qmath.h>
#include "db/core/cell.h"
#include "db/core/db.h"
#include "db/io/write_def.h"
#include "db/util/array.h"
#include "db/util/property_definition.h"
#include "db/util/vector_object_var.h"
#include "graphics_scene.h"
#include "util/util.h"
#include "items/li_manager.h"

namespace open_edi {
namespace gui {

class GraphicsView : public QGraphicsView {
    Q_OBJECT
  public:
    GraphicsView(QWidget* parent = nullptr);
    ~GraphicsView();

    LI_DieArea*   li_die_area;
    LI_Instances* li_instances;
    LI_Pins*      li_pins;
    LI_Manager*   li_manager;
    void          readData();

  signals:
    void sendPos(int x, int y);

  public slots:
    void slotZoomIn(bool);
    void slotZoomOut(bool);
    void slotReadData();
    void setPinsVisible(bool);

  protected:
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;

  private:
    void           zoomBy_(qreal factor);
    int            die_area_x_;
    int            die_area_y_;
    int            view_width_;
    int            view_height_;
    int            scale_factor_{1};
    GraphicsScene* scene_;

    void __zoom(qreal value);
};
} // namespace gui
} // namespace open_edi
#endif