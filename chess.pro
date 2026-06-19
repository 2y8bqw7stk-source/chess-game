QT += core gui widgets

CONFIG += c++17
TEMPLATE = app
TARGET   = JeuEchecs
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    ChessBoard.cpp \
    Plateau.cpp

HEADERS += \
    MainWindow.h \
    ChessBoard.h \
    Plateau.h
CONFIG(release, debug|release): QMAKE_CXXFLAGS += -O2
