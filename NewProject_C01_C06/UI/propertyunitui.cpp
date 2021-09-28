#include "propertyunitui.h"
#include "ui_propertyunitui.h"

propertyUnitUI::propertyUnitUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::propertyUnitUI)
{
    ui->setupUi(this);
}

propertyUnitUI::~propertyUnitUI()
{
    delete ui;
}
