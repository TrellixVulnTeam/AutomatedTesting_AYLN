#ifndef DEALDATATOOL_H
#define DEALDATATOOL_H

#include <QObject>
#include <QScriptEngine>
#include <basedealdatatool.h>


class dealdatatool : public QObject,public Basedealdatatoolinterface
{
     Q_OBJECT

public:
    explicit dealdatatool(QObject *parent = nullptr);

    Q_INVOKABLE QString calcultate(const QString &calcultateStr,const QMap<QString,QString> mapInfo);

    Q_INVOKABLE QString calcultate(const QString &calcultateStr);

private:
    QScriptEngine scriptEngin;
};


#endif // DEALDATATOOL_H
