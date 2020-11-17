#ifndef GRAPHICVIEW_H
#define GRAPHICVIEW_H
#include <QGraphicsView>

namespace open_edi {
namespace gui {

class GraphicView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicView(QObject *parent = nullptr);
    ~GraphicView();
};

}
}
#endif // GRAPHICVIEW_H
