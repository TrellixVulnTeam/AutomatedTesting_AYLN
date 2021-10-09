#include "LoopTestDialog.h"
#include "ui_LoopTestDialog.h"
#include "ConfigParse.h"
#include <QMessageBox>

LoopTestDialog::LoopTestDialog(QWidget* parent) : QDialog(parent), ui(new Ui::LoopTestDialog)
{
    ui->setupUi(this);

    setWindowTitle(tr("Loop Test"));
    ui->unitLoopLayout->setContentsMargins(10, 10, 10, 10);
    ui->unitLoopLayout->setSpacing(10);
    for (int i(0); i < CFG_PARSE.getUnitCount(); i++) {
        LoopTestWidget* form = new LoopTestWidget(this, i);
        ui->unitLoopLayout->addWidget(form, i, 0);
        connect(form, &LoopTestWidget::loopStart, this, &LoopTestDialog::loopStart);
        connect(form, &LoopTestWidget::loopStop, this, &LoopTestDialog::loopStop);
        connect(this, &LoopTestDialog::startTest, form, &LoopTestWidget::onTestStart);
        connect(this, &LoopTestDialog::finishTest, form, &LoopTestWidget::onTestFinished);
        connect(this, &LoopTestDialog::stopLoopTestWhileError, form, &LoopTestWidget::onStopLoopTestWhileError);
        m_loopWs.push_back(form);
    }
}

void LoopTestDialog::closeEvent(QCloseEvent* evt)
{
    bool loopFlag = false;
    for (int i(0); i < CFG_PARSE.getUnitCount(); i++) {
        LoopTestWidget* form = m_loopWs[i];
        loopFlag |= form->getLoopFlag();
    }

    if (loopFlag) {
        QMessageBox::information(this, tr("hint"), tr("In the loop test, please loop out test first "));
        evt->ignore();
    }
}

LoopTestDialog::~LoopTestDialog()
{
    delete ui;
}

void LoopTestDialog::onTestStart(int _slot)
{
    if (isVisible() && !isHidden())
        emit startTest(_slot);
}

void LoopTestDialog::onTestFinished(int _slot)
{
    if (isVisible() && !isHidden())
        emit finishTest(_slot);
}
