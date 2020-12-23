#include "action_abstract.h"

namespace open_edi {
namespace gui {

ActionAbstract::ActionAbstract(QString name, QGraphicsView &view, QObject *parent)
      : QObject(parent)
      , view_(&view)
      , action_name_(name)
{

}


}
}
