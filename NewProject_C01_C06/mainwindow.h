#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QBrush>
#include <QWaitCondition>
#include <QTimer>
#include <QDebug>
#include <QItemSelectionModel>
#include <QProcess>
#include "items.h"
#include "mytimer.h"
#include <QThread>
#include <ctime>
#include <QTime>
#include <QDateTime>
#include <QDir>
#include <QTranslator>
#include <QMessageBox>
#include <serialporttool.h>
#include <tcpsockettool.h>
#include <testaction.h>
#include <logunitui.h>
#include <testplaninfo.h>
#include "UnitsForm.h"
#include "SNDialog.h"
#include <QTcpSocket>
#include "LoginDialog.h"
#include "UserManagerDialog.h"
#include "LoopTestDialog.h"
#include "AboutDialog.h"
#include "LoadProfileDialog.h"
#include "PreferencesDialog.h"
#include "MtcpProtocol.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

signals:
    void startTestSingle();
    void singleStartWithTcp(qint16 slot, const QString& sn);
    void tcpDisconnected();
    void appExit();
    void stopLoopTestWhileError(int slot);
    void clickLanguageActionSignal(QTranslator& tran);
    void mtcpConnectedStatus(int status);

protected:
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;
    virtual void closeEvent(QCloseEvent* event) override;

private slots:
    void on_start();
    void on_currentChanged(const QModelIndex& current, const QModelIndex& previous);
    void updateTimeUI();
    int produceNum(int shang);
    void updateSystemTimeUI();
    void flushMainUiTable(const Items* tempItem, int row, int soltNum, int slot);
    void flushClearTable();
    void flushClearTable(qint16 slot);
    void stopTimer(int slot, TEST_RESULT result);

    void on_leftBtn_clicked();
    void on_rightBtn_clicked();
    void on_mainBtn_clicked();
    void onStartAllTest(const QList<QString>& list);
    void onSingleStart(qint16 slot, const QString& sn);
    void onUnitStart(qint16 slot, const QString& sn, const std::string& mtcpFilePath);

    void onUpdateUIInfo(const QString& opID, const QString& lotName, const QString& productionMode,
                        const QString& siteID, QString& projectID);
    void onLoginAction();
    void onLoopTestAction();
    void onLoadProfileAction();
    void onPreferencesAction();
    void ononCommunicationToolAction();
    void onAboutAction();
    void onUserManagerAction();
    void onloginSuccess(const QString& userName, User::Authority authority);
    void onUpdateLogViewUI(const QString& logStr, int testUnit);
    void onLanguageTriggered(bool isChineseBtn);
    void onMtcpStatusChanged(bool status);
    void onMtcpConnectedStatus(int status);
    void onGetLotName(const QString& lotName, int& ret);
    void onSavePivotFile(const QString& path, int& ret);

private:
    void generateUnits(const int& UnitCount);
    void generateTable(const int& UnitCount);
    void initTestMode();
    void reloadTestPlan();

    void tcpClientSendData(const QString& msg);
    void Delay_MSec(unsigned int msec);
    void updatePermissions();
    void updateWindowTitle();
    QString getUI_Info() const;

private:
    Ui::MainWindow* ui;

    AboutDialog* m_aboutDialog = NULL;
    UserManagerDialog* m_userManagerDialog = NULL;
    LoginDialog* m_loginDialog = NULL;
    LoopTestDialog* m_loopDialog = NULL;
    QLabel* m_authorityLabel = NULL;
    SNDialog* snDialog = NULL;
    MyTimer* timer = NULL;
    QTimer* systemTimer = NULL;
    QLabel* systemTimeLable = NULL;
    QProcess* m_commTool = NULL;
    LoadProfileDialog* m_loadProfileDialog = NULL;
    PreferencesDialog* m_preferencesDialog = NULL;
    DynamicMtcp* m_mtcp = NULL;

    QItemSelectionModel* theSelection;
    QList<Items*> testItemList;
    QList<TestAction*> testactionList;
    QList<logUnitUI*> logUnitUIList;
    QList<UnitsForm*> unitUIList;
    QList<QTableView*> tableViewList;

    int totalNum;
    int tempRowIndex;
    int testModeIndex = 0;
    bool ishandlerConnected = false;
    bool isExit = false;

    QString itemSn;

    std::shared_ptr<TestPlanInfo> m_testplan_tool = nullptr;
    std::shared_ptr<TestPlanInfo> m_testplan_tool_flow = nullptr;

    QTranslator m;
};

#endif  // MAINWINDOW_H
