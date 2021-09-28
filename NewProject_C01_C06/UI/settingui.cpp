#include "settingui.h"
#include "ui_settingui.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMetaProperty>
#include <QMenu>
#include <QAction>
#include <additemui.h>
#include <QTableWidgetItem>
#include <QCloseEvent>
#include <QMessageBox>

SettingUI::SettingUI(const QList<Items*> testItemList, QWidget* parent) : QWidget(parent), ui(new Ui::SettingUI)
{
    ui->setupUi(this);
    this->ItemsList = testItemList;

    this->loadWidget();

    ui->ItemtableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->ItemtableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(show_menu(QPoint)));
    connect(this, SIGNAL(close()), this, SLOT(updateToCsv()));
}

void SettingUI::loadWidget()
{

    ui->ItemtableWidget->setRowCount(this->ItemsList.count());
    ui->ItemtableWidget->setColumnCount(1);
    ui->ItemtableWidget->setWindowTitle("TestItems");
    ui->ItemtableWidget->setHorizontalHeaderLabels({ "TestItems" });
    ui->ItemtableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->ItemtableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ItemtableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int i = 0; i < this->ItemsList.count(); i++) {

        Items* tempItem = this->ItemsList[i];
        ui->ItemtableWidget->setItem(i, 0, new QTableWidgetItem(tempItem->ItemName));
    }

    connect(ui->ItemtableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(getItem(QTableWidgetItem*)));
    glayout = new QGridLayout();

    propertyNumlist = { "ItemName", "Status",      "Show",        "Group",  "Snyc",     "ClassType",
                        "Target",   "Ref",         "Function",    "Suffix", "Param",    "LogicFunction",
                        "StartStr", "EndStr",      "Timeout",     "Retry",  "isBreak",  "SaveGlobal",
                        "Global",   "lowerLimits", "upperLimits", "Unit",   "Relation", "Reserve" };
    QHBoxLayout* tempLayout;
    for (int i = 0; i < propertyNumlist.count(); i++) {

        propertyUI* property = new propertyUI(propertyNumlist[i], "", ui->scrollAreaWidgetContents);
        propertyMap.insert(propertyNumlist[i], property);

        if (i % 2 == 0) {

            tempLayout = new QHBoxLayout(this);
            tempLayout->setMargin(10);
            tempLayout->addWidget(property);
        } else {
            tempLayout->addWidget(property);
            glayout->addLayout(tempLayout, i / 2, 0);
        }
    }
    glayout->setSpacing(5);
    glayout->setHorizontalSpacing(10);
    glayout->setVerticalSpacing(3);
    glayout->setMargin(0);
    ui->detailWidget->widget()->setLayout(glayout);
}

void SettingUI::getItem(QTableWidgetItem* tableItem)
{

    Items* item = this->ItemsList[tableItem->row()];
    for (int i = 0; i < propertyMap.keys().count(); i++) {

        QString key = propertyMap.keys()[i];
        propertyUI* property = propertyMap[key];
        property->itemValueLable->clear();
        QString valueStr = this->getValue(item, key);
        property->itemValueLable->setText(valueStr);
    }
}

QString SettingUI::getValue(const Items* Itemp, QString key)
{

    if (key == "ItemName")
        return Itemp->ItemName;
    if (key == "Status")
        return Itemp->Status;
    if (key == "Show")
        return Itemp->Show;
    if (key == "Group")
        return Itemp->Group;
    if (key == "Snyc")
        return Itemp->Snyc;

    if (key == "ClassType")
        return Itemp->ClassType;
    if (key == "Target")
        return Itemp->Target;
    if (key == "Ref")
        return Itemp->Ref;
    if (key == "Function")
        return Itemp->Function;
    if (key == "Suffix")
        return Itemp->Suffix;

    if (key == "Param")
        return Itemp->Param;
    if (key == "LogicFunction")
        return Itemp->LogicFunction;
    if (key == "StartStr")
        return Itemp->StartStr;
    if (key == "EndStr")
        return Itemp->EndStr;
    if (key == "Timeout")
        return Itemp->Timeout;

    if (key == "Retry")
        return Itemp->Retry;
    if (key == "isBreak")
        return Itemp->isBreak;
    if (key == "SaveGlobal")
        return Itemp->SaveGlobal;
    if (key == "Global")
        return Itemp->Global;
    if (key == "lowerLimits")
        return Itemp->lowerLimits;

    if (key == "upperLimits")
        return Itemp->upperLimits;
    if (key == "Unit")
        return Itemp->Unit;
    if (key == "Relation")
        return Itemp->Relation;
    return key;
}

