#ifndef SPORTSWIDGET_H
#define SPORTSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "MoveBtn.h"

enum ModuleType
{
    LightSource = 0,
    Product
};

enum BtnType
{
    XPlusBtn = 0,
    XMinusBtn,
    YPlusBtn,
    YMinusBtn,
    ZPlusBtn,
    ZMinusBtn,
    X1PlusBtn,
    X1MinusBtn,
    BPlusBtn,
    BMinusBtn,
    GPlusBtn,
    GMinusBtn,
};
Q_DECLARE_METATYPE(BtnType);

namespace Ui
{
class SportsWidget;
}

class SportsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SportsWidget(ModuleType type = LightSource, QWidget* parent = 0);
    ~SportsWidget();

    ModuleType getModuleType() const
    {
        return m_moduleType;
    }

signals:
    void moduleBtnClicked(BtnType btnType);

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

    ModuleType m_moduleType;
};

#endif  // SPORTSWIDGET_H
