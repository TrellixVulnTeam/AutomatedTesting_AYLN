#include "testaction.h"
#include <QApplication>
#include <QtMath>
#include <QMessageBox>
#include "ConfigParse.h"
#include "LogHelper.h"
#include "Util.h"

TestAction::TestAction(const int& num, const std::shared_ptr<TestPlanInfo>& tempTestplaninfo,
                       const std::shared_ptr<TestPlanInfo>& tempFlowinfo, const QString& positionStr)
{
    offsetNum = 4;
    unitNum = num;
    m_testSpecTool = tempTestplaninfo;
    m_testFlowTool = tempFlowinfo;

    qRegisterMetaType<TEST_RESULT>("TEST_RESULT");
    MLOG_INFO("Creat a new TestAction");

    isReady = true;
    titleStr = tempTestplaninfo->getHeadTitle();

    if (CFG_PARSE.getSlot().find("1") != std::string::npos) {
        slotID = QString("%1").arg(unitNum + 3);
    } else {
        slotID = QString("%1").arg(unitNum + 4);
    }

    palgo = AlgoSingleTon::getInstance();
    initTcpSocketForHandler();

    QList<QString> list = positionStr.split("\n");
    list_Xp = list[1 + 5 * unitNum].split(",");
    list_Y = list[2 + 5 * unitNum].split(",");
    list_Z = list[3 + 5 * unitNum].split(",");
    list_Beta = list[4 + 5 * unitNum].split(",");
    list_Gamma = list[5 + 5 * unitNum].split(",");

    connect(this, &TestAction::showWarning, this, &TestAction::onShowWarning);
    connect(this, &TestAction::showInfo, this, &TestAction::onShowInfo);
}

void TestAction::reloadTestPlan(const std::shared_ptr<TestPlanInfo>& tempTestplaninfo,
                                const std::shared_ptr<TestPlanInfo>& tempFlowinfo, const QString& positionStr)
{
    m_testSpecTool = tempTestplaninfo;
    m_testFlowTool = tempFlowinfo;
    titleStr = tempTestplaninfo->getHeadTitle();

    QList<QString> list = positionStr.split("\n");
    list_Xp = list[1 + 5 * unitNum].split(",");
    list_Y = list[2 + 5 * unitNum].split(",");
    list_Z = list[3 + 5 * unitNum].split(",");
    list_Beta = list[4 + 5 * unitNum].split(",");
    list_Gamma = list[5 + 5 * unitNum].split(",");
}

void TestAction::connectDevice()
{
    m_devices = new ConfigDevice();
    device_th = new QThread();
    m_devices->moveToThread(device_th);
    connect(device_th, &QThread::finished, device_th, &QThread::deleteLater);
    device_th->start();

    connect(m_devices, &ConfigDevice::errorHappend, [&](const QString& msg) {
        m_loopStopFlag = true;
        emit showWarning(msg);
    });
    connect(this, &TestAction::connectDevices, m_devices, &ConfigDevice::onConnectDevices,
            Qt::BlockingQueuedConnection);
    emit connectDevices(CFG_PARSE.getUnitDevicesMap(), QString::number(unitNum + offsetNum));
    m_devices->setTcpCallBack(onDealWithSocketRecv, this);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::vector<std::string> names = m_devices->getConnectFailedDevices();
    if (!ishandlerConnected) {
        names.push_back("MainBoard");
    }
    emit connectedDeviceSignal(names);
}

