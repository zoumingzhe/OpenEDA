#ifndef LGI_BASE_H
#define LGI_BASE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainterPath>
#include <QPen>

#include "../items/li_base.h"
#define VIEW_SPACE 2

namespace open_edi {
namespace gui {

class LI_Base;

class LGI_Base : public QGraphicsItem {
  public:
    LGI_Base(/* args */);
    ~LGI_Base();

    void setItemSize(int w, int h);
    void setMap(QPixmap* img);
    void setLiBase(LI_Base* li_base);

  protected:
    int      w{1};
    int      h{1};
    QPixmap* img{nullptr};

    virtual QRectF boundingRect() const;

    virtual void paint(QPainter*                       painter,
                       const QStyleOptionGraphicsItem* option,
                       QWidget*                        widget = nullptr) override;

    LI_Base* li_base;
};
} // namespace gui
} // namespace open_edi

#endif