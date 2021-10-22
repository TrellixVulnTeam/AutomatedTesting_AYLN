#ifndef LOOPTESTDIALOG_H
#define LOOPTESTDIALOG_H

#include <QDialog>
#include "LoopTestWidget.h"

namespace Ui
{
class LoopTestDialog;
}

class LoopTestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoopTestDialog(QWidget* parent = 0);
    ~LoopTestDialog();

signals:
    void loopStart(int _slot);
    void loopStop(int _slot);
    void startTest(int _slot);
    void finishTest(int _slot);
    void stopLoopTestWhileError(int slot);

public slots:
    void onTestStart(int _slot);
    void onTestFinished(int _slot);

private:
    void closeEvent(QCloseEvent* evt) override;
    void showEvent(QShowEvent* event);

private:
    Ui::LoopTestDialog* ui;
    std::vector<LoopTestWidget*> m_loopWs;
};

#endif  // LOOPTESTDIALOG_H
