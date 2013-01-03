TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../3rd/Box2D/include

LIBS += -L../3rd/Box2D/lib -lBox2D

unix:!macx: LIBS += -lrt -lGLU -lGL -lSDL

CONFIG(debug, debug|release) {
    DEFINES += DRASH_DEBUG
    QMAKE_CXXFLAGS += -O0
}

CONFIG(release, debug|release) {
    DEFINES += DRASH_RELEASE
    QMAKE_CXXFLAGS += -O2
}

SOURCES += \
    ../src/ui/uiwidget.cpp \
    ../src/ui/uisystem.cpp \
    ../src/ui/uislider.cpp \
    ../src/ui/uicontrol.cpp \
    ../src/ui/uibutton.cpp \
    ../src/test/test5.cpp \
    ../src/test/test4.cpp \
    ../src/test/test3.cpp \
    ../src/test/test2.cpp \
    ../src/test/test1.cpp \
    ../src/test/test.cpp \
    ../src/subsystem/templatesystem.cpp \
    ../src/subsystem/subsystem.cpp \
    ../src/subsystem/playerssystem.cpp \
    ../src/subsystem/explosionsystem.cpp \
    ../src/scene/sceneobject.cpp \
    ../src/scene/scene.cpp \
    ../src/scene/player.cpp \
    ../src/scene/physobserver.cpp \
    ../src/scene/figure.cpp \
    ../src/scene/explosion.cpp \
    ../src/scene/destructiongraph.cpp \
    ../src/misc/segment2.cpp \
    ../src/misc/ray.cpp \
    ../src/misc/plane.cpp \
    ../src/misc/matrix4.cpp \
    ../src/misc/graphics.cpp \
    ../src/joints/joint.cpp \
    ../src/greng/rendererbufferextension.cpp \
    ../src/greng/renderer.cpp \
    ../src/greng/meshmanager.cpp \
    ../src/diag/timer.cpp \
    ../src/diag/logger.cpp \
    ../src/debugdrawsystem/debugdrawsystem.cpp \
    ../src/debugdrawsystem/camera.cpp \
    ../src/app/appeventsystem.cpp \
    ../src/app/appeventprocessor.cpp \
    ../src/app/appeventcombination.cpp \
    ../src/app/appevent.cpp \
    ../src/app/app.cpp \
    ../src/sdl/main.cpp

HEADERS += \
    ../src/ui/uiwidget.h \
    ../src/ui/uisystem.h \
    ../src/ui/uislider.h \
    ../src/ui/uicontrol.h \
    ../src/ui/uibutton.h \
    ../src/test/test5.h \
    ../src/test/test4.h \
    ../src/test/test3.h \
    ../src/test/test2.h \
    ../src/test/test1.h \
    ../src/test/test.h \
    ../src/subsystem/templatesystem.h \
    ../src/subsystem/subsystem.h \
    ../src/subsystem/playerssystem.h \
    ../src/subsystem/explosionsystem.h \
    ../src/scene/sceneobject.h \
    ../src/scene/scene.h \
    ../src/scene/player.h \
    ../src/scene/physobserver.h \
    ../src/scene/figure.h \
    ../src/scene/explosion.h \
    ../src/scene/destructiongraph.h \
    ../src/misc/vec4.h \
    ../src/misc/vec3.h \
    ../src/misc/vec2.h \
    ../src/misc/segment2.h \
    ../src/misc/ray.h \
    ../src/misc/plane.h \
    ../src/misc/matrix4.h \
    ../src/misc/math.h \
    ../src/misc/graphics.h \
    ../src/misc/color4.h \
    ../src/misc/color3.h \
    ../src/misc/animatedparam.h \
    ../src/joints/joint.h \
    ../src/greng/vertex.h \
    ../src/greng/rendererbufferextension.h \
    ../src/greng/renderer.h \
    ../src/greng/meshmanager.h \
    ../src/greng/mesh.h \
    ../src/diag/timer.h \
    ../src/diag/logger.h \
    ../src/diag/assert.h \
    ../src/debugdrawsystem/debugdrawsystem.h \
    ../src/debugdrawsystem/camera.h \
    ../src/app/appeventsystem.h \
    ../src/app/appeventprocessor.h \
    ../src/app/appeventcombination.h \
    ../src/app/appevent.h \
    ../src/app/app.h

