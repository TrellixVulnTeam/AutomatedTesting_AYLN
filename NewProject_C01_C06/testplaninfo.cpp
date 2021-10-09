#include "testplaninfo.h"
#include "LogHelper.h"
#include "ConfigParse.h"

TestPlanInfo::TestPlanInfo(const QString& path, QObject* parent) : QObject(parent)
{
    QString value;
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        value = file.readAll();
    }
    file.close();

    LOG_INFO("");
    LOG_INFO("[Main]Get items from path: %s", path.toStdString().c_str());
    QList<QString> contentList = value.split("\r\n");
    for (int i = 1; i < contentList.count() - 1; i++) {

        QList<QString> subcontentList = contentList[i].split(",");
        Items* tempItem = new Items(subcontentList, i, this);
        LOG_INFO("[Main] %s %s %s %s %s %s %s", tempItem->Index.toStdString().c_str(),
                 tempItem->Group.toStdString().c_str(), tempItem->ItemName.toStdString().c_str(),
                 tempItem->ClassType.toStdString().c_str(), tempItem->Ref.toStdString().c_str(),
                 tempItem->Function.toStdString().c_str(), tempItem->Param.toStdString().c_str());

        if (tempItem->Status == "1") {
            if (path.contains("Spec")) {
                if (tempItem->Show != "1") {
                    continue;
                } else {
                    m_showList.push_back(tempItem);
                }
            }

            std::map<QString, std::vector<Items*>>::iterator iter = m_testPlanItemMap.find(tempItem->Group);
            if (iter != m_testPlanItemMap.end()) {
                (*iter).second.push_back(tempItem);
            } else {
                std::vector<Items*> itemVec = { tempItem };
                m_testPlanItemMap.insert(std::pair<QString, std::vector<Items*>>(tempItem->Group, itemVec));
            }
        }
    }
    if (path.contains("Spec"))
        generateHeadTitle(m_showList);

    LOG_INFO("");
}

void TestPlanInfo::generateHeadTitle(const std::vector<Items*>& tempList)
{

    QString titleStr =
        "SN,Result,Slot,StartTime,EndTime,SW_ver,SiteID,ProjectID,MachineID,LotName,OperatorID,TestMode,FailMessage,";
    QString lowLimitStr = "lowLimitStr,,,,,,,,,,,,,";
    QString uplowerLimitStr = "uplowerLimitStr,,,,,,,,,,,,,";
    QString UnitStr = "UnitStr,,,,,,,,,,,,,";

    for (int i = 0; i < tempList.size(); i++) {
        Items* tempItem = tempList[i];
        titleStr += tempItem->ItemName + ",";
        lowLimitStr += tempItem->lowerLimits + ",";
        uplowerLimitStr += tempItem->upperLimits + ",";
        UnitStr += tempItem->Unit + ",";
    }

    m_headTitle = titleStr + "\n" + lowLimitStr + "\n" + uplowerLimitStr + "\n" + UnitStr + "";
}

std::vector<Items*> TestPlanInfo::getTestPlanShowItems()
{
    std::vector<Items*> items;
    for (int i = 0; i < CFG_PARSE.getGroupOrderVec().size(); i++) {
        QString groupName = QString::fromStdString(CFG_PARSE.getGroupOrderItem(i));
        if (m_testPlanItemMap.find(groupName) != m_testPlanItemMap.end()) {
            foreach (Items* it, m_testPlanItemMap[groupName]) {
                items.push_back(it);
            }
        }
    }
    return items;
}
