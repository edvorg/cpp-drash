#include <stdio.h>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <Box2D/Box2D.h>
#include <math.h>
#include "cdebugrenderer.h"
#include "clogger.h"
#include "cscene.h"
#include "cobjectsolidbody.h"

using namespace drash;

bool Init(CScene& _scene);
void Run(CScene& _scene);
void Render(CScene& _scene);
void Release(CScene &_scene);

int main(int _argc, char *_argv[])
{
    CScene scene;

    if ( Init(scene) == false )
    {
        return 0;
    }

    Run(scene);
    Release(scene);

    return 0;
}

// TODO: wrap the sdl initialization code in CApp class
// realize CApp::Init(CAppParams& _params, CScene& _scene)
// realize CApp::Run()
// realize CApp::Release()
// realize dummy CAppParams class

bool Init(CScene& _scene)
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

    CSceneParams params;

    if ( _scene.Init(params) == false )
    {
        return false;
    }

    CObjectSolidBody* obj1 = _scene.CreateObject<CObjectSolidBody>();
    CObjectSolidBody* obj2 = _scene.CreateObject<CObjectSolidBody>();

    _scene.DestroyObject(obj1);
    //_scene.DestroyObject(obj2);

    return true;
}

void Run(CScene &_scene)
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

        // TODO: compute delta time and call _scene.Step()
        
        Render(_scene);
    }
}

void Render(CScene &_scene)
{
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _scene.Draw();

    SDL_GL_SwapBuffers();
}

void Release(CScene &_scene)
{
    _scene.Release();

    SDL_Quit();
}
