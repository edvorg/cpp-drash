SOURCES += \
    ../src/main.cpp \
    ../src/cdebugrenderer.cpp \
    ../src/csceneobject.cpp \
    ../src/cscene.cpp \
    ../src/cvec2.cpp \
    ../src/clogger.cpp \
    ../src/capp.cpp \
    ../src/ctimer.cpp \
    ../src/ccirclebody.cpp \
    ../src/ccamera.cpp \
    ../src/csolidbody.cpp

CONFIG += console
CONFIG -= qt

unix|win32: CONFIG += link_pkgconfig
unix|win32: PKGCONFIG += sdl gl
unix|win32: LIBS += -lBox2D -lrt

HEADERS += \
    ../src/cdebugrenderer.h \
    ../src/csceneobject.h \
    ../src/cscene.h \
    ../src/cvec2.h \
    ../src/clogger.h \
    ../src/capp.h \
    ../src/ctimer.h \
    ../src/ccamera.h \
    ../src/ccirclebody.h \
    ../src/csolidbody.h

CONFIG(debug, debug|release) {
    DEFINES += DRASH_DEBUG
}
