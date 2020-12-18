#ifndef LGI_BASE_H
#define LGI_BASE_H

#include <QGraphicsItem>
#include <QPainterPath>
#include <QPen>
#include <QGraphicsScene>


#define VIEW_SPACE 2

namespace open_edi {
namespace gui {

class LGI_Base : public QGraphicsItem {
  public:
    LGI_Base(/* args */);
    ~LGI_Base();

    void setItemSize(int w, int h);
    void setMap(QPixmap* img);

  protected:

    int      w{1};
    int      h{1};
    QPixmap* img{nullptr};

    virtual QRectF boundingRect() const;

    virtual void paint(QPainter*                       painter,
                       const QStyleOptionGraphicsItem* option,
                       QWidget*                        widget = nullptr) override;



};
} // namespace gui
} // namespace open_edi

#endif