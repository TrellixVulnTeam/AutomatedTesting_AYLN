#ifndef SERIALPORTTOOL_H
#define SERIALPORTTOOL_H

#include <QObject>
#include <basecommunication.h>
#include <QSerialPort>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QEventLoop>
#include <mutex>

class serialportTool : public QObject, public BaseCommunicationInterface
{
    Q_OBJECT
public:
    explicit serialportTool(QObject* parent = nullptr);
    ~serialportTool();
    bool open(const DeviceInfo& info);
    Q_INVOKABLE void closeDevice();
    void sendData(const QString& msg);
    Q_INVOKABLE void clearBuffer();
    Q_INVOKABLE QString sendDataWithResponse(const QString& msg, const float& timeout);

private:
    QSerialPort* serialPort = NULL;
    void Delay_MSec(unsigned int msec);

signals:
    void sendAndRecvSignals(const QString& msg, const float& timeout);
    void errorHappend(const QString& msg);

private slots:
    void onSendAndRecv(const QString& msg, const float& timeout);

public slots:
    void receiveData();

private:
    int m_threadID;
    std::mutex _mutex;

    QString response;
    QString suffix;
};

#endif  // SERIALPORTTOOL_H
