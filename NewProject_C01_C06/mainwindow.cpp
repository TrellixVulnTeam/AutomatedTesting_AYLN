#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSplitter>
#include <QMessageBox>
#include <QSpacerItem>
#include <QHostAddress>
#include <thread>
#include <memory>
#include <iostream>
#include "LogHelper.h"
#include "ConfigParse.h"
#include "MtcpFileHelper.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_startLoadForm = new StartLoadForm();
    m_startLoadForm->show();

    updatePermissions();
    updateWindowTitle();
    ui->label_sw_Cfg->setText(QString::fromStdString(CFG_PARSE.getConfig()));

    dcInitializeSystem(NULL);
    dcInitializeModule(NULL);
    LOG_INFO("[Main] Initialize Matlab");

#ifdef LzgDebug
    CFG_PARSE.setTestInfo(KMtcp, false);
    std::string v = "V1.0.0_L";
    CFG_PARSE.setTestInfo(KVersion, v);
    LOG_INFO("[Main] Run debug mode on Zhigen computer!");

    std::vector<DeviceInfo> infos;
    for (int i = 0; i < 6; i++) {
        DeviceInfo info("", QString("COM%1").arg(i * 2 + 1).toStdString(), -1, "", -1, "", true, true);
        infos.push_back(info);
    }

    for (int i = 0; i < 3; i++) {
        DeviceInfo info("", "127.0.0.1", 8800 + i + 1, "", -1, "", true, true);
        infos.push_back(info);
    }

    CFG_PARSE.setUnitInfo(infos);

#else
    LOG_INFO("[Main] Run debug mode on other computer!");

    std::vector<DeviceInfo> infos;
    for (int i = 0; i < 6; i++) {
        if (i % 2 == 0) {
            DeviceInfo info("", QString("CB0%1%2").arg(CFG_PARSE.getOffset() - 1 + i / 2).arg(1).toStdString(), -1,
                            QString("TEST_Uart%1%2").arg(CFG_PARSE.getOffset() + i / 2).arg(1).toStdString(), -1, "",
                            true, true);
            infos.push_back(info);
        } else {
            DeviceInfo info("", QString("CB0%1%2").arg(CFG_PARSE.getOffset() - 1 + i / 2).arg(2).toStdString(), -1,
                            QString("TEST_Uart%1%2").arg(CFG_PARSE.getOffset() + i / 2).arg(2).toStdString(), -1, "",
                            true, true);
            infos.push_back(info);
        }
    }

    for (int i = 0; i < 3; i++) {
        DeviceInfo info("", "", -1, QString("TEST_Socket%1").arg(CFG_PARSE.getOffset() + i).toStdString(), -1, "", true,
                        true);
        infos.push_back(info);
    }

    CFG_PARSE.setUnitInfo(infos);
