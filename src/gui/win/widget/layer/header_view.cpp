#include "header_view.h"
#include <QPainter>
#include <QCheckBox>

namespace open_edi {
namespace gui {

HeaderView::HeaderView(QWidget* parent)
    : QHeaderView (Qt::Horizontal, parent)
    , selectable_(true)
    , visible_(true)
{
    setSectionsClickable(true);
    connect(this, SIGNAL(sectionClicked(int)), this, SLOT(slotHeaderClicked(int)));
}

HeaderView::~HeaderView()
{
}

void HeaderView::paintSection(QPainter *painter, const QRect &rect, int logicIndex) const
{
    painter->save();
    QHeaderView::paintSection(painter, rect, logicIndex);
    painter->restore();

    switch (logicIndex) {
    case 0:
    case 1:
        QHeaderView::paintSection(painter, rect, logicIndex);
        break;
    case 2:{
        QStyleOptionButton checkBoxStyle;
        checkBoxStyle.state = selectable_ ? QStyle::State_On : QStyle::State_Off;
        checkBoxStyle.state |= QStyle::State_Enabled;
        checkBoxStyle.iconSize = QSize(24, 24);
        QRect rc(rect.left()+(rect.width()-24)/2,rect.top()+ (rect.height()-24)/2,24,24);
        checkBoxStyle.rect = rc;
        QCheckBox checkBox;
        style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &checkBoxStyle, painter, &checkBox);
    }
        break;
    case 3:{
        QStyleOptionButton checkBoxStyle;
        checkBoxStyle.state = visible_ ? QStyle::State_On : QStyle::State_Off;
        checkBoxStyle.state |= QStyle::State_Enabled;
        checkBoxStyle.iconSize = QSize(24, 24);
        QRect rc(rect.left()+(rect.width()-24)/2,rect.top()+(rect.height()-24)/2,24,24);
        checkBoxStyle.rect = rc;
        QCheckBox checkBox;
        style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &checkBoxStyle, painter, &checkBox);
    }
        break;
    default:
        break;

    }
}

void HeaderView::slotHeaderClicked(int index)
{
    if(index == 2)
    {
        selectable_ = !selectable_;
        repaint();
        emit columnCheck(index, selectable_);
    }
    if(index == 3)
    {
        visible_ = !visible_;
        repaint();
        emit columnCheck(index, visible_);
    }
}

}
}
