#ifndef DESIGNBROSWERWGT_H
#define DESIGNBROSWERWGT_H

#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>

namespace open_edi {
namespace gui {

class DesignBroswerWgt : public QWidget
{
    Q_OBJECT
public:
    explicit DesignBroswerWgt(QWidget *parent = nullptr);
    ~DesignBroswerWgt();

private:
    QTreeView  *design_tree_;
    QStandardItemModel  *design_model_;
};

}
}



#endif // DESIGNBROSWERWGT_H