#endif

    totalNum = 0;

    m_testplan_tool =
        std::shared_ptr<TestPlanInfo>(new TestPlanInfo(QString::fromStdString(CFG_PARSE.getSpecCsvPath())));
    m_testplan_tool_flow =
        std::shared_ptr<TestPlanInfo>(new TestPlanInfo(QString::fromStdString(CFG_PARSE.getFlowCsvPath())));
    QString positionStr = FileTool::readContentWithPath(QString::fromStdString(CFG_PARSE.getPositionCsvPath()));
    m_positionDialog = new PositionDialog(positionStr, this);
    connect(m_positionDialog, &PositionDialog::positionDataChanged, this, &MainWindow::onPositionDataChanged);
    connect(ui->actionPosition, &QAction::triggered, this, &MainWindow::onPositionAction);

    systemTimer = new QTimer();
    connect(systemTimer, SIGNAL(timeout()), this, SLOT(updateSystemTimeUI()));
    systemTimer->start(1000);
    systemTimeLable = new QLabel(this);
    statusBar()->addPermanentWidget(systemTimeLable);

    ui->StationName_lable->setText(QString::fromStdString(CFG_PARSE.getProjectName()));
    QLabel* locationLable = new QLabel("    DXD - JCiT    ", this);
    locationLable->setMinimumSize(locationLable->sizeHint());
    statusBar()->addWidget(locationLable, 5);

    m_authorityLabel = new QLabel(tr("Authority: Invalid"), this);
    statusBar()->addWidget(m_authorityLabel, 4);

    statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));

    initTestMode();
    generateTable(CFG_PARSE.getUnitCount());
    generateUnits(CFG_PARSE.getUnitCount());

    connect(ui->StartButton, SIGNAL(clicked(bool)), this, SLOT(on_start()));
    connect(theSelection, SIGNAL(currentChanged(QModelIndex, QModelIndex)), this,
            SLOT(on_currentChanged(QModelIndex, QModelIndex)));
    connect(ui->actionLogin, &QAction::triggered, this, &MainWindow::onLoginAction);
    connect(ui->actionUserManager, &QAction::triggered, this, &MainWindow::onUserManagerAction);
    connect(ui->actionLoadProfile, &QAction::triggered, this, &MainWindow::onLoadProfileAction);
    connect(ui->actionPerference, &QAction::triggered, this, &MainWindow::onPreferencesAction);
    connect(ui->actionCommunicationTool, &QAction::triggered, this, &MainWindow::ononCommunicationToolAction);
    connect(this, &MainWindow::mtcpConnectedStatus, this, &MainWindow::onMtcpConnectedStatus);

    m_mtcp = CreateMtcpHandle("mtcp");
    onMtcpStatusChanged(CFG_PARSE.getMtcp());

    m_loginDialog = new LoginDialog(this);
    connect(m_loginDialog, &LoginDialog::loginSuccessSignal, this, &MainWindow::onloginSuccess);
    m_userManagerDialog = new UserManagerDialog(this);
    m_loginDialog->onUpdateUserSetting(m_userManagerDialog->getUserSetting());
    m_loginDialog->autoLogin();

    m_aboutDialog = new AboutDialog(this);
    connect(this, &MainWindow::clickLanguageActionSignal, m_aboutDialog, &AboutDialog::onClickLanguageAction);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAboutAction);

    m_loopDialog = new LoopTestDialog(this);
    connect(ui->actionLoopTest, &QAction::triggered, this, &MainWindow::onLoopTestAction);

    timer = new MyTimer();
    connect(timer, SIGNAL(signalUpdateTimeUI()), this, SLOT(updateTimeUI()));

    for (int i = 0; i < CFG_PARSE.getUnitCount(); i++) {
        TestAction* testaction = new TestAction(i, m_testplan_tool, m_testplan_tool_flow, positionStr);
        connect(testaction, &TestAction::updateLogViewUI, this, &MainWindow::onUpdateLogViewUI);
        connect(testaction, SIGNAL(flushUiWithRow(const Items*, int, int, int)), this,
                SLOT(flushMainUiTable(const Items*, int, int, int)));
        connect(testaction, &TestAction::stopTimer, this, &MainWindow::stopTimer);
        connect(this, SIGNAL(appExit()), testaction, SLOT(algoReleaseSystem()));
        connect(testaction, &TestAction::unitStart, this, &MainWindow::onUnitStart);
        connect(testaction, SIGNAL(updateUIInfo(QString, QString, QString, QString, QString&, QString&)), this,
                SLOT(onUpdateUIInfo(QString, QString, QString, QString, QString&, QString&)));
        connect(testaction, &TestAction::connectedDeviceSignal, unitUIList[i], &UnitsForm::onConnectedDevice);
        connect(unitUIList[i], &UnitsForm::originBtnClicked, testaction, &TestAction::onGoHome);
        connect(unitUIList[i], &UnitsForm::motorBtnClicked, testaction, &TestAction::onMotorBtnClicked);
        connect(m_loopDialog, &LoopTestDialog::loopStart, testaction, &TestAction::onLoopStart);
        connect(m_loopDialog, &LoopTestDialog::loopStop, testaction, &TestAction::onLoopStop);
        connect(testaction, &TestAction::unitStart, m_loopDialog, &LoopTestDialog::onTestStart);
        connect(testaction, &TestAction::stopTimer, m_loopDialog, &LoopTestDialog::onTestFinished);
        connect(testaction, &TestAction::stopLoopTestWhileError, m_loopDialog, &LoopTestDialog::stopLoopTestWhileError);
        connect(testaction, &TestAction::getLotName, this, &MainWindow::onGetLotName);
        connect(testaction, &TestAction::savePivotFile, this, &MainWindow::onSavePivotFile,
                Qt::BlockingQueuedConnection);
        testaction->connectDevice();
        testactionList.append(testaction);
    }
    m_startLoadForm->close();

    //    QTimer::singleShot(1000, this, [&]() { m_loginDialog->exec(); });
}

void MainWindow::reloadTestPlan()
{
    LOG_INFO("[Main] Reload new test plan");
    updateWindowTitle();

    m_testplan_tool =
        std::shared_ptr<TestPlanInfo>(new TestPlanInfo(QString::fromStdString(CFG_PARSE.getSpecCsvPath())));
    m_testplan_tool_flow =
        std::shared_ptr<TestPlanInfo>(new TestPlanInfo(QString::fromStdString(CFG_PARSE.getFlowCsvPath())));
    QString positionStr = FileTool::readContentWithPath(QString::fromStdString(CFG_PARSE.getPositionCsvPath()));

    for (int i = 0; i < CFG_PARSE.getUnitCount(); i++) {
        QTableView* view = tableViewList[i];
        QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(view->model());
        if (NULL != model) {
            testactionList.at(i)->reloadTestPlan(m_testplan_tool, m_testplan_tool_flow, positionStr);

            model->clear();

            model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("Index")));
            model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("TestName")));
            model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("LowerLimit")));
            model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("UpperLimit")));
            model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("Unit")));
            model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("value")));

            std::vector<Items*> tempItemList = m_testplan_tool->getTestPlanShowItems();
            for (int k = 0; k < tempItemList.size(); k++) {
                Items* tempItem = tempItemList[k];
                QStandardItem* name = new QStandardItem(tempItem->QSItemName->text());
                QStandardItem* lower = new QStandardItem(tempItem->QSLowerLimits->text());
                QStandardItem* upper = new QStandardItem(tempItem->QSUpperLimits->text());
                QStandardItem* unit = new QStandardItem(tempItem->QSUnit->text());

                model->setItem(k, 0, new QStandardItem(QString::number(k + 1)));
                model->setItem(k, 1, name);
                model->setItem(k, 2, lower);
                model->setItem(k, 3, upper);
                model->setItem(k, 4, unit);
                model->item(k, 0)->setTextAlignment(Qt::AlignCenter);
                model->item(k, 1)->setTextAlignment(Qt::AlignCenter);
                model->item(k, 2)->setTextAlignment(Qt::AlignCenter);
                model->item(k, 3)->setTextAlignment(Qt::AlignCenter);
                model->item(k, 4)->setTextAlignment(Qt::AlignCenter);
                model->item(k, 0)->setForeground(QBrush(QColor(0, 0, 0)));
                model->item(k, 1)->setForeground(QBrush(QColor(0, 0, 0)));
                model->item(k, 2)->setForeground(QBrush(QColor(0, 0, 0)));
                model->item(k, 3)->setForeground(QBrush(QColor(0, 0, 0)));
                model->item(k, 4)->setForeground(QBrush(QColor(0, 0, 0)));
            }
        }
    }
}

