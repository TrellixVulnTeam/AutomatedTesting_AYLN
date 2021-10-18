#include "configdevice.h"
#include "LogHelper.h"

ConfigDevice::ConfigDevice(QObject* parent) : QObject(parent) {}

void ConfigDevice::onConnectDevices(std::map<std::string, DeviceInfo>& devicMap, const QString& soltNum)
{
    bool isConnectSuccess = true;

    for (std::map<std::string, DeviceInfo>::iterator it = devicMap.begin(); it != devicMap.end(); ++it) {
        DeviceInfo dev = it->second;
        if (dev.target == "Uart" && dev.exist && (dev.ref.find(soltNum.toStdString() + "1") != std::string::npos || dev.ref.find(soltNum.toStdString() + "2") != std::string::npos)) {
            serialportTool* serialport_tool = new serialportTool(this);
            connect(serialport_tool, &serialportTool::errorHappend, this, &ConfigDevice::errorHappend);

            std::string comname = dev.path;  //"AU045DSM";

            foreach (const QSerialPortInfo& info, QSerialPortInfo::availablePorts()) {
                if (info.serialNumber().contains(QString::fromStdString(dev.path))) {
                    dev.path = info.systemLocation().toStdString();
                    break;
                }
            }

            if (serialport_tool->open(dev)) {
                QVariant temp_QVariant = QVariant::fromValue((void*)serialport_tool);
                deviceDic.insert(QString::fromStdString(dev.ref), temp_QVariant);
                qDebug() << "serialport_tool open success!" << QString::fromStdString(dev.ref);

            } else {
                isConnectSuccess = false;
                m_connectFailedDevices.push_back(dev.ref);
                LOG_FATAL("[Unit%d] Serial port connect failed:SerialportName->: %s baudrate-> %d", soltNum.toInt(),
                          comname.c_str(), dev.num);
            }
        } else if (dev.target == "Socket" && dev.exist && dev.ref.find(soltNum.toStdString()) != std::string::npos) {

            TcpSocketTool* socketTool = new TcpSocketTool(this);
            connect(socketTool, &TcpSocketTool::errorHappend, this, &ConfigDevice::errorHappend);

            if (socketTool->open(dev)) {
                QVariant temp_QVariant = QVariant::fromValue((void*)socketTool);
                deviceDic.insert(QString::fromStdString(dev.ref), temp_QVariant);
                qDebug() << "socketTool open success!" << QString::fromStdString(dev.ref);

            } else {
                isConnectSuccess = false;
                LOG_FATAL("[Unit%d] Failed to connect to server, ip %s port %d", soltNum.toInt(), dev.path.c_str(),
                          dev.num);
            }
        } else if (dev.target == "Aglient" && dev.exist && dev.ref.find(soltNum.toStdString()) != std::string::npos) {

        } else {
        }
    }

    if (isConnectSuccess)
        emit connectedDeviceSignal(m_connectFailedDevices);
}

void ConfigDevice::clearBuffer()
{
    QMap<QString, QVariant>::iterator iter = deviceDic.begin();
    while (iter != deviceDic.end()) {
        QObject* deviceObject = (QObject*)iter.value().value<void*>();
        QMetaObject::invokeMethod(deviceObject, "clearBuffer", Qt::BlockingQueuedConnection);
        iter++;
    }
}

void ConfigDevice::setTcpCallBack(TcpCallback callback, void* context)
{
    QMap<QString, QVariant>::iterator iter = deviceDic.begin();
    while (iter != deviceDic.end()) {
        QString ref = iter.key();
        if (ref.contains("Socket")) {
            QObject* deviceObject = (QObject*)iter.value().value<void*>();
            TcpSocketTool* socket = qobject_cast<TcpSocketTool*>(deviceObject);
            socket->setTcpCallBack(callback, context);
        }

        iter++;
    }
}
