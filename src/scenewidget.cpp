#include "scenewidget.h"

#include "misc/cvec2.h"
#include "test/appevent.h"

using namespace drash;

drash::EventKey ConvertKey(int _key)
{
    switch (_key)
    {
    case Qt::Key_W:
        return drash::EventKeyW;
    case Qt::Key_A:
        return drash::EventKeyA;
    case Qt::Key_S:
        return drash::EventKeyS;
    case Qt::Key_D:
        return drash::EventKeyD;
    case Qt::Key_Space:
        return drash::EventKeySpace;
    default:
        return drash::EventKeyUnknown;
    }
}

SceneWidget::SceneWidget(QWidget *parent) :
    QGLWidget(parent)
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
    mApp->GetDebugDrawSystem().ScreenSpaceToWorldSpace(res, _depth);

    return res;
}

void SceneWidget::resizeGL( int _w, int _h )
{
    QGLWidget::resizeGL( _w, _h );

    mWidth = _w ? _w : 1;
    mHeight = _h ? _h : 1;

    glViewport( 0, 0, mWidth, mHeight );

    if (mApp != nullptr)
    {
        mApp->GetDebugDrawSystem().SetAspectRatio(mWidth / mHeight);
    }
}

void SceneWidget::paintGL()
{
    QGLWidget::paintGL();

    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if (mApp != nullptr)
    {
        mApp->Render();

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
        glVertex3f(mCursorPos.x, mCursorPos.y - 0.02 * mApp->GetDebugDrawSystem().GetAspectRatio(), -1);
        glVertex3f(mCursorPos.x, mCursorPos.y + 0.02 * mApp->GetDebugDrawSystem().GetAspectRatio(), -1);
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
        if (mApp->GetDebugDrawSystem().GetActiveCam() == nullptr)
		{
			return;
		}
        CSceneObjectGeometry g;

        CExplosionParams p;
        p.mLifeTime = 1;
        p.mStregth = -5;
        p.mRadius = 200;
        auto cam = mApp->GetDebugDrawSystem().GetActiveCam();
        p.mPos = WidgetSpaceToWorldSpace(CVec2(_event->x(),
                                               _event->y()),
                                               -cam->GetZ().Get());
        mApp->GetScene().CreateObject<CExplosion>(g, p);
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

void SceneWidget::keyPressEvent( QKeyEvent *_event )
{
    QGLWidget::keyPressEvent(_event);

    if ( mApp == nullptr ||
         mApp->GetDebugDrawSystem().GetActiveCam() == nullptr )
    {
        return;
    }

    mApp->PushEvent(CAppEvent(EventKeyboard, ConvertKey(_event->key())));
}

void SceneWidget::wheelEvent( QWheelEvent *_event )
{
    QGLWidget::wheelEvent(_event);

    if ( mApp == nullptr ||
         mApp->GetDebugDrawSystem().GetActiveCam() == nullptr )
    {
        return;
    }

    float pos = mApp->GetDebugDrawSystem().GetActiveCam()->GetZ().GetTarget();
    pos += _event->delta() / 10.0f;
    mApp->GetDebugDrawSystem().GetActiveCam()->SetZTarget( pos, 0.3, AnimationBehaviorSingle );
}

void SceneWidget::RemoveObjects()
{
    mApp->GetScene().DestroyObjects();
}
