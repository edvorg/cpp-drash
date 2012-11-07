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
    mTestApp(NULL),
    mWidth(1),
    mHeight(1)
{
    resize( parent->size() );
}

void SceneWidget::resizeGL( int _w, int _h )
{
    QGLWidget::resizeGL( _w, _h );

    glViewport( 0, 0, _w, _h );

    mWidth = _w;
    mHeight = _h;
}

void SceneWidget::paintGL()
{
    QGLWidget::paintGL();

    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 60.0, mWidth / mHeight, 1.0f, 1000.0f );

    if ( mTestApp != NULL )
    {
        mTestApp->GetScene().Draw( *mTestApp->GetCamera() );
        mTestApp->Render();
    }

    swapBuffers();
}

void SceneWidget::SetTestApp( drash::CTestApp *_app )
{
    mTestApp = _app;
}

void SceneWidget::mousePressEvent( QMouseEvent *_event )
{
    QGLWidget::mousePressEvent(_event);

    switch ( _event->button() )
    {
    case Qt::LeftButton:
        if ( mTestApp != NULL && mTestApp->GetCamera() != NULL )
        {
            CVec2 pos;
            pos = PointSDLToWorldPoint( _event->pos().x(),
                                        _event->pos().y(),
                                        mTestApp->GetCamera()->GetZoom(),
                                        mTestApp->GetCamera()->GetZoom(),
                                        mHeight,
                                        mWidth);

            CBoomParams p;
            p.mLifeTime = 2;
            p.mPos = pos;
            p.mStregth = -3;
            mTestApp->GetScene().CreateObject<CExplosion>(p);
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

    if ( mTestApp == NULL ||
         mTestApp->GetCamera() == NULL )
    {
        return;
    }

    switch ( _event->key() )
    {
    case Qt::Key_A:
        mTestApp->GetPlayersSystem().OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionEndMoveLeft, CVec2() ), 0 );
        break;

    case Qt::Key_D:
        mTestApp->GetPlayersSystem().OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionEndMoveRight, CVec2() ), 0 );
        break;

    default:
        break;
    }
}

void SceneWidget::keyPressEvent( QKeyEvent *_event )
{
    QGLWidget::keyPressEvent(_event);

    if ( mTestApp == NULL ||
         mTestApp->GetCamera() == NULL )
    {
        return;
    }

    switch ( _event->key() )
    {
    case Qt::Key_Space:
        mTestApp->GetPlayersSystem().OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionJump, CVec2() ), 0 );
        break;

    case Qt::Key_A:
        mTestApp->GetPlayersSystem().OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionMoveLeft, CVec2() ), 0 );
        break;

    case Qt::Key_D:
        mTestApp->GetPlayersSystem().OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionMoveRight, CVec2() ), 0 );
        break;

    case Qt::Key_W:
        mTestApp->GetPlayersSystem().OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionMoveDeep, CVec2() ), 0 );
        break;

    case Qt::Key_S:
        mTestApp->GetPlayersSystem().OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionMoveOut, CVec2() ), 0 );
        break;

    default:
        break;
    }
}

void SceneWidget::wheelEvent( QWheelEvent *_event )
{
    QGLWidget::wheelEvent(_event);

    if ( mTestApp == NULL ||
         mTestApp->GetCamera() == NULL )
    {
        return;
    }

    float pos = mTestApp->GetCamera()->GetZoomTarget();
    pos += _event->delta() / 10.0f;
    mTestApp->GetCamera()->SetZoomTarget( pos, 0.3 );
}
