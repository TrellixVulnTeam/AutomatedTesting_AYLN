#ifndef BASEFILEMANAGERTOOL_H
#define BASEFILEMANAGERTOOL_H

#include <QString>

class BaseFileInterface
{

public:

    virtual QString readContentWithPath(const QString &path)=0;

    virtual void writeContentWithPath(const QString &title,const QString &msg,const QString &path, const QString &suffix)=0;   //接收返回信息
};

#endif // BASEFILEMANAGERTOOL_H
