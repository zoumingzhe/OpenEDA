#include "ribbon_page.h"
#include "ribbon_group.h"

namespace open_edi {
namespace gui {

RibbonPage::RibbonPage(QWidget *parent)
    : QWidget(parent)
    , page_title_("Ribbon Page")
{
    setObjectName("RibbonPage");
    init();
}

RibbonPage::~RibbonPage()
{
}

void RibbonPage::init()
{
    main_layout_ = new QHBoxLayout;
    main_layout_->setContentsMargins(0,0,0,0);
    main_layout_->setSpacing(0);
    main_layout_->addStretch();
    setLayout(main_layout_);
}

RibbonGroup* RibbonPage::addGroup(QString title){

    RibbonGroup *group = new RibbonGroup(title);

    main_layout_->insertWidget(main_layout_->count()-1, group);
    group_list_.append(group);
    return group;
}

void RibbonPage::addGroup(RibbonGroup *group){
    main_layout_->insertWidget(main_layout_->count()-1, group);
    group_list_.append(group);
}

void RibbonPage::removeGroup(RibbonGroup *group){
    int index = main_layout_->indexOf(group);
    main_layout_->removeWidget(group);
    group_list_.removeAt(index);
}

void RibbonPage::clearGroups(){
    for(auto group : group_list_){
        removeGroup(group);
    }
}

void RibbonPage::setTitle(const QString &title){
    page_title_ = title;
}

QString RibbonPage::title() const{
    return page_title_;
}


}
}
