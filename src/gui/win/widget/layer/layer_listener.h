#ifndef LAYER_LISTENER_H
#define LAYER_LISTENER_H

#include "layer.h"

namespace open_edi {
namespace gui {

class LayerListener
{
public:
    LayerListener();
    virtual ~LayerListener(){}

    virtual void layerActivated(Layer*);

    virtual void layerAdded(Layer*){}

    virtual void layerRemoved(Layer*){}

    virtual void layerEdit(Layer*){}

    virtual void layerVisible(Layer*){}

};

}
}

#endif // LAYER_LISTENER_H
