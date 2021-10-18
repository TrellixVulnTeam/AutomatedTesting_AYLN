#ifndef MOTORDIALOG_H
#define MOTORDIALOG_H

#include <QDialog>
#include "SportsWidget.h"

namespace Ui
{
class MotorDialog;
}

class MotorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MotorDialog(QWidget* parent = 0);
    ~MotorDialog();

private slots:
    void onModuleBtnClicked(BtnType btnType);

    void on_homeingBtn_clicked();
    void on_startBtn_clicked();
    void on_pauseBtn_clicked();
    void on_stopBtn_clicked();
    void on_xHomeBtn_clicked();
    void on_yHomeBtn_clicked();
    void on_zHomeBtn_clicked();
    void on_x1HomeBtn_clicked();
    void on_gHomeBtn_clicked();
    void on_bHomeBtn_clicked();
    void on_aHomeBtn_clicked();
    void on_homeBtn_clicked();
    void on_resetBtn_clicked();
    void on_cameraBtn_clicked();
    void on_aBtn_clicked();
    void on_clipBtn_clicked();
    void on_sendBtn_clicked();
    void on_readBtn_clicked();
    void on_DCBtn_clicked();
    void on_va1Btn_clicked();
    void on_va2Btn_clicked();

private:
    void appendLogTextEdit(const QString& msg);

private:
    Ui::MotorDialog* ui;
    SportsWidget* m_lightSourceW = NULL;
    SportsWidget* m_productW = NULL;
};

#endif  // MOTORDIALOG_H
