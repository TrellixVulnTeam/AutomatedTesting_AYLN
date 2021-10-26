#include "algosingleton.h"

AlgoSingleTon* AlgoSingleTon::getInstance()
{
    static AlgoSingleTon* pAlgo = nullptr;
    if (true /*pAlgo == nullptr*/) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        pAlgo = new AlgoSingleTon();
        qDebug() << "Creat a AlgoSingleTon!";
    }
    return pAlgo;
}

AlgoSingleTon::~AlgoSingleTon() {}

void AlgoSingleTon::algo_dcReleaseSystem()
{
    //    dcReleaseSystem();
    qDebug() << "dcReleaseSystem";
}

void AlgoSingleTon::getDataFromStream(QString dataStream, double** t, double** targetV, size_t* count, int skipRow)
{
    QString str = dataStream;
    size_t fileSz = str.size();
    uint8_t* fileData = NULL;
    fileData = (uint8_t*)qstrdup(str.toLatin1().constData());
    //    delete[] sequence;

    //    size_t fileSz = 0;
    //    uint8_t* fileData = mlFile_readFile(csvFile, &fileSz);
    t_mlCsvDetail fileFormat;
    mlCsv_getDataFormat(NULL, fileData, fileSz, fileFormat);
    size_t maxDataSz = fileFormat.numOfRows();
    *t = (double*)malloc(maxDataSz * sizeof(double));
    *targetV = (double*)malloc(maxDataSz * sizeof(double));

    size_t dataCount = 0;
    for (size_t rIdx = skipRow; rIdx < fileFormat.numOfRows(); rIdx++) {

        const char* rowStr = (char*)fileData + fileFormat.rowStartPos(rIdx);
        size_t itemSize = fileFormat.rowItemSize(rIdx);
        if (itemSize < 2) {
            continue;
        }
        std::string itemT(rowStr + fileFormat.itemStartPos(rIdx, 0), fileFormat.itemLen(rIdx, 0));
        std::string itemVoltage(rowStr + fileFormat.itemStartPos(rIdx, 1), fileFormat.itemLen(rIdx, 1));
        (*t)[dataCount] = std::stod(itemT);
        (*targetV)[dataCount] = std::stod(itemVoltage);
        dataCount++;
    }
    *count = dataCount;
    FREE_MEM(fileData);
}

void AlgoSingleTon::demo_loadTargetCsv(const char* csvFile, double** t, double** targetV, size_t* count, int skipRow)
{
    size_t fileSz = 0;
    uint8_t* fileData = mlFile_readFile(csvFile, &fileSz);
    t_mlCsvDetail fileFormat;
    double st11 = mlUtil_getCurrentTime();

    mlCsv_getDataFormat(NULL, fileData, fileSz, fileFormat);
    size_t maxDataSz = fileFormat.numOfRows();
    *t = (double*)malloc(maxDataSz * sizeof(double));
    *targetV = (double*)malloc(maxDataSz * sizeof(double));

    size_t dataCount = 0;
    for (size_t rIdx = skipRow; rIdx < fileFormat.numOfRows(); rIdx++) {
        const char* rowStr = (char*)fileData + fileFormat.rowStartPos(rIdx);
        size_t itemSize = fileFormat.rowItemSize(rIdx);
        if (itemSize < 2) {
            continue;
        }
        std::string itemT(rowStr + fileFormat.itemStartPos(rIdx, 0), fileFormat.itemLen(rIdx, 0));
        std::string itemVoltage(rowStr + fileFormat.itemStartPos(rIdx, 1), fileFormat.itemLen(rIdx, 1));
        (*t)[dataCount] = std::stod(itemT);
        (*targetV)[dataCount] = std::stod(itemVoltage);
        dataCount++;
    }

    printf_s("*** Process DEMO Data with CSV for fuction ***  in %fs\n", mlUtil_getCurrentTime() - st11);
    *count = dataCount;
    FREE_MEM(fileData);
}

void AlgoSingleTon::demo_loadTargetCsvNew(const char* csvFile, double** t, double** targetV, size_t* count)
{

    double st11 = mlUtil_getCurrentTime();
    size_t maxDataSz = 500000;
    *t = (double*)malloc(maxDataSz * sizeof(double));
    *targetV = (double*)malloc(maxDataSz * sizeof(double));

    ifstream in(csvFile);
    string s;
    string itemT;
    string itemVoltage;

    size_t dataCount = 0;
    while (getline(in, s)) {
        dataCount++;
        itemT = "";
        itemVoltage = "";
        splitWithSpecialString(s, ",", itemT, itemVoltage);
        if (dataCount != 1) {
            (*t)[dataCount - 2] = std::stod(itemT);
            (*targetV)[dataCount - 2] = std::stod(itemVoltage);
        }
    }
    *count = dataCount - 1;
    printf_s("*** Process DEMO Data with CSV for fuction ***  in %fs\n", mlUtil_getCurrentTime() - st11);
}

void AlgoSingleTon::splitWithSpecialString(const std::string& str, const std::string& pattern, std::string& colum1,
                                           std::string& colum2)
{
    std::string strs = str;
    size_t pos = strs.find(pattern);
    size_t size = strs.size();

    colum1 = strs.substr(0, pos);
    colum2 = strs.substr(pos + 1, size);
}

t_mlError AlgoSingleTon::dynTargetDemo()
{
    t_mlError err = kMLErr_OK;
    int fomCnt = 0;
    double* fomData = NULL;
    char** fomName = NULL;
    const char** inputFileGroup = NULL;
    int numOfFiles = 1;

#ifdef __APPLE__
    std::string sampleDataRoot = std::string(mlUtil_GetUserHomePath()) + "/Dev/Drogon_DS3/";
    std::string samplePath = "/DynamicTarget/dtsRaw/";
    std::string sampleFileName1 = "1225_WC_90mm-s_VA1-5KHz__01-07_14-48-21_1.csv";
    std::string sampleFileName2 = "1225_WC_90mm-s_VA1-5KHz__01-07_14-48-26_2.csv";
    std::string sampleFileName3 = "1225_WC_90mm-s_VA1-5KHz__01-07_14-48-30_3.csv";

    std::vector<std::string> sampleCsv(numOfFiles);
    sampleCsv[0] = sampleDataRoot + "/" + samplePath + "/" + sampleFileName1;
    sampleCsv[1] = sampleDataRoot + "/" + samplePath + "/" + sampleFileName2;
    sampleCsv[2] = sampleDataRoot + "/" + samplePath + "/" + sampleFileName3;

    std::string outputDir = std::string(mlUtil_GetUserHomePath()) + "/Downloads/Results/" + mlUtil_getTimestampStr(1);
#else
    std::string sampleDataRoot = "D:\\DemoData\\";
    std::string samplePath = "JPTMaestro\\";
    std::string sampleFileName1 = "75RPM_C01_Test1.csv";
    std::string sampleFileName2 = "75RPM_C01_Test2.csv";
    std::string sampleFileName3 = "AC_Coupled.csv";

    std::vector<std::string> sampleCsv(numOfFiles);
    sampleCsv[0] = sampleDataRoot + "\\" + samplePath + "\\" + sampleFileName1;
    // sampleCsv[1] = sampleDataRoot + "\\" + samplePath + "\\" + sampleFileName2;
    // sampleCsv[2] = sampleDataRoot + "\\" + samplePath + "\\" + sampleFileName3;

    std::string outputDir = "D:\\DemoOut\\" + mlUtil_getTimestampStr(1);
#endif

    std::vector<double*> timeData;
    std::vector<double*> voltageDat;
    std::vector<int> dataSize;
    do {
        printf_s("*** Dynamic Target Demo ***\n");

        double st = mlUtil_getCurrentTime();  // TIMING BLOCK START, time data is stored as double

        inputFileGroup = (const char**)malloc(sizeof(const char*) * numOfFiles);
        for (size_t idx = 0; idx < numOfFiles; idx++) {
            inputFileGroup[idx] = sampleCsv[idx].c_str();

            size_t count = 0;
            double* tData = NULL;
            double* targetVData = NULL;

            demo_loadTargetCsv(sampleCsv[idx].c_str(), &tData, &targetVData, &count,
                               1);  // This is the code to load csv file

            timeData.push_back(tData);
            voltageDat.push_back(targetVData);
            dataSize.push_back((int)count);
        }

        // Setup Inputs to DynTarget (FFT BASED FOMS)

        t_dcDynTargetInput input;
        // input.vMeasured = 100; //11.83f;
        // input.targetAngle = 45.0f;

        int vMeasured_size = sizeof(input.velocity);
        for (int i = 0; i < numOfFiles; i++)
            input.velocity[i] = 7;
        if (numOfFiles == 1)
            input.velocity[1] = 7;
        input.GainTIA = 1.0;
        // input.lambda = 850;
        input.windowFFT = 4e-4;      // 2e-4;
        input.windowFFTIntv = 4e-4;  // input.windowFFT;
        input.cutoffFreq = 3e5;      // 6e5;
        // input.iPdThreshold = -5;
        input.dutSn = "P017";
        input.targetType = k_dcTargetType_whiteCard;
        input.outputDir = outputDir.c_str();
        input.lm = NULL;

        // END Setup of Input parameters

        st = mlUtil_getCurrentTime();  // get current time

        err = dcAlgo_processDynTarget(&input, (const double**)timeData.data(), (const double**)voltageDat.data(),
                                      dataSize.data(), numOfFiles, &fomCnt, &fomData, &fomName);

        // swap code below for loading data from file instead of processing from double arrays.

        // err = dcAlgo_processDynTargetFromFile(&input, inputFileGroup, numOfFiles, &fomCnt, &fomData, &fomName);

        printf_s("*** Process with CSV *** %d, %s in %f\n", err, mlError_getErrorString(err),
                 mlUtil_getCurrentTime() - st);
        printf_s("* FOM Count witg CSV %d\n", fomCnt);
        for (size_t idx = 0; idx < fomCnt; idx++) {
            printf_s("* %f %s\n", fomData[idx], fomName[idx]);
        }

    } while (0);
    for (size_t idx = 0; idx < timeData.size(); idx++) {
        FREE_MEM(timeData[idx]);
        FREE_MEM(voltageDat[idx]);
    }
    for (int idx = 0; idx < numOfFiles; idx++) {
        inputFileGroup[idx] = NULL;
    }
    FREE_MEM(inputFileGroup);
    FREE_MEM(fomData);
    for (size_t idx = 0; idx < fomCnt; idx++) {
        FREE_MEM(fomName[idx]);
    }
    FREE_MEM(fomName);
    return err;
}

