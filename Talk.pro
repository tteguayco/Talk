TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    socket.cpp

HEADERS += \
    socket.h

unix:!macx: LIBS += -lpthread-2.23
