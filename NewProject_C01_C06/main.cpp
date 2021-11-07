#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QMessageBox>
#include <QFile>
#include <QIcon>
#include <QDesktopWidget>
#include <exception>
#include <sstream>
#include "LogHelper.h"
#include "ConfigParse.h"
#include "Util.h"
#include "MiniDumper.h"

int main(int argc, char* argv[])
{
    QApplication::setAttribute(Qt::AA_UseOpenGLES);
    QApplication app(argc, argv);

    SetUnhandledExceptionFilter(ExceptionFilter);

    try {
        QApplication::setStyle(QStyleFactory::create("Fusion"));
#ifdef Q_OS_MAC
        struct rlimit rl;
        getrlimit(RLIMIT_NOFILE, &rl);

        rl.rlim_cur = qMin((rlim_t)5000, rl.rlim_max);
        setrlimit(RLIMIT_NOFILE, &rl);

        QString assfile = ":/Qss/Stylesheet/Mac.qss";
        QFont font("Arial", 12);
        QFile qssFile(assfile);
#else
        QString assfile = ":/Qss/Stylesheet/Win.qss";
        QFont font("Arial,SimSun", 10);
        QFile qssFile(assfile);
#endif
        app.setWindowIcon(QIcon(":/Debug/Debug/logo.png"));

        app.setFont(font);
        QString qss;
        qssFile.open(QFile::ReadOnly);

        if (qssFile.isOpen()) {
            qss = QLatin1String(qssFile.readAll());
            qApp->setStyleSheet(qss);
            qssFile.close();
        }

#ifdef Q_OS_MAC
        QString configPath = QApplication::applicationDirPath().replace("MacOS", "Resources/Config.xml");
#else;
        QString configPath = QApplication::applicationDirPath() + "/Config/Config.xml";
#endif

        INIT_LOG("D:\\vault\\JCiT\\TestManagerLog\\TestManager.log");
        CFG_PARSE.prestrain(configPath.toStdString());
        LOG_INFO("[Main] Preloads the configuration file");

        QString logPath = QString::fromStdString(CFG_PARSE.getLogPath()) + "TestManagerLog/";
        Util::MakeNDir(logPath.toStdString());
        logPath.append("TestManager.log");
        UNINT_LOG();
        INIT_LOG(logPath.toStdString().c_str());

        MainWindow w;
        w.resize(QSize(1560, 950));
        w.show();

        return app.exec();
    }
    catch (const std::exception& e) {
        std::ostringstream errStr;
        errStr << e.what() << std::endl << "The program will exit now!" << std::endl;
        QMessageBox::critical(nullptr, "Critical question happened", errStr.str().c_str());
        exit(-1);
    }
}
