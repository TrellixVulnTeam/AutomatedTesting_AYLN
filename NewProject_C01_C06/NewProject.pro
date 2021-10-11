#-------------------------------------------------
#
# Project created by QtCreator 2021-05-18T09:56:18
#
#-------------------------------------------------

QT       += core gui \
            widgets \
            serialport \
            network \
            script

TARGET = NewProject
TEMPLATE = app
CONFIG += c++11


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += COMMONLIB_LIB

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$PWD/Devices/Devices.pri)
include($$PWD/Tools/Tools.pri)
include($$PWD/UI/UI.pri)
include($$PWD/Algo/Algo.pri)
include($$PWD/User/User.pri)

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    testaction.cpp \
    testplaninfo.cpp \



HEADERS += \
    mainwindow.h \
    testaction.h \
    testplaninfo.h \


FORMS += \
    mainwindow.ui \

RESOURCES += \
    $$PWD/Resources/Project.qrc


DISTFILES += \
    Config//Testflow.csv \
    Config//TestSpec.csv \
    Config//Testflow_DOE1.csv \
    Config//TestSpec_DOE.csv \
    Config//TestSpec_DOE.csv \
    Config/TestSpec_DOE.csv \
    Config//Testflow_DOE.csv \
    Config//Position.csv \
    Config/Config.xml

# only qmake once
defineReplace(remove_extra_config_parameter) {
    configs = $$1
    debug_and_release_params = # 匹配预选队列
    keys = debug Debug release Release debug_and_release
    for (iter, configs) {
        contains(keys, $$iter) {
            debug_and_release_params += $$iter
        }
    }

    for (iter, debug_and_release_params) {
        configs -= $$iter # 移除预选队列的属性
    }

    configs += $$last(debug_and_release_params) # 添加(保留)预选队列的最后属性

    return($$configs)
}
CONFIG = $$remove_extra_config_parameter($$CONFIG)

MOC_DIR = temp/moc
RCC_DIR = temp/rcc
UI_DIR = temp/ui
OBJECTS_DIR = temp/obj


#linux：LIBS += your_lib_path/your_lib动态库
#linux：LIBS += -L your_lib_path -lyour_lib//经过测试了
#win32：LIBS += your_lib_path/your_lib例如：
#LIBS += -L lib/pcsc/ -lpcsclite
#LIBS += lib/pcsc/libpcsclite.a
#2.添加头文件
#INCLUDEPATH += your_include_path例如：
#INCLUDEPATH += . /usr/local/include（点号后面有空格）

#INCLUDEPATH += /usr/local/lib
#LIBS += /usr/local/lib/libInstantPudding


#QMAKE_POST_LINK = cp -a $$PWD/Config.json $$OUT_PWD/NewProject.app/Contents/Resources/Config.json
#QMAKE_POST_LINK = cp -a $$PWD/Testplan.csv $$OUT_PWD/NewProject.app/Contents/Resources/Testplan.csv


macx{
    message(cp -a $$PWD/Config.json $$OUT_PWD/$${TARGET}.app/Contents/Resources/)
    message(cp -a $$PWD/Testplan.csv $$OUT_PWD/$${TARGET}.app/Contents/Resources/)
    QMAKE_POST_LINK = cp -a /Users/h/Desktop/QT/custom/SoftWareFrame/NewProject/Config/Config.json $$OUT_PWD/NewProject.app/Contents/Resources/Config.json&&cp -a /Users/h/Desktop/QT/custom/SoftWareFrame/NewProject/Config/Testplan.csv $$OUT_PWD/NewProject.app/Contents/Resources/Testplan.csv
}

