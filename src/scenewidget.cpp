#include "scenewidget.h"

#include "misc/cvec2.h"
#include <GL/glu.h>

using namespace drash;

SceneWidget::SceneWidget(QWidget *parent) :
    QGLWidget(parent),
    mTestApp(NULL),
    mWidth(1),
    mHeight(1)
{
    resize( parent->size() );
    setMouseTracking(true);
}

CVec2 SceneWidget::ScreenSpaceToWorldSpace(const CVec2 &_from, float _depth)
{
    double aspect = mWidth / mHeight;

    double fov = mFov * M_PI / 180.0;

    double c = _depth / cos(fov / 2.0); // hypotenuse

    double near_height = 2.0 * sqrt( c*c - _depth*_depth );
    double near_width = near_height * aspect;

    CVec2 res = _from;

    res.x /= mWidth;
    res.y /= mHeight;

    res.x -= 0.5;
    res.y -= 0.5;
    res.y *= -1;

    res.x *= near_width;
    res.y *= near_height;

    return res;
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

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( mFov, mWidth / mHeight, 1.0f, 1000.0f );

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0, 1, 0);
    glVertex3f(mCursorPos.x, mCursorPos.y, -1);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(0, 1, 0);
    glVertex3f(mCursorPos.x - 0.05, mCursorPos.y, -1);
    glVertex3f(mCursorPos.x + 0.05, mCursorPos.y, -1);
    glVertex3f(mCursorPos.x, mCursorPos.y - 0.05, -1);
    glVertex3f(mCursorPos.x, mCursorPos.y + 0.05, -1);
    glEnd();

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
    {
        CBoomParams p;
        p.mFigures.resize(1);
        p.mLifeTime = 1;
        p.mStregth = -5;
        auto cam = mTestApp->GetCamera();
        p.mPos = ScreenSpaceToWorldSpace(CVec2(_event->x(),
                                               _event->y()),
                                         cam->m_ZoomMax - cam->GetZoom());
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

void SceneWidget::mouseMoveEvent(QMouseEvent *_event)
{
    QGLWidget::mouseMoveEvent(_event);
    mCursorPos = ScreenSpaceToWorldSpace(CVec2(_event->x(),
                                               _event->y()),
                                         1);
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
