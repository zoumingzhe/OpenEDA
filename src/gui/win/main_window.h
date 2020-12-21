#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QGraphicsView>
#include <QMainWindow>
#include <QMap>
#include <QMdiArea>

#include "./layout/graphics_view.h"

namespace open_edi {
namespace gui {

#define MAINWINDOW MainWindow::getInstance()

class ActionHandler;
class MDIWindow;
class RibbonMenuBar;
class ActionGroupManager;

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

  public:
    static MainWindow* getInstance() {
        if (!instance_) {
            instance_ = new MainWindow;
        }
        return instance_;
    }

  private:
    void init();
    void addActions();
    void createCentralWindow();

signals:
    void windowChanged(bool);

  private:
    static MainWindow* instance_;
    QGraphicsView*     view_;
    QMdiArea*          mdi_area_;
    MDIWindow*         current_subwindow_;
    QList<MDIWindow*>  window_list_;
    RibbonMenuBar*     ribbon_;

    ActionHandler*          action_handler_;
    QMap<QString, QAction*> action_map_;
    ActionGroupManager*     action_manager_;
    GraphicsView*           graphics_view_;
    QAction*                import_design_;
};

} // namespace gui
} // namespace open_edi
#endif // MAINWINDOW_H
