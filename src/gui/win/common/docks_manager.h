#ifndef DOCKS_PRODUCER_H
#define DOCKS_PRODUCER_H

#include <QObject>

namespace open_edi {
namespace gui {

class MainWindow;
class ActionHandler;
class DocksManager : public QObject
{
    Q_OBJECT
public:
    explicit DocksManager(QObject *parent = nullptr);

    void createDockWidgets();

signals:

public slots:

private:
    MainWindow *main_window_;
};

}
}

#endif // DOCKS_PRODUCER_H
