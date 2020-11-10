#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>


namespace open_edi {
namespace gui {

class ToolBar : public QToolBar
{
public:
    explicit ToolBar(const QString &title, QWidget *parent);
    ~ToolBar();

private:
};

}
}
#endif // TOOLBAR_H
