#ifndef LGI_DIEAREA_H
#define LGI_DIEAREA_H

#include <QGraphicsItem>
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QPen>

#include "lgi_base.h"

namespace open_edi {
namespace gui {

class LGI_DieArea : public LGI_Base {
  public:
    LGI_DieArea(/* args */);
    ~LGI_DieArea();

};
} // namespace gui
} // namespace open_edi

#endif