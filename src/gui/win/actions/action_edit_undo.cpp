#include "action_edit_undo.h"

namespace open_edi {
namespace gui {

ActionEditUndo::ActionEditUndo(QGraphicsView &view, QObject* parent)
    : ActionAbstract ("Undo", view, parent)
{
    qDebug() << "edit undo .........";
}

ActionEditUndo::~ActionEditUndo()
{

}

}
}

