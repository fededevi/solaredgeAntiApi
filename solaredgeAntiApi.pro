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
    json/single_include/nlohmann/json.hpp \
    solaredgerequest.h

# Install rules for binary file
target.path = /home/pi
!isEmpty(target.path): INSTALLS += target
