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

INCLUDEPATH += ../3rd/Box2D/include\
    ../src/

SOURCES += ../src/main.cpp\
        ../src/gamewindow.cpp \
    ../src/scenewidget.cpp \
    ../src/cscene.cpp \
    ../src/test/ctestapp3-drashtest1.cpp \
    ../src/test/ctestapp2.cpp \
    ../src/test/ctestapp1.cpp \
    ../src/misc/cvec2.cpp \
    ../src/diag/ctimer.cpp \
    ../src/diag/clogger.cpp \
    ../src/sceneobjects/csceneobject.cpp \
    ../src/sceneobjects/cplayer.cpp \
    ../src/sceneobjects/cdrashbody.cpp \
    ../src/sceneobjects/ccamera.cpp \
    ../src/sceneobjects/explosion.cpp \
    ../src/test/test.cpp \
    ../src/test/ctestapp4.cpp \
    ../src/subsystem/subsystem.cpp \
    ../src/subsystem/explosionsystem.cpp \
    ../src/subsystem/playerssystem.cpp \
    ../src/editor/editorwindow.cpp \
    ../src/editor/editorobject.cpp \
    ../src/misc/graphics.cpp \
    ../src/subsystem/templatesystem.cpp \
    ../src/physobserver.cpp \
    ../src/sceneobjects/figure.cpp \
    ../src/subsystem/debugdrawsystem.cpp \
    ../src/test/app.cpp \
    ../src/test/appevent.cpp \
    ../src/editor/sceneeditor.cpp\
   ../src/joints/joint.cpp \
    ../src/editor/ceditorapp.cpp \
    ../src/test/appeventprocessor.cpp

HEADERS  += ../src/gamewindow.h \
    ../src/scenewidget.h \
    ../src/sceneobjects.h \
    ../src/cscene.h \
    ../src/test/ctestapp3-drashtest1.h \
    ../src/test/ctestapp2.h \
    ../src/test/ctestapp1.h \
    ../src/misc/math.h \
    ../src/misc/cvec2.h \
    ../src/misc/canimatedparam.h \
    ../src/diag/ctimer.h \
    ../src/diag/clogger.h \
    ../src/sceneobjects/csceneobject.h \
    ../src/sceneobjects/cplayer.h \
    ../src/sceneobjects/cdrashbody.h \
    ../src/sceneobjects/ccamera.h \
    ../src/sceneobjects/explosion.h \
    ../src/test/test.h \
    ../src/diag/assert.h \
    ../src/test/ctestapp4.h \
    ../src/subsystem/subsystem.h \
    ../src/subsystem/explosionsystem.h \
    ../src/subsystem/playerssystem.h \
    ../src/editor/editorwindow.h \
    ../src/editor/editorobject.h \
    ../src/subsystem/templatesystem.h \
    ../src/misc/graphics.h \
    ../src/physobserver.h \
    ../src/sceneobjects/figure.h \
    ../src/subsystem/debugdrawsystem.h \
    ../src/test/app.h \
    ../src/test/appevent.h \
    ../src/editor/sceneeditor.h \
    ../src/joints/joint.h \
    ../src/editor/ceditorapp.h \
    ../src/test/appeventprocessor.h

FORMS    += ../src/gamewindow.ui \
    ../src/editor/editorwindow.ui

LIBS += -L../3rd/Box2D/lib -lBox2D

unix:!macx: LIBS += -lrt -lGLU

CONFIG(debug, debug|release) {
    DEFINES += DRASH_DEBUG
    QMAKE_CXXFLAGS += -O0
}

CONFIG(release, debug|release) {
    DEFINES += DRASH_RELEASE
    QMAKE_CXXFLAGS += -O2
}
