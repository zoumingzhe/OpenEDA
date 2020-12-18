#ifndef ACTION_VIEW_ZOOMIN_H
#define ACTION_VIEW_ZOOMIN_H

#include "action_abstract.h"

namespace open_edi {
namespace gui {

class ActionViewZoomin : public ActionAbstract
{
    Q_OBJECT
public:
    ActionViewZoomin(QGraphicsView &view, QObject* parent = nullptr);
    virtual ~ActionViewZoomin();
};

}
}



#endif // ACTION_VIEW_ZOOMIN_H
