#-------------------------------------------------
#
# Project created by QtCreator 2012-10-07T13:07:30
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = drash
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../3rd/Box2D/include

SOURCES += ../src/main.cpp\
        ../src/gamewindow.cpp \
    ../src/scenewidget.cpp \
    ../src/cscene.cpp \
    ../src/test/ctestapp3-drashtest1.cpp \
    ../src/test/ctestapp2.cpp \
    ../src/test/ctestapp1.cpp \
    ../src/test/ctestapp.cpp \
    ../src/misc/math.cpp \
    ../src/misc/cvec2.cpp \
    ../src/diag/ctimer.cpp \
    ../src/diag/clogger.cpp \
    ../src/sceneobjects/csceneobject.cpp \
    ../src/sceneobjects/cplayer.cpp \
    ../src/sceneobjects/cgrenade.cpp \
    ../src/sceneobjects/cdrashbody.cpp \
    ../src/sceneobjects/ccamera.cpp \
    ../src/sceneobjects/cbullet.cpp \
    ../src/sceneobjects/cboom.cpp \
    ../src/sound/alsound.cpp

HEADERS  += ../src/gamewindow.h \
    ../src/scenewidget.h \
    ../src/sceneobjects.h \
    ../src/cscene.h \
    ../src/capp.h \
    ../src/test/ctestapp3-drashtest1.h \
    ../src/test/ctestapp2.h \
    ../src/test/ctestapp1.h \
    ../src/test/ctestapp.h \
    ../src/misc/math.h \
    ../src/misc/cvec2.h \
    ../src/misc/canimatedparam.h \
    ../src/graphics/cdebugrenderer.h \
    ../src/diag/ctimer.h \
    ../src/diag/clogger.h \
    ../src/sceneobjects/csceneobject.h \
    ../src/sceneobjects/cplayer.h \
    ../src/sceneobjects/cgrenade.h \
    ../src/sceneobjects/cdrashbody.h \
    ../src/sceneobjects/ccamera.h \
    ../src/sceneobjects/cbullet.h \
    ../src/sceneobjects/cboom.h \
    ../src/sound/alsound.h

FORMS    += ../src/gamewindow.ui

LIBS += -L../3rd/Box2D/lib
LIBS += -lBox2D -lGLU -lopenal -lvorbis -lvorbisfile
