#include <stdio.h>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <Box2D/Box2D.h>
#include <math.h>
#include "cdebugrenderer.h"


int main(int argc, char *argv[]) {
    SDL_Event event;
    srand(0);
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetCaption("hello, sdl", NULL);
    SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_OPENGL);
    glViewport(0,0,800,600);
    static int inc = 0;
    CDebugRenderer render;
    if (!render.Init()) exit(1);
    //assert(render.Init() == false);
    while (true) {
        SDL_PollEvent(&event);
        if (event.type == SDL_MOUSEBUTTONDOWN){
            break;
        }
        if (event.type == SDL_QUIT)
                break;

        glClear(GL_COLOR_BUFFER_BIT);

//        glMatrixMode( GL_MODELVIEW );

//        glLoadIdentity();
//        glTranslatef(-200,-200,0);
//          glRotatef(inc, 0,0,1);
//        glMatrixMode( GL_PROJECTION );
//        glLoadIdentity();
//        glOrtho(-400.0f,400.0f,-300.0f,300.0f,1.0f,-1.0f);

//        glBegin(GL_LINE_LOOP);
//        float radius = 40;

//        for(int i = 1 ;i < 20 ; i++){
//            float angle = (float)i/M_PI * 180;
//            glVertex2f(cos(angle)*radius,sin(angle)*radius);
//        }
////        glVertex3f( 0.0f, 100.0f, 0.0f);
////        glVertex3f(-100.0f,-1.0f, 0.0f);
////        glVertex3f( 100.0f,-1.0f, 0.0f);
//        glEnd();

////        glBegin(GL_TRIANGLES);
////        glVertex3f( 100.0f, 100.0f, 0.0f);
////        glVertex3f(-100.0f,-1.0f, 0.0f);
////        glVertex3f( 150.0f,-1.0f, 0.0f);
////        glEnd();



        b2Vec2 ver[3];
        //for(int i = 0 ; i < 3 ; i++){
            //int x1 = 10 + i;//rand()%800;
            //int y1 = 20 + i;//rand()%600;
            //ver[i].Set(x1,y1);
        //}

        ver[0].Set(0,0);
        ver[1].Set(10,20);
        ver[2].Set(30,0);

        b2Color color;
        color.Set(1,0.1,1);
        int32 s = 3;
        render.DrawPolygon(ver,s,color);
        render.DrawCircle(ver[1],50,color);
        SDL_GL_SwapBuffers();

        inc++;

    }
    SDL_Quit();
    return 0;
}
