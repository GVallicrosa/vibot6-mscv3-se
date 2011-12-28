QT      += core gui

TARGET   = gui
TEMPLATE = app


SOURCES += main.cpp \
           gui.cpp

HEADERS += gui.h

FORMS   += gui.ui \
           options.ui


# stole from opencv
SOURCES += window_QT.cpp
HEADERS += window_QT.h


# ihls_nhs
INCLUDEPATH += ../ihls_nhs/
DEPENDPATH  += ../ihls_nhs/
SOURCES += ihls.cpp nhs.cpp math_utils.cpp
HEADERS += ihls.h   nhs.h   math_utils.h

## post_processing
INCLUDEPATH += ../post_processing/
DEPENDPATH  += ../post_processing/
SOURCES += PostProcessing.cpp
HEADERS += PostProcessing.h

## rational_supershape_2d
INCLUDEPATH += ../rational_supershape_2d/
DEPENDPATH  += ../rational_supershape_2d/
SOURCES += rationalsupershape2d.cpp
HEADERS += rationalsupershape2d.h irationalsupershape2d.h

## RotationalOffset
INCLUDEPATH += ../RotationalOffset/
DEPENDPATH  += ../RotationalOffset/
SOURCES += RotationalOffset.cpp
HEADERS += RotationalOffset.h


## 3rdparty
INCLUDEPATH += ../3rdparty/
DEPENDPATH += ../3rdparty/


## For windows
#win32 {
#    INCLUDEPATH += ""
#    LIBS += ""
#}

# For Linux
unix {
    # using pkg-config
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}

RESOURCES += \
    window_QT.qrc
