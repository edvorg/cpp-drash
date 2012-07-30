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

bool CApp::init(CAppParams &_params)
{
    (void)_params; // while Unused

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

    mCurrTime = 0;
    mPrevTime = 0;

    mInitialized = true;
    return true;
}

void CApp::Release()
{
    assert(mInitialized == true);

    mScene.Release();
    SDL_Quit();
}

void CApp::Run()
{
    assert(mInitialized == true);

    gettimeofday( &mTime, NULL );
    mCurrTime = mPrevTime = mTime.tv_sec * 1000000 + mTime.tv_usec;

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
    }
}

void CApp::Update()
{
    gettimeofday( &mTime, NULL );
    mCurrTime = mTime.tv_sec * 1000000 + mTime.tv_usec;
    mScene.Step( mCurrTime - mPrevTime );
    mPrevTime = mCurrTime;

//    CCircleBodyParams params;
//    params.mRadius = 10;
//    params.mDynamic = true;
//    params.mRestitution = 10;
//    mScene.CreateObject< CObjectCircleBody >(params);

    Render();
}

void CApp::Render()
{
    glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    mScene.Draw();

    SDL_GL_SwapBuffers();
}

}// namespace drash
