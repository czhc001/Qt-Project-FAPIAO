#-------------------------------------------------
#
# Project created by QtCreator 2018-11-22T19:50:27
#
#-------------------------------------------------

QT       += core gui network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FAPIAO
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
    mainwindow.cpp \
    loginwindow.cpp \
    alertdialog.cpp \
    managementwindow.cpp \
    adduserdialog.cpp \
    modifyuserdialog.cpp \
    mycontroller.cpp \
    yesruledialog.cpp \
    rrdialog.cpp \
    run.cpp \
    qocr.cpp

HEADERS += \
    mainwindow.h \
    loginwindow.h \
    alertdialog.h \
    managementwindow.h \
    adduserdialog.h \
    modifyuserdialog.h \
    mycontroller.h \
    yesruledialog.h \
    rrdialog.h \
    run.h \
    head.h \
    qocr.h


INCLUDEPATH += "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0\include"
INCLUDEPATH += "D:\Caffe\NugetPackages\OpenCV.2.4.10\build\native\include\opencv2"
INCLUDEPATH += "D:\Caffe\NugetPackages\OpenCV.2.4.10\build\native\include\opencv"
INCLUDEPATH += "D:\Caffe\NugetPackages\OpenCV.2.4.10\build\native\include"
INCLUDEPATH += "D:\Caffe\NugetPackages\OpenBLAS.0.2.14.1\lib\native\include"
INCLUDEPATH += "D:\Caffe\NugetPackages\protobuf-v120.2.6.1\build\native\include\google"
INCLUDEPATH += "D:\Caffe\NugetPackages\protobuf-v120.2.6.1\build\native\include"
INCLUDEPATH += "D:\Caffe\NugetPackages\glog.0.3.3.0\build\native\include\glog"
INCLUDEPATH += "D:\Caffe\NugetPackages\glog.0.3.3.0\build\native\include"
INCLUDEPATH += "D:\Caffe\NugetPackages\gflags.2.1.2.1\build\native\include\gflags"
INCLUDEPATH += "D:\Caffe\NugetPackages\gflags.2.1.2.1\build\native\include"
INCLUDEPATH += "D:\Caffe\NugetPackages\boost.1.59.0.0\lib\native\include\boost"
INCLUDEPATH += "D:\Caffe\NugetPackages\boost.1.59.0.0\lib\native\include"
INCLUDEPATH += "D:\Caffe\caffe\include\caffe"
INCLUDEPATH += "D:\Caffe\caffe\include"


LIBS += -L$$PWD/lib -lopencv_calib3d2410
LIBS += -L$$PWD/lib -lopencv_contrib2410
LIBS += -L$$PWD/lib -lopencv_core2410
LIBS += -L$$PWD/lib -lopencv_features2d2410
LIBS += -L$$PWD/lib -lopencv_flann2410
LIBS += -L$$PWD/lib -lopencv_gpu2410
LIBS += -L$$PWD/lib -lopencv_highgui2410
LIBS += -L$$PWD/lib -lopencv_imgproc2410
LIBS += -L$$PWD/lib -lopencv_legacy2410
LIBS += -L$$PWD/lib -lopencv_ml2410
LIBS += -L$$PWD/lib -lopencv_objdetect2410
LIBS += -L$$PWD/lib -lopencv_ts2410
LIBS += -L$$PWD/lib -lopencv_video2410
LIBS += -L$$PWD/lib -lcaffe
LIBS += -L$$PWD/lib -llibcaffe
LIBS += -L$$PWD/lib -llibglog
LIBS += -L$$PWD/lib -lgflags
LIBS += -L$$PWD/lib libopenblas.dll.a
LIBS += -L$$PWD/lib -llibprotobuf
LIBS += -L$$PWD/lib -lleveldb
LIBS += -L$$PWD/lib -llmdb
LIBS += -L$$PWD/lib -lhdf5
LIBS += -L$$PWD/lib -lhdf5_hl
LIBS += -L$$PWD/lib -llibboost_date_time-vc120-mt-1_59
LIBS += -L$$PWD/lib -llibboost_filesystem-vc120-mt-1_59
LIBS += -L$$PWD/lib -llibboost_system-vc120-mt-1_59
LIBS += -L$$PWD/lib -llibboost_thread-vc120-mt-1_59
LIBS += -L$$PWD/lib -llibboost_chrono-vc120-mt-1_59

FORMS += \
    mainwindow.ui \
    loginwindow.ui \
    alertdialog.ui \
    managementwindow.ui \
    adduserdialog.ui \
    modifyuserdialog.ui \
    yesruledialog.ui \
    rrdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    icon.qrc

RC_ICONS = 512.ico
