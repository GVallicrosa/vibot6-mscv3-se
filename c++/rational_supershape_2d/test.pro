QT       += core console

TARGET = Test
TEMPLATE = app


SOURCES += main.cpp \
           test.cpp \
           rationalsupershape2d.cpp

HEADERS  += test.h \
            rationalsupershape2d.h \
            irationalsupershape2d.h


INCLUDEPATH += ../3rdparty/
DEPENDPATH += ../3rdparty/
