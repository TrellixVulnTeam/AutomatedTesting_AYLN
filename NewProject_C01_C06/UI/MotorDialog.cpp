#include "MotorDialog.h"
#include "ui_MotorDialog.h"
#include <QDateTime>
#include <QDebug>
#include <QShowEvent>
#include <QMessageBox>
#include <QPushButton>

#define U_CKECK()                                                                                              \
    if (m_uart1 == NULL || m_uart2 == NULL) {                                                                  \
        QMessageBox::warning(this, tr("Warning"),                                                              \
                             tr("Device 1 or device 2 is disconnected. Check the connection and try again.")); \
        return;                                                                                                \
    }

MotorDialog::MotorDialog(ConfigDevice* device, int slot, QWidget* parent)
    : m_devices(device), m_slot(slot), QDialog(parent), ui(new Ui::MotorDialog)
{
    ui->setupUi(this);
    setWindowTitle(QString("Unit-%1").arg(m_slot) + " Motor Dialog");
    //    Qt::WindowFlags windowFlag = Qt::Dialog;
    //    windowFlag |= Qt::WindowMinMaxButtonsHint;
    //    windowFlag |= Qt::WindowCloseButtonHint;
    //    setWindowFlags(windowFlag);

    resize(m_maxW, m_maxH);
    connect(this, &MotorDialog::buttonClickEvent, this, &MotorDialog::onButtonClickEvent);

    m_lightSourceW = new SportsWidget(LightSource, this);
    connect(m_lightSourceW, &SportsWidget::moduleBtnClicked, this, &MotorDialog::onModuleBtnClicked);
    ui->lightSourceLay->setContentsMargins(0, 0, 30, 0);
    ui->lightSourceLay->setSpacing(0);
    ui->lightSourceLay->addWidget(m_lightSourceW);

    m_productW = new SportsWidget(Product, this);
    connect(m_productW, &SportsWidget::moduleBtnClicked, this, &MotorDialog::onModuleBtnClicked);
    ui->productLay->setContentsMargins(0, 0, 30, 0);
    ui->productLay->setSpacing(0);
    ui->productLay->addWidget(m_productW);

    m_uart1 = (QObject*)m_devices->deviceDic[QString("TEST_Uart%2%3").arg(m_slot).arg("1")].value<void*>();
    m_uart2 = (QObject*)m_devices->deviceDic[QString("TEST_Uart%2%3").arg(m_slot).arg("2")].value<void*>();
    m_tcp = (QObject*)m_devices->deviceDic[QString("TEST_Socket%1").arg(m_slot)].value<void*>();

}

MotorDialog::~MotorDialog()
{
    delete ui;
}

void MotorDialog::onButtonClickEvent(QString msg)
{
    QString axis;
    QString value;
    QString direction;
    QString cmdStr;
    QString responseStr;
    QString suffix = "[DONE]\r\n";
    QString logInfo;
    QList<QString> infoList = msg.split(",");
    axis = infoList[0];
    direction = infoList[1];
    value = QString("%1").arg(ui->stepValueSpinBox->value());
    cmdStr = convertCmd(axis,value,direction);

    if(axis.contains("Xs") || axis.contains("Y") || axis.contains("Z"))
    {
        QMetaObject::invokeMethod(m_uart2, "sendDataWithResponse", Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QString, responseStr), Q_ARG(QString, cmdStr),
                                  Q_ARG(float, 3), Q_ARG(QString, suffix));
    }
    else
    {
        QMetaObject::invokeMethod(m_uart1, "sendDataWithResponse", Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QString, responseStr), Q_ARG(QString, cmdStr),
                                  Q_ARG(float, 3), Q_ARG(QString, suffix));
    }

    logInfo = QString("%1:%2").arg(axis).arg(responseStr);
    logInfo = logInfo.replace("\r\n", "");
    logInfo = logInfo.replace("\n", "");
    ui->logTextEdit->moveCursor(QTextCursor::End);
    ui->logTextEdit->setTextColor(Qt::blue);
    ui->logTextEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz "));
    if (msg.contains("error", Qt::CaseInsensitive) || msg.contains("critical", Qt::CaseInsensitive)
        || msg.contains("failed", Qt::CaseInsensitive)) {
        ui->logTextEdit->moveCursor(QTextCursor::End);
        ui->logTextEdit->setTextColor(Qt::red);
        ui->logTextEdit->insertPlainText(logInfo);
    } else {
        ui->logTextEdit->moveCursor(QTextCursor::End);
        ui->logTextEdit->setTextColor(Qt::black);
        ui->logTextEdit->insertPlainText(logInfo);
    }
}

