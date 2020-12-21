#ifndef LAYER_LIST_H
#define LAYER_LIST_H

#include "layer.h"
#include <QList>

namespace open_edi {
namespace gui {

class LayerListener;

class LayerList
{
public:
    LayerList();
    virtual ~LayerList();

    int count() const{
        return layers_.count();
    }

    Layer* at(int i){
        return layers_.at(i);
    }

    QList<Layer*>::Iterator begin();
    QList<Layer*>::Iterator end();

private:
    QList<Layer*> layers_;
    QList<LayerListener*> listeners_;
    Layer* active_layer_;
};

}
}

#endif // LAYER_LIST_H
