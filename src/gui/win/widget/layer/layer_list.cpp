#include "layer_list.h"
#include "layer_listener.h"

namespace open_edi {
namespace gui {

LayerList::LayerList()
{
    active_layer_ = nullptr;

}

LayerList::~LayerList()
{

}

QList<Layer*>::Iterator LayerList::begin(){
    return layers_.begin();
}

QList<Layer*>::Iterator LayerList::end(){
    return layers_.end();
}


}
}