void SettingUI::updateListView() {}

void SettingUI::show_menu(const QPoint pos)
{
    QMenu* menu = new QMenu(ui->ItemtableWidget);
    QAction* pnew = new QAction("Add item", ui->ItemtableWidget);
    QAction* pnew1 = new QAction("Delete item", ui->ItemtableWidget);
    QAction* pnew2 = new QAction("Modify item", ui->ItemtableWidget);
    menu->addAction(pnew);
    menu->addAction(pnew1);
    menu->addAction(pnew2);
    menu->move(cursor().pos());
    menu->show();

    int x = pos.x();
    int y = pos.y();

    this->chooseModelIndex = ui->ItemtableWidget->indexAt(QPoint(x, y));
    int row = this->chooseModelIndex.row();
    int cols = this->chooseModelIndex.column();
    qDebug() << row << "=============" << cols;
    connect(pnew, SIGNAL(triggered()), this, SLOT(addTestItem_click()));
    connect(pnew1, SIGNAL(triggered()), this, SLOT(deleteItem_click()));
    connect(pnew2, SIGNAL(triggered()), this, SLOT(changeItem_click()));
}

void SettingUI::addTestItem_click()
{

    int row = this->chooseModelIndex.row();

    Items* item = this->ItemsList[row];
    addItemUI* itemUI = new addItemUI(item, this);
    connect(itemUI, SIGNAL(addNewTestItem_Signal(Items*)), this, SLOT(addItemFromItemUIObject(Items*)));
    itemUI->show();
}

void SettingUI::changeItem_click()
{

    int row = this->chooseModelIndex.row();
    Items* item = this->ItemsList[row];
    addItemUI* itemUI = new addItemUI(item, this);
    connect(itemUI, SIGNAL(addNewTestItem_Signal(Items*)), this, SLOT(changeItemFromItemUIObject(Items*)));
    itemUI->show();
}

void SettingUI::deleteItem_click()
{

    int row = this->chooseModelIndex.row();

    ui->ItemtableWidget->removeRow(row);

    this->ItemsList.removeAt(row);
}

void SettingUI::changeItemFromItemUIObject(Items* addItem)
{

    propertyMap["ItemName"]->itemValueLable->setText(addItem->ItemName);
    propertyMap["Status"]->itemValueLable->setText(addItem->Status);
    propertyMap["Show"]->itemValueLable->setText(addItem->Show);
    propertyMap["Group"]->itemValueLable->setText(addItem->Group);
    propertyMap["Snyc"]->itemValueLable->setText(addItem->Snyc);
    propertyMap["ClassType"]->itemValueLable->setText(addItem->ClassType);
    propertyMap["Target"]->itemValueLable->setText(addItem->Target);
    propertyMap["Ref"]->itemValueLable->setText(addItem->Ref);
    propertyMap["Function"]->itemValueLable->setText(addItem->Function);
    propertyMap["Suffix"]->itemValueLable->setText(addItem->Suffix);
    propertyMap["Param"]->itemValueLable->setText(addItem->Param);
    propertyMap["LogicFunction"]->itemValueLable->setText(addItem->LogicFunction);
    propertyMap["StartStr"]->itemValueLable->setText(addItem->StartStr);
    propertyMap["EndStr"]->itemValueLable->setText(addItem->EndStr);
    propertyMap["Timeout"]->itemValueLable->setText(addItem->Timeout);
    propertyMap["Retry"]->itemValueLable->setText(addItem->Retry);
    propertyMap["isBreak"]->itemValueLable->setText(addItem->isBreak);
    propertyMap["SaveGlobal"]->itemValueLable->setText(addItem->SaveGlobal);
    propertyMap["Global"]->itemValueLable->setText(addItem->Global);
    propertyMap["lowerLimits"]->itemValueLable->setText(addItem->lowerLimits);
    propertyMap["upperLimits"]->itemValueLable->setText(addItem->upperLimits);
    propertyMap["Unit"]->itemValueLable->setText(addItem->Unit);
    propertyMap["Relation"]->itemValueLable->setText(addItem->Relation);
    qDebug() << addItem->ItemName << "upperLimits:" << addItem->upperLimits;

    int row = this->chooseModelIndex.row();
    this->ItemsList.replace(row, addItem);
}

