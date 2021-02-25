QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp
LIBS += -fopenmp -lgomp

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    atmosphere.cpp \
    grid.cpp \
    helper_functions.cpp \
    integrate.cpp \
    lmfao.cpp \
    main.cpp \
    mainwindow.cpp \
    raytracer.cpp

HEADERS += \
    atmosphere.h \
    cloud.h \
    grid.h \
    helper_functions.h \
    integrate.h \
    lmfao.h \
    mainwindow.h \
    raytracer.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    raytracing_ui.qrc
