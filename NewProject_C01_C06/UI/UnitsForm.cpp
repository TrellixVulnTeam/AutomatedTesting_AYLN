#include "UnitsForm.h"
#include "ui_UnitsForm.h"
#include <QDebug>
#include <QGridLayout>

UnitsForm::UnitsForm(QWidget* parent, qint16 _slot) : QWidget(parent), unitSlot(_slot), ui(new Ui::UnitsForm)
{
    ui->setupUi(this);
    singleDialog = new QDialog(this);
    QGridLayout* layout = new QGridLayout(singleDialog);
    layout->setContentsMargins(5, 5, 5, 5);
    snForm = new SNInputForm(singleDialog, _slot);
    connect(snForm, &SNInputForm::singleStart, this, &UnitsForm::singleStart);
    layout->addWidget(snForm, 0, 0);
    singleDialog->setLayout(layout);
    singleDialog->setWindowTitle("Single Test");
    singleDialog->setStyleSheet("QDialog{min-height:2em;min-width:25em;max-height:2em;max-width:25em;}");

    ui->label_handlerState->setText("disconnected");
    ui->label_handlerState->setStyleSheet("QLabel{color:#FF0000;}");
    //    ui->label_netStateTittle->setStyleSheet("QLabel{background:#FF0000;}");

    on_enableCheck_clicked(true);
    ui->snLab->setText("N/A");
    ui->timeLab->setText("0.0");
    testStatue = TEST_STATE::INITIALIZING;
    setTestStateStr();

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &UnitsForm::onTimerTimeout);

    m_mouseTimer = new QTimer(this);
    connect(m_mouseTimer, &QTimer::timeout, this, &UnitsForm::onMouseClick);
    connect(ui->originBtn, &QPushButton::clicked, [&]() {
        User::Authority authority = CFG_PARSE.getAuthority();
        if (authority != User::Invalid && authority != User::Operator)
            emit originBtnClicked();
        else
            QMessageBox::warning(
                this, tr("Warning"),
                tr("The current account has insufficient permissions. Please upgrade the permissions."));
    });
}

UnitsForm::~UnitsForm()
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }
    if (m_mouseTimer->isActive()) {
        m_mouseTimer->stop();
    }

    delete ui;
}

void UnitsForm::setUnitInfo(const UNIT_INFO& info)
{
    unitInfo = info;
    ui->unitName->setText(info.unitName);
    ui->TestPlanNameLab->setText(info.testplanName);
}

bool UnitsForm::getEnable() const
{
    return ui->enableCheck->checkState();
}

void UnitsForm::on_enableCheck_clicked(bool checked)
{
    if (checked) {
        setStyleSheet("");
        ui->unitGroupBox->setStyleSheet("#unitGroupBox{border: 2px solid #D5BEB2;}");
    } else {
        setStyleSheet("color: grey;border-radius: 16px;");
        ui->unitGroupBox->setStyleSheet("#unitGroupBox{border: 2px solid grey;}");
    }
    ui->enableCheck->setStyleSheet("color: black");
}

void UnitsForm::scanSN(const QString& SN)
{
    sn = SN;
    ui->snLab->setText(sn);
    ui->timeLab->setText("0.0");
    testStatue = TEST_STATE::READY;
    setTestStateStr();
}

QString UnitsForm::getSN() const
{
    return sn.isEmpty() ? "N/A" : sn;
}

void UnitsForm::setTestStateStr()
{
    switch (testStatue) {
    case TEST_STATE::INITIALIZING:
        ui->stateLab->setText("Initializing");
        break;
    case TEST_STATE::READY:
        ui->stateLab->setText("Ready");
        setStyleSheet("background-color: #ccffcc");
        break;
    case TEST_STATE::TESTING:
        ui->stateLab->setText("Testing");
        setStyleSheet("background-color: #fdf4bf");
        break;
    case TEST_STATE::FINISHED:
        ui->stateLab->setText("Finished");
        break;
    default:
        ui->stateLab->setText("Some Errors Happened");
    }
}

void UnitsForm::setTestState(TEST_STATE state)
{
    testStatue = state;
    setTestStateStr();
}

TEST_STATE UnitsForm::getTestState() const
{
    return testStatue;
}

void UnitsForm::onReseting()
{
    ui->enableCheck->setEnabled(false);
}

void UnitsForm::finishReset()
{
    ui->enableCheck->setEnabled(true);
}

void UnitsForm::onTestStart(const QString& SN)
{
    ui->enableCheck->setEnabled(false);
    ui->timeLab->setText("0.0");
    if (!getEnable()) {
        return;
    }

    if (SN == "") {
        if (sn.isEmpty()) {
            throw std::runtime_error("Can't start test with empty sn.");
        }
    }

    if (m_timer->isActive()) {
        m_timer->stop();
    }
    initialTime();
    m_timer->start(100);

    testStatue = TEST_STATE::TESTING;
    setTestStateStr();

    emit testStart(unitInfo, sn);  // start signal
}

void UnitsForm::onTestEnd(TEST_RESULT result)
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }

    tcnt++;
    if (result == TEST_PASS) {
        pcnt++;
        setStyleSheet("background-color: #00FF00");
    } else
        setStyleSheet("background-color: #ff5c64");

    ui->PTcnt->setText(QString("%1/%2").arg(pcnt).arg(tcnt));

    testStatue = TEST_STATE::FINISHED;
    setTestStateStr();

    onTimerTimeout();
    sn.clear();
    ui->enableCheck->setEnabled(true);
}

void UnitsForm::onTimerTimeout()
{
    m_endTime = QDateTime::currentDateTime();
    correctTime();
}

void UnitsForm::initialTime()
{
    m_endTime = m_startTime = QDateTime::currentDateTime();
}

void UnitsForm::correctTime()
{
    qint64 duration = m_startTime.msecsTo(m_endTime);
    const QString timeStr = QString::number(duration / 1000) + "." + QString::number(duration % 1000) + " s";
    ui->timeLab->setText(timeStr);
}

void UnitsForm::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    m_mouseTimer->start(300);
}

void UnitsForm::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    m_mouseTimer->stop();

    User::Authority authority = CFG_PARSE.getAuthority();
    if (authority != User::Invalid)
        emit doubleMouseClick(unitSlot);
}

void UnitsForm::onMouseClick()
{
    m_mouseTimer->stop();
    User::Authority authority = CFG_PARSE.getAuthority();
    if (authority != User::Invalid)
        if (testStatue != TESTING && getEnable())
            singleDialog->exec();
}

void UnitsForm::clearSN()
{
    snForm->clearSN();
    singleDialog->close();
}

void UnitsForm::onConnectedDevice(const std::vector<std::string>& names)
{
    std::string str;
    if (names.size() == 0) {
        str = "connected";
    } else {
        foreach (std::string name, names) {
            str += name;
            str += " disconnected;";
        }
    }

    updateNetState(QString::fromStdString(str));
}

void UnitsForm::updateNetState(const QString& str)
{
    ui->label_handlerState->setText(str);
    if ("connected" == str) {
        ui->label_handlerState->setStyleSheet("QLabel{color:#00FF00;}");
    } else {
        ui->label_handlerState->setStyleSheet("QLabel{color:#FF0000;}");
    }
}
