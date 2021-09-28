#include "logfilemanager.h"


////验证单例
//logFileManager * fileLog1 = &logFileManager::instance();
//fileLog1->print();


//logFileManager * fileLog2 = &logFileManager::instance();
//fileLog2->print();

void logFileManager::print(){

     qDebug()<<"logFileManager 打印输出信息";
     qDebug()<<this;
}