void MotorDialog::onModuleBtnClicked(BtnType btnType)
{
    switch (btnType) {
    case XPlusBtn:
        emit buttonClickEvent("Xs,0x1");
        break;
    case XMinusBtn:
        emit buttonClickEvent("Xs,0x0");
        break;
    case YPlusBtn:
        emit buttonClickEvent("Y,0x0");
        break;
    case YMinusBtn:
        emit buttonClickEvent("Y,0x1");
        break;
    case ZPlusBtn:
        emit buttonClickEvent("Z,0x1");
        break;
    case ZMinusBtn:
        emit buttonClickEvent("Z,0x0");
        break;
    case X1PlusBtn:
        emit buttonClickEvent("Xp,0x0");
        break;
    case X1MinusBtn:
        emit buttonClickEvent("Xp,0x1");
        break;
    case BPlusBtn:
        emit buttonClickEvent("beta,0x0");
        break;
    case BMinusBtn:
        emit buttonClickEvent("beta,0x1");
        break;
    case GPlusBtn:
        emit buttonClickEvent("gamma,0x1");
        break;
    case GMinusBtn:
        emit buttonClickEvent("gamma,0x0");
        break;
    default:
        break;
    }
}

void MotorDialog::on_homeingBtn_clicked()
{
    // Example

    //    U_CKECK();
    //    std::shared_ptr<std::thread> _thread = std::shared_ptr<std::thread>(new std::thread([&]() {
    //        QString res;
    //        QMetaObject::invokeMethod(m_uart1, "sendDataWithResponse", Qt::BlockingQueuedConnection,
    //                                  Q_RETURN_ARG(QString, res), Q_ARG(QString, "11111111111111"), Q_ARG(float, 5),
    //                                  Q_ARG(QString, "[DONE]\r\n"));
    //        emit buttonClickEvent(res);

    //        if (res.contains("[DONE]\r\n")) {
    //            QMetaObject::invokeMethod(m_uart2, "sendDataWithResponse", Qt::BlockingQueuedConnection,
    //                                      Q_RETURN_ARG(QString, res), Q_ARG(QString, "2222222222"), Q_ARG(float, 5),
    //                                      Q_ARG(QString, "[DONE]\r\n"));
    //            emit buttonClickEvent(res);
    //        }
    //    }));
    //    _thread->detach();
}

void MotorDialog::on_startBtn_clicked() {}

void MotorDialog::on_pauseBtn_clicked() {}

void MotorDialog::on_stopBtn_clicked() {}

void MotorDialog::on_xHomeBtn_clicked() {}

void MotorDialog::on_yHomeBtn_clicked() {}

void MotorDialog::on_zHomeBtn_clicked() {}

void MotorDialog::on_x1HomeBtn_clicked() {}

void MotorDialog::on_gHomeBtn_clicked() {}

void MotorDialog::on_bHomeBtn_clicked() {}

void MotorDialog::on_aHomeBtn_clicked() {}

void MotorDialog::on_homeBtn_clicked() {}

void MotorDialog::on_resetBtn_clicked() {}

void MotorDialog::on_aBtn_clicked() {}

void MotorDialog::on_clipBtn_clicked() {}

void MotorDialog::on_sendBtn_clicked() {}

void MotorDialog::on_readBtn_clicked() {}

void MotorDialog::on_DCBtn_clicked() {}

void MotorDialog::on_va1Btn_clicked()
{

}

void MotorDialog::on_va2Btn_clicked() {}

void MotorDialog::on_setCMBtn_clicked() {}

void MotorDialog::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);
    if (NULL == m_animation) {

        m_animation = new QPropertyAnimation(this, "windowOpacity");
        connect(m_animation, &QPropertyAnimation::finished, [&]() {
            if (m_groupAnimations.size() == 0) {
                QPropertyAnimation* ani = new QPropertyAnimation(ui->groupBox_control, "geometry");
                ani->setDuration(1000);
                ani->setStartValue(QRect(ui->groupBox_control->geometry().x(), -ui->groupBox_control->height(),
                                         ui->groupBox_control->width(), ui->groupBox_control->height()));
                ani->setEndValue(QRect(ui->groupBox_control->geometry()));
                ani->setEasingCurve(QEasingCurve::OutBounce);
                ani->start();
                m_groupAnimations.push_back(ani);

                QPropertyAnimation* ani2 = new QPropertyAnimation(ui->groupBox_move, "geometry");
                ani2->setDuration(1000);
                ani2->setStartValue(QRect(ui->groupBox_move->geometry().x(), -ui->groupBox_move->height(),
                                          ui->groupBox_move->width(), ui->groupBox_move->height()));
                ani2->setEndValue(QRect(ui->groupBox_move->geometry()));
                ani2->setEasingCurve(QEasingCurve::OutBounce);
                ani2->start();
                m_groupAnimations.push_back(ani2);
            } else {
                foreach (QPropertyAnimation* ani, m_groupAnimations) {
                    ani->start();
                }
            }
        });

        m_animation->setDuration(1000);
        m_animation->setStartValue(0);
        m_animation->setEndValue(0.95);
        m_animation->start();
    } else {
        m_animation->start();
    }
}


