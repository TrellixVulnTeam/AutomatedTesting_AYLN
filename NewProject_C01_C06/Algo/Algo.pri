
HEADERS += \
#    $$PWD/beamProfilerChamber.h \
#    $$PWD/DaenerysCore.h \
#    $$PWD/livChamber.h \
#    $$PWD/ml_csv.h \
#    $$PWD/ml_error.h \
#    $$PWD/ml_file.h \
#    $$PWD/ml_log.h \
#    $$PWD/ml_log_def.h \
#    $$PWD/ml_math.h \
#    $$PWD/ml_string.h \
#    $$PWD/ml_typedef.h \
#    $$PWD/ml_utils.h \
#    $$PWD/mziChamber.h \
#    $$PWD/targetChamber.h \
#    $$PWD/targetver.h \
    $$PWD/algosingleton.h

SOURCES += \
    $$PWD/algosingleton.cpp

INCLUDEPATH +=  $$PWD

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./ -lDaenerysCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./ -lDaenerysCored
else:unix: LIBS += -L$$PWD/./ -lDaenerysCore

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./ -lMesaLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./ -lMesaLibraryd
else:unix: LIBS += -L$$PWD/./ -lMesaLibrary

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
