#ifndef TESTACTION_H
#define TESTACTION_H

#include <QDebug>
#include <QThread>
#include <items.h>
#include <configdevice.h>
#include <QMetaObject>
#include <QProcess>
#include <QMutex>
#include <filetool.h>
#include <string.h>
#include <testplaninfo.h>
#include <mutex>
#include "algosingleton.h"
#include <QHostAddress>
#include <thread>
#include <QTcpSocket>
using namespace std;
/*****************
TEST GROUP:
1.OFFSET
2.NOISE
3.C01
4.C02
5.C03
6.C04
7.C05
8.C06
******************/

typedef void (*SetUnitLogCallback)(const QString& logStr, int testUnit, void* context);

enum TEST_RESULT
{
    TEST_PASS = 0,
    TEST_FAIL,
    TEST_ERROR
};
Q_DECLARE_METATYPE(TEST_RESULT);

class SingletonMutex
{
public:
    ~SingletonMutex()
    {
        qDebug() << "destructor called!";
    }
    static SingletonMutex& getInstance()
    {
        static SingletonMutex instance;
        return instance;
    }

public:
    void lock()
    {
        mtx.lock();
    }

    void unlock()
    {
        mtx.unlock();
    }

private:
    SingletonMutex()
    {
        qDebug() << "Create SingletonMutex";
    }

    std::mutex mtx;
    Q_DISABLE_COPY(SingletonMutex)
};

class TestAction : public QThread
{
    Q_OBJECT
public:
    explicit TestAction(const int& num, const std::shared_ptr<TestPlanInfo>& tempTestplaninfo,
                        const std::shared_ptr<TestPlanInfo>& tempFlowinfo, const QString& positionStr);
    ~TestAction();

    void connectDevice();
    void reloadTestPlan(const std::shared_ptr<TestPlanInfo>& tempTestplaninfo,
                        const std::shared_ptr<TestPlanInfo>& tempFlowinfo, const QString& positionStr);

    static void onDealWithSocketRecv(const QByteArray& recv, Items* tempItem, void* context);

public:
    QString itemSn;
    QString uiInfo;

protected:
    void run();

signals:
    void updateLogViewUI(const QString& logStr, int testUnit);
    void connectedDeviceSignal(const std::vector<std::string>& names);
    void flushUiWithRow(const Items* tempItem, int row, int colum, int slot);
    void stopTimer(int slot, TEST_RESULT result);
    void unitStart(qint16 slot, const QString& sn, const std::string& mtcpFilePath);
    void tcpDisconnected();
    void sendDataToServer(const QString& msg);
    void updateUIInfo(const QString& opID, const QString& lotName, const QString& productionMode, const QString& siteID,
                      QString& projectID, QString& config);
    void showWarning(const QString& msg);
    void showInfo(const QString& msg);
    void stopLoopTestWhileError(int slot);
    void connectDevices(std::map<std::string, DeviceInfo>& devicMap, const QString& soltNum);
    void getLotName(const QString& lotName, int& ret);
    void savePivotFile(const QString& path, int& ret);
    void startProcess(const QString& cmd);

public slots:
    void algoReleaseSystem();
    void tcpConnect();
    void tcpClientSendData(const QString& msg);
    void onGoHome();
    void onLoopStart(int _slot);
    void onLoopStop(int _slot);
    void onShowWarning(const QString& msg);
    void onShowInfo(const QString& msg);
    void onStartProcess(const QString& cmd);
	void onTakePhotos();

private:
    bool openCylinder();
    bool closeCylinder();

