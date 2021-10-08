#include "MtcpFileHelper.h"
#include "Util.h"

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QApplication>

const QString K_CONFIG = "/Config/";
const QString K_BACKUP = "/Config/Backup/";

const QString lotStartFile = "LotStart.csv";
const QString lotEndFile = "LotEnd.csv";
const QString pivotRequest = "PivotRequest.csv";

const QString buildConfig = "BuildConfig.csv";

MtcpFileHelper::MtcpFileHelper() {}

bool MtcpFileHelper::fileChecksum(const QString filePath, MTCP_FILE_TYPE type)
{
    const QString TMPath = QApplication::applicationDirPath();
    QString backupPath = TMPath + K_BACKUP;
    switch (type) {
    case M_START:
        backupPath += lotStartFile;
        break;
    case M_END:
        backupPath += lotEndFile;
        break;
    case M_PIVOT:
        backupPath += pivotRequest;
        break;
    case M_CONFIG:
        backupPath += buildConfig;
        break;
    default:
        return false;
    }

    QFile file_backup(backupPath);
    if (!file_backup.exists()) {
        return false;
    }

    QFile file(filePath);
    if (file.exists()) {
        file.remove();
    }

    return QFile::copy(backupPath, filePath);
}

const QMap<QString, QString> MtcpFileHelper::inputBuildConfig()
{
    const QString TMPath = QApplication::applicationDirPath();
    try {
        QFile file(TMPath + K_CONFIG + buildConfig);
        if (!file.open(QIODevice::ReadOnly)) {
            std::string err = std::string("OPEN FILE FAILED, ") + (TMPath + K_CONFIG + buildConfig).toStdString();
            throw std::runtime_error(err);
        }

        QTextStream out(&file);
        QStringList csvLines = out.readAll().split("\n");

        QMap<QString, QString> bulidConfigMap;
        for (int i = 1; i < csvLines.length(); i++) {
            QStringList csvline = csvLines.at(i).split(',');
            if (csvline.length() > 3) {
                bulidConfigMap.insert(csvline.at(2), csvline.at(1));
            }
        }
        return bulidConfigMap;
    }
    catch (std::runtime_error& e) {
        fileChecksum(TMPath + K_CONFIG + buildConfig, M_CONFIG);
        throw e;
    }
}

const QString MtcpFileHelper::inputCurrLotName()
{
    const QString TMPath = QApplication::applicationDirPath();
    try {
        QFile file(TMPath + K_CONFIG + lotStartFile);
        if (!file.open(QIODevice::ReadOnly)) {
            std::string err = std::string("OPEN FILE FAILED, ") + (TMPath + K_CONFIG + lotStartFile).toStdString();
            throw std::runtime_error(err);
        }

        QTextStream out(&file);
        QStringList csvLines = out.readAll().split("\n");
        if (csvLines.length() < 4) {
            file.close();
            throw std::runtime_error("READ FILE FAILED, File format error.");
        }

        QStringList csvline = csvLines.at(3).split(',');
        if (csvline.length() < 12) {
            file.close();
            throw std::runtime_error("READ FILE FAILED, File format error.");
        }
        return csvline.at(10);
    }
    catch (std::runtime_error& e) {
        fileChecksum(TMPath + K_CONFIG + lotStartFile, M_START);
        throw e;
    }
}

const QString MtcpFileHelper::outputLotStart(const QString& lotName)
{
    const QString TMPath = QApplication::applicationDirPath();
    try {
        QFile file(TMPath + K_CONFIG + lotStartFile);
        if (!file.open(QIODevice::ReadWrite)) {
            std::string err = std::string("OPEN FILE FAILED, ") + (TMPath + K_CONFIG + lotStartFile).toStdString();
            throw std::runtime_error(err);
        }

        QTextStream out(&file);
        QStringList csvLines = out.readAll().split("\n");
        if (csvLines.length() < 4) {
            file.close();
            throw std::runtime_error("READ FILE FAILED, File format error.");
        }

        QStringList csvline = csvLines.at(3).split(',');
        if (csvline.length() < 12) {
            file.close();
            throw std::runtime_error("READ FILE FAILED, File format error.");
        }
        csvline.replace(10, lotName);
        csvLines.replace(3, csvline.join(","));

        file.resize(0);
        out << csvLines.join("\n");
        file.close();
        return TMPath + K_CONFIG + lotStartFile;
    }
    catch (std::runtime_error& e) {
        fileChecksum(TMPath + K_CONFIG + lotStartFile, M_START);
        throw e;
    }
}

