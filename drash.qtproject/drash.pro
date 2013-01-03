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

LIBS += -L../3rd/Box2D/lib -lBox2D

unix:!macx: LIBS += -lrt -lGLU -lGLEW

CONFIG(debug, debug|release) {
    DEFINES += DRASH_DEBUG
    QMAKE_CXXFLAGS += -O0
}

CONFIG(release, debug|release) {
    DEFINES += DRASH_RELEASE
    QMAKE_CXXFLAGS += -O2
}


SOURCES += ../src/qt/main.cpp\
        ../src/qt/gamewindow.cpp \
    ../src/qt/scenewidget.cpp \
    ../src/debugdrawsystem/camera.cpp \
    ../src/scene/explosion.cpp \
    ../src/test/test.cpp \
    ../src/subsystem/subsystem.cpp \
    ../src/subsystem/explosionsystem.cpp \
    ../src/subsystem/playerssystem.cpp \
    ../src/editor/editorwindow.cpp \
    ../src/misc/graphics.cpp \
    ../src/subsystem/templatesystem.cpp \
    ../src/scene/physobserver.cpp \
    ../src/scene/figure.cpp \
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
    ../src/scene/destructiongraph.cpp \
    ../src/test/test5.cpp \
    ../src/misc/plane.cpp \
    ../src/misc/ray.cpp \
    ../src/misc/matrix4.cpp \
    ../src/scene/scene.cpp \
    ../src/test/test3.cpp \
    ../src/test/test2.cpp \
    ../src/test/test1.cpp \
    ../src/scene/sceneobject.cpp \
    ../src/scene/player.cpp \
    ../src/editor/editorapp.cpp \
    ../src/diag/logger.cpp \
    ../src/diag/timer.cpp \
    ../src/misc/segment2.cpp \
    ../src/greng/meshmanager.cpp \
    ../src/greng/renderer.cpp \
    ../src/greng/loadmeshobj.cpp

HEADERS  += ../src/qt/gamewindow.h \
    ../src/qt/scenewidget.h \
    ../src/misc/math.h \
    ../src/misc/animatedparam.h \
    ../src/debugdrawsystem/camera.h \
    ../src/scene/explosion.h \
    ../src/test/test.h \
    ../src/diag/assert.h \
    ../src/subsystem/subsystem.h \
    ../src/subsystem/explosionsystem.h \
    ../src/subsystem/playerssystem.h \
    ../src/editor/editorwindow.h \
    ../src/subsystem/templatesystem.h \
    ../src/misc/graphics.h \
    ../src/scene/physobserver.h \
    ../src/scene/figure.h \
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
    ../src/scene/destructiongraph.h \
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
    ../src/scene/player.h \
    ../src/scene/sceneobject.h \
    ../src/test/test2.h \
    ../src/test/test3.h \
    ../src/scene/scene.h \
    ../src/test/test1.h \
    ../src/misc/segment2.h \
    ../src/misc/color3.h \
    ../src/misc/color4.h \
    ../src/greng/meshmanager.h \
    ../src/greng/vertex.h \
    ../src/greng/mesh.h \
    ../src/greng/renderer.h \
    ../src/greng/loadmeshobj.h

FORMS    += ../src/qt/gamewindow.ui \
    ../src/editor/editorwindow.ui
