// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2014 Edward Knyshov, Yuriy Shatilin.

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

#include <GL/glew.h>
#include <SDL/SDL_image.h>
#include "scenewidget.h"

#include <QMouseEvent>
#include <QDebug>

#include "../app/app.h"
#include "../app/appevent.h"

using namespace drash;

drash::EventKey ConvertKey(int _key) {
    switch (_key) {
    case Qt::Key_Q:
        return drash::EventKeyQ;
    case Qt::Key_W:
        return drash::EventKeyW;
    case Qt::Key_E:
        return drash::EventKeyE;
    case Qt::Key_R:
        return drash::EventKeyR;
    case Qt::Key_A:
        return drash::EventKeyA;
    case Qt::Key_S:
        return drash::EventKeyS;
    case Qt::Key_D:
        return drash::EventKeyD;
    case Qt::Key_F:
        return drash::EventKeyF;
    case Qt::Key_Z:
        return drash::EventKeyZ;
    case Qt::Key_X:
        return drash::EventKeyX;
    case Qt::Key_C:
        return drash::EventKeyC;
    case Qt::Key_V:
        return drash::EventKeyV;
    case Qt::Key_Space:
        return drash::EventKeySpace;
    case Qt::Key_Escape:
        return drash::EventKeyEscape;
    case Qt::Key_Control:
        return drash::EventKeyControl;
    case Qt::Key_Shift:
        return drash::EventKeyShift;
    case Qt::Key_Alt:
        return drash::EventKeyAlt;
    case Qt::Key_Meta:
        return drash::EventKeyMeta;
    case Qt::LeftButton:
        return drash::EventKeyLeftButton;
    case Qt::RightButton:
        return drash::EventKeyRightButton;
    case Qt::MiddleButton:
        return drash::EventKeyMiddleButton;
    default:
        return drash::EventKeyUnknown;
    }
}

SceneWidget::SceneWidget(QWidget* parent) : QGLWidget(parent) {
    setMouseTracking(true);
    setAcceptDrops(true);
}

SceneWidget::~SceneWidget() { IMG_Quit(); }

CVec2f SceneWidget::WidgetSpaceToScreenSpace(const CVec2f& _from) const {
    CVec2f res = _from;

    res.mX /= mWidth;
    res.mY /= mHeight;

    res.mX -= 0.5;
    res.mY -= 0.5;
    res.mY *= -1;

    return res;
}

void SceneWidget::initializeGL() {
    QGLWidget::initializeGL();

    int img_flags = IMG_INIT_PNG;

    if (IMG_Init(img_flags) != img_flags) {
        mApp = nullptr;
        return;
    }

    if (glewInit() != GLEW_OK) {
        mApp = nullptr;
        return;
    }

    if (mApp != nullptr) {
        if (mApp->Init() == false) {
            mApp = nullptr;
        }
    }
}

void SceneWidget::resizeGL(int _w, int _h) {
    QGLWidget::resizeGL(_w, _h);

    mWidth = _w ? _w : 1;
    mHeight = _h ? _h : 1;

    glViewport(0, 0, mWidth, mHeight);

    if (mApp != nullptr) {
        mApp->GetGrengSystems().GetCameraManager().SetAspectRatio(mWidth /
                                                                  mHeight);
        mApp->GetUISystem().SetAspectRatio(mWidth / mHeight);
    }
}

void SceneWidget::paintGL() {
    QGLWidget::paintGL();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mApp != nullptr) {
        mApp->Render();
    }
}

void SceneWidget::mousePressEvent(QMouseEvent* _event) {
    QGLWidget::mousePressEvent(_event);
    setFocus();
    if (mApp == nullptr) {
        return;
    }

    CVec2f pos = WidgetSpaceToScreenSpace(CVec2f(_event->x(), _event->y()));
    mApp->SetCursorPos(pos);
    int x = 0;
    int y = 0;
    mApp->GetUISystem().ScreenSpaceToUISpace(pos, x, y);
    mApp->GetUISystem().SetCursorPos(x, y);

    mApp->GetEventSystem().BeginEvent(CAppEvent(ConvertKey(_event->button())));
    mApp->GetUISystem().BeginEvent();
}

void SceneWidget::mouseReleaseEvent(QMouseEvent* _event) {
    QGLWidget::mouseReleaseEvent(_event);

    if (mApp == nullptr) {
        return;
    }

    mApp->GetEventSystem().EndEvent(CAppEvent(ConvertKey(_event->button())));
    mApp->GetUISystem().EndEvent();
}

void SceneWidget::mouseMoveEvent(QMouseEvent* _event) {
    QGLWidget::mouseMoveEvent(_event);

    if (mApp == nullptr) {
        return;
    }

    CVec2f pos = WidgetSpaceToScreenSpace(CVec2f(_event->x(), _event->y()));
    mApp->SetCursorPos(pos);
    int x = 0;
    int y = 0;
    mApp->GetUISystem().ScreenSpaceToUISpace(pos, x, y);
    mApp->GetUISystem().SetCursorPos(x, y);
}

void SceneWidget::keyPressEvent(QKeyEvent* _event) {
    QGLWidget::keyPressEvent(_event);

    if (mApp == nullptr) {
        return;
    }

    if (_event->isAutoRepeat() == false) {
        mApp->GetEventSystem().BeginEvent(CAppEvent(ConvertKey(_event->key())));
    }
}

void SceneWidget::keyReleaseEvent(QKeyEvent* _event) {
    QGLWidget::keyReleaseEvent(_event);

    if (mApp == nullptr) {
        return;
    }

    if (_event->isAutoRepeat() == false) {
        mApp->GetEventSystem().EndEvent(CAppEvent(ConvertKey(_event->key())));
    }
}

void SceneWidget::wheelEvent(QWheelEvent* _event) {
    QGLWidget::wheelEvent(_event);

    if (mApp == nullptr) {
        return;
    }

    mApp->GetEventSystem().BeginEvent(
        CAppEvent(_event->delta() > 0 ? EventKeyWheelUp : EventKeyWheelDown));
    mApp->GetEventSystem().EndEvent(
        CAppEvent(_event->delta() > 0 ? EventKeyWheelUp : EventKeyWheelDown));
}

void SceneWidget::dropEvent(QDropEvent* _event) {
    qDebug() << "Drop event";
    mApp->GetEventSystem().EndEvent(CAppEvent(EventDragDrop));
    _event->accept();
}

void SceneWidget::dragMoveEvent(QDragMoveEvent* _event) {
    _event->accept();
    //    QGLWidget::mouseMoveEvent(_event);

    if (mApp == nullptr) {
        return;
    }
    CVec2f pos =
        WidgetSpaceToScreenSpace(CVec2f(_event->pos().x(), _event->pos().y()));
    mApp->SetCursorPos(pos);
    int x = 0;
    int y = 0;
    mApp->GetUISystem().ScreenSpaceToUISpace(pos, x, y);
    mApp->GetUISystem().SetCursorPos(x, y);
}

void SceneWidget::dragEnterEvent(QDragEnterEvent* _event) {
    qDebug() << "Drag eneter event";

    mApp->GetEventSystem().BeginEvent(CAppEvent(EventDragDrop));

    _event->accept();
}

void SceneWidget::dragLeaveEvent(QDragLeaveEvent* _event) {
    qDebug() << "Drag leave event";

    mApp->GetEventSystem().BeginEvent(CAppEvent(EventDragLeave));
    mApp->GetEventSystem().EndEvent(CAppEvent(EventDragLeave));

    mApp->GetEventSystem().CancelEvent(CAppEvent(EventDragDrop));

    _event->ignore();
}
