#include "logunitui.h"
#include "ui_logunitui.h"

logUnitUI::logUnitUI(QWidget* parent) : QGroupBox(parent), ui(new Ui::logUnitUI)
{
    ui->setupUi(this);

    UnitSN = ui->unitSN;
    UnitLogView = ui->unitTextBrowser;
    UnitSNNum = ui->UnitLable;
    findBtn = ui->findButton;

    connect(this->findBtn, SIGNAL(clicked(bool)), this, SLOT(onFindBtnTriggered()));

    findDlg = new QDialog(this);
    findDlg->setWindowTitle(tr("lookup"));
    findLineEdit = new QLineEdit(findDlg);

    QPushButton* btn = new QPushButton(tr("Next"), findDlg);
    QVBoxLayout* layout = new QVBoxLayout(findDlg);
    layout->addWidget(findLineEdit);
    layout->addWidget(btn);

    connect(btn, SIGNAL(clicked(bool)), this, SLOT(showFindText()));

    //  this->UnitLogView->setStyleSheet("background-color: LightCyan ;");
}

void logUnitUI::showFindText()
{
    QString str = findLineEdit->text();

    bool ret = UnitLogView->find(str, QTextDocument::FindBackward);
    if (!ret) {
        QMessageBox::warning(this, tr("lookup"), tr("Can't find %1").arg(str));
    }
}

void logUnitUI::onFindBtnTriggered()
{

    findDlg->show();
}

logUnitUI::~logUnitUI()
{
    delete ui;
}
