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
#include <iostream>
namespace drash
{

CAppParams::CAppParams()
{
}

CApp::CApp():
    mInitialized(false)
{

}

bool CApp::init( const CAppParams &_params )
{

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

    mDebugRenderer.SetFlags(0xffffffff);
    mDebugRenderer.ClearFlags(b2Draw::e_aabbBit);

    if ( mDebugRenderer.Init() == false )
    {
        LOG_WARN( "CScene::Init(): debug renderer init failed" );
    }
    else
    {
        mScene.SetDebugRenderer(&mDebugRenderer);
    }

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

    CVec2 ver[4];
    ver[0].Set(-100,-10);
    ver[1].Set(100,-10);
    ver[2].Set(100,10);
    ver[3].Set(-100,10);
    CSolidBodyParams params;
    params.mVertices = ver;
    params.mVerticesCount =4;
    params.mPos.Set(0,-50);
    params.mDynamic = false;
    mScene.CreateObject< CObjectSolidBody>(params);
    mTimer.Reset(true);

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
                if ( event.button.button == SDL_BUTTON_LEFT )
                {
                    mTimer.SetPaused( !mTimer.IsPaused() );
                }
                else if ( event.button.button == SDL_BUTTON_WHEELDOWN )
                {
                    mDebugRenderer.SetZoom( mDebugRenderer.GetZoom() + 0.5f); //* mTimer.GetDeltaTime() );
                }
                else if ( event.button.button == SDL_BUTTON_WHEELUP )
                {
                    mDebugRenderer.SetZoom( mDebugRenderer.GetZoom() - 0.5f); //* mTimer.GetDeltaTime() );
                }
                else
                {
                    go = false;
                    break;
                }
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
    mTimer.Tick();
    mScene.Step( mTimer.GetDeltaTime() );

    CSolidBodyParams par;
    par.mAngle = 4;
    par.mMass = 50;
    par.mRestitution = (rand()%2) ? 0:10;
    par.mPos.Rand(-100,100,0.1);
    mScene.CreateObject< CObjectSolidBody >(par);

}

void CApp::Render()
{
    glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    mScene.Draw();

    SDL_GL_SwapBuffers();
}

}// namespace drash
