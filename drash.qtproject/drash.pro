SOURCES += \
    ../src/main.cpp \
    ../src/graphics/cdebugrenderer.cpp \
    ../src/sceneobjects/csceneobject.cpp \
    ../src/cscene.cpp \
    ../src/cvec2.cpp \
    ../src/clogger.cpp \
    ../src/capp.cpp \
    ../src/ctimer.cpp \
    ../src/ccontactlistener.cpp \
    ../src/sceneobjects/ccamera.cpp \
    ../src/sceneobjects/csolidbody.cpp \
    ../src/canimatedparam.cpp \
    ../src/sceneobjects/cdrashbody.cpp \
    ../src/clist.cpp \
    ../src/sceneobjects/cplayer.cpp \
    ../src/cplayerevent.cpp \
    ../src/sceneobjects/cbullet.cpp \
    ../src/ctestapp.cpp \
    ../src/ctestapp1.cpp \
    ../src/ctestapp2.cpp


CONFIG += console
CONFIG -= qt

unix|win32: CONFIG += link_pkgconfig
unix|win32: PKGCONFIG += sdl gl
unix|win32: LIBS += -lBox2D -lrt

HEADERS += \
    ../src/graphics/cdebugrenderer.h \
    ../src/sceneobjects/csceneobject.h \
    ../src/cscene.h \
    ../src/cvec2.h \
    ../src/clogger.h \
    ../src/capp.h \
    ../src/ctimer.h \
    ../src/ccontactlistener.h\
    ../src/sceneobjects/ccamera.h \
    ../src/sceneobjects/csolidbody.h \
    ../src/canimatedparam.h \
    ../src/sceneobjects/cdrashbody.h \
    ../src/clist.h \
    ../src/sceneobjects/cplayer.h \
    ../src/sceneobjects.h \
    ../src/cplayerevent.h \
    ../src/sceneobjects/cbullet.h \
    ../src/ctestapp.h \
    ../src/ctestapp1.h \
    ../src/ctestapp2.h

CONFIG(debug, debug|release) {
    DEFINES += DRASH_DEBUG
}
