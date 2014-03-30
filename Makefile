CPP_FLAGS=-std=c++1y -stdlib=libc++ -O3 -Wall
INCLUDES=-I3rd
CC=clang++

LIBS=-lrt -lGL -lSDL -lGLEW -lSDL_image -lopenal -lvorbis -lvorbisfile -lc++abi -lpthread

EXECUTABLE=drash

SOURCES += \
    src/ui/uiwidget.cpp \
    src/ui/uisystem.cpp \
    src/ui/uislider.cpp \
    src/ui/uicontrol.cpp \
    src/ui/uibutton.cpp \
    src/test/test5.cpp \
    src/test/test4.cpp \
    src/test/test3.cpp \
    src/test/test2.cpp \
    src/test/test1.cpp \
    src/test/test.cpp \
    src/scene/geometrymanager.cpp \
    src/players/playerssystem.cpp \
    src/scene/sceneobject.cpp \
    src/scene/scene.cpp \
	src/players/player.cpp \
    src/scene/figure.cpp \
    src/misc/ray.cpp \
    src/misc/plane.cpp \
    src/misc/matrix4.cpp \
    src/greng/renderer.cpp \
    src/greng/meshmanager.cpp \
    src/diag/timer.cpp \
    src/diag/logger.cpp \
    src/greng/camera.cpp \
    src/app/appeventsystem.cpp \
    src/app/appeventprocessor.cpp \
    src/app/appeventcombination.cpp \
    src/app/appevent.cpp \
    src/app/app.cpp \
    src/sdl/main.cpp \
    src/greng/loadmeshobj.cpp \
    src/greng/texturemanager.cpp \
    src/greng/framebuffermanager.cpp \
    src/greng/vertexshadermanager.cpp \
    src/greng/fragmentshadermanager.cpp \
    src/greng/shaderprogrammanager.cpp \
    src/test/test6.cpp \
    src/misc/vec2.cpp \
    src/scene/joint.cpp \
    src/test/test7.cpp \
    src/greng/cameramanager.cpp \
    src/debugrenderer/debugrenderer.cpp \
    src/misc/moveablepoint.cpp \
    src/levelmanager/leveldesc.cpp \
    src/levelmanager/levelmanager.cpp \
    src/test/test8.cpp \
    src/misc/rotationablepoint.cpp \
    src/test/test9.cpp \
    src/greng/grengsystemsset.cpp \
    src/test/test10.cpp \
    src/test/test11.cpp \
    src/test/test12.cpp \
    src/greng/spotlight.cpp \
    3rd/Box2D/Collision/b2BroadPhase.cpp \
    3rd/Box2D/Collision/b2CollideCircle.cpp \
    3rd/Box2D/Collision/b2CollideEdge.cpp \
    3rd/Box2D/Collision/b2CollidePolygon.cpp \
    3rd/Box2D/Collision/b2Collision.cpp \
    3rd/Box2D/Collision/b2Distance.cpp \
    3rd/Box2D/Collision/b2DynamicTree.cpp \
    3rd/Box2D/Collision/b2TimeOfImpact.cpp \
    3rd/Box2D/Collision/Shapes/b2ChainShape.cpp \
    3rd/Box2D/Collision/Shapes/b2CircleShape.cpp \
    3rd/Box2D/Collision/Shapes/b2EdgeShape.cpp \
    3rd/Box2D/Collision/Shapes/b2PolygonShape.cpp \
    3rd/Box2D/Common/b2BlockAllocator.cpp \
    3rd/Box2D/Common/b2Draw.cpp \
    3rd/Box2D/Common/b2Math.cpp \
    3rd/Box2D/Common/b2Settings.cpp \
    3rd/Box2D/Common/b2StackAllocator.cpp \
    3rd/Box2D/Common/b2Timer.cpp \
    3rd/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp \
    3rd/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
    3rd/Box2D/Dynamics/Contacts/b2CircleContact.cpp \
    3rd/Box2D/Dynamics/Contacts/b2Contact.cpp \
    3rd/Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
    3rd/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
    3rd/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
    3rd/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
    3rd/Box2D/Dynamics/Contacts/b2PolygonContact.cpp \
    3rd/Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
    3rd/Box2D/Dynamics/Joints/b2FrictionJoint.cpp \
    3rd/Box2D/Dynamics/Joints/b2GearJoint.cpp \
    3rd/Box2D/Dynamics/Joints/b2Joint.cpp \
    3rd/Box2D/Dynamics/Joints/b2MotorJoint.cpp \
    3rd/Box2D/Dynamics/Joints/b2MouseJoint.cpp \
    3rd/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
    3rd/Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
    3rd/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
    3rd/Box2D/Dynamics/Joints/b2RopeJoint.cpp \
    3rd/Box2D/Dynamics/Joints/b2WeldJoint.cpp \
    3rd/Box2D/Dynamics/Joints/b2WheelJoint.cpp \
    3rd/Box2D/Dynamics/b2Body.cpp \
    3rd/Box2D/Dynamics/b2ContactManager.cpp \
    3rd/Box2D/Dynamics/b2Fixture.cpp \
    3rd/Box2D/Dynamics/b2Island.cpp \
    3rd/Box2D/Dynamics/b2World.cpp \
    3rd/Box2D/Dynamics/b2WorldCallbacks.cpp \
    3rd/Box2D/Rope/b2Rope.cpp \
    src/trash/alsound.cpp \
    src/misc/dimensions.cpp \
    src/misc/screenshaker.cpp

OBJECTS=${SOURCES:.cpp=.o}

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	${CC} ${CPP_FLAGS} ${LIBS} ${OBJECTS} -o ${EXECUTABLE}

.cpp.o:
	${CC} ${CPP_FLAGS} ${INCLUDES} $< -c -o $@

check-syntax:
	${CC} ${CPP_FLAGS} ${INCLUDES} -fsyntax-only -fno-color-diagnostics ${CHK_SOURCES}

print-cflags:
	echo ${CPP_FLAGS} ${INCLUDES}
