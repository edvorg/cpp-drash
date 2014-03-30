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

#ifndef MOVEABLEPOINT_H
#define MOVEABLEPOINT_H

#include "../greng/camera.h"
#include "../greng/renderer.h"

namespace drash {

    class MoveablePoint {
    public:
        MoveablePoint();
        MoveablePoint(Vec3f _point, greng::Camera* _camera);

        void SetCenter(const Vec3f& _center);
        void SetCamera(greng::Camera* _camera);
        inline Vec3f GetCenter() const;
        void Step(double);
        void Render(greng::Renderer& _render);

        void SetCursorPos(const Vec2f& _pos);

        void ClickBegin();
        void ClickPressing();
        void ClickEnd();

        inline void SetSize(float _size);

        inline void SetAxisOX(bool _val);
        inline void SetAxisOY(bool _val);
        inline void SetAxisOZ(bool _val);

    private:
        bool axisOX = true;
        bool axisOY = true;
        bool axisOZ = true;

        greng::Camera* currentCamera = nullptr;

        void Calculate();

        greng::Camera* GetCamera();

        Vec3f center;
        Vec3f x;
        Vec3f y;
        Vec3f z;

        Vec3f axisDrawK;
        unsigned int axisOver = 0;
        Vec3f firstClick;
        unsigned int axisMoving = 0;

        bool moving = false;

        Vec2f cursorPos;

        float lineSizeWorld = 1.0f;
        float lineSizeScreen = 0.05;
    };

    inline Vec3f MoveablePoint::GetCenter() const { return center; }

    inline void MoveablePoint::SetSize(float _size) {
        lineSizeScreen = math::Abs(_size);
    }

    inline void MoveablePoint::SetAxisOX(bool _val) { axisOX = _val; }

    inline void MoveablePoint::SetAxisOY(bool _val) { axisOY = _val; }

    inline void MoveablePoint::SetAxisOZ(bool _val) { axisOZ = _val; }

} // namespace drash
#endif // MOVEABLEPOINT_H