void TestAction::run()
{
    MLOG_INFO("\n\n\n\nStart New run thread.");
    isReady = false;
    m_loopStopFlag = false;
    testResult = TEST_FAIL;
    QObject* deviceObject;
    QObject* deviceObject_Uart1;
    QObject* deviceObject_Uart2;

    QByteArray response;
    QString response2;

    QString content;

    // MTCP File
    QString pivotStr = "index,group,groupinfo,item,starttime,endtime,testtime,low,high,unit,value,result\n";
    QString failMessge = "";
    int flushRow = 0;

    QString testResultStr = "PASS";
    plus_beta = 0;
    plus_gamma = 0;
    plus_Xp = 0;
    plus_Y = 0;
    plus_Z = 0;
    m_logPath = QString::fromStdString(CFG_PARSE.getLogPath())
                + QString("Unit%1/%2/%3/")
                      .arg(unitNum + offsetNum)
                      .arg(itemSn)
                      .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    MLOG_INFO("Log path: " + m_logPath);

    deviceLogPath = m_logPath + "DeviceLog/";
    rawDataPath = m_logPath + "RawData/" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + "/";
    convertDataPath = m_logPath + "ConvertData/" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + "/";
    outputDataPath = m_logPath + "OutputData/";
    outputDataTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    lotFilePath =
        QApplication::applicationDirPath() + "/Config/" + (lotName_ST.isEmpty() ? "Lot001" : lotName_ST) + ".txt";

    try {
        startTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

        for (int i = 0; i < CFG_PARSE.getGroupOrderVec().size(); i++) {
            if (m_loopStopFlag)
                break;

            QString groupName = QString::fromStdString(CFG_PARSE.getGroupOrderItem(i));
            std::vector<Items*> tempFlowItemList = m_testFlowTool->getTestPlanItemMap()[groupName];
            if (groupName.contains("offset", Qt::CaseInsensitive)) {
                if (rms == 0) {
                    QFile aFile(lotFilePath);
                    if (aFile.exists()) {
                        if (aFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

                            QString rmsStr = QString::fromLatin1(aFile.readAll());
                            rms = rmsStr.toDouble();
                            aFile.close();
                            MLOG_INFO(QString("Continue %1, Read rms %2.").arg(groupName).arg(rmsStr));
                            continue;
                        }
                    }
                } else {
                    MLOG_INFO(QString("Continue %1").arg(groupName));
                    continue;
                }
            }

            m_isSendOnly = false;
            for (int j = 0; j < tempFlowItemList.size(); j++) {
                if (m_loopStopFlag)
                    break;

                QString otherLog = "";
                response = "";
                response2 = "";
                Items* tempItem = tempFlowItemList[j];
                if (tempItem->Status == "1") {
                    tempItem->StartTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
                    QString _info = QString("ItemName %1, Ref %2, Param %3, Suffix %4")
                                        .arg(tempItem->ItemName)
                                        .arg(tempItem->Ref)
                                        .arg(tempItem->Param)
                                        .arg(tempItem->Suffix);
                    MLOG_INFO(_info);

                    if (tempItem->ClassType == "communication") {

                        QString cmdStr;
                        if (tempItem->Ref.contains("Uart")) {

                            emit sendDataToServer(
                                QString("Vlog#Test cell %1#%2$").arg(this->unitNum + offsetNum).arg(tempItem->Param));

                            deviceObject_Uart1 =
                                (QObject*)m_devices
                                    ->deviceDic[QString("%1%2%3").arg(tempItem->Ref).arg(unitNum + offsetNum).arg("1")]
                                    .value<void*>();
                            deviceObject_Uart2 =
                                (QObject*)m_devices
                                    ->deviceDic[QString("%1%2%3").arg(tempItem->Ref).arg(unitNum + offsetNum).arg("2")]
                                    .value<void*>();

                            QString tmpStr = tempItem->Param;
                            cmdStr = tmpStr;

                            if (tmpStr.contains("X_N") || tmpStr.contains("X_P")) {
                                createrXpOrXsCmd(tmpStr, cmdStr);
                                if (cmdStr.isEmpty())
                                    continue;
                            } else if (tmpStr.contains("|Z")) {
                                dealYParallelZCmd(tmpStr, tempItem, deviceObject_Uart1, deviceObject_Uart2, response2,
                                                  otherLog);
                            } else if (tmpStr.contains("Move_To")) {
                                dealMoveCmd(tmpStr, tempItem, deviceObject_Uart1, deviceObject_Uart2, response2,
                                            otherLog);
                            }

                            if (!tmpStr.contains("Move_To") && !tmpStr.contains("|Z")) {
                                QMetaObject::invokeMethod(deviceObject_Uart1, tempItem->Function.toStdString().c_str(),
                                                          Qt::BlockingQueuedConnection,
                                                          Q_RETURN_ARG(QString, response2), Q_ARG(QString, cmdStr),
                                                          Q_ARG(float, 5));
                                otherLog.append(QString("\n cmd:%1 response:%2").arg(response2).arg(cmdStr));

                                QString msg = QString("%1 Uart1 cmd:%2 response:%3\n")
                                                  .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                                                  .arg(cmdStr)
                                                  .arg(response2);
                                FileTool::writeContentWithPath("", msg, deviceLogPath,
                                                               QString("serialport_%1.txt").arg(outputDataTime));
                            }

                            // wait for socket data ready
                            std::chrono::milliseconds timespan(20000);
                            auto start = std::chrono::steady_clock::now();

                            while (m_isSendOnly) {
                                QApplication::processEvents();
                                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                                std::chrono::duration<double> diff = std::chrono::steady_clock::now() - start;
                                if (std::chrono::milliseconds(static_cast<long>(diff.count() * 1000)) >= timespan) {
                                    throw std::runtime_error("Move_to wait for data timeout!");
                                }
                            }

                        } else {
                            cmdStr = tempItem->Param;
                            if (cmdStr.contains("SET_pcbADCsample")) {
                                emit sendDataToServer(QString("Vlog#Test cell %1#SET_pcbADCsample of %2$")
                                                          .arg(this->unitNum + offsetNum)
                                                          .arg(tempItem->Group));
                            } else {
                                emit sendDataToServer(QString("Vlog#Test cell %1#GET_pcbADCdata of %2$")
                                                          .arg(this->unitNum + offsetNum)
                                                          .arg(tempItem->Group));
                            }
                            otherLog.append(QString("\n start communication:%1")
                                                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")));
                            deviceObject =
                                (QObject*)m_devices->deviceDic[tempItem->Ref + QString::number(unitNum + offsetNum)]
                                    .value<void*>();

                            response.clear();
                            if (tempItem->Function == "sendDataWithResponse") {
                                QMetaObject::invokeMethod(deviceObject, tempItem->Function.toStdString().c_str(),
                                                          Qt::BlockingQueuedConnection, Q_ARG(QString, cmdStr),
                                                          Q_ARG(float, 20), Q_ARG(Items*, tempItem));
                            } else {
                                m_isSendOnly = true;
                                QMetaObject::invokeMethod(deviceObject, tempItem->Function.toStdString().c_str(),
                                                          Qt::BlockingQueuedConnection, Q_ARG(QString, cmdStr),
                                                          Q_ARG(Items*, tempItem));
                            }

                            otherLog.append(QString("\n end communication:%1")
                                                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")));

                            if (cmdStr.contains("SET_pcbADCsample")) {
                                QString msg = QString("%1 TcpSocket cmd:%2 response:%3\n")
                                                  .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                                                  .arg(cmdStr)
                                                  .arg(QString::fromUtf8(response));
                                FileTool::writeContentWithPath("", msg, deviceLogPath,
                                                               QString("serialport_%1.txt").arg(outputDataTime));
                            } else {
                                QString msg = QString("%1 TcpSocket cmd:%2 response:rawdata\n")
                                                  .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                                                  .arg(cmdStr);
                                FileTool::writeContentWithPath("", msg, deviceLogPath,
                                                               QString("serialport_%1.txt").arg(outputDataTime));
                            }
                        }
                    }

                    if (tempItem->ClassType == "ShowItems") {
                        // 2.get items from local file.
                        QString itemFilePath;
                        QString itemFileName;
                        otherLog.append(QString("\n start get items from local file:%1")
                                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")));
                        if (groupName == "Noise") {
                            itemFilePath = outputPath + "MaestroERSFoMs/";
                            itemFileName = "MaestroERSFoMs_NoiseFoMs.csv";
                        } else {
                            itemFilePath = outputPath + "MaestroERSFoMs/";
                            itemFileName = "MaestroERSFoMs_FoMs.csv";
                        }

                        otherLog.append(QString("\n end get items from local file:%1")
                                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")));
                        std::vector<Items*> currentSpecItemList = m_testSpecTool->getTestPlanItemMap()[groupName];
                        getItemsFromLocalFile(itemFilePath, itemFileName, currentSpecItemList);

                        // 3.show the items in table.
                        otherLog.append(QString("\n start show the items in table:%1")
                                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")));
                        for (int k = 0; k < currentSpecItemList.size(); k++) {
                            judgmentWithoutMTCP(currentSpecItemList[k], failMessge);
                            tempItem->Result &= currentSpecItemList[k]->Result;
                            content = content + currentSpecItemList[k]->Value + ",";
                            pivotStr +=
                                QString("%1,%2,,#%3,0000-00-00 00:00:00,0000-00-00 00:00:00,0,%4,%5,%6,%7,%8\n")
                                    .arg(currentSpecItemList[k]->Index)
                                    .arg(currentSpecItemList[k]->Group.contains("Noise") ? "PDDC" : "DIFF")
                                    .arg(currentSpecItemList[k]->ItemName)
                                    .arg(currentSpecItemList[k]->lowerLimits == "nan"
                                             ? ""
                                             : currentSpecItemList[k]->lowerLimits)
                                    .arg(currentSpecItemList[k]->upperLimits == "nan"
                                             ? ""
                                             : currentSpecItemList[k]->upperLimits)
                                    .arg(currentSpecItemList[k]->Unit == "NA" ? "" : currentSpecItemList[k]->Unit)
                                    .arg(currentSpecItemList[k]->Value)
                                    .arg(currentSpecItemList[k]->Result ? "PASS" : "FAIL");

                            QString _info = QString("ItemName %1, Lower %2, Upper %3, Unit %4, Value %5")
                                                .arg(currentSpecItemList[k]->ItemName)
                                                .arg(currentSpecItemList[k]->lowerLimits)
                                                .arg(currentSpecItemList[k]->upperLimits)
                                                .arg(currentSpecItemList[k]->Unit)
                                                .arg(currentSpecItemList[k]->Value);
                            MLOG_INFO(_info);
                            emit flushUiWithRow(currentSpecItemList[k], flushRow++, 5, unitNum);
                        }
                        otherLog.append(QString("\n end show the items in table:%1")
                                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")));

                        currentSpecItemList.clear();
                    }

                    if (tempItem->ClassType == "MtcpInfo") {
                        std::vector<Items*> currentSpecItemList = m_testSpecTool->getTestPlanItemMap()[groupName];
                        for (int k = 0; k < currentSpecItemList.size(); k++) {
                            if (currentSpecItemList[k]->ItemName.contains("MODULE_SN")) {
                                currentSpecItemList[k]->Value = itemSn;
                            } else if (currentSpecItemList[k]->ItemName.contains("MOD_LOT_NAME")) {
                                currentSpecItemList[k]->Value = lotName_ST;
                            } else if (currentSpecItemList[k]->ItemName.contains("TEST_SW_VER")) {
                                currentSpecItemList[k]->Value = QString::fromStdString(CFG_PARSE.getVersion());
                            } else {
                                currentSpecItemList[k]->Value = currentSpecItemList[k]->Param;
                            }

                            content = content + currentSpecItemList[k]->Value + ",";
                            pivotStr +=
                                QString("%1,%2,,#%3,0000-00-00 00:00:00,0000-00-00 00:00:00,0,%4,%5,%6,%7,PASS\n")
                                    .arg(currentSpecItemList[k]->Index)
                                    .arg(currentSpecItemList[k]->Group)
                                    .arg(currentSpecItemList[k]->ItemName)
                                    .arg(currentSpecItemList[k]->lowerLimits == "nan"
                                             ? ""
                                             : currentSpecItemList[k]->lowerLimits)
                                    .arg(currentSpecItemList[k]->upperLimits == "nan"
                                             ? ""
                                             : currentSpecItemList[k]->upperLimits)
                                    .arg(currentSpecItemList[k]->Unit == "NA" ? "" : currentSpecItemList[k]->Unit)
                                    .arg(currentSpecItemList[k]->Value);

                            emit flushUiWithRow(currentSpecItemList[k], flushRow++, 5, unitNum);
                        }
                    }

                    if (response.size() < 100) {
                        tempItem->Response = response;
                    } else {
                        tempItem->Response = "rawdata";
                    }

                    response.clear();
                    response2.clear();
                }
            }
        }

        // judge the result
        if (m_loopStopFlag) {
            testResult = TEST_FAIL;
            testResultStr = "FAIL";
        } else {
            if (failMessge != "") {
                testResult = TEST_FAIL;
                testResultStr = "FAIL";
            } else {
                testResult = TEST_PASS;
            }
        }

        endTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        //    emit sendDataToServer("TestFinished!$");

        FileTool::writeContentWithPath("", pivotStr, QString::fromStdString(m_mtcpFilePath), "/PivotReport.csv");
        int ret = -1;
        emit savePivotFile(QString::fromStdString(m_mtcpFilePath) + "/PivotReport.csv", ret);
        if (ret != 0) {
            testResult = TEST_FAIL;
            testResultStr = "FAIL";
            failMessge += "Send pivot to Mtcp failed.";
        }
        saveUnitCSVData(itemSn, testResultStr, QString::number(unitNum + offsetNum), failMessge, uiInfo, content);
    }
    catch (std::runtime_error& e) {
        m_loopStopFlag = true;
        emit showWarning(tr("Runtime error: %1").arg(e.what()));
    }
    catch (...) {
        m_loopStopFlag = true;
        emit showWarning(tr("Unknow error happened, Stop test."));
    }

    content.clear();
    isReady = true;
    itemSn.clear();

    response.clear();
    response2.clear();

    emit stopTimer(unitNum, testResult);

    m_devices->clearBuffer();
    MLOG_INFO("End run thread.\n\n");
    quit();
}

void TestAction::onDealWithSocketRecv(const QByteArray& recv, Items* tempItem, void* context)
{
    TestAction* pthis = static_cast<TestAction*>(context);
    try {
        if (recv.contains(tempItem->Suffix.toLatin1()) || recv.contains("raw data")) {

            if (recv.length() > 200) {
                pthis->rawDataFilePath = pthis->convertDataPath;
                pthis->rawDataFileName =
                    tempItem->Group + "_" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".csv";

                std::string resStr = recv.toStdString();
                QByteArray fileStr;
                std::vector<std::string> tempVec;
                char buf[6];
                memset(buf, 0, sizeof(buf));

                std::stringstream csvContent;
                if (tempItem->Group == "Offset") {
                    csvContent << "TIME[S],CH2[V]\n";
                } else if (tempItem->Group == "Noise") {
                    csvContent << "TIME[S],CHannel1_Current[uA]\n";
                } else {
                    csvContent << "TIME[S],CHannel1_Current[uA]\n";
                }

                std::vector<double*> timeData;
                std::vector<double*> voltageData;
                std::vector<int> dataSize;
                size_t count = 0;
                double* tData = NULL;
                double* targetVData = NULL;

                count = floor((resStr.size() - 17 - 9) / 4);
                dataSize.push_back(count);

                tData = (double*)malloc(count * sizeof(double));
                targetVData = (double*)malloc(count * sizeof(double));

                double sum = 0;
                double timeStep = CFG_PARSE.getTimeStep();
                // filter row data[unit:V][ and ][DONE]\r\n
                for (unsigned int i = 17; i < resStr.size() - 9; i++) {
                    snprintf(buf, sizeof(buf), "%02x", (uint8_t)resStr[i]);
                    tempVec.push_back(buf);
                    if (tempVec.size() == 4) {
                        std::string hexS = tempVec[3] + tempVec[2] + tempVec[1] + tempVec[0];
                        int c = QString::fromStdString(hexS).toInt(nullptr, 16);
                        float d = *(float*)&c;

                        double _t = timeStep * (floor((i - 17) / 4) + 1);
                        tData[(int)floor((i - 17) / 4) + 1] = _t;

                        if (tempItem->Group == "Offset") {
                            if (d > 10 || d < 1) {
                                pthis->MLOG_INFO("Raw data analysis error.");
                                throw std::runtime_error("Raw data analysis error.");
                            }
                            sum += d * d;
                            fileStr.append(QString("%1,\r\n").arg(d));
                            csvContent << _t << "," << d << "\n";
                            targetVData[(int)floor((i - 17) / 4) + 1] = (double)d;
                        } else {
                            double tmpVal = 1e6 * (d - pthis->rms) / 10000.0;
                            fileStr.append(QString("%1,\n").arg(QString::number(d, 'f', 12)));
                            csvContent << _t << "," << tmpVal << "\n";
                            targetVData[(int)floor((i - 17) / 4) + 1] = tmpVal;
                            //                            qDebug() << d << "   " << rms << "    " <<
                            //                            QString::number(tmpVal);
                        }
                        tempVec.clear();
                    }
                }

                timeData.push_back(tData);
                voltageData.push_back(targetVData);

                if (tempItem->Group == "Offset") {
                    pthis->rms = sqrt(sum / (count * 1.0));
                    QFile aFile(pthis->lotFilePath);
                    if (aFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QByteArray rmsStr = QString::number(pthis->rms).toLatin1();
                        aFile.write(rmsStr);
                        pthis->MLOG_INFO(
                            QString("Write rms %1 to %2").arg(QString::number(pthis->rms)).arg(pthis->lotFilePath));
                        aFile.close();
                    }
                }

                pthis->writeContentWithPath(pthis->rawDataPath, pthis->rawDataFileName, fileStr);
                FileTool::writeContentWithPath("", QString::fromStdString(csvContent.str()), pthis->rawDataFilePath,
                                               pthis->rawDataFileName);

                if (tempItem->Group != "Offset") {
                    if (tempItem->Group == "Noise") {
                        pthis->outputPath =
                            pthis->outputDataPath + "Noise/" + pthis->outputDataTime + "/" + tempItem->Group + "/";
                        Util::MakeNDir(pthis->outputPath.toStdString());
                        pthis->palgo->jptProcessSingleNoise(pthis->outputPath, timeData, voltageData, dataSize);
                        //                                        palgo->jptProcessSingleNoiseDemo(rawDataFilePath,
                        //                                        rawDataFileName, outputPath);
                    } else {
                        pthis->outputPath = pthis->outputDataPath + "SingleStream/DC_Coupled/" + "/"
                                            + pthis->outputDataTime + "/" + tempItem->Group + "/";
                        Util::MakeNDir(pthis->outputPath.toStdString());

                        float targetAngle;
                        float velcoty;
                        CFG_PARSE.getTargetAngleAndVel(tempItem->Group.toStdString(), targetAngle, velcoty);
                        pthis->palgo->jptProcessSingle(pthis->outputPath, velcoty, targetAngle, timeData, voltageData,
                                                       dataSize);
                        //                                        palgo->jptProcessSingleDemo(rawDataFilePath,
                        //                                        rawDataFileName, outputPath,
                        //                                                                    velcoty,
                        //                                                                    targetAngle);
                    }

                    for (size_t idx = 0; idx < timeData.size(); idx++) {
                        FREE_MEM(timeData[idx]);
                        FREE_MEM(voltageData[idx]);
                    }
                }

                pthis->m_isSendOnly = false;
            } else {
                qDebug() << "response:" << recv;
            }
        }
    }
    catch (std::runtime_error& e) {
        pthis->m_loopStopFlag = true;
        emit pthis->showWarning(tr("Runtime error: %1").arg(e.what()));
    }
    catch (...) {
        pthis->m_loopStopFlag = true;
        emit pthis->showWarning(tr("Unknow error happened, Stop test."));
    }
}

void TestAction::createrXpOrXsCmd(QString str, QString& cmd)
{
    cmd.clear();
    if (str.contains("X_N")) {
        str = str.replace("X_N", "");
    } else {
        str = str.replace("X_P", "");
    }

    float step = str.toFloat() / 1000.0;
    int plusCnt = 0;

    if (str.contains("X_N")) {
        plusCnt = (int)CFG_PARSE.getXp() * step / 0.25;
    } else {
        plusCnt = -(int)CFG_PARSE.getXp() * step / 0.25;
    }

    if (abs(plusCnt) != 0) {
        QString plusStr = QString("%1").arg(plusCnt);
        QString plus_direction;
        QString plus_hex1;
        QString plus_hex2;
        QString plus_hex3;
        QString plus_hex4;

        calPlusCntToHex(plusStr, plus_direction, plus_hex1, plus_hex2, plus_hex3, plus_hex4);
        cmd = QString("%1,%2,%3,%4,%5,%6]")
                  .arg("SET_pcb[0x1,0x82,0x3")
                  .arg(plus_direction)
                  .arg(plus_hex4)
                  .arg(plus_hex3)
                  .arg(plus_hex2)
                  .arg(plus_hex1);

        MLOG_INFO(str + " create cmd: " + cmd);
    }
}

void TestAction::dealYParallelZCmd(QString str, Items* item, QObject* devObj1, QObject* devObj2, QString& repStr,
                                   QString& logStr)
{
    QList<QString> tList = str.split("|");
    QString cmd_Y = tList[0];
    QString cmd_Z = tList[1];

    QString plusStr;
    QString plus_direction;
    QString plus_hex1;
    QString plus_hex2;
    QString plus_hex3;
    QString plus_hex4;

    cmd_Y = cmd_Y.replace("Y_", "");
    cmd_Z = cmd_Z.replace("Z_", "");

    float step_y;
    int plusCnt_y = 0;
    float step_z;
    int plusCnt_z = 0;
    if (cmd_Y.contains("P")) {
        cmd_Y = cmd_Y.replace("P", "");
    } else {
        cmd_Y = cmd_Y.replace("N", "");
    }
    step_y = cmd_Y.toFloat() / 1000.0;
    plusCnt_y = (int)20000.0 * step_y;

    if (cmd_Z.contains("P")) {
        cmd_Z = cmd_Z.replace("P", "");
    } else {
        cmd_Z = cmd_Z.replace("N", "");
    }
    step_z = cmd_Z.toFloat() / 1000.0;
    plusCnt_z = (int)20000.0 * step_z;

    if (abs(plusCnt_y) != 0) {
        plusStr = QString("%1").arg(plusCnt_y);
        calPlusCntToHex(plusStr, plus_direction, plus_hex1, plus_hex2, plus_hex3, plus_hex4);
        cmd_Y = QString("%1,%2,%3,%4,%5,%6]")
                    .arg("SET_pcb[0x1,0x82,0x3")
                    .arg(plus_direction)
                    .arg(plus_hex4)
                    .arg(plus_hex3)
                    .arg(plus_hex2)
                    .arg(plus_hex1);
        MLOG_INFO(str + " create cmd Y: " + cmd_Y);
    } else {
        cmd_Y.clear();
    }

    if (abs(plusCnt_z) != 0) {
        plusStr = QString("%1").arg(plusCnt_z);
        calPlusCntToHex(plusStr, plus_direction, plus_hex1, plus_hex2, plus_hex3, plus_hex4);
        cmd_Z = QString("%1,%2,%3,%4,%5,%6]")
                    .arg("SET_pcb[0x1,0x82,0x4")
                    .arg(plus_direction)
                    .arg(plus_hex4)
                    .arg(plus_hex3)
                    .arg(plus_hex2)
                    .arg(plus_hex1);
        MLOG_INFO(str + " create cmd Z: " + cmd_Z);
    } else {
        cmd_Z.clear();
    }

    if (!cmd_Y.isEmpty()) {
        QString tcmd = "SET_pcb[0x1,0x84,0x41,0x0,0x0,0x0,0x0,0x0]";
        QMetaObject::invokeMethod(devObj1, item->Function.toStdString().c_str(), Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QString, repStr), Q_ARG(QString, tcmd), Q_ARG(float, 5));

        QThread::msleep(100);

        tcmd = "SET_pcb[0x1,0x82,0x41,0x3,0xef,0xff,0xff,0xff]";
        QMetaObject::invokeMethod(devObj1, item->Function.toStdString().c_str(), Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QString, repStr), Q_ARG(QString, tcmd), Q_ARG(float, 5));
        QThread::msleep(100);

        QMetaObject::invokeMethod(devObj2, item->Function.toStdString().c_str(), Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QString, repStr), Q_ARG(QString, cmd_Y), Q_ARG(float, 5));

        FileTool::writeContentWithPath("",
                                       QString("%1 Uart2 cmd:%2 response:%3\n")
                                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                                           .arg(cmd_Y)
                                           .arg(repStr),
                                       deviceLogPath, QString("serialport_%1.txt").arg(outputDataTime));

        logStr.append(QString("\n cmd:%1 response:%2").arg(cmd_Y).arg(repStr));
        QThread::msleep(100);
    }

    if (!cmd_Z.isEmpty()) {
        QMetaObject::invokeMethod(devObj2, item->Function.toStdString().c_str(), Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QString, repStr), Q_ARG(QString, cmd_Z), Q_ARG(float, 5));

        FileTool::writeContentWithPath("",
                                       QString("%1 Uart2 cmd:%2 response:%3\n")
                                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                                           .arg(cmd_Z)
                                           .arg(repStr),
                                       deviceLogPath, QString("serialport_%1.txt").arg(outputDataTime));
        logStr.append(QString("\n cmd:%1 response:%2").arg(cmd_Z).arg(repStr));
        QThread::msleep(1000);
    }
}

void TestAction::dealMoveCmd(QString str, Items* item, QObject* devObj1, QObject* devObj2, QString& repStr,
                             QString& logStr)
{
    QList<QString> cmdList;
    QString m_Y;
    QString m_Z;
    QString m_Xp;
    QString m_beta;
    QString m_gamma;

    QRegExp rx("\\d+");
    QString indexStr;
    rx.indexIn(str, 0);
    indexStr = rx.cap(0);
    int _index = indexStr.toInt();
    m_Xp = list_Xp[_index];
    m_Y = list_Y[_index];
    m_Z = list_Z[_index];
    m_beta = list_Beta[_index];
    m_gamma = list_Gamma[_index];

    QString plusStr;
    QString plus_direction;
    QString plus_hex1;
    QString plus_hex2;
    QString plus_hex3;
    QString plus_hex4;

    int plusCnt = 0;
    float distance = 0;
    float degree = 0;

    if (str.contains("C01")) {
        // 5.send Xp-axis pluse
        degree = m_Xp.toFloat();
        plusCnt = -(int)CFG_PARSE.getXp() * degree / 0.25;
        if (!str.contains("C00")) {
            plus_Xp += plusCnt;
        } else {
            plusCnt = -plus_Xp;
        }
        if (abs(plusCnt) != 0) {
            plusStr = QString("%1").arg(plusCnt);
            calPlusCntToHex(plusStr, plus_direction, plus_hex1, plus_hex2, plus_hex3, plus_hex4);
            plusStr = QString("%1,%2,%3,%4,%5,%6]")
                          .arg("SET_pcb[0x1,0x82,0x3")
                          .arg(plus_direction)
                          .arg(plus_hex4)
                          .arg(plus_hex3)
                          .arg(plus_hex2)
                          .arg(plus_hex1);
            MLOG_INFO(str + " create cmd: " + plusStr);
            cmdList.append(plusStr);
        } else {
            cmdList.append("null");
        }

        // 2.send Y-axis pluse
        distance = m_Y.toFloat();
        plusCnt = -(int)CFG_PARSE.getY() * distance;
        if (!str.contains("C00")) {
            plus_Y += plusCnt;
        } else {
            plusCnt = -plus_Y;
        }
        if (abs(plusCnt) != 0) {
            plusStr = QString("%1").arg(plusCnt);
            calPlusCntToHex(plusStr, plus_direction, plus_hex1, plus_hex2, plus_hex3, plus_hex4);
            plusStr = QString("%1,%2,%3,%4,%5,%6]")
                          .arg("SET_pcb[0x1,0x82,0x3")
                          .arg(plus_direction)
                          .arg(plus_hex4)
                          .arg(plus_hex3)
                          .arg(plus_hex2)
                          .arg(plus_hex1);
            MLOG_INFO(str + " create cmd: " + plusStr);
            cmdList.append(plusStr);
        } else {
            cmdList.append("null");
        }

        // 1.send Z-axis pluse
        distance = m_Z.toFloat();
        plusCnt = (int)CFG_PARSE.getZ() * distance;
        if (!str.contains("C00")) {
            plus_Z += plusCnt;
        } else {
            plusCnt = -plus_Z;
        }
        if (abs(plusCnt) != 0) {
            plusStr = QString("%1").arg(plusCnt);
            calPlusCntToHex(plusStr, plus_direction, plus_hex1, plus_hex2, plus_hex3, plus_hex4);
            plusStr = QString("%1,%2,%3,%4,%5,%6]")
                          .arg("SET_pcb[0x1,0x82,0x4")
                          .arg(plus_direction)
                          .arg(plus_hex4)
                          .arg(plus_hex3)
                          .arg(plus_hex2)
                          .arg(plus_hex1);
            MLOG_INFO(str + " create cmd: " + plusStr);
            cmdList.append(plusStr);
        } else {
            cmdList.append("null");
        }
    } else {
        // 1.send Z-axis pluse
        distance = m_Z.toFloat();
        plusCnt = (int)CFG_PARSE.getY() * distance;
        if (!str.contains("C00")) {
            plus_Z += plusCnt;
        } else {
            plusCnt = -plus_Z;
        }
        if (abs(plusCnt) != 0) {
            plusStr = QString("%1").arg(plusCnt);
            calPlusCntToHex(plusStr, plus_direction, plus_hex1, plus_hex2, plus_hex3, plus_hex4);
            plusStr = QString("%1,%2,%3,%4,%5,%6]")
                          .arg("SET_pcb[0x1,0x82,0x4")
                          .arg(plus_direction)
                          .arg(plus_hex4)
                          .arg(plus_hex3)
                          .arg(plus_hex2)
                          .arg(plus_hex1);
            MLOG_INFO(str + " create cmd: " + plusStr);
            cmdList.append(plusStr);
        } else {
            cmdList.append("null");
        }

        // 2.send Y-axis pluse
        distance = m_Y.toFloat();
        plusCnt = -(int)CFG_PARSE.getZ() * distance;
        if (!str.contains("C00")) {
            plus_Y += plusCnt;
        } else {
            plusCnt = -plus_Y;
        }
        if (abs(plusCnt) != 0) {
            plusStr = QString("%1").arg(plusCnt);
            calPlusCntToHex(plusStr, plus_direction, plus_hex1, plus_hex2, plus_hex3, plus_hex4);
            plusStr = QString("%1,%2,%3,%4,%5,%6]")
                          .arg("SET_pcb[0x1,0x82,0x3")
                          .arg(plus_direction)
                          .arg(plus_hex4)
                          .arg(plus_hex3)
                          .arg(plus_hex2)
                          .arg(plus_hex1);
            MLOG_INFO(str + " create cmd: " + plusStr);
            cmdList.append(plusStr);
        } else {
            cmdList.append("null");
        }

        // 5.send Xp-axis pluse
        degree = m_Xp.toFloat();
        plusCnt = -(int)CFG_PARSE.getXp() * degree / 0.25;
        if (!str.contains("C00")) {
            plus_Xp += plusCnt;
        } else {
            plusCnt = -plus_Xp;
        }
        if (abs(plusCnt) != 0) {
            plusStr = QString("%1").arg(plusCnt);
            calPlusCntToHex(plusStr, plus_direction, plus_hex1, plus_hex2, plus_hex3, plus_hex4);
            plusStr = QString("%1,%2,%3,%4,%5,%6]")
                          .arg("SET_pcb[0x1,0x82,0x3")
                          .arg(plus_direction)
                          .arg(plus_hex4)
                          .arg(plus_hex3)
                          .arg(plus_hex2)
                          .arg(plus_hex1);
            MLOG_INFO(str + " create cmd: " + plusStr);
            cmdList.append(plusStr);
        } else {
            cmdList.append("null");
        }
    }

    // 3.send beta-axis pluse
    degree = m_beta.toFloat();
    plusCnt = -(int)CFG_PARSE.getBeta() * degree / 0.45;

    if (!str.contains("C00") && !str.contains("C05") && !str.contains("C06")) {
        plus_beta += plusCnt;
    } else {
        if (str.contains("C05")) {
            plusCnt = -plus_beta;
        } else {
            plusCnt = 0;
        }
    }

    if (abs(plusCnt) != 0) {
        plusStr = QString("%1").arg(plusCnt);
        calPlusCntToHex(plusStr, plus_direction, plus_hex1, plus_hex2, plus_hex3, plus_hex4);
        plusStr = QString("%1,%2,%3,%4,%5,%6]")
                      .arg("SET_pcb[0x1,0x82,0x2")
                      .arg(plus_direction)
                      .arg(plus_hex4)
                      .arg(plus_hex3)
                      .arg(plus_hex2)
                      .arg(plus_hex1);
        MLOG_INFO(str + " create cmd: " + plusStr);
        cmdList.append(plusStr);
    } else {
        cmdList.append("null");
    }

    // 4.send gamma-axis pluse
    degree = m_gamma.toFloat();
    plusCnt = (int)CFG_PARSE.getGamma() * degree / 0.55;
    if (!str.contains("C00") && !str.contains("C06")) {
        plus_gamma += plusCnt;
    } else {
        if (str.contains("C06")) {
            plusCnt = -plus_gamma;
        } else {
            plusCnt = 0;
        }
    }

    if (abs(plusCnt) != 0) {
        plusStr = QString("%1").arg(plusCnt);
        calPlusCntToHex(plusStr, plus_direction, plus_hex1, plus_hex2, plus_hex3, plus_hex4);
        plusStr = QString("%1,%2,%3,%4,%5,%6]")
                      .arg("SET_pcb[0x1,0x82,0x4")
                      .arg(plus_direction)
                      .arg(plus_hex4)
                      .arg(plus_hex3)
                      .arg(plus_hex2)
                      .arg(plus_hex1);
        MLOG_INFO(str + " create cmd: " + plusStr);
        cmdList.append(plusStr);

    } else {
        cmdList.append("null");
    }

    for (int i = 0; i < cmdList.count(); i++) {
        if (cmdList[i] == "null")
            continue;
        QString cmdStr = cmdList[i];

        if (str.contains("C01")) {
            if (i == 1 || i == 2) {
                QMetaObject::invokeMethod(devObj2, item->Function.toStdString().c_str(), Qt::BlockingQueuedConnection,
                                          Q_RETURN_ARG(QString, repStr), Q_ARG(QString, cmdStr), Q_ARG(float, 10));
                logStr.append(QString("\n cmd:%1 response:%2").arg(repStr).arg(cmdStr));

                QThread::msleep(50);
                FileTool::writeContentWithPath("",
                                               QString("%1 Uart2 cmd:%2 response:%3\n")
                                                   .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                                                   .arg(cmdStr)
                                                   .arg(repStr),
                                               deviceLogPath, QString("serialport_%1.txt").arg(outputDataTime));
            } else {

                QThread::msleep(50);
                QMetaObject::invokeMethod(devObj1, item->Function.toStdString().c_str(), Qt::BlockingQueuedConnection,
                                          Q_RETURN_ARG(QString, repStr), Q_ARG(QString, cmdStr), Q_ARG(float, 10));
                logStr.append(QString("\n cmd:%1 response:%2").arg(repStr).arg(cmdStr));

                FileTool::writeContentWithPath("",
                                               QString("%1 Uart1 cmd:%2 response:%3\n")
                                                   .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                                                   .arg(cmdStr)
                                                   .arg(repStr),
                                               deviceLogPath, QString("serialport_%1.txt").arg(outputDataTime));
            }
        } else {
            if (i < 2) {
                QMetaObject::invokeMethod(devObj2, item->Function.toStdString().c_str(), Qt::BlockingQueuedConnection,
                                          Q_RETURN_ARG(QString, repStr), Q_ARG(QString, cmdStr), Q_ARG(float, 10));

                QThread::msleep(100);
                logStr.append(QString("\n cmd:%1 response:%2").arg(repStr).arg(cmdStr));

                FileTool::writeContentWithPath("",
                                               QString("%1 Uart2 cmd:%2 response:%3\n")
                                                   .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                                                   .arg(cmdStr)
                                                   .arg(repStr),
                                               deviceLogPath, QString("serialport_%1.txt").arg(outputDataTime));
            } else {
                QMetaObject::invokeMethod(devObj1, item->Function.toStdString().c_str(), Qt::BlockingQueuedConnection,
                                          Q_RETURN_ARG(QString, repStr), Q_ARG(QString, cmdStr), Q_ARG(float, 10));
                logStr.append(QString("\n cmd:%1 response:%2").arg(repStr).arg(cmdStr));

                FileTool::writeContentWithPath("",
                                               QString("%1 Uart1 cmd:%2 response:%3\n")
                                                   .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                                                   .arg(cmdStr)
                                                   .arg(repStr),
                                               deviceLogPath, QString("serialport_%1.txt").arg(outputDataTime));
            }
        }
        logStr.append(QString("\n motor response:%1").arg(repStr));
        QThread::msleep(100);
    }

    QThread::msleep(1000);
    if (item->Group.contains("C04")) {
        QThread::msleep(1000);
    }
}

void TestAction::getItemsFromLocalFile(const QString& filePath, const QString& fileName,
                                       std::vector<Items*>& showItemList)
{
    if (!Util::IsFileExist((filePath + fileName).toStdString())) {
        std::string err = (filePath + fileName).toStdString() + " is not exist.";
        throw std::runtime_error(err);
    }
    QString itemContent = FileTool::readContentWithPath(filePath + fileName);
    QStringList contentList = itemContent.split("\n");
    QStringList valueList = contentList[1].split(",");

    if (valueList.count() == showItemList.size()) {
        for (int i = 0; i < showItemList.size(); i++) {
            showItemList[i]->Value = valueList[i];
        }
    } else {
        throw runtime_error("output file length != show items length.");
    }
}

void TestAction::saveUnitCSVData(const QString& itemSn, const QString& result, const QString& slot,
                                 const QString& failMsg, const QString& uiInfo, const QString& itemContents)
{
    QString testStopTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString Conents = "\n" + itemSn + "," + result + "," + slot + "," + startTime + "," + endTime + "," + trial_ST + ","
                      + uiInfo + "," + failMsg + "," + itemContents;
    if (result == "PASS") {
        result_R = "1";
    } else {
        result_R = "0";
    }

    csvInfo_R =
        itemSn + "," + result + "," + slot + "," + testStopTime + "," + testStopTime + "," + trial_ST + "," + uiInfo;
    failItems_R = failMsg;

    testItems_R = itemContents;
    qDebug() << Conents;

    SingletonMutex::getInstance().lock();
    QString filePaths = QString::fromStdString(CFG_PARSE.getLogPath()) + "Summary/"
                        + QDateTime::currentDateTime().toString("yyyy-MM-dd") + "/";
    FileTool::writeContentWithPath(titleStr, Conents, filePaths, "summary.csv");
    SingletonMutex::getInstance().unlock();
}

QString TestAction::saveUnitTxtLog(const Items* testItem)
{
    QString logInfo = "\n No.:" + testItem->Index + "\n tid:" + testItem->ItemName + "\n lower:" + testItem->lowerLimits
                      + "\n upper:" + testItem->upperLimits + "\n unit:" + testItem->Unit + "\n prefix str:"
                      + testItem->StartStr + "\n suffix str:" + testItem->EndStr + "\n device:" + testItem->Target
                      + "\n cmd:" + testItem->Param + "\n return data:" + testItem->Response
                      + "\n deal data:" + testItem->Value + "\n result:" + (testItem->Result ? "PASS" : "FALSE")
                      + "\n start time:" + testItem->StartTime + "\n endtime:" + testItem->StopTime
                      + "\n otherLog:" + testItem->others + "\n****************************************\n\n";

    FileTool::writeContentWithPath(QString("\n\n\n\n%1").arg(itemSn), logInfo, m_logPath, "/Unit.log");
    return logInfo;
}

void TestAction::killAllDevice(ConfigDevice* tempConfigDevice)
{
    QObject* deviceObject;

    QMap<QString, QVariant>::iterator iter = tempConfigDevice->deviceDic.begin();

    while (iter != tempConfigDevice->deviceDic.end()) {
        qDebug() << "Iterator " << iter.key() << ":" << iter.value();

        deviceObject = (QObject*)iter.value().value<void*>();

        QMetaObject::invokeMethod(deviceObject, "closeDevice", Qt::BlockingQueuedConnection);

        iter++;
    }
}

TestAction::~TestAction()
{
    if (NULL != m_devices) {
        killAllDevice(m_devices);
    }
    if (NULL != device_th) {
        device_th->quit();
        device_th->wait();
        device_th = NULL;
    }
}

QString TestAction::dealWithResponseStr(const QString& responseStr)
{
    QString tempStr = responseStr;
    tempStr = tempStr.replace("[DONE],", "").replace("ADCSample,", "");
    return tempStr;
}

void TestAction::saveRawdataToLocalFile(const QString& filePath, const QString& fileName, const QString& rawDataPath,
                                        const QString& content, QString& groupName, QString& otherLog)
{
    QString str = content;

    if (isHexData) {
        QList<QString> hexArr = str.split(" ");
        QList<double> rawData;
        QString fileStr;
        QString llStr;

        if (groupName == "Offset") {
            double sum = 0;
            for (int i = 0; i < hexArr.count() - 4; i += 4) {
                QString hexStr1 = hexArr[i + 3] + hexArr[i + 2] + hexArr[i + 1] + hexArr[i];
                int c = hexStr1.toInt(nullptr, 16);
                float d = *(float*)&c;
                if (d > 10 || d < 1) {
                    isDatabad = true;
                    return;
                }
                sum += d * d;
                rawData.append(d);
            }

            rms = sqrt(sum / (rawData.count() * 1.0));

            for (int i = 0; i < rawData.count(); i++) {
                fileStr.append(QString("%1,\r\n").arg((double_t)rawData[i]));
            }

            writeContentWithPath(rawDataPath, fileName, fileStr);
        } else {
            for (int i = 0; i < hexArr.count() - 4; i += 4) {
                QString hexStr1 = hexArr[i + 3] + hexArr[i + 2] + hexArr[i + 1] + hexArr[i];
                int c = hexStr1.toInt(nullptr, 16);
                float d = *(float*)&c;
                rawData.append(d);
            }
            QThread::usleep(10);
            for (int i = 0; i < rawData.count(); i++) {
                double tmpVal = 1e6 * (rawData[i] - rms) / 10000.0;
                fileStr.append(QString("%1,\r\n").arg(QString::number(tmpVal, 'f', 12)));
                llStr.append(QString("%1,\n").arg(QString::number(rawData[i], 'f', 12)));
            }
            writeContentWithPath(rawDataPath, QString("%1").arg(fileName), llStr);
        }
        str = fileStr;
    } else {
        str = content;
        str = str.replace(",", "\r\n");
    }

    size_t fileSz = str.size();
    uint8_t* fileData = NULL;
    otherLog.append(
        QString("\n start write local file1:%1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")));

    fileData = (uint8_t*)qstrdup(str.toLatin1().constData());
    otherLog.append(
        QString("\n start write local file2:%1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")));

    t_mlCsvDetail fileFormat;
    mlCsv_getDataFormat(NULL, fileData, fileSz, fileFormat);
    otherLog.append(
        QString("\n start write local file:%3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")));

    std::stringstream csvContent;
    if (groupName == "Offset") {
        csvContent << "TIME[S],CH2[V]\n";
        for (size_t idx = 0; idx < fileFormat.numOfRows() - 1; idx++) {
            const char* rowStr = (char*)fileData + fileFormat.rowStartPos(idx);
            std::string itemVoltage(rowStr + fileFormat.itemStartPos(idx, 0), fileFormat.itemLen(idx, 0));
            csvContent << CFG_PARSE.getTimeStep() * (idx + 1) << "," << itemVoltage << "\n";
        }
    } else if (groupName == "Noise") {
        csvContent << "TIME[S],CHannel1_Current[uA]\n";
        for (size_t idx = 0; idx < fileFormat.numOfRows() - 1; idx++) {
            const char* rowStr = (char*)fileData + fileFormat.rowStartPos(idx);
            std::string itemVoltage(rowStr + fileFormat.itemStartPos(idx, 0), fileFormat.itemLen(idx, 0));
            csvContent << CFG_PARSE.getTimeStep() * (idx + 1) << "," << itemVoltage << "\n";
        }
    } else {
        csvContent << "TIME[S],CHannel1_Current[uA]\n";
        for (size_t idx = 0; idx < fileFormat.numOfRows() - 1; idx++) {
            const char* rowStr = (char*)fileData + fileFormat.rowStartPos(idx);
            std::string itemVoltage(rowStr + fileFormat.itemStartPos(idx, 0), fileFormat.itemLen(idx, 0));
            csvContent << CFG_PARSE.getTimeStep() * (idx + 1) << "," << itemVoltage << "\n";
        }
    }
    FileTool::writeContentWithPath("", QString::fromStdString(csvContent.str()), filePath, fileName);
}

