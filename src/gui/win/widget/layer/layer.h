#ifndef LAYER_H
#define LAYER_H

#include <QString>

namespace open_edi {
namespace gui {

class Layer
{
public:
    Layer(const QString &name);

private:
    QString name_;
    bool visible;
};

}
}


#endif // LAYER_H
