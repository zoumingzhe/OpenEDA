#include <QDebug>
#include <QGraphicsView>


#include "mdi_window.h"
#include "../layout/graphics_view.h"

namespace open_edi {
namespace gui {

MDIWindow::MDIWindow(const QString title,QWidget* parent)
    : QMdiSubWindow (parent)
{
    setWindowTitle(title);

    GraphicsView *view = new GraphicsView(this);
    setWidget(view);
}

MDIWindow::~MDIWindow()
{
}

void MDIWindow::layerActivated(Layer* layer)
{
    qDebug() << "......layer activated.....";
}

void MDIWindow::layerAdded(Layer* layer)
{
    qDebug() << "......layerAdded.....";
}

void MDIWindow::layerRemoved(Layer* layer)
{
    qDebug() << "......layerRemoved.....";
}

void MDIWindow::layerEdit(Layer* layer)
{
    qDebug() << "......layerEdit.....";
}

void MDIWindow::layerVisible(Layer* layer)
{
    qDebug() << "......layerVisible.....";
}


}
}

