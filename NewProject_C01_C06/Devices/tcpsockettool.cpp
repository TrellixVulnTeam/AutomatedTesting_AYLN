#include "tcpsockettool.h"
#include <QThread>
#include "LogHelper.h"
#include "ml_file.h"
#include "filetool.h"

TcpSocketTool::TcpSocketTool(QObject* parent) : QObject(parent)
{
    mp_clientSocket = new QTcpSocket();
    connect(mp_clientSocket, SIGNAL(readyRead()), this, SLOT(ClientRecvData()));
    connect(mp_clientSocket, SIGNAL(disconnected()), this, SLOT(disconnectPort()));
    connect(this, &TcpSocketTool::sendAndRecvSignals, this, &TcpSocketTool::onSendAndRecv,
            Qt::BlockingQueuedConnection);
    connect(this, &TcpSocketTool::sendCmdSignal, this, &TcpSocketTool::onSendCmd, Qt::BlockingQueuedConnection);
    m_threadID = GetCurrentThreadId();
}

TcpSocketTool::~TcpSocketTool()
{
    if (NULL != mp_clientSocket) {
        delete mp_clientSocket;
        mp_clientSocket = NULL;
    }
}

bool TcpSocketTool::open(const DeviceInfo& info)
{
    QString ip = QString::fromStdString(info.path);
    int port = info.num;
    suffix = QString::fromStdString(info.suffix);

    mp_clientSocket->connectToHost(ip, port);
    if (!mp_clientSocket->waitForConnected(1000)) {
        return false;
    } else {
        return true;
    }
}

void TcpSocketTool::clearBuffer()
{
    array_global.clear();
}

void TcpSocketTool::closeDevice()
{
    mp_clientSocket->close();
}

void TcpSocketTool::onSendCmd(const QString& cmd)
{
    if (!mp_clientSocket->isWritable() || !mp_clientSocket->isReadable()) {
        emit errorHappend("ClientSocket status is incorrect. Read/write operations are prohibited.");
        return;
    }

    char sendMsgChar[1024] = { 0 };
    strcpy(sendMsgChar, cmd.toStdString().c_str());
    int sendRe = mp_clientSocket->write(sendMsgChar, strlen(sendMsgChar));
    if (sendRe == -1) {
        emit errorHappend("ClientSocket response error.");
    }
}

void TcpSocketTool::onSendAndRecv(const QString& msg, const float& timeout)
{
    if (!mp_clientSocket->isWritable() || !mp_clientSocket->isReadable()) {
        emit errorHappend("ClientSocket status is incorrect. Read/write operations are prohibited.");
        return;
    }

    char sendMsgChar[1024] = { 0 };
    strcpy(sendMsgChar, msg.toStdString().c_str());
    int sendRe = mp_clientSocket->write(sendMsgChar, strlen(sendMsgChar));
    if (sendRe == -1) {
        return;
    }

    float temp_timeout = 0;
    while (temp_timeout < timeout) {

        Delay_MSec(10);
        temp_timeout = temp_timeout + 0.01;

        if (array_global.contains(suffix.toLatin1())) {
            break;
        }
    }

    if (!array_global.contains(QByteArray::fromStdString(suffix.toStdString()))) {
        emit errorHappend(QString("ClientSocket send: %1 failed, timeout %2").arg(msg).arg(timeout));
    }
}

void TcpSocketTool::sendData(const QString& msg, Items* tempItem)
{
    std::unique_lock<std::mutex> lock(_mutex);
    m_currItem = tempItem;
    array_global.clear();
    if (GetCurrentThreadId() == m_threadID)
        onSendCmd(msg);
    else
        emit sendCmdSignal(msg);
}

void TcpSocketTool::sendDataWithResponse(const QString& msg, const float& timeout, Items* tempItem)
{
    std::unique_lock<std::mutex> lock(_mutex);
    m_currItem = tempItem;
    array_global.clear();
    if (GetCurrentThreadId() == m_threadID)
        onSendAndRecv(msg, timeout);
    else
        emit sendAndRecvSignals(msg, timeout);
}

void TcpSocketTool::ClientRecvData()
{
    try {
        QByteArray array = mp_clientSocket->readAll();
        if (!array.isEmpty()) {
            array_global.append(array);
        }

        if (array_global.length() > 200 && array_global.contains(suffix.toLatin1())) {
            if (NULL != m_tcpCallback) {
                m_tcpCallback(array_global, m_currItem, m_context);
            }
        }
    }
    catch (...) {
        emit errorHappend(QString("TcpSocketTool ClientRecvData error."));
    }
}

void TcpSocketTool::disconnectPort()
{
    if (NULL != mp_clientSocket && mp_clientSocket->isOpen()) {
        mp_clientSocket->close();
        array_global.clear();
    }
}

void TcpSocketTool::Delay_MSec(unsigned int msec)
{
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, SLOT(quit()));
    loop.exec();
}

void TcpSocketTool::setTcpCallBack(TcpCallback callback, void* context)
{
    m_tcpCallback = callback;
    m_context = context;
}