t_mlError AlgoSingleTon::dynTargetEnvDemo()
{
    t_mlError err = kMLErr_OK;
    int fomCnt = 0;
    double* fomData = NULL;
    char** fomName = NULL;
    const char** inputFileGroup = NULL;
    int numOfFiles = 1;

#ifdef __APPLE__
    std::string sampleDataRoot = std::string(mlUtil_GetUserHomePath()) + "/Dev/Drogon_DS3/";
    std::string samplePath = "/DynamicTarget/dtsRaw/";
    std::string sampleFileName1 = "1225_WC_90mm-s_VA1-5KHz__01-07_14-48-21_1.csv";
    std::string sampleFileName2 = "1225_WC_90mm-s_VA1-5KHz__01-07_14-48-26_2.csv";
    std::string sampleFileName3 = "1225_WC_90mm-s_VA1-5KHz__01-07_14-48-30_3.csv";

    std::vector<std::string> sampleCsv(numOfFiles);
    sampleCsv[0] = sampleDataRoot + "/" + samplePath + "/" + sampleFileName1;
    sampleCsv[1] = sampleDataRoot + "/" + samplePath + "/" + sampleFileName2;
    sampleCsv[2] = sampleDataRoot + "/" + samplePath + "/" + sampleFileName3;

    std::string outputDir = std::string(mlUtil_GetUserHomePath()) + "/Downloads/Results/" + mlUtil_getTimestampStr(1);
#else
    std::string sampleDataRoot = "D:\\DemoData\\";
    std::string samplePath = "JPTMaestro\\";
    std::string sampleFileName1 = "75RPM_C01_Test1.csv";
    std::string sampleFileName2 = "75RPM_C01_Test5.csv";

    std::vector<std::string> sampleCsv(numOfFiles);
    sampleCsv[0] = sampleDataRoot + "\\" + samplePath + "\\" + sampleFileName1;
    // sampleCsv[1] = sampleDataRoot + "\\" + samplePath + "\\" + sampleFileName2;

    std::string outputDir = "D:\\DemoOut\\" + mlUtil_getTimestampStr(1);
#endif

    std::vector<double*> timeData;
    std::vector<double*> voltageDat;
    std::vector<int> dataSize;
    do {
        printf_s("*** Dynamic Target Demo ***\n");
        double st = mlUtil_getCurrentTime();

        inputFileGroup = (const char**)malloc(sizeof(const char*) * numOfFiles);
        for (size_t idx = 0; idx < numOfFiles; idx++) {
            inputFileGroup[idx] = sampleCsv[idx].c_str();

            size_t count = 0;
            double* tData = NULL;
            double* targetVData = NULL;
            demo_loadTargetCsv(sampleCsv[idx].c_str(), &tData, &targetVData, &count, 1);

            timeData.push_back(tData);
            voltageDat.push_back(targetVData);
            dataSize.push_back((int)count);
        }

        t_dcDynTargetEnvInput input;
        int vMeasured_size = sizeof(input.velocity);
        for (int i = 0; i < numOfFiles; i++)
            input.velocity[i] = 7;
        // input.vMeasured = 100; //11.83f;
        input.targetAngle = 56.8f;
        input.GainTIA = 1.0;  // Use GainTIA = 1 if data is already in uA
        // input.lambda = 0.850; // wavelength is in um. but this is already hardcoded and cannot be changed.
        input.FDthr = 1.414;
        input.dataSamplingRate = 625e3;

        input.dutSn = "P017";
        input.outputDir = outputDir.c_str();
        input.lm = NULL;

        st = mlUtil_getCurrentTime();
        // err = dcAlgo_processDynTargetEnvFromFile(&input, inputFileGroup, numOfFiles, &fomCnt, &fomData, &fomName);
        err = dcAlgo_processDynTargetEnv(&input, (const double**)timeData.data(), (const double**)voltageDat.data(),
                                         dataSize.data(), numOfFiles, &fomCnt, &fomData, &fomName);
        //
        printf_s("*** Process with CSV *** %d, %s in %f\n", err, mlError_getErrorString(err),
                 mlUtil_getCurrentTime() - st);
        printf_s("* FOM Count witg CSV %d\n", fomCnt);
        for (size_t idx = 0; idx < fomCnt; idx++) {
            printf_s("* %f %s\n", fomData[idx], fomName[idx]);
        }

    } while (0);
    for (size_t idx = 0; idx < timeData.size(); idx++) {
        FREE_MEM(timeData[idx]);
        FREE_MEM(voltageDat[idx]);
    }
    for (int idx = 0; idx < numOfFiles; idx++) {
        inputFileGroup[idx] = NULL;
    }
    FREE_MEM(inputFileGroup);
    FREE_MEM(fomData);
    for (size_t idx = 0; idx < fomCnt; idx++) {
        FREE_MEM(fomName[idx]);
    }
    FREE_MEM(fomName);
    return err;
}

