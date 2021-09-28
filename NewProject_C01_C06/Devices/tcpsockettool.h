#ifndef TCPSOCKETTOOL_H
#define TCPSOCKETTOOL_H

#include <QObject>
#include <basecommunication.h>
#include <QTcpSocket>
#include <QMessageBox>
#include "string.h"
#include <QTime>
#include <QTimer>
#include <QEventLoop>
#include <mutex>
#include "items.h"

typedef void (*TcpCallback)(const QByteArray& recv, Items* tempItem, void* context);

class TcpSocketTool : public QObject, public BaseCommunicationInterface
{
    Q_OBJECT
public:
    explicit TcpSocketTool(QObject* parent = nullptr);
    ~TcpSocketTool();

    bool open(const DeviceInfo& info);
    Q_INVOKABLE void closeDevice();
    Q_INVOKABLE void clearBuffer();
    Q_INVOKABLE void sendDataWithResponse(const QString& msg, const float& timeout, Items* tempItem);
    Q_INVOKABLE void sendData(const QString& msg, Items* tempItem);

    void setTcpCallBack(TcpCallback callback, void* context);

private:
    QTcpSocket* mp_clientSocket = NULL;
    void Delay_MSec(unsigned int msec);
    QByteArray array_global;

signals:
    void sendAndRecvSignals(const QString& msg, const float& timeout);
    void errorHappend(const QString& msg);
    void sendCmdSignal(const QString& cmd);

private slots:
    void onSendAndRecv(const QString& msg, const float& timeout);
    void onSendCmd(const QString& cmd);

public slots:
    void ClientRecvData();
    void disconnectPort();

private:
    int m_threadID;
    std::mutex _mutex;

    QString suffix;
    TcpCallback m_tcpCallback = NULL;
    void* m_context = nullptr;
    Items* m_currItem = NULL;
};

#endif  // TCPSOCKETTOOL_H