    void initTcpSocketForHandler();
    void killAllDevice(ConfigDevice*);  // Kill device
    void judgmentWithoutMTCP(Items* tmpItem, QString& failMsg);
    void writeContentWithPath(const QString& filePath, const QString& fileName, const QString& content);
    void calPlusCntToHex(const QString& plusStr, QString& direction, QString& hex1, QString& hex2, QString& hex3,
                         QString& hex4);
    void saveRawdataToLocalFile(const QString& filePath, const QString& fileName, const QString& rawDataPath,
                                const QString& content, QString& groupName, QString& otherLog);
    void saveUnitCSVData(const QString& itemSn, const QString& result, const QString& slot, const QString& failMsg,
                         const QString& uiInfo, const QString& itemContents);

    void MLOG_INFO(QString msg);
    void MLOG_FATAL(QString msg);
    void MLOG_ERROR(QString msg);
    void MLOG_WARN(QString msg);
    void MLOG_DEBUG(QString msg);

    void createrXpOrXsCmd(QString str, QString& cmd);
    void dealYParallelZCmd(QString str, Items* item, QObject* devObj1, QObject* devObj2, QString& repStr,
                           QString& logStr);
    void dealMoveCmd(QString str, Items* item, QObject* devObj1, QObject* devObj2, QString& repStr, QString& logStr);

    QString saveUnitTxtLog(const Items*);
    QString dealWithResponseStr(const QString& responseStr);
    void getItemsFromLocalFile(const QString& filePath, const QString& fileName, std::vector<Items*>& showItemList);

    QString convertCmd(QString axis, QString value);
    double getComp_Factor(QString groupName);

private:
    TEST_RESULT testResult;
    QMutex* mutex;
    AlgoSingleTon* palgo = NULL;
    QTcpSocket* tcpClient = NULL;
    ConfigDevice* m_devices = NULL;
    std::shared_ptr<QProcess> m_process = nullptr;

    int offsetNum;
    int unitNum;
    int plus_beta;
    int plus_gamma;
    int plus_Xp;
    int plus_Y;
    int plus_Z;

    double rms = 0;

    bool isDatabad = false;
    bool isHexData = true;
    bool isReady = true;
    bool ishandlerConnected = false;
    bool m_loopStopFlag = false;
    bool isReseting = false;
    bool m_isPosition_O = false;
    bool isFlowEnd = false;

    QString slotID;
    QString titleStr;
    QString startTime;
    QString endTime;
    QString tcpRecvStr = "";
    QString errorMessage_H = "";
    QString csvInfo_H = "";
    QString failMessage_H = "";
    QString errorCode_H = "0";
    QString result_R = "1";
    QString csvInfo_R = "";
    QString failItems_R = "";
    QString testItems_R = "";
    QString m_logPath;
    QString deviceLogPath;
    QString outputDataTime;
    QString rawDataPath;
    QString convertDataPath;
    QString outputDataPath;
    QString lotFilePath;
    QString rawDataFilePath;
    QString rawDataFileName;
    QString outputPath;

    // StartTest info
    QString sn_ST = "";
    QString opID_ST = "";
    QString lotName_ST = "";
    QString productionMode_ST = "";
    QString siteID_ST = "";
    QString projectID_ST = "";
    QString trial_ST = "";
    QString user_ST = "";
    QString mesState_ST = "";
    QString mtcpState_ST = "";
    QString machineID_ST = "";
    QString config_ST = "";

    QString fixtureID = "";

    QList<QString> timeStepList;
    QList<Items*> flowItemList;
    QList<QString> list_Xp;
    QList<QString> list_Y;
    QList<QString> list_Z;
    QList<QString> list_Beta;
    QList<QString> list_Gamma;
    QList<QString> list_cmpFactor;
    std::string m_mtcpFilePath;

    std::shared_ptr<TestPlanInfo> m_testFlowTool = nullptr;
    std::shared_ptr<TestPlanInfo> m_testSpecTool = nullptr;

    bool m_isSendOnly = false;
    QByteArray m_sendOnlyRecvByteArr;
    QThread* device_th = NULL;

    QString beta_Offset = "";
    QString Xs_Offset = "";
    QString Z_Offset = "";


};

#endif  // TESTACTION_H
