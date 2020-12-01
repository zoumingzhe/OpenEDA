#ifndef ELABEL_H
#define ELABEL_H

#include <QLabel>

namespace open_edi {
namespace gui {

class ELabel : public QLabel
{
    Q_OBJECT
public:
    explicit ELabel(QWidget *parent = nullptr);

    void setPixmap(const QPixmap &pixmap);

signals:
    void show();
    void hide();

public slots:

private:
    void mousePressEvent(QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private:
    QPixmap pixmap_;
    bool    is_Show_;
};

}

}

#endif // ELABEL_H
