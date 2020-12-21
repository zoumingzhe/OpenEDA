#ifndef ACTION_ABSTRACT_H
#define ACTION_ABSTRACT_H

#include <QObject>
#include <QGraphicsView>
#include <QDebug>

namespace open_edi {
namespace gui {

class ActionAbstract : public QObject
{
    Q_OBJECT
public:
    explicit ActionAbstract(QString name, QGraphicsView &view, QObject *parent = nullptr);

signals:

public slots:

private:
    QGraphicsView *view_;
    QString action_name_;
};

}
}

#endif // ACTION_ABSTRACT_H
