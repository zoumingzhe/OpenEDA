#ifndef QUICKACCESS_BAR_H
#define QUICKACCESS_BAR_H

#include <QToolBar>
#include <QAction>
#include <QHBoxLayout>
#include <QMenu>

namespace open_edi {
namespace gui {

class QuickAccessBar : public QToolBar
{
public:
    QuickAccessBar(QWidget* parent = nullptr);
    ~QuickAccessBar();

    void init();
    void addAction(QAction *action);
    QAction* addAction(const QIcon &icon, const QString &text);

    void setArrowVisible(bool visible);

public slots:
    void quickMenuriggered(QMouseEvent *e);
    void menuTriggered(QMouseEvent *e);

private:
    QMenu  *menu_;
    QAction *separ_action_;
    QAction *arrow_action_;
};



}
}



#endif // QUICKACCESS_BAR_H
