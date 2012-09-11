SOURCES += \
    ../src/main.cpp \
    ../src/graphics/cdebugrenderer.cpp \
    ../src/sceneobjects/csceneobject.cpp \
    ../src/cscene.cpp \
    ../src/cvec2.cpp \
    ../src/diag/clogger.cpp \
    ../src/capp.cpp \
    ../src/ctimer.cpp \
    ../src/ccontactlistener.cpp \
    ../src/sceneobjects/ccamera.cpp \
    ../src/sceneobjects/csolidbody.cpp \
    ../src/sceneobjects/cdrashbody.cpp \
    ../src/misc/clist.cpp \
    ../src/sceneobjects/cplayer.cpp \
    ../src/cplayerevent.cpp \
    ../src/sceneobjects/cbullet.cpp \
    ../src/test/ctestapp.cpp \
    ../src/test/ctestapp1.cpp \
    ../src/test/ctestapp2.cpp \
    ../src/sceneobjects/cgrenade.cpp


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
    ../src/cvec2.h \
    ../src/diag/clogger.h \
    ../src/capp.h \
    ../src/ctimer.h \
    ../src/ccontactlistener.h\
    ../src/sceneobjects/ccamera.h \
    ../src/sceneobjects/csolidbody.h \
    ../src/canimatedparam.h \
    ../src/sceneobjects/cdrashbody.h \
    ../src/misc/clist.h \
    ../src/sceneobjects/cplayer.h \
    ../src/sceneobjects.h \
    ../src/cplayerevent.h \
    ../src/sceneobjects/cbullet.h \
    ../src/test/ctestapp.h \
    ../src/test/ctestapp1.h \
    ../src/test/ctestapp2.h \
    ../src/cboomparams.h \
    ../src/sceneobjects/cgrenade.h

CONFIG(debug, debug|release) {
    DEFINES += DRASH_DEBUG
}