t_mlError AlgoSingleTon::jptProcessSingleDemoFromStream(QString dataStream, QString outputPath)
{
    t_mlError err = kMLErr_OK;
    int fomCnt = 0;
    double* fomData = NULL;
    char** fomName = NULL;
    const char** inputFileGroup = NULL;
    int numOfFiles = 1;

#ifdef __APPLE__
    std::string sampleDataRoot = std::string(mlUtil_GetUserHomePath()) + "/Dev/Drogon_DS3/";
    std::string samplePath = "/DynamicTarget/dtsRaw/";
    std::string sampleFileName1 = "1225_WC_90mm-s_VA1-5KHz__01-07_14-48-21_1.csv";
    std::string sampleFileName2 = "1225_WC_90mm-s_VA1-5KHz__01-07_14-48-26_2.csv";
    std::string sampleFileName3 = "1225_WC_90mm-s_VA1-5KHz__01-07_14-48-30_3.csv";

    std::vector<std::string> sampleCsv(numOfFiles);
    sampleCsv[0] = sampleDataRoot + "/" + samplePath + "/" + sampleFileName1;
    sampleCsv[1] = sampleDataRoot + "/" + samplePath + "/" + sampleFileName2;
    sampleCsv[2] = sampleDataRoot + "/" + samplePath + "/" + sampleFileName3;

    std::string outputDir = std::string(mlUtil_GetUserHomePath()) + "/Downloads/Results/" + mlUtil_getTimestampStr(1);
#else
    //    std::string sampleDataRoot = "D:\\DemoData\\";
    //    std::string samplePath = "JPTMaestro\\";
    //    std::string sampleFileName1 = "75RPM_C01_Test1";
    //    std::string sampleFileName2 = "75RPM_C01_Test5";
    //    std::string sampleFileName3 = "AC_Coupled";
    //    std::string sampleFileName4 = "DC_Coupled";
    //    std::string sampleFileName5 = "OFFSET";
    //    std::string slotStr = slot.toStdString();

    std::vector<std::string> sampleCsv(numOfFiles);
    //    sampleCsv[0] = filePath.toStdString() + fileName.toStdString();//sampleDataRoot + "\\" + samplePath + "\\" +
    //    sampleFileName1 + ".csv"; // demo on DC coupled data
    // sampleCsv[1] = sampleDataRoot + "\\" + samplePath + "\\" + sampleFileName2; // jptProcessSingle takes only 1 file
    // atm.

    std::string outputDir = outputPath.toStdString();  //"D:\\DemoOut\\jptDEMO\\SingleStream\\" + slotStr + "\\"  +
                                                       // sampleFileName4 + mlUtil_getTimestampStr(1);
#endif

    std::vector<double*> timeData;
    std::vector<double*> voltageDat;
    std::vector<int> dataSize;
    do {
        printf_s("*** Maestro Tester Demo 2021-03-26 CurrentThreadId:%d***\n", GetCurrentThreadId());
        double st = mlUtil_getCurrentTime();
        double st0 = mlUtil_getCurrentTime();
        inputFileGroup = (const char**)malloc(sizeof(const char*) * numOfFiles);
        for (size_t idx = 0; idx < numOfFiles; idx++) {
            inputFileGroup[idx] = sampleCsv[idx].c_str();

            size_t count = 0;
            double* tData = NULL;
            double* targetVData = NULL;
            //            demo_loadTargetCsv(sampleCsv[idx].c_str(), &tData, &targetVData, &count, 1); // jpt Data
            //            starts at line 2

            getDataFromStream(dataStream, &tData, &targetVData, &count, 1);

            timeData.push_back(tData);
            voltageDat.push_back(targetVData);
            dataSize.push_back((int)count);
        }

        /*
            USE THE FOLLOWING TYPEDEF for dcAlgo_jptProcessSingle(...) This is a faster implementation

        typedef struct t_jptMaestroInput {
            double	velocity[100];		// measured velocity or rpm, vendor input, in mm/s
            float	GainTIA;		    // TIA Gain, in MOhm
            float	targetAngle;	    // AOI incident angle of light on target, depends on Corner
            float 	FDthr;			    // Deprecated, not used, but kept here in case
            float	dataSamplingRate;	// 625KHz for JPT tester
            int		enablePlot;         // Enable Plot or not
            int     isNoiseFoM;         // Toggle between noise and Isig FoM
            const char*	dutSn;		    // DUT Sn used for output result file.
            const char*	outputDir;	    // file path for output result file, set to NULL if no output needed
            CML_logManager*	lm;		    // [Optional] Logger, set to NULL if not needed
        }t_jptMaestroInput;



        */

        t_jptMaestroInput input;

        input.velocity[0] = 7;  // legacy implementation, for possible different v's for multi-file processing
        // input.vMeasured        = 100; //11.83f;
        input.targetAngle = 56.8f;
        input.GainTIA =
            1;  // Use GainTIA = 1 if data is already in uA, test data uses 55e3 for AC gain
                // input.lambda = 0.850; // wavelength is in um. but this is already hardcoded and cannot be changed.
        input.FDthr = 1.414;  // not really used but it needs a number. It will be deprecated if EE doesn't need it.
        input.dataSamplingRate = 625e3;
        input.enablePlot = 1;
        input.isNoiseFoM =
            0;  // this toggles between full FOM calc and noise FOM calc. 0 = full FOM, 1 = noise FOMs only
        input.dutSn = "P017";
        input.outputDir = outputDir.c_str();
        input.lm = NULL;

        st = mlUtil_getCurrentTime();

        err = dcAlgo_jptProcessSingle(&input, (const double**)timeData.data(), (const double**)voltageDat.data(),
                                      dataSize.data(), numOfFiles, &fomCnt, &fomData, &fomName);

        // To load file instead of stream, uncomment the following code and comment out the code above.
        // err = dcAlgo_jptProcessSingleFromFile(&input, inputFileGroup, numOfFiles, &fomCnt, &fomData, &fomName);

        printf_s("*** Process DEMO Data with CSV *** %d, %s in %f st0=%f\n", err, mlError_getErrorString(err),
                 mlUtil_getCurrentTime() - st, mlUtil_getCurrentTime() - st0);
        printf_s("*** Process DEMO Data with CSV *** %d, %s in %f\n", err, mlError_getErrorString(err),
                 mlUtil_getCurrentTime() - st);
        printf_s("* FOM Count with CSV %d\n", fomCnt);
        for (size_t idx = 0; idx < fomCnt; idx++) {
            printf_s("* %f %s\n", fomData[idx], fomName[idx]);
        }

    } while (0);
    for (size_t idx = 0; idx < timeData.size(); idx++) {
        FREE_MEM(timeData[idx]);
        FREE_MEM(voltageDat[idx]);
    }
    for (int idx = 0; idx < numOfFiles; idx++) {
        inputFileGroup[idx] = NULL;
    }
    FREE_MEM(inputFileGroup);
    FREE_MEM(fomData);
    for (size_t idx = 0; idx < fomCnt; idx++) {
        FREE_MEM(fomName[idx]);
    }
    FREE_MEM(fomName);
    return err;
}

t_mlError AlgoSingleTon::jptProcessSingle(const QString& outputPath, double velcoty, float targetAngle,
                                          const std::vector<double*>& timeData, const std::vector<double*>& voltageData,
                                          const std::vector<int>& dataSize)
{
    t_mlError err = kMLErr_OK;
    int fomCnt = 0;
    double* fomData = NULL;
    char** fomName = NULL;
    int numOfFiles = 1;

    std::string outputDir = outputPath.toStdString();
    do {
        t_jptMaestroInput input;

        input.velocity[0] = velcoty;
        input.targetAngle = targetAngle;
        input.GainTIA = 1;
        input.FDthr = 1.414;
        input.dataSamplingRate = 625e3;
        input.enablePlot = 1;
        input.isNoiseFoM = 0;
        input.dutSn = "P017";
        input.outputDir = outputDir.c_str();
        input.lm = NULL;

        try {
            static QMutex mutex;
            QMutexLocker locker(&mutex);
            err = dcAlgo_jptProcessSingle(&input, (const double**)timeData.data(), (const double**)voltageData.data(),
                                          dataSize.data(), numOfFiles, &fomCnt, &fomData, &fomName);
        }
        catch (...) {
            qDebug() << "DLL error when deal data.";
        }

    } while (0);

    FREE_MEM(fomData);
    for (size_t idx = 0; idx < fomCnt; idx++) {
        FREE_MEM(fomName[idx]);
    }
    FREE_MEM(fomName);
    return err;
}

