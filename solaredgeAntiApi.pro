TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lcurl -lpthread

SOURCES += \
        averager.cpp \
        daikincontroller.cpp \
        daikinload.cpp \
        httprequest.cpp \
        loadinterface.cpp \
        main.cpp \
        solaredgerequest.cpp

HEADERS += \
    averager.h \
    daikincontroller.h \
    daikinload.h \
    httprequest.h \
    json.hpp \
    loadinterface.h \
    solaredgerequest.h

# Install rules for binary file
target.path = /home/pi
!isEmpty(target.path): INSTALLS += target