void TestAction::writeContentWithPath(const QString& filePath, const QString& fileName, const QString& content)
{
    if (!QDir(filePath).exists()) {

        QDir().mkpath(filePath);
    }

    QString Path = filePath + fileName;
    QFile aFile(Path);

    if (aFile.open(QIODevice::WriteOnly | QIODevice::Text)) {

        QByteArray writeBuffer = content.toLocal8Bit();
        aFile.write((writeBuffer));
        aFile.close();
    }
}

void TestAction::judgmentWithoutMTCP(Items* tmpItem, QString& failMsg)
{
    if (tmpItem->Value.toDouble() > tmpItem->upperLimits.toDouble()
        || tmpItem->Value.toDouble() < tmpItem->lowerLimits.toDouble()) {

        failMsg.append(QString("%1;").arg(tmpItem->ItemName));
        tmpItem->Result = false;
    }
}

void TestAction::algoReleaseSystem() {}

void TestAction::initTcpSocketForHandler()
{
    tcpClient = new QTcpSocket(this);
    connect(tcpClient, &QTcpSocket::connected, [=]() {
        ishandlerConnected = true;
        emit connectedDeviceSignal(m_devices->getConnectFailedDevices());

        // 1. send register code to server
        // format: Register#761001#1$
        tcpClientSendData(
            QString("Register#Fixture%1#%2#%3$").arg(unitNum + offsetNum).arg(unitNum + offsetNum).arg(this->titleStr));
    });

    connect(tcpClient, &QTcpSocket::readyRead, [=]() {
        QByteArray array = tcpClient->readAll();
        tcpRecvStr.append(QString::fromUtf8(array));
        if (tcpRecvStr.contains("$")) {
            MLOG_INFO("Socket Msg: " + tcpRecvStr);
            QString tmpStr = tcpRecvStr;
            tcpRecvStr.clear();

            if (tmpStr.contains("Register#")) {
                if (!tmpStr.contains("Register#1")) {
                    // Re-send the Register info.
                    tcpClientSendData(QString("Register#Fixture%1#%2#%3$")
                                          .arg(unitNum + offsetNum)
                                          .arg(unitNum + offsetNum)
                                          .arg(titleStr));
                }
            } else if (tmpStr.contains("Status$")) {
                if (isReady) {
                    tcpClientSendData("Status#1$");
                } else {
                    tcpClientSendData("Status#0$");
                }
            } else if (tmpStr.contains("Grip#1$")) {
                if (isReady) {
                    bool isClose = closeCylinder();
                    if (isClose) {
                        tcpClientSendData("Grip#1$");
                    } else {
                        tcpClientSendData("Grip#0$");
                    }

                } else {
                    tcpClientSendData("Grip#0$");
                }
            } else if (tmpStr.contains("Grip#0$")) {
                if (isReady) {
                    bool isOpen = openCylinder();
                    if (isOpen) {
                        tcpClientSendData("Grip#1$");
                    } else {
                        tcpClientSendData("Grip#0$");
                    }

                } else {
                    tcpClientSendData("Grip#0$");
                }
            } else if (tmpStr.contains("StartTest#")) {
                if (CFG_PARSE.getAuthority() == User::Invalid) {
                    QMessageBox::warning(NULL, tr("Warning"), tr("You can't start test, please login first!"));
                    return;
                }
                if (isReady) {
                    QList<QString> infoArray = tmpStr.split("#");
                    if (infoArray.count() < 7) {
                        return;
                    }

                    sn_ST = infoArray[1];
                    opID_ST = infoArray[2];
                    lotName_ST = infoArray[3];
                    productionMode_ST = infoArray[4];
                    siteID_ST = infoArray[5];
                    projectID_ST = infoArray[6];
                    projectID_ST = projectID_ST.replace("$", "");
                    trial_ST = infoArray[7];
                    trial_ST = trial_ST.replace("$", "");
                    if (lotName_ST.toStdString() != CFG_PARSE.getLotName()) {
                        int ret = -1;
                        emit getLotName(lotName_ST, ret);
                        if (ret == 0) {
                            CFG_PARSE.setTestInfo(KLotName, lotName_ST.toStdString());
                        } else {
                            return;
                        }
                    }

                    emit updateUIInfo(opID_ST, lotName_ST, productionMode_ST, siteID_ST, projectID_ST);

                    m_mtcpFilePath = CFG_PARSE.getLogPath() + "MtcpLog/" + CFG_PARSE.getLotName() + "/Unit"
                                     + std::to_string(unitNum + offsetNum) + "/" + sn_ST.toStdString() + "/"
                                     + QDateTime::currentDateTime().toString("yyyyMMddhhmmss").toStdString();

                    emit unitStart(unitNum, sn_ST, m_mtcpFilePath);

                    tcpClientSendData("StartTest#1$");
                } else {
                    tcpClientSendData("Status#0$");
                }
            } else if (tmpStr.contains("Result$")) {
                if (isReady) {
                    QString tcpCmd = QString("Result#%1#%2#%3#%4$").arg(result_R, csvInfo_R, failItems_R, testItems_R);
                    tcpClientSendData(tcpCmd);
                } else {
                    tcpClientSendData("Status#0$");
                }
            } else if (tmpStr.contains("ErrorCode$")) {
                if (errorMessage_H.length() == 0) {
                    tcpClientSendData("ErrorCode#0$");
                } else {
                }
            } else {
            }
        }
    });

    connect(tcpClient, &QTcpSocket::disconnected, [=]() {
        ishandlerConnected = false;
        std::vector<std::string> names = m_devices->getConnectFailedDevices();
        names.push_back("MainBoard");
        emit connectedDeviceSignal(names);
    });

    connect(this, SIGNAL(sendDataToServer(QString)), this, SLOT(tcpClientSendData(QString)));
    connect(this, SIGNAL(tcpDisconnected()), this, SLOT(tcpConnect()), Qt::BlockingQueuedConnection);

    tcpConnect();
    //    std::shared_ptr<std::thread> _thread = std::shared_ptr<std::thread>(new std::thread([&]() {
    //        while (true) {
    //            if (!ishandlerConnected) {
    //                emit tcpDisconnected();
    //            }
    //            QThread::sleep(10);
    //        }
    //    }));
    //    _thread->detach();
}