t_mlError AlgoSingleTon::jptProcessSingleDemo(QString filePath, QString fileName, QString outputPath, double velcoty,
                                              float targetAngle)
{
    t_mlError err = kMLErr_OK;
    int fomCnt = 0;
    double* fomData = NULL;
    char** fomName = NULL;
    const char** inputFileGroup = NULL;
    int numOfFiles = 1;

    std::vector<std::string> sampleCsv(numOfFiles);
    sampleCsv[0] = filePath.toStdString() + fileName.toStdString();  // demo on DC coupled data

    std::string outputDir = outputPath.toStdString();

    std::vector<double*> timeData;
    std::vector<double*> voltageDat;
    std::vector<int> dataSize;
    do {
        printf_s("*** Maestro Tester Demo 2021-03-26 CurrentThreadId:%d***\n", GetCurrentThreadId());
        double st = mlUtil_getCurrentTime();
        double st0 = mlUtil_getCurrentTime();
        inputFileGroup = (const char**)malloc(sizeof(const char*) * numOfFiles);
        for (size_t idx = 0; idx < numOfFiles; idx++) {
            inputFileGroup[idx] = sampleCsv[idx].c_str();

            size_t count = 0;
            double* tData = NULL;
            double* targetVData = NULL;
            demo_loadTargetCsv(sampleCsv[idx].c_str(), &tData, &targetVData, &count, 1);  // jpt Data starts at line 2

            timeData.push_back(tData);
            voltageDat.push_back(targetVData);
            dataSize.push_back((int)count);
        }

        /*USE THE FOLLOWING TYPEDEF for dcAlgo_jptProcessSingle(...) This is a faster implementation
        typedef struct t_jptMaestroInput {
            double	velocity[100];		// measured velocity or rpm, vendor input, in mm/s
            float	GainTIA;		    // TIA Gain, in MOhm
            float	targetAngle;	    // AOI incident angle of light on target, depends on Corner
            float 	FDthr;			    // Deprecated, not used, but kept here in case
            float	dataSamplingRate;	// 625KHz for JPT tester
            int		enablePlot;         // Enable Plot or not
            int     isNoiseFoM;         // Toggle between noise and Isig FoM
            const char*	dutSn;		    // DUT Sn used for output result file.
            const char*	outputDir;	    // file path for output result file, set to NULL if no output needed
            CML_logManager*	lm;		    // [Optional] Logger, set to NULL if not needed
        }t_jptMaestroInput;
        */

        t_jptMaestroInput input;

        input.velocity[0] = velcoty;  // legacy implementation, for possible different v's for multi-file processing
        // input.vMeasured        = 100; //11.83f;
        input.targetAngle = targetAngle;
        input.GainTIA =
            1;  // Use GainTIA = 1 if data is already in uA, test data uses 55e3 for AC gain
                // input.lambda = 0.850; // wavelength is in um. but this is already hardcoded and cannot be changed.
        input.FDthr = 1.414;  // not really used but it needs a number. It will be deprecated if EE doesn't need it.
        input.dataSamplingRate = 625e3;
        input.enablePlot = 1;
        input.isNoiseFoM =
            0;  // this toggles between full FOM calc and noise FOM calc. 0 = full FOM, 1 = noise FOMs only
        input.dutSn = "P017";
        input.outputDir = outputDir.c_str();
        input.lm = NULL;

        try {
            err = dcAlgo_jptProcessSingle(&input, (const double**)timeData.data(), (const double**)voltageDat.data(),
                                          dataSize.data(), numOfFiles, &fomCnt, &fomData, &fomName);
        }
        catch (...) {
            qDebug() << "DLL error when deal data.";
        }

        st = mlUtil_getCurrentTime();

        printf_s("*** Process DEMO Data with CSV *** %d, %s in loadcsv = %f algo = %f\n", err,
                 mlError_getErrorString(err), st - st0, mlUtil_getCurrentTime() - st);
        printf_s("* FOM Count with CSV %d\n", fomCnt);
        for (size_t idx = 0; idx < fomCnt; idx++) {
            printf_s("* %f %s\n", fomData[idx], fomName[idx]);
        }

    } while (0);
    for (size_t idx = 0; idx < timeData.size(); idx++) {
        FREE_MEM(timeData[idx]);
        FREE_MEM(voltageDat[idx]);
    }
    for (int idx = 0; idx < numOfFiles; idx++) {
        inputFileGroup[idx] = NULL;
    }
    FREE_MEM(inputFileGroup);
    FREE_MEM(fomData);
    for (size_t idx = 0; idx < fomCnt; idx++) {
        FREE_MEM(fomName[idx]);
    }
    FREE_MEM(fomName);
    return err;
}

t_mlError AlgoSingleTon::jptProcessDualStreamDemo()
{
    t_mlError err = kMLErr_OK;

    int fomCnt1 = 0;
    double* fomData1 = NULL;
    char** fomName1 = NULL;

    int fomCnt2 = 0;
    double* fomData2 = NULL;
    char** fomName2 = NULL;

    const char** inputFileGroup = NULL;
    int numOfFiles = 3;

    std::string sampleDataRoot = "D:\\DemoData\\";
    std::string samplePath = "JPTMaestro\\";
    std::string sampleFileName1 = "75RPM_C01_Test1";
    std::string sampleFileName2 = "75RPM_C01_Test5";
    std::string sampleFileName3 = "AC_Coupled";
    std::string sampleFileName4 = "DC_Coupled";
    std::string sampleFileName5 = "OFFSET";

    std::vector<std::string> sampleCsv(numOfFiles);
    sampleCsv[0] = sampleDataRoot + "\\" + samplePath + "\\" + sampleFileName3 + ".csv";  // demo on DC coupled data
    sampleCsv[1] = sampleDataRoot + "\\" + samplePath + "\\" + sampleFileName4 + ".csv";  //
    sampleCsv[2] = sampleDataRoot + "\\" + samplePath + "\\" + sampleFileName5 + ".csv";  //

    std::string outputDir1 = "D:\\DemoOut\\jptDEMO\\DualStream\\AC" + mlUtil_getTimestampStr(1);
    std::string outputDir2 = "D:\\DemoOut\\jptDEMO\\DualStream\\DC" + mlUtil_getTimestampStr(1);

    std::vector<double*> timeData1;
    std::vector<double*> voltageDat1;
    std::vector<int> dataSize1;

    std::vector<double*> timeData2;
    std::vector<double*> voltageDat2;
    std::vector<int> dataSize2;

    std::vector<double*> timeData3;
    std::vector<double*> voltageDat3;
    std::vector<int> dataSize3;

    do {
        printf_s("*** Maestro Tester Dual Stream Demo 2021-03-26 ***\n");
        double st = mlUtil_getCurrentTime();  // start timing

        inputFileGroup = (const char**)malloc(sizeof(const char*) * numOfFiles);
        for (size_t idx = 0; idx < numOfFiles; idx++) {
            inputFileGroup[idx] = sampleCsv[idx].c_str();
        }  // not used here

        size_t count1 = 0;
        double* tData1 = NULL;
        double* targetVData1 = NULL;
        demo_loadTargetCsv(sampleCsv[0].c_str(), &tData1, &targetVData1, &count1, 1);  // Load AC coupled file

        size_t count2 = 0;
        double* tData2 = NULL;
        double* targetVData2 = NULL;
        demo_loadTargetCsv(sampleCsv[1].c_str(), &tData2, &targetVData2, &count2, 1);  // load DC coupled file

        size_t count3 = 0;
        double* tData3 = NULL;
        double* targetVData3 = NULL;
        demo_loadTargetCsv(sampleCsv[2].c_str(), &tData3, &targetVData3, &count3, 1);  // load OFFSET coupled file

        timeData1.push_back(tData1);  // Kept so construction is same as other functions
        voltageDat1.push_back(targetVData1);
        dataSize1.push_back((int)count1);

        timeData2.push_back(tData2);
        voltageDat2.push_back(targetVData2);
        dataSize2.push_back((int)count2);

        timeData3.push_back(tData3);
        voltageDat3.push_back(targetVData3);
        dataSize3.push_back((int)count3);

        //		} // for loop

        /*
        USE THE FOLLOWING TYPEDEF for dcAlgo_jptProcessSingle(...) This is a faster implementation

        typedef struct t_jptMaestroInput {
        double	velocity[100];		// measured velocity or rpm, vendor input, in mm/s
        float	GainTIA;		    // TIA Gain, in MOhm
        float	targetAngle;	    // AOI incident angle of light on target, depends on Corner
        float 	FDthr;			    // Deprecated, not used, but kept here in case
        float	dataSamplingRate;	// 625KHz for JPT tester
        int		enablePlot;         // Enable Plot or not
        int     isNoiseFoM;         // Toggle between noise and Isig FoM
        const char*	dutSn;		    // DUT Sn used for output result file.
        const char*	outputDir;	    // file path for output result file, set to NULL if no output needed
        CML_logManager*	lm;		    // [Optional] Logger, set to NULL if not needed
        }t_jptMaestroInput;



        */

        t_jptMaestroInput input1;

        input1.velocity[0] = 7;  // legacy implementation, for possible different v's for multi-file processing
                                 // input.vMeasured        = 100; //11.83f;
        input1.targetAngle = 56.8f;
        input1.GainTIA = 55e-3;  // Use GainTIA = 1 if data is already in uA, test data uses 55e3 for AC gain
                                 // input.lambda = 0.850; // wavelength is in um. but this is already hardcoded and
                                 // cannot be changed.
        input1.FDthr = 1.414;    // not really used but it needs a number. It will be deprecated if EE doesn't need it.
        input1.dataSamplingRate = 625e3;
        input1.enablePlot = 1;
        input1.isNoiseFoM =
            0;  // this toggles between full FOM calc and noise FOM calc. 0 = full FOM, 1 = noise FOMs only
        input1.dutSn = "P017";
        input1.outputDir = outputDir1.c_str();  // OUTPUT FOLDER FOR AC coupled combined data
        input1.lm = NULL;

        t_jptMaestroInput input2;

        input2.velocity[0] = 7;  // legacy implementation, for possible different v's for multi-file processing
                                 // input.vMeasured        = 100; //11.83f;
        input2.targetAngle = 56.8f;
        input2.GainTIA = 55e-3;  // Use GainTIA = 1 if data is already in uA, test data uses 55e3 for AC gain
                                 // input.lambda = 0.850; // wavelength is in um. but this is already hardcoded and
                                 // cannot be changed.
        input2.FDthr = 1.414;    // not really used but it needs a number. It will be deprecated if EE doesn't need it.
        input2.dataSamplingRate = 625e3;
        input2.enablePlot = 1;
        input2.isNoiseFoM =
            0;  // this toggles between full FOM calc and noise FOM calc. 0 = full FOM, 1 = noise FOMs only
        input2.dutSn = "P017";
        input2.outputDir = outputDir2.c_str();  // OUTPUT FOLDER FOR DC coupled combined data
        input2.lm = NULL;

        double ACDCGainRatio = 11;

        st = mlUtil_getCurrentTime();

        err = dcAlgo_ACDCCombineStreams(
            &input1, &input2, (const double**)timeData1.data(), (const double**)voltageDat1.data(),
            (const int*)dataSize1.data(), (const double**)timeData2.data(), (const double**)voltageDat2.data(),
            (const int*)dataSize2.data(), (const double**)timeData3.data(), (const double**)voltageDat3.data(),
            (const int*)dataSize3.data(), (const double)ACDCGainRatio, &fomCnt1, &fomData1, &fomName1, &fomCnt2,
            &fomData2, &fomName2);

        printf_s("*** Process DEMO Data with CSV *** %d, %s in %f\n", err, mlError_getErrorString(err),
                 mlUtil_getCurrentTime() - st);
        printf_s("|------ AC coupled FOMS -------| \n");
        printf_s("* FOM Count with CSV %d\n", fomCnt1);
        for (size_t idx = 0; idx < fomCnt1; idx++) {
            printf_s("* %f %s\n", fomData1[idx], fomName1[idx]);
        }
        printf_s("|------ DC coupled FOMS -------| \n");
        printf_s("* FOM Count with CSV %d\n", fomCnt2);
        for (size_t idx = 0; idx < fomCnt2; idx++) {
            printf_s("* %f %s\n", fomData2[idx], fomName2[idx]);
        }

    } while (0);
    for (size_t idx = 0; idx < timeData1.size(); idx++) {
        FREE_MEM(timeData1[idx]);
        FREE_MEM(voltageDat1[idx]);
        FREE_MEM(timeData2[idx]);
        FREE_MEM(voltageDat2[idx]);
        FREE_MEM(timeData3[idx]);
        FREE_MEM(voltageDat3[idx]);
    }
    for (int idx = 0; idx < numOfFiles; idx++) {
        inputFileGroup[idx] = NULL;
    }
    FREE_MEM(inputFileGroup);
    FREE_MEM(fomData1);
    FREE_MEM(fomData2);
    for (size_t idx = 0; idx < fomCnt1; idx++) {
        FREE_MEM(fomName1[idx]);
    }
    for (size_t idx = 0; idx < fomCnt2; idx++) {
        FREE_MEM(fomName2[idx]);
    }
    FREE_MEM(fomName1);
    FREE_MEM(fomName2);

    return err;
}

