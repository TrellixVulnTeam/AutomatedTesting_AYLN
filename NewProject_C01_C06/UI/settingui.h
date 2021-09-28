#ifndef SETTINGUI_H
#define SETTINGUI_H

#include <QWidget>
#include <items.h>
#include <QTableWidgetItem>
#include <propertyui.h>
#include <QGridLayout>
#include <QMap>
#include <QCloseEvent>

namespace Ui
{
class SettingUI;
}

class SettingUI : public QWidget
{
    Q_OBJECT

public:
    explicit SettingUI(const QList<Items*> testItemList, QWidget* parent = 0);

    ~SettingUI();

public slots:
    void getItem(QTableWidgetItem*);
    void show_menu(const QPoint pos);
    void addTestItem_click();
    void deleteItem_click();
    void changeItem_click();
    void addItemFromItemUIObject(Items*);
    void changeItemFromItemUIObject(Items*);
    void updateToCsv();

private:
    Ui::SettingUI* ui;
    void loadWidget();
    void closeEvent(QCloseEvent* event);
    QString getValue(const Items* Itemp, QString key);

    void updateListView();
    QList<Items*> ItemsList;
    QGridLayout* glayout;
    QMap<QString, propertyUI*> propertyMap;
    QList<QString> propertyNumlist;
    QList<QString*> propertyList;
    QModelIndex chooseModelIndex;
};

#endif  // SETTINGUI_H
