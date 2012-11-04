#include "ctestapp.h"

#include <GL/gl.h>

namespace drash
{

CTestApp::CTestApp():
    mCamera(NULL)
{
}

CTestApp::~CTestApp()
{
}

bool CTestApp::Init()
{
    CSceneParams params;
    params.mGravity.Set( 0, -9.8 );

    if ( mScene.Init(params) == false )
    {
        return false;
    }

    mExplosionSystem.SetScene(&mScene);

    CCameraParams p;
    mCamera = mScene.CreateObject< CCamera >(p);

    if ( mCamera == NULL )
    {
        return false;
    }

    mTimer.Reset(true);

    return true;
}

void CTestApp::Release()
{
    if ( mCamera != NULL )
    {
        mScene.DestroyObject(mCamera);
        mCamera = NULL;
    }

    mScene.Release();
}

void CTestApp::Update()
{
    mTimer.Tick();
    mScene.Step( mTimer.GetDeltaTime() );
}

void CTestApp::Render()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f );

    float y = ( static_cast<float>( mCamera->GetZoom() - 1 ) / ( mCamera->m_ZoomMax - 1 ) ) * 1.7 - 0.85f;

    glBegin(GL_LINES);
    glColor3f( 0, 1, 0 );
    glVertex2f( -0.85f, -0.9f );
    glColor3f( 0, 1, 0 );
    glVertex2f( -0.85f, 0.9f );
    glColor3f( 0, 1, 0 );
    glVertex2f( -0.95f, -0.9f );
    glColor3f( 0, 1, 0 );
    glVertex2f( -0.75f, -0.9f );
    glColor3f( 0, 1, 0 );
    glVertex2f( -0.95f, 0.9f );
    glColor3f( 0, 1, 0 );
    glVertex2f( -0.75f, 0.9f );

    glColor3f( 0, 1, 0 );
    glVertex2f( -0.9f, y );
    glColor3f( 0, 1, 0 );
    glVertex2f( -0.8f, y );
    glEnd();
}

CScene &CTestApp::GetScene()
{
    return mScene;
}

const CScene &CTestApp::GetScene() const
{
    return mScene;
}

CCamera *CTestApp::GetCamera()
{
    return mCamera;
}

} // namespace drash
