#ifndef SPORTSWIDGET_H
#define SPORTSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "MoveBtn.h"

namespace Ui
{
class SportsWidget;
}

class SportsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SportsWidget(QWidget* parent = 0);
    ~SportsWidget();

private:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onXBtn1Clicked();
    void onXBtn2Clicked();

    void onYBtn1Clicked();
    void onYBtn2Clicked();

    void onZBtn1Clicked();
    void onZBtn2Clicked();

private:
    Ui::SportsWidget* ui;
    MoveBtn* xButton1;
    MoveBtn* xButton2;
    MoveBtn* yButton1;
    MoveBtn* yButton2;
    MoveBtn* zButton1;
    MoveBtn* zButton2;

    int m_lastWidth = 0;
    int m_lastHeight = 0;
};

#endif  // SPORTSWIDGET_H