void SettingUI::addItemFromItemUIObject(Items* addItem)
{

    propertyMap["ItemName"]->itemValueLable->setText(addItem->ItemName);
    propertyMap["Status"]->itemValueLable->setText(addItem->Status);
    propertyMap["Show"]->itemValueLable->setText(addItem->Show);
    propertyMap["Group"]->itemValueLable->setText(addItem->Group);
    propertyMap["Snyc"]->itemValueLable->setText(addItem->Snyc);
    propertyMap["ClassType"]->itemValueLable->setText(addItem->ClassType);
    propertyMap["Target"]->itemValueLable->setText(addItem->Target);
    propertyMap["Ref"]->itemValueLable->setText(addItem->Ref);
    propertyMap["Function"]->itemValueLable->setText(addItem->Function);
    propertyMap["Suffix"]->itemValueLable->setText(addItem->Suffix);
    propertyMap["Param"]->itemValueLable->setText(addItem->Param);
    propertyMap["LogicFunction"]->itemValueLable->setText(addItem->LogicFunction);
    propertyMap["StartStr"]->itemValueLable->setText(addItem->StartStr);
    propertyMap["EndStr"]->itemValueLable->setText(addItem->EndStr);
    propertyMap["Timeout"]->itemValueLable->setText(addItem->Timeout);
    propertyMap["Retry"]->itemValueLable->setText(addItem->Retry);
    propertyMap["isBreak"]->itemValueLable->setText(addItem->isBreak);
    propertyMap["SaveGlobal"]->itemValueLable->setText(addItem->SaveGlobal);
    propertyMap["Global"]->itemValueLable->setText(addItem->Global);
    propertyMap["lowerLimits"]->itemValueLable->setText(addItem->lowerLimits);
    propertyMap["upperLimits"]->itemValueLable->setText(addItem->upperLimits);
    propertyMap["Unit"]->itemValueLable->setText(addItem->Unit);
    propertyMap["Relation"]->itemValueLable->setText(addItem->Relation);
    qDebug() << addItem->ItemName << "upperLimits:" << addItem->upperLimits;

    int row = this->chooseModelIndex.row();
    ui->ItemtableWidget->insertRow(row);
    ui->ItemtableWidget->setItem(row, 0, new QTableWidgetItem(addItem->ItemName));
    ui->ItemtableWidget->selectRow(row);
    this->ItemsList.insert(row, addItem);
}

void SettingUI::updateToCsv()
{

    QString csvConent = "Status,Show,Grop,ItemName,Snyc,ClassType,Target,Ref,Function,Suffix,"
                        "Param,LogicFunction,StartStr,EndStr,Timeout,Retry,isBreak,SaveGlobal,"
                        "GLOBAL,LowerLimits,UpperLimits,Unit,Relation\n";

    QString status;
    for (int i = 0; i < this->ItemsList.count(); i++) {

        Items* temp = this->ItemsList[i];
        status = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,"
                         "%16,%17,%18,%19,%20,%21,%22,%23\r\n")
                     .arg(temp->Status)
                     .arg(temp->Show)
                     .arg(temp->Group)
                     .arg(temp->ItemName)
                     .arg(temp->Snyc)
                     .arg(temp->ClassType)
                     .arg(temp->Target)
                     .arg(temp->Ref)
                     .arg(temp->Function)
                     .arg(temp->Suffix)
                     .arg(temp->Param)
                     .arg(temp->LogicFunction)
                     .arg(temp->StartStr)
                     .arg(temp->EndStr)
                     .arg(temp->Timeout)
                     .arg(temp->Retry)
                     .arg(temp->isBreak)
                     .arg(temp->SaveGlobal)
                     .arg(temp->Global)
                     .arg(temp->lowerLimits)
                     .arg(temp->upperLimits)
                     .arg(temp->Unit)
                     .arg(temp->Relation)
                     .arg(temp->Global)
                     .arg(temp->lowerLimits);
        csvConent.append(status);
    }

    qDebug() << csvConent;

    qDebug() << QApplication::applicationDirPath();
    QString filePath =
        QApplication::applicationDirPath()
        + "/Config/Testplan.csv";  // QApplication::applicationDirPath().replace("MacOS", "Resources/Testplan.csv");

    QFile aFile(filePath);
    if (aFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QByteArray writeBuffer = csvConent.toUtf8();
        aFile.write((writeBuffer));
        aFile.close();
    }
}

void SettingUI::closeEvent(QCloseEvent* event)
{
    switch (QMessageBox::information(this, tr("CT Control View"),
                                     tr("Please confirm whether to save the modified file"), tr("Save and Exit"),
                                     tr("Exit without save"), tr("Remodifly"), 1)) {
    case 0:
        event->accept();
        this->updateToCsv();
        break;
    case 1:
        event->accept();
        break;
    default:
        event->ignore();
        break;
    }
}

SettingUI::~SettingUI()
{
    delete ui;
}
