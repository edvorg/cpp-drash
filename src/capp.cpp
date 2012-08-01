#include "capp.h"

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <Box2D/Box2D.h>
#include <math.h>
#include "clogger.h"
#include "cdebugrenderer.h"
#include "cscene.h"
#include "csolidbody.h"
#include "ccirclebody.h"
#include <iostream>
using namespace std;
namespace drash
{

void CAppParams::SetCommandLine( int _argc, char *_argv[] )
{
    for(int i = 0 ; i < _argc ; i++){
        string buff(_argv[i]);
        mArgv.push_back( buff );
    }
}

CAppParams::CAppParams()
{
}

CApp::CApp():
    mInitialized(false),
    mScene(),
    mTimer(),
    mDebugRenderer(),
    mCamera(NULL)
{

}

bool CApp::Init( const CAppParams &_params )
{
    if ( mInitialized == true )
    {
        LOG_WARN("CApp::Init(): app already initialized");
        return true;
    }

    /////////////////////////
    // sdl/opengl init

    if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
    {
        LOG_ERR( "CApp::Init(): SDL_Init failed" );
        return false;
    }

    SDL_WM_SetCaption( "Drash", NULL );

    if ( SDL_SetVideoMode( mWidth, mHeight, 32, SDL_HWSURFACE | SDL_OPENGL ) == NULL )
    {
        LOG_ERR( "CApp::Init(): SDL_SetVideoMode failed" );
        SDL_Quit();
        return false;
    }

    glViewport( 0, 0, mWidth, mHeight );

    /////////////////////////
    // scene init

    CSceneParams params;
    params.mGravity.Set( 0.0f, -9.8f );

    if ( mScene.Init(params) == false )
    {
        SDL_Quit();
        return false;
    }

    /////////////////////////
    // camera init

    CCameraParams p;
    mCamera = mScene.CreateObject< CCamera >(p);

    if ( mCamera == NULL )
    {
        mScene.Release();
        SDL_Quit();
        return false;
    }

    /////////////////////////
    // debug renderer init

    mDebugRenderer.SetFlags(0xffffffff);
    mDebugRenderer.ClearFlags(b2Draw::e_aabbBit);
    mDebugRenderer.SetCamera(mCamera);

    if ( mDebugRenderer.Init() == false )
    {
        LOG_WARN( "CScene::Init(): debug renderer init failed" );
        mScene.SetDebugRenderer(NULL);
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
    if ( mInitialized == false )
    {
        LOG_WARN( "CApp::Release(): app is not initialized" );
        return;
    }

    if ( mCamera != NULL )
    {
        mScene.DestroyObject(mCamera);
        mCamera = NULL;
    }

    mScene.Release();
    SDL_Quit();

    mInitialized = false;
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
    mScene.CreateObject< CSolidBody>(params)->SetTargetAngle(M_PI * 10);

    const unsigned int delta = 100;
    const unsigned int speed = 3;
    const float zoomdelta = 0.35f;
    const unsigned int zoomspeed = 5;

    mCamera->SetPosTargetSpeed( CVec2(speed) );
    mCamera->SetZoomTargetSpeed(zoomspeed);

    bool movexr = false;
    bool movexl = false;
    bool moveyu = false;
    bool moveyd = false;

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
                    mCamera->SetZoomTarget( mCamera->GetZoom() + zoomdelta * mCamera->GetZoom() );
                }
                else if ( event.button.button == SDL_BUTTON_WHEELUP )
                {
                    mCamera->SetZoomTarget( mCamera->GetZoom() - zoomdelta * mCamera->GetZoom() );
                }
                else
                {
                    go = false;
                    break;
                }
            }
            else if ( event.type == SDL_KEYDOWN )
            {
                if ( event.key.keysym.sym == SDLK_RIGHT )
                {
                    movexr = true;
                    movexl = false;
                }
                else if ( event.key.keysym.sym == SDLK_LEFT )
                {
                    movexr = false;
                    movexl = true;
                }
                else if ( event.key.keysym.sym == SDLK_UP )
                {
                    moveyu = true;
                    moveyd = false;
                }
                else if ( event.key.keysym.sym == SDLK_DOWN )
                {
                    moveyu = false;
                    moveyd = true;
                }
            }
            else if ( event.type == SDL_KEYUP )
            {
                if ( event.key.keysym.sym == SDLK_RIGHT )
                {
                    movexr = false;
                }
                else if ( event.key.keysym.sym == SDLK_LEFT )
                {
                    movexl = false;
                }
                else if ( event.key.keysym.sym == SDLK_UP )
                {
                    moveyu = false;
                }
                else if ( event.key.keysym.sym == SDLK_DOWN )
                {
                    moveyd = false;
                }
            }
        }

        CVec2 newt = mCamera->GetPos();

        if ( movexr )
        {
            newt.x += delta / mCamera->GetZoom();
            mCamera->SetPosTarget( newt );
        }
        else if ( movexl )
        {
            newt.x -= delta / mCamera->GetZoom();
            mCamera->SetPosTarget( newt );
        }

        if ( moveyu )
        {
            newt.y += delta / mCamera->GetZoom();
            mCamera->SetPosTarget( newt );
        }
        else if ( moveyd )
        {
            newt.y -= delta / mCamera->GetZoom();
            mCamera->SetPosTarget( newt );
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
    assert( mInitialized == true );

    mTimer.Tick();
    mScene.Step( mTimer.GetDeltaTime() );

    CSolidBodyParams par;
    par.mPos.Rand(-100,100);
    mScene.CreateObject< CSolidBody >(par);
}

void CApp::Render()
{
    assert( mInitialized == true );

    glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    mScene.Draw();

    SDL_GL_SwapBuffers();
}

}// namespace drash
