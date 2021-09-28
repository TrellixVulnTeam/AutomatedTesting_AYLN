#ifndef USER_H
#define USER_H

#include <QCryptographicHash>
#include <QObject>
#include <QVariantMap>

#define PASSWD "Passwd"
#define AUTHORITY "Authority"
#define TESTE "TE"
#define OPERATOR "OP"
#define DEVELOPER "Engineer"

#define DEFAULT_PW "OP123"
#define DEFAULT_TESTE_PW "TE123"
#define DEFAULT_DEVELOPER_PW "SW123"

class User : public QObject
{
    Q_OBJECT

public:
    typedef enum
    {
        Operator,
        Developer,
        TestE,
        Invalid
    } Authority;
    Q_ENUM(Authority)

    explicit User(QObject* parent = nullptr);

    const QString& getName() const
    {
        return m_name;
    }

    void setName(const QString& name)
    {
        m_name = name;
    }

    const QString& getPasswd() const
    {
        return m_passwd;
    }

    void setPasswd(const QString& passwd)
    {
        m_passwd = passwd;
    }

    Authority getAuthority() const
    {
        return m_authority;
    }

    void setAuthority(const Authority& authority)
    {
        m_authority = authority;
    }

    const QVariantMap& getUserInfo() const
    {
        return m_userInfo;
    }

    void serialize();

private:
    QString m_name;
    QString m_passwd;
    Authority m_authority;
    QVariantMap m_userInfo;
};

#endif  // USER_H
