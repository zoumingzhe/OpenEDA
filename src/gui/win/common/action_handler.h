#ifndef ACTIONHANDLER_H
#define ACTIONHANDLER_H

#include <QObject>
#include "../actions/action_abstract.h"
#include "define.h"

namespace open_edi {
namespace gui {

class ActionHandler : public QObject
{
    Q_OBJECT

public:
    explicit ActionHandler(QObject *parent = nullptr);
    virtual ~ActionHandler();


    ActionAbstract* getCurrentAction();
    ActionAbstract* setCurrentAction(EDAGui::ActionType type);

    void setView(QGraphicsView *view);

signals:
    void sendReadData();
public slots:
    void slotImportDesign();
    void slotSaveDesign();
    void slotSetPreference();
    void slotFindSelectObject();
    void slotEditUndo();
    void slotEditRedo();
    void slotEditHighLight();
    void slotViewZoomIn();
    void slotViewZoomOut();

private:
    QGraphicsView* view_;
};


}
}
#endif // ACTIONHANDLER_H
