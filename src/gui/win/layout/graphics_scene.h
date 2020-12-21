#ifndef LAYOUT_GRAPHICS_SCENE_H
#define LAYOUT_GRAPHICS_SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPoint>
#include <QPainter>

namespace open_edi {
namespace gui {

class GraphicsScene : public QGraphicsScene {
    Q_OBJECT
  public:
    explicit GraphicsScene(QObject* parent = nullptr);
    ~GraphicsScene();

  protected:
    virtual void drawBackground(QPainter* painter, const QRectF& rect) override;

    // signals:
    //   void sendPos(QPointF point);
};

} // namespace gui
} // namespace open_edi

#endif // GRAPHICSSCENCE_H
