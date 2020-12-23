#ifndef LAYER_WIDGET_H
#define LAYER_WIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QTableView>
#include <QMap>
#include "layer_listener.h"
#include <QAbstractTableModel>

namespace open_edi {
namespace gui {

class LayerList;

class LayerModel : public QAbstractTableModel{
public:
    enum{
        NAME,
        COLORPATTERN,
        VISIBLE,
        SELECTABLE
    };

    enum IconSize{
        WIDTH = 32,
        HEIGH = 24
    };

    LayerModel(QObject* parent = nullptr);
    ~LayerModel();

    Qt::ItemFlags flags(const QModelIndex &index) const;

    int columnCount(const QModelIndex &) const {
        return SELECTABLE + 1;
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
                                   int role = Qt::EditRole);

    void setLayerList(LayerList* list);
    Layer* getLayer(int row) const;

private:
    QList<Layer*> layers_;
    QMap<int, Qt::CheckState> select_check_map_;
    QMap<int, Qt::CheckState> visible_check_map_;
};


class LayerWidget : public QWidget, public LayerListener
{
    Q_OBJECT
public:
    explicit LayerWidget(QWidget* parent = nullptr);
    ~LayerWidget();

    void setLayerList(LayerList* layerList);
    void update();
    void activateLayer(Layer* layer);

    virtual void layerActivated(Layer* layer);
    virtual void layerAdded(Layer*);
    virtual void layerRemoved(Layer*);
    virtual void layerEdit(Layer*);
    virtual void layerVisible(Layer*);

signals:

public slots:
    void slotActivated(QModelIndex Index);
    void slotItemClicked(QListWidgetItem* item);

private:
    void createItems();

private:
    LayerList* layer_list_;
    QListWidget* list_widget_;
    QStackedWidget* stacked_widget_;
    QMap<QListWidgetItem*, QWidget*> item_map_;

    QTableView* layer_view_;
    LayerModel* layer_model_;
    QTableView* cell_view_;
    QTableView* net_view_;
};

}
}


#endif // LAYER_WIDGET_H
