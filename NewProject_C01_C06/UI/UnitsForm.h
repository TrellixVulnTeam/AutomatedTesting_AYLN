#ifndef UNITSFORM_H
#define UNITSFORM_H

#include <QWidget>
#include <QMetaType>
#include <QDateTime>
#include <QTimer>
#include "testaction.h"
#include <QDialog>
#include "SNInputForm.h"

class SNInputForm;

struct UNIT_INFO
{
    UNIT_INFO::UNIT_INFO(const QString& _unitName = "Unit-1", const QString& _testplanName = "TestPlan.csv")
        : unitName(_unitName), testplanName(_testplanName)
    {
    }
    QString unitName;
    QString testplanName;
};
Q_DECLARE_METATYPE(UNIT_INFO);

enum TEST_STATE
{
    INITIALIZING = 0,
    READY,
    TESTING,
    FINISHED,
    WRONG
};
Q_DECLARE_METATYPE(TEST_STATE);

namespace Ui
{
class UnitsForm;
}

class UnitsForm : public QWidget
{
    Q_OBJECT

public:
    explicit UnitsForm(QWidget* parent = 0, qint16 _slot = 0);
    ~UnitsForm();

    void setUnitInfo(const UNIT_INFO& info);
    bool getEnable() const;
    void scanSN(const QString& SN);
    QString getSN() const;
    TEST_STATE getTestState() const;
    void setTestState(TEST_STATE state);
    void clearSN();

public slots:
    void onConnectedDevice(const std::vector<std::string>& names);
    void onTestStart(const QString& SN = "");
    void onTestEnd(TEST_RESULT result);
    void updateNetState(const QString& str);
    void onReseting();
    void finishReset();

signals:
    void testStart(const UNIT_INFO& info, const QString& sn);
    void doubleMouseClick(qint16 slot);
    void singleStart(qint16 slot, const QString& sn, const std::string& mtcpFilePath);
    void originBtnClicked();
    void motorBtnClicked();

private slots:
    void on_enableCheck_clicked(bool checked);
    void onMouseClick();
    void onTimerTimeout();
    void on_motorBtn_clicked();

private:
    void setTestStateStr();
    void initialTime();
    void correctTime();

    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    Ui::UnitsForm* ui;
    TEST_STATE testStatue;
    QString sn;
    qint16 unitSlot = 0;
    UNIT_INFO unitInfo;

    QDateTime m_startTime;
    QDateTime m_endTime;
    QTimer* m_timer = NULL;
    QTimer* m_mouseTimer = NULL;
    int pcnt = 0;
    int tcnt = 0;
    QDialog* singleDialog = NULL;
    SNInputForm* snForm = NULL;
    int m_checkedNum = 0;
};

#endif  // UNITSFORM_H
