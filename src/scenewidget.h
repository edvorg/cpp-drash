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

//    virtual void initializeGL();
    virtual void resizeGL( int _w, int _h );
    virtual void paintGL();

//    virtual void initializeOverlayGL();
//    virtual void resizeOverlayGL(int w, int h);
//    virtual void paintOverlayGL();

//    virtual void glInit();
//    virtual void glDraw();

    void SetTestApp( drash::CTestApp *_app );

    virtual void mousePressEvent( QMouseEvent * _event );
    virtual void keyReleaseEvent( QKeyEvent *_event );
    virtual void keyPressEvent( QKeyEvent *_event );
    virtual void wheelEvent( QWheelEvent *_event );

private:
    drash::CTestApp *mTestApp;
    float mWidth;
    float mHeight;
};

#endif // SCENEWIDGET_H
