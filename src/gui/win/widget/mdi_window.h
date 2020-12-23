#ifndef MDI_WINDOW_H
#define MDI_WINDOW_H

#include <QMdiSubWindow>
#include "layer/layer_listener.h"

namespace open_edi {
namespace gui {

class MDIWindow : public QMdiSubWindow, public LayerListener
{
    Q_OBJECT

public:
    MDIWindow(const QString title, QWidget* parent = nullptr);
    ~MDIWindow();

    void layerActivated(Layer* layer);
    void layerAdded(Layer* layer);
    void layerRemoved(Layer* layer);
    void layerEdit(Layer* layer);
    void layerVisible(Layer* layer);

private:


};

}
}

#endif // MDI_WINDOW_H
