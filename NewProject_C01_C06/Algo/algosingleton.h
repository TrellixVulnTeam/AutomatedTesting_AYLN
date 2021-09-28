#ifndef ALGOSINGLETON_H
#define ALGOSINGLETON_H

#include <QObject>
#include <QDebug>
#include <QMutex>
#if defined(_MSC_VER)
#include "stdafx.h"
#endif

#include "ml_error.h"
#include "ml_utils.h"
#include "ml_csv.h"
#include "ml_file.h"

#include "targetChamber.h"  // ONLY NEED targetChamber.h
#include "DaenerysCore.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <mutex>

using namespace std;
class AlgoSingleTon : public QObject
{
    Q_OBJECT
public:
    AlgoSingleTon(QObject* parent = nullptr) : QObject(parent) {}  //防止外部调用构造创建对象
    ~AlgoSingleTon();                                              //析构函数
    AlgoSingleTon(const AlgoSingleTon&);                           //阻止拷贝创建对象
    AlgoSingleTon& operator=(const AlgoSingleTon&);                //阻止赋值对象
public:
    static AlgoSingleTon* getInstance();
    void demo_loadTargetCsv(const char* csvFile, double** t, double** targetV, size_t* count, int skipRow = 1);
    void demo_loadTargetCsvNew(const char* csvFile, double** t, double** targetV, size_t* count);
    void splitWithSpecialString(const std::string& str, const std::string& pattern, std::string& cloum1,
                                std::string& colum2);
    void algo_dcReleaseSystem();
    t_mlError dynTargetDemo();
    t_mlError dynTargetEnvDemo();
    t_mlError jptProcessSingleDemo(QString filePath, QString fileName, QString outputPath, double velcoty,
                                   float targetAngle);
    t_mlError jptProcessSingle(const QString& outputPath, double velcoty, float targetAngle,
                               const std::vector<double*>& timeData, const std::vector<double*>& voltageData,
                               const std::vector<int>& dataSize);
    t_mlError jptProcessDualStreamDemo();
    t_mlError jptProcessDualStreamFromFileDemo(int slot);
    t_mlError jptProcessSingleNoiseDemo(QString filePath, QString fileName, QString outputPath);
    t_mlError jptProcessSingleNoise(const QString& outputPath, const std::vector<double*>& timeData,
                                    const std::vector<double*>& voltageData, const std::vector<int>& dataSize);
    t_mlError retroProcessSingleDemo(QString filePath, QString fileName, QString outputPath, double vrpm,
                                     float targetAngle);
    t_mlError retroProcessSingleNoiseDemo(QString filePath, QString fileName, QString outputPath);

    t_mlError jptProcessSingleDemoFromStream(QString dataStream, QString outputPath);
    t_mlError jptProcessSingleNoiseDemoFromStream(QString dataStream, QString outputPath);
    void getDataFromStream(QString dataStream, double** t, double** targetV, size_t* count, int skipRow = 1);

    // private:
    //    Q_DISABLE_COPY(AlgoSingleTon)
};

#endif  // ALGOSINGLETON_H
