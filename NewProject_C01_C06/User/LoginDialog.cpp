

#include <QLayout>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QDesktopWidget>
#include <QApplication>
#include "UserManagerDialog.h"
#include "LoginDialog.h"

LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    QGridLayout* grid = new QGridLayout(this);
    m_loginIconLab = new QLabel(this);
    m_loginIconLab->setPixmap(QPixmap(":/Icons/user.png"));
    m_loginNameLab = new QLabel(tr("Name:"), this);
    m_loginPwdLab = new QLabel(tr("Password:"), this);
    m_loginNameComboBox = new QComboBox(this);
    //    m_loginNameComboBox->setEditable(true);

    m_loginPwdLineEdit = new QLineEdit(this);
    m_loginPwdLineEdit->setEchoMode(QLineEdit::Password);
    m_loginInBtn = new QPushButton(tr("Login"), this);
    m_cancelBtn = new QPushButton(tr("Cancel"), this);

    grid->addWidget(m_loginIconLab, 0, 1);
    grid->addWidget(m_loginNameLab, 1, 0);
    grid->addWidget(m_loginNameComboBox, 1, 1, 1, 2);
    grid->addWidget(m_loginPwdLab, 2, 0);
    grid->addWidget(m_loginPwdLineEdit, 2, 1, 1, 2);
    grid->addWidget(m_loginInBtn, 3, 0);
    grid->addWidget(m_cancelBtn, 3, 2);

    connect(m_loginInBtn, &QPushButton::clicked, this, &LoginDialog::onLogin);
    connect(m_cancelBtn, &QPushButton::clicked, this, &LoginDialog::onCancel);
}

void LoginDialog::onLogin()
{
    QString userName = m_loginNameComboBox->currentText();
    QString password = m_loginPwdLineEdit->text();
    if (userName.isEmpty()) {
        QMessageBox::information(this, tr("init"), tr("Please enter user name."));
        return;
    }
    if (password.isEmpty()) {
        QMessageBox::information(this, tr("init"), tr("Please enter password."));
        return;
    }

    if (NULL != m_userSetting) {
        QStringList userInfo = m_userSetting->allKeys();
        if (!userInfo.contains(userName)) {
            QMessageBox::information(this, tr("init"), tr("The user does not exist !"));
            return;
        }
        QVariantMap value = m_userSetting->value(userName).toMap();

        User::Authority authority = static_cast<User::Authority>(value.value(AUTHORITY).toInt());
        if (value.value(PASSWD).toString()
            == QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex()) {
            emit loginSuccessSignal(userName, authority);
            m_loginPwdLineEdit->clear();
            close();
        } else {
            QMessageBox::information(this, tr("init"), tr("Password error !"));
            m_loginPwdLineEdit->clear();
        }
    }
}

void LoginDialog::keyReleaseEvent(QKeyEvent* event)
{
    Qt::KeyboardModifiers modi = event->modifiers();
    if (modi & Qt::ControlModifier) {
        int key = event->key();
        switch (key) {
        case Qt::Key_0:  // ctrl + 0 quickly login developer mode
            m_loginNameComboBox->setCurrentText(DEVELOPER);
            m_loginPwdLineEdit->setText(DEFAULT_DEVELOPER_PW);
            emit m_loginInBtn->clicked(true);
            break;
        default:
            break;
        }
    }
    return QDialog::keyReleaseEvent(event);
}

void LoginDialog::onCancel()
{
    this->close();
}

void LoginDialog::showEvent(QShowEvent* ev)
{
    m_loginPwdLineEdit->setFocus();
    QRect rctAvaild = QApplication::desktop()->availableGeometry();
    move((rctAvaild.width() - width()) / 2, (rctAvaild.height() - height()) / 2);
    QDialog::showEvent(ev);
}

void LoginDialog::onUpdateUserSetting(QSettings* setting)
{
    m_userSetting = setting;
    m_loginNameComboBox->clear();
    m_loginNameComboBox->addItems(m_userSetting->allKeys());
    m_loginNameComboBox->setCurrentText(OPERATOR);
}

void LoginDialog::clickLanguageAction(QTranslator&)
{
    m_loginNameLab->setText(tr("Name:"));
    m_loginPwdLab->setText(tr("Password:"));
    m_loginInBtn->setText(tr("Login"));
    m_cancelBtn->setText(tr("Cancel"));
}