void MainWindow::onUpdateUIInfo(const QString& opID, const QString& lotName, const QString& productionMode,
                                const QString& siteID, QString& projectID, QString& config)
{
    ui->lineEdit_OperatorID->setText(opID);
    ui->lineEdit_LotName->setText(lotName);
    ui->label_ProjectID->setText(projectID);
    ui->comboBox_TestMode->setCurrentText(productionMode);
    ui->label_SiteID->setText(siteID);
    ui->label_sw_Cfg->setText(config);
}

void MainWindow::Delay_MSec(unsigned int msec)
{
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, SLOT(quit()));
    loop.exec();
}

void MainWindow::on_start()
{
    if (NULL == snDialog) {
        snDialog = new SNDialog(this, CFG_PARSE.getUnitCount());
        connect(snDialog, &SNDialog::startAll, this, &MainWindow::onStartAllTest);
        connect(snDialog, &SNDialog::singleStart, this, &MainWindow::onSingleStart);
    }
    snDialog->execDialog(unitUIList);
}

void MainWindow::onStartAllTest(const QList<QString>& list)
{
    flushClearTable();

    for (int i = 0; i < CFG_PARSE.getUnitCount(); i++) {
        UnitsForm* uForm = unitUIList.at(i);
        if (uForm->getEnable()) {
            TestAction* tempAction = testactionList[i];
            QString sn = list.at(i);
            if (sn.isEmpty()) {
                //                QMessageBox::warning(this, "Start Test Fail", "Can't start test with empty sn.");
                continue;
            }
            tempAction->itemSn = sn;
            tempAction->uiInfo = getUI_Info();
            logUnitUI* temp_unit = logUnitUIList[i];
            temp_unit->UnitSN->setText(tempAction->itemSn);
            temp_unit->UnitLogView->clear();

            uForm->scanSN(tempAction->itemSn);
            tempAction->start();
            totalNum++;
            uForm->onTestStart();
            snDialog->clearSN(i);
        }
    }

    timer->beginTimer(1000);
    snDialog->close();
}

void MainWindow::onSingleStart(qint16 slot, const QString& sn)
{
    flushClearTable(slot);
    UnitsForm* uForm = unitUIList.at(slot);
    if (uForm->getEnable()) {
        TestAction* tempAction = testactionList[slot];
        if (sn.isEmpty()) {
            QMessageBox::warning(this, "Start Test Fail", "Can't start test with empty sn.");
            return;
        }
        tempAction->itemSn = sn;
        tempAction->uiInfo = getUI_Info();
        logUnitUI* temp_unit = logUnitUIList[slot];
        temp_unit->UnitSN->setText(tempAction->itemSn);
        temp_unit->UnitLogView->clear();

        uForm->scanSN(tempAction->itemSn);
        tempAction->start();
        totalNum++;
        uForm->onTestStart();
        snDialog->clearSN(slot);
    }
    //    ui->timeLable->setText("0");
    timer->beginTimer(1000);

    snDialog->close();
}

