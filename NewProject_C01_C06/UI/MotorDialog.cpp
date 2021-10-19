#include "MotorDialog.h"
#include "ui_MotorDialog.h"
#include <QDateTime>

MotorDialog::MotorDialog(QWidget* parent) : QDialog(parent), ui(new Ui::MotorDialog)
{
    ui->setupUi(this);
    Qt::WindowFlags windowFlag = Qt::Dialog;
    windowFlag |= Qt::WindowMinMaxButtonsHint;
    windowFlag |= Qt::WindowCloseButtonHint;
    setWindowFlags(windowFlag);
    resize(1560, 950);

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

    //    setStyleSheet("QDialog{min-height:10em;min-width:10em;max-height:34em;max-width:34em;}");
}

MotorDialog::~MotorDialog()
{
    delete ui;
}

void MotorDialog::appendLogTextEdit(const QString& msg)
{
    ui->logTextEdit->moveCursor(QTextCursor::End);
    ui->logTextEdit->setTextColor(Qt::blue);
    ui->logTextEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz "));
    if (msg.contains("error", Qt::CaseInsensitive) || msg.contains("critical", Qt::CaseInsensitive)) {
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
        appendLogTextEdit("XPlusBtn clicked");
        break;
    case XMinusBtn:
        appendLogTextEdit("XMinusBtn clicked");
        break;
    case YPlusBtn:
        appendLogTextEdit("YPlusBtn clicked");
        break;
    case YMinusBtn:
        appendLogTextEdit("YMinusBtn clicked");
        break;
    case ZPlusBtn:
        appendLogTextEdit("ZPlusBtn clicked");
        break;
    case ZMinusBtn:
        appendLogTextEdit("ZMinusBtn clicked");
        break;
    case X1PlusBtn:
        appendLogTextEdit("X1PlusBtn clicked");
        break;
    case X1MinusBtn:
        appendLogTextEdit("X1MinusBtn clicked");
        break;
    case BPlusBtn:
        appendLogTextEdit("BPlusBtn clicked");
        break;
    case BMinusBtn:
        appendLogTextEdit("BMinusBtn clicked");
        break;
    case GPlusBtn:
        appendLogTextEdit("GPlusBtn clicked");
        break;
    case GMinusBtn:
        appendLogTextEdit("GMinusBtn clicked");
        break;
    default:
        break;
    }
}

void MotorDialog::on_homeingBtn_clicked() {}

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
