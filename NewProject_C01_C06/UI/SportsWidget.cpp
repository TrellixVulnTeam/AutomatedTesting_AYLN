#include "SportsWidget.h"
#include "ui_SportsWidget.h"
#include <QPainter>
#include <QBrush>
#include <QDebug>

const int btnW = 45;
const int btnH = 45;
const QStringList lightSourceColors = { "#1d953f", "#ea66a6", "#decb00" };
const QStringList productColors = { "#BDA377", "#C78B8F", "#de773f" };

SportsWidget::SportsWidget(ModuleType type, QWidget* parent)
    : m_moduleType(type), QWidget(parent), ui(new Ui::SportsWidget)
{
    ui->setupUi(this);
    setAutoFillBackground(true);
    qRegisterMetaType<BtnType>("BtnType");
    // x
    xButton1 = new MoveBtn(this);
    if (m_moduleType == LightSource) {
        xButton1->setName("X+");
        xButton1->setBackground(lightSourceColors.at(0));
    } else {
        xButton1->setName("X1-");
        xButton1->setBackground(productColors.at(0));
    }
    xButton1->setIcon(":/Icons/1.png");
    xButton1->setSize(btnW, btnH);
    connect(xButton1, &MoveBtn::clicked, this, &SportsWidget::onXBtn1Clicked);

    xButton2 = new MoveBtn(this);
    if (m_moduleType == LightSource) {
        xButton2->setName("X-");
        xButton2->setBackground(lightSourceColors.at(0));
    } else {
        xButton2->setName("X1+");
        xButton2->setBackground(productColors.at(0));
    }
    xButton2->setIcon(":/Icons/2.png");
    xButton2->setSize(btnW, btnH);
    connect(xButton2, &MoveBtn::clicked, this, &SportsWidget::onXBtn2Clicked);

    // y
    yButton1 = new MoveBtn(this);
    if (m_moduleType == LightSource) {
        yButton1->setName("Y+");
        yButton1->setIcon(":/Icons/4.png");
        yButton1->setBackground(lightSourceColors.at(1));
    } else {
        yButton1->setName("B-");
        yButton1->setIcon(":/Icons/2.png");
        yButton1->setBackground(productColors.at(1));
    }
    yButton1->setSize(btnW, btnH);
    connect(yButton1, &MoveBtn::clicked, this, &SportsWidget::onYBtn1Clicked);

    yButton2 = new MoveBtn(this);
    if (m_moduleType == LightSource) {
        yButton2->setName("Y-");
        yButton2->setIcon(":/Icons/3.png");
        yButton2->setBackground(lightSourceColors.at(1));
    } else {
        yButton2->setName("B+");
        yButton2->setIcon(":/Icons/1.png");
        yButton2->setBackground(productColors.at(1));
    }
    yButton2->setSize(btnW, btnH);
    connect(yButton2, &MoveBtn::clicked, this, &SportsWidget::onYBtn2Clicked);

    // z
    zButton1 = new MoveBtn(this);
    if (m_moduleType == LightSource) {
        zButton1->setName("Z+");
        zButton1->setIcon(":/Icons/2.png");
        zButton1->setBackground(lightSourceColors.at(2));
    } else {
        zButton1->setName("G-");
        zButton1->setIcon(":/Icons/3.png");
        zButton1->setBackground(productColors.at(2));
    }
    zButton1->setSize(btnW, btnH);
    connect(zButton1, &MoveBtn::clicked, this, &SportsWidget::onZBtn1Clicked);

    zButton2 = new MoveBtn(this);
    if (m_moduleType == LightSource) {
        zButton2->setName("Z-");
        zButton2->setIcon(":/Icons/1.png");
        zButton2->setBackground(lightSourceColors.at(2));
    } else {
        zButton2->setName("G+");
        zButton2->setIcon(":/Icons/4.png");
        zButton2->setBackground(productColors.at(2));
    }
    zButton2->setSize(btnW, btnH);
    connect(zButton2, &MoveBtn::clicked, this, &SportsWidget::onZBtn2Clicked);
}