void MainWindow::onUnitStart(qint16 slot, const QString& sn, const std::string& mtcpFilePath)
{

    flushClearTable(slot);
    qDebug() << getUI_Info();
    UnitsForm* uForm = unitUIList.at(slot);
    if (uForm->getEnable()) {
        TestAction* tempAction = testactionList[slot];
        if (sn.isEmpty()) {
            QMessageBox::warning(this, "Start Test Fail", "Can't start test with empty sn.");
            return;
        }
        tempAction->itemSn = sn;
        tempAction->uiInfo = getUI_Info();
        logUnitUI* temp_unit = logUnitUIList[slot];
        temp_unit->UnitSN->setText(tempAction->itemSn);
        temp_unit->UnitLogView->clear();
        uForm->scanSN(tempAction->itemSn);

        if (CFG_PARSE.getMtcp()) {
            if (NULL != m_mtcp && m_mtcp->isOpen()) {

                QString pivotRequest;
                m_mtcp->setTestCSVPath(mtcpFilePath + "/PivotRequest_Return.csv");

                try {
                    pivotRequest =
                        MtcpFileHelper::outputPivotRequest(sn, QString::fromStdString(CFG_PARSE.getLotName()),
                                                           QString::fromStdString(CFG_PARSE.getVersion()));
                    const char* ret = m_mtcp->SendGENL(pivotRequest.toStdString(), DynamicMtcp::DirectionType::REQUEST);
                    if (!QString::fromLocal8Bit(ret).isEmpty()) {
                        throw std::runtime_error("MTCP Fail.");
                    }
                }
                catch (...) {
                    try {
                        pivotRequest =
                            MtcpFileHelper::outputPivotRequest(sn, QString::fromStdString(CFG_PARSE.getLotName()),
                                                               QString::fromStdString(CFG_PARSE.getVersion()));
                        const char* ret =
                            m_mtcp->SendGENL(pivotRequest.toStdString(), DynamicMtcp::DirectionType::REQUEST);
                        if (!QString::fromLocal8Bit(ret).isEmpty()) {
                            QMessageBox::warning(this, tr("Set MTCP REQUEST Failed"), QString::fromLocal8Bit(ret));
                            uForm->onTestEnd(TEST_FAIL);
                            return;
                        }
                    }
                    catch (std::runtime_error& e) {
                        QMessageBox::warning(this, tr("Set MTCP Failed"), e.what());
                        uForm->onTestEnd(TEST_FAIL);
                        return;
                    }
                    catch (MTCP_ProtocolException& e) {
                        QMessageBox::warning(this, tr("Set MTCP Failed"),
                                             tr("errorCode: %1, errorStr: %2")
                                                 .arg(e.getErrCode())
                                                 .arg(QString::fromStdString(e.getErrStr())));
                        uForm->onTestEnd(TEST_FAIL);
                        return;
                    }
                }

                Util::MakeNDir(mtcpFilePath);
                QFile::copy(pivotRequest, QString::fromStdString(mtcpFilePath) + "/PivotRequest.csv");

            } else {
                QMessageBox::warning(this, tr("Connect MTCP Failed"), tr("MTCP REQUEST failed."));
                uForm->onTestEnd(TEST_FAIL);
                return;
            }
        }

        tempAction->start();
        totalNum++;
        uForm->onTestStart();
    }
    //    ui->timeLable->setText("0");
    timer->beginTimer(1000);
    uForm->clearSN();
}

void MainWindow::on_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{

    if (current.isValid()) {

        qDebug() << QString::number(current.row());
        qDebug() << QString::number(current.column());
        QString str = QString("Current cell:%1ROW ---------- %2CLO").arg(current.row()).arg(current.column());

        qDebug() << str;
        qDebug() << previous;
    }
}

void MainWindow::updateTimeUI()
{
    //    QString tempTime = ui->timeLable->text();
    //    int num = tempTime.toInt() + 1;
    //    ui->timeLable->setText(QString::number(num));
}

void MainWindow::updateSystemTimeUI()
{

    QDateTime curDateTime = QDateTime::currentDateTime();
    QString TimeText = "System Time:" + curDateTime.toString("yyyy-MM-dd hh:mm:ss");
    systemTimeLable->setText(TimeText);
}

int MainWindow::produceNum(int shang)
{
    qsrand(time(NULL));
    int n = qrand() % shang;

    return n;
}

void MainWindow::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
}

void MainWindow::generateUnits(const int& UnitCount)
{
    QWidget* widget = new QWidget(ui->stackedWidget);
    QGridLayout* gridLayout = new QGridLayout(widget);
    gridLayout->setContentsMargins(0, 0, 30, 0);
    gridLayout->setSpacing(0);

    ui->tabWidget->setTabEnabled(0, true);
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setStyleSheet("QTabBar::tab:disabled {width: 0; color: transparent;}");

    int rowCount = 3;
    for (int i = 0; i < UnitCount; i++) {

        UnitsForm* unitFrom = new UnitsForm(widget, i);
        connect(unitFrom, &UnitsForm::doubleMouseClick, [this](qint16 slot) {
            ui->tabWidget->setCurrentIndex(1);
            ui->tabStackedWidget->setCurrentIndex(slot);
            ui->tabWidget->setTabEnabled(0, false);
            ui->tabWidget->setTabEnabled(1, true);
            ui->tabWidget->setStyleSheet("QTabBar::tab:disabled {width: 0; color: transparent;}");
        });
        connect(unitFrom, &UnitsForm::singleStart, this, &MainWindow::onUnitStart);

        const QString unitName = "Unit-" + QString::number(i + CFG_PARSE.getOffset());
        unitFrom->setObjectName(unitName);
        gridLayout->addWidget(unitFrom, i / rowCount, i % rowCount + 1);

        std::string testSpecName = Util::GetFileName(CFG_PARSE.getSpecCsvPath());
        UNIT_INFO info(unitName, QString::fromStdString(testSpecName));
        unitFrom->setUnitInfo(info);
        unitUIList.append(unitFrom);
    }

    widget->setLayout(gridLayout);
    ui->stackedWidget->addWidget(widget);
}

void MainWindow::initTestMode()
{
    ui->comboBox_TestMode->addItem("MP");
    ui->comboBox_TestMode->addItem("GRR");
    ui->comboBox_TestMode->addItem("AUDIT");
    ui->comboBox_TestMode->addItem("OffLine");
    ui->comboBox_TestMode->setCurrentIndex(0);
}

