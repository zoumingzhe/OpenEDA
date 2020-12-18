#include "ribbon_title_bar.h"
#include "quick_access_bar.h"
#include <QHBoxLayout>

namespace open_edi {
namespace gui {

RibbonTitleBar::RibbonTitleBar(QWidget *parent)
    : QWidget(parent)
    , title_(new QLabel)
    , left_accessbar_(new QuickAccessBar)
    , right_systembar_(new QuickAccessBar)
{
    setObjectName("RibbonTitleBar");
    init();
}

RibbonTitleBar::~RibbonTitleBar()
{
}

void RibbonTitleBar::init()
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setMargin(1);
    layout->setSpacing(0);
    setLayout(layout);

    layout->addWidget(left_accessbar_);
    layout->addStretch();
    layout->addWidget(title_);
    layout->addStretch();
    layout->addWidget(right_systembar_);
    right_systembar_->setArrowVisible(false);
}

void RibbonTitleBar::setTitle(const QString &title)
{
    title_->setText(title);
}

void RibbonTitleBar::addQuickAction(QAction *action)
{
    left_accessbar_->addAction(action);
}

QAction* RibbonTitleBar::addQuickAction(const QIcon &icon, const QString &text)
{
    return left_accessbar_->addAction(icon, text);
}

void RibbonTitleBar::addSystemAction(QAction* action)
{
    right_systembar_->addAction(action);
}

QAction* RibbonTitleBar::addSystemAction(const QIcon &icon, const QString &text)
{
    return right_systembar_->addAction(icon, text);
}

}
}
