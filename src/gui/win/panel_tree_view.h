#ifndef PANELTREEVIEW_H
#define PANELTREEVIEW_H

#include <QTreeView>


namespace open_edi {
namespace gui {

class PanelTreeView : public QTreeView
{
public:
    PanelTreeView(QObject *parent = nullptr);
};


}
}
#endif // PANELTREEVIEW_H