void TestAction::tcpConnect()
{
    QString name = "MainBoard";
    QString ip = "127.0.0.1";  //"169.254.1.200";
    qint16 port;
    port = 6050;
    if (unitNum == 0) {
        port = 6052;
    } else if (unitNum == 1) {
        port = 6053;
    } else {
        port = 6054;
    }
    tcpClient->connectToHost(QHostAddress(ip), port);
    //    tcpClient->waitForConnected(-1);
}

void TestAction::tcpClientSendData(const QString& msg)
{
    tcpClient->write(msg.toUtf8().data());
}

void TestAction::calPlusCntToHex(const QString& plusStr, QString& direction, QString& hex1, QString& hex2,
                                 QString& hex3, QString& hex4)
{
    int hexStrLength;
    int plusCnt = plusStr.toInt();
    if (plusCnt > 0) {
        direction = "0x1";
    } else {
        direction = "0x0";
    }

    QString plusHexStr = QString::number(abs(plusCnt), 16).right(8);
    hexStrLength = plusHexStr.length();
    if (hexStrLength) {
        for (int i = 0; i < 8 - hexStrLength; i++) {
            plusHexStr.prepend("0");
        }
    }
    hex1 = plusHexStr.mid(6, 2);
    hex2 = plusHexStr.mid(4, 2);
    hex3 = plusHexStr.mid(2, 2);
    hex4 = plusHexStr.mid(0, 2);
    hex1.prepend("0x");
    hex2.prepend("0x");
    hex3.prepend("0x");
    hex4.prepend("0x");
}

