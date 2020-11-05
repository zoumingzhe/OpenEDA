#include "log_widget.h"

namespace open_edi {
namespace gui {

LogWidget::LogWidget(const QString &title, QWidget *parent, Qt::WindowFlags flags)
    : QDockWidget(parent, flags)
{
    setObjectName(title);
    setWindowTitle(title);

    log_text_ = new QPlainTextEdit(this);
    log_text_->setReadOnly(true);
    setWidget(log_text_);
}

LogWidget::~LogWidget()
{
    delete  log_text_;
    log_text_ = nullptr;
}

}
}
