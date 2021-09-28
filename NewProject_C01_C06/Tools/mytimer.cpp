#include "mytimer.h"
#include <QDebug>
#include <QTimerEvent>

#define TIMER_TIMEOUT (1000)

MyTimer::MyTimer(QObject* parent) : QObject(parent) {}

MyTimer::~MyTimer() {}

void MyTimer::beginTimer(int time)
{
    m_nTimerID = startTimer(time);
}

void MyTimer::cancellTimer()
{
    if (m_nTimerID != -1)
        killTimer(m_nTimerID);
}

void MyTimer::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_nTimerID) {
        handleTimeout();
    }
}

void MyTimer::handleTimeout()
{
    emit signalUpdateTimeUI();
}
