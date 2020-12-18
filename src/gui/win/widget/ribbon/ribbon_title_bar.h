#ifndef RIBBON_TITLE_BAR_H
#define RIBBON_TITLE_BAR_H

#include <QWidget>
#include <QLabel>

namespace open_edi {
namespace gui {

class QuickAccessBar;
class RibbonTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit RibbonTitleBar(QWidget *parent = nullptr);
    ~RibbonTitleBar();
    void init();

    void setTitle(const QString &title);
    void addQuickAction(QAction* action);
    QAction* addQuickAction(const QIcon &icon, const QString &text);
    void addSystemAction(QAction* action);
    QAction* addSystemAction(const QIcon &icon, const QString &text);

signals:

public slots:


private:
    QLabel *title_;
    QuickAccessBar *left_accessbar_;
    QuickAccessBar *right_systembar_;
};

}
}
#endif // RIBBON_TITLE_BAR_H
