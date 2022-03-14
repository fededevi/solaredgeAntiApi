TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lcurl

SOURCES += \
        daikincontroller.cpp \
        httprequest.cpp \
        main.cpp \
        solaredgerequest.cpp

HEADERS += \
    daikincontroller.h \
    httprequest.h \
    json/single_include/nlohmann/json.hpp \
    solaredgerequest.h

# Install rules for binary file
target.path = /home/pi
!isEmpty(target.path): INSTALLS += target
