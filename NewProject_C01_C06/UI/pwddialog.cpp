#include "pwddialog.h"
#include "ui_pwddialog.h"
#include "QDebug"
PwdDialog::PwdDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PwdDialog)
{
    ui->setupUi(this);
}

PwdDialog::~PwdDialog()
{
    delete ui;
}

void PwdDialog::on_pushButton_confirm_clicked()
{
    if(ui->lineEdit_pwd->text() == "123")
    {
        emit onPwdOK();
        this->close();
    }
    else
    {
        ui->lineEdit_pwd->clear();
    }
}