t_mlError AlgoSingleTon::jptProcessDualStreamFromFileDemo(int slot)
{
    t_mlError err = kMLErr_OK;

    int fomCnt1 = 0;
    double* fomData1 = NULL;
    char** fomName1 = NULL;

    int fomCnt2 = 0;
    double* fomData2 = NULL;
    char** fomName2 = NULL;

    const char** inputFileGroup = NULL;
    int numOfFiles = 3;
    std::string slotStr = "slot_" + slot;
    std::string sampleDataRoot = "D:\\DemoData\\";
    std::string samplePath = "JPTMaestro\\";
    std::string sampleFileName1 = "75RPM_C01_Test1";
    std::string sampleFileName2 = "75RPM_C01_Test5";
    std::string sampleFileName3 = "AC_Coupled";
    std::string sampleFileName4 = "DC_Coupled";
    std::string sampleFileName5 = "OFFSET";
    std::string sampleFileName6 = "DUT_2_T0_Painted_75RPM_AC_C01_20210414155743";
    std::string sampleFileName7 = "DUT_2_T0_Painted_75RPM_DC_C01_20210414155744";
    std::string sampleFileName8 = "_T0_Painted_75RPM_DC_Offset_20210414155549";
    std::string sampleFileName9 = "F9W114568YS40010_T0_Sn_Coating_75RPM_AC_C01_20210429154010";
    std::string sampleFileName10 = "F9W114568YS40010_T0_Sn_Coating_75RPM_DC_C01_20210429154011";
    std::string sampleFileName11 = "_T0_Painted_75RPM_DC_Offset_20210414155549";
    std::string intermediatePath = "";  // "DLL Error\\\\20210428_Error\\";

    std::vector<std::string> sampleCsv(numOfFiles);
    sampleCsv[0] = sampleDataRoot + "\\" + samplePath + "\\" + intermediatePath + "\\" + sampleFileName6
                   + ".csv";  // demo on DC coupled data
    sampleCsv[1] = sampleDataRoot + "\\" + samplePath + "\\" + intermediatePath + "\\" + sampleFileName7 + ".csv";   //
    sampleCsv[2] = sampleDataRoot + "\\" + samplePath + "\\" + intermediatePath + "\\" + sampleFileName11 + ".csv";  //

    std::string outputDir1 = "D:\\DemoOut\\jptDEMO\\" + slotStr + "DualStream\\AC" + mlUtil_getTimestampStr(1);
    std::string outputDir2 = "D:\\DemoOut\\jptDEMO\\" + slotStr + "DualStream\\DC" + mlUtil_getTimestampStr(1);

    std::cout << " These files will be sent to the DLL.\n";
    std::cout << sampleCsv[0].c_str() << "\n";
    std::cout << sampleCsv[1].c_str() << "\n";
    std::cout << sampleCsv[2].c_str() << "\n";

    do {
        printf_s("*** Maestro Tester Dual Stream Demo From File 2021-04-30 ***\n");
        double st = mlUtil_getCurrentTime();  // start timing

        /*
                USE THE FOLLOWING TYPEDEF for dcAlgo_jptProcessSingle(...) This is a faster implementation

                typedef struct t_jptMaestroInput {
                double	velocity[100];		// measured velocity or rpm, vendor input, in mm/s
                float	GainTIA;		    // TIA Gain, in MOhm
                float	targetAngle;	    // AOI incident angle of light on target, depends on Corner
                float 	FDthr;			    // Deprecated, not used, but kept here in case
                float	dataSamplingRate;	// 625KHz for JPT tester
                int		enablePlot;         // Enable Plot or not
                int     isNoiseFoM;         // Toggle between noise and Isig FoM
                const char*	dutSn;		    // DUT Sn used for output result file.
                const char*	outputDir;	    // file path for output result file, set to NULL if no output needed
                CML_logManager*	lm;		    // [Optional] Logger, set to NULL if not needed
                }t_jptMaestroInput;



        */

        t_jptMaestroInput input1;

        input1.velocity[0] = 7.9348;  // legacy implementation, for possible different v's for multi-file processing
                                      // input.vMeasured        = 100; //11.83f;
        input1.targetAngle = 51.0f;
        input1.GainTIA = 0.005;  // Use GainTIA = 1 if data is already in uA, test data uses 55e3 for AC gain
                                 // input.lambda = 0.850; // wavelength is in um. but this is already hardcoded and
                                 // cannot be changed.
        input1.FDthr = 1.414;    // not really used but it needs a number. It will be deprecated if EE doesn't need it.
        input1.dataSamplingRate = 625e3;
        input1.enablePlot = 1;
        input1.isNoiseFoM =
            0;  // this toggles between full FOM calc and noise FOM calc. 0 = full FOM, 1 = noise FOMs only
        input1.dutSn = "P017";
        input1.outputDir = outputDir1.c_str();  // OUTPUT FOLDER FOR AC coupled combined data
        input1.lm = NULL;

        t_jptMaestroInput input2;

        input2.velocity[0] = 7.9348;  // legacy implementation, for possible different v's for multi-file processing
                                      // input.vMeasured        = 100; //11.83f;
        input2.targetAngle = 51.0f;
        input2.GainTIA = 0.005;  // Use GainTIA = 1 if data is already in uA, test data uses 55e3 for AC gain
                                 // input.lambda = 0.850; // wavelength is in um. but this is already hardcoded and
                                 // cannot be changed.
        input2.FDthr = 1.414;    // not really used but it needs a number. It will be deprecated if EE doesn't need it.
        input2.dataSamplingRate = 625e3;
        input2.enablePlot = 1;
        input2.isNoiseFoM =
            0;  // this toggles between full FOM calc and noise FOM calc. 0 = full FOM, 1 = noise FOMs only
        input2.dutSn = "P017";
        input2.outputDir = outputDir2.c_str();  // OUTPUT FOLDER FOR DC coupled combined data
        input2.lm = NULL;

        double ACDCGainRatio = 1;

        st = mlUtil_getCurrentTime();

        err = dcAlgo_ACDCCombineStreamsFromFile(&input1, &input2, (const char*)sampleCsv[0].c_str(),
                                                (const char*)sampleCsv[1].c_str(), (const char*)sampleCsv[2].c_str(),
                                                (const double)ACDCGainRatio, &fomCnt1, &fomData1, &fomName1, &fomCnt2,
                                                &fomData2, &fomName2);

        printf_s("*** Process DEMO Data with CSV *** %d, %s in %f\n", err, mlError_getErrorString(err),
                 mlUtil_getCurrentTime() - st);
        printf_s("|------ AC coupled FOMS -------| \n");
        printf_s("* FOM Count with CSV %d\n", fomCnt1);
        for (size_t idx = 0; idx < fomCnt1; idx++) {
            printf_s("* %f %s\n", fomData1[idx], fomName1[idx]);
        }
        printf_s("|------ DC coupled FOMS -------| \n");
        printf_s("* FOM Count with CSV %d\n", fomCnt2);
        for (size_t idx = 0; idx < fomCnt2; idx++) {
            printf_s("* %f %s\n", fomData2[idx], fomName2[idx]);
        }

    } while (0);

    FREE_MEM(fomData1);
    FREE_MEM(fomData2);

    for (size_t idx = 0; idx < fomCnt1; idx++) {
        FREE_MEM(fomName1[idx]);
    }

    for (size_t idx = 0; idx < fomCnt2; idx++) {
        FREE_MEM(fomName2[idx]);
    }

    FREE_MEM(fomName1);
    FREE_MEM(fomName2);

    return err;
}

