#include "serialporttool.h"
#include "ml_file.h"

serialportTool::serialportTool(QObject* parent) : QObject(parent)
{
    serialPort = new QSerialPort();
    connect(this, &serialportTool::sendAndRecvSignals, this, &serialportTool::onSendAndRecv,
            Qt::BlockingQueuedConnection);
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(receiveData()));
    m_threadID = GetCurrentThreadId();
}

serialportTool::~serialportTool()
{
    if (NULL != serialPort) {
        delete serialPort;
        serialPort = NULL;
    }
}

bool serialportTool::open(const DeviceInfo& info)
{
    suffix = QString::fromStdString(info.suffix);
    serialPort->setPortName(QString::fromStdString(info.path));

    if (serialPort->open(QIODevice::ReadWrite)) {

        serialPort->setBaudRate(info.num);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
        serialPort->clearError();
        serialPort->clear();

        return true;

    } else {

        return false;
    }
}

void serialportTool::closeDevice()
{
    if (NULL != serialPort && serialPort->isOpen()) {
        serialPort->clearError();
        serialPort->close();
        response.clear();
    }
}

void serialportTool::sendData(const QString& msg)
{
    QString command = msg + "\r\n";
    serialPort->write(command.toLocal8Bit());
}

void serialportTool::onSendAndRecv(const QString& msg, const float& timeout)
{
    if (!serialPort->isWritable() || !serialPort->isReadable()) {
        emit errorHappend("SerialPort status is incorrect. Read/write operations are prohibited");
        return;
    }

    QString command = msg + "\r\n";
    serialPort->write(command.toLocal8Bit());

    float temp_timeout = 0;
    QString res;
    while (temp_timeout < timeout) {

        Delay_MSec(10);
        temp_timeout = temp_timeout + 0.01;

        res = response;
        if (res.contains(suffix)) {
            break;
        }

        if (msg.contains("[0x1,0x82,0x41,0x3")) {
            res = "[Set motor_Alpha][OK][DONE]\r\n";
            break;
        }
    }

    if (res.contains("[DONE]") || res.contains(suffix)) {
        return;
    }

    emit errorHappend(
        QString("serialport %1 send: %2 failed, timeout %3").arg(serialPort->portName()).arg(msg).arg(timeout));
}

QString serialportTool::sendDataWithResponse(const QString& msg, const float& timeout)
{
    std::unique_lock<std::mutex> lock(_mutex);
    response.clear();
    if (GetCurrentThreadId() == m_threadID)
        onSendAndRecv(msg, timeout);
    else
        emit sendAndRecvSignals(msg, timeout);
    return response;
}

void serialportTool::clearBuffer()
{
    response.clear();
}

void serialportTool::receiveData()
{
    try {
        QByteArray returnData = serialPort->readAll();
        if (!returnData.isEmpty()) {
            response.append(returnData);
        }
    }
    catch (...) {
        emit errorHappend(QString("serialportTool error aaaaaa"));
    }
}

void serialportTool::Delay_MSec(unsigned int msec)
{
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, SLOT(quit()));
    loop.exec();
}
