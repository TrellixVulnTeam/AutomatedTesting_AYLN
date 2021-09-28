#ifndef CONFIGITEM_H
#define CONFIGITEM_H

#include <QObject>
#include <QJsonObject>

class ConfigItem : public QObject
{
    Q_OBJECT
public:
    explicit ConfigItem(QJsonObject& jsonObject, QObject* parent = 0);

    QString target;
    QString path;
    QString num;
    QString ref;
    QString timeout;
    QString suffix;
    QString exist;
    QString Propert;
    QString index;

signals:

public slots:
};

#endif // CONFIGITEM_H
