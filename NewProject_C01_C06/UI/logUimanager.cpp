#include "logUimanager.h"

LogUiManager::LogUiManager(QObject *parent) : QObject(parent)
{

}


void LogUiManager::updateLogView(Items * testItem)
{
    QString LogStr =  "\n测试项序号:" + testItem->Index+
                      "\n测试项名称:" + testItem->ItemName +
                      "\n测试项下限:" + testItem->lowerLimits+
                      "\n测试项上限:" + testItem->upperLimits +
                      "\n测试项单位:" + testItem->Unit +
                      "\n前缀字符串:" + testItem->StartStr+
                      "\n后缀字符串:" + testItem->EndStr+
                      "\n执行的设备:" + testItem->Target+
                      "\n发送的命令:" + testItem->Param+
                      "\n返回的数据:" + testItem->Response+
                      "\n处理的数据:" + testItem->Value+
                      "\n处理的结果:" + testItem->Result+
                      "\n开始的时间:" + testItem->StartTime+
                      "\n结束的时间:" + testItem->StopTime+
                      "\n\n***************************\n\n";

   emit updateLogViewSignals(LogStr);
}