t_mlError AlgoSingleTon::jptProcessSingleNoiseDemoFromStream(QString dataStream, QString outputPath)
{
    printf_s("\n\n=========================jptProcessSingleNoiseDemo============================\n");

    t_mlError err = kMLErr_OK;
    int fomCnt = 0;
    double* fomData = NULL;
    char** fomName = NULL;
    const char** inputFileGroup = NULL;
    int numOfFiles = 1;

    std::string sampleDataRoot = "D:\\DemoData\\";
    std::string samplePath = "JPTMaestro\\";
    std::string sampleFileName1 = "75RPM_C01_Test1";
    std::string sampleFileName2 = "75RPM_C01_Test5";
    std::string sampleFileName3 = "AC_Coupled";
    std::string sampleFileName4 = "DC_Coupled";
    std::string sampleFileName5 = "OFFSET";
    std::string sampleFileName6 = "NOISE";

    std::vector<std::string> sampleCsv(numOfFiles);
    //    sampleCsv[0] = filePath.toStdString() + fileName.toStdString();//sampleDataRoot + "\\" + samplePath + "\\" +
    //    sampleFileName6 + ".csv"; // demo on Noise data

    std::string outputDir = outputPath.toStdString();  //"D:\\DemoOut\\jptDEMO\\Noise\\" + mlUtil_getTimestampStr(1);

    std::vector<double*> timeData;
    std::vector<double*> voltageDat;
    std::vector<int> dataSize;
    do {
        printf_s("*** Maestro Tester Noise FoM Demo 2021-03-26 ***\n");
        double st = mlUtil_getCurrentTime();

        inputFileGroup = (const char**)malloc(sizeof(const char*) * numOfFiles);
        for (size_t idx = 0; idx < numOfFiles; idx++) {
            inputFileGroup[idx] = sampleCsv[idx].c_str();

            size_t count = 0;
            double* tData = NULL;
            double* targetVData = NULL;
            //            demo_loadTargetCsv(sampleCsv[idx].c_str(), &tData, &targetVData, &count, 1); // jpt Data
            //            starts at line 2

            getDataFromStream(dataStream, &tData, &targetVData, &count, 1);

            timeData.push_back(tData);
            voltageDat.push_back(targetVData);
            dataSize.push_back((int)count);
        }

        /*
        USE THE FOLLOWING TYPEDEF for dcAlgo_jptProcessSingle(...) This is a faster implementation

        typedef struct t_jptMaestroInput {
        double	velocity[100];		// measured velocity or rpm, vendor input, in mm/s
        float	GainTIA;		    // TIA Gain, in MOhm
        float	targetAngle;	    // AOI incident angle of light on target, depends on Corner
        float 	FDthr;			    // Deprecated, not used, but kept here in case
        float	dataSamplingRate;	// 625KHz for JPT tester
        int		enablePlot;         // Enable Plot or not
        int     isNoiseFoM;         // Toggle between noise and Isig FoM
        const char*	dutSn;		    // DUT Sn used for output result file.
        const char*	outputDir;	    // file path for output result file, set to NULL if no output needed
        CML_logManager*	lm;		    // [Optional] Logger, set to NULL if not needed
        }t_jptMaestroInput;



        */

        t_jptMaestroInput input;

        input.velocity[0] = 7;  // legacy implementation, for possible different v's for multi-file processing
                                // input.vMeasured        = 100; //11.83f;
        input.targetAngle = 56.8f;
        input.GainTIA = 55e-3;  // Use GainTIA = 1 if data is already in uA, test data uses 55e3 for AC gain
                                // input.lambda = 0.850; // wavelength is in um. but this is already hardcoded and
                                // cannot be changed.
        input.FDthr = 1.414;    // not really used but it needs a number. It will be deprecated if EE doesn't need it.
        input.dataSamplingRate = 625e3;
        input.enablePlot = 1;
        input.isNoiseFoM =
            1;  // this toggles between full FOM calc and noise FOM calc. 0 = full FOM, 1 = noise FOMs only
        input.dutSn = "P017";
        input.outputDir = outputDir.c_str();
        input.lm = NULL;

        st = mlUtil_getCurrentTime();

        err = dcAlgo_jptProcessSingle(&input, (const double**)timeData.data(), (const double**)voltageDat.data(),
                                      dataSize.data(), numOfFiles, &fomCnt, &fomData, &fomName);

        printf_s("*** Process DEMO Data with CSV *** %d, %s in %f\n", err, mlError_getErrorString(err),
                 mlUtil_getCurrentTime() - st);
        printf_s("* FOM Count with CSV %d\n", fomCnt);
        for (size_t idx = 0; idx < fomCnt; idx++) {
            printf_s("* %f %s\n", fomData[idx], fomName[idx]);
        }

    } while (0);

    printf_s("\n\n=========================jptProcessSingleNoiseDemo============================\n\n");
    for (size_t idx = 0; idx < timeData.size(); idx++) {
        FREE_MEM(timeData[idx]);
        FREE_MEM(voltageDat[idx]);
    }
    for (int idx = 0; idx < numOfFiles; idx++) {
        inputFileGroup[idx] = NULL;
    }
    FREE_MEM(inputFileGroup);
    FREE_MEM(fomData);
    for (size_t idx = 0; idx < fomCnt; idx++) {
        FREE_MEM(fomName[idx]);
    }
    FREE_MEM(fomName);
    return err;
}

