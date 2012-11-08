#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QtOpenGL/QtOpenGL>
#include "diag/ctimer.h"
#include "sceneobjects.h"
#include "test/ctestapp.h"
#include "cscene.h"

class SceneWidget : public QGLWidget
{
public:
    explicit SceneWidget( QWidget *parent = 0 );

    drash::CVec2 ScreenSpaceToWorldSpace(const drash::CVec2& _from, float _depth);

    virtual void resizeGL( int _w, int _h );
    virtual void paintGL();

    void SetTestApp( drash::CTestApp *_app );

    virtual void mousePressEvent( QMouseEvent * _event );
    virtual void mouseMoveEvent(QMouseEvent *_event);
    virtual void keyReleaseEvent( QKeyEvent *_event );
    virtual void keyPressEvent( QKeyEvent *_event );
    virtual void wheelEvent( QWheelEvent *_event );

private:
    constexpr static double mFov = 60.0;

    drash::CTestApp *mTestApp;
    float mWidth;
    float mHeight;
    drash::CVec2 mCursorPos;
};

#endif // SCENEWIDGET_H
