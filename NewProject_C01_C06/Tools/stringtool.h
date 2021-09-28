#ifndef STRINGTOOL_H
#define STRINGTOOL_H

#include <QObject>
#include <QDebug>
#include <time.h>
#include <QMap>
#include <QScriptEngine>
#include "QRegularExpression"
#include "QRegularExpressionMatch"

class StringTool : public QObject
{
    Q_OBJECT
public:
    explicit StringTool(QObject *parent = nullptr);

    Q_INVOKABLE QString getMiddleStr(const QString &resource,const QString &startStr, QString endStr);

    QString appendCalculateStr(const QString &,QMap<QString,QString> &);

    Q_INVOKABLE QString calcultate(const QString &calcultateStr);

    QString produceNum(int shang);

private:
    QScriptEngine scriptEngin;

signals:

public slots:
};

#endif // STRINGTOOL_H