t_mlError AlgoSingleTon::jptProcessSingleNoise(const QString& outputPath, const std::vector<double*>& timeData,
                                               const std::vector<double*>& voltageData,
                                               const std::vector<int>& dataSize)
{
    t_mlError err = kMLErr_OK;
    int fomCnt = 0;
    double* fomData = NULL;
    char** fomName = NULL;
    int numOfFiles = 1;

    std::string outputDir = outputPath.toStdString();
    do {
        t_jptMaestroInput input;

        input.velocity[0] = 7.93;
        input.targetAngle = 51.0f;
        input.GainTIA = 1;
        input.FDthr = 1.414;
        input.dataSamplingRate = 625e3;
        input.enablePlot = 1;
        input.isNoiseFoM = 1;
        input.dutSn = "P017";
        input.outputDir = outputDir.c_str();
        input.lm = NULL;

        static QMutex mutex;
        QMutexLocker locker(&mutex);
        err = dcAlgo_jptProcessSingle(&input, (const double**)timeData.data(), (const double**)voltageData.data(),
                                      dataSize.data(), numOfFiles, &fomCnt, &fomData, &fomName);

    } while (0);

    for (size_t idx = 0; idx < fomCnt; idx++) {
        printf_s("* %f %s\n", fomData[idx], fomName[idx]);
    }

    FREE_MEM(fomData);
    for (size_t idx = 0; idx < fomCnt; idx++) {
        FREE_MEM(fomName[idx]);
    }
    FREE_MEM(fomName);
    return err;
}

t_mlError AlgoSingleTon::jptProcessSingleNoiseDemo(QString filePath, QString fileName, QString outputPath)
{

    printf_s("\n\n=========================jptProcessSingleNoiseDemo============================\n");

    t_mlError err = kMLErr_OK;
    int fomCnt = 0;
    double* fomData = NULL;
    char** fomName = NULL;
    const char** inputFileGroup = NULL;
    int numOfFiles = 1;

    //    std::string sampleDataRoot = "D:\\DemoData\\";
    //    std::string samplePath = "JPTMaestro\\";
    //    std::string sampleFileName1 = "75RPM_C01_Test1";
    //    std::string sampleFileName2 = "75RPM_C01_Test5";
    //    std::string sampleFileName3 = "AC_Coupled";
    //    std::string sampleFileName4 = "DC_Coupled";
    //    std::string sampleFileName5 = "OFFSET";
    //    std::string sampleFileName6 = "NOISE";

    std::vector<std::string> sampleCsv(numOfFiles);
    sampleCsv[0] = filePath.toStdString() + fileName.toStdString();  // sampleDataRoot + "\\" + samplePath + "\\" +
                                                                     // sampleFileName6 + ".csv"; // demo on Noise data

    std::string outputDir = outputPath.toStdString();  //"D:\\DemoOut\\jptDEMO\\Noise\\" + mlUtil_getTimestampStr(1);

    std::vector<double*> timeData;
    std::vector<double*> voltageDat;
    std::vector<int> dataSize;
    do {
        printf_s("*** Maestro Tester Noise FoM Demo 2021-03-26 ***\n");
        double st = mlUtil_getCurrentTime();

        inputFileGroup = (const char**)malloc(sizeof(const char*) * numOfFiles);
        for (size_t idx = 0; idx < numOfFiles; idx++) {
            inputFileGroup[idx] = sampleCsv[idx].c_str();

            size_t count = 0;
            double* tData = NULL;
            double* targetVData = NULL;
            demo_loadTargetCsv(sampleCsv[idx].c_str(), &tData, &targetVData, &count, 1);  // jpt Data starts at line 2

            //            demo_loadTargetCsvNew(sampleCsv[idx].c_str(), &tData, &targetVData, &count);

            timeData.push_back(tData);
            voltageDat.push_back(targetVData);
            dataSize.push_back((int)count);
        }

        /*
        USE THE FOLLOWING TYPEDEF for dcAlgo_jptProcessSingle(...) This is a faster implementation

        typedef struct t_jptMaestroInput {
        double	velocity[100];		// measured velocity or rpm, vendor input, in mm/s
        float	GainTIA;		    // TIA Gain, in MOhm
        float	targetAngle;	    // AOI incident angle of light on target, depends on Corner
        float 	FDthr;			    // Deprecated, not used, but kept here in case
        float	dataSamplingRate;	// 625KHz for JPT tester
        int		enablePlot;         // Enable Plot or not
        int     isNoiseFoM;         // Toggle between noise and Isig FoM
        const char*	dutSn;		    // DUT Sn used for output result file.
        const char*	outputDir;	    // file path for output result file, set to NULL if no output needed
        CML_logManager*	lm;		    // [Optional] Logger, set to NULL if not needed
        }t_jptMaestroInput;



        */

        t_jptMaestroInput input;

        input.velocity[0] = 7.93;  // legacy implementation, for possible different v's for multi-file processing
                                   // input.vMeasured        = 100; //11.83f;
        input.targetAngle = 51.0f;
        input.GainTIA = 1;    // Use GainTIA = 1 if data is already in uA, test data uses 55e3 for AC gain
                              // input.lambda = 0.850; // wavelength is in um. but this is already hardcoded and
                              // cannot be changed.
        input.FDthr = 1.414;  // not really used but it needs a number. It will be deprecated if EE doesn't need it.
        input.dataSamplingRate = 625e3;
        input.enablePlot = 1;
        input.isNoiseFoM =
            1;  // this toggles between full FOM calc and noise FOM calc. 0 = full FOM, 1 = noise FOMs only
        input.dutSn = "P017";
        input.outputDir = outputDir.c_str();
        input.lm = NULL;

        st = mlUtil_getCurrentTime();

        err = dcAlgo_jptProcessSingle(&input, (const double**)timeData.data(), (const double**)voltageDat.data(),
                                      dataSize.data(), numOfFiles, &fomCnt, &fomData, &fomName);

        printf_s("*** Process DEMO Data with CSV *** %d, %s in %f\n", err, mlError_getErrorString(err),
                 mlUtil_getCurrentTime() - st);
        printf_s("* FOM Count with CSV %d\n", fomCnt);
        for (size_t idx = 0; idx < fomCnt; idx++) {
            printf_s("* %f %s\n", fomData[idx], fomName[idx]);
        }

    } while (0);

    printf_s("\n\n=========================jptProcessSingleNoiseDemo============================\n\n");
    for (size_t idx = 0; idx < timeData.size(); idx++) {
        FREE_MEM(timeData[idx]);
        FREE_MEM(voltageDat[idx]);
    }
    for (int idx = 0; idx < numOfFiles; idx++) {
        inputFileGroup[idx] = NULL;
    }
    FREE_MEM(inputFileGroup);
    FREE_MEM(fomData);
    for (size_t idx = 0; idx < fomCnt; idx++) {
        FREE_MEM(fomName[idx]);
    }
    FREE_MEM(fomName);
    return err;
}

