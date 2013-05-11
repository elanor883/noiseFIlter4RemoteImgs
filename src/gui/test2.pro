#-------------------------------------------------
#
# Project created by QtCreator 2013-04-07T16:56:17
#
#-------------------------------------------------

QT       += core gui

TARGET = test2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../filter/filter.cpp

HEADERS  += mainwindow.h \
    ../filter/filter.h

FORMS    += mainwindow.ui


INCLUDEPATH +=  /opt/ros/fuerte/include/
LIBS += -L/opt/ros/fuerte/lib -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_ts -lopencv_video -lopencv_videostab
OPENCV_BUILD_3RDPARTY_LIBS=TRUE
