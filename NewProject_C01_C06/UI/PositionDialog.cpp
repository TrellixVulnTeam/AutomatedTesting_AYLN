#include "PositionDialog.h"
#include "ui_PositionDialog.h"

PositionDialog::PositionDialog(QWidget* parent) : QDialog(parent), ui(new Ui::PositionDialog)
{
    ui->setupUi(this);
    Qt::WindowFlags windowFlag = Qt::Dialog;
    windowFlag |= Qt::WindowMinMaxButtonsHint;
    windowFlag |= Qt::WindowCloseButtonHint;
    setWindowFlags(windowFlag);

    setStyleSheet("QDialog{min-height:10em;min-width:10em;max-height:34em;max-width:34em;}");
}

PositionDialog::~PositionDialog()
{
    delete ui;
}
