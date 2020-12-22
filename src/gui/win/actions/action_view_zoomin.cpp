#include "action_view_zoomin.h"

namespace open_edi {
namespace gui {

ActionViewZoomin::ActionViewZoomin(QGraphicsView &view, QObject* parent)
    :ActionAbstract ("Zoom In", view, parent)
{
   qDebug() << "zoom in .........";
}

ActionViewZoomin::~ActionViewZoomin()
{

}

}
}

