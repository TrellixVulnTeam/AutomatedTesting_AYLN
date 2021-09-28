#ifndef PROPERTYUNITUI_H
#define PROPERTYUNITUI_H

#include <QWidget>

namespace Ui {
class propertyUnitUI;
}

class propertyUnitUI : public QWidget
{
    Q_OBJECT

public:
    explicit propertyUnitUI(QWidget *parent = 0);
    ~propertyUnitUI();

private:
    Ui::propertyUnitUI *ui;
};

#endif // PROPERTYUNITUI_H
