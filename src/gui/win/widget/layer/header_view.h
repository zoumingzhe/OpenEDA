#ifndef HEADER_VIEW_H
#define HEADER_VIEW_H

#include <QHeaderView>
#include <QMouseEvent>

namespace open_edi {
namespace gui {

class HeaderView : public QHeaderView
{
    Q_OBJECT

public:
    HeaderView(QWidget* parent = nullptr);
    ~HeaderView() override;

protected:
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;
 //   void mousePressEvent(QMouseEvent *event) override;

public slots:
    void slotHeaderClicked(int index);

signals:
    void columnCheck(int logicIndex, bool checked);

private:
    bool selectable_;
    bool visible_;
};

}
}

#endif // HEADER_VIEW_H
