#ifndef CONTROLPANELWIDGET_H
#define CONTROLPANELWIDGET_H

#include <QDockWidget>
#include <QTreeView>
#include "panel_tree_view.h"
#include "panel_tree_model.h"

class PanelTreeModel;
class PanelTreeView;

namespace open_edi{
namespace gui{

class ControlPanelWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit ControlPanelWidget(const QString &title, QWidget *parent = nullptr, Qt::WindowFlags flags = 0);
    ~ControlPanelWidget();
signals:

public slots:

private:
    PanelTreeModel  *model_;
    PanelTreeView   *view_;
};


}
}



#endif // CONTROLPANELWIDGET_H