SportsWidget::~SportsWidget()
{
    delete ui;
}

void SportsWidget::onXBtn1Clicked()
{
    if (m_moduleType == LightSource) {
        emit moduleBtnClicked(XPlusBtn);
    } else {
        emit moduleBtnClicked(X1MinusBtn);
    }
}

void SportsWidget::onXBtn2Clicked()
{
    if (m_moduleType == LightSource) {
        emit moduleBtnClicked(XMinusBtn);
    } else {
        emit moduleBtnClicked(X1PlusBtn);
    }
}

void SportsWidget::onYBtn1Clicked()
{
    if (m_moduleType == LightSource) {
        emit moduleBtnClicked(YPlusBtn);
    } else {
        emit moduleBtnClicked(BMinusBtn);
    }
}

void SportsWidget::onYBtn2Clicked()
{
    if (m_moduleType == LightSource) {
        emit moduleBtnClicked(YMinusBtn);
    } else {
        emit moduleBtnClicked(BPlusBtn);
    }
}

void SportsWidget::onZBtn1Clicked()
{
    if (m_moduleType == LightSource) {
        emit moduleBtnClicked(ZPlusBtn);
    } else {
        emit moduleBtnClicked(GMinusBtn);
    }
}

void SportsWidget::onZBtn2Clicked()
{
    if (m_moduleType == LightSource) {
        emit moduleBtnClicked(ZMinusBtn);
    } else {
        emit moduleBtnClicked(GPlusBtn);
    }
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
    QPen pen(QColor(m_moduleType == LightSource ? lightSourceColors.at(0) : productColors.at(0)), 4, Qt::SolidLine,
             Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    if (m_moduleType == LightSource) {
        painter.drawPixmap(rect(), QPixmap(":/MainUI/LightSource.png"));
    } else {
        painter.drawPixmap(rect(), QPixmap(":/MainUI/Product.png"));
    }

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
    pen.setColor(QColor(m_moduleType == LightSource ? lightSourceColors.at(1) : productColors.at(1)));
    painter.setPen(pen);
    painter.drawLine(QPoint(half_w - w_mismatch, half_h - h_mismatch),
                     QPoint(half_w + w_mismatch, half_h + h_mismatch));

    zButton1->move(half_w - w_mismatch - btnW, half_h + h_mismatch);
    zButton1->show();
    zButton2->move(half_w + w_mismatch, half_h - h_mismatch - btnH);
    zButton2->show();
    pen.setColor(QColor(m_moduleType == LightSource ? lightSourceColors.at(2) : productColors.at(2)));
    painter.setPen(pen);
    painter.drawLine(QPoint(half_w - w_mismatch, half_h + h_mismatch),
                     QPoint(half_w + w_mismatch, half_h - h_mismatch));

    pen.setColor(QColor("black"));
    painter.setPen(pen);
    painter.drawRoundedRect(QRect((w - btnW) / 2, btnReserved, xButton1->width(), xButton1->height()), 3, 3);
    painter.drawRoundedRect(QRect((w - btnW) / 2, h - btnReserved - btnH, xButton2->width(), xButton2->height()), 3, 3);
    painter.drawRoundedRect(QRect(half_w + w_mismatch, half_h + h_mismatch, yButton1->width(), yButton1->height()), 3,
                            3);
    painter.drawRoundedRect(
        QRect(half_w - w_mismatch - btnW, half_h - h_mismatch - btnH, yButton2->width(), yButton2->height()), 3, 3);
    painter.drawRoundedRect(
        QRect(half_w - w_mismatch - btnW, half_h + h_mismatch, zButton1->width(), zButton1->height()), 3, 3);
    painter.drawRoundedRect(
        QRect(half_w + w_mismatch, half_h - h_mismatch - btnH, zButton2->width(), zButton2->height()), 3, 3);

    QWidget::paintEvent(event);
}
