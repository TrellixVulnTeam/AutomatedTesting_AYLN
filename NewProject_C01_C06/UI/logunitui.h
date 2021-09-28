#ifndef LOGUNITUI_H
#define LOGUNITUI_H

#include <QGroupBox>
#include <QLabel>
#include <QTextBrowser>
#include <QPushButton>
#include <QLineEdit>
#include <QDialog>
#include <QVBoxLayout>
#include <QDialog>
#include <QMessageBox>

namespace Ui
{
class logUnitUI;

}

class logUnitUI : public QGroupBox
{
    Q_OBJECT

public:
    explicit logUnitUI(QWidget* parent = 0);

    QLabel* UnitSN;
    QLabel* UnitSNNum;
    QTextBrowser* UnitLogView;
    QPushButton* findBtn;

    ~logUnitUI();

private slots:
    void showFindText();
    void onFindBtnTriggered();

private:
    Ui::logUnitUI* ui;
    QLineEdit* findLineEdit;
    QDialog* findDlg;
};

#endif  // LOGUNITUI_H
