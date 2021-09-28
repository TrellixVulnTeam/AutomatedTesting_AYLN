#include "SNDialog.h"
#include "ui_SNDialog.h"

SNDialog::SNDialog(QWidget* parent, qint16 _uutCount) : QDialog(parent), uutCount(_uutCount), ui(new Ui::SNDialog)
{
    ui->setupUi(this);

    ui->allStartBtn->setFocusPolicy(Qt::NoFocus);
    setStyleSheet("background: white");
    ui->singleSNInputLayout->setContentsMargins(10, 10, 10, 10);
    ui->singleSNInputLayout->setSpacing(10);
    for (int i(0); i < uutCount; i++) {
        SNInputForm* form = new SNInputForm(this, i);
        connect(form, &SNInputForm::singleStart, this, &SNDialog::singleStart);
        connect(form, &SNInputForm::returnPressed, this, &SNDialog::onReturnPressed);
        ui->singleSNInputLayout->addWidget(form, i, 0);
        snForms.push_back(form);
    }
}

SNDialog::~SNDialog()
{
    delete ui;
}

void SNDialog::execDialog(QList<UnitsForm*> forms)
{
    unitForms.clear();
    unitForms = forms;
    bool isSetFocus = false;
    for (int i(0); i < uutCount; i++) {
        UnitsForm* form = forms.at(i);
        if (form->getEnable()) {
            if (!isSetFocus) {
                isSetFocus = true;
                snForms.at(i)->setSNFocus();
            }
        } else {
            snForms.at(i)->setEnabled(false);
        }
    }

    QDialog::exec();
}

void SNDialog::on_allStartBtn_clicked()
{
    QList<QString> snList;
    for (int i(0); i < uutCount; i++) {
        snList.push_back(snForms.at(i)->getSN());
    }
    emit startAll(snList);
}

void SNDialog::clearSN(qint16 slot)
{
    snForms.at(slot)->clearSN();
}

void SNDialog::onReturnPressed(qint16 slot)
{
    qint16 count = uutCount;
    qint16 nextSlot = slot;
    while (count > 0) {
        nextSlot++;
        if (nextSlot >= uutCount)
            nextSlot = 0;
        if (unitForms.at(nextSlot)->getEnable()) {
            snForms.at(nextSlot)->setSNFocus();
            break;
        }
        count--;
    }
}
