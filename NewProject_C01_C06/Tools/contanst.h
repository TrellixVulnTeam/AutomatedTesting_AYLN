#ifndef CONTANST_H
#define CONTANST_H

#include <QObject>

class Contanst : public QObject
{
    Q_OBJECT
public:
    explicit Contanst(QObject *parent = nullptr);

    QString TestStatusFail();     //测试失败
    QString TestStatusPass();     //测试成功
    QString TestStatusError();    //测试异常
    QString TestStatusInit();     //测试初始状态


signals:

public slots:
};

#endif // CONTANST_H