QString MotorDialog::convertCmd(QString axis, QString value, QString& direction)
{
    QString plusStr;
    QString plus_direction;
    QString plus_hex1;
    QString plus_hex2;
    QString plus_hex3;
    QString plus_hex4;

    int plusCnt = 0;
    float distance = 0;
    float degree = 0;

    plus_direction = direction;

    if (axis.contains("Xs")) {
        distance = value.toFloat();
        plusCnt = (int)CFG_PARSE.getXs() * distance;

        if (abs(plusCnt) != 0) {
            plusStr = QString("%1").arg(plusCnt);
            calPlusCntToHex(plusStr, plus_hex1, plus_hex2, plus_hex3, plus_hex4);
            plusStr = QString("%1,%2,%3,%4,%5,%6]")
                          .arg("SET_pcb[0x1,0x82,0x2")
                          .arg(plus_direction)
                          .arg(plus_hex4)
                          .arg(plus_hex3)
                          .arg(plus_hex2)
                          .arg(plus_hex1);
        } else {
            plusStr = "";
        }
    } else if (axis.contains("Y")) {
        distance = value.toFloat();
        plusCnt = -(int)CFG_PARSE.getY() * distance;

        if (abs(plusCnt) != 0) {
            plusStr = QString("%1").arg(plusCnt);
            calPlusCntToHex(plusStr, plus_hex1, plus_hex2, plus_hex3, plus_hex4);
            plusStr = QString("%1,%2,%3,%4,%5,%6]")
                          .arg("SET_pcb[0x1,0x82,0x3")
                          .arg(plus_direction)
                          .arg(plus_hex4)
                          .arg(plus_hex3)
                          .arg(plus_hex2)
                          .arg(plus_hex1);
        } else {
            plusStr = "";
        }
    } else if (axis.contains("Z")) {
        distance = value.toFloat();
        plusCnt = (int)CFG_PARSE.getZ() * distance;

        if (abs(plusCnt) != 0) {
            plusStr = QString("%1").arg(plusCnt);
            calPlusCntToHex(plusStr, plus_hex1, plus_hex2, plus_hex3, plus_hex4);
            plusStr = QString("%1,%2,%3,%4,%5,%6]")
                          .arg("SET_pcb[0x1,0x82,0x4")
                          .arg(plus_direction)
                          .arg(plus_hex4)
                          .arg(plus_hex3)
                          .arg(plus_hex2)
                          .arg(plus_hex1);
        } else {
            plusStr = "";
        }

    } else if (axis.contains("Xp")) {
        distance = value.toFloat();
        plusCnt = -(int)CFG_PARSE.getXp() * distance / 0.25;
        if (abs(plusCnt) != 0) {
            plusStr = QString("%1").arg(plusCnt);
            calPlusCntToHex(plusStr, plus_hex1, plus_hex2, plus_hex3, plus_hex4);
            plusStr = QString("%1,%2,%3,%4,%5,%6]")
                          .arg("SET_pcb[0x1,0x82,0x3")
                          .arg(plus_direction)
                          .arg(plus_hex4)
                          .arg(plus_hex3)
                          .arg(plus_hex2)
                          .arg(plus_hex1);
        } else {
            plusStr = "";
        }
    } else if (axis.contains("beta")) {
        degree = value.toFloat();
        plusCnt = -(int)CFG_PARSE.getBeta() * degree / 0.45;

        if (abs(plusCnt) != 0) {
            plusStr = QString("%1").arg(plusCnt);
            calPlusCntToHex(plusStr, plus_hex1, plus_hex2, plus_hex3, plus_hex4);
            plusStr = QString("%1,%2,%3,%4,%5,%6]")
                          .arg("SET_pcb[0x1,0x82,0x2")
                          .arg(plus_direction)
                          .arg(plus_hex4)
                          .arg(plus_hex3)
                          .arg(plus_hex2)
                          .arg(plus_hex1);
        } else {
            plusStr = "";
        }

    } else if (axis.contains("gamma")) {
        degree = value.toFloat();
        plusCnt = (int)CFG_PARSE.getGamma() * degree / 0.55;

        if (abs(plusCnt) != 0) {
            plusStr = QString("%1").arg(plusCnt);
            calPlusCntToHex(plusStr, plus_hex1, plus_hex2, plus_hex3, plus_hex4);
            plusStr = QString("%1,%2,%3,%4,%5,%6]")
                          .arg("SET_pcb[0x1,0x82,0x4")
                          .arg(plus_direction)
                          .arg(plus_hex4)
                          .arg(plus_hex3)
                          .arg(plus_hex2)
                          .arg(plus_hex1);

        } else {
            plusStr = "";
        }

    } else {
        plusStr = "";
    }
    return plusStr;
}


void MotorDialog::calPlusCntToHex(const QString& plusStr, QString& hex1, QString& hex2,
                                 QString& hex3, QString& hex4)
{
    int hexStrLength;
    int plusCnt = plusStr.toInt();
//    if (plusCnt > 0) {
//        direction = "0x1";
//    } else {
//        direction = "0x0";
//    }

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

void MotorDialog::on_SpeedCombobox_currentIndexChanged(int index)
{
    if(index == 0)
    {
        ui->stepValueSpinBox->setSingleStep(0.001);
        ui->stepValueSpinBox->setMaximum(0.01);
        ui->stepValueSpinBox->setValue(0.001);
    }
    else
    {
        ui->stepValueSpinBox->setSingleStep(0.1);
        ui->stepValueSpinBox->setMaximum(1.0);
        ui->stepValueSpinBox->setValue(0.1);
    }
}
