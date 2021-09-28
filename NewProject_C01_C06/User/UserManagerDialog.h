#ifndef USERMANAGERDIALOG_H
#define USERMANAGERDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QList>
#include <QMessageBox>
#include <QSettings>
#include <QStandardItemModel>
#include <QTableWidget>
#include <QVariantMap>
#include <QTranslator>

#include "User.h"

namespace Ui
{
class UserManagerDialog;
}

class UserManagerDialog : public QDialog
{
    Q_OBJECT
public:
    UserManagerDialog(QWidget* parent = NULL);
    QSettings* getUserSetting()
    {
        return m_userSetting;
    }
    enum UserOperat
    {
        AddUser,
        ModifyUser
    };

private:
    Ui::UserManagerDialog* ui;
    QTableWidget* m_table;
    QVariantMap m_userInfoMap;

    QSettings* m_userSetting = NULL;

    void initUserList();
    void sortItems();
    void userInfoManager(UserOperat operat);
    bool isDefaultUser(const QString& userName);
    User::Authority getUserAuthority(UserOperat operat);

signals:
    void userInfoChanged(QSettings* setting);

private slots:
    void onAdd();
    void onModify();
    void onDeleteUser();

    void onAddDone();

    void onModifyDone();
    void onCancel();

    void onUpdateModifyWidgetOperatorCombobox(const QString& userName);
    void onUpdateModifyWidgetUserNameCombobox(QSettings* setting);
public slots:
    void clickLanguageAction(QTranslator& tran);
};

#endif  // USERMANAGERDIALOG_H
