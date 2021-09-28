#ifndef FILETOOL_H
#define FILETOOL_H

#include <QObject>
#include <basefilemanagertool.h>
#include <QDebug>
#include <QFile>
#include <QDir>

class FileTool : public BaseFileInterface
{
public:
    static QString readContentWithPath(const QString& path);

    static void writeContentWithPath(const QString& title, const QString& msg, const QString& path,
                                     const QString& suffix);  //接收返回信息
};

#endif  // FILETOOL_H
