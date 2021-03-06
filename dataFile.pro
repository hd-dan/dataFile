TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
        main.cpp \
    data_file.cpp \
    df_file.cpp \
    xml_file.cpp

HEADERS += \
    data_file.h \
    df_file.h \
    xml_file.h

macx {
    INCLUDEPATH += /usr/local/Cellar/boost/1.70.0/include/
    LIBS += -L/usr/local/Cellar/boost/1.70.0/lib -lboost_thread-mt
}
unix:!macx {
    LIBS += -lboost_thread
}
LIBS += -lboost_system
