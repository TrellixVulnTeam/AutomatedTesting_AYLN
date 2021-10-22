#include "StartLoadForm.h"
#include <QPainter>

StartLoadForm::StartLoadForm(QWidget* parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);           // 设置窗口无边框
    setAttribute(Qt::WA_TranslucentBackground, true);  // 设置窗口半透明
    setFixedSize(900, 600);
    setWindowFlags(Qt::SplashScreen | Qt::WindowStaysOnTopHint);
}

void StartLoadForm::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    if (m_num == 0) {
        QPainter painter(this);
        painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
        painter.drawPixmap(rect(), QPixmap(":/MainUI/dxd.png"));
    }
    m_num++;
}
