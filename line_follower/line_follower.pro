## \file
# This file is used by qmake to generate a Makefile for the project.

# Define the Qt modules used by the project.
QT       += core gui serialport

# Conditionally add the Qt widgets module for Qt versions greater than 4.
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Set the C++ standard used by the project.
CONFIG += c++17
CONFIG += create_docs

# Optionally disable deprecated Qt APIs.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Define the project source files.
SOURCES += \
    main.cpp \
    mainwindow.cpp

# Define the project header files.
HEADERS += \
    mainwindow.h

# Define the project UI files.
FORMS += \
    mainwindow.ui

# Define the project translation files.
TRANSLATIONS += \
    line_follower_en_150.ts

# Enable the lrelease tool to generate the translations.
CONFIG += lrelease

# Embed the translations in the application binary.
CONFIG += embed_translations

# Define the installation path for the application binary.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    background.jpg \
    background.png \
    bgg.jpg \
    model.jpg \
    model_small.png \
    movingroad.gif \
    real_bg.png
