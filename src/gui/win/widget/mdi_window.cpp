#include "mdi_window.h"
#include <QDebug>
#include <QGraphicsView>

namespace open_edi {
namespace gui {

MDIWindow::MDIWindow(const QString title,QWidget* parent)
    : QMdiSubWindow (parent)
{
    setWindowTitle(title);

    QGraphicsView *view = new QGraphicsView(this);
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

