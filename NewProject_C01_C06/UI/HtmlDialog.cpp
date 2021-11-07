#include "HtmlDialog.h"
#include "ui_HtmlDialog.h"
#include <QIcon>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include "FileWatcherDialog.h"

const int BtnW = 35;
const QString ZMQResultStr = "Result";
const QString ZMQListStr = "ChangeList";
const QString ZMQErrStr = "ErrorStr";

HtmlDialog::HtmlDialog(QSettings* setting, QWidget* parent)
    : m_setting(setting), QDialog(parent), ui(new Ui::HtmlDialog)
{
    ui->setupUi(this);
    Qt::WindowFlags windowFlag = Qt::Dialog;
    windowFlag |= Qt::WindowMaximizeButtonHint;
    windowFlag |= Qt::WindowCloseButtonHint;
    setWindowFlags(windowFlag);

    connect(this, &HtmlDialog::showWaring, this, &HtmlDialog::onShowWaring, Qt::BlockingQueuedConnection);
    connect(this, &HtmlDialog::showFileWatcherDialig, this, &HtmlDialog::onShowFileWatcherDialig,
            Qt::BlockingQueuedConnection);

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
    connect(m_leftBtn, &QPushButton::clicked, this, &HtmlDialog::onLeftBtnClicked);

    m_rightBtn = new QPushButton(this);
    m_rightBtn->setFlat(true);
    m_rightBtn->setText("");
    m_rightBtn->resize(BtnW, BtnW);
    m_rightBtn->setIconSize(QSize(BtnW, BtnW));
    m_rightBtn->setIcon(QIcon(QPixmap(":/Debug/Debug/nextt.png")));
    m_rightBtn->setHidden(true);
    connect(m_rightBtn, &QPushButton::clicked, this, &HtmlDialog::onRrightBtnClicked);

    m_zmqRep = CreateZmqRepHandle();
    m_zmqRep->SetCallBack(zmqRepCallBack, this);
    m_zmqRep->Bind("127.0.0.1", 4399);
    m_zmqRep->Start();
}

HtmlDialog::~HtmlDialog()
{
    delete ui;
}

std::string HtmlDialog::zmqRepCallBack(void* buf, long len, void* context)
{
    QVariantMap cmdMap;
    HtmlDialog* pThis = (HtmlDialog*)context;

    try {
        if (len < 1) {
            cmdMap.insert(ZMQResultStr, false);
            cmdMap.insert(ZMQErrStr, "The buffer read is empty.");
            QJsonDocument doc = QJsonDocument::fromVariant(cmdMap);
            QByteArray array = doc.toJson();
            QString str(array);
            return str.toStdString();
        }

        const QString recvMsg = QString::fromLocal8Bit((char*)buf, len);

        QJsonParseError json_error;
        QJsonDocument document = QJsonDocument::fromJson(recvMsg.toUtf8(), &json_error);
        if (json_error.error != QJsonParseError::NoError || !document.isObject()) {
            cmdMap.insert(ZMQResultStr, false);
            cmdMap.insert(ZMQErrStr, json_error.errorString());
            QJsonDocument doc = QJsonDocument::fromVariant(cmdMap);
            QByteArray array = doc.toJson();
            QString str(array);
            return str.toStdString();
        }

        QJsonObject obj = document.object();
        QString errStr = obj.value("ErrorStr").toString();
        QJsonArray changeList = obj.value("ChangeList").toArray();

        if (!errStr.isEmpty()) {
            emit pThis->showWaring(errStr);
            cmdMap.insert(ZMQResultStr, false);
            cmdMap.insert(ZMQErrStr, "Show Python error to user finished.");
            QJsonDocument doc = QJsonDocument::fromVariant(cmdMap);
            QByteArray array = doc.toJson();
            QString str(array);
            return str.toStdString();
        }

        bool isok;
        emit pThis->showFileWatcherDialig(changeList, &isok);

        cmdMap.insert(ZMQResultStr, isok);
        cmdMap.insert(ZMQErrStr, "");
        QJsonDocument doc = QJsonDocument::fromVariant(cmdMap);
        QByteArray array = doc.toJson();
        QString str(array);
        return str.toStdString();
    }
    catch (...) {
        cmdMap.insert(ZMQResultStr, false);
        cmdMap.insert(ZMQErrStr, "Some error happened.");
        QJsonDocument doc = QJsonDocument::fromVariant(cmdMap);
        QByteArray array = doc.toJson();
        QString str(array);
        return str.toStdString();
    }
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
}

void HtmlDialog::leaveEvent(QEvent*)
{
    m_leftBtn->setHidden(true);
    m_rightBtn->setHidden(true);
}

void HtmlDialog::onLeftBtnClicked()
{
    QUrl baseUrl = "file:///C:/Users/Zhigen/Desktop/ChangeLog/2021-11-05-20-44-39.html";
    m_webView->setUrl(baseUrl);
}

void HtmlDialog::onRrightBtnClicked()
{
    QUrl baseUrl = "file:///C:/Users/Zhigen/Desktop/ChangeLog/2021-11-05-20-57-55.html";
    m_webView->setUrl(baseUrl);
}

void HtmlDialog::onShowWaring(const QString& msg)
{
    QMessageBox::warning(this, "Warning", msg);
}

void HtmlDialog::onShowFileWatcherDialig(const QJsonArray& jsonArr, bool* isok)
{
    FileWatcherDialog fDialog(jsonArr, m_setting, isok);
    fDialog.exec();
}
