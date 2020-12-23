#ifndef ACTION_EDIT_UNDO_H
#define ACTION_EDIT_UNDO_H


#include "action_abstract.h"

namespace open_edi {
namespace gui {


class ActionEditUndo : public ActionAbstract
{
    Q_OBJECT
public:
    ActionEditUndo(QGraphicsView &view, QObject* parent = nullptr);
    virtual ~ActionEditUndo();

};

}
}



#endif // ACTION_EDIT_UNDO_H