bool TestAction::openCylinder()
{
    QObject* deviceObject_open =
        (QObject*)m_devices->deviceDic[QString("TEST_Uart%1%2").arg(unitNum + offsetNum).arg("1")].value<void*>();

    QString response = "";
    QString tmpStr = "SET_pcb[0x1,0x83,0x5,0x0,0,0,0,0]";

    QMetaObject::invokeMethod(deviceObject_open, "sendDataWithResponse", Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QString, response), Q_ARG(QString, tmpStr), Q_ARG(float, 5));
    if (response.contains("[DONE]")) {
        return true;
    } else {
        return false;
    }
}

bool TestAction::closeCylinder()
{
    QObject* deviceObject_close =
        (QObject*)m_devices->deviceDic[QString("TEST_Uart%1%2").arg(unitNum + offsetNum).arg("1")].value<void*>();

    QString response = "";
    QString tmpStr = "SET_pcb[0x1,0x83,0x5,0x1,0,0,0,0]";

    QMetaObject::invokeMethod(deviceObject_close, "sendDataWithResponse", Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QString, response), Q_ARG(QString, tmpStr), Q_ARG(float, 5));

    if (response.contains("[DONE]")) {
        return true;
    } else {
        return false;
    }
}

void TestAction::MLOG_INFO(QString msg)
{
    emit updateLogViewUI(msg, unitNum);
    msg = QString("[Unit%1] ").arg(unitNum + offsetNum) + msg;
    LOG_INFO(msg.toStdString().c_str());
}

