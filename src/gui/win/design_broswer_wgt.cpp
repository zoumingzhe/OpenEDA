#include "design_broswer_wgt.h"

namespace open_edi {
namespace gui {

DesignBroswerWgt::DesignBroswerWgt(QWidget *parent)
    :QWidget(parent)
{
    setWindowFlags(Qt::Tool);

    design_model_ = new QStandardItemModel;
    design_tree_ = new QTreeView(this);
    design_tree_->setModel(design_model_);

    setMinimumSize(200,300);
}

DesignBroswerWgt::~DesignBroswerWgt()
{
    delete design_model_;
    design_model_ = nullptr;
    delete design_tree_;
    design_tree_ = nullptr;
}


}
}
