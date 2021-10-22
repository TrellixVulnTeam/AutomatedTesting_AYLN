#ifndef MOTORDIALOG_H
#define MOTORDIALOG_H

#include <QDialog>
#include "SportsWidget.h"
#include <configdevice.h>
#include <QPropertyAnimation>

namespace Ui
{
class MotorDialog;
}

class MotorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MotorDialog(ConfigDevice* device, int slot, QWidget* parent = 0);
    ~MotorDialog();

signals:
    void appendLogTextEdit(QString msg);

private slots:
    void onModuleBtnClicked(BtnType btnType);
    void onAppendLogTextEdit(QString msg);

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
    void on_aBtn_clicked();
    void on_clipBtn_clicked();
    void on_sendBtn_clicked();
    void on_readBtn_clicked();
    void on_DCBtn_clicked();
    void on_va1Btn_clicked();
    void on_va2Btn_clicked();
    void on_setCMBtn_clicked();

private:
    void showEvent(QShowEvent* event) override;

private:
    Ui::MotorDialog* ui;
    SportsWidget* m_lightSourceW = NULL;
    SportsWidget* m_productW = NULL;
    ConfigDevice* m_devices = NULL;
    int m_slot = 0;
    int m_maxW = 1560;
    int m_maxH = 950;

    QObject* m_uart1 = NULL;
    QObject* m_uart2 = NULL;
    QPropertyAnimation* m_animation = NULL;
    std::vector<QPropertyAnimation*> m_groupAnimations;
};

#endif  // MOTORDIALOG_H
