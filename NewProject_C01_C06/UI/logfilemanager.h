#ifndef LOGFILEMANAGER_H
#define LOGFILEMANAGER_H

#include <QObject>
#include <QDebug>

class logFileManager: public QObject
{
public:
    static logFileManager& instance()
    {
        static logFileManager qinstance;
        return qinstance;
    }

    void print();
private:
    logFileManager(){}
    logFileManager(const logFileManager&){}
    logFileManager& operator==(const logFileManager&){}
};


#endif // LOGFILEMANAGER_H
