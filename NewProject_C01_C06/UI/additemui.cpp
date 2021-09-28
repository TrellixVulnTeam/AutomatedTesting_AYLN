#include "additemui.h"
#include "ui_additemui.h"

addItemUI::addItemUI(Items* tempItem, QWidget* parent) : QFrame(parent), ui(new Ui::addItemUI)
{
    ui->setupUi(this);

    ui->pushButton_save->setStyleSheet(
        "QPushButton:hover{background-color: rgb(20, 62, 134);border:none;color:rgb(100, 1000, 100);}"
        "QPushButton:checked{background-color: rgb(20, 62, 134);border:none;color:rgb(255, 255, 255);}"
        "QPushButton:{background-color: rgb(170, 200, 50);}");

    ui->plainTextEdit_Status->setPlainText(tempItem->Status);
    ui->plainTextEdit_Show->setPlainText(tempItem->Show);
    ui->plainTextEdit_Group->setPlainText(tempItem->Group);
    ui->plainTextEdit_ItemName->setPlainText(tempItem->ItemName);
    ui->plainTextEdit_ClassType->setPlainText(tempItem->ClassType);
    ui->plainTextEdit_Snyc->setPlainText(tempItem->Snyc);
    ui->plainTextEdit_Target->setPlainText(tempItem->Target);
    ui->plainTextEdit_Ref->setPlainText(tempItem->Ref);
    ui->plainTextEdit_Function->setPlainText(tempItem->Function);
    ui->plainTextEdit_Param->setPlainText(tempItem->Param);
    ui->plainTextEdit_LogicFunction->setPlainText(tempItem->LogicFunction);
    ui->plainTextEdit_StartStr->setPlainText(tempItem->StartStr);
    ui->plainTextEdit_Suffix->setPlainText(tempItem->Suffix);
    ui->plainTextEdit_Timeout->setPlainText(tempItem->Timeout);
    ui->plainTextEdit_Retry->setPlainText(tempItem->Retry);
    ui->plainTextEdit_isBreak->setPlainText(tempItem->isBreak);
    ui->plainTextEdit_SaveGlobal->setPlainText(tempItem->SaveGlobal);
    ui->plainTextEdit_Global->setPlainText(tempItem->Global);
    ui->plainTextEdit_lowerLimits->setPlainText(tempItem->lowerLimits);
    ui->plainTextEdit_upperLimits->setPlainText(tempItem->upperLimits);
    ui->plainTextEdit_Unit->setPlainText(tempItem->Unit);
    ui->plainTextEdit_Relation->setPlainText(tempItem->Relation);

    //关联信号和槽
    connect(ui->pushButton_save, SIGNAL(clicked(bool)), this, SLOT(saveNewTestItem()));
}

// Status,SHOW,GROUP,ItemName,Plugin,ClassType,Target,Ref,Function,
// Suffix,Param,LogicFunction,StartStr,EndStr,Timeout,Retry,isBreak,
// SaveGlobal,GLOBAL,LowerLimits,UpperLimits,Unit,Relation

void addItemUI::saveNewTestItem()
{

    Items* newItem = new Items(this);
    newItem->Status = ui->plainTextEdit_Status->toPlainText();
    newItem->Show = ui->plainTextEdit_Show->toPlainText();
    newItem->Group = ui->plainTextEdit_Group->toPlainText();
    newItem->ItemName = ui->plainTextEdit_ItemName->toPlainText();
    newItem->ClassType = ui->plainTextEdit_ClassType->toPlainText();
    newItem->Target = ui->plainTextEdit_Target->toPlainText();
    newItem->Ref = ui->plainTextEdit_Ref->toPlainText();
    newItem->Function = ui->plainTextEdit_Function->toPlainText();
    newItem->Suffix = ui->plainTextEdit_Suffix->toPlainText();
    newItem->Param = ui->plainTextEdit_Param->toPlainText();
    newItem->LogicFunction = ui->plainTextEdit_LogicFunction->toPlainText();
    newItem->StartStr = ui->plainTextEdit_StartStr->toPlainText();
    newItem->EndStr = ui->plainTextEdit_EndStr->toPlainText();
    newItem->Timeout = ui->plainTextEdit_Timeout->toPlainText();
    newItem->Retry = ui->plainTextEdit_Retry->toPlainText();
    newItem->isBreak = ui->plainTextEdit_isBreak->toPlainText();
    newItem->SaveGlobal = ui->plainTextEdit_SaveGlobal->toPlainText();
    newItem->Global = ui->plainTextEdit_Global->toPlainText();
    newItem->lowerLimits = ui->plainTextEdit_lowerLimits->toPlainText();
    newItem->upperLimits = ui->plainTextEdit_upperLimits->toPlainText();
    newItem->Unit = ui->plainTextEdit_Unit->toPlainText();
    newItem->Relation = ui->plainTextEdit_Relation->toPlainText();
    newItem->Snyc = ui->plainTextEdit_Snyc->toPlainText();

    emit addNewTestItem_Signal(newItem);
    this->close();
}

addItemUI::~addItemUI()
{
    delete ui;
}
