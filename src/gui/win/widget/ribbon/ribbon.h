#ifndef RIBBON_H
#define RIBBON_H

#include <QMenuBar>
#include <QStackedWidget>
#include <QTabBar>
#include <QPropertyAnimation>
#include <QMap>

namespace open_edi {
namespace gui {

class RibbonTitleBar;
class RibbonPage;

class RibbonMenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit RibbonMenuBar(QWidget *parent = nullptr);
    ~RibbonMenuBar();

    void init();

    RibbonPage* addPage(const QString &text);
    void addPage(RibbonPage *page);
    RibbonPage* insertPage(int index, const QString &text);
    RibbonTitleBar* getTitleBar() const;

    void removePage(RibbonPage *page);
    int pageCount() const;

signals:

public slots:

    void slotCurrentChanged(int index);

private:
    QTabBar *tab_bar_;
    QStackedWidget *page_bar_;
    QMap<QString, RibbonPage*> page_map_;
    RibbonTitleBar *ribbon_titlebar_;

    bool show_ribbon_;
};

}
}

#endif // RIBBON_H
