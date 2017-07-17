TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    socket.cpp \
    client.cpp

HEADERS += \
    socket.h \
    client.h

unix:!macx: LIBS += -lpthread-2.23
