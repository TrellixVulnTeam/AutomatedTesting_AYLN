#include "HtmlDialog.h"
#include "ui_HtmlDialog.h"
#include <QIcon>

const int BtnW = 35;

HtmlDialog::HtmlDialog(QWidget* parent) : QDialog(parent), ui(new Ui::HtmlDialog)
{
    ui->setupUi(this);
    Qt::WindowFlags windowFlag = Qt::Dialog;
    windowFlag |= Qt::WindowMaximizeButtonHint;
    windowFlag |= Qt::WindowCloseButtonHint;
    setWindowFlags(windowFlag);

    m_webView = new QWebEngineView(this);
    QUrl baseUrl = "file:///C:/Users/Zhigen/Desktop/ChangeLog/2021-11-05-20-57-55.html";
    m_webView->setUrl(baseUrl);
    ui->layout->addWidget(m_webView);

    m_leftBtn = new QPushButton(this);
    m_leftBtn->setFlat(true);
    m_leftBtn->setText("");
    m_leftBtn->resize(BtnW, BtnW);
    m_leftBtn->setIconSize(QSize(BtnW, BtnW));
    m_leftBtn->setIcon(QIcon(QPixmap(":/Debug/Debug/previous.png")));
    m_leftBtn->setHidden(true);
    connect(m_leftBtn, &QPushButton::clicked, this, &HtmlDialog::leftBtnClicked);

    m_rightBtn = new QPushButton(this);
    m_rightBtn->setFlat(true);
    m_rightBtn->setText("");
    m_rightBtn->resize(BtnW, BtnW);
    m_rightBtn->setIconSize(QSize(BtnW, BtnW));
    m_rightBtn->setIcon(QIcon(QPixmap(":/Debug/Debug/nextt.png")));
    m_rightBtn->setHidden(true);
    connect(m_rightBtn, &QPushButton::clicked, this, &HtmlDialog::rightBtnClicked);

    ui->rollbackBtn->setHidden(true);
    connect(ui->rollbackBtn, &QPushButton::clicked, this, &HtmlDialog::rollbackBtnClicked);
}

HtmlDialog::~HtmlDialog()
{
    delete ui;
}

void HtmlDialog::paintEvent(QPaintEvent* event)
{
    int w = this->width();
    int h = this->height();

    m_leftBtn->move(10, (h - BtnW) / 2);
    m_rightBtn->move(w - BtnW - 10, (h - BtnW) / 2);

    QDialog::paintEvent(event);
}

void HtmlDialog::enterEvent(QEvent*)
{
    m_leftBtn->setHidden(false);
    m_rightBtn->setHidden(false);
    ui->rollbackBtn->setHidden(false);
}

void HtmlDialog::leaveEvent(QEvent*)
{
    m_leftBtn->setHidden(true);
    m_rightBtn->setHidden(true);
    ui->rollbackBtn->setHidden(true);
}

void HtmlDialog::leftBtnClicked()
{
    QUrl baseUrl = "file:///C:/Users/Zhigen/Desktop/ChangeLog/2021-11-05-20-44-39.html";
    m_webView->setUrl(baseUrl);
}

void HtmlDialog::rightBtnClicked()
{
    QUrl baseUrl = "file:///C:/Users/Zhigen/Desktop/ChangeLog/2021-11-05-20-57-55.html";
    m_webView->setUrl(baseUrl);
}

void HtmlDialog::rollbackBtnClicked() {}
