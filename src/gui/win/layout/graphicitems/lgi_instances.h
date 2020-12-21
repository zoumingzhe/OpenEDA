#ifndef LGI_INSTANCES_H
#define LGI_INSTANCES_H

#include <QGraphicsItem>
#include <QPainterPath>
#include <QPen>

#include "lgi_base.h"

namespace open_edi {
namespace gui {

class LGI_Instances : public LGI_Base {
  public:
    LGI_Instances(/* args */);
    ~LGI_Instances();
};
} // namespace gui
} // namespace open_edi

#endif