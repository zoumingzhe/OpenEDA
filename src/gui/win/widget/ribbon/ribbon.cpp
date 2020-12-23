#include "ribbon.h"
#include "ribbon_page.h"
#include "ribbon_title_bar.h"
#include <QLineEdit>


namespace open_edi {
namespace gui {

RibbonMenuBar::RibbonMenuBar(QWidget *parent)
    : QMenuBar (parent)
    , show_ribbon_(true)
{
    setObjectName("RibbonMenuBar");
    init();
}

RibbonMenuBar::~RibbonMenuBar()
{

}

void RibbonMenuBar::init()
{
    tab_bar_ = new QTabBar;
    tab_bar_->setFixedHeight(38);
    tab_bar_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    tab_bar_->setObjectName("RibbonMenuTabBar");

    page_bar_ = new QStackedWidget;
    page_bar_->setFixedHeight(100);
    page_bar_->setObjectName("RibbonMenuStackedWidget");

    ribbon_titlebar_ = new RibbonTitleBar;
    ribbon_titlebar_->setFixedHeight(28);

    QHBoxLayout *tabLayout = new QHBoxLayout;
    tabLayout->addWidget(tab_bar_);
/*    tabLayout->addSpacerItem(new QSpacerItem(100,1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    QLabel *searchLabel = new QLabel(tr("Search Instance:"));
    QLineEdit *lineEdit = new QLineEdit;
    tabLayout->addWidget(searchLabel);
    tabLayout->addWidget(lineEdit);
    */
    tabLayout->addStretch();

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->addWidget(ribbon_titlebar_);
    vLayout->addLayout(tabLayout);
    vLayout->addWidget(page_bar_);
    setLayout(vLayout);

    connect(tab_bar_, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentChanged(int)));
}

RibbonPage* RibbonMenuBar::addPage(const QString &text)
{
    if(page_map_.contains(text))
        return page_map_[text];

    RibbonPage* page = new RibbonPage(this);
    page->setTitle(text);

    int index = tab_bar_->addTab(text);
    page_bar_->insertWidget(index, page);
    page_map_.insert(text, page);

    return page;
}

void RibbonMenuBar::addPage(RibbonPage *page)
{
    if(page_map_.contains(page->title()))
        return;

    int index = tab_bar_->addTab(page->title());
    page_bar_->insertWidget(index, page);
    page_map_.insert(page->title(), page);
}

RibbonPage* RibbonMenuBar::insertPage(int index, const QString &text)
{
    if(tab_bar_->tabText(index) == text)
        return page_map_[text];

    RibbonPage* page = new RibbonPage(this);
    page->setTitle(text);

    tab_bar_->insertTab(index, text);
    page_bar_->insertWidget(index, page);
    page_map_.insert(text, page);

    return page;
}

void RibbonMenuBar::removePage(RibbonPage *page)
{
    if(!page_map_.contains(page->title()))
        return;

    int index = page_bar_->indexOf(page);
    page_bar_->removeWidget(page);
    tab_bar_->removeTab(index);
    page_map_.remove(page->title());
}

int RibbonMenuBar::pageCount() const{
    return page_map_.size();
}

void RibbonMenuBar::slotCurrentChanged(int index){
    page_bar_->setCurrentIndex(index);
    page_bar_->show();
}

RibbonTitleBar* RibbonMenuBar::getTitleBar() const{
    return ribbon_titlebar_;
}



}
}
