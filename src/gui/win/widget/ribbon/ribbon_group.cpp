#include "ribbon_group.h"
#include "ribbon_button.h"

#include <QMenu>
#include <QPaintEvent>
#include <QPainter>

namespace open_edi {
namespace gui {

RibbonGroup::RibbonGroup(QString title, QWidget *parent)
    : QWidget(parent)
{
    setObjectName("RibbonGroupWidget");   
    init(); 
    setTitle(title);
}

RibbonGroup::~RibbonGroup()
{
}

void RibbonGroup::init()
{
    group_layout_ = new QHBoxLayout();
    group_layout_->setSpacing(0);
    group_layout_->setContentsMargins(0, 0, 0, 0);

    large_bar_ = new QToolBar();
    large_bar_->setOrientation(Qt::Horizontal);
    large_bar_->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    small_bar_ = new QToolBar();
    small_bar_->setOrientation(Qt::Vertical);
    small_bar_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    grid_layout_ = new QGridLayout();
    grid_layout_->setSpacing(0);
    grid_layout_->setContentsMargins(0, 0, 0, 0);
    group_layout_->addLayout(grid_layout_);

    QHBoxLayout* titleLayout = new QHBoxLayout;
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(0, 0, 0, 0);

    group_title_ = new QLabel();
    group_title_->setStyleSheet("font: 10pt 微软雅黑");
    group_title_->setFixedHeight(18);
    titleLayout->addWidget(group_title_);

    pop_button_ = new RibbonButton();
    pop_button_->setText(">");
    pop_button_->setFixedSize(QSize(15, 15));
    pop_button_->setVisible(false);
//   connect(pop_button_, SIGNAL(triggered(QAction*)), this, SIGNAL(popButtonClicked()));
    titleLayout->addWidget(pop_button_);

    QVBoxLayout* bodyLayout = new QVBoxLayout;
    bodyLayout->setSpacing(0);
    bodyLayout->setContentsMargins(0, 0, 0, 0);
    bodyLayout->addLayout(group_layout_);
    bodyLayout->addStretch();
    bodyLayout->addLayout(titleLayout);

    QWidget *line = new QWidget;
    line->setFixedWidth(1);
    line->setAutoFillBackground(true);
    line->setBackgroundRole(QPalette::Mid);

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addLayout(bodyLayout);
    mainLayout->addWidget(line);
    setLayout(mainLayout);
}

QMenu* RibbonGroup::addMenu(const QIcon &icon, const QString &text, Qt::ToolButtonStyle style)
{
    QMenu *menu = new QMenu(text, this);
    QAction *action = menu->menuAction();
    action->setIcon(icon);
    addAction(action, style);

    return menu;
}

void RibbonGroup::addAction(QAction* action, int row, int col, int rowSpan , int columnSpan, Qt::Alignment alignment)
{
    RibbonButton *button = new RibbonButton();
    button->setIcon(action->icon());
    button->setText(action->text());
    button->setToolTip(action->toolTip());
//    button->setMenu(action->menu());
    button->setPopupMode(QToolButton::InstantPopup);
    button->setDefaultAction(action);
    connect(button, SIGNAL(triggered(QAction *)), action, SIGNAL(triggered()));
    grid_layout_->addWidget(button, row, col, rowSpan, columnSpan, alignment);
}

void RibbonGroup::addAction(QAction *action, Qt::ToolButtonStyle style)
{
    if(style == Qt::ToolButtonTextUnderIcon)
    {
        large_bar_->addAction(action);
        group_layout_->insertWidget(0, large_bar_);   //two LayoutItems in group_layout_ at most
    }
    else if (style == Qt::ToolButtonTextBesideIcon)
    {
        small_bar_->addAction(action);
        if(group_layout_->count() == 1){
            group_layout_->insertWidget(0, small_bar_);
        }else {
            group_layout_->insertWidget(1, small_bar_);
        }
    }
}

void RibbonGroup::addAction(const QIcon &icon, const QString &text, Qt::ToolButtonStyle style)
{
    QAction *action = new QAction(icon, text);
    addAction(action, style);
}

void RibbonGroup::setTitle(const QString title){
    group_title_->setText(title);
    group_title_->setAlignment(Qt::AlignCenter);
}


void RibbonGroup::addWidget(QWidget *widget, int row, int col, int rowSpan, int columnSpan,
                            Qt::Alignment alignment)
{
    grid_layout_->addWidget(widget, row, col, rowSpan, columnSpan, alignment);
}

void RibbonGroup::removeWidget(QWidget *widget){
    grid_layout_->removeWidget(widget);
}

void RibbonGroup::setExpandButtonVisible(bool visible){
    pop_button_->setVisible(visible);
}

bool RibbonGroup::isExpandButtonVisible() const
{
    return  pop_button_->isVisible();
}

void RibbonGroup::clear()
{
}

}
}
