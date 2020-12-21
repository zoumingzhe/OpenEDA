#ifndef ACTIONFILEIMPORTDESIGN_H
#define ACTIONFILEIMPORTDESIGN_H

#include "action_abstract.h"

namespace open_edi {
namespace gui {

class ActionFileImportDesign : public ActionAbstract
{
    Q_OBJECT
public:
    ActionFileImportDesign(QGraphicsView &view, QObject* parent = nullptr);
    virtual ~ActionFileImportDesign();

signals:

public slots:


};


}
}



#endif // ACTIONFILEIMPORTDESIGN_H