t_mlError AlgoSingleTon::retroProcessSingleDemo(QString filePath, QString fileName, QString outputPath, double vrpm,
                                                float targetAngle)
{
    printf_s("\n\n=========================retroProcessSingleDemo============================\n");

    t_mlError err = kMLErr_OK;
    int fomCnt = 0;
    double* fomData = NULL;
    char** fomName = NULL;
    const char** inputFileGroup = NULL;
    int numOfFiles = 1;

    std::vector<std::string> sampleCsv(numOfFiles);
    sampleCsv[0] = filePath.toStdString() + fileName.toStdString();

    std::string outputDir = outputPath.toStdString();

    std::vector<double*> timeData;
    std::vector<double*> voltageDat;
    std::vector<int> dataSize;
    do {
        printf_s("*** Retro Maestro Tester Demo 2021-06-25 ***\n");
        double st = mlUtil_getCurrentTime();

        inputFileGroup = (const char**)malloc(sizeof(const char*) * numOfFiles);
        for (size_t idx = 0; idx < numOfFiles; idx++) {
            inputFileGroup[idx] = sampleCsv[idx].c_str();

            size_t count = 0;
            double* tData = NULL;
            double* targetVData = NULL;

            printf("sampleCsv[%d].c_str():%s\n", idx, sampleCsv[idx].c_str());
            demo_loadTargetCsv(sampleCsv[idx].c_str(), &tData, &targetVData, &count, 1);  // jpt Data starts at line 2

            timeData.push_back(tData);
            voltageDat.push_back(targetVData);
            dataSize.push_back((int)count);
        }

        /*
        USE THE FOLLOWING TYPEDEF for dcAlgo_retroProcessSingle(...) This is a faster implementation

        typedef struct t_retroMaestroInput {
        double	Vrpm;		        // DUT RPM
        float	GainTIA;		    // TIA Gain, in MOhm
        float	targetAngle;	    // AOI incident angle of light on target, depends on Corner
        double 	threshold;			// EE to define threshold for percentage FOMs
        float	dataSamplingRate;	// 625KHz for JPT tester
        int		enablePlot;         // Enable Plot or not - not yet implemented.
        int     isNoiseFoM;         // Toggle between noise and Isig FoM
        const char*	dutSn;		    // DUT Sn used for output result file.
        const char*	outputDir;	    // file path for output result file, set to NULL if no output needed
        CML_logManager*	lm;		    // [Optional] Logger, set to NULL if not needed
        }t_retroMaestroInput;



        */

        t_retroMaestroInput input;

        //        input.Vrpm = 60;
        //        input.targetAngle = 56.8f;
        input.Vrpm = vrpm;
        input.targetAngle = targetAngle;
        input.GainTIA = 1;
        input.threshold = 0.6;  // EE to defined threshold
        input.dataSamplingRate = 625e3;
        input.enablePlot = 0;
        input.isNoiseFoM = 0;
        input.dutSn = "OKP1";
        input.outputDir = outputDir.c_str();
        input.lm = NULL;

        st = mlUtil_getCurrentTime();
        err = dcAlgo_retroProcessSingle(&input, (const double**)timeData.data(), (const double**)voltageDat.data(),
                                        dataSize.data(), numOfFiles, &fomCnt, &fomData, &fomName);

        printf_s("*** Process DEMO Data with CSV *** %d, %s in %f\n", err, mlError_getErrorString(err),
                 mlUtil_getCurrentTime() - st);
        printf_s("* FOM Count with CSV %d\n", fomCnt);
        for (size_t idx = 0; idx < fomCnt; idx++) {
            printf_s("* %f %s\n", fomData[idx], fomName[idx]);
        }

    } while (0);

    printf_s("\n=========================retroProcessSingleDemo============================\n\n");
    for (size_t idx = 0; idx < timeData.size(); idx++) {
        FREE_MEM(timeData[idx]);
        FREE_MEM(voltageDat[idx]);
    }
    for (int idx = 0; idx < numOfFiles; idx++) {
        inputFileGroup[idx] = NULL;
    }
    FREE_MEM(inputFileGroup);
    FREE_MEM(fomData);
    for (size_t idx = 0; idx < fomCnt; idx++) {
        FREE_MEM(fomName[idx]);
    }
    FREE_MEM(fomName);
    return err;
}

t_mlError AlgoSingleTon::retroProcessSingleNoiseDemo(QString filePath, QString fileName, QString outputPath)
{

    printf_s("\n\n=========================jptProcessSingleNoiseDemo============================\n");
    t_mlError err = kMLErr_OK;
    int fomCnt = 0;
    double* fomData = NULL;
    char** fomName = NULL;
    const char** inputFileGroup = NULL;
    int numOfFiles = 1;

    std::vector<std::string> sampleCsv(numOfFiles);
    sampleCsv[0] = filePath.toStdString() + fileName.toStdString();

    std::string outputDir = outputPath.toStdString();

    std::vector<double*> timeData;
    std::vector<double*> voltageDat;
    std::vector<int> dataSize;
    do {
        printf_s("*** Retro Maestro Tester Demo 2021-06-25 ***\n");
        double st = mlUtil_getCurrentTime();

        inputFileGroup = (const char**)malloc(sizeof(const char*) * numOfFiles);
        for (size_t idx = 0; idx < numOfFiles; idx++) {
            inputFileGroup[idx] = sampleCsv[idx].c_str();

            size_t count = 0;
            double* tData = NULL;
            double* targetVData = NULL;
            demo_loadTargetCsv(sampleCsv[idx].c_str(), &tData, &targetVData, &count, 1);  // jpt Data starts at line 2

            timeData.push_back(tData);
            voltageDat.push_back(targetVData);
            dataSize.push_back((int)count);
        }

        /*
        USE THE FOLLOWING TYPEDEF for dcAlgo_retroProcessSingle(...) This is a faster implementation

        typedef struct t_retroMaestroInput {
        double	Vrpm;		        // DUT RPM
        float	GainTIA;		    // TIA Gain, in MOhm
        float	targetAngle;	    // AOI incident angle of light on target, depends on Corner
        double 	threshold;			// EE to define threshold for percentage FOMs
        float	dataSamplingRate;	// 625KHz for JPT tester
        int		enablePlot;         // Enable Plot or not - not yet implemented.
        int     isNoiseFoM;         // Toggle between noise and Isig FoM
        const char*	dutSn;		    // DUT Sn used for output result file.
        const char*	outputDir;	    // file path for output result file, set to NULL if no output needed
        CML_logManager*	lm;		    // [Optional] Logger, set to NULL if not needed
        }t_retroMaestroInput;
        */

        t_retroMaestroInput input;

        input.Vrpm = 60;  // legacy implementation, for possible different v's for multi-file processing
                          // input.vMeasured        = 100; //11.83f;
        input.targetAngle = 51.0f;
        input.GainTIA =
            1;  // Use GainTIA = 1 if data is already in uA, test data uses 55e3 for AC gain
                // input.lambda = 0.850; // wavelength is in um. but this is already hardcoded and cannot be changed.
        input.threshold = 0.6;  // EE to defined threshold
        input.dataSamplingRate = 625e3;
        input.enablePlot = 0;
        input.isNoiseFoM =
            1;  // this toggles between full FOM calc and noise FOM calc. 0 = full FOM, 1 = noise FOMs only
        input.dutSn = "OKP1";
        input.outputDir = outputDir.c_str();
        input.lm = NULL;

        st = mlUtil_getCurrentTime();

        err = dcAlgo_retroProcessSingle(&input, (const double**)timeData.data(), (const double**)voltageDat.data(),
                                        dataSize.data(), numOfFiles, &fomCnt, &fomData, &fomName);

        // To load file instead of stream, uncomment the following code and comment out the code above.
        // err = dcAlgo_jptProcessSingleFromFile(&input, inputFileGroup, numOfFiles, &fomCnt, &fomData, &fomName);

        printf_s("*** Process DEMO Data with CSV *** %d, %s in %f\n", err, mlError_getErrorString(err),
                 mlUtil_getCurrentTime() - st);
        printf_s("* FOM Count with CSV %d\n", fomCnt);
        for (size_t idx = 0; idx < fomCnt; idx++) {
            printf_s("* %f %s\n", fomData[idx], fomName[idx]);
        }

    } while (0);
    printf_s("\n=========================jptProcessSingleNoiseDemo============================\n\n");
    for (size_t idx = 0; idx < timeData.size(); idx++) {
        FREE_MEM(timeData[idx]);
        FREE_MEM(voltageDat[idx]);
    }
    for (int idx = 0; idx < numOfFiles; idx++) {
        inputFileGroup[idx] = NULL;
    }
    FREE_MEM(inputFileGroup);
    FREE_MEM(fomData);
    for (size_t idx = 0; idx < fomCnt; idx++) {
        FREE_MEM(fomName[idx]);
    }
    FREE_MEM(fomName);
    return err;
}
