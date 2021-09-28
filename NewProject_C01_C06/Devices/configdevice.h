#ifndef CONFIGDEVICE_H
#define CONFIGDEVICE_H

#include <QObject>
#include <QDebug>
#include <serialporttool.h>
#include <tcpsockettool.h>
#include <QThread>
#include <QSerialPortInfo>
#include "ConfigParse.h"

class ConfigDevice : public QObject
{
    Q_OBJECT
public:
    explicit ConfigDevice(QObject* parent = nullptr);
    void clearBuffer();
    void setTcpCallBack(TcpCallback callback, void* context);

    QMap<QString, QVariant> deviceDic;

    int size()
    {
        return m_connectFailedDevices.size();
    }

    std::vector<std::string> getConnectFailedDevices() const
    {
        return m_connectFailedDevices;
    }

signals:
    void connectedDeviceSignal(const std::vector<std::string>& names);
    void errorHappend(const QString& msg);

public slots:
    void onConnectDevices(std::map<std::string, DeviceInfo>& devicMap, const QString& soltNum);

private:
    std::vector<std::string> m_connectFailedDevices;
    std::thread* m_connectThread = NULL;
};

#endif  // CONFIGDEVICE_H
