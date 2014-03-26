TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../3rd

unix:!macx: LIBS += -lrt -lGL -lSDL -lGLEW -lSDL_image -lopenal -lvorbis -lvorbisfile

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
    ../src/scene/geometrymanager.cpp \
    ../src/players/playerssystem.cpp \
    ../src/explosion/explosionsystem.cpp \
    ../src/scene/sceneobject.cpp \
    ../src/scene/scene.cpp \
	../src/players/player.cpp \
    ../src/scene/figure.cpp \
    ../src/misc/ray.cpp \
    ../src/misc/plane.cpp \
    ../src/misc/matrix4.cpp \
    ../src/greng/renderer.cpp \
    ../src/greng/meshmanager.cpp \
    ../src/diag/timer.cpp \
    ../src/diag/logger.cpp \
    ../src/greng/camera.cpp \
    ../src/app/appeventsystem.cpp \
    ../src/app/appeventprocessor.cpp \
    ../src/app/appeventcombination.cpp \
    ../src/app/appevent.cpp \
    ../src/app/app.cpp \
    ../src/sdl/main.cpp \
    ../src/greng/loadmeshobj.cpp \
    ../src/greng/texturemanager.cpp \
    ../src/greng/vertexshadermanager.cpp \
    ../src/greng/fragmentshadermanager.cpp \
    ../src/greng/shaderprogrammanager.cpp \
    ../src/test/test6.cpp \
    ../src/misc/vec2.cpp \
    ../src/scene/joint.cpp \
    ../src/test/test7.cpp \
    ../src/greng/cameramanager.cpp \
    ../src/debugrenderer/debugrenderer.cpp \
    ../src/misc/moveablepoint.cpp \
    ../src/levelmanager/leveldesc.cpp \
    ../src/levelmanager/levelmanager.cpp \
    ../src/test/test8.cpp \
    ../src/misc/rotationablepoint.cpp \
    ../src/test/test9.cpp \
    ../src/greng/grengsystemsset.cpp \
    ../src/test/test10.cpp \
    ../src/greng/spotlight.cpp \
    ../3rd/Box2D/Collision/b2BroadPhase.cpp \
    ../3rd/Box2D/Collision/b2CollideCircle.cpp \
    ../3rd/Box2D/Collision/b2CollideEdge.cpp \
    ../3rd/Box2D/Collision/b2CollidePolygon.cpp \
    ../3rd/Box2D/Collision/b2Collision.cpp \
    ../3rd/Box2D/Collision/b2Distance.cpp \
    ../3rd/Box2D/Collision/b2DynamicTree.cpp \
    ../3rd/Box2D/Collision/b2TimeOfImpact.cpp \
    ../3rd/Box2D/Collision/Shapes/b2ChainShape.cpp \
    ../3rd/Box2D/Collision/Shapes/b2CircleShape.cpp \
    ../3rd/Box2D/Collision/Shapes/b2EdgeShape.cpp \
    ../3rd/Box2D/Collision/Shapes/b2PolygonShape.cpp \
    ../3rd/Box2D/Common/b2BlockAllocator.cpp \
    ../3rd/Box2D/Common/b2Draw.cpp \
    ../3rd/Box2D/Common/b2Math.cpp \
    ../3rd/Box2D/Common/b2Settings.cpp \
    ../3rd/Box2D/Common/b2StackAllocator.cpp \
    ../3rd/Box2D/Common/b2Timer.cpp \
    ../3rd/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp \
    ../3rd/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
    ../3rd/Box2D/Dynamics/Contacts/b2CircleContact.cpp \
    ../3rd/Box2D/Dynamics/Contacts/b2Contact.cpp \
    ../3rd/Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
    ../3rd/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
    ../3rd/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
    ../3rd/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
    ../3rd/Box2D/Dynamics/Contacts/b2PolygonContact.cpp \
    ../3rd/Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
    ../3rd/Box2D/Dynamics/Joints/b2FrictionJoint.cpp \
    ../3rd/Box2D/Dynamics/Joints/b2GearJoint.cpp \
    ../3rd/Box2D/Dynamics/Joints/b2Joint.cpp \
    ../3rd/Box2D/Dynamics/Joints/b2MotorJoint.cpp \
    ../3rd/Box2D/Dynamics/Joints/b2MouseJoint.cpp \
    ../3rd/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
    ../3rd/Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
    ../3rd/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
    ../3rd/Box2D/Dynamics/Joints/b2RopeJoint.cpp \
    ../3rd/Box2D/Dynamics/Joints/b2WeldJoint.cpp \
    ../3rd/Box2D/Dynamics/Joints/b2WheelJoint.cpp \
    ../3rd/Box2D/Dynamics/b2Body.cpp \
    ../3rd/Box2D/Dynamics/b2ContactManager.cpp \
    ../3rd/Box2D/Dynamics/b2Fixture.cpp \
    ../3rd/Box2D/Dynamics/b2Island.cpp \
    ../3rd/Box2D/Dynamics/b2World.cpp \
    ../3rd/Box2D/Dynamics/b2WorldCallbacks.cpp \
    ../3rd/Box2D/Rope/b2Rope.cpp \
    ../src/trash/alsound.cpp

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
    ../src/scene/geometrymanager.h \
    ../src/players/playerssystem.h \
    ../src/explosion/explosionsystem.h \
    ../src/scene/sceneobject.h \
    ../src/scene/scene.h \
	../src/players/player.h \
    ../src/scene/figure.h \
    ../src/explosion/explosion.h \
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
    ../src/app/appeventsystem.h \
    ../src/app/appeventprocessor.h \
    ../src/app/appeventcombination.h \
    ../src/app/appevent.h \
    ../src/app/app.h \
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
    ../src/test/test6.h \
    ../src/misc/animator.h \
    ../src/scene/joint.h \
    ../src/test/test7.h \
    ../src/greng/cameramanager.h \
    ../src/debugrenderer/debugrenderer.h \
    ../src/misc/moveablepoint.h \
    ../src/levelmanager/leveldesc.h \
    ../src/levelmanager/levelmanager.h \
    ../src/test/test8.h \
    ../src/misc/rotationablepoint.h \
    ../src/test/test9.h \
    precompiled.h \
    ../src/precompiled.h \
    ../src/greng/grengsystemsset.h \
    ../src/test/test10.h \
    ../src/greng/spotlight.h \
    ../3rd/Box2D/Collision/b2BroadPhase.h \
    ../3rd/Box2D/Collision/b2Collision.h \
    ../3rd/Box2D/Collision/b2Distance.h \
    ../3rd/Box2D/Collision/b2DynamicTree.h \
    ../3rd/Box2D/Collision/b2TimeOfImpact.h \
    ../3rd/Box2D/Collision/Shapes/b2ChainShape.h \
    ../3rd/Box2D/Collision/Shapes/b2CircleShape.h \
    ../3rd/Box2D/Collision/Shapes/b2EdgeShape.h \
    ../3rd/Box2D/Collision/Shapes/b2PolygonShape.h \
    ../3rd/Box2D/Collision/Shapes/b2Shape.h \
    ../3rd/Box2D/Common/b2BlockAllocator.h \
    ../3rd/Box2D/Common/b2Draw.h \
    ../3rd/Box2D/Common/b2GrowableStack.h \
    ../3rd/Box2D/Common/b2Math.h \
    ../3rd/Box2D/Common/b2Settings.h \
    ../3rd/Box2D/Common/b2StackAllocator.h \
    ../3rd/Box2D/Common/b2Timer.h \
    ../3rd/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.h \
    ../3rd/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.h \
    ../3rd/Box2D/Dynamics/Contacts/b2CircleContact.h \
    ../3rd/Box2D/Dynamics/Contacts/b2Contact.h \
    ../3rd/Box2D/Dynamics/Contacts/b2ContactSolver.h \
    ../3rd/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.h \
    ../3rd/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.h \
    ../3rd/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h \
    ../3rd/Box2D/Dynamics/Contacts/b2PolygonContact.h \
    ../3rd/Box2D/Dynamics/Joints/b2DistanceJoint.h \
    ../3rd/Box2D/Dynamics/Joints/b2FrictionJoint.h \
    ../3rd/Box2D/Dynamics/Joints/b2GearJoint.h \
    ../3rd/Box2D/Dynamics/Joints/b2Joint.h \
    ../3rd/Box2D/Dynamics/Joints/b2MotorJoint.h \
    ../3rd/Box2D/Dynamics/Joints/b2MouseJoint.h \
    ../3rd/Box2D/Dynamics/Joints/b2PrismaticJoint.h \
    ../3rd/Box2D/Dynamics/Joints/b2PulleyJoint.h \
    ../3rd/Box2D/Dynamics/Joints/b2RevoluteJoint.h \
    ../3rd/Box2D/Dynamics/Joints/b2RopeJoint.h \
    ../3rd/Box2D/Dynamics/Joints/b2WeldJoint.h \
    ../3rd/Box2D/Dynamics/Joints/b2WheelJoint.h \
    ../3rd/Box2D/Dynamics/b2Body.h \
    ../3rd/Box2D/Dynamics/b2ContactManager.h \
    ../3rd/Box2D/Dynamics/b2Fixture.h \
    ../3rd/Box2D/Dynamics/b2Island.h \
    ../3rd/Box2D/Dynamics/b2TimeStep.h \
    ../3rd/Box2D/Dynamics/b2World.h \
    ../3rd/Box2D/Dynamics/b2WorldCallbacks.h \
    ../3rd/Box2D/Rope/b2Rope.h \
    ../3rd/Box2D/Box2D.h \
    ../src/editor/editorapp.h \
    ../src/editor/editorwindow.h \
    ../src/editor/sceneeditorapp.h \
    ../src/qt/gamewindow.h \
    ../src/qt/scenewidget.h \
    ../src/trash/alsound.h

