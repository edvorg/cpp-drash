// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QGLWidget>
#include "../misc/vec2.h"

namespace drash
{
    class CApp;
}

class SceneWidget : public QGLWidget
{
    Q_OBJECT

public:
    SceneWidget( QWidget *parent = 0 );
    virtual ~SceneWidget();

    /// converts coordinates from window space (0,0)..(mWidth, mHeight)
    /// to screen space (-0.5, -0.5)..(0.5, 0.5)

    virtual void initializeGL() override;
    virtual void resizeGL( int _w, int _h ) override;
    virtual void paintGL() override;

    inline void SetApp( drash::CApp *_app );
    inline drash::CApp *GetApp();

protected:
    virtual void mousePressEvent( QMouseEvent * _event ) override;
    virtual void mouseReleaseEvent(QMouseEvent *_event) override;
    virtual void mouseMoveEvent(QMouseEvent *_event) override;
    virtual void keyPressEvent( QKeyEvent *_event ) override;
    virtual void keyReleaseEvent(QKeyEvent *_event) override;
    virtual void wheelEvent( QWheelEvent *_event ) override;

    drash::CApp *mApp = nullptr;
    float mWidth = 1.0f;
    float mHeight = 1.0f;

private:
    drash::CVec2f WidgetSpaceToScreenSpace(const drash::CVec2f& _from) const;
};

inline void SceneWidget::SetApp(drash::CApp *_app)
{
    mApp = _app;
}

inline drash::CApp *SceneWidget::GetApp()
{
    return mApp;
}

#endif // SCENEWIDGET_H
