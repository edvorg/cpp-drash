#include "capp.h"

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <Box2D/Box2D.h>
#include <math.h>
#include "clogger.h"
#include "cdebugrenderer.h"
#include "cscene.h"
#include "cobjectsolidbody.h"
#include "cobjectcirclebody.h"
//#include <time.h>

namespace drash{

CApp::CApp():
    mInitialized(false)
{

}

bool CApp::init( const CAppParams &_params )
{
    (void)_params; // while Unused // wtf???

    if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
    {
        LOG_ERR( "SDL_Init failed" );
        return false;
    }

    SDL_WM_SetCaption( "Drash", NULL );

    if ( SDL_SetVideoMode( mWidth, mHeight, 32, SDL_HWSURFACE | SDL_OPENGL ) == NULL )
    {
        LOG_ERR( "SDL_SetVideoMode failed" );
        return false;
    }

    glViewport( 0, 0, mWidth, mHeight );

    CSceneParams params;
    params.mGravity.Set( 0.0f, -9.8f );

    if ( mScene.Init(params) == false )
    {
        return false;
    }

    mPrevTime = 0;

    mInitialized = true;
    return true;
}

void CApp::Release()
{
    assert( mInitialized == true );

    mScene.Release();
    SDL_Quit();
}

void CApp::Run()
{
    assert( mInitialized == true );

    timeval time;
    gettimeofday( &time, NULL );
    mPrevTime = time.tv_sec * 1000000 + time.tv_usec;

    for ( ;; )
    {
        bool go = true;
        SDL_Event event;

        while ( SDL_PollEvent(&event) )
        {
            // Catch all events
            if ( event.type == SDL_MOUSEBUTTONDOWN ||
                 event.type == SDL_QUIT )
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
        Update();

        Render();
    }
}

void CApp::Update()
{
    timeval time;
    gettimeofday( &time, NULL );
    unsigned int currtime = time.tv_sec * 1000000 + time.tv_usec;

    mScene.Step( currtime - mPrevTime );

    mPrevTime = currtime;
}

void CApp::Render()
{
    glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    mScene.Draw();

    SDL_GL_SwapBuffers();
}

}// namespace drash