void MainWindow::generateTable(const int& UnitCount)
{
    for (int i = 0; i < UnitCount; i++) {
        QSplitter* splitter = new QSplitter(ui->tabStackedWidget);
        splitter->setOrientation(Qt::Vertical);
        splitter->setHandleWidth(10);

        QTableView* view = new QTableView(splitter);

        QStandardItemModel* model = new QStandardItemModel(2, 6, this);
        theSelection = new QItemSelectionModel(model, this);

        view->setModel(model);
        view->horizontalHeader()->setStretchLastSection(true);
        view->setSelectionModel(theSelection);
        view->setSelectionMode(QAbstractItemView::ExtendedSelection);
        view->setSelectionBehavior(QAbstractItemView::SelectItems);
        view->verticalHeader()->hide();

        model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("Index")));
        model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("TestName")));
        model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("LowerLimit")));
        model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("UpperLimit")));
        model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("Unit")));
        model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("value")));

        std::vector<Items*> tempItemList = m_testplan_tool->getTestPlanShowItems();
        for (int k = 0; k < tempItemList.size(); k++) {
            Items* tempItem = tempItemList[k];
            QStandardItem* name = new QStandardItem(tempItem->QSItemName->text());
            QStandardItem* lower = new QStandardItem(tempItem->QSLowerLimits->text());
            QStandardItem* upper = new QStandardItem(tempItem->QSUpperLimits->text());
            QStandardItem* unit = new QStandardItem(tempItem->QSUnit->text());

            model->setItem(k, 0, new QStandardItem(QString::number(k + 1)));
            model->setItem(k, 1, name);
            model->setItem(k, 2, lower);
            model->setItem(k, 3, upper);
            model->setItem(k, 4, unit);
            model->item(k, 0)->setTextAlignment(Qt::AlignCenter);
            model->item(k, 1)->setTextAlignment(Qt::AlignCenter);
            model->item(k, 2)->setTextAlignment(Qt::AlignCenter);
            model->item(k, 3)->setTextAlignment(Qt::AlignCenter);
            model->item(k, 4)->setTextAlignment(Qt::AlignCenter);
            model->item(k, 0)->setForeground(QBrush(QColor(0, 0, 0)));
            model->item(k, 1)->setForeground(QBrush(QColor(0, 0, 0)));
            model->item(k, 2)->setForeground(QBrush(QColor(0, 0, 0)));
            model->item(k, 3)->setForeground(QBrush(QColor(0, 0, 0)));
            model->item(k, 4)->setForeground(QBrush(QColor(0, 0, 0)));
        }

        view->setSelectionBehavior(QAbstractItemView::SelectRows);
        view->setEditTriggers(QAbstractItemView::SelectedClicked);
        view->showGrid();
        view->setGridStyle(Qt::DashDotLine);
        view->setCornerButtonEnabled(true);
        tableViewList.append(view);

        logUnitUI* temp_unit = new logUnitUI(splitter);
        temp_unit->UnitSNNum->setText("SN" + QString::number(i + 1));
        logUnitUIList.append(temp_unit);

        splitter->addWidget(view);
        splitter->addWidget(temp_unit);
        splitter->setStretchFactor(0, 10);
        splitter->setStretchFactor(1, 1);
        ui->tabStackedWidget->addWidget(splitter);
    }
}

void MainWindow::onUpdateLogViewUI(const QString& logStr, int testUnit)
{
    logUnitUI* unitLogUi = logUnitUIList[testUnit];
    if (unitLogUi->UnitLogView->toPlainText().length() > 10000) {

        unitLogUi->UnitLogView->clear();
    }

    unitLogUi->UnitLogView->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") + " " + logStr);
}

void MainWindow::stopTimer(int slot, TEST_RESULT result)
{
    totalNum--;

    if (totalNum == 0) {
        timer->cancellTimer();
    }

    UnitsForm* uForm = unitUIList.at(slot);
    if (uForm->getEnable())
        uForm->onTestEnd(result);
}

void MainWindow::flushMainUiTable(const Items* tempItem, int row, int colum, int slot)
{
    UnitsForm* uForm = unitUIList.at(slot);
    if (!uForm->getEnable()) {
        return;
    }
    QTableView* tView = tableViewList.at(slot);
    QStandardItemModel* mode = dynamic_cast<QStandardItemModel*>(tView->model());
    if (mode == NULL)
        return;

    QStandardItem* temp = mode->takeItem(row, colum);
    temp->setText(tempItem->Value);
    mode->setItem(row, colum, temp);

    if (!tempItem->Result) {
        mode->item(row, colum)->setForeground(QBrush(QColor(255, 0, 0)));

    } else {
        mode->item(row, colum)->setForeground(QBrush(QColor(0, 0, 255)));
    }

    if (!tView->isHidden() && tView->isVisible()) {
        QModelIndex nowIndex = mode->index(row, colum, QModelIndex());
        tView->setCurrentIndex(nowIndex);
    }
}

void MainWindow::flushClearTable()
{
    for (int slot(0); slot < CFG_PARSE.getUnitCount(); slot++) {
        UnitsForm* uForm = unitUIList.at(slot);
        if (!uForm->getEnable()) {
            continue;
        }
        QTableView* tView = tableViewList.at(slot);
        QStandardItemModel* mode = dynamic_cast<QStandardItemModel*>(tView->model());
        if (mode == NULL)
            continue;

        for (int i = 0; i < m_testplan_tool->getShowList().size(); i++) {

            mode->setItem(i, 5, new QStandardItem(""));
            mode->item(i, 5)->setTextAlignment(Qt::AlignCenter);
            mode->item(i, 5)->setForeground(QBrush(QColor(0, 0, 0)));
        }
    }
}

