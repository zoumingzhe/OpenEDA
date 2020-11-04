#ifndef PANELTREEMODEL_H
#define PANELTREEMODEL_H

#include <QStandardItemModel>


namespace open_edi {
namespace gui {

class PanelTreeModel : public QStandardItemModel
{
public:
    PanelTreeModel(QObject *parent = nullptr);
};

}
}
#endif // PANELTREEMODEL_H
