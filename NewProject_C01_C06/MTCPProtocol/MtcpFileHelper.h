#ifndef MTCPFILEHELPER_H
#define MTCPFILEHELPER_H

#include <QObject>
#include <QMap>

enum MTCP_FILE_TYPE
{
    M_START = 0,
    M_END,
    M_PIVOT,
    M_CONFIG
};

class MtcpFileHelper
{
public:
    MtcpFileHelper();

    static const QMap<QString, QString> inputBuildConfig();
    static const QString inputCurrLotName();
    static const QString outputLotStart(const QString& lotName);
    static const QString outputLotEnd(const QString& lotName);
    static const QString outputPivotRequest(const QString& sn, const QString& socketSN, const QString& lotName,
                                            const QString& version);

    static bool fileChecksum(const QString filePath, MTCP_FILE_TYPE type = M_START);
};

#endif  // MTCPFILEHELPER_H
