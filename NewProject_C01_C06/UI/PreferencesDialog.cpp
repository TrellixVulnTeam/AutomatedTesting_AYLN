#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"
#include <QFileDialog>
#include <QMessageBox>
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
    if (text.toInt() != ConfigParse::getInstance().getAlpha()) {
        ConfigParse::getInstance().setTestInfo(KAlpha, text.toInt());
        isChange = true;
    }

    text = ui->BetaLineEdit->text();
    if (text.toInt() != ConfigParse::getInstance().getBeta()) {
        ConfigParse::getInstance().setTestInfo(KBeta, text.toInt());
        isChange = true;
    }

    text = ui->GammaLineEdit->text();
    if (text.toInt() != ConfigParse::getInstance().getGamma()) {
        ConfigParse::getInstance().setTestInfo(KGamma, text.toInt());
        isChange = true;
    }

    text = ui->XpLineEdit->text();
    if (text.toInt() != ConfigParse::getInstance().getXp()) {
        ConfigParse::getInstance().setTestInfo(KXp, text.toInt());
        isChange = true;
    }

    text = ui->XsLineEdit->text();
    if (text.toInt() != ConfigParse::getInstance().getXs()) {
        ConfigParse::getInstance().setTestInfo(KXs, text.toInt());
        isChange = true;
    }

    text = ui->YLineEdit->text();
    if (text.toInt() != ConfigParse::getInstance().getY()) {
        ConfigParse::getInstance().setTestInfo(KY, text.toInt());
        isChange = true;
    }

    text = ui->ZLineEdit->text();
    if (text.toInt() != ConfigParse::getInstance().getZ()) {
        ConfigParse::getInstance().setTestInfo(KZ, text.toInt());
        isChange = true;
    }

    text = ui->TimeStepLineEdit->text();
    if (text.toDouble() != ConfigParse::getInstance().getTimeStep()) {
        ConfigParse::getInstance().setTestInfo(KTimeStep, text.toDouble());
        isChange = true;
    }

    bool check = ui->MtcpCheckBox->isChecked();
    if (check != ConfigParse::getInstance().getMtcp()) {
        ConfigParse::getInstance().setTestInfo(KMtcp, check);
    }

    check = ui->MesCheckBox->isChecked();
    if (check != ConfigParse::getInstance().getMes()) {
        ConfigParse::getInstance().setTestInfo(KMes, check);
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
    ui->AlphaLineEdit->setText(QString::number(ConfigParse::getInstance().getAlpha()));
    ui->BetaLineEdit->setText(QString::number(ConfigParse::getInstance().getBeta()));
    ui->GammaLineEdit->setText(QString::number(ConfigParse::getInstance().getGamma()));
    ui->XpLineEdit->setText(QString::number(ConfigParse::getInstance().getXp()));
    ui->XsLineEdit->setText(QString::number(ConfigParse::getInstance().getXs()));
    ui->YLineEdit->setText(QString::number(ConfigParse::getInstance().getY()));
    ui->ZLineEdit->setText(QString::number(ConfigParse::getInstance().getZ()));
    ui->TimeStepLineEdit->setText(QString::number(ConfigParse::getInstance().getTimeStep()));

    ui->MtcpCheckBox->setChecked(ConfigParse::getInstance().getMtcp());
    ui->MesCheckBox->setChecked(ConfigParse::getInstance().getMes());
}

void PreferencesDialog::clickLanguageAction(QTranslator& tran)
{
    qApp->installTranslator(&tran);
    ui->retranslateUi(this);
}