void TestAction::MLOG_FATAL(QString msg)
{
    emit updateLogViewUI(msg, unitNum);
    msg = QString("[Unit%1] ").arg(unitNum + offsetNum) + msg;
    LOG_FATAL(msg.toStdString().c_str());
}

void TestAction::MLOG_ERROR(QString msg)
{
    emit updateLogViewUI(msg, unitNum);
    msg = QString("[Unit%1] ").arg(unitNum + offsetNum) + msg;
    LOG_ERROR(msg.toStdString().c_str());
}

void TestAction::MLOG_WARN(QString msg)
{
    emit updateLogViewUI(msg, unitNum);
    msg = QString("[Unit%1] ").arg(unitNum + offsetNum) + msg;
    LOG_WARN(msg.toStdString().c_str());
}

void TestAction::MLOG_DEBUG(QString msg)
{
    emit updateLogViewUI(msg, unitNum);
    msg = QString("[Unit%1] ").arg(unitNum + offsetNum) + msg;
    LOG_DEBUG(msg.toStdString().c_str());
}

void TestAction::onGoHome()
{
    if (isReseting) {
        QMessageBox::warning(NULL, "Warning", "Reseting! Can not reset again!");
        return;
    }

    std::shared_ptr<std::thread> _thread = std::shared_ptr<std::thread>(new std::thread([&]() {
        isReseting = true;
        MLOG_INFO("Origin Button Clicked");
        QObject* deviceObject_Uart1 =
            (QObject*)m_devices->deviceDic[QString("TEST_Uart%1%2").arg(unitNum + offsetNum).arg("1")].value<void*>();
        QObject* deviceObject_Uart2 =
            (QObject*)m_devices->deviceDic[QString("TEST_Uart%1%2").arg(unitNum + offsetNum).arg("2")].value<void*>();

        QString response;

        // LD-X +5mm
        QMetaObject::invokeMethod(deviceObject_Uart2, "sendDataWithResponse", Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QString, response),
                                  Q_ARG(QString, "SET_pcb[0x1,0x82,0x2,0x1,0,0,0x61,0xa8]"), Q_ARG(float, 5));
        QThread::msleep(2000);

        // LD-Y
        QMetaObject::invokeMethod(deviceObject_Uart2, "sendDataWithResponse", Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QString, response), Q_ARG(QString, "SET_pcb[0x1,0x85,0x3,0x0,0,0,0,0]"),
                                  Q_ARG(float, 5));
        QThread::msleep(5000);

        // LD-Z
        QMetaObject::invokeMethod(deviceObject_Uart2, "sendDataWithResponse", Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QString, response), Q_ARG(QString, "SET_pcb[0x1,0x85,0x4,0x0,0,0,0,0]"),
                                  Q_ARG(float, 5));
        QThread::msleep(5000);

        // DUT-X
        QMetaObject::invokeMethod(deviceObject_Uart1, "sendDataWithResponse", Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QString, response), Q_ARG(QString, "SET_pcb[0x1,0x85,0x3,0x0,0,0,0,0]"),
                                  Q_ARG(float, 5));
        QThread::msleep(5000);

        // beta
        QMetaObject::invokeMethod(deviceObject_Uart1, "sendDataWithResponse", Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QString, response), Q_ARG(QString, "SET_pcb[0x1,0x85,0x2,0x0,0,0,0,0]"),
                                  Q_ARG(float, 5));
        QThread::msleep(5000);

        // gamma
        QMetaObject::invokeMethod(deviceObject_Uart1, "sendDataWithResponse", Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QString, response), Q_ARG(QString, "SET_pcb[0x1,0x85,0x4,0x0,0,0,0,0]"),
                                  Q_ARG(float, 5));
        QThread::msleep(5000);

        // LD-X
        QMetaObject::invokeMethod(deviceObject_Uart2, "sendDataWithResponse", Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QString, response), Q_ARG(QString, "SET_pcb[0x1,0x85,0x2,0x0,0,0,0,0]"),
                                  Q_ARG(float, 5));
        QThread::msleep(5000);

        // for automation pick up
        // LD-Z +5mm
        QMetaObject::invokeMethod(deviceObject_Uart2, "sendDataWithResponse", Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QString, response),
                                  Q_ARG(QString, "SET_pcb[0x1,0x82,0x4,0x1,0,0,0x61,0xa8]"), Q_ARG(float, 5));
        QThread::msleep(1000);

        if (unitNum == 0) {
            // LD-X -1.78mm
            QMetaObject::invokeMethod(deviceObject_Uart2, "sendDataWithResponse", Qt::BlockingQueuedConnection,
                                      Q_RETURN_ARG(QString, response),
                                      Q_ARG(QString, "SET_pcb[0x1,0x82,0x2,0,0,0,0x22,0xc4]"), Q_ARG(float, 5));
            qDebug() << "LD-X:" << response;
            // beta
            QMetaObject::invokeMethod(deviceObject_Uart1, "sendDataWithResponse", Qt::BlockingQueuedConnection,
                                      Q_RETURN_ARG(QString, response),
                                      Q_ARG(QString, "SET_pcb[0x1,0x82,0x2,0x1,0,0,0,0x58]"), Q_ARG(float, 5));

        } else if (unitNum == 1) {
            QMetaObject::invokeMethod(deviceObject_Uart2, "sendDataWithResponse", Qt::BlockingQueuedConnection,
                                      Q_RETURN_ARG(QString, response),
                                      Q_ARG(QString, "SET_pcb[0x1,0x82,0x2,0,0,0,0x2f,0x44]"), Q_ARG(float, 5));
            // beta
            QMetaObject::invokeMethod(deviceObject_Uart1, "sendDataWithResponse", Qt::BlockingQueuedConnection,
                                      Q_RETURN_ARG(QString, response),
                                      Q_ARG(QString, "SET_pcb[0x1,0x82,0x2,0x1,0,0,0,0x16]"), Q_ARG(float, 5));
        } else if (unitNum == 2) {
            QMetaObject::invokeMethod(deviceObject_Uart2, "sendDataWithResponse", Qt::BlockingQueuedConnection,
                                      Q_RETURN_ARG(QString, response),
                                      Q_ARG(QString, "SET_pcb[0x1,0x82,0x2,0x0,0,0,0x27,0x10]"), Q_ARG(float, 5));
            // beta
            QMetaObject::invokeMethod(deviceObject_Uart1, "sendDataWithResponse", Qt::BlockingQueuedConnection,
                                      Q_RETURN_ARG(QString, response),
                                      Q_ARG(QString, "SET_pcb[0x1,0x82,0x2,0x1,0,0,0,0x16]"), Q_ARG(float, 5));
        }
        QThread::msleep(1000);

        emit showInfo(tr("fixture %1 reset finished!").arg(unitNum + offsetNum));
        isReseting = false;
    }));
    _thread->detach();
}

