#ifndef TESTPLANINFO_H
#define TESTPLANINFO_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include "items.h"
#include <configitem.h>

#include <QObject>

class TestPlanInfo : public QObject
{
    Q_OBJECT

public:
    explicit TestPlanInfo(const QString& path, QObject* parent = nullptr);
    std::vector<Items*> getTestPlanShowItems();

    std::vector<Items*> getShowList() const
    {
        return m_showList;
    }

    QString getHeadTitle() const
    {
        return m_headTitle;
    }

    std::map<QString, std::vector<Items*>> getTestPlanItemMap() const
    {
        return m_testPlanItemMap;
    }

private:
    void generateHeadTitle(const std::vector<Items*>& tempList);

private:
    std::vector<Items*> m_showList;
    QString m_headTitle;
    std::map<QString, std::vector<Items*>> m_testPlanItemMap;
};

#endif  // TESTPLANINFO_H
