#ifndef RIBBON_GROUP_H
#define RIBBON_GROUP_H

#include <QWidget>
#include <QToolBar>
#include <QBoxLayout>
#include <QActionGroup>
#include "ribbon_button.h"
#include <QLabel>

namespace open_edi {
namespace gui {

class RibbonGroup : public QWidget
{
    Q_OBJECT
public:

    explicit RibbonGroup(QString title, QWidget *parent = nullptr);
    ~RibbonGroup();

    void init();

    void setTitle(const QString title);

    void addAction(QAction* action, int row, int col, int rowSpan = 1, int columnSpan = 1,
                   Qt::Alignment alignment = Qt::Alignment());

    void addAction(const QIcon &icon, const QString &text, Qt::ToolButtonStyle style);
    void addAction(QAction *action, Qt::ToolButtonStyle style);

    void addWidget(QWidget *widget, int row, int col, int rowSpan = 1, int columnSpan = 1,
                   Qt::Alignment alignment = Qt::Alignment());
    void removeWidget(QWidget *widget);
    void clear();

    QMenu* addMenu(const QIcon &icon, const QString &text, Qt::ToolButtonStyle style = Qt::ToolButtonFollowStyle);

    QActionGroup* getActionGroup() const;

    void setExpandButtonVisible(bool visible = true);
    bool isExpandButtonVisible() const;

protected:

signals:
    void popButtonClicked();

public slots:

private:
    QToolBar *large_bar_;
    QToolBar *small_bar_;
    QHBoxLayout *group_layout_;
    QGridLayout *grid_layout_;
    QLabel* group_title_;
    RibbonButton *pop_button_;
};

}
}
#endif // RIBBON_GROUP_H
