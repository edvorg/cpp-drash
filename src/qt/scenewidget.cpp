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

#define GL_GLEXT_PROTOTYPES
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

CVec2f SceneWidget::WidgetSpaceToScreenSpace(const Vec2f& _from) const {
    Vec2f res = _from;

    res.x /= width;
    res.y /= height;

    res.x -= 0.5;
    res.y -= 0.5;
    res.y *= -1;

    return res;
}

void SceneWidget::initializeGL() {
    QGLWidget::initializeGL();

    int img_flags = IMG_INIT_PNG;

    if (IMG_Init(img_flags) != img_flags) {
        app = nullptr;
        return;
    }

    if (glewInit() != GLEW_OK) {
        app = nullptr;
        return;
    }

    if (app != nullptr) {
        if (app->Init() == false) {
            app = nullptr;
        }
    }
}

void SceneWidget::resizeGL(int _w, int _h) {
    QGLWidget::resizeGL(_w, _h);

    width = _w ? _w : 1;
    height = _h ? _h : 1;

    glViewport(0, 0, width, height);

    if (app != nullptr) {
        app->GetGreng().GetCameraManager().SetAspectRatio(width / height);
        app->GetUISystem().SetAspectRatio(width / height);
    }
}

void SceneWidget::paintGL() {
    QGLWidget::paintGL();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (app != nullptr) {
        app->Render();
    }
}

void SceneWidget::mousePressEvent(QMouseEvent* _event) {
    QGLWidget::mousePressEvent(_event);
    setFocus();
    if (app == nullptr) {
        return;
    }

    Vec2f pos = WidgetSpaceToScreenSpace(Vec2f(_event->x(), _event->y()));
    app->SetCursorPos(pos);
    int x = 0;
    int y = 0;
    app->GetUISystem().ScreenSpaceToUISpace(pos, x, y);
    app->GetUISystem().SetCursorPos(x, y);

    app->GetEventSystem().BeginEvent(AppEvent(ConvertKey(_event->button())));
    app->GetUISystem().BeginEvent();
}

void SceneWidget::mouseReleaseEvent(QMouseEvent* _event) {
    QGLWidget::mouseReleaseEvent(_event);

    if (app == nullptr) {
        return;
    }

    app->GetEventSystem().EndEvent(AppEvent(ConvertKey(_event->button())));
    app->GetUISystem().EndEvent();
}

void SceneWidget::mouseMoveEvent(QMouseEvent* _event) {
    QGLWidget::mouseMoveEvent(_event);

    if (app == nullptr) {
        return;
    }

    Vec2f pos = WidgetSpaceToScreenSpace(Vec2f(_event->x(), _event->y()));
    app->SetCursorPos(pos);
    int x = 0;
    int y = 0;
    app->GetUISystem().ScreenSpaceToUISpace(pos, x, y);
    app->GetUISystem().SetCursorPos(x, y);
}

void SceneWidget::keyPressEvent(QKeyEvent* _event) {
    QGLWidget::keyPressEvent(_event);

    if (app == nullptr) {
        return;
    }

    if (_event->isAutoRepeat() == false) {
        app->GetEventSystem().BeginEvent(AppEvent(ConvertKey(_event->key())));
    }
}

void SceneWidget::keyReleaseEvent(QKeyEvent* _event) {
    QGLWidget::keyReleaseEvent(_event);

    if (app == nullptr) {
        return;
    }

    if (_event->isAutoRepeat() == false) {
        app->GetEventSystem().EndEvent(AppEvent(ConvertKey(_event->key())));
    }
}

void SceneWidget::wheelEvent(QWheelEvent* _event) {
    QGLWidget::wheelEvent(_event);

    if (app == nullptr) {
        return;
    }

    app->GetEventSystem().BeginEvent(
        AppEvent(_event->delta() > 0 ? EventKeyWheelUp : EventKeyWheelDown));
    app->GetEventSystem().EndEvent(
        AppEvent(_event->delta() > 0 ? EventKeyWheelUp : EventKeyWheelDown));
}

void SceneWidget::dropEvent(QDropEvent* _event) {
    qDebug() << "Drop event";
    app->GetEventSystem().EndEvent(AppEvent(EventDragDrop));
    _event->accept();
}

void SceneWidget::dragMoveEvent(QDragMoveEvent* _event) {
    _event->accept();
    //    QGLWidget::mouseMoveEvent(_event);

    if (app == nullptr) {
        return;
    }
    Vec2f pos =
        WidgetSpaceToScreenSpace(Vec2f(_event->pos().x(), _event->pos().y()));
    app->SetCursorPos(pos);
    int x = 0;
    int y = 0;
    app->GetUISystem().ScreenSpaceToUISpace(pos, x, y);
    app->GetUISystem().SetCursorPos(x, y);
}

void SceneWidget::dragEnterEvent(QDragEnterEvent* _event) {
    qDebug() << "Drag eneter event";

    app->GetEventSystem().BeginEvent(AppEvent(EventDragDrop));

    _event->accept();
}

void SceneWidget::dragLeaveEvent(QDragLeaveEvent* _event) {
    qDebug() << "Drag leave event";

    app->GetEventSystem().BeginEvent(AppEvent(EventDragLeave));
    app->GetEventSystem().EndEvent(AppEvent(EventDragLeave));

    app->GetEventSystem().CancelEvent(AppEvent(EventDragDrop));

    _event->ignore();
}