void MainWindow::flushClearTable(qint16 slot)
{
    UnitsForm* uForm = unitUIList.at(slot);
    if (!uForm->getEnable()) {
        return;
    }
    QTableView* tView = tableViewList.at(slot);
    QStandardItemModel* mode = dynamic_cast<QStandardItemModel*>(tView->model());
    if (mode == NULL)
        return;

    for (int i = 0; i < m_testplan_tool->getShowList().size(); i++) {

        mode->setItem(i, 5, new QStandardItem(""));
        mode->item(i, 5)->setTextAlignment(Qt::AlignCenter);
        mode->item(i, 5)->setForeground(QBrush(QColor(0, 0, 0)));
    }
}

MainWindow::~MainWindow()
{
    delete m_startLoadForm;
    m_startLoadForm = NULL;

    for (int i = 0; i < CFG_PARSE.getUnitCount(); i++) {
        TestAction* testaction = testactionList.at(i);
        delete testaction;
        testaction = NULL;
    }

    dcReleaseModule();
    dcReleaseSystem();
    emit appExit();
    if (systemTimer->isActive()) {
        systemTimer->stop();
        delete systemTimer;
        systemTimer = NULL;
    }

    timer->cancellTimer();
    delete timer;
    timer = NULL;

    delete ui;
}

void MainWindow::on_leftBtn_clicked()
{
    if (ui->tabStackedWidget->currentIndex() == 0) {
        ui->tabStackedWidget->setCurrentIndex(ui->tabStackedWidget->count() - 1);
    } else {
        ui->tabStackedWidget->setCurrentIndex(ui->tabStackedWidget->currentIndex() - 1);
    }
}

void MainWindow::on_rightBtn_clicked()
{
    if (ui->tabStackedWidget->currentIndex() == ui->tabStackedWidget->count() - 1) {
        ui->tabStackedWidget->setCurrentIndex(0);
    } else {
        ui->tabStackedWidget->setCurrentIndex(ui->tabStackedWidget->currentIndex() + 1);
    }
}

void MainWindow::on_mainBtn_clicked()
{
    ui->tabWidget->setTabEnabled(0, true);
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setStyleSheet("QTabBar::tab:disabled {width: 0; color: transparent;}");
    ui->tabWidget->setCurrentIndex(0);
}

QString MainWindow::getUI_Info() const
{
    return QString("%1,%2,%3,%4,%5,%6,%7,%8")
        .arg(QString::fromStdString(CFG_PARSE.getVersion()))
        .arg(ui->label_SiteID->text())
        .arg(ui->label_ProjectID->text())
        .arg(ui->label_machineID->text())
        .arg(ui->label_sw_Cfg->text())
        .arg(ui->lineEdit_LotName->text())
        .arg(ui->lineEdit_OperatorID->text())
        .arg(ui->comboBox_TestMode->currentText());
}

void MainWindow::onLoginAction()
{
    if (ui->actionLogin->text() == tr("Login")) {
        if (NULL != m_loginDialog) {
            m_loginDialog->exec();
        }
    } else if (ui->actionLogin->text() == tr("Logout")) {
        if (NULL != m_loginDialog) {
            m_authorityLabel->setText(tr("Authority: Invalid"));
            CFG_PARSE.setAuthority(User::Invalid);
            updatePermissions();
            ui->actionLogin->setText(tr("Login"));
            LOG_INFO("[Main] Logout");
        }
    }
}

void MainWindow::onUserManagerAction()
{
    m_userManagerDialog->exec();
}

void MainWindow::onloginSuccess(const QString& userName, User::Authority authority)
{
    LOG_INFO("[Main] Login Authority: %s", userName.toStdString().c_str());
    CFG_PARSE.setAuthority(authority);
    updatePermissions();

    m_authorityLabel->setText(tr("Authority: %1").arg(userName));
    if (CFG_PARSE.getAuthority() != User::Invalid) {
        ui->actionLogin->setText(tr("Logout"));
    }
}

void MainWindow::onLoopTestAction()
{
    if (NULL != m_loopDialog)
        m_loopDialog->exec();
}

void MainWindow::onAboutAction()
{
    if (NULL != m_aboutDialog)
        m_aboutDialog->exec();
}

void MainWindow::ononCommunicationToolAction()
{
    if (NULL == m_commTool) {
        m_commTool = new QProcess(this);
        connect(m_commTool,
                static_cast<void (QProcess::*)(int exitCode, QProcess::ExitStatus exitStatus)>(&QProcess::finished),
                [&](int exitCode) {
                    const QString info =
                        QString("Process %1 exit with exit code: %2").arg(m_commTool->program()).arg(exitCode);
                    LOG_INFO(info.toStdString().c_str());
                });
    }
    Util::LaunchThirdProgram(m_commTool, "NetTools");
}

void MainWindow::onLoadProfileAction()
{
    if (NULL == m_loadProfileDialog) {
        m_loadProfileDialog = new LoadProfileDialog(this);
        connect(m_loadProfileDialog, &LoadProfileDialog::loadCSVFileSuccessSignal, this, &MainWindow::reloadTestPlan);
    }
    m_loadProfileDialog->exec();
}

