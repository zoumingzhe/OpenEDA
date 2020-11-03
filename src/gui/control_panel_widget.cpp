#include "control_panel_widget.h"


namespace open_edi{
namespace gui {


ControlPanelWidget::ControlPanelWidget(const QString &title, QWidget *parent, Qt::WindowFlags flags)
    : QDockWidget(parent, flags)
{
    setObjectName(title);
    setWindowTitle(title);

    model_ = new PanelTreeModel;
    view_ = new PanelTreeView;
    view_->setModel(model_);
    setWidget(view_);
}

ControlPanelWidget::~ControlPanelWidget()
{
    delete model_;
    model_ = nullptr;
    delete view_;
    view_ = nullptr;
}


}
}
