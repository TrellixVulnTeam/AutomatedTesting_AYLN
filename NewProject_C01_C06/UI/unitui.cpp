#include "unitui.h"
#include "ui_unitui.h"

UnitUI::UnitUI(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::UnitUI)
{
    ui->setupUi(this);
}

UnitUI::~UnitUI()
{
    delete ui;
}
