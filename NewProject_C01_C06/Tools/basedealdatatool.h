#ifndef BASEDEALDATATOOL_H
#define BASEDEALDATATOOL_H


#include <QMap>
class Basedealdatatoolinterface
{

public:
   virtual QString calcultate(const QString &calcultateStr,const QMap<QString,QString> mapInfo)=0;
};


#endif // BASEDEALDATATOOL_H
