#include <stdio.h>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <Box2D/Box2D.h>
#include <math.h>
#include "cdebugrenderer.h"

using namespace drash;

int main(int argc, char *argv[]) {


    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetCaption("hello, sdl", NULL);
    SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_OPENGL);
    glViewport(0,0,800,600);

    CDebugRenderer render;
    assert(render.Init() == true);

    for(;;) {
        bool go = true;
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            // Catch all events
            if (event.type == SDL_MOUSEBUTTONDOWN){
                go = false;
                break;
            }
            if (event.type == SDL_QUIT){
                go = false;
                break;
            }
        }
        // If event for exit
        if (!go)
            break;
        glClear(GL_COLOR_BUFFER_BIT);


        b2Vec2 verForPolygon[4];

        verForPolygon[0].Set(0,0);
        verForPolygon[1].Set(70,50);
        verForPolygon[2].Set(100,0);
//        verForPolygon[2].Set(,);
//        int32 countVertex = 3;
        b2Color color;
        color.Set(1,0.1,1);

//        render.DrawPolygon(verForPolygon,3,color);

        verForPolygon[0].Set(150,80);
        verForPolygon[1].Set(150,130);
        verForPolygon[2].Set(250,130);
        verForPolygon[3].Set(250,80);

        color.Set(1,0,0);
//        render.DrawPolygo.n(verForPolygon,4,color);

        color.Set(0,1,0);
//        render.DrawCircle(b2Vec2(-150,70),120,color);

        color.Set(1,1,1);
//        render.DrawSolidCircle(b2Vec2(-70,-200),100,b2Vec2(0,0),color);

//        render.DrawSegment(b2Vec2(100,50),b2Vec2(15,50),color);
        render.DrawTransform(b2Transform(verForPolygon[0],b2Rot(120 *M_PI / 180)));
        SDL_GL_SwapBuffers();

    }

    SDL_Quit();

    return 0;
}

// Sorry, it is conflict merging

//=======
//#include "cscene.h"
//#include "clogger.h"

//using namespace drash;

////int main()
////{
////    CScene scene;

////    CSceneParams sceneparams;
////    sceneparams.mGravity.set(0, -10);
////    scene.Init(sceneparams);

////    CSceneObjectParams params;

////    CSceneObject* obj1 = scene.CreateObject(params);
////    CSceneObject* obj2 = scene.CreateObject(params);
////    CSceneObject* obj3 = scene.CreateObject(params);

////    scene.DestroyObject(obj1);

////>>>>>>> origin/master
////    return 0;
////}
