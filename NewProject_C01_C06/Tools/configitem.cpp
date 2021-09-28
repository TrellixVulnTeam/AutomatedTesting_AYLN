#include "configitem.h"

ConfigItem::ConfigItem(QJsonObject& jsonObject, QObject* parent) : QObject(parent)
{

    target = jsonObject.value("target").toString();
    path = jsonObject.value("path").toString();
    num = jsonObject.value("num").toString();
    ref = jsonObject.value("ref").toString();
    timeout = jsonObject.value("timeout").toString();
    suffix = jsonObject.value("suffix").toString();
    exist = jsonObject.value("exist").toString();
    Propert = jsonObject.value("Propert").toString();
}
