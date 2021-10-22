#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPropertyAnimation>
#include "ConfigParse.h"

PreferencesDialog::PreferencesDialog(QWidget* parent) : QDialog(parent), ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);  // remove title bar
    this->setStyleSheet("QDialog{min-height:25em;min-width:35em;border: 2px solid rgb(216, 214, 209);}");
    connect(ui->okBtn, &QPushButton::clicked, this, &PreferencesDialog::onOk);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &PreferencesDialog::onCancel);

    this->initPreferencesDialogFormConfig();
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::onOk()
{
    checkPreferencesSettingIsChange();

    this->close();
    return;
}

bool PreferencesDialog::checkPreferencesSettingIsChange()
{
    bool isChange = false;

    QString text = ui->AlphaLineEdit->text();
    if (text.toInt() != CFG_PARSE.getAlpha()) {
        CFG_PARSE.setTestInfo(KAlpha, text.toInt());
        isChange = true;
    }

    text = ui->BetaLineEdit->text();
    if (text.toInt() != CFG_PARSE.getBeta()) {
        CFG_PARSE.setTestInfo(KBeta, text.toInt());
        isChange = true;
    }

    text = ui->GammaLineEdit->text();
    if (text.toInt() != CFG_PARSE.getGamma()) {
        CFG_PARSE.setTestInfo(KGamma, text.toInt());
        isChange = true;
    }

    text = ui->XpLineEdit->text();
    if (text.toInt() != CFG_PARSE.getXp()) {
        CFG_PARSE.setTestInfo(KXp, text.toInt());
        isChange = true;
    }

    text = ui->XsLineEdit->text();
    if (text.toInt() != CFG_PARSE.getXs()) {
        CFG_PARSE.setTestInfo(KXs, text.toInt());
        isChange = true;
    }

    text = ui->YLineEdit->text();
    if (text.toInt() != CFG_PARSE.getY()) {
        CFG_PARSE.setTestInfo(KY, text.toInt());
        isChange = true;
    }

    text = ui->ZLineEdit->text();
    if (text.toInt() != CFG_PARSE.getZ()) {
        CFG_PARSE.setTestInfo(KZ, text.toInt());
        isChange = true;
    }

    text = ui->TimeStepLineEdit->text();
    if (text.toDouble() != CFG_PARSE.getTimeStep()) {
        CFG_PARSE.setTestInfo(KTimeStep, text.toDouble());
        isChange = true;
    }

    bool check = ui->MtcpCheckBox->isChecked();
    QString ip = ui->MtcpIPLineEdit->text();
    QString port = ui->MtcpPortLineEdit->text();
    if (ip.toStdString() != CFG_PARSE.getMtcpIP()) {
        CFG_PARSE.setTestInfo(KMtcpIP, ip.toStdString());
        isChange = true;
        if (check && !CFG_PARSE.getMtcpPort().empty()) {
            emit mtcpStatusChanged(check);
        }
    }

    if (port.toStdString() != CFG_PARSE.getMtcpPort()) {
        CFG_PARSE.setTestInfo(KMtcpPort, port.toStdString());
        isChange = true;
        if (check && !CFG_PARSE.getMtcpIP().empty()) {
            emit mtcpStatusChanged(check);
        }
    }

    if (check != CFG_PARSE.getMtcp()) {
        CFG_PARSE.setTestInfo(KMtcp, check);
        emit mtcpStatusChanged(check);
    }

    check = ui->MesCheckBox->isChecked();
    if (check != CFG_PARSE.getMes()) {
        CFG_PARSE.setTestInfo(KMes, check);
    }

    return isChange;
}
void PreferencesDialog::onCancel()
{
    initPreferencesDialogFormConfig();
    this->close();
    return;
}

void PreferencesDialog::initPreferencesDialogFormConfig()
{
    ui->AlphaLineEdit->setText(QString::number(CFG_PARSE.getAlpha()));
    ui->BetaLineEdit->setText(QString::number(CFG_PARSE.getBeta()));
    ui->GammaLineEdit->setText(QString::number(CFG_PARSE.getGamma()));
    ui->XpLineEdit->setText(QString::number(CFG_PARSE.getXp()));
    ui->XsLineEdit->setText(QString::number(CFG_PARSE.getXs()));
    ui->YLineEdit->setText(QString::number(CFG_PARSE.getY()));
    ui->ZLineEdit->setText(QString::number(CFG_PARSE.getZ()));
    ui->TimeStepLineEdit->setText(QString::number(CFG_PARSE.getTimeStep()));
    ui->MtcpIPLineEdit->setText(QString::fromStdString(CFG_PARSE.getMtcpIP()));
    ui->MtcpPortLineEdit->setText(QString::fromStdString(CFG_PARSE.getMtcpPort()));

    ui->MtcpCheckBox->setChecked(CFG_PARSE.getMtcp());
    ui->MesCheckBox->setChecked(CFG_PARSE.getMes());
}

void PreferencesDialog::clickLanguageAction(QTranslator& tran)
{
    qApp->installTranslator(&tran);
    ui->retranslateUi(this);
}

void PreferencesDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);

    QPropertyAnimation* animation = new QPropertyAnimation(this, "geometry");
    animation->setStartValue(QRect(-this->width(), -this->height(), this->width(), this->height()));
    animation->setEndValue(QRect(this->geometry()));
    //    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}