const QString MtcpFileHelper::outputLotEnd(const QString& lotName)
{
    const QString TMPath = QApplication::applicationDirPath();
    try {
        QFile file(TMPath + K_CONFIG + lotEndFile);
        if (!file.open(QIODevice::ReadWrite)) {
            std::string err = std::string("OPEN FILE FAILED, ") + (TMPath + K_CONFIG + lotEndFile).toStdString();
            throw std::runtime_error(err);
        }

        QTextStream out(&file);
        QStringList csvLines = out.readAll().split("\n");
        if (csvLines.length() < 2) {
            file.close();
            throw std::runtime_error("READ FILE FAILED, File format error.");
        }

        QStringList csvline = csvLines.at(1).split(',');
        if (csvline.length() < 12) {
            file.close();
            throw std::runtime_error("READ FILE FAILED, File format error.");
        }
        csvline.replace(10, lotName);
        csvLines.replace(1, csvline.join(","));

        file.resize(0);
        out << csvLines.join("\n");
        file.close();
        return TMPath + K_CONFIG + lotEndFile;
    }
    catch (std::runtime_error& e) {
        fileChecksum(TMPath + K_CONFIG + lotEndFile, M_END);
        throw e;
    }
}

const QString MtcpFileHelper::outputPivotRequest(const QString& sn, const QString& socketSN, const QString& lotName,
                                                 const QString& version)
{
    const QString TMPath = QApplication::applicationDirPath();
    try {
        QFile file(TMPath + K_CONFIG + pivotRequest);
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            std::string err = std::string("OPEN FILE FAILED, ") + (TMPath + K_CONFIG + pivotRequest).toStdString();
            throw std::runtime_error(err);
        }

        QTextStream out(&file);
        QStringList csvLines = out.readAll().split("\n");
        if (csvLines.length() < 24) {
            file.close();
            throw std::runtime_error("READ FILE FAILED, File format error.");
        }

        QStringList csvline = csvLines.at(1).split(',');
        if (csvline.length() < 12) {
            file.close();
            throw std::runtime_error("READ FILE FAILED, File format error.");
        }
        csvline.replace(10, sn);
        csvLines.replace(1, csvline.join(","));

        csvline = csvLines.at(2).split(',');
        QString sock_sn = "1";
        if (socketSN.contains("r", Qt::CaseInsensitive))
            sock_sn = "2";
        if (csvline.length() < 12) {
            file.close();
            throw std::runtime_error("READ FILE FAILED, File format error.");
        }
        csvline.replace(10, sock_sn);
        csvLines.replace(2, csvline.join(","));

        csvline = csvLines.at(3).split(',');
        if (csvline.length() < 12) {
            file.close();
            throw std::runtime_error("READ FILE FAILED, File format error.");
        }
        csvline.replace(10, lotName);
        csvLines.replace(3, csvline.join(","));

        csvline = csvLines.at(6).split(',');
        if (csvline.length() < 12) {
            file.close();
            throw std::runtime_error("READ FILE FAILED, File format error.");
        }
        csvline.replace(10, version);
        csvLines.replace(6, csvline.join(","));

        csvline = csvLines.at(22).split(',');
        if (csvline.length() < 12) {
            file.close();
            throw std::runtime_error("READ FILE FAILED, File format error.");
        }
        csvline.replace(10, sn);
        csvLines.replace(22, csvline.join(","));

        csvline = csvLines.at(23).split(',');
        if (csvline.length() < 12) {
            file.close();
            throw std::runtime_error("READ FILE FAILED, File format error.");
        }
        csvline.replace(10, sock_sn);
        csvLines.replace(23, csvline.join(","));

        file.resize(0);
        out << csvLines.join("\n");
        file.close();
        return TMPath + K_CONFIG + pivotRequest;
    }
    catch (std::runtime_error& e) {
        fileChecksum(TMPath + K_CONFIG + pivotRequest, M_PIVOT);
        throw e;
    }
}
