#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMap>
#include <QAction>
#include <QGraphicsView>

namespace open_edi {
namespace gui {

#define MAINWINDOW MainWindow::getInstance()

class ActionHandler;
class MDIWindow;
class RibbonMenuBar;
class ActionGroupManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    static MainWindow* getInstance(){
        if(!instance_){
            instance_ = new MainWindow;
        }
        return instance_;
    }

private:
    void init();
    void addActions();
    void createCentralWindow();

private:
    static MainWindow* instance_;
    QGraphicsView* view_;
    QMdiArea* mdi_area_;
    MDIWindow* current_subwindow_;
    QList<MDIWindow*> window_list_;
    RibbonMenuBar* ribbon_;

    ActionHandler* action_handler_;
    QMap<QString, QAction*> action_map_;
    ActionGroupManager* action_manager_;
};


}
}
#endif // MAINWINDOW_H
