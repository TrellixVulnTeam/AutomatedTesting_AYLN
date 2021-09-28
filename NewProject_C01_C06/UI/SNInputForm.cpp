#include "SNInputForm.h"
#include "ui_SNInputForm.h"
#include <QRegExp>

const QString SNRegExp = "\\S{17}";

SNInputForm::SNInputForm(QWidget* parent, qint16 _slot) : QWidget(parent), slot(_slot), ui(new Ui::SNInputForm)
{
    ui->setupUi(this);
    ui->snLab->setText(QString("SN%1:").arg(_slot + 1));

    QRegExp exp(SNRegExp);
    QValidator* snValidator = new QRegExpValidator(exp, this);
    ui->snLineEdit->setValidator(snValidator);
    ui->snLineEdit->setFocus();
    ui->singleStartBtn->setFocusPolicy(Qt::NoFocus);

    connect(ui->snLineEdit, &QLineEdit::returnPressed, [&]() { emit returnPressed(slot); });
}

SNInputForm::~SNInputForm()
{
    delete ui;
}

void SNInputForm::setSNFocus()
{
    ui->snLineEdit->setFocus();
}

QString SNInputForm::getSN() const
{
    return ui->snLineEdit->text();
}

void SNInputForm::clearSN()
{
    ui->snLineEdit->clear();
}

void SNInputForm::on_singleStartBtn_clicked()
{
    emit singleStart(slot, ui->snLineEdit->text());
}
