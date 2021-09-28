#ifndef BASECOMMUNICATION_H
#define BASECOMMUNICATION_H

#include <QMap>
#include "ConfigParse.h"

class BaseCommunicationInterface
{

public:
    virtual bool open(const DeviceInfo& info) = 0;
    virtual void clearBuffer() = 0;
    virtual void closeDevice() = 0;
    void sendData(const QString& msg);
    QString sendDataWithResponse(const QString& msg, const float& timeout);
};

#endif  // BASECOMMUNICATION_H
