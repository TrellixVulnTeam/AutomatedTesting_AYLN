#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QObject>
#include <QDebug>
#include "items.h"


class LogUiManager : public QObject
{
    Q_OBJECT
public:
    explicit LogUiManager(QObject *parent = nullptr);


    void updateLogView(Items * testItems);

signals:
    void updateLogViewSignals(const QString &logStr);


public slots:

};

#endif // LOGMANAGER_H
