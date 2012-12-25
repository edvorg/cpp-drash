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
    ../src/debugdrawsystem/camera.cpp \
    ../src/sceneobjects/explosion.cpp \
    ../src/test/test.cpp \
    ../src/subsystem/subsystem.cpp \
    ../src/subsystem/explosionsystem.cpp \
    ../src/subsystem/playerssystem.cpp \
    ../src/editor/editorwindow.cpp \
    ../src/misc/graphics.cpp \
    ../src/subsystem/templatesystem.cpp \
    ../src/physobserver.cpp \
    ../src/sceneobjects/figure.cpp \
    ../src/debugdrawsystem/debugdrawsystem.cpp \
    ../src/app/app.cpp \
    ../src/app/appevent.cpp \
    ../src/editor/sceneeditor.cpp\
   ../src/joints/joint.cpp \
    ../src/app/appeventsystem.cpp \
    ../src/app/appeventprocessor.cpp \
    ../src/app/appeventcombination.cpp \
    ../src/ui/uisystem.cpp \
    ../src/ui/uicontrol.cpp \
    ../src/test/test4.cpp \
    ../src/ui/uiwidget.cpp \
    ../src/ui/uibutton.cpp \
    ../src/ui/uislider.cpp \
    ../src/sceneobjects/destructiongraph.cpp \
    ../src/test/test5.cpp \
    ../src/misc/vec3.cpp \
    ../src/misc/plane.cpp \
    ../src/misc/ray.cpp \
    ../src/misc/matrix4.cpp \
    ../src/misc/vec4.cpp \
    ../src/misc/vec2.cpp \
    ../src/scene.cpp \
    ../src/test/test3.cpp \
    ../src/test/test2.cpp \
    ../src/test/test1.cpp \
    ../src/sceneobjects/sceneobject.cpp \
    ../src/sceneobjects/player.cpp \
    ../src/editor/editorapp.cpp \
    ../src/diag/logger.cpp \
    ../src/diag/timer.cpp

HEADERS  += ../src/gamewindow.h \
    ../src/scenewidget.h \
    ../src/misc/math.h \
    ../src/misc/animatedparam.h \
    ../src/debugdrawsystem/camera.h \
    ../src/sceneobjects/explosion.h \
    ../src/test/test.h \
    ../src/diag/assert.h \
    ../src/subsystem/subsystem.h \
    ../src/subsystem/explosionsystem.h \
    ../src/subsystem/playerssystem.h \
    ../src/editor/editorwindow.h \
    ../src/subsystem/templatesystem.h \
    ../src/misc/graphics.h \
    ../src/physobserver.h \
    ../src/sceneobjects/figure.h \
    ../src/debugdrawsystem/debugdrawsystem.h \
    ../src/app/app.h \
    ../src/app/appevent.h \
    ../src/editor/sceneeditor.h \
    ../src/joints/joint.h \
    ../src/app/appeventsystem.h \
    ../src/app/appeventprocessor.h \
    ../src/app/appeventcombination.h \
    ../src/ui/uisystem.h \
    ../src/ui/uicontrol.h \
    ../src/test/test4.h \
    ../src/ui/uiwidget.h \
    ../src/ui/uibutton.h \
    ../src/ui/uislider.h \
    ../src/sceneobjects/destructiongraph.h \
    ../src/test/test5.h \
    ../src/misc/vec3.h \
    ../src/misc/plane.h \
    ../src/misc/ray.h \
    ../src/misc/matrix4.h \
    ../src/misc/vec4.h \
    ../src/misc/vec2.h \
    ../src/diag/logger.h \
    ../src/diag/timer.h \
    ../src/editor/editorapp.h \
    ../src/sceneobjects/player.h \
    ../src/sceneobjects/sceneobject.h \
    ../src/test/test2.h \
    ../src/test/test3.h \
    ../src/scene.h \
    ../src/test/test1.h

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
