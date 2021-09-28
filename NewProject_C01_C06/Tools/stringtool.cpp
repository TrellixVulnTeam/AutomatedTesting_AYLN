#include "stringtool.h"

StringTool::StringTool(QObject* parent) : QObject(parent) {}

QString StringTool::getMiddleStr(const QString& resource, const QString& startStr, QString endStr)
{

    if (endStr.contains("\\r\\n")) {
        endStr = endStr.replace("\\r\\n", "\r\n");
    }

    if (resource.indexOf(startStr) == -1) {

        qDebug() << "不包含startStr-->:" << startStr;
    }

    if (resource.indexOf(endStr) == -1) {

        qDebug() << "不包含endStr-->:" << endStr;
    }

    if (resource.indexOf(startStr) == -1 || resource.indexOf(endStr) == -1) {

        qDebug() << "resource:" << resource;
        return resource;
    }

    //找到第一个出现的位置
    int x = resource.indexOf(startStr) + startStr.length();

    //截取start后面的字符串
    QString tempStr = resource.mid(x);

    qDebug() << "截取start后面的字符串:" << tempStr;

    qDebug() << "打印endStr:" << endStr;

    //截取左边的字符串
    tempStr = tempStr.left(tempStr.length() - endStr.length());
    qDebug() << "截取左边的字符串:" << tempStr;

    return tempStr;
}

QString StringTool::calcultate(const QString& calcultateStr)
{

    QScriptValue scripVal = scriptEngin.evaluate(calcultateStr).toString();
    return scripVal.toString();
}

QString StringTool::produceNum(int shang)
{

    qsrand(time(NULL));
    int n = qrand() % shang;

    return QString::number(n);
}
