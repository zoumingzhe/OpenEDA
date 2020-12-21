#ifndef LGI_PINS_H
#define LGI_PINS_H

#include <QGraphicsItem>
#include <QPainterPath>
#include <QPen>

#include "lgi_base.h"

namespace open_edi {
namespace gui {

class LGI_Pins : public LGI_Base {
  public:
    LGI_Pins(/* args */);
    ~LGI_Pins();
};
} // namespace gui
} // namespace open_edi

#endif