#include "scenewidget.h"

#include "misc/cvec2.h"

using namespace drash;

SceneWidget::SceneWidget(QWidget *parent) :
    QGLWidget(parent),
    mTestApp(NULL)
{
    setMouseTracking(true);
}

SceneWidget::~SceneWidget()
{
}

CVec2 SceneWidget::WidgetSpaceToScreenSpace(const CVec2 &_from) const
{
    CVec2 res = _from;

    res.x /= mWidth;
    res.y /= mHeight;

    res.x -= 0.5;
    res.y -= 0.5;
    res.y *= -1;

    return res;
}

CVec2 SceneWidget::WidgetSpaceToWorldSpace(const CVec2 &_from, float _depth) const
{
    CVec2 res = WidgetSpaceToScreenSpace(_from);
    mTestApp->GetDebugDrawSystem().ScreenSpaceToWorldSpace(res, _depth);

    return res;
}

void SceneWidget::resizeGL( int _w, int _h )
{
    QGLWidget::resizeGL( _w, _h );

    mWidth = _w ? _w : 1;
    mHeight = _h ? _h : 1;

    glViewport( 0, 0, mWidth, mHeight );

    if (mTestApp != nullptr)
    {
        mTestApp->GetDebugDrawSystem().SetAspectRatio(mWidth / mHeight);
    }
}

void SceneWidget::paintGL()
{
    QGLWidget::paintGL();

    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if (mTestApp != nullptr)
    {
        mTestApp->Render();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-0.5, 0.5, -0.5, 0.5, 1, -1);

        glDisable(GL_DEPTH_TEST);

        glPointSize(4);
        glBegin(GL_POINTS);
        glColor3f(1, 0, 0);
        glVertex3f(mCursorPos.x, mCursorPos.y, -1);
        glEnd();
        glBegin(GL_LINES);
        glColor3f(0, 1, 0);
        glVertex3f(mCursorPos.x - 0.02, mCursorPos.y, -1);
        glVertex3f(mCursorPos.x + 0.02, mCursorPos.y, -1);
        glVertex3f(mCursorPos.x, mCursorPos.y - 0.02 * mTestApp->GetDebugDrawSystem().GetAspectRatio(), -1);
        glVertex3f(mCursorPos.x, mCursorPos.y + 0.02 * mTestApp->GetDebugDrawSystem().GetAspectRatio(), -1);
        glEnd();

        glEnable(GL_DEPTH_TEST);
    }

    swapBuffers();
}

void SceneWidget::SetTestApp( drash::CApp *_app )
{
    mApp = _app;
}

void SceneWidget::mousePressEvent( QMouseEvent *_event )
{
    QGLWidget::mousePressEvent(_event);

    switch ( _event->button() )
    {
    case Qt::LeftButton:
    {
		if (mTestApp->GetDebugDrawSystem().GetActiveCam() == nullptr)
		{
			return;
		}
        CSceneObjectGeometry g;

        CExplosionParams p;
        p.mLifeTime = 1;
        p.mStregth = -5;
        p.mRadius = 200;
        auto cam = mTestApp->GetDebugDrawSystem().GetActiveCam();
        p.mPos = WidgetSpaceToWorldSpace(CVec2(_event->x(),
                                               _event->y()),
                                               -cam->GetZ().Get());
        mTestApp->GetScene().CreateObject<CExplosion>(g, p);
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
    mCursorPos = WidgetSpaceToScreenSpace(CVec2(_event->x(),
                                                _event->y()));
}

void SceneWidget::keyReleaseEvent( QKeyEvent *_event )
{
    QGLWidget::keyPressEvent(_event);

    if ( mTestApp == nullptr ||
         mTestApp->GetDebugDrawSystem().GetActiveCam() == nullptr )
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

    if ( mTestApp == nullptr ||
         mTestApp->GetDebugDrawSystem().GetActiveCam() == nullptr )
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

    if ( mTestApp == nullptr ||
         mTestApp->GetDebugDrawSystem().GetActiveCam() == nullptr )
    {
        return;
    }

    float pos = mTestApp->GetDebugDrawSystem().GetActiveCam()->GetZ().GetTarget();
    pos += _event->delta() / 10.0f;
    mTestApp->GetDebugDrawSystem().GetActiveCam()->SetZTarget( pos, 0.3, AnimationBehaviorSingle );
}

void SceneWidget::RemoveObjects()
{
    mTestApp->GetScene().DestroyObjects();
}
