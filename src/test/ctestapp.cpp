#include "ctestapp.h"

#include <GL/gl.h>

namespace drash
{

CTestApp::CTestApp():
    mCamera(NULL),
    mScene(NULL)
{
}

CTestApp::~CTestApp()
{
}

bool CTestApp::Init( CScene *_scene, CCamera *_camera )
{
    if ( _camera == NULL )
    {
        return false;
    }
    if ( _scene == NULL )
    {
        return false;
    }

    mCamera = _camera;
    mScene = _scene;

    return true;
}

void CTestApp::Release()
{
    mScene = NULL;
    mCamera = NULL;
}

void CTestApp::Update()
{
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

CScene *CTestApp::GetScene()
{
    return mScene;
}

CCamera *CTestApp::GetCamera()
{
    return mCamera;
}

} // namespace drash
