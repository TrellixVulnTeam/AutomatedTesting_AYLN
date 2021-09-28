#include "LoopTestWidget.h"
#include "ui_LoopTestWidget.h"
#include <QRegExp>
#include <QRegExpValidator>
#include <QMessageBox>
#include <QTimer>

LoopTestWidget::LoopTestWidget(QWidget* parent, int _slot) : QWidget(parent), m_slot(_slot), ui(new Ui::LoopTestWidget)
{
    ui->setupUi(this);
    ui->groupBox->setTitle(QString("Unit%1").arg(m_slot + 1));
    //    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    //    setWindowTitle(tr("LoopTest"));
    QRegExp regx("^([1-9]\\d{0,4}|100000)");
    ui->loopCountLineEdit->setValidator(new QRegExpValidator(regx));
    ui->timeIntervalLineEdit->setValidator(new QRegExpValidator(regx));
    connect(ui->loopBtn, &QPushButton::clicked, this, &LoopTestWidget::onLoop);
}

LoopTestWidget::~LoopTestWidget()
{
    delete ui;
}

void LoopTestWidget::onTestStart(int _slot)
{
    if (m_slot != _slot)
        return;

    if (m_currentCount != m_loopCount) {
        ui->loopBtn->setEnabled(true);
        ui->loopBtn->setText(tr("Loop Stop"));
        m_loopFlag = true;
    }
}

void LoopTestWidget::onTestFinished(int _slot)
{
    if (m_slot != _slot)
        return;

    ++m_currentCount;
    if (m_currentCount <= m_loopCount) {
        ui->currentLoopLineEdit->setText(QString("%1").arg(m_currentCount));
        int timeDelay = ui->timeIntervalLineEdit->text().toInt();
        QTimer::singleShot(timeDelay, this, [&]() { emit loopStart(m_slot); });
    } else {
        setEditable(true);
        ui->loopBtn->setEnabled(true);
        ui->loopBtn->setText(tr("Loop Start"));
        m_loopFlag = false;
    }
}

void LoopTestWidget::onStopLoopTestWhileError(int _slot)
{
    if (m_slot != _slot)
        return;

    if (m_loopFlag) {
        emit loopStop(m_slot);
        setEditable(true);
        m_currentCount = m_loopCount = 0;

        QMessageBox::warning(this, "Waring", "Some error occurred and the loop test was stopped.");
    }
}

void LoopTestWidget::onLoop()
{
    if (m_loopFlag) {
        emit loopStop(m_slot);
        setEditable(true);
        m_currentCount = m_loopCount = 0;
    } else {
        if (ui->loopCountLineEdit->text().isEmpty() || ui->timeIntervalLineEdit->text().isEmpty()) {
            QMessageBox::information(this, tr("hint"), tr("Please input Loop Count or IntervalTime! "));
            return;
        }

        m_currentCount = 1;
        m_loopCount = ui->loopCountLineEdit->text().toInt();
        ui->currentLoopLineEdit->setText(QString::number(m_currentCount));

        emit loopStart(m_slot);
        setEditable(false);
    }
}

void LoopTestWidget::setEditable(bool enable)
{
    ui->loopCountLineEdit->setEnabled(enable);
    ui->timeIntervalLineEdit->setEnabled(enable);
    ui->loopBtn->setEnabled(true);
}

void LoopTestWidget::clickLanguageAction(QTranslator& tran)
{
    qApp->installTranslator(&tran);
    ui->retranslateUi(this);
    setWindowTitle(tr("LoopTest"));
    ui->loopBtn->setText(tr("Loop Start"));
}
