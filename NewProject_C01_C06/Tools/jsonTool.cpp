#include "jsonTool.h"

jsonTool::jsonTool(QString path, QObject* parent) : QObject(parent)
{
    QString value;
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        value = file.readAll();
    }
    file.close();

    QJsonParseError error;
    QJsonDocument json = QJsonDocument::fromJson(value.toUtf8(), &error);

    if (error.error == QJsonParseError::NoError) {
        if (json.isArray()) {
            QJsonArray array = json.array();
            qDebug() << array;

            for (int i = 0; i < array.size(); i++) {

                QJsonValue value = array.at(i);

                QJsonObject obj = value.toObject();

                Items* item = new Items(obj, i, this);
                this->itemList.append(item);
            }

            this->generateHeadTitle(itemList);
        }

        if (json.isObject()) {
            QJsonObject obj = json.object();
            qDebug() << obj.value("name");

            QString string = obj.value("name").toString();
            qDebug() << string;
        }

    } else {
        qDebug() << error.errorString();
    }
}

QString jsonTool::getItemsFromJson(const QString& path, const QString& key)
{

    QString value;
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        value = file.readAll();
    }
    file.close();

    QJsonParseError* error = new QJsonParseError;
    QJsonDocument json = QJsonDocument::fromJson(value.toUtf8(), error);

    if (error->error == QJsonParseError::NoError) {

        if (json.isObject()) {
            QJsonObject obj = json.object();
            return obj.value(key).toString();
        } else {
            return error->errorString();
        }
    } else {
        return error->errorString();
    }
}

void jsonTool::getConfigFromJson(const QString& path)
{

    QString value;
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        value = file.readAll();
    }
    file.close();
    QJsonParseError* error = new QJsonParseError;
    QJsonDocument json = QJsonDocument::fromJson(value.toUtf8(), error);

    if (error->error == QJsonParseError::NoError) {

        if (json.isObject()) {
            QJsonObject obj = json.object();

            QJsonArray UnitArr = obj.value("Unit").toArray();
            QJsonArray CommonArr = obj.value("Comm").toArray();
            QJsonArray ChooseArr = obj.value("IsChoose").toArray();
            this->UnitCount = obj.value("UnitCount").toInt();

            qDebug() << "=========================devices=========================";
            for (int i = 0; i < this->UnitCount; i++) {
                for (int arrIndex = 0; arrIndex < UnitArr.count(); arrIndex++) {

                    QJsonObject subObj = UnitArr.at(arrIndex).toObject();
                    ConfigItem* temp = new ConfigItem(subObj, this);
                    qDebug() << temp->ref;
                    //                     temp->path = temp->path.replace("N",QString::number(i));
                    //                     qDebug()<< temp->path;
                    this->deviceList.append(temp);
                }
            }

            for (int arrIndex = 0; arrIndex < CommonArr.count(); arrIndex++) {
                QJsonObject subObj = CommonArr.at(arrIndex).toObject();
                ConfigItem* temp = new ConfigItem(subObj, this);
                this->deviceList.append(temp);
            }

            for (int arrIndex = 0; arrIndex < ChooseArr.count(); arrIndex++) {
                QString subValue = ChooseArr.at(arrIndex).toString();
                this->chooseList.append(subValue);
            }

            for (int i = 0; i < this->deviceList.count(); i++) {

                ConfigItem* temp = this->deviceList[i];
                qDebug() << "打印配置中的串口信息:" << temp->path + "\n";
            }
        }

    } else {

        qDebug() << error->errorString();
    }
}

void jsonTool::saveDataToJson(const QString& path, const QString& key, const QString& value)
{

    QString Content;
    QFile file(path);
    qDebug() << path;
    if (!file.open(QIODevice::ReadWrite)) {
        qDebug() << "File open error";
    } else {
        qDebug() << "File open!";

        Content = file.readAll();
    }

    file.resize(0);

    QJsonParseError error;
    QJsonDocument json = QJsonDocument::fromJson(Content.toUtf8(), &error);

    if (error.error == QJsonParseError::NoError) {

        if (json.isObject()) {
            QJsonObject obj = json.object();
            obj.insert(key, value);
            json.setObject(obj);
            file.write(json.toJson());
            file.close();
        }
    }
}

void jsonTool::generateHeadTitle(const QList<Items*>& tempList)
{

    QString titleStr = "SN,Result,Slot,StartTime,EndTime,FailMessage,";
    QString lowLimitStr = "lowLimitStr,,,,,,";
    QString uplowerLimitStr = "uplowerLimitStr,,,,,,";
    QString UnitStr = "UnitStr,,,,,,";

    for (int i = 0; i < tempList.count(); i++) {
        Items* tempItem = tempList[i];
        titleStr += tempItem->ItemName + ",";
        lowLimitStr += tempItem->lowerLimits + ",";
        uplowerLimitStr += tempItem->upperLimits + ",";
        UnitStr += tempItem->Unit + ",";
    }

    this->headTitle = titleStr + "\n" + lowLimitStr + "\n" + uplowerLimitStr + "\n" + UnitStr + "\n";

    qDebug() << "this is headTitle----->" << this->headTitle;
}
