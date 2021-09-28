#include "propertyui.h"
#include "ui_propertyui.h"

propertyUI::propertyUI(QString itemName,QString itemValue,QWidget *parent) :
    QFrame(parent),
    ui(new Ui::propertyUI)
{
    ui->setupUi(this);
    ui->label_name->setText(itemName);
    ui->textEdit_value->setText(itemValue);
    this->itemValueLable = ui->textEdit_value;
}

propertyUI::~propertyUI()
{
    delete ui;
}
