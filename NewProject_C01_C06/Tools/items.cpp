#include "items.h"
#include "Util.h"

Items::Items(QObject* parent) : QObject(parent) {}

Items::Items(QJsonObject& jsonObject, int& index, QObject* parent) : QObject(parent)
{

    Status = jsonObject.value("Status").toString();
    Show = jsonObject.value("Show").toString();
    Group = jsonObject.value("Group").toString();
    ItemName = jsonObject.value("ItemName").toString();
    Snyc = jsonObject.value("Snyc").toString();
    ClassType = jsonObject.value("ClassType").toString();
    Target = jsonObject.value("Target").toString();
    Ref = jsonObject.value("Ref").toString();
    Param = jsonObject.value("Param").toString();
    Function = convertParam(jsonObject.value("Function").toString());
    Suffix = jsonObject.value("Suffix").toString();
    Param = jsonObject.value("Param").toString();
    LogicFunction = jsonObject.value("LogicFunction").toString();
    StartStr = jsonObject.value("StartStr").toString();
    EndStr = jsonObject.value("EndStr").toString();
    Timeout = jsonObject.value("Timeout").toString();
    Retry = jsonObject.value("Retry").toString();
    isBreak = jsonObject.value("isBreak").toString();
    SaveGlobal = jsonObject.value("SaveGlobal").toString();
    Global = jsonObject.value("Global").toString();
    lowerLimits = jsonObject.value("lowerLimits").toString();
    upperLimits = jsonObject.value("upperLimits").toString();
    Unit = jsonObject.value("Unit").toString();
    Relation = jsonObject.value("Relation").toString();
    Value = "";
    Result = true;
    Response = "";
    Index = QString::number(index);

    QSIndex = new QStandardItem(Index);
    QSItemName = new QStandardItem(jsonObject.value("ItemName").toString());
    QSLowerLimits = new QStandardItem(jsonObject.value("lowerLimits").toString());
    QSUpperLimits = new QStandardItem(jsonObject.value("upperLimits").toString());
    QSUnit = new QStandardItem(jsonObject.value("Unit").toString());
    QSValue = new QStandardItem("");
}

//   Status,SHOW,GROUP,ItemName,Plugin,ClassType,Target,Ref,
//   Function,Suffix,Param,LogicFunction,StartStr,EndStr,Timeout,
//   Retry,isBreak,SaveGlobal,GLOBAL,LowerLimits,UpperLimits,Unit,Relation
Items::Items(const QList<QString>& contentList, const int IndexNum, QObject* parent) : QObject(parent)
{
    int num = 0;
    Status = contentList[num++];
    Show = contentList[num++];
    Group = contentList[num++];
    ItemName = contentList[num++];
    Snyc = contentList[num++];
    ClassType = contentList[num++];
    Target = contentList[num++];
    Ref = contentList[num++];
    Function = contentList[num++];
    Suffix = contentList[num++];
    Param = convertParam(contentList[num++]);
    LogicFunction = contentList[num++];
    StartStr = contentList[num++];
    EndStr = contentList[num++];
    Timeout = contentList[num++];
    Retry = contentList[num++];
    isBreak = contentList[num++];
    SaveGlobal = contentList[num++];
    Global = contentList[num++];
    lowerLimits = contentList[num++];
    upperLimits = contentList[num++];
    Unit = contentList[num++];
    Relation = contentList[num++];
    Value = "";
    Result = true;
    Response = "";
    Index = QString::number(IndexNum);
    others = "";

    QSIndex = new QStandardItem(Index);
    QSItemName = new QStandardItem(ItemName);
    QSLowerLimits = new QStandardItem(lowerLimits);
    QSUpperLimits = new QStandardItem(upperLimits);
    QSUnit = new QStandardItem(Unit);
    QSValue = new QStandardItem("");

    //   description();
}

QString Items::convertParam(const QString& oldStr)
{
    std::string s = oldStr.toStdString();
    Util::Replace(s, ";", ",", 1);
    return QString::fromStdString(s);
}

void Items::description()
{

    QString LogStr = "\n测试项序号:" + Index + "\n测试项名称:" + ItemName + "\n测试项下限:" + lowerLimits
                     + "\n测试项上限:" + upperLimits + "\n测试项单位:" + Unit + "\n前缀字符串:" + StartStr
                     + "\n后缀字符串:" + EndStr + "\n执行的设备:" + Target + "\n发送的命令:" + Param + "\n返回的数据:"
                     + Response + "\n处理的数据:" + Value + "\n处理的结果:" + (Result ? "PASS" : "FALSE")
                     + "\n开始的时间:" + StartTime + "\n结束的时间:" + StopTime + "\n\n***************************\n\n";
    qDebug() << "LogStr--->" << LogStr;
}

Items::~Items() {}
