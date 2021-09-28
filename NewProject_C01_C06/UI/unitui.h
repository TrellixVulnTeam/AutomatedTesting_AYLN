#ifndef UNITUI_H
#define UNITUI_H

#include <QGroupBox>

namespace Ui {
class UnitUI;
}

class UnitUI : public QGroupBox
{
    Q_OBJECT

public:
    explicit UnitUI(QWidget *parent = 0);
    ~UnitUI();

private:
    Ui::UnitUI *ui;
};

#endif // UNITUI_H
