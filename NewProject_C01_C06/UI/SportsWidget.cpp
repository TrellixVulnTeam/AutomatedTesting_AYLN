#include "SportsWidget.h"
#include "ui_SportsWidget.h"
#include <QPainter>
#include <QBrush>
#include <QDebug>

const int btnW = 45;
const int btnH = 45;
const QStringList lightSourceColors = { "#1d953f", "#ea66a6", "#decb00" };
const QStringList productColors = { "BDA377", "C78B8F", "#de773f" };

SportsWidget::SportsWidget(QWidget* parent) : QWidget(parent), ui(new Ui::SportsWidget)
{
    ui->setupUi(this);
    setAutoFillBackground(true);
    // x
    xButton1 = new MoveBtn(this);
    xButton1->setName("X+");
    xButton1->setSize(btnW, btnH);
    xButton1->setBackground(lightSourceColors.at(0));
    xButton1->setIcon(":/Icons/1.png");
    connect(xButton1, &MoveBtn::clicked, this, &SportsWidget::onXBtn1Clicked);

    xButton2 = new MoveBtn(this);
    xButton2->setName("X-");
    xButton2->setSize(btnW, btnH);
    xButton2->setBackground(lightSourceColors.at(0));
    xButton2->setIcon(":/Icons/2.png");
    connect(xButton2, &MoveBtn::clicked, this, &SportsWidget::onXBtn2Clicked);

    // y
    yButton1 = new MoveBtn(this);
    yButton1->setName("Y+");
    yButton1->setSize(btnW, btnH);
    yButton1->setBackground(lightSourceColors.at(1));
    yButton1->setIcon(":/Icons/4.png");
    connect(yButton1, &MoveBtn::clicked, this, &SportsWidget::onYBtn1Clicked);

    yButton2 = new MoveBtn(this);
    yButton2->setName("Y-");
    yButton2->setSize(btnW, btnH);
    yButton2->setBackground(lightSourceColors.at(1));
    yButton2->setIcon(":/Icons/3.png");
    connect(yButton2, &MoveBtn::clicked, this, &SportsWidget::onYBtn2Clicked);

    // z
    zButton1 = new MoveBtn(this);
    zButton1->setName("Z+");
    zButton1->setSize(btnW, btnH);
    zButton1->setBackground(lightSourceColors.at(2));
    zButton1->setIcon(":/Icons/2.png");
    connect(zButton1, &MoveBtn::clicked, this, &SportsWidget::onZBtn1Clicked);

    zButton2 = new MoveBtn(this);
    zButton2->setName("Z-");
    zButton2->setSize(btnW, btnH);
    zButton2->setBackground(lightSourceColors.at(2));
    zButton2->setIcon(":/Icons/1.png");
    connect(zButton2, &MoveBtn::clicked, this, &SportsWidget::onZBtn2Clicked);
}

SportsWidget::~SportsWidget()
{
    delete ui;
}

void SportsWidget::onXBtn1Clicked()
{
    qDebug() << xButton1->getName() + " clicked.";
}

void SportsWidget::onXBtn2Clicked()
{
    qDebug() << xButton2->getName() + " clicked.";
}

void SportsWidget::onYBtn1Clicked()
{
    qDebug() << yButton1->getName() + " clicked.";
}

void SportsWidget::onYBtn2Clicked()
{
    qDebug() << yButton2->getName() + " clicked.";
}

void SportsWidget::onZBtn1Clicked()
{
    qDebug() << zButton1->getName() + " clicked.";
}

void SportsWidget::onZBtn2Clicked()
{
    qDebug() << zButton2->getName() + " clicked.";
}

void SportsWidget::paintEvent(QPaintEvent* event)
{
    int w = this->width();
    int h = this->height();

    if (w <= btnW * 2 || h <= btnH * 2) {
        QWidget::paintEvent(event);
        return;
    }

    //    if (isHidden() || !isVisible()) {
    //        QWidget::paintEvent(event);
    //        return;
    //    }

    //    if (w != m_lastWidth || h != m_lastHeight) {
    //    }

    int btnReserved = 10;
    m_lastWidth = w;
    m_lastHeight = h;

    QPainter painter(this);
    QPen pen(QColor(lightSourceColors.at(0)), 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawPixmap(rect(), QPixmap(":/MainUI/dxd.png"));

    xButton1->move((w - btnW) / 2, btnReserved);
    xButton1->show();

    xButton2->move((w - btnW) / 2, h - btnReserved - btnH);
    xButton2->show();
    painter.drawLine(QPoint((w - btnW) / 2 + btnW / 2, btnReserved + btnH),
                     QPoint((w - btnW) / 2 + btnW / 2, h - btnReserved - btnH));

    int half_w = w / 2;
    int half_h = h / 2;
    int halfLineLen = half_h - btnH - btnReserved;

    int h_mismatch = halfLineLen / 2;
    int w_mismatch = sqrt(pow(halfLineLen, 2) - pow(h_mismatch, 2));

    yButton1->move(half_w + w_mismatch, half_h + h_mismatch);
    yButton1->show();
    yButton2->move(half_w - w_mismatch - btnW, half_h - h_mismatch - btnH);
    yButton2->show();
    pen.setColor(QColor(lightSourceColors.at(1)));
    painter.setPen(pen);
    painter.drawLine(QPoint(half_w - w_mismatch, half_h - h_mismatch),
                     QPoint(half_w + w_mismatch, half_h + h_mismatch));

    zButton1->move(half_w - w_mismatch - btnW, half_h + h_mismatch);
    zButton1->show();
    zButton2->move(half_w + w_mismatch, half_h - h_mismatch - btnH);
    zButton2->show();
    pen.setColor(QColor(lightSourceColors.at(2)));
    painter.setPen(pen);
    painter.drawLine(QPoint(half_w - w_mismatch, half_h + h_mismatch),
                     QPoint(half_w + w_mismatch, half_h - h_mismatch));

    QWidget::paintEvent(event);
}
