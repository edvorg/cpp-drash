TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../3rd/Box2D/include

LIBS += -L../3rd/Box2D/lib -lBox2D

unix:!macx: LIBS += -lrt -lGL -lSDL -lGLEW -lSDL_image

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
    ../src/scene/geometrymanager.cpp \
    ../src/scene/sceneobject.cpp \
    ../src/scene/scene.cpp \
    ../src/scene/figure.cpp \
    ../src/misc/ray.cpp \
    ../src/misc/plane.cpp \
    ../src/misc/matrix4.cpp \
    ../src/greng/renderer.cpp \
    ../src/greng/meshmanager.cpp \
    ../src/diag/timer.cpp \
    ../src/diag/logger.cpp \
    ../src/greng/camera.cpp \
    ../src/appeventsystem/appeventsystem.cpp \
    ../src/appeventsystem/appeventprocessor.cpp \
    ../src/appeventsystem/appeventcombination.cpp \
    ../src/appeventsystem/appevent.cpp \
    ../src/sdl/main.cpp \
    ../src/greng/loadmeshobj.cpp \
    ../src/greng/texturemanager.cpp \
    ../src/greng/vertexshadermanager.cpp \
    ../src/greng/fragmentshadermanager.cpp \
    ../src/greng/shaderprogrammanager.cpp \
    ../src/misc/vec2.cpp \
    ../src/scene/joint.cpp \
    ../src/greng/cameramanager.cpp \
    ../src/misc/moveablepoint.cpp \
    ../src/misc/rotationablepoint.cpp \
    ../src/greng/grengsystemsset.cpp \
    ../src/greng/spotlight.cpp \
    ../src/root/root.cpp \
    ../src/root/screen.cpp \
    ../src/mainscreen/mainscreen.cpp

HEADERS += \
    ../src/ui/uiwidget.h \
    ../src/ui/uisystem.h \
    ../src/ui/uislider.h \
    ../src/ui/uicontrol.h \
    ../src/ui/uibutton.h \
    ../src/scene/geometrymanager.h \
    ../src/scene/sceneobject.h \
    ../src/scene/scene.h \
    ../src/scene/figure.h \
    ../src/misc/vec4.h \
    ../src/misc/vec3.h \
    ../src/misc/vec2.h \
    ../src/misc/ray.h \
    ../src/misc/plane.h \
    ../src/misc/matrix4.h \
    ../src/misc/math.h \
    ../src/misc/color4.h \
    ../src/misc/color3.h \
    ../src/greng/vertex.h \
    ../src/greng/renderer.h \
    ../src/greng/meshmanager.h \
    ../src/greng/mesh.h \
    ../src/diag/timer.h \
    ../src/diag/logger.h \
    ../src/diag/assert.h \
    ../src/greng/camera.h \
    ../src/appeventsystem/appeventsystem.h \
    ../src/appeventsystem/appeventprocessor.h \
    ../src/appeventsystem/appeventcombination.h \
    ../src/appeventsystem/appevent.h \
    ../src/greng/loadmeshobj.h \
    ../src/greng/texturemanager.h \
    ../src/greng/texture.h \
    ../src/misc/objectfactory.h \
    ../src/greng/vertexshadermanager.h \
    ../src/greng/vertexshader.h \
    ../src/greng/fragmentshader.h \
    ../src/greng/fragmentshadermanager.h \
    ../src/greng/shaderprogram.h \
    ../src/greng/shaderprogrammanager.h \
    ../src/greng/pointlight.h \
    ../src/misc/animator.h \
    ../src/scene/joint.h \
    ../src/greng/cameramanager.h \
    ../src/misc/moveablepoint.h \
    ../src/misc/rotationablepoint.h \
    precompiled.h \
    ../src/precompiled.h \
    ../src/greng/grengsystemsset.h \
    ../src/greng/spotlight.h \
    ../src/root/root.h \
    ../src/root/screen.h \
    ../src/mainscreen/mainscreen.h

