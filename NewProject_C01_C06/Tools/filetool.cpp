#include "filetool.h"

QString FileTool::readContentWithPath(const QString& path)
{

    QFile aFile(path);

    if (!aFile.exists(path)) {
        return "File does not exist";
    }

    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

        return "File open exception";
    }

    return aFile.readAll();
}

void FileTool::writeContentWithPath(const QString& title, const QString& msg, const QString& path,
                                    const QString& suffix, bool isClear)
{
    if (!QDir(path).exists()) {

        QDir().mkpath(path);
    }

    QString filePath = path + suffix;
    QFile aFile(filePath);
    if (aFile.exists(filePath)) {
        if (aFile.open(QIODevice::Append)) {

            QByteArray writeBuffer = msg.toUtf8();
            if (isClear)
                aFile.resize(0);

            aFile.write((writeBuffer));
            aFile.close();
        }
    } else {
        if (aFile.open(QIODevice::WriteOnly | QIODevice::Text)) {

            QString content = title + msg;

            QByteArray writeBuffer = content.toUtf8();
            aFile.write((writeBuffer));
            aFile.close();
        } else {
            qDebug() << "File creation failed";
        }
    }
}
