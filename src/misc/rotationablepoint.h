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

#ifndef DRASH_ROTATIONABLEPOINT_H
#define DRASH_ROTATIONABLEPOINT_H

namespace greng {

    class Renderer;
    class Camera;
}

namespace drash {

    class RotationablePoint {
    public:
        RotationablePoint(greng::Renderer& _renderer, greng::Camera& _camera);

        void Step(double);
        void Render();

        inline void SetPoint(const Vec3f& _point);
        inline void SetRotation(const Vec3f& _rotation);

        inline void SetCursorPos(const Vec2f& _pos);
        void RotateBegin();
        void RotateEnd();

        float radius = 2;

        inline void SetAxisOX(const bool _val);
        inline void SetAxisOY(const bool _val);
        inline void SetAxisOZ(const bool _val);

        inline const Vec3f& GetRotation() const;

    private:
        Vec3f rotationDelta;
        Vec2f cursorPos = Vec2f(0);

        greng::Renderer& renderer;
        greng::Camera& camera;

        Vec3f point = Vec3f(0);
        Vec3f rotation = Vec3f(0);

        unsigned int axisOvered = 0;
        unsigned int axisRotating = 0;

        bool axisOX = true;
        bool axisOY = true;
        bool axisOZ = true;
    };

    inline void RotationablePoint::SetPoint(const Vec3f& _point) {
        point = _point;
    }

    inline void RotationablePoint::SetRotation(const Vec3f& _rotation) {
        rotation = _rotation;
    }

    inline void RotationablePoint::SetCursorPos(const Vec2f& _pos) {
        cursorPos = _pos;
    }

    inline void RotationablePoint::SetAxisOX(const bool _val) { axisOX = _val; }

    inline void RotationablePoint::SetAxisOY(const bool _val) { axisOY = _val; }

    inline void RotationablePoint::SetAxisOZ(const bool _val) { axisOZ = _val; }

    inline const Vec3f& RotationablePoint::GetRotation() const {
        return rotation;
    }

} // namespace drash

#endif // DRASH_ROTATIONABLEPOINT_H
