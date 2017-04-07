#-------------------------------------------------
#
# Project created by QtCreator 2017-03-31T14:21:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ray-Tracer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14

SOURCES += main.cpp\
        mainwindow.cpp \
    ../Ray-Tracing/base_object.cpp \
    ../Ray-Tracing/camera.cpp \
    ../Ray-Tracing/geometry.cpp \
    ../Ray-Tracing/ray_tracer.cpp \
    ../Ray-Tracing/scene.cpp \
    ../Ray-Tracing/light_source.cpp \
    painter.cpp

HEADERS  += mainwindow.h \
    ../Ray-Tracing/base_object.h \
    ../Ray-Tracing/camera.h \
    ../Ray-Tracing/color.h \
    ../Ray-Tracing/geometry.h \
    ../Ray-Tracing/light_source.h \
    ../Ray-Tracing/ray_tracer.h \
    ../Ray-Tracing/scene.h \
    ../Ray-Tracing/solid_object.h \
    painter.h \
    ../Ray-Tracing/bound_box.h

FORMS    += mainwindow.ui

DISTFILES += \
    input \
    scene01.rt \
    input4.txt \
    input246.txt
