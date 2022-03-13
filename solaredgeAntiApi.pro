TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lcurl

SOURCES += \
        httprequest.cpp \
        main.cpp \
        solaredgerequest.cpp

HEADERS += \
    httprequest.h \
    solaredgerequest.h
