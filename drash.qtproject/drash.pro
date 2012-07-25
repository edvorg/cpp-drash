SOURCES += \
    ../src/main.cpp \
    ../src/cdebugrenderer.cpp

CONFIG += console
CONFIG -= qt
#CONFIG += link_pkconfig
#PKGCONFIG += sdl
#LIBS += sdl

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += sdl gl
unix: LIBS += -lBox2D

HEADERS += \
    ../src/cdebugrenderer.h
