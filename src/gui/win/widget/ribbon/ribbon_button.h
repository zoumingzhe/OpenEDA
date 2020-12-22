#ifndef RIBBON_BUTTON_H
#define RIBBON_BUTTON_H

#include <QToolButton>

namespace open_edi {
namespace gui {

class RibbonButton : public QToolButton
{
    Q_OBJECT
public:
    explicit RibbonButton(QWidget *parent = nullptr);
    virtual ~RibbonButton();

signals:
    void menuActionClicked(QMouseEvent *e);

protected:
    virtual void mousePressEvent(QMouseEvent *e);
};


}
}
#endif // RIBBON_BUTTON_H
