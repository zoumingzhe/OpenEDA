
#include "graphics_scene.h"

namespace open_edi {
namespace gui {

GraphicsScene::GraphicsScene(QObject* parent) {
    
}

GraphicsScene::~GraphicsScene() {
}

void GraphicsScene::drawBackground(QPainter* painter, const QRectF& rect) {
    painter->fillRect(rect,QBrush(QColor("#393939")));
}


} // namespace gui
} // namespace open_edi
