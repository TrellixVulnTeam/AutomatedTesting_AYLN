#include "dealdatatool.h"
#include "QRegularExpression"
#include "QRegularExpressionMatch"
#include <QDebug>

dealdatatool::dealdatatool(QObject *parent) : QObject(parent)
{

}


QString dealdatatool::calcultate(const QString &calcultateStr){

    QScriptValue scripVal = scriptEngin.evaluate(calcultateStr).toString();
    return scripVal.toString();

}



QString dealdatatool::calcultate(const QString &calcultateStr,const QMap<QString,QString> mapInfo){

    QString newCalcultateStr = calcultateStr;
    QString pattern3 = "\\w+";
    QRegularExpression regularExpression(pattern3);
        int index = 0;
        QRegularExpressionMatch match;
        do {
            match = regularExpression.match(calcultateStr,index);
            if(match.hasMatch()) {
                index = match.capturedEnd();
                QString key = match.captured(0);
                qDebug()<<"("<<match.capturedStart()<<","<<index<<") "<<key;

                if(mapInfo.contains(key)){

                     newCalcultateStr.replace(key,mapInfo[key]);
                }
            } else {
                break;
            }
        } while(index < calcultateStr.length());

     //从mapInfo获取具体内容替换
     QScriptValue scripVal = scriptEngin.evaluate(newCalcultateStr).toString();

     return scripVal.toString();
}
