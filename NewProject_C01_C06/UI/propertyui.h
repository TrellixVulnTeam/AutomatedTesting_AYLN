#ifndef PROPERTYUI_H
#define PROPERTYUI_H

#include <QFrame>
#include <QLabel>
#include <QLineEdit>
namespace Ui
{
class propertyUI;
}

class propertyUI : public QFrame
{
    Q_OBJECT

public:
    explicit propertyUI(QString itemName, QString itemValue, QWidget* parent = 0);
    QLabel* itemDesLable;
    QLineEdit* itemValueLable;

    ~propertyUI();

private:
    Ui::propertyUI* ui;
};

#endif  // PROPERTYUI_H
