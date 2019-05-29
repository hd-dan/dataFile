TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    data_file.cpp \
    xml_file.cpp

HEADERS += \
    data_file.h \
    xml_file.h

LIBS += -lboost_system -lboost_thread
