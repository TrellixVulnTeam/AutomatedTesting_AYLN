#ifndef PWDDIALOG_H
#define PWDDIALOG_H

#include <QDialog>

namespace Ui {
class PwdDialog;
}

class PwdDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PwdDialog(QWidget *parent = 0);
    ~PwdDialog();

signals:
    void onPwdOK();

private slots:
    void on_pushButton_confirm_clicked();

private:
    Ui::PwdDialog *ui;
};

#endif // PWDDIALOG_H
