#ifndef JSONTOOL_H
#define JSONTOOL_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include "items.h"
#include "configitem.h"

class jsonTool : public QObject
{
    Q_OBJECT
public:
    explicit jsonTool(QString path, QObject* parent = nullptr);

    QString getItemsFromJson(const QString& path, const QString& key);

    void getConfigFromJson(const QString& path);

    void saveDataToJson(const QString& path, const QString& key, const QString& value);

    QList<Items*> itemList;
    QList<ConfigItem*> deviceList;
    QList<QString> chooseList;
    int UnitCount;
    QString headTitle;

private:
    void generateHeadTitle(const QList<Items*>& tempList);

signals:

public slots:
};

#endif  // JSONTOOL_H