void MainWindow::onPreferencesAction()
{
    if (NULL == m_preferencesDialog) {
        m_preferencesDialog = new PreferencesDialog(this);
        connect(m_preferencesDialog, &PreferencesDialog::mtcpStatusChanged, this, &MainWindow::onMtcpStatusChanged);
    }
    m_preferencesDialog->exec();
}

void MainWindow::onMtcpStatusChanged(bool status)
{
    if (status) {
        if (!m_mtcp->isOpen()) {
            if (CFG_PARSE.getMtcpIP().empty() || CFG_PARSE.getMtcpPort().empty()) {
                QMessageBox::warning(this, "Connect Mtcp Failed", "Can't connect empty Ip or empty port.");
                return;
            } else {
                int ret = m_mtcp->open(CFG_PARSE.getMtcpIP(), std::stoi(CFG_PARSE.getMtcpPort()), 2000);
                if (ret != 0) {
                    std::shared_ptr<std::thread> _thread = std::shared_ptr<std::thread>(new std::thread([&]() {
                        while (!isExit && !m_mtcp->isOpen() && CFG_PARSE.getMtcp()) {
                            emit mtcpConnectedStatus(1);
                            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                            emit mtcpConnectedStatus(2);
                            if (CFG_PARSE.getMtcp() && !CFG_PARSE.getMtcpIP().empty()
                                && !CFG_PARSE.getMtcpPort().empty())
                                m_mtcp->open(CFG_PARSE.getMtcpIP(), std::stoi(CFG_PARSE.getMtcpPort()), 1000);
                            else
                                break;
                        }
                        if (m_mtcp->isOpen()) {
                            m_mtcp->setLogPath(CFG_PARSE.getLogPath() + "MtcpLog/MTCP_Log");
                            emit mtcpConnectedStatus(0);
                        } else {
                            emit mtcpConnectedStatus(1);
                        }
                    }));
                    _thread->detach();

                    //                    QMessageBox::warning(this, "Connect MTCP Failed",
                    //                                         QString("ip: %1, port: %2")
                    //                                             .arg(QString::fromStdString(CFG_PARSE.getMtcpIP()))
                    //                                             .arg(QString::fromStdString(CFG_PARSE.getMtcpPort())));
                } else {
                    m_mtcp->setLogPath(CFG_PARSE.getLogPath() + "MtcpLog/MTCP_Log");
                    emit mtcpConnectedStatus(0);
                }
            }
        }
    } else {
        if (NULL != m_mtcp && m_mtcp->isOpen()) {
            m_mtcp->close();
        }
        emit mtcpConnectedStatus(1);
    }
}

void MainWindow::onGetLotName(const QString& lotName, int& ret)
{
    if (CFG_PARSE.getMtcp()) {
        if (!m_mtcp->isOpen()) {
            QMessageBox::warning(this, "Mtcp set lotName failed", "Mtcp was not connected.");
            return;
        }

        try {
            const QString lotEnd = MtcpFileHelper::outputLotEnd(QString::fromStdString(CFG_PARSE.getLotName()));
            const char* res = m_mtcp->SendGENL(lotEnd.toStdString(), DynamicMtcp::DirectionType::REPORT);
            std::string lotEndFileSavePath = CFG_PARSE.getLogPath() + "MtcpLog/" + CFG_PARSE.getLotName();
            Util::MakeNDir(lotEndFileSavePath);
            QFile::copy(lotEnd, QString::fromStdString(lotEndFileSavePath) + "/LotEnd.csv");

            const QString lotStart = MtcpFileHelper::outputLotStart(lotName);
            res = m_mtcp->SendGENL(lotStart.toStdString(), DynamicMtcp::DirectionType::REPORT);
            if (!QString::fromLocal8Bit(res).isEmpty()) {
                throw std::runtime_error(std::string(res));
            }
            std::string lotStartFileSavePath = CFG_PARSE.getLogPath() + "MtcpLog/" + lotName.toStdString();
            Util::MakeNDir(lotStartFileSavePath);
            QFile::copy(lotStart, QString::fromStdString(lotStartFileSavePath) + "/LotStart.csv");
            ret = 0;
        }
        catch (std::runtime_error& e) {
            QMessageBox::warning(this, tr("Start Lot Failed"), QString::fromStdString(e.what()));
        }
    } else {
        ret = 0;
    }
}

void MainWindow::onSavePivotFile(const QString& path, int& ret)
{
    if (CFG_PARSE.getMtcp()) {
        if (!m_mtcp->isOpen()) {
            QMessageBox::warning(this, "Mtcp send pivot failed", "Mtcp was not connected.");
            return;
        }

        std::string fl = Util::GetFilePath(path.toStdString());
        m_mtcp->setTestCSVPath(fl + "/PivotReport_Return.csv");
        try {
            const char* res = m_mtcp->SendGENL(path.toStdString(), DynamicMtcp::DirectionType::REPORT);
            if (!QString::fromLocal8Bit(res).isEmpty()) {
                throw std::runtime_error(std::string(res));
            }
            ret = 0;
        }
        catch (std::runtime_error& e) {
            QMessageBox::warning(this, tr("Send Pivot to Mtcp failed"), QString::fromStdString(e.what()));
        }
        catch (MTCP_ProtocolException& e) {
            QMessageBox::warning(this, tr("Send Pivot to Mtcp failed"), QString::fromStdString(e.getErrStr()));
        }
    } else {
        ret = 0;
    }
}

