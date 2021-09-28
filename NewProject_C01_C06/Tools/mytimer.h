#ifndef MYTIMER_H
#define MYTIMER_H

#include <QObject>

class MyTimer : public QObject
{
    Q_OBJECT
public:
    explicit MyTimer(QObject* parent = nullptr);
    ~MyTimer();
    void handleTimeout();
    void beginTimer(int time);
    void cancellTimer();
    virtual void timerEvent(QTimerEvent* event);

private:
    int m_nTimerID = -1;

signals:
    void signalUpdateTimeUI();

public slots:
};

#endif  // MYTIMER_H
