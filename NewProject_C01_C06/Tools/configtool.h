#ifndef CONFIGTOOL_H
#define CONFIGTOOL_H

#include <QObject>

class ConfigTool : public QObject
{
    Q_OBJECT
public:
    explicit ConfigTool(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CONFIGTOOL_H
