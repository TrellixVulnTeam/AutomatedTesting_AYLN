#ifndef ADDITEMUI_H
#define ADDITEMUI_H

#include <QFrame>
#include <items.h>

namespace Ui
{
class addItemUI;
}

class addItemUI : public QFrame
{
    Q_OBJECT

public:
    explicit addItemUI(Items*, QWidget* parent = 0);
    ~addItemUI();

private slots:
    void saveNewTestItem();

private:
    Ui::addItemUI* ui;

signals:
    void addNewTestItem_Signal(Items*);  //新增测试项的信号
};

#endif  // ADDITEMUI_H
