#ifndef ACTIONPRODUCER_H
#define ACTIONPRODUCER_H

#include <QObject>
#include <QMap>

class QAction;
class QActionGroup;

namespace open_edi {
namespace gui {

class ActionGroupManager;

class ActionProducer : public QObject
{
    Q_OBJECT
public:
    explicit ActionProducer(QObject *parent = nullptr, QObject *hander = nullptr);
    void fillActionContainer(QMap<QString, QAction*> &map, ActionGroupManager *manager);

private:
    QObject *action_hander_;
    QObject *main_window_;
};


}
}
#endif // ACTIONPRODUCER_H
