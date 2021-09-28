#include "mainaction.h"

mainAction::mainAction(const int& num, const QMap<QString, QVariant>& tempDeviceList, QObject* parent) : QObject(parent)
{
    this->unitNum = num;
    this->deviceDic = tempDeviceList;

    QThread* m_pThread = new QThread(this);
    this->moveToThread(m_pThread);
    connect(this, &mainAction::startSignals, this, &mainAction::startTest);
    m_pThread->start();

    emit startSignals();
}

void mainAction::startTest()
{

    serialportTool* serialport_tool = (serialportTool*)this->deviceDic["CTR_Uart"].value<void*>();

    QString tempString = serialport_tool->sendDataWithResponse("start", 2);

    qDebug() << tempString;

    qDebug() << "Method of re creating thread";

    qDebug() << __FUNCTION__ << " currentThreadId = " << QThread::currentThread();
}
