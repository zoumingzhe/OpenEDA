#include "layer_widget.h"
#include "layer_list.h"

#include <QDebug>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QCheckBox>
#include "header_view.h"

namespace open_edi {
namespace gui {

LayerModel::LayerModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

LayerModel::~LayerModel(){

}

Qt::ItemFlags LayerModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    if(index.column() == 2 || index.column() == 3)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

int LayerModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent)

    return layers_.size();
}

bool LayerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    if(role == Qt::CheckStateRole && index.column() == 2){
        select_check_map_[index.row()] = (value == Qt::Checked? Qt::Checked : Qt::Unchecked);
    }else if (role == Qt::CheckStateRole && index.column() == 3) {
        visible_check_map_[index.row()] = (value == Qt::Checked? Qt::Checked : Qt::Unchecked);
    }
    return true;
}

QVariant LayerModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() >= layers_.size())
        return QVariant();

    if(role == Qt::CheckStateRole && index.column() == 2){
        if(select_check_map_[index.row()]){
            return select_check_map_[index.row()] == Qt::Checked? Qt::Checked : Qt::Unchecked;
        }
        return Qt::Unchecked;
    }else if (role == Qt::CheckStateRole && index.column() == 3) {
        if(visible_check_map_[index.row()]){
            return visible_check_map_[index.row()] == Qt::Checked? Qt::Checked : Qt::Unchecked;
        }
        return Qt::Unchecked;
    }
}


QModelIndex LayerModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if(row >= layers_.size() || row < 0)
        return QModelIndex();

    return createIndex(row, column);
}

QModelIndex LayerModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)

    return QModelIndex();
}

bool LayerModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    QAbstractTableModel::setHeaderData(section, orientation, value, role);
    return true;
}

void LayerModel::setLayerList(LayerList* list)
{
    beginResetModel();
    layers_.clear();
    if (list == nullptr) {
        endResetModel();
        return;
    }
    for(int i=0; i < list->count(); ++i) {
        layers_.append(list->at(i));
    }

    endResetModel();
}

Layer* LayerModel::getLayer(int row) const
{
    if(row >= layers_.size() || row < 0)
        return nullptr;

    return layers_.at(row);
}

LayerWidget::LayerWidget(QWidget *parent)
    : QWidget(parent)
    , layer_list_(nullptr)
    , layer_view_(nullptr)
    , layer_model_(nullptr)
{
    setObjectName("LayerWidget");
    stacked_widget_ = new QStackedWidget(this);

    list_widget_ = new QListWidget(this);
    list_widget_->setMaximumWidth(150);
    connect(list_widget_, &QListWidget::itemClicked, this, &LayerWidget::slotItemClicked);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(2,2,2,2);
    layout->setSpacing(0);
    layout->addWidget(list_widget_);
    layout->addWidget(stacked_widget_);
    setLayout(layout);

    createItems();
}

LayerWidget::~LayerWidget()
{
}

void LayerWidget::slotItemClicked(QListWidgetItem* item)
{
    QWidget* widget = item_map_.find(item).value();
    if(widget){
        stacked_widget_->setCurrentWidget(widget);
    }
}

void LayerWidget::createItems()
{
    layer_model_ = new LayerModel(this);

    layer_view_ = new QTableView(this);
    layer_view_->setModel(layer_model_);
    layer_view_->setShowGrid(true);
    layer_view_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layer_view_->setFocusPolicy(Qt::NoFocus);
    layer_view_->setMinimumHeight(200);
    HeaderView* header = new HeaderView(layer_view_);
    layer_view_->setHorizontalHeader(header);

    layer_view_->setColumnWidth(LayerModel::COLORPATTERN, LayerModel::WIDTH);
    layer_view_->setColumnWidth(LayerModel::VISIBLE, LayerModel::WIDTH);
    layer_view_->setColumnWidth(LayerModel::SELECTABLE, LayerModel::WIDTH);
    connect(layer_view_, &QTableView::clicked, this, &LayerWidget::slotActivated);

    stacked_widget_->addWidget(layer_view_);
    stacked_widget_->setCurrentWidget(layer_view_);

    QListWidgetItem* item = new QListWidgetItem("Layer");
    list_widget_->addItem(item);

    item_map_.insert(item, layer_view_);
}


void LayerWidget::setLayerList(LayerList* list)
{
    layer_list_ = list;
    update();
}

void LayerWidget::activateLayer(Layer* layer)
{
    qDebug() << "......LayerWidget::activatedlayer......";
}

void LayerWidget::update()
{
    qDebug() << "......LayerWidget::update......";
}

void LayerWidget::layerActivated(Layer* layer)
{
    qDebug() << "......LayerWidget::layer activated.....";
}

void LayerWidget::layerAdded(Layer* layer)
{
    qDebug() << "......LayerWidget::layerAdded.....";
}

void LayerWidget::layerRemoved(Layer* layer)
{
    qDebug() << "......LayerWidget::layerRemoved.....";
}

void LayerWidget::layerEdit(Layer* layer)
{
    qDebug() << "......LayerWidget::layerEdit.....";
}

void LayerWidget::layerVisible(Layer* layer)
{
    qDebug() << "......LayerWidget::layerVisible.....";
}

void LayerWidget::slotActivated(QModelIndex index)
{
    qDebug() << "......LayerWidget::slotActivated.....";
}


}
}
