#include <stdio.h>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <Box2D/Box2D.h>
#include <math.h>
#include "cdebugrenderer.h"
#include "clogger.h"

using namespace drash;

bool Init();
void Run();
void Render();
void Release();

int main(int _argc, char *_argv[])
{
    if (!Init())
    {
        return 0;
    }

    Run();
    Release();

    return 0;
}

bool Init()
{
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
    {
        LOG_ERR("SDL_Init failed");
        return false;
    }

    SDL_WM_SetCaption("Drash", NULL);

    if ( SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_OPENGL) == NULL )
    {
        LOG_ERR("SDL_SetVideoMode failed");
        return false;
    }

    glViewport(0,0,800,600);

    CDebugRenderer render;

    if (render.Init() == false)
    {
        LOG_ERR("debug render init failed");
        return false;
    }

    // TODO: init scene here

    return true;
}

void Run()
{
    for(;;)
    {
        bool go = true;
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            // Catch all events
            if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_QUIT)
            {
                go = false;
                break;
            }
        }

        // If event for exit
        if (!go)
        {
            break;
        }

        Render();
    }
}

void Render()
{
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO: draw scene objects

    SDL_GL_SwapBuffers();
}

void Release()
{
    // TODO: release scene here

    SDL_Quit();
}
