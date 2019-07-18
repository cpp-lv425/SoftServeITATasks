TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++17
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        codeanalyzer.cpp \
        main.cpp

HEADERS += \
    codeanalyzer.h \