void MainWindow::onMtcpConnectedStatus(int status)
{
    if (status == 0) {
        setStyleSheet("#MainWindow{border:3px solid blue; border-radius: 3px;}");
    } else if (status == 1) {
        setStyleSheet("#MainWindow{border:3px solid red; border-radius: 3px;}");
    } else {
        setStyleSheet("#MainWindow{border:3px solid gray; border-radius: 3px;}");
    }
}

void MainWindow::updatePermissions()
{
    User::Authority authority = CFG_PARSE.getAuthority();
    switch (authority) {
    case User::Invalid:
        ui->lineEdit_OperatorID->setReadOnly(true);
        ui->lineEdit_LotName->setReadOnly(true);
        ui->label_sw_Cfg->setReadOnly(true);
        ui->comboBox_TestMode->setEnabled(false);
        ui->StartButton->setHidden(true);
        ui->actionUserManager->setEnabled(false);
        ui->actionPerference->setEnabled(false);
        ui->actionLoadProfile->setEnabled(false);
        ui->actionLoopTest->setEnabled(false);
        ui->actionPosition->setEnabled(false);
        ui->actionCommunicationTool->setEnabled(false);
        break;
    case User::Operator:
        ui->lineEdit_OperatorID->setReadOnly(true);
        ui->lineEdit_LotName->setReadOnly(true);
        ui->label_sw_Cfg->setReadOnly(true);
        ui->comboBox_TestMode->setEnabled(false);
        ui->StartButton->setHidden(true);
        ui->actionUserManager->setEnabled(false);
        ui->actionPerference->setEnabled(false);
        ui->actionLoadProfile->setEnabled(false);
        ui->actionLoopTest->setEnabled(false);
        ui->actionPosition->setEnabled(false);
        ui->actionCommunicationTool->setEnabled(false);
        break;
    case User::TestE:
        ui->lineEdit_OperatorID->setReadOnly(false);
        ui->lineEdit_LotName->setReadOnly(false);
        ui->label_sw_Cfg->setReadOnly(false);
        ui->comboBox_TestMode->setEnabled(true);
        ui->StartButton->setHidden(true);
        ui->actionUserManager->setEnabled(true);
        ui->actionPerference->setEnabled(true);
        ui->actionLoadProfile->setEnabled(true);
        ui->actionLoopTest->setEnabled(true);
        ui->actionPosition->setEnabled(true);
        ui->actionCommunicationTool->setEnabled(false);
        break;
    case User::Developer:
        ui->lineEdit_OperatorID->setReadOnly(false);
        ui->lineEdit_LotName->setReadOnly(false);
        ui->label_sw_Cfg->setReadOnly(false);
        ui->comboBox_TestMode->setEnabled(true);
        ui->StartButton->setHidden(false);
        ui->actionUserManager->setEnabled(true);
        ui->actionPerference->setEnabled(true);
        ui->actionLoadProfile->setEnabled(true);
        ui->actionLoopTest->setEnabled(true);
        ui->actionPosition->setEnabled(true);
        ui->actionCommunicationTool->setEnabled(true);
        break;
    default:
        break;
    }
}

void MainWindow::updateWindowTitle()
{
    std::string flowName = Util::GetFileNameWithOutExt(CFG_PARSE.getFlowCsvPath());
    std::string specName = Util::GetFileNameWithOutExt(CFG_PARSE.getSpecCsvPath());
    std::string positionName = Util::GetFileNameWithOutExt(CFG_PARSE.getPositionCsvPath());

    const QString titleStr = QString::fromStdString(CFG_PARSE.getVersion())
                             + "                                                              "
                               "                                                            "
                             + QString("%1&%2&%3")
                                   .arg(QString::fromStdString(flowName))
                                   .arg(QString::fromStdString(specName))
                                   .arg(QString::fromStdString(positionName));
    setWindowTitle(titleStr);
}

void MainWindow::onLanguageTriggered(bool isChineseBtn)
{
    if (isChineseBtn) {
    }
    QString languagePath = QApplication::applicationDirPath() + "/Languages/Chinese.qm";

    if (!isChineseBtn) {
        languagePath = QApplication::applicationDirPath() + "/Languages/English.qm";
    }
    QFile file(languagePath);
    if (file.exists()) {
        m.load(languagePath);
        qApp->installTranslator(&m);
        ui->retranslateUi(this);
        emit clickLanguageActionSignal(m);
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    isExit = true;
    m_mtcp->destroy();
    dcReleaseModule();
    dcReleaseSystem();
    QMainWindow::closeEvent(event);
}

void MainWindow::onPositionAction()
{
    if (NULL != m_positionDialog) {
        m_positionDialog->exec();
    }
}

void MainWindow::onPositionDataChanged(const QString& data)
{
    QString positionName = QString::fromStdString(Util::GetFileName(CFG_PARSE.getPositionCsvPath()));
    QString path = QString::fromStdString(CFG_PARSE.getPositionCsvPath());
    path = path.replace(positionName, "");

    LOG_INFO("[Main] Update %s%s", path.toStdString().c_str(), positionName.toStdString().c_str());
    FileTool::writeContentWithPath("", data, path, positionName, true);

    for (int i = 0; i < CFG_PARSE.getUnitCount(); i++) {
        testactionList.at(i)->reloadPosition(data);
    }
}
