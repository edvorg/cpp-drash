SOURCES += \
    ../src/main.cpp \
    ../src/cdebugrenderer.cpp \
    ../src/csceneobject.cpp \
    ../src/cscene.cpp \
    ../src/cvec2.cpp \
    ../src/clogger.cpp \
    ../src/cobjectsolidbody.cpp

CONFIG += console
CONFIG -= qt

unix|win32: CONFIG += link_pkgconfig
unix|win32: PKGCONFIG += sdl gl
unix|win32: LIBS += -lBox2D

HEADERS += \
    ../src/cdebugrenderer.h \
    ../src/csceneobject.h \
    ../src/cscene.h \
    ../src/cvec2.h \
    ../src/clogger.h \
    ../src/cobjectsolidbody.h

