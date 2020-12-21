#ifndef RIBBON_PAGE_H
#define RIBBON_PAGE_H

#include <QWidget>
#include <QHBoxLayout>

namespace open_edi {
namespace gui {

class RibbonGroup;

class RibbonPage : public QWidget
{
    Q_OBJECT
public:
    explicit RibbonPage(QWidget *parent = nullptr);
    ~RibbonPage();

    void init();

    void addGroup(RibbonGroup *group);
    RibbonGroup* addGroup(QString title);
    void removeGroup(RibbonGroup *group);
    void clearGroups();
    void setTitle(const QString &title);
    QString title() const;

signals:

public slots:

private:
    QString page_title_;
    QHBoxLayout *main_layout_;
    QList<RibbonGroup*> group_list_;
};


}
}
#endif // RIBBON_PAGE_H
