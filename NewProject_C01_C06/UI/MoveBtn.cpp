#include "MoveBtn.h"
#include "ui_MoveBtn.h"
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

MoveBtn::MoveBtn(QWidget* parent) : QWidget(parent), ui(new Ui::MoveBtn)
{
    ui->setupUi(this);
    setSize();
}

MoveBtn::~MoveBtn()
{
    delete ui;
}

QString MoveBtn::getName() const
{
    return ui->nameLabel->text();
}

void MoveBtn::setBackground(const QString& cStr)
{
    setStyleSheet(QString("QWidget{background-color:%1;}"
                          "QWidget#MoveBtn{border: 2px solid black;}")
                      .arg(cStr));
    m_color = cStr;
}

void MoveBtn::setSize(int w, int h)
{
    resize(w, h);
}

void MoveBtn::setName(const QString& name)
{
    ui->nameLabel->setText(name);
}

void MoveBtn::setIcon(const QString& iconPath)
{
    ui->iconLabel->setPixmap(QPixmap(iconPath));
}

void MoveBtn::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        setStyleSheet("QWidget{background-color:#00ae9d;}"
                      "QWidget#MoveBtn{border: 2px solid black;}");
        emit clicked();
    }
}

void MoveBtn::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    setStyleSheet(QString("QWidget{background-color:%1;}"
                          "QWidget#MoveBtn{border: 2px solid black;}")
                      .arg(m_color));
}

void MoveBtn::enterEvent(QEvent* event)
{
    Q_UNUSED(event);
    setStyleSheet("QWidget{background-color:gray;}"
                  "QWidget#MoveBtn{border: 2px solid black;}");
}

void MoveBtn::leaveEvent(QEvent* event)
{
    Q_UNUSED(event);
    setStyleSheet(QString("QWidget{background-color:%1;}"
                          "QWidget#MoveBtn{border: 2px solid black;}")
                      .arg(m_color));
}
