
include(../Library.pri)
include(../MTCPProtocol/MTCPProtocol.pri)

INCLUDEPATH +=  $$PWD

QT       = core

TARGET = MTCPProtocol

INCLUDEPATH += $$PWD/../Libs/include/Python36/
LIBS += -lpython36

QMAKE_POST_LINK = cd $$DESTDIR && move /Y $${TARGET}.dll $${TARGET}.pyd

SOURCES += \
    mtcpprotocol.cpp

