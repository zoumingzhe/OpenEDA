#ifndef GRAPHICSSCENCE_H
#define GRAPHICSSCENCE_H

#include <QGraphicsScene>

namespace open_edi {
namespace gui {

class GraphicScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicScene(QObject *parent = nullptr);
    ~GraphicScene();
};


}
}


#endif // GRAPHICSSCENCE_H