void TestAction::onLoopStart(int _slot)
{
    if (_slot != unitNum)
        return;

    if (CFG_PARSE.getAuthority() == User::Invalid) {
        QMessageBox::warning(NULL, tr("Warning"), tr("You can't start test, please login first!"));
        return;
    }

    // todo 这里需要添加一些电机动作
    emit updateUIInfo(QString("Op123"), QString("Lot001"), QString("MP"), QString("RS"), QString("P1"));
    CFG_PARSE.setTestInfo(KLotName, "Lot001");
    m_mtcpFilePath = CFG_PARSE.getLogPath() + "MtcpLog/" + CFG_PARSE.getLotName() + "/Unit"
                     + std::to_string(unitNum + offsetNum) + "/SNASDFJY365GDTRE1/"
                     + QDateTime::currentDateTime().toString("yyyyMMddhhmmss").toStdString();
    emit unitStart(unitNum, "SNASDFJY365GDTRE1", m_mtcpFilePath);
}

void TestAction::onLoopStop(int _slot)
{
    if (_slot != unitNum)
        return;

    m_loopStopFlag = true;
}

void TestAction::onShowWarning(const QString& msg)
{
    MLOG_WARN(msg);
    emit stopLoopTestWhileError(unitNum);
    QMessageBox::warning(NULL, tr("Warning-Unit%1").arg(unitNum + 1), msg);
}

void TestAction::onShowInfo(const QString& msg)
{
    MLOG_INFO(msg);
    QMessageBox::information(NULL, tr("Information-Unit%1").arg(unitNum + 1), msg);
}
