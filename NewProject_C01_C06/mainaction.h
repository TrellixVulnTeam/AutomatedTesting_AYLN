#ifndef MAINACTION_H
#define MAINACTION_H

#include <QObject>
#include <items.h>
#include <QMap>
#include <QDebug>
#include <serialporttool.h>
#include <QThread>

class mainAction : public QObject
{
    Q_OBJECT
public:
    explicit mainAction(const int&, const QMap<QString, QVariant>&, QObject* parent = nullptr);

    int unitNum;
    QList<Items*> testItemList;
    QMap<QString, QVariant> deviceDic;

signals:
    void startSignals();

public slots:
    void startTest();
};

#endif  // MAINACTION_H
