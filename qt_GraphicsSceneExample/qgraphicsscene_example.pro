TEMPLATE = app
TARGET = simle_qgraphics_example
INCLUDEPATH += .

QT += core gui widgets

SOURCES += main.cpp \
    random_points.cpp \
    scene.cpp \
    star.cpp

HEADERS += \
    random_points.h \
    scene.h \
    star.h
