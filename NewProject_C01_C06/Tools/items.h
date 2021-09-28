#ifndef ITEMS_H
#define ITEMS_H

#include <QObject>
#include <QJsonObject>
#include <QStandardItem>
#include <QDebug>

class Items : public QObject
{
    Q_OBJECT
public:
    explicit Items(QObject* parent = 0);
    explicit Items(QJsonObject& jsonObject, int& Index, QObject* parent = 0);
    explicit Items(const QList<QString>& contentList, const int Index, QObject* parent = 0);
    ~Items();

public:
    QString Index;
    QString Status;
    QString Show;
    QString Group;
    QString ItemName;
    QString Snyc;
    QString ClassType;
    QString Target;
    QString Ref;
    QString Function;
    QString Suffix;
    QString Param;
    QString LogicFunction;
    QString StartStr;
    QString EndStr;
    QString Timeout;
    QString Retry;
    QString isBreak;
    QString SaveGlobal;
    QString Global;
    QString lowerLimits;
    QString upperLimits;
    QString Unit;
    QString Relation;

    QString Value;
    QString Response;
    QString StartTime;
    QString StopTime;
    QString others;
    bool Result = true;

    QStandardItem* QSIndex;
    QStandardItem* QSItemName;
    QStandardItem* QSLowerLimits;
    QStandardItem* QSUpperLimits;
    QStandardItem* QSUnit;
    QStandardItem* QSValue;

    void description();
    QString convertParam(const QString& oldStr);

private:
};

#endif  // ITEMS_H
