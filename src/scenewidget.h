#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QtOpenGL/QtOpenGL>
#include "diag/ctimer.h"
#include "sceneobjects.h"
#include "test/ctestapp.h"
#include "cscene.h"
#include <QGLWidget>
//class CVec2;

class SceneWidget : public QGLWidget
{
    Q_OBJECT

public:
    SceneWidget( QWidget *parent = 0 );
    virtual ~SceneWidget();

    /// converts coordinates from window space (0,0)..(mWidth, mHeight)
    /// to screen space (-0.5, -0.5)..(0.5, 0.5)
    drash::CVec2 WidgetSpaceToScreenSpace(const drash::CVec2& _from) const;

    /// converts coordinates from window space (0,0)..(mWidth, mHeight) to world space
    /// taking into account depth (distance from camera to required layer) and activated camera position
    drash::CVec2 WidgetSpaceToWorldSpace(const drash::CVec2& _from, float _depth) const;

    virtual void resizeGL( int _w, int _h );
    virtual void paintGL();

    void SetTestApp( drash::CTestApp *_app );

    virtual void mousePressEvent( QMouseEvent * _event );
    virtual void mouseMoveEvent(QMouseEvent *_event);
    virtual void keyReleaseEvent( QKeyEvent *_event );
    virtual void keyPressEvent( QKeyEvent *_event );
    virtual void wheelEvent( QWheelEvent *_event );

protected:
    constexpr static double mFov = 60.0;
    drash::CTestApp *mTestApp;
    float mWidth = 1.0f;
    float mHeight = 1.0f;
    drash::CVec2 mCursorPos;
public slots:
    void RemoveObjects();
};

#endif // SCENEWIDGET_H
