#ifndef LOOPTESTWIDGET_H
#define LOOPTESTWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include <QTranslator>

namespace Ui
{
class LoopTestWidget;
}

class LoopTestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoopTestWidget(QWidget* parent = 0, int _slot = 0);
    ~LoopTestWidget();

    bool getLoopFlag() const
    {
        return m_loopFlag;
    }

signals:
    void loopStart(int _slot);
    void loopStop(int _slot);

public slots:
    void onTestStart(int _slot);
    void onTestFinished(int _slot);
    void clickLanguageAction(QTranslator& tran);
    void onStopLoopTestWhileError(int _slot);

private slots:
    void onLoop();

private:
    void setEditable(bool enable);

    Ui::LoopTestWidget* ui = nullptr;
    int32_t m_loopCount = 0;
    int32_t m_currentCount = 0;
    bool m_loopFlag = false;

    int m_slot = 0;
};

#endif  // LOOPTESTWIDGET_H
