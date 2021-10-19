#include "MotorDialog.h"
#include "ui_MotorDialog.h"
#include <QDateTime>
#include <QDebug>
#include <QShowEvent>
#include <QMessageBox>

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
    resize(1560, 950);

    connect(this, &MotorDialog::appendLogTextEdit, this, &MotorDialog::onAppendLogTextEdit);

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
}

MotorDialog::~MotorDialog()
{
    delete ui;
}

void MotorDialog::onAppendLogTextEdit(QString msg)
{
    msg = msg.replace("\r\n", "");
    msg = msg.replace("\n", "");
    ui->logTextEdit->moveCursor(QTextCursor::End);
    ui->logTextEdit->setTextColor(Qt::blue);
    ui->logTextEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz "));
    if (msg.contains("error", Qt::CaseInsensitive) || msg.contains("critical", Qt::CaseInsensitive)
        || msg.contains("failed", Qt::CaseInsensitive)) {
        ui->logTextEdit->moveCursor(QTextCursor::End);
        ui->logTextEdit->setTextColor(Qt::red);
        ui->logTextEdit->insertPlainText(msg);
    } else {
        ui->logTextEdit->moveCursor(QTextCursor::End);
        ui->logTextEdit->setTextColor(Qt::black);
        ui->logTextEdit->insertPlainText(msg);
    }
}

void MotorDialog::onModuleBtnClicked(BtnType btnType)
{
    switch (btnType) {
    case XPlusBtn:
        emit appendLogTextEdit("XPlusBtn clicked");
        break;
    case XMinusBtn:
        emit appendLogTextEdit("XMinusBtn clicked");
        break;
    case YPlusBtn:
        emit appendLogTextEdit("YPlusBtn clicked");
        break;
    case YMinusBtn:
        emit appendLogTextEdit("YMinusBtn clicked");
        break;
    case ZPlusBtn:
        emit appendLogTextEdit("ZPlusBtn clicked");
        break;
    case ZMinusBtn:
        emit appendLogTextEdit("ZMinusBtn clicked");
        break;
    case X1PlusBtn:
        emit appendLogTextEdit("X1PlusBtn clicked");
        break;
    case X1MinusBtn:
        emit appendLogTextEdit("X1MinusBtn clicked");
        break;
    case BPlusBtn:
        emit appendLogTextEdit("BPlusBtn clicked");
        break;
    case BMinusBtn:
        emit appendLogTextEdit("BMinusBtn clicked");
        break;
    case GPlusBtn:
        emit appendLogTextEdit("GPlusBtn clicked");
        break;
    case GMinusBtn:
        emit appendLogTextEdit("GMinusBtn clicked");
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
    //        emit appendLogTextEdit(res);

    //        if (res.contains("[DONE]\r\n")) {
    //            QMetaObject::invokeMethod(m_uart2, "sendDataWithResponse", Qt::BlockingQueuedConnection,
    //                                      Q_RETURN_ARG(QString, res), Q_ARG(QString, "2222222222"), Q_ARG(float, 5),
    //                                      Q_ARG(QString, "[DONE]\r\n"));
    //            emit appendLogTextEdit(res);
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

void MotorDialog::on_va1Btn_clicked() {}

void MotorDialog::on_va2Btn_clicked() {}

void MotorDialog::on_setCMBtn_clicked() {}

void MotorDialog::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);
}