win32{
    # 如果想编译 64bit TEST_MODE设置为Win64,编译 32bit TEST_MODE设置为Win32
    TEST_MODE = Win64

    QMAKE_CXXFLAGS_EXCEPTIONS_ON = /EHa
    QMAKE_CXXFLAGS_STL_ON = /EHa

    TM_SOURCE_TREE = $$PWD
    isEmpty(TM_BUILD_TREE) {
        sub_dir = $$_PRO_FILE_PWD_
        sub_dir ~= s,^$$re_escape($$PWD),,
        TM_BUILD_TREE = $$clean_path($$OUT_PWD)
        TM_BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
    }

    TM_APP_PATH = $$TM_BUILD_TREE/Bin
    LIBS_PATH = $$TM_SOURCE_TREE/Libs

    DEFINES += WIN32 _WINDOWS WIN32_LEAN_AND_MEAN
    LIBS += -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32  -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32

    LINK_LIBRARY_PATH = $$LIBS_PATH/Win/$$TEST_MODE
    CONFIG(debug,debug|release){
        DESTDIR = $$TM_APP_PATH/$$TEST_MODE/Debug
        LIBS += -L$$LINK_LIBRARY_PATH/Debug/  \
                      -L$$TM_APP_PATH/$$TEST_MODE/Debug
        LIBS += -llog4cplusUD
        LIBS += -lMtcpProtocol
        Mode = debug
    }else{
        DESTDIR = $$TM_APP_PATH/$$TEST_MODE/Release
        LIBS += -L$$LINK_LIBRARY_PATH/Release/ \
                       -L$$TM_APP_PATH/$$TEST_MODE/Release
        LIBS += -llog4cplusU
        LIBS += -lMtcpProtocol
        Mode = release
    }

    TRANSLATIONS += $$PWD\Resources\Languages\English.ts \
                    $$PWD\Resources\Languages\Chinese.ts
    RC_ICONS = $$TM_SOURCE_TREE/Resources/Icons/JCiT.ico

    LIBS += -L$$LINK_LIBRARY_PATH/Commom/
    INCLUDEPATH += $$LIBS_PATH/include/
    INCLUDEPATH += $$LIBS_PATH/include/Algo/
    INCLUDEPATH += $$LIBS_PATH/include/Mtcp/

    PY_PATH = $$TM_SOURCE_TREE/Python
    PY_PATH ~= s,/,\\,g

    DEST_DIR = $$DESTDIR
    DEST_DIR ~= s,/,\\,g
    Config_DIR = $$DESTDIR\Config
    Config_DIR ~= s,/,\\,g

    LANGUAGES_DIR = $$PWD\Resources\Languages
    LANGUAGES_DIR ~= s,/,\\,g

    THRID_CONFIG = $$PWD\Config
    THRID_CONFIG ~= s,/,\\,g

    THRID_LIB_PATH = $$LIBS_PATH/Win
    THRID_LIB_PATH ~= s,/,\\,g
    TM_APP_PATH ~= s,/,\\,g

    system(mkdir $$Config_DIR)
#    system(copy /y $$THRID_CONFIG\*.xml $$Config_DIR)
#    system(copy /y $$THRID_CONFIG\*.csv $$Config_DIR)
    system(xcopy /y /e $$THRID_CONFIG $$Config_DIR)

    system(mkdir $$DEST_DIR\Python)
    system(xcopy /s /y /e $$PY_PATH $$DEST_DIR\Python)

    system(copy /y $$THRID_LIB_PATH\\$$TEST_MODE\\$$Mode\*.dll $$DEST_DIR)
    system(copy /y $$THRID_LIB_PATH\\$$TEST_MODE\\$$Mode\*.exe $$DEST_DIR)

    system(copy /y $$THRID_LIB_PATH\\$$TEST_MODE\Commom\*.dll $$DEST_DIR)
    system(copy /y $$THRID_LIB_PATH\\$$TEST_MODE\Commom\*.exe $$DEST_DIR)

    system(mkdir $$DEST_DIR\platforms)
    system(xcopy /s/e/y $$THRID_LIB_PATH\\$$TEST_MODE\\$$Mode\platforms\*.* $$DEST_DIR\platforms)

    system(mkdir $$DEST_DIR\imageformats)
    system(xcopy /s /e /y $$THRID_LIB_PATH\\$$TEST_MODE\\$$Mode\imageformats\*.* $$DEST_DIR\imageformats)
    system(copy /y  $$THRID_LIB_PATH\\$$TEST_MODE\\$$Mode\VcredistLib\*.dll $$DEST_DIR)
}
