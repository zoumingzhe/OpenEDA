#include "action_import_design.h"

namespace open_edi {
namespace gui {

ActionFileImportDesign::ActionFileImportDesign(QGraphicsView &view, QObject *parent)
    : ActionAbstract("Import Design", view, parent)
{
    qDebug() << "........import design .........";
}

ActionFileImportDesign::~ActionFileImportDesign()
{

}

}
}
