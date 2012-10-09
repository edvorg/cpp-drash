#include "scenewidget.h"

#include "misc/cvec2.h"
#include <GL/glu.h>

using namespace drash;

// TODO: this function is incorrect
CVec2 PointSDLToWorldPoint( unsigned int _x,
                            unsigned int _y,
                            float _zoom,
                            const CVec2 & _posCamera,
                            unsigned  int _height,
                            unsigned int _width )
{
    CVec2 res;
    res.y = - (float)_y / _zoom + ( (float)_height / _zoom ) / 2.0f;
    res.x =  ( -(float)_width / _zoom ) / 2.0f + (float)_x / _zoom;
    res += _posCamera;
    return res;
}

SceneWidget::SceneWidget(QWidget *parent) :
    QGLWidget(parent),
    mCamera(NULL),
    mScene(NULL),
    mTestApp(NULL),
    mWidth(1),
    mHeight(1)
{
    resize( parent->size() );
}

void SceneWidget::resizeGL(int w, int h)
{
    QGLWidget::resizeGL( w, h );

    glViewport( 0, 0, w, h );

    mWidth = w;
    mHeight = h;
}

void SceneWidget::paintGL()
{
    QGLWidget::paintGL();

    if ( mScene == NULL ||
         mCamera == NULL )
    {
        return;
    }

    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 60.0, mWidth / mHeight, 1.0f, 1000.0f );

    mScene->Draw(*mCamera);

    if ( mTestApp != NULL )
    {
        mTestApp->Render();
    }

    swapBuffers();
}

void SceneWidget::SetScene( drash::CScene *_scene )
{
    mScene = _scene;
}

void SceneWidget::SetCamera( drash::CCamera *_camera )
{
    mCamera = _camera;
}

void SceneWidget::SetTestApp(drash::CTestApp *_app)
{
    mTestApp = _app;
}

void SceneWidget::mousePressEvent( QMouseEvent *_event )
{
    QGLWidget::mousePressEvent(_event);

    switch ( _event->button() )
    {
    case Qt::LeftButton:
        {
            CVec2 pos;
            pos = PointSDLToWorldPoint( _event->pos().x(),
                                        _event->pos().y(),
                                        mCamera->GetZoom(),
                                        mCamera->GetZoom(),
                                        mHeight,
                                        mWidth);

            mScene->OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionFire, pos ), 0 );
            break;
        }

    case Qt::RightButton:
        QCoreApplication::quit();
        break;

    default:
        break;
    }
}

void SceneWidget::keyReleaseEvent( QKeyEvent *_event )
{
    QGLWidget::keyPressEvent(_event);

    switch ( _event->key() )
    {
    case Qt::Key_A:
        mScene->OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionEndMoveLeft, CVec2() ), 0 );
        break;

    case Qt::Key_D:
        mScene->OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionEndMoveRight, CVec2() ), 0 );
        break;

    default:
        break;
    }
}

void SceneWidget::keyPressEvent( QKeyEvent *_event )
{
    QGLWidget::keyPressEvent(_event);

    switch ( _event->key() )
    {
    case Qt::Key_Space:
        mScene->OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionJump, CVec2() ), 0 );
        break;

    case Qt::Key_A:
        mScene->OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionMoveLeft, CVec2() ), 0 );
        break;

    case Qt::Key_D:
        mScene->OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionMoveRight, CVec2() ), 0 );
        break;

    case Qt::Key_W:
        mScene->OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionMoveDeep, CVec2() ), 0 );
        break;

    case Qt::Key_S:
        mScene->OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionMoveOut, CVec2() ), 0 );
        break;

    default:
        break;
    }
}

void SceneWidget::wheelEvent( QWheelEvent *_event )
{
    QGLWidget::wheelEvent(_event);

    float pos = mCamera->GetZoomTarget();
    pos += _event->delta() / 10.0f;
    mCamera->SetZoomTarget( pos, 0.3 );
}
