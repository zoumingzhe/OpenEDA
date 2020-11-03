#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QDockWidget>
#include <QPlainTextEdit>


namespace open_edi {
namespace gui {

class LogWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit LogWidget(const QString &title, QWidget *parent = nullptr, Qt::WindowFlags flags = 0);
    ~LogWidget();

signals:

public slots:

private:
    QPlainTextEdit   *log_text_;
};


}
}
#endif // LOGWIDGET_H
