#include "scenewidget.h"

#include <GL/glu.h>

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
