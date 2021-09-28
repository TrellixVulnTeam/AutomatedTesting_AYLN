#include "User.h"
#include <QByteArray>

User::User(QObject* parent) : QObject(parent), m_authority(Operator) {}

void User::serialize()
{
    m_userInfo[AUTHORITY] = m_authority;

    QByteArray byteText = m_passwd.toUtf8();
    QString strPwdMd5 = QCryptographicHash::hash(byteText, QCryptographicHash::Md5).toHex();
    QString md5;
    md5.append(strPwdMd5);
    m_userInfo[PASSWD] = md5;
}
