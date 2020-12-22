#ifndef LI_BASE_H
#define LI_BASE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPixmap>

#define NO_DRAW   0
#define IMG_MODE  1
#define PATH_MODE 2
#define DRAW_MODE IMG_MODE

namespace open_edi {
namespace gui {

class LI_Base {
  public:
    LI_Base(const LI_Base& other) = delete;
    LI_Base& operator=(const LI_Base& rhs) = delete;

    explicit LI_Base(int* scale_factor) { this->scale_factor = scale_factor; };
    ~LI_Base(){};

    virtual void           preDraw() = 0;
    virtual void           refreshBoundSize();
    virtual void           setVisible(bool visible);
    virtual QGraphicsItem* getGraphicItem() = 0;
    virtual void           draw(QPainter* painter);

    enum ObjType {
        kDieArea,
        kInstance,
        kPin
    };
    ObjType type;

  protected:
    int          bound_width{1};
    int          bound_height{1};
    QPixmap*     img{nullptr};
    bool         visible{false};
    int*         scale_factor{nullptr};
    QPainterPath painter_path;
};

} // namespace gui
} // namespace open_edi

#endif