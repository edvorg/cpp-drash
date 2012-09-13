SOURCES += \
    ../src/main.cpp \
    ../src/graphics/cdebugrenderer.cpp \
    ../src/sceneobjects/csceneobject.cpp \
    ../src/cscene.cpp \
    ../src/diag/clogger.cpp \
    ../src/capp.cpp \
    ../src/sceneobjects/ccamera.cpp \
    ../src/sceneobjects/csolidbody.cpp \
    ../src/sceneobjects/cdrashbody.cpp \
    ../src/misc/clist.cpp \
    ../src/sceneobjects/cplayer.cpp \
    ../src/sceneobjects/cbullet.cpp \
    ../src/test/ctestapp.cpp \
    ../src/test/ctestapp1.cpp \
    ../src/test/ctestapp2.cpp \
    ../src/sceneobjects/cgrenade.cpp \
    ../src/test/ctestapp3-drashtest1.cpp \
    ../src/diag/ctimer.cpp \
    ../src/misc/cvec2.cpp \
    ../src/sceneobjects/cboom.cpp


CONFIG += console
CONFIG -= qt

unix|win32: CONFIG += link_pkgconfig
unix|win32: PKGCONFIG += sdl gl
unix|win32: LIBS += -lBox2D -lrt

QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    ../src/graphics/cdebugrenderer.h \
    ../src/sceneobjects/csceneobject.h \
    ../src/cscene.h \
    ../src/diag/clogger.h \
    ../src/capp.h \
    ../src/sceneobjects/ccamera.h \
    ../src/sceneobjects/csolidbody.h \
    ../src/sceneobjects/cdrashbody.h \
    ../src/misc/clist.h \
    ../src/sceneobjects/cplayer.h \
    ../src/sceneobjects.h \
    ../src/sceneobjects/cbullet.h \
    ../src/test/ctestapp.h \
    ../src/test/ctestapp1.h \
    ../src/test/ctestapp2.h \
    ../src/sceneobjects/cgrenade.h \
    ../src/test/ctestapp3-drashtest1.h \
    ../src/diag/ctimer.h \
    ../src/misc/cvec2.h \
    ../src/misc/canimatedparam.h \
    ../src/sceneobjects/cboom.h

CONFIG(debug, debug|release) {
    DEFINES += DRASH_DEBUG
}
