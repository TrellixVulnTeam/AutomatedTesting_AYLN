#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include <QDialog>
#include <QAction>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPoint>
#include <QMouseEvent>
#include <QComboBox>
#include <QSettings>
#include <QObject>
#include <QTranslator>

#include "User.h"

const QString LogoutStr = QObject::tr("Logout");
const QString LoginStr = QObject::tr("Login");

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    LoginDialog(QWidget* parent = nullptr);
    void autoLogin();

private:
    QLabel* m_loginIconLab = NULL;
    QLabel* m_loginNameLab = NULL;
    QLabel* m_loginPwdLab = NULL;

    QComboBox* m_loginNameComboBox = NULL;
    QLineEdit* m_loginPwdLineEdit = NULL;
    QPushButton* m_loginInBtn = NULL;
    QPushButton* m_cancelBtn = NULL;
    QSettings* m_userSetting = NULL;

protected:
    void showEvent(QShowEvent* ev);
    void keyReleaseEvent(QKeyEvent* event);

public slots:
    void onUpdateUserSetting(QSettings* setting);
    void clickLanguageAction(QTranslator&);

private slots:
    void onLogin();
    void onCancel();

signals:
    void loginSuccessSignal(const QString& userName, User::Authority authority);
};

#endif  // LOGINDIALOG_H
